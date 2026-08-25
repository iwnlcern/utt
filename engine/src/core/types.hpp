#pragma once

#include <array>
#include <cassert>
#include <cstdint>

namespace uttt {

enum class Seat : uint8_t { X = 0, O = 1 };
constexpr Seat opponent(Seat s) { return s == Seat::X ? Seat::O : Seat::X; }

enum class TieState : uint8_t { X = 0, O = 1, NullFirstMove = 2 };
enum class LocalStatus : uint8_t { Open = 0, XWon = 1, OWon = 2, FullDraw = 3 };
enum class TerminalKind : uint8_t { None, MacroWinX, MacroWinO, AllClosed };
enum class ApplyError : uint8_t {
  TerminalParent,
  CellOccupied,
  BoardClosed,
  WrongForcedBoard,
  OutOfRange
};

struct Move {
  uint8_t board;
  uint8_t cell;
  bool operator==(const Move&) const = default;
};

struct MoveList {
  std::array<Move, 81> m{};
  uint8_t n = 0;
  void push(Move mv) {
    assert(n < m.size());
    m[n++] = mv;
  }
};

inline constexpr int8_t kForcedAny = -1;

// Cell bit c is row-major: bit 0 is r0c0 and bit 8 is r2c2.
inline constexpr std::array<uint16_t, 8> kWinLines = {
    0b111000000, 0b000111000, 0b000000111,
    0b100100100, 0b010010010, 0b001001001,
    0b100010001, 0b001010100};

}  // namespace uttt
