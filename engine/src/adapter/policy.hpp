#pragma once

#include "adapter/wire.hpp"
#include "core/clock.hpp"
#include "root/alt_solver.hpp"
#include "root/bid_matrix.hpp"
#include "root/rmplus.hpp"
#include "search/backup.hpp"
#include "search/search.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace uttt {

struct Policy {
  virtual wire::TurnReply choose(const wire::TurnRequest &, Clock &) = 0;
  virtual ~Policy() = default;
};

struct PolicyDiagnostics {
  bool p2_checked = false;
  RootClass root_class = InBand;
  bool root_search_cancelled = false;
  bool child_search_cancelled = false;
  bool matrix_bypassed = false;
  bool matrix_constructed = false;
  bool matrix_complete = false;
  bool matrix_solved = false;
  bool matrix_action_published = false;
  uint64_t matrix_entries = 0;
  int rm_iterations = 0;
  bool certificate_attempted = false;
  bool certificate_published = false;
  uint64_t alternation_searches = 0;
  std::size_t alternation_memo_entries = 0;
  CollisionStats threshold_tt_before_alt{};
  CollisionStats threshold_tt_after_alt{};
  CutCounters root_cuts{};
};

namespace policy_detail {

inline uint8_t flat(Move move) {
  return static_cast<uint8_t>(9 * move.board + move.cell);
}

inline Move unflatten(uint8_t move) {
  return {static_cast<uint8_t>(move / 9), static_cast<uint8_t>(move % 9)};
}

inline const char *quality_name(Quality quality) {
  switch (quality) {
  case Quality::Exact:
    return "exact";
  case Quality::Bound:
    return "bound";
  case Quality::Estimate:
    return "estimate";
  }
  return "estimate";
}

inline uint8_t legal_preferred(const wire::TurnRequest &request,
                               uint8_t preferred) {
  const auto found =
      std::find_if(request.legal.begin(), request.legal.end(),
                   [preferred](Move move) { return flat(move) == preferred; });
  return found == request.legal.end() ? flat(request.legal.front()) : preferred;
}

struct Decision {
  int64_t bid = 0;
  uint8_t move = 0;
  SearchResult searched{};
};

struct CertifiedChild {
  uint8_t move = 0;
  TInterval t{0.0, 1.0};
  Quality quality = Quality::Estimate;
  bool complete = false;
};

inline bool sound_complete(const CertifiedChild &child) {
  return child.complete && child.quality != Quality::Estimate;
}

inline std::optional<uint8_t>
dominant_move(const std::vector<CertifiedChild> &children, Seat seat) {
  std::optional<uint8_t> selected;
  for (const CertifiedChild &candidate : children) {
    if (!sound_complete(candidate))
      return std::nullopt;
    bool dominates = true;
    for (const CertifiedChild &other : children) {
      if (&candidate == &other)
        continue;
      if (seat == Seat::X ? candidate.t.hi > other.t.lo
                          : candidate.t.lo < other.t.hi) {
        dominates = false;
        break;
      }
    }
    if (dominates && (!selected || candidate.move < *selected))
      selected = candidate.move;
  }
  return selected;
}

inline std::optional<RootAction> certified_forced_action(
    RootClass root_class, Seat seat, Tie tie, uint64_t total,
    uint64_t own_stack, Aggregates aggregates,
    const std::vector<CertifiedChild> &x_children,
    const std::vector<CertifiedChild> &o_children, bool aggregates_complete) {
  const bool own_forced = (root_class == XForced && seat == Seat::X) ||
                          (root_class == OForced && seat == Seat::O);
  if (!own_forced || !aggregates_complete || !aggregates.has_x ||
      !aggregates.has_o || x_children.empty() || o_children.empty())
    return std::nullopt;

  if (aggregates.a.hi <= aggregates.b.lo) {
    const auto move =
        dominant_move(seat == Seat::X ? x_children : o_children, seat);
    if (!move)
      return std::nullopt;
    const TInterval r = critical_r_enclosure(aggregates.a, aggregates.b);
    const uint64_t low_ceiling = ceil_exact(r.lo, total);
    const uint64_t high_ceiling = ceil_exact(r.hi, total);
    if (low_ceiling != high_ceiling || high_ceiling > own_stack)
      return std::nullopt;
    return RootAction{static_cast<int64_t>(high_ceiling), *move};
  }

  if (aggregates.a.lo <= aggregates.b.hi || (tie != Tie::X && tie != Tie::O))
    return std::nullopt;
  const Seat tie_seat = tie == Tie::X ? Seat::X : Seat::O;
  const auto move =
      dominant_move(tie_seat == Seat::X ? x_children : o_children, tie_seat);
  if (!move)
    return std::nullopt;
  return RootAction{0, *move};
}

} // namespace policy_detail

struct EnginePolicy final : Policy {
  const PolicyDiagnostics &last_diagnostics() const { return diagnostics_; }

  wire::TurnReply choose(const wire::TurnRequest &request,
                         Clock &clock) override {
    assert(!request.legal.empty());
    diagnostics_ = {};
    const int64_t start = clock.now_ms();
    const RequestDeadlines deadlines =
        request_deadlines(start, request.time_ms);
    const uint8_t first = policy_detail::flat(request.legal.front());
    SearchResult fallback{{0.0, 1.0},        first, first,
                          Quality::Estimate, 0,     false};
    policy_detail::Decision decision{0, first, fallback};
    Search<UtttModel> root_search(16);
    const int64_t total = request.ctx.budget_x + request.ctx.budget_o;
    const Window play_window = make_play_window(
        request.ctx.budget_x, total, UtttModel::empties(request.pos));
    const SearchResult published = run_root_stages<SearchResult>(
        fallback, 1, 4, clock, deadlines,
        [&](int depth, int64_t hard) -> std::optional<SearchResult> {
          const SearchResult result = root_search.solve(
              request.pos, request.pos.tie,
              Limits{depth, 25'000, true, true, 12, 64,
                     [&] { return clock.now_ms() >= deadlines.search_stop; }},
              play_window);
          diagnostics_.root_cuts.min_dominance += result.cuts.min_dominance;
          diagnostics_.root_cuts.max_dominance += result.cuts.max_dominance;
          diagnostics_.root_cuts.window_lo += result.cuts.window_lo;
          diagnostics_.root_cuts.window_hi += result.cuts.window_hi;
          diagnostics_.root_cuts.precision += result.cuts.precision;
          diagnostics_.root_cuts.hull_blocked += result.cuts.hull_blocked;
          diagnostics_.root_search_cancelled =
              diagnostics_.root_search_cancelled || root_search.was_cancelled();
          if (!result.complete || clock.now_ms() >= hard)
            return std::nullopt;
          return result;
        },
        [&](SearchResult staged, int64_t hard) {
          decision.searched = staged;
          const int64_t bx = request.ctx.budget_x;
          const int64_t bo = request.ctx.budget_o;
          const uint8_t preferred =
              request.ctx.seat == Seat::X ? staged.best_x : staged.best_o;
          decision.bid = 0;
          decision.move = policy_detail::legal_preferred(request, preferred);
          const RootClass root_class = p2_classify(
              staged.t, bx, bx + bo, UtttModel::empties(request.pos));
          diagnostics_.p2_checked = true;
          diagnostics_.root_class = root_class;
          if (clock.now_ms() >= hard) {
            return staged;
          }

          const int child_depth = std::max<int>(1, staged.depth - 1);
          Search<UtttModel> child_search(16);
          const auto solve_child = [&](const Position &child, Tie h2,
                                       int64_t stop_at) {
            SearchResult result = child_search.solve(
                child, h2,
                Limits{child_depth, 12'000, true, false, 12, 64,
                       [&] { return clock.now_ms() >= stop_at; }});
            diagnostics_.child_search_cancelled =
                diagnostics_.child_search_cancelled ||
                child_search.was_cancelled();
            return result;
          };

          Aggregates aggregates{};
          auto x_children = UtttModel::children_x(request.pos);
          auto o_children = UtttModel::children_o(request.pos);
          bid_matrix_detail::order_production_children(request.pos, Seat::X,
                                                       x_children);
          bid_matrix_detail::order_production_children(request.pos, Seat::O,
                                                       o_children);
          constexpr std::size_t kAnchorSweepLimit = 12;
          const int64_t anchor_stop =
              std::min(hard, saturating_deadline(deadlines.search_stop,
                                                 deadlines.reserve / 2));
          const std::size_t x_limit =
              std::min(x_children.size(), kAnchorSweepLimit);
          const std::size_t o_limit =
              std::min(o_children.size(), kAnchorSweepLimit);
          bool aggregates_complete =
              x_limit == x_children.size() && o_limit == o_children.size();
          std::vector<policy_detail::CertifiedChild> certified_x;
          std::vector<policy_detail::CertifiedChild> certified_o;
          certified_x.reserve(x_limit);
          certified_o.reserve(o_limit);
          for (std::size_t index = 0; index < x_limit; ++index) {
            if (clock.now_ms() >= anchor_stop) {
              aggregates_complete = false;
              break;
            }
            const SearchResult result =
                solve_child(x_children[index].state, Tie::O, anchor_stop);
            certified_x.push_back({x_children[index].move, result.t,
                                   result.quality, result.complete});
            if (!result.complete) {
              aggregates_complete = false;
              continue;
            }
            fold_x(aggregates, result.t);
            if (result.quality == Quality::Estimate)
              aggregates_complete = false;
          }
          for (std::size_t index = 0; index < o_limit; ++index) {
            if (clock.now_ms() >= anchor_stop) {
              aggregates_complete = false;
              break;
            }
            const SearchResult result =
                solve_child(o_children[index].state, Tie::X, anchor_stop);
            certified_o.push_back({o_children[index].move, result.t,
                                   result.quality, result.complete});
            if (!result.complete) {
              aggregates_complete = false;
              continue;
            }
            fold_o(aggregates, result.t);
            if (result.quality == Quality::Estimate)
              aggregates_complete = false;
          }
          const uint8_t best_x =
              policy_detail::legal_preferred(request, staged.best_x);
          const uint8_t best_o =
              policy_detail::legal_preferred(request, staged.best_o);
          if (root_class != InBand) {
            diagnostics_.certificate_attempted = true;
            const uint64_t own_stack =
                static_cast<uint64_t>(request.ctx.seat == Seat::X ? bx : bo);
            if (const auto certified = policy_detail::certified_forced_action(
                    root_class, request.ctx.seat, request.pos.tie,
                    static_cast<uint64_t>(bx + bo), own_stack, aggregates,
                    certified_x, certified_o, aggregates_complete)) {
              decision.bid = certified->bid;
              decision.move =
                  policy_detail::legal_preferred(request, certified->move);
              diagnostics_.certificate_published = true;
              diagnostics_.matrix_bypassed = true;
              return staged;
            }
          }
          if (clock.now_ms() >= hard)
            return staged;
          const TInterval anchor_a = aggregates.has_x ? aggregates.a : staged.t;
          const TInterval anchor_b = aggregates.has_o ? aggregates.b : staged.t;
          const Anchors anchors =
              production_anchors(anchor_a, anchor_b, bx + bo, best_x, best_o);

          struct CachedChild {
            Position state;
            Tie tie;
            SearchResult result;
          };
          std::vector<CachedChild> cache;
          AltSolver<UtttModel> alternate;
          PayoffFn<Position> payoff =
              [&](Position child, Tie h2, int64_t bx2,
                  int64_t bo2) -> std::optional<PayoffResult> {
            if (UtttModel::terminal(child) != TerminalKind::None)
              return production_payoff<UtttModel>(child, h2, bx2, bo2, {});
            if (bx2 + bo2 == 0) {
              diagnostics_.threshold_tt_before_alt =
                  child_search.tt_stats() ? *child_search.tt_stats()
                                          : CollisionStats{};
              const AltResult result = alternate.solve(
                  child, h2, AltLimits{child_depth, 12'000, 64, [&] {
                                         return clock.now_ms() >= hard;
                                       }});
              diagnostics_.child_search_cancelled =
                  diagnostics_.child_search_cancelled ||
                  alternate.was_cancelled();
              diagnostics_.threshold_tt_after_alt =
                  child_search.tt_stats() ? *child_search.tt_stats()
                                          : CollisionStats{};
              diagnostics_.alternation_searches =
                  alternate.unique_root_searches();
              diagnostics_.alternation_memo_entries = alternate.memo_entries();
              if (!result.complete)
                return std::nullopt;
              return PayoffResult{result.value,
                                  result.quality == Quality::Exact};
            }
            auto found = std::find_if(
                cache.begin(), cache.end(), [&](const CachedChild &entry) {
                  return entry.tie == h2 && entry.state.identity_equal(child);
                });
            if (found == cache.end()) {
              cache.push_back({child, h2, solve_child(child, h2, hard)});
              found = std::prev(cache.end());
            }
            if (!found->result.complete)
              return std::nullopt;
            return production_payoff<UtttModel>(child, h2, bx2, bo2,
                                                found->result);
          };
          diagnostics_.matrix_constructed = true;
          const auto matrix = build_bid_matrix<UtttModel>(
              request.pos, request.pos.tie, bx, bo, anchors, std::move(payoff),
              [&] { return clock.now_ms() >= hard; });
          diagnostics_.matrix_complete = matrix.complete;
          diagnostics_.matrix_entries = matrix.entries_evaluated;
          if (!matrix.complete)
            return staged;
          const RMPlusResult solution = solve_rmplus(
              matrix.payoffs, 10'000, [&] { return clock.now_ms() >= hard; });
          diagnostics_.rm_iterations = solution.iterations;
          if (solution.iterations == 0 || clock.now_ms() >= hard)
            return staged;
          diagnostics_.matrix_solved = true;
          const std::size_t selected =
              select_root_action(matrix, solution, request.ctx.seat);
          const auto &actions = request.ctx.seat == Seat::X
                                    ? matrix.row_actions
                                    : matrix.column_actions;
          decision.bid = actions[selected].bid;
          decision.move = actions[selected].move;
          diagnostics_.matrix_action_published = true;
          staged.quality = matrix_quality(staged.quality, matrix.all_exact);
          decision.searched = staged;
          return staged;
        });
    decision.searched = published;
    const Move move = policy_detail::unflatten(decision.move);
    assert(std::find(request.legal.begin(), request.legal.end(), move) !=
           request.legal.end());
    return {
        request.request_id,
        decision.bid,
        move,
        {policy_detail::quality_name(decision.searched.quality),
         decision.searched.t.lo, decision.searched.t.hi,
         decision.searched.depth, decision.searched.complete},
    };
  }

private:
  PolicyDiagnostics diagnostics_{};
};

using PlaceholderPolicy = EnginePolicy;

} // namespace uttt
