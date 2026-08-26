#include "doctest/doctest.h"
#include "core/position.hpp"

#include <cstdint>

using namespace uttt;

namespace {

uint64_t perft(const Position& p, int depth) {
  if (depth == 0 || p.terminal() != TerminalKind::None) return 1;
  MoveList moves;
  p.legal_moves(moves);
  uint64_t nodes = 0;
  for (int i = 0; i < moves.n; ++i) {
    nodes += perft(p.applied(moves.m[i], Seat::X).value(), depth - 1);
    nodes += perft(p.applied(moves.m[i], Seat::O).value(), depth - 1);
  }
  return nodes;
}

}  // namespace

TEST_CASE("both-mover perft regression pins") {
  const auto p = Position::initial();
  CHECK(perft(p, 1) == 18);
  // Regression pins generated once from this implementation's both-mover
  // expansion; they detect movegen/apply drift rather than assert external truth.
  CHECK(perft(p, 2) == 320);
  CHECK(perft(p, 3) == 5632);
  CHECK(perft(p, 4) == 97920);
}
