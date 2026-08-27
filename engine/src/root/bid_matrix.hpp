#pragma once

#include "eval/eval.hpp"
#include "root/p2_gate.hpp"
#include "search/game_model.hpp"
#include "search/search.hpp"

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstdint>
#include <functional>
#include <limits>
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
  int ordinal;
  bool exact;
  bool operator==(const PayoffResult &) const = default;
};

struct RootAction {
  int64_t bid;
  uint8_t move;
  bool operator==(const RootAction &) const = default;
};

template <class State>
using PayoffFn = std::function<PayoffResult(State, Tie, int64_t, int64_t)>;

template <class State> struct RootMatrix {
  std::vector<RootAction> row_actions;
  std::vector<RootAction> column_actions;
  std::vector<std::vector<int>> payoffs;
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
  if ((h != Tie::X && h != Tie::O) || bx < 0 || bo < 0 ||
      bx > std::numeric_limits<uint32_t>::max() ||
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
    std::vector<int> values;
    values.reserve(result.column_actions.size());
    for (RootAction column : result.column_actions) {
      if (stop && stop()) {
        result.complete = false;
        result.payoffs.push_back(std::move(values));
        return result;
      }
      PayoffResult entry{};
      if (row.bid > column.bid || (row.bid == column.bid && h == Tie::X)) {
        entry = payoff(bid_matrix_detail::child_with_move<typename M::State>(
                           x_children, row.move),
                       Tie::O, bx - row.bid, bo);
      } else {
        entry = payoff(bid_matrix_detail::child_with_move<typename M::State>(
                           o_children, column.move),
                       Tie::X, bx, bo - column.bid);
      }
      if (entry.ordinal < -1 || entry.ordinal > 1)
        throw std::logic_error("root payoff ordinal is outside {-1,0,+1}");
      values.push_back(entry.ordinal);
      ++result.entries_evaluated;
      result.all_exact = result.all_exact && entry.exact;
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
    return {M::chip_sign(child, bx, bo), true};
  case TerminalKind::None:
    break;
  }
  const int64_t total = bx + bo;
  const RootClass forced =
      p2_classify(searched.t, bx, total, M::empties(child));
  if (forced == XForced)
    return {1, true};
  if (forced == OForced)
    return {-1, true};
  const double midpoint = (searched.t.lo + searched.t.hi) / 2.0;
  return {bid_matrix_detail::compare_ratio_to_binary64(bx, total, midpoint),
          false};
}

inline Quality matrix_quality(Quality search_quality, bool all_exact) {
  return all_exact ? search_quality : Quality::Estimate;
}

} // namespace uttt
