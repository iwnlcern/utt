#pragma once

#include <bit>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "search/game_model.hpp"

namespace uttt {

struct Ttt3State {
  uint16_t x = 0;
  uint16_t o = 0;
  TieState tie = TieState::NullFirstMove;

  static Ttt3State from_board(std::string_view board, TieState tie_state) {
    if (board.size() != 9)
      throw std::invalid_argument("ttt3 board must have nine cells");
    Ttt3State result{0, 0, tie_state};
    for (int cell = 0; cell < 9; ++cell) {
      const uint16_t bit = uint16_t(1u << cell);
      if (board[cell] == 'X')
        result.x |= bit;
      else if (board[cell] == 'O')
        result.o |= bit;
      else if (board[cell] != '.')
        throw std::invalid_argument("invalid ttt3 cell");
    }
    return result;
  }

  uint32_t dense_code() const {
    uint32_t result = 0;
    uint32_t place = 1;
    for (int cell = 0; cell < 9; ++cell) {
      const uint16_t bit = uint16_t(1u << cell);
      if ((x & bit) != 0) result += place;
      if ((o & bit) != 0) result += 2 * place;
      place *= 3;
    }
    return result;
  }

  bool operator==(const Ttt3State&) const = default;
};

struct Ttt3Model {
  using State = Ttt3State;

  static std::vector<ModelChild<State>> children_x(const State& state) {
    return children(state, Seat::X);
  }

  static std::vector<ModelChild<State>> children_o(const State& state) {
    return children(state, Seat::O);
  }

  static TerminalKind terminal(const State& state) {
    if (has_win(state.x)) return TerminalKind::MacroWinX;
    if (has_win(state.o)) return TerminalKind::MacroWinO;
    if ((state.x | state.o) == 0x1FFu) return TerminalKind::AllClosed;
    return TerminalKind::None;
  }

  static int empties(const State& state) {
    return 9 - std::popcount(static_cast<uint16_t>(state.x | state.o));
  }

  static TTKey tt_key(const State& state) {
    const uint64_t code = state.dense_code();
    return {code, static_cast<uint32_t>(splitmix64(code))};
  }

  static PosId pos_id(const State& state) {
    PosId result;
    result.x[0] = state.x;
    result.o[0] = state.o;
    result.tie = state.tie;
    return result;
  }

  static int chip_sign(const State&, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }

 private:
  static bool has_win(uint16_t marks) {
    for (uint16_t line : kWinLines) {
      if ((marks & line) == line) return true;
    }
    return false;
  }

  static uint64_t splitmix64(uint64_t input) {
    uint64_t value = input + 0x9E3779B97F4A7C15ULL;
    value = (value ^ (value >> 30)) * 0xBF58476D1CE4E5B9ULL;
    value = (value ^ (value >> 27)) * 0x94D049BB133111EBULL;
    return value ^ (value >> 31);
  }

  static std::vector<ModelChild<State>> children(const State& state,
                                                 Seat mover) {
    std::vector<ModelChild<State>> result;
    if (terminal(state) != TerminalKind::None) return result;
    result.reserve(empties(state));
    const uint16_t occupied = static_cast<uint16_t>(state.x | state.o);
    for (uint8_t cell = 0; cell < 9; ++cell) {
      const uint16_t bit = uint16_t(1u << cell);
      if ((occupied & bit) != 0) continue;
      State child = state;
      (mover == Seat::X ? child.x : child.o) |= bit;
      child.tie = mover == Seat::X ? TieState::O : TieState::X;
      result.push_back({child, cell});
    }
    return result;
  }
};

static_assert(GameModel<Ttt3Model>);

}  // namespace uttt
