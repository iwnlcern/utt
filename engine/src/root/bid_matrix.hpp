#pragma once

#include "eval/eval.hpp"
#include "root/p2_gate.hpp"
#include "root/rmplus.hpp"
#include "search/game_model.hpp"
#include "search/search.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace uttt {

struct Anchors {
  int64_t k_star;
  uint8_t best_x;
  uint8_t best_o;
};

struct PayoffResult {
  double value;
  bool exact;
  bool operator==(const PayoffResult &) const = default;
};

struct RootAction {
  int64_t bid;
  uint8_t move;
  bool operator==(const RootAction &) const = default;
};

template <class State>
using PayoffFn =
    std::function<std::optional<PayoffResult>(State, Tie, int64_t, int64_t)>;

template <class State> struct RootMatrix {
  std::vector<RootAction> row_actions;
  std::vector<RootAction> column_actions;
  std::vector<std::vector<double>> payoffs;
  bool all_exact = true;
  bool complete = true;
  uint64_t entries_evaluated = 0;
};

std::vector<int64_t> candidate_bids(int64_t stack, int64_t k_star);
int64_t exact_half_up_product(double fraction, int64_t scale);

inline Anchors production_anchors(TInterval a, TInterval b, int64_t total,
                                  uint8_t best_x, uint8_t best_o) {
  const double a_mid = (a.lo + a.hi) / 2.0;
  const double b_mid = (b.lo + b.hi) / 2.0;
  const double r_root =
      a_mid > b_mid ? 0.0 : (b_mid - a_mid) / (1.0 - a_mid + b_mid);
  return {exact_half_up_product(r_root, total), best_x, best_o};
}

inline TInterval critical_r_enclosure(TInterval a, TInterval b) {
  if (!std::isfinite(a.lo) || !std::isfinite(a.hi) || !std::isfinite(b.lo) ||
      !std::isfinite(b.hi) || a.lo < 0.0 || a.lo > a.hi || b.lo < 0.0 ||
      b.lo > b.hi || a.hi > 1.0 || b.hi > 1.0)
    throw std::invalid_argument("critical-r intervals are outside [0,1]");
  const double lo_numerator = sub_down(b.lo, a.hi);
  const double lo_denominator = add_up(sub_up(1.0, a.hi), b.lo);
  const double hi_numerator = sub_up(b.hi, a.lo);
  const double hi_denominator = add_down(sub_down(1.0, a.lo), b.hi);
  return {
      lo_numerator <= 0.0
          ? 0.0
          : std::clamp(div_down(lo_numerator, lo_denominator), 0.0, 1.0),
      hi_denominator <= 0.0
          ? 1.0
          : std::clamp(div_up(hi_numerator, hi_denominator), 0.0, 1.0),
  };
}

namespace bid_matrix_detail {

template <class State> inline bool is_ttt3_opening(const State &state) {
  if constexpr (requires {
                  state.x;
                  state.o;
                } && std::same_as<std::remove_cvref_t<decltype(state.x)>,
                                  uint16_t>) {
    return state.x == 0 && state.o == 0;
  }
  return false;
}

template <class State, class Children>
inline void order_production_children(const State &state, Seat mover,
                                      Children &children) {
  if constexpr (std::same_as<State, Position>) {
    std::stable_sort(children.begin(), children.end(),
                     [&](const auto &lhs, const auto &rhs) {
                       const auto left =
                           tactical_order_key(state, mover, lhs.move);
                       const auto right =
                           tactical_order_key(state, mover, rhs.move);
                       if (left != right)
                         return left > right;
                       return lhs.move < rhs.move;
                     });
  }
}

template <class State, class Children>
inline std::vector<uint8_t> candidate_moves(const State &state,
                                            uint8_t preferred, Seat mover,
                                            Children children) {
  if (is_ttt3_opening(state))
    return {0, 1, 4, 8};
  order_production_children(state, mover, children);
  std::vector<uint8_t> result;
  result.reserve(4);
  const auto add = [&](uint8_t move) {
    if (result.size() == 4)
      return;
    if (std::none_of(children.begin(), children.end(),
                     [move](const auto &child) { return child.move == move; }))
      return;
    if (std::find(result.begin(), result.end(), move) == result.end())
      result.push_back(move);
  };
  add(preferred);
  for (const auto &child : children)
    add(child.move);
  return result;
}

template <class State, class Children>
inline State child_with_move(const Children &children, uint8_t move) {
  const auto found =
      std::find_if(children.begin(), children.end(),
                   [move](const auto &child) { return child.move == move; });
  if (found == children.end())
    throw std::logic_error("root action references an illegal move");
  return found->state;
}

inline int compare_ratio_to_binary64(int64_t numerator, int64_t denominator,
                                     double threshold) {
  if (numerator < 0 || denominator < 0 || numerator > denominator ||
      !std::isfinite(threshold) || threshold < 0.0 || threshold > 1.0) {
    throw std::invalid_argument("ratio comparison inputs are out of range");
  }
  if (denominator == 0)
    return 0;
  if (threshold == 0.0)
    return numerator == 0 ? 0 : 1;
  if (threshold == 1.0)
    return numerator == denominator ? 0 : -1;
  const auto decomposed = p2_detail::decompose_unit_endpoint(threshold);
  const unsigned shift = static_cast<unsigned>(-decomposed.exponent);
  if (p2_detail::shift_overflows_wide(static_cast<uint64_t>(numerator), shift))
    return numerator == 0 ? -1 : 1;
  const p2_detail::Wide left =
      p2_detail::shifted_wide(static_cast<uint64_t>(numerator), shift);
  const p2_detail::Wide right = p2_detail::scaled_mantissa(
      decomposed, static_cast<uint64_t>(denominator));
  return (left > right) - (left < right);
}

} // namespace bid_matrix_detail

template <GameModel M>
RootMatrix<typename M::State>
build_bid_matrix(typename M::State state, Tie h, int64_t bx, int64_t bo,
                 Anchors anchors, PayoffFn<typename M::State> payoff,
                 std::function<bool()> stop = {}) {
  if ((h != Tie::X && h != Tie::O && h != Tie::NullFirstMove) || bx < 0 ||
      bo < 0 || bx > std::numeric_limits<uint32_t>::max() ||
      bo > std::numeric_limits<uint32_t>::max() ||
      bx + bo > std::numeric_limits<uint32_t>::max()) {
    throw std::invalid_argument("root matrix inputs are out of range");
  }
  const auto x_children = M::children_x(state);
  const auto o_children = M::children_o(state);
  if (x_children.empty() || o_children.empty())
    throw std::invalid_argument("root matrix requires a nonterminal state");
  const auto bids_x = candidate_bids(bx, anchors.k_star);
  const auto bids_o = candidate_bids(bo, anchors.k_star);
  const auto moves_x = bid_matrix_detail::candidate_moves(state, anchors.best_x,
                                                          Seat::X, x_children);
  const auto moves_o = bid_matrix_detail::candidate_moves(state, anchors.best_o,
                                                          Seat::O, o_children);

  RootMatrix<typename M::State> result;
  for (int64_t bid : bids_x)
    for (uint8_t move : moves_x)
      result.row_actions.push_back({bid, move});
  for (int64_t bid : bids_o)
    for (uint8_t move : moves_o)
      result.column_actions.push_back({bid, move});
  result.payoffs.reserve(result.row_actions.size());
  for (RootAction row : result.row_actions) {
    std::vector<double> values;
    values.reserve(result.column_actions.size());
    for (RootAction column : result.column_actions) {
      if (stop && stop()) {
        result.complete = false;
        result.payoffs.push_back(std::move(values));
        return result;
      }
      std::optional<PayoffResult> entry;
      if (row.bid == column.bid && h == Tie::NullFirstMove) {
        const auto x_entry =
            payoff(bid_matrix_detail::child_with_move<typename M::State>(
                       x_children, row.move),
                   Tie::O, bx - row.bid, bo);
        if (!x_entry) {
          result.complete = false;
          result.payoffs.push_back(std::move(values));
          return result;
        }
        if (!std::isfinite(x_entry->value) || x_entry->value < -1.0 ||
            x_entry->value > 1.0)
          throw std::logic_error("root coin-branch payoff is not finite in [-1,+1]");
        if (stop && stop()) {
          result.complete = false;
          result.payoffs.push_back(std::move(values));
          return result;
        }
        const auto o_entry =
            payoff(bid_matrix_detail::child_with_move<typename M::State>(
                       o_children, column.move),
                   Tie::X, bx, bo - column.bid);
        if (!o_entry) {
          result.complete = false;
          result.payoffs.push_back(std::move(values));
          return result;
        }
        if (!std::isfinite(o_entry->value) || o_entry->value < -1.0 ||
            o_entry->value > 1.0)
          throw std::logic_error("root coin-branch payoff is not finite in [-1,+1]");
        entry = PayoffResult{0.5 * x_entry->value + 0.5 * o_entry->value,
                             x_entry->exact && o_entry->exact};
      } else if (row.bid > column.bid ||
                 (row.bid == column.bid && h == Tie::X)) {
        entry = payoff(bid_matrix_detail::child_with_move<typename M::State>(
                           x_children, row.move),
                       Tie::O, bx - row.bid, bo);
      } else {
        entry = payoff(bid_matrix_detail::child_with_move<typename M::State>(
                           o_children, column.move),
                       Tie::X, bx, bo - column.bid);
      }
      if (!entry) {
        result.complete = false;
        result.payoffs.push_back(std::move(values));
        return result;
      }
      if (!std::isfinite(entry->value) || entry->value < -1.0 ||
          entry->value > 1.0)
        throw std::logic_error("root payoff is not finite in [-1,+1]");
      values.push_back(entry->value);
      ++result.entries_evaluated;
      result.all_exact = result.all_exact && entry->exact;
    }
    result.payoffs.push_back(std::move(values));
  }
  return result;
}

template <GameModel M>
PayoffResult production_payoff(const typename M::State &child, Tie h,
                               int64_t bx, int64_t bo,
                               const SearchResult &searched) {
  (void)h;
  if (bx < 0 || bo < 0 || bx + bo > std::numeric_limits<uint32_t>::max())
    throw std::invalid_argument("production payoff budgets are out of range");
  switch (M::terminal(child)) {
  case TerminalKind::MacroWinX:
    return {1, true};
  case TerminalKind::MacroWinO:
    return {-1, true};
  case TerminalKind::AllClosed:
    return {static_cast<double>(M::chip_sign(child, bx, bo)), true};
  case TerminalKind::None:
    break;
  }
  const int64_t total = bx + bo;
  if (total == 0)
    throw std::invalid_argument(
        "nonterminal zero-total payoff requires the alternation solver");
  const RootClass forced =
      p2_classify(searched.t, bx, total, M::empties(child));
  if (forced == XForced)
    return {1, true};
  if (forced == OForced)
    return {-1, true};
  const double p = static_cast<double>(bx) / static_cast<double>(total);
  return {std::clamp(8.0 * (p - searched.t_est), -1.0, 1.0), false};
}

template <class State>
std::size_t select_root_action(const RootMatrix<State> &matrix,
                               const RMPlusResult &solution, Seat seat) {
  const bool x_seat = seat == Seat::X;
  if ((!x_seat && seat != Seat::O) ||
      solution.row_strategy.size() != matrix.row_actions.size() ||
      solution.column_strategy.size() != matrix.column_actions.size() ||
      matrix.payoffs.size() != matrix.row_actions.size() ||
      std::any_of(matrix.payoffs.begin(), matrix.payoffs.end(),
                  [&](const auto &row) {
                    return row.size() != matrix.column_actions.size();
                  }))
    throw std::invalid_argument("root action extraction inputs do not align");

  const auto &actions = x_seat ? matrix.row_actions : matrix.column_actions;
  const auto &probabilities =
      x_seat ? solution.row_strategy : solution.column_strategy;
  const double maximum_probability =
      *std::max_element(probabilities.begin(), probabilities.end());
  std::vector<double> values(actions.size(), 0.0);
  if (x_seat) {
    for (std::size_t i = 0; i < matrix.row_actions.size(); ++i)
      for (std::size_t j = 0; j < matrix.column_actions.size(); ++j)
        values[i] += matrix.payoffs[i][j] * solution.column_strategy[j];
  } else {
    for (std::size_t j = 0; j < matrix.column_actions.size(); ++j)
      for (std::size_t i = 0; i < matrix.row_actions.size(); ++i)
        values[j] += solution.row_strategy[i] * matrix.payoffs[i][j];
  }
  const auto better_action = [&](std::size_t candidate, std::size_t incumbent) {
    if (values[candidate] != values[incumbent])
      return x_seat ? values[candidate] > values[incumbent]
                    : values[candidate] < values[incumbent];
    if (actions[candidate].bid != actions[incumbent].bid)
      return actions[candidate].bid < actions[incumbent].bid;
    return actions[candidate].move < actions[incumbent].move;
  };
  std::size_t chosen = static_cast<std::size_t>(std::find(probabilities.begin(),
                                                          probabilities.end(),
                                                          maximum_probability) -
                                                probabilities.begin());
  for (std::size_t index = chosen + 1; index < actions.size(); ++index)
    if (probabilities[index] == maximum_probability &&
        better_action(index, chosen))
      chosen = index;
  for (std::size_t index = 0; index < chosen; ++index)
    if (probabilities[index] == maximum_probability &&
        better_action(index, chosen))
      chosen = index;
  return chosen;
}

inline Quality matrix_quality(Quality search_quality, bool all_exact) {
  return all_exact ? search_quality : Quality::Estimate;
}

} // namespace uttt
