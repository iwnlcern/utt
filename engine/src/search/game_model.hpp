#pragma once

#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <vector>

#include "core/position.hpp"
#include "core/zobrist.hpp"

namespace uttt {

struct TTKey {
  uint64_t key;
  uint32_t tag;
  bool operator==(const TTKey&) const = default;
};

struct PosId {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  int8_t forced = kForcedAny;
  TieState tie = TieState::NullFirstMove;
  bool operator==(const PosId&) const = default;
};

template <class StateT>
struct ModelChild {
  StateT state;
  uint8_t move;
};

template <class M>
concept GameModel =
    requires(const typename M::State& state, int64_t bx, int64_t bo) {
      { M::children_x(state) } -> std::ranges::input_range;
      { M::children_o(state) } -> std::ranges::input_range;
      requires std::same_as<
          std::ranges::range_value_t<decltype(M::children_x(state))>,
          ModelChild<typename M::State>>;
      requires std::same_as<
          std::ranges::range_value_t<decltype(M::children_o(state))>,
          ModelChild<typename M::State>>;
      { M::terminal(state) } -> std::same_as<TerminalKind>;
      { M::empties(state) } -> std::same_as<int>;
      { M::tt_key(state) } -> std::same_as<TTKey>;
      { M::pos_id(state) } -> std::same_as<PosId>;
      { M::chip_sign(state, bx, bo) } -> std::same_as<int>;
    };

struct UtttModel {
  using State = Position;

  static std::vector<ModelChild<State>> children_x(const State& state) {
    return children(state, Seat::X);
  }

  static std::vector<ModelChild<State>> children_o(const State& state) {
    return children(state, Seat::O);
  }

  static TerminalKind terminal(const State& state) { return state.terminal(); }

  static int empties(const State& state) {
    int result = 0;
    for (int board = 0; board < 9; ++board) {
      if (((state.closed >> board) & 1u) != 0) continue;
      const auto occupied =
          static_cast<uint16_t>(state.x[board] | state.o[board]);
      result += 9 - std::popcount(occupied);
    }
    return result;
  }

  static TTKey tt_key(const State& state) {
    return {state.key, zobrist_tag_full(state)};
  }

  static PosId pos_id(const State& state) {
    return {state.x, state.o, state.forced, state.tie};
  }

  static int chip_sign(const State&, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }

 private:
  static std::vector<ModelChild<State>> children(const State& state,
                                                 Seat mover) {
    MoveList moves;
    state.legal_moves(moves);
    std::vector<ModelChild<State>> result;
    result.reserve(moves.n);
    for (int i = 0; i < moves.n; ++i) {
      const Move move = moves.m[i];
      result.push_back({state.applied(move, mover).value(),
                        static_cast<uint8_t>(9 * move.board + move.cell)});
    }
    return result;
  }
};

static_assert(GameModel<UtttModel>);

}  // namespace uttt
