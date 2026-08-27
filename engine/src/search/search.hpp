#pragma once

#include "search/backup.hpp"
#include "search/game_model.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <limits>

namespace uttt {

enum class Quality : uint8_t { Exact, Bound, Estimate };

struct Limits {
  int max_depth = 0;
  uint64_t node_cap = std::numeric_limits<uint64_t>::max();
};

struct SearchResult {
  TInterval t{0.0, 1.0};
  uint8_t best_x = std::numeric_limits<uint8_t>::max();
  uint8_t best_o = std::numeric_limits<uint8_t>::max();
  Quality quality = Quality::Estimate;
  uint8_t depth = 0;
  bool complete = false;
};

template <GameModel M> struct Search {
  using State = typename M::State;

  SearchResult solve(State state, Tie h, Limits limits) {
    nodes_ = 0;
    const int horizon = std::max(limits.max_depth, 0);
    node_cap_ = limits.node_cap;

    NodeResult result;
    // TieState::NullFirstMove is the repository's existing representation of
    // F-C6's null root. It is a request for the two-conditional envelope, not
    // a claim about the simultaneous hidden-coin game.
    if (h == Tie::NullFirstMove) {
      const NodeResult conditional_x = dfs(state, Tie::X, horizon);
      if (!conditional_x.complete)
        return incomplete_result();
      const NodeResult conditional_o = dfs(state, Tie::O, horizon);
      if (!conditional_o.complete)
        return incomplete_result();
      result = conditional_x;
      result.t = {
          std::min(conditional_x.t.lo, conditional_o.t.lo),
          std::max(conditional_x.t.hi, conditional_o.t.hi),
      };
      result.quality =
          combine_quality(conditional_x.quality, conditional_o.quality);
    } else {
      assert(h == Tie::X || h == Tie::O);
      result = dfs(state, h, horizon);
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
    };
  }

private:
  struct NodeResult {
    TInterval t{0.0, 1.0};
    uint8_t best_x = std::numeric_limits<uint8_t>::max();
    uint8_t best_o = std::numeric_limits<uint8_t>::max();
    Quality quality = Quality::Estimate;
    bool complete = false;
  };

  uint64_t nodes_ = 0;
  uint64_t node_cap_ = 0;

  static Quality combine_quality(Quality lhs, Quality rhs) {
    if (lhs == Quality::Estimate || rhs == Quality::Estimate) {
      return Quality::Estimate;
    }
    if (lhs == Quality::Bound || rhs == Quality::Bound)
      return Quality::Bound;
    return Quality::Exact;
  }

  static SearchResult incomplete_result() {
    return {
        {0.0, 1.0},
        std::numeric_limits<uint8_t>::max(),
        std::numeric_limits<uint8_t>::max(),
        Quality::Estimate,
        0,
        false,
    };
  }

  NodeResult dfs(const State &state, Tie h, int remaining_depth) {
    if (nodes_ >= node_cap_)
      return {};
    ++nodes_;

    switch (M::terminal(state)) {
    case TerminalKind::MacroWinX:
      return {{0.0, 0.0}, no_move(), no_move(), Quality::Exact, true};
    case TerminalKind::MacroWinO:
      return {{1.0, 1.0}, no_move(), no_move(), Quality::Exact, true};
    case TerminalKind::AllClosed:
      return {{0.5, 0.5}, no_move(), no_move(), Quality::Exact, true};
    case TerminalKind::None:
      break;
    }

    if (remaining_depth == 0) {
      return {{0.0, 1.0}, no_move(), no_move(), Quality::Estimate, true};
    }

    const auto x_children = M::children_x(state);
    const auto o_children = M::children_o(state);
    assert(!x_children.empty() && !o_children.empty());

    Aggregates aggregates{};
    Quality quality = Quality::Exact;
    uint8_t best_x = no_move();
    double best_x_hi = std::numeric_limits<double>::infinity();
    for (const auto &child : x_children) {
      const NodeResult searched = dfs(child.state, Tie::O, remaining_depth - 1);
      if (!searched.complete)
        return {};
      fold_x(aggregates, searched.t);
      quality = combine_quality(quality, searched.quality);
      if (searched.t.hi < best_x_hi ||
          (searched.t.hi == best_x_hi && child.move < best_x)) {
        best_x_hi = searched.t.hi;
        best_x = child.move;
      }
    }

    uint8_t best_o = no_move();
    double best_o_lo = -std::numeric_limits<double>::infinity();
    for (const auto &child : o_children) {
      const NodeResult searched = dfs(child.state, Tie::X, remaining_depth - 1);
      if (!searched.complete)
        return {};
      fold_o(aggregates, searched.t);
      quality = combine_quality(quality, searched.quality);
      if (searched.t.lo > best_o_lo ||
          (searched.t.lo == best_o_lo && child.move > best_o)) {
        best_o_lo = searched.t.lo;
        best_o = child.move;
      }
    }

    return {
        backup_node(aggregates, h), best_x, best_o, quality, true,
    };
  }

  static constexpr uint8_t no_move() {
    return std::numeric_limits<uint8_t>::max();
  }
};

extern template struct Search<UtttModel>;

} // namespace uttt
