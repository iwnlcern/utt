#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "search/backup.hpp"
#include "test_rational.hpp"
#include "ttt3_model.hpp"

namespace uttt {

struct ContNode {
  TestRational T{0};
  TestRational r{0};
  uint8_t best_x = 0;
  uint8_t best_o = 0;
};

namespace continuous_detail {

class Solver {
public:
  ContNode solve(Ttt3State state, Tie tie) {
    if (tie != Tie::X && tie != Tie::O) {
      throw std::invalid_argument("continuous ttt3 tie owner must be X or O");
    }
    state.tie = tie;
    return solve_memoized(state, tie);
  }

private:
  std::unordered_map<uint64_t, ContNode> memo_;

  static uint64_t memo_key(const Ttt3State &state, Tie tie) {
    return uint64_t{state.x} | (uint64_t{state.o} << 9) |
           (uint64_t{tie == Tie::O} << 18);
  }

  const ContNode &solve_memoized(const Ttt3State &state, Tie tie) {
    const uint64_t key = memo_key(state, tie);
    if (const auto found = memo_.find(key); found != memo_.end()) {
      return found->second;
    }

    switch (Ttt3Model::terminal(state)) {
    case TerminalKind::MacroWinX:
      return memo_.emplace(key, ContNode{TestRational{0}, TestRational{0}})
          .first->second;
    case TerminalKind::MacroWinO:
      return memo_.emplace(key, ContNode{TestRational{1}, TestRational{0}})
          .first->second;
    case TerminalKind::AllClosed:
      return memo_.emplace(key, ContNode{TestRational{1, 2}, TestRational{0}})
          .first->second;
    case TerminalKind::None:
      break;
    }

    const auto x_children = Ttt3Model::children_x(state);
    const auto o_children = Ttt3Model::children_o(state);
    if (x_children.empty() || o_children.empty()) {
      throw std::logic_error("nonterminal ttt3 state has no children");
    }

    TestRational a = solve_memoized(x_children.front().state, Tie::O).T;
    uint8_t best_x = x_children.front().move;
    // Children are cell-ascending. Strict replacement preserves Python's
    // stable min/max tie rule: the first (lowest) equal-valued move wins.
    for (std::size_t i = 1; i < x_children.size(); ++i) {
      const TestRational child_T =
          solve_memoized(x_children[i].state, Tie::O).T;
      if (child_T < a) {
        a = child_T;
        best_x = x_children[i].move;
      }
    }

    TestRational b = solve_memoized(o_children.front().state, Tie::X).T;
    uint8_t best_o = o_children.front().move;
    for (std::size_t i = 1; i < o_children.size(); ++i) {
      const TestRational child_T =
          solve_memoized(o_children[i].state, Tie::X).T;
      if (b < child_T) {
        b = child_T;
        best_o = o_children[i].move;
      }
    }

    ContNode result;
    result.best_x = best_x;
    result.best_o = best_o;
    if (a <= b) {
      const TestRational denominator = TestRational{1} - a + b;
      result.T = b / denominator;
      result.r = (b - a) / denominator;
    } else {
      result.T = tie == Tie::X ? a : b;
      result.r = TestRational{0};
    }
    return memo_.emplace(key, std::move(result)).first->second;
  }
};

} // namespace continuous_detail

inline ContNode solve_continuous(Ttt3State state, Tie tie) {
  static thread_local continuous_detail::Solver solver;
  return solver.solve(state, tie);
}

} // namespace uttt
