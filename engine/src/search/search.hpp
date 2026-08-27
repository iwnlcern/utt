#pragma once

#include "eval/eval.hpp"
#include "search/backup.hpp"
#include "search/game_model.hpp"
#include "search/tt.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <type_traits>
#include <vector>

namespace uttt {

enum class Quality : uint8_t { Exact, Bound, Estimate };

struct Limits {
  int max_depth = 0;
  uint64_t node_cap = std::numeric_limits<uint64_t>::max();
  bool use_tt = false;
  bool widen_free_choice = true;
  std::size_t widening_k = 12;
  // Cancellation is checked once before search and then before every Nth
  // node. A zero cadence is normalized to one.
  uint64_t stop_check_nodes = 256;
  std::function<bool()> stop_requested{};
};

struct Window {
  TInterval w{0.0, 1.0};
  double eps_node = 0.0;
};

struct CutCounters {
  uint64_t min_dominance = 0;
  uint64_t max_dominance = 0;
  uint64_t window_lo = 0;
  uint64_t window_hi = 0;
  uint64_t precision = 0;
  uint64_t hull_blocked = 0;
};

struct SearchResult {
  TInterval t{0.0, 1.0};
  uint8_t best_x = std::numeric_limits<uint8_t>::max();
  uint8_t best_o = std::numeric_limits<uint8_t>::max();
  Quality quality = Quality::Estimate;
  uint8_t depth = 0;
  bool complete = false;
  bool hull = false;
  CutCounters cuts{};
};

template <GameModel M> struct Search {
  using State = typename M::State;

  explicit Search(uint8_t tt_entries_log2 = TT::kDefaultEntriesLog2,
                  TT::Mode tt_mode = TT::Mode::Play)
      : tt_entries_log2_(tt_entries_log2), tt_mode_(tt_mode) {}

  const CollisionStats *tt_stats() const { return tt_ ? &tt_->stats : nullptr; }
  uint64_t evaluator_calls() const { return evaluator_calls_; }
  bool was_cancelled() const { return cancelled_; }

  SearchResult solve(State state, Tie h, Limits limits,
                     Window window = {{0.0, 1.0}, 0.0}) {
    assert(0.0 <= window.w.lo && window.w.lo <= window.w.hi &&
           window.w.hi <= 1.0);
    assert(window.eps_node >= 0.0);
    nodes_ = 0;
    evaluator_calls_ = 0;
    cuts_ = {};
    const int horizon = std::max(limits.max_depth, 0);
    node_cap_ = limits.node_cap;
    cuts_enabled_ = window.eps_node > 0.0 || window.w.lo > 0.0 ||
                    window.w.hi < 1.0;
    tt_enabled_ = limits.use_tt;
    widen_free_choice_ = limits.widen_free_choice;
    widening_k_ = std::max<std::size_t>(limits.widening_k, 6);
    stop_check_nodes_ = std::max<uint64_t>(limits.stop_check_nodes, 1);
    stop_requested_ = std::move(limits.stop_requested);
    cancelled_ = false;
    if (poll_stop())
      return incomplete_result();
    if (tt_enabled_) {
      if (!tt_)
        tt_.emplace(tt_entries_log2_, tt_mode_);
      ++generation_;
    }

    NodeResult result;
    // TieState::NullFirstMove is the repository's existing representation of
    // F-C6's null root. It is a request for the two-conditional envelope, not
    // a claim about the simultaneous hidden-coin game.
    if (h == Tie::NullFirstMove) {
      const NodeResult conditional_x =
          dfs(state, Tie::X, horizon, window, false);
      if (!conditional_x.complete)
        return incomplete_result();
      const NodeResult conditional_o =
          dfs(state, Tie::O, horizon, window, false);
      if (!conditional_o.complete)
        return incomplete_result();
      result = conditional_x;
      result.t = {
          std::min(conditional_x.t.lo, conditional_o.t.lo),
          std::max(conditional_x.t.hi, conditional_o.t.hi),
      };
      result.quality =
          combine_quality(conditional_x.quality, conditional_o.quality);
      result.hull = false;
    } else {
      assert(h == Tie::X || h == Tie::O);
      result = dfs(state, h, horizon, window, false);
    }

    if (!result.complete)
      return incomplete_result();
    return {
        result.t,
        result.best_x,
        result.best_o,
        result.quality,
        static_cast<uint8_t>(std::min(horizon, 255)),
        true,
        result.hull,
        cuts_,
    };
  }

private:
  struct NodeResult {
    TInterval t{0.0, 1.0};
    uint8_t best_x = std::numeric_limits<uint8_t>::max();
    uint8_t best_o = std::numeric_limits<uint8_t>::max();
    Quality quality = Quality::Estimate;
    bool complete = false;
    bool hull = false;
    double guide = 0.5;
  };

  uint64_t nodes_ = 0;
  uint64_t evaluator_calls_ = 0;
  uint64_t node_cap_ = 0;
  CutCounters cuts_{};
  bool cuts_enabled_ = false;
  uint8_t tt_entries_log2_;
  TT::Mode tt_mode_;
  std::optional<TT> tt_;
  uint8_t generation_ = 0;
  bool tt_enabled_ = false;
  bool widen_free_choice_ = true;
  std::size_t widening_k_ = 12;
  uint64_t stop_check_nodes_ = 256;
  std::function<bool()> stop_requested_{};
  bool cancelled_ = false;

  enum class WindowSide : uint8_t { None, Low, High };

  bool poll_stop() {
    if (cancelled_)
      return true;
    if (stop_requested_ && stop_requested_()) {
      cancelled_ = true;
      return true;
    }
    return false;
  }

  struct IntervalIntersection {
    TInterval value{};
    bool empty = true;
  };

  static Quality combine_quality(Quality lhs, Quality rhs) {
    if (lhs == Quality::Estimate || rhs == Quality::Estimate) {
      return Quality::Estimate;
    }
    if (lhs == Quality::Bound || rhs == Quality::Bound)
      return Quality::Bound;
    return Quality::Exact;
  }

  SearchResult incomplete_result() const {
    return {
        {0.0, 1.0},
        std::numeric_limits<uint8_t>::max(),
        std::numeric_limits<uint8_t>::max(),
        Quality::Estimate,
        0,
        false,
        false,
        cuts_,
    };
  }

  static WindowSide side_of(TInterval interval, TInterval window) {
    if (interval.hi < window.lo)
      return WindowSide::Low;
    if (interval.lo > window.hi)
      return WindowSide::High;
    return WindowSide::None;
  }

  static IntervalIntersection intersect(TInterval lhs, TInterval rhs) {
    const TInterval value{std::max(lhs.lo, rhs.lo),
                          std::min(lhs.hi, rhs.hi)};
    return {value, value.lo > value.hi};
  }

  static std::optional<TInterval>
  unite(IntervalIntersection lhs, IntervalIntersection rhs) {
    if (lhs.empty && rhs.empty)
      return std::nullopt;
    if (lhs.empty)
      return rhs.value;
    if (rhs.empty)
      return lhs.value;
    return TInterval{std::min(lhs.value.lo, rhs.value.lo),
                     std::max(lhs.value.hi, rhs.value.hi)};
  }

  static TInterval reachable_a(const Aggregates &aggregates,
                               bool has_unvisited) {
    if (!aggregates.has_x)
      return {0.0, 1.0};
    if (has_unvisited)
      return {0.0, aggregates.a.hi};
    return aggregates.a;
  }

  static TInterval reachable_b(const Aggregates &aggregates,
                               bool has_unvisited) {
    if (!aggregates.has_o)
      return {0.0, 1.0};
    if (has_unvisited)
      return {aggregates.b.lo, 1.0};
    return aggregates.b;
  }

  static bool is_hull(const Aggregates &aggregates) {
    assert(aggregates.has_x && aggregates.has_o);
    return aggregates.a.hi > aggregates.b.lo &&
           aggregates.a.lo <= aggregates.b.hi;
  }

  std::optional<Window> x_child_window(const Aggregates &aggregates, Tie h,
                                       Window parent) const {
    const double domain_hi =
        aggregates.has_x ? aggregates.a.hi : 1.0;
    const TInterval b = aggregates.has_o ? aggregates.b : TInterval{0.0, 1.0};
    const IntervalIntersection ordered = intersect(
        x_preimage(parent.w, b), {0.0, std::min(domain_hi, b.hi)});
    const IntervalIntersection zugzwang =
        h == Tie::X ? intersect(parent.w, {0.0, domain_hi})
                    : IntervalIntersection{{0.0, domain_hi}, false};
    const auto combined = unite(ordered, zugzwang);
    if (!combined)
      return std::nullopt;
    return Window{*combined, parent.eps_node};
  }

  std::optional<Window> o_child_window(const Aggregates &aggregates, Tie h,
                                       Window parent,
                                       bool has_unvisited_x) const {
    const double domain_lo =
        aggregates.has_o ? aggregates.b.lo : 0.0;
    const TInterval a = reachable_a(aggregates, has_unvisited_x);
    const IntervalIntersection ordered = intersect(
        o_preimage(parent.w, a), {std::max(domain_lo, a.lo), 1.0});
    const IntervalIntersection zugzwang =
        h == Tie::O ? intersect(parent.w, {domain_lo, 1.0})
                    : IntervalIntersection{{domain_lo, 1.0}, false};
    const auto combined = unite(ordered, zugzwang);
    if (!combined)
      return std::nullopt;
    return Window{*combined, parent.eps_node};
  }

  static Quality as_bound(Quality quality) {
    return quality == Quality::Estimate ? Quality::Estimate : Quality::Bound;
  }

  template <class Child>
  NodeResult refine_child(const Child &child, Tie tie, int remaining_depth,
                          TInterval bound) {
    const bool restore_cuts = cuts_enabled_;
    cuts_enabled_ = false;
    const NodeResult refined = dfs(child.state, tie, remaining_depth - 1,
                                   Window{{0.0, 1.0}, 0.0}, true);
    cuts_enabled_ = restore_cuts;
    if (!refined.complete || refined.t.lo < bound.lo ||
        refined.t.hi > bound.hi)
      return {};
    return refined;
  }

  template <class Children>
  std::optional<uint8_t> refine_best_x(const Children &children,
                                       const std::vector<TInterval> &bounds,
                                       int remaining_depth) {
    assert(!children.empty() && children.size() == bounds.size());
    std::size_t seed = 0;
    for (std::size_t index = 1; index < children.size(); ++index) {
      if (bounds[index].hi < bounds[seed].hi ||
          (bounds[index].hi == bounds[seed].hi &&
           children[index].move < children[seed].move))
        seed = index;
    }

    const NodeResult first =
        refine_child(children[seed], Tie::O, remaining_depth, bounds[seed]);
    if (!first.complete)
      return std::nullopt;
    double best_hi = first.t.hi;
    uint8_t best_move = children[seed].move;

    for (std::size_t index = 0; index < children.size(); ++index) {
      if (index == seed || bounds[index].lo > best_hi)
        continue;
      const NodeResult refined = refine_child(children[index], Tie::O,
                                              remaining_depth, bounds[index]);
      if (!refined.complete)
        return std::nullopt;
      if (refined.t.hi < best_hi ||
          (refined.t.hi == best_hi && children[index].move < best_move)) {
        best_hi = refined.t.hi;
        best_move = children[index].move;
      }
    }
    return best_move;
  }

  template <class Children>
  std::optional<uint8_t> refine_best_o(const Children &children,
                                       const std::vector<TInterval> &bounds,
                                       int remaining_depth) {
    assert(!children.empty() && children.size() == bounds.size());
    std::size_t seed = 0;
    for (std::size_t index = 1; index < children.size(); ++index) {
      if (bounds[index].lo > bounds[seed].lo ||
          (bounds[index].lo == bounds[seed].lo &&
           children[index].move > children[seed].move))
        seed = index;
    }

    const NodeResult first =
        refine_child(children[seed], Tie::X, remaining_depth, bounds[seed]);
    if (!first.complete)
      return std::nullopt;
    double best_lo = first.t.lo;
    uint8_t best_move = children[seed].move;

    for (std::size_t index = 0; index < children.size(); ++index) {
      if (index == seed || bounds[index].hi < best_lo)
        continue;
      const NodeResult refined = refine_child(children[index], Tie::X,
                                              remaining_depth, bounds[index]);
      if (!refined.complete)
        return std::nullopt;
      if (refined.t.lo > best_lo ||
          (refined.t.lo == best_lo && children[index].move > best_move)) {
        best_lo = refined.t.lo;
        best_move = children[index].move;
      }
    }
    return best_move;
  }

  std::optional<NodeResult>
  cutoff_result(const Aggregates &aggregates, Tie h, bool unvisited_x,
                bool unvisited_o, Quality quality, uint8_t best_x,
                uint8_t best_o, Window window, bool allow_cut) {
    if (!allow_cut || !aggregates.has_x || !aggregates.has_o)
      return std::nullopt;

    const TInterval a = reachable_a(aggregates, unvisited_x);
    const TInterval b = reachable_b(aggregates, unvisited_o);
    TInterval reachable{};
    WindowSide side = WindowSide::None;
    bool hull = false;
    if (a.hi <= b.lo) {
      reachable = f_backup(a, b);
      side = side_of(reachable, window.w);
    } else if (a.lo > b.hi) {
      reachable = h == Tie::X ? a : b;
      side = side_of(reachable, window.w);
    } else {
      hull = true;
      const TInterval ordered = f_backup(a, b);
      const TInterval zugzwang = h == Tie::X ? a : b;
      const WindowSide ordered_side = side_of(ordered, window.w);
      const WindowSide zugzwang_side = side_of(zugzwang, window.w);
      reachable = {std::min(ordered.lo, zugzwang.lo),
                   std::max(ordered.hi, zugzwang.hi)};
      if (ordered_side != WindowSide::None &&
          ordered_side == zugzwang_side) {
        side = ordered_side;
      } else if (ordered_side != WindowSide::None ||
                 zugzwang_side != WindowSide::None) {
        ++cuts_.hull_blocked;
      }
    }

    if (side == WindowSide::Low) {
      ++cuts_.window_lo;
      return NodeResult{reachable, best_x, best_o,
                        unvisited_x || unvisited_o ? as_bound(quality) : quality,
                        true, hull};
    }
    if (side == WindowSide::High) {
      ++cuts_.window_hi;
      return NodeResult{reachable, best_x, best_o,
                        unvisited_x || unvisited_o ? as_bound(quality) : quality,
                        true, hull};
    }
    if ((unvisited_x || unvisited_o) && window.eps_node > 0.0 &&
        width(reachable) <= window.eps_node) {
      ++cuts_.precision;
      return NodeResult{reachable, best_x, best_o, as_bound(quality), true,
                        hull};
    }
    return std::nullopt;
  }

  static uint8_t encode_quality(Quality quality) {
    switch (quality) {
    case Quality::Exact:
      return kTTQualityExact;
    case Quality::Bound:
      return kTTQualityBound;
    case Quality::Estimate:
      return kTTQualityEstimate;
    }
    return kTTQualityEstimate;
  }

  static Quality decode_quality(uint8_t flags) {
    switch (flags & kTTQualityMask) {
    case kTTQualityExact:
      return Quality::Exact;
    case kTTQualityBound:
      return Quality::Bound;
    default:
      return Quality::Estimate;
    }
  }

  static bool interval_satisfies(TInterval interval, Window window) {
    if (window.eps_node > 0.0 && width(interval) <= window.eps_node)
      return true;
    return side_of(interval, window.w) != WindowSide::None;
  }

  static bool identity_carries_tie(const PosId &id, Tie h) {
    return id.tie == h;
  }

  bool usable(const TTEntry &entry, int remaining_depth, Window window) const {
    const Quality quality = decode_quality(entry.flags);
    if (quality == Quality::Estimate || (entry.flags & kTTComplete) == 0)
      return false;
    return entry.depth >= remaining_depth ||
           interval_satisfies({entry.lo, entry.hi}, window);
  }

  static bool is_free_choice(const State &state) {
    if constexpr (requires { M::free_choice(state); }) {
      return M::free_choice(state);
    } else if constexpr (std::same_as<State, Position>) {
      return state.forced == kForcedAny;
    } else {
      return false;
    }
  }

  template <class Children>
  Children scheduled_children(Children children, const State &state,
                              Seat mover) {
    if (!widen_free_choice_ || !is_free_choice(state)) return children;
    if constexpr (std::same_as<State, Position>) {
      std::stable_sort(children.begin(), children.end(),
                       [&](const auto &lhs, const auto &rhs) {
        const auto left = tactical_order_key(state, mover, lhs.move);
        const auto right = tactical_order_key(state, mover, rhs.move);
        if (left != right) return left > right;
        ++evaluator_calls_;
        const double left_eval = eval_estimate(lhs.state);
        ++evaluator_calls_;
        const double right_eval = eval_estimate(rhs.state);
        if (left_eval != right_eval)
          return mover == Seat::X ? left_eval < right_eval
                                  : left_eval > right_eval;
        return lhs.move < rhs.move;
      });
    }
    if (children.size() > widening_k_) children.resize(widening_k_);
    return children;
  }

  static double guide_backup(double a, double b, Tie h) {
    if (a <= b) return b / (1.0 - a + b);
    return h == Tie::X ? a : b;
  }

  NodeResult dfs(const State &state, Tie h, int remaining_depth, Window window,
                 bool allow_cut) {
    if (cancelled_ ||
        (nodes_ != 0 && nodes_ % stop_check_nodes_ == 0 && poll_stop()))
      return {};
    if (nodes_ >= node_cap_)
      return {};
    ++nodes_;

    const PosId id = M::pos_id(state);
    const bool cacheable = tt_enabled_ && identity_carries_tie(id, h);
    if (cacheable) {
      const auto hit = tt_->probe(M::tt_key(state), id);
      if (hit && usable(*hit, remaining_depth, window)) {
        return {{hit->lo, hit->hi},
                hit->move_x,
                hit->move_o,
                decode_quality(hit->flags),
                true,
                (hit->flags & kTTHull) != 0};
      }
    }

    const NodeResult result =
        search_node(state, h, remaining_depth, window, allow_cut);
    if (cacheable && result.complete) {
      TTEntry entry{};
      entry.lo = result.t.lo;
      entry.hi = result.t.hi;
      entry.move_x = result.best_x;
      entry.move_o = result.best_o;
      entry.depth = static_cast<uint8_t>(std::min(remaining_depth, 255));
      entry.gen = generation_;
      entry.flags = encode_quality(result.quality);
      if (result.quality == Quality::Exact)
        entry.flags |= kTTComplete;
      if (result.hull)
        entry.flags |= kTTHull;
      tt_->store(M::tt_key(state), id, entry);
    }
    return result;
  }

  NodeResult search_node(const State &state, Tie h, int remaining_depth,
                         Window window, bool allow_cut) {
    switch (M::terminal(state)) {
    case TerminalKind::MacroWinX:
      return {{0.0, 0.0}, no_move(), no_move(), Quality::Exact, true, false, 0.0};
    case TerminalKind::MacroWinO:
      return {{1.0, 1.0}, no_move(), no_move(), Quality::Exact, true, false, 1.0};
    case TerminalKind::AllClosed:
      return {{0.5, 0.5}, no_move(), no_move(), Quality::Exact, true, false, 0.5};
    case TerminalKind::None:
      break;
    }

    if (remaining_depth == 0) {
      double guide = 0.5;
      if constexpr (std::same_as<State, Position>) {
        ++evaluator_calls_;
        guide = eval_estimate(state);
      }
      return {{0.0, 1.0}, no_move(), no_move(), Quality::Estimate, true,
              false, guide};
    }

    const auto all_x_children = M::children_x(state);
    const auto all_o_children = M::children_o(state);
    const auto x_children = scheduled_children(all_x_children, state, Seat::X);
    const auto o_children = scheduled_children(all_o_children, state, Seat::O);
    assert(!x_children.empty() && !o_children.empty());
    const bool widened_x = x_children.size() < all_x_children.size();
    const bool widened_o = o_children.size() < all_o_children.size();

    Aggregates aggregates{};
    Quality quality = Quality::Exact;
    uint8_t best_x = no_move();
    double best_x_hi = std::numeric_limits<double>::infinity();
    bool unknown_x = widened_x;
    double guide_a = 1.0;
    std::vector<TInterval> x_bounds;
    if (cuts_enabled_ && !allow_cut)
      x_bounds.reserve(x_children.size());
    for (std::size_t index = 0; index < x_children.size(); ++index) {
      const auto &child = x_children[index];
      const auto child_window = cuts_enabled_
                                    ? x_child_window(aggregates, h, window)
                                    : std::optional<Window>{window};
      if (!child_window && allow_cut) {
        unknown_x = true;
        continue;
      }
      const NodeResult searched = dfs(
          child.state, Tie::O, remaining_depth - 1,
          child_window.value_or(Window{{0.0, 1.0}, window.eps_node}), true);
      if (!searched.complete)
        return {};
      if (cuts_enabled_ && !allow_cut)
        x_bounds.push_back(searched.t);
      quality = combine_quality(quality, searched.quality);
      guide_a = std::min(guide_a, searched.guide);
      if (searched.t.hi < best_x_hi ||
          (searched.t.hi == best_x_hi && child.move < best_x)) {
        best_x_hi = searched.t.hi;
        best_x = child.move;
      }
      if (cuts_enabled_ && aggregates.has_x &&
          searched.t.lo >= aggregates.a.hi) {
        ++cuts_.min_dominance;
      } else {
        fold_x(aggregates, searched.t);
      }
      const bool unvisited_x = unknown_x || index + 1 < x_children.size();
      if (const auto cut = cutoff_result(
              aggregates, h, unvisited_x, true, quality, best_x, no_move(),
              window, allow_cut))
        return *cut;
    }

    uint8_t best_o = no_move();
    double best_o_lo = -std::numeric_limits<double>::infinity();
    bool unknown_o = widened_o;
    double guide_b = 0.0;
    std::vector<TInterval> o_bounds;
    if (cuts_enabled_ && !allow_cut)
      o_bounds.reserve(o_children.size());
    for (std::size_t index = 0; index < o_children.size(); ++index) {
      const auto &child = o_children[index];
      const auto child_window = cuts_enabled_
                                    ? o_child_window(aggregates, h, window,
                                                     unknown_x)
                                    : std::optional<Window>{window};
      if (!child_window && allow_cut) {
        unknown_o = true;
        continue;
      }
      const NodeResult searched = dfs(
          child.state, Tie::X, remaining_depth - 1,
          child_window.value_or(Window{{0.0, 1.0}, window.eps_node}), true);
      if (!searched.complete)
        return {};
      if (cuts_enabled_ && !allow_cut)
        o_bounds.push_back(searched.t);
      quality = combine_quality(quality, searched.quality);
      guide_b = std::max(guide_b, searched.guide);
      if (searched.t.lo > best_o_lo ||
          (searched.t.lo == best_o_lo && child.move > best_o)) {
        best_o_lo = searched.t.lo;
        best_o = child.move;
      }
      if (cuts_enabled_ && aggregates.has_o &&
          searched.t.hi <= aggregates.b.lo) {
        ++cuts_.max_dominance;
      } else {
        fold_o(aggregates, searched.t);
      }
      const bool unvisited_o = unknown_o || index + 1 < o_children.size();
      if (const auto cut = cutoff_result(
              aggregates, h, unknown_x, unvisited_o, quality, best_x, best_o,
              window, allow_cut))
        return *cut;
    }

    if (cuts_enabled_ && !allow_cut) {
      const auto refined_x =
          refine_best_x(x_children, x_bounds, remaining_depth);
      if (!refined_x)
        return {};
      const auto refined_o =
          refine_best_o(o_children, o_bounds, remaining_depth);
      if (!refined_o)
        return {};
      best_x = *refined_x;
      best_o = *refined_o;
    }

    if (unknown_x || unknown_o) {
      const TInterval a = reachable_a(aggregates, unknown_x);
      const TInterval b = reachable_b(aggregates, unknown_o);
      Aggregates reachable{a, b, true, true};
      return {backup_node(reachable, h), best_x, best_o, as_bound(quality),
              true, is_hull(reachable), guide_backup(guide_a, guide_b, h)};
    }

    return {
        backup_node(aggregates, h), best_x, best_o, quality, true,
        is_hull(aggregates), guide_backup(guide_a, guide_b, h),
    };
  }

  static constexpr uint8_t no_move() {
    return std::numeric_limits<uint8_t>::max();
  }
};

extern template struct Search<UtttModel>;

} // namespace uttt
