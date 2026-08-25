#include "core/position.hpp"

#include "core/local_table.hpp"

namespace uttt {
namespace {

bool completed(uint16_t mask) {
  for (uint16_t line : kWinLines) {
    if ((mask & line) == line) return true;
  }
  return false;
}

TieState tie_for(Seat seat) {
  return seat == Seat::X ? TieState::X : TieState::O;
}

}  // namespace

Position Position::initial() { return Position{}; }

TerminalKind Position::terminal() const {
  if (completed(macro_x)) return TerminalKind::MacroWinX;
  if (completed(macro_o)) return TerminalKind::MacroWinO;
  if (closed == 0x1FFu) return TerminalKind::AllClosed;
  return TerminalKind::None;
}

void Position::legal_moves(MoveList& out) const {
  out.n = 0;
  if (terminal() != TerminalKind::None) return;
  const auto& table = LocalTable::instance();
  auto append_board = [&](int b) {
    const uint16_t empty = table.empties(tern[b]);
    for (int c = 0; c < 9; ++c) {
      if ((empty >> c) & 1u) out.push({uint8_t(b), uint8_t(c)});
    }
  };
  if (forced != kForcedAny) {
    append_board(forced);
    return;
  }
  for (int b = 0; b < 9; ++b) {
    if (((closed >> b) & 1u) == 0) append_board(b);
  }
}

std::expected<Position, ApplyError> Position::applied(Move mv, Seat mover) const {
  if (terminal() != TerminalKind::None) return std::unexpected(ApplyError::TerminalParent);
  if (mv.board >= 9 || mv.cell >= 9) return std::unexpected(ApplyError::OutOfRange);
  if (forced != kForcedAny && forced != static_cast<int8_t>(mv.board)) {
    return std::unexpected(ApplyError::WrongForcedBoard);
  }
  if ((closed >> mv.board) & 1u) return std::unexpected(ApplyError::BoardClosed);
  const uint16_t bit = uint16_t(1u << mv.cell);
  if (((x[mv.board] | o[mv.board]) & bit) != 0) {
    return std::unexpected(ApplyError::CellOccupied);
  }

  Position next = *this;
  auto& marks = mover == Seat::X ? next.x : next.o;
  marks[mv.board] |= bit;
  next.tern[mv.board] = ternary_code(next.x[mv.board], next.o[mv.board]);
  const auto status = LocalTable::instance().status(next.tern[mv.board]);
  if (status != LocalStatus::Open) {
    next.closed |= uint16_t(1u << mv.board);
    if (status == LocalStatus::XWon) next.macro_x |= uint16_t(1u << mv.board);
    if (status == LocalStatus::OWon) next.macro_o |= uint16_t(1u << mv.board);
  }
  next.forced = ((next.closed >> mv.cell) & 1u) != 0
      ? kForcedAny
      : static_cast<int8_t>(mv.cell);
  next.tie = tie_for(opponent(mover));
  next.key = 0;
  return next;
}

bool Position::identity_equal(const Position& p) const {
  return x == p.x && o == p.o && forced == p.forced && tie == p.tie;
}

std::expected<Position, ImportError> Position::from_parts(
    const std::array<uint16_t, 9>& xs,
    const std::array<uint16_t, 9>& os,
    int8_t next_forced,
    TieState next_tie) {
  if (next_forced < kForcedAny || next_forced > 8) {
    return std::unexpected(ImportError::ForcedOutOfDomain);
  }
  if (static_cast<uint8_t>(next_tie) > static_cast<uint8_t>(TieState::NullFirstMove)) {
    return std::unexpected(ImportError::TieOutOfDomain);
  }

  Position p;
  p.x = xs;
  p.o = os;
  p.forced = next_forced;
  p.tie = next_tie;
  bool any_mark = false;
  const auto& table = LocalTable::instance();
  for (int b = 0; b < 9; ++b) {
    if ((p.x[b] & p.o[b]) != 0) return std::unexpected(ImportError::OverlappingMasks);
    if (((p.x[b] | p.o[b]) & ~0x1FFu) != 0) {
      return std::unexpected(ImportError::InvalidLocal);
    }
    any_mark = any_mark || p.x[b] != 0 || p.o[b] != 0;
    p.tern[b] = ternary_code(p.x[b], p.o[b]);
    if (!table.valid(p.tern[b])) return std::unexpected(ImportError::InvalidLocal);
    const LocalStatus status = table.status(p.tern[b]);
    if (status != LocalStatus::Open) p.closed |= uint16_t(1u << b);
    if (status == LocalStatus::XWon) p.macro_x |= uint16_t(1u << b);
    if (status == LocalStatus::OWon) p.macro_o |= uint16_t(1u << b);
  }
  if (p.forced != kForcedAny && ((p.closed >> p.forced) & 1u) != 0) {
    return std::unexpected(ImportError::ForcedToClosedBoard);
  }
  if (any_mark && p.tie == TieState::NullFirstMove) {
    return std::unexpected(ImportError::TieNullAfterFirstMark);
  }
  p.key = 0;
  return p;
}

}  // namespace uttt
