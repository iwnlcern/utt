#pragma once

#include <array>
#include <cstdint>
#include <expected>

#include "core/types.hpp"

namespace uttt {

enum class ImportError : uint8_t {
  OverlappingMasks,
  InvalidLocal,
  ForcedOutOfDomain,
  ForcedToClosedBoard,
  TieOutOfDomain,
  TieNullAfterFirstMark
};

struct Position {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  std::array<uint32_t, 9> tern{};
  uint16_t macro_x = 0;
  uint16_t macro_o = 0;
  uint16_t closed = 0;
  int8_t forced = 4;
  TieState tie = TieState::NullFirstMove;
  uint64_t key = 0;

  static Position initial();
  TerminalKind terminal() const;
  void legal_moves(MoveList& out) const;
  std::expected<Position, ApplyError> applied(Move mv, Seat mover) const;
  bool identity_equal(const Position& p) const;

  static std::expected<Position, ImportError> from_parts(
      const std::array<uint16_t, 9>& x,
      const std::array<uint16_t, 9>& o,
      int8_t forced,
      TieState tie);
};

}  // namespace uttt
