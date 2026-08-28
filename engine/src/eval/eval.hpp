#pragma once

#include "core/local_table.hpp"
#include "core/position.hpp"
#include "eval/weights.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <tuple>

namespace uttt {

inline constexpr std::size_t kEvalFeatureCount = kEvalWeights.size();
using EvalFeatures = std::array<double, kEvalFeatureCount>;

inline double tactical_richness(const LocalTable &table, uint32_t code,
                                Seat side) {
  return 4.0 * std::popcount(table.win(code, side)) +
         2.0 * std::popcount(table.fork(code, side)) +
         std::popcount(table.win(code, opponent(side))) +
         std::popcount(table.fork(code, opponent(side)));
}

inline EvalFeatures eval_features(const Position &position) {
  EvalFeatures result{};
  result[0] = 1.0;
  const auto &table = LocalTable::instance();
  for (int board = 0; board < 9; ++board) {
    const LocalStatus status = table.status(position.tern[board]);
    switch (status) {
    case LocalStatus::Open: ++result[1]; break;
    case LocalStatus::XWon: ++result[2]; break;
    case LocalStatus::OWon: ++result[3]; break;
    case LocalStatus::FullDraw: ++result[4]; break;
    }
    if (status != LocalStatus::Open) continue;
    const double win_x = std::popcount(table.win(position.tern[board], Seat::X));
    const double win_o = std::popcount(table.win(position.tern[board], Seat::O));
    result[5] += win_x;
    result[6] += win_o;
    result[7] += win_o;
    result[8] += win_x;
    result[9] += std::popcount(table.fork(position.tern[board], Seat::X));
    result[10] += std::popcount(table.fork(position.tern[board], Seat::O));
  }
  const uint16_t blocks_x =
      static_cast<uint16_t>(position.closed & ~position.macro_x);
  const uint16_t blocks_o =
      static_cast<uint16_t>(position.closed & ~position.macro_o);
  for (uint16_t line : kWinLines) {
    if ((line & blocks_x) == 0)
      result[11] += std::popcount(static_cast<uint16_t>(line & position.macro_x));
    if ((line & blocks_o) == 0)
      result[12] += std::popcount(static_cast<uint16_t>(line & position.macro_o));
  }
  if (position.forced != kForcedAny) {
    const uint16_t legal = table.empties(position.tern[position.forced]);
    for (int cell = 0; cell < 9; ++cell) {
      if (((legal >> cell) & 1u) == 0) continue;
      if (((position.closed >> cell) & 1u) == 0) {
        result[13] += tactical_richness(table, position.tern[cell], Seat::X);
        result[14] += tactical_richness(table, position.tern[cell], Seat::O);
        continue;
      }
      // A closed destination releases the next player to ANY board, so this
      // edge reaches every still-open board in the forced-board graph.
      for (int destination = 0; destination < 9; ++destination) {
        if (((position.closed >> destination) & 1u) != 0) continue;
        result[13] +=
            tactical_richness(table, position.tern[destination], Seat::X);
        result[14] +=
            tactical_richness(table, position.tern[destination], Seat::O);
      }
    }
  }
  return result;
}

inline double eval_estimate(const Position &position) {
  const EvalFeatures features = eval_features(position);
  double score = 0.0;
  for (std::size_t i = 0; i < features.size(); ++i)
    score += features[i] * kEvalWeights[i];
  return 1.0 / (1.0 + std::exp(-score));
}

inline std::tuple<int, int, int, int, int>
tactical_order_key(const Position &parent, Seat mover, uint8_t flat_move) {
  const uint8_t board = flat_move / 9;
  const uint8_t cell = flat_move % 9;
  const uint16_t bit = static_cast<uint16_t>(1u << cell);
  const auto &table = LocalTable::instance();
  const auto child = parent.applied({board, cell}, mover);
  const int macro_win = child &&
      child->terminal() == (mover == Seat::X ? TerminalKind::MacroWinX
                                             : TerminalKind::MacroWinO);
  const int local_win = (table.win(parent.tern[board], mover) & bit) != 0;
  const int block =
      (table.win(parent.tern[board], opponent(mover)) & bit) != 0;
  const int fork = (table.fork(parent.tern[board], mover) & bit) != 0;
  int routing = 0;
  if (((parent.closed >> cell) & 1u) == 0) {
    const uint32_t destination = parent.tern[cell];
    routing = 4 * std::popcount(table.win(destination, mover)) +
              2 * std::popcount(table.fork(destination, mover)) +
              std::popcount(table.win(destination, opponent(mover))) +
              std::popcount(table.fork(destination, opponent(mover)));
  }
  return {macro_win, local_win, block, fork, routing};
}

} // namespace uttt
