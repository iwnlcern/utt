#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "search/backup.hpp"
#include "ttt3_model.hpp"

namespace uttt {

enum Outcome { OWin = 0, Draw = 1, XWin = 2 };

namespace discrete_detail {

class Solver {
public:
  Outcome solve(Ttt3State state, Tie tie, int bx, int bo) {
    if (tie != Tie::X && tie != Tie::O) {
      throw std::invalid_argument("discrete ttt3 tie owner must be X or O");
    }
    const int64_t total = int64_t{bx} + int64_t{bo};
    if (bx < 0 || bo < 0 || total > 64) {
      throw std::invalid_argument(
          "discrete ttt3 total budget must be in [0,64]");
    }
    assert(bx >= 0);
    assert(bo >= 0);
    assert(total <= 64);
    state.tie = tie;
    return solve_memoized(state, tie, bx, bo);
  }

private:
  std::unordered_map<uint64_t, Outcome> memo_;

  static uint64_t memo_key(const Ttt3State &state, Tie tie, int bx, int bo) {
    return uint64_t{state.x} | (uint64_t{state.o} << 9) |
           (uint64_t{tie == Tie::O} << 18) | (static_cast<uint64_t>(bx) << 19) |
           (static_cast<uint64_t>(bo) << 26);
  }

  Outcome solve_memoized(const Ttt3State &state, Tie tie, int bx, int bo) {
    const uint64_t key = memo_key(state, tie, bx, bo);
    if (const auto found = memo_.find(key); found != memo_.end()) {
      return found->second;
    }

    Outcome terminal_outcome = Draw;
    switch (Ttt3Model::terminal(state)) {
    case TerminalKind::MacroWinX:
      terminal_outcome = XWin;
      break;
    case TerminalKind::MacroWinO:
      terminal_outcome = OWin;
      break;
    case TerminalKind::AllClosed:
      terminal_outcome = bx > bo ? XWin : bx < bo ? OWin : Draw;
      break;
    case TerminalKind::None:
      return solve_nonterminal(state, tie, bx, bo, key);
    }
    return memo_.emplace(key, terminal_outcome).first->second;
  }

  Outcome solve_nonterminal(const Ttt3State &state, Tie tie, int bx, int bo,
                            uint64_t key) {
    const auto x_children = Ttt3Model::children_x(state);
    const auto o_children = Ttt3Model::children_o(state);
    if (x_children.empty() || o_children.empty()) {
      throw std::logic_error("nonterminal ttt3 state has no legal moves");
    }

    // For each bid, retain the mover's best intended move at the exact
    // post-payment budget. This is the C9a reduction of the full Cartesian
    // (bid, intended-move) matrix: the losing side's intended move cannot
    // affect the successor, while the winning side's move remains explicit.
    std::vector<Outcome> x_move_value(static_cast<std::size_t>(bx) + 1, OWin);
    for (int bid_x = 0; bid_x <= bx; ++bid_x) {
      for (const auto &child : x_children) {
        x_move_value[bid_x] =
            std::max(x_move_value[bid_x],
                     solve_memoized(child.state, Tie::O, bx - bid_x, bo));
      }
    }

    std::vector<Outcome> o_move_value(static_cast<std::size_t>(bo) + 1, XWin);
    for (int bid_o = 0; bid_o <= bo; ++bid_o) {
      for (const auto &child : o_children) {
        o_move_value[bid_o] =
            std::min(o_move_value[bid_o],
                     solve_memoized(child.state, Tie::X, bx, bo - bid_o));
      }
    }

    std::vector<Outcome> suffix_o_min = o_move_value;
    for (int bid_o = bo; bid_o-- > 0;) {
      suffix_o_min[bid_o] =
          std::min(suffix_o_min[bid_o], suffix_o_min[bid_o + 1]);
    }
    std::vector<Outcome> suffix_x_max = x_move_value;
    for (int bid_x = bx; bid_x-- > 0;) {
      suffix_x_max[bid_x] =
          std::max(suffix_x_max[bid_x], suffix_x_max[bid_x + 1]);
    }

    Outcome maximin = OWin;
    for (int bid_x = 0; bid_x <= bx; ++bid_x) {
      Outcome row_min = XWin;
      bool has_entry = false;

      // X wins against O bids <= bid_x when X owns ties, otherwise < bid_x.
      if (tie == Tie::X || bid_x > 0) {
        row_min = x_move_value[bid_x];
        has_entry = true;
      }
      const int first_o_win = tie == Tie::X ? bid_x + 1 : bid_x;
      if (first_o_win <= bo) {
        row_min = has_entry ? std::min(row_min, suffix_o_min[first_o_win])
                            : suffix_o_min[first_o_win];
        has_entry = true;
      }
      assert(has_entry);
      maximin = std::max(maximin, row_min);
    }

    Outcome minimax = XWin;
    for (int bid_o = 0; bid_o <= bo; ++bid_o) {
      Outcome column_max = OWin;
      bool has_entry = false;

      // O wins against X bids < bid_o when X owns ties, otherwise <= bid_o.
      if (tie == Tie::O || bid_o > 0) {
        column_max = o_move_value[bid_o];
        has_entry = true;
      }
      const int first_x_win = tie == Tie::X ? bid_o : bid_o + 1;
      if (first_x_win <= bx) {
        column_max = has_entry ? std::max(column_max, suffix_x_max[first_x_win])
                               : suffix_x_max[first_x_win];
        has_entry = true;
      }
      assert(has_entry);
      minimax = std::min(minimax, column_max);
    }

    if (maximin != minimax) {
      throw std::logic_error("discrete ttt3 pure maximin differs from minimax");
    }
    assert(maximin == minimax);
    return memo_.emplace(key, maximin).first->second;
  }
};

} // namespace discrete_detail

inline Outcome solve_discrete(Ttt3State state, Tie tie, int bx, int bo) {
  static thread_local discrete_detail::Solver solver;
  return solver.solve(state, tie, bx, bo);
}

} // namespace uttt
