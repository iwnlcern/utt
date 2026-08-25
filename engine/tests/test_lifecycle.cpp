#include "doctest/doctest.h"
#include "core/local_table.hpp"
#include "core/naive_local.hpp"
#include "core/naive_position.hpp"
#include "core/position.hpp"

#include <algorithm>
#include <cstdint>

using namespace uttt;

namespace {

uint64_t next_random(uint64_t& state) {
  state ^= state << 13;
  state ^= state >> 7;
  state ^= state << 17;
  return state;
}

bool move_less(Move a, Move b) {
  return a.board < b.board || (a.board == b.board && a.cell < b.cell);
}

void require_same_moves(MoveList a, MoveList b, int game, int ply) {
  CAPTURE(game);
  CAPTURE(ply);
  REQUIRE(a.n == b.n);
  std::sort(a.m.begin(), a.m.begin() + a.n, move_less);
  std::sort(b.m.begin(), b.m.begin() + b.n, move_less);
  for (int i = 0; i < a.n; ++i) REQUIRE(a.m[i] == b.m[i]);
}

}  // namespace

TEST_CASE("randomized optimized/reference lifecycle equivalence") {
  uint64_t rng = 0xD1CEB00C5EED1234ULL;
  for (int game = 0; game < 2000; ++game) {
    auto p = Position::initial();
    auto ref = naive::RefPosition::initial();
    int ply = 0;
    while (true) {
      CAPTURE(game);
      CAPTURE(ply);
      REQUIRE(p.terminal() == ref.terminal());
      MoveList fast_moves;
      MoveList ref_moves;
      p.legal_moves(fast_moves);
      ref.legal_moves(ref_moves);
      require_same_moves(fast_moves, ref_moves, game, ply);

      uint16_t macro_x = 0;
      uint16_t macro_o = 0;
      uint16_t closed = 0;
      for (int b = 0; b < 9; ++b) {
        REQUIRE(p.tern[b] == ternary_code(p.x[b], p.o[b]));
        const auto local = naive::eval_local(p.x[b], p.o[b]);
        if (local.status != LocalStatus::Open) closed |= uint16_t(1u << b);
        if (local.status == LocalStatus::XWon) macro_x |= uint16_t(1u << b);
        if (local.status == LocalStatus::OWon) macro_o |= uint16_t(1u << b);
      }
      REQUIRE(p.closed == closed);
      REQUIRE(p.macro_x == macro_x);
      REQUIRE(p.macro_o == macro_o);
      if (ply == 0) REQUIRE(p.tie == TieState::NullFirstMove);

      if (fast_moves.n == 0) break;
      const Move mv = fast_moves.m[next_random(rng) % fast_moves.n];
      const Seat mover = (next_random(rng) & 1u) != 0 ? Seat::X : Seat::O;
      const Position parent = p;
      auto child = p.applied(mv, mover);
      REQUIRE(child.has_value());
      REQUIRE(p.identity_equal(parent));
      p = child.value();
      ref = ref.applied_unchecked(mv, mover);
      REQUIRE(p.tie == (mover == Seat::X ? TieState::O : TieState::X));
      ++ply;
    }
  }
}
