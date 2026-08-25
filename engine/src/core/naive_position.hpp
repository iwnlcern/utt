#pragma once

#include <array>
#include <cstdint>

#include "core/naive_local.hpp"

namespace uttt::naive {

struct RefPosition {
  std::array<uint8_t, 81> cells{};
  int8_t forced = 4;
  TieState tie = TieState::NullFirstMove;

  static RefPosition initial() { return RefPosition{}; }

  void local_masks(int board, uint16_t& x, uint16_t& o) const {
    x = 0;
    o = 0;
    for (int c = 0; c < 9; ++c) {
      if (cells[9 * board + c] == 1) x |= uint16_t(1u << c);
      if (cells[9 * board + c] == 2) o |= uint16_t(1u << c);
    }
  }

  LocalStatus local_status(int board) const {
    uint16_t x = 0;
    uint16_t o = 0;
    local_masks(board, x, o);
    return eval_local(x, o).status;
  }

  static bool completed(uint16_t mask) {
    for (uint16_t line : kWinLines) {
      if ((mask & line) == line) return true;
    }
    return false;
  }

  TerminalKind terminal() const {
    uint16_t macro_x = 0;
    uint16_t macro_o = 0;
    uint16_t closed = 0;
    for (int b = 0; b < 9; ++b) {
      const auto status = local_status(b);
      if (status != LocalStatus::Open) closed |= uint16_t(1u << b);
      if (status == LocalStatus::XWon) macro_x |= uint16_t(1u << b);
      if (status == LocalStatus::OWon) macro_o |= uint16_t(1u << b);
    }
    if (completed(macro_x)) return TerminalKind::MacroWinX;
    if (completed(macro_o)) return TerminalKind::MacroWinO;
    if (closed == 0x1FFu) return TerminalKind::AllClosed;
    return TerminalKind::None;
  }

  void legal_moves(MoveList& out) const {
    out.n = 0;
    if (terminal() != TerminalKind::None) return;
    auto append = [&](int board) {
      if (local_status(board) != LocalStatus::Open) return;
      for (int cell = 0; cell < 9; ++cell) {
        if (cells[9 * board + cell] == 0) {
          out.push({uint8_t(board), uint8_t(cell)});
        }
      }
    };
    if (forced != kForcedAny) {
      append(forced);
      return;
    }
    for (int board = 0; board < 9; ++board) append(board);
  }

  RefPosition applied_unchecked(Move mv, Seat mover) const {
    RefPosition next = *this;
    next.cells[9 * mv.board + mv.cell] = mover == Seat::X ? 1 : 2;
    next.forced = next.local_status(mv.cell) == LocalStatus::Open
        ? static_cast<int8_t>(mv.cell)
        : kForcedAny;
    next.tie = mover == Seat::X ? TieState::O : TieState::X;
    return next;
  }
};

}  // namespace uttt::naive
