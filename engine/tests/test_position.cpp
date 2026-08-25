#include "doctest/doctest.h"
#include "core/position.hpp"

using namespace uttt;

static Position make_macro_win_x() {
  auto p = Position::initial();
  auto step = [&](Move m) { p = p.applied(m, Seat::X).value(); };
  step({4, 0}); REQUIRE(p.forced == 0);
  step({0, 0}); REQUIRE(p.forced == 0);
  step({0, 1}); REQUIRE(p.forced == 1);
  step({1, 0}); REQUIRE(p.forced == 0);
  step({0, 2}); REQUIRE(((p.closed >> 0) & 1) != 0); REQUIRE(p.forced == 2);
  step({2, 1}); REQUIRE(p.forced == 1);
  step({1, 1}); REQUIRE(p.forced == 1);
  step({1, 2}); REQUIRE(((p.closed >> 1) & 1) != 0); REQUIRE(p.forced == 2);
  step({2, 0}); REQUIRE(p.forced == kForcedAny);
  step({2, 2});
  return p;
}

TEST_CASE("initial state") {
  auto p = Position::initial();
  CHECK(p.forced == 4);
  CHECK(p.tie == TieState::NullFirstMove);
  MoveList ml;
  p.legal_moves(ml);
  CHECK(ml.n == 9);
  for (int i = 0; i < ml.n; ++i) CHECK(ml.m[i].board == 4);
}

TEST_CASE("apply validation") {
  auto p = Position::initial();
  CHECK(p.applied({3, 0}, Seat::X).error() == ApplyError::WrongForcedBoard);
  CHECK(p.applied({4, 9}, Seat::X).error() == ApplyError::OutOfRange);
  auto q = p.applied({4, 4}, Seat::X).value();
  CHECK(q.applied({4, 4}, Seat::O).error() == ApplyError::CellOccupied);
  CHECK(q.forced == 4);
  CHECK(q.tie == TieState::O);
  CHECK(p.tie == TieState::NullFirstMove);
}

TEST_CASE("closure, routing, closed-board ANY") {
  auto p = Position::initial();
  p = p.applied({4, 0}, Seat::X).value();
  p = p.applied({0, 4}, Seat::O).value();
  p = p.applied({4, 1}, Seat::X).value();
  p = p.applied({1, 4}, Seat::O).value();
  p = p.applied({4, 2}, Seat::X).value();
  CHECK(((p.closed >> 4) & 1) != 0);
  CHECK(((p.macro_x >> 4) & 1) != 0);
  CHECK(p.forced == 2);
  p = p.applied({2, 4}, Seat::O).value();
  CHECK(p.forced == kForcedAny);
  MoveList ml;
  p.legal_moves(ml);
  for (int i = 0; i < ml.n; ++i) CHECK(((p.closed >> ml.m[i].board) & 1) == 0);
}

TEST_CASE("terminal totality: macro win") {
  auto t = make_macro_win_x();
  CHECK(t.terminal() == TerminalKind::MacroWinX);
  MoveList ml;
  t.legal_moves(ml);
  CHECK(ml.n == 0);
  CHECK(t.applied({8, 8}, Seat::O).error() == ApplyError::TerminalParent);
}

TEST_CASE("terminal totality: all-closed mechanical fixture") {
  std::array<uint16_t, 9> ax{};
  std::array<uint16_t, 9> ao{};
  ax.fill(0b011100011);
  ao.fill(0b100011100);
  auto t = Position::from_parts(ax, ao, kForcedAny, TieState::X).value();
  CHECK(t.terminal() == TerminalKind::AllClosed);
  MoveList ml;
  t.legal_moves(ml);
  CHECK(ml.n == 0);
  CHECK(t.applied({0, 0}, Seat::X).error() == ApplyError::TerminalParent);
}

TEST_CASE("mechanical import rejects locked inconsistencies") {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  x[0] = 1;
  o[0] = 1;
  CHECK(Position::from_parts(x, o, kForcedAny, TieState::X).error() ==
        ImportError::OverlappingMasks);
  x = {};
  o = {};
  CHECK(Position::from_parts(x, o, 10, TieState::X).error() ==
        ImportError::ForcedOutOfDomain);
  x[0] = 1;
  CHECK(Position::from_parts(x, o, kForcedAny, TieState::NullFirstMove).error() ==
        ImportError::TieNullAfterFirstMark);
}
