#include "doctest/doctest.h"
#include "core/position.hpp"
#include "core/zobrist.hpp"

#include <array>
#include <cstdint>

using namespace uttt;

namespace {
uint64_t next_random(uint64_t& state) {
  state ^= state << 13;
  state ^= state >> 7;
  state ^= state << 17;
  return state;
}
}  // namespace

TEST_CASE("incremental key equals full recompute along random games") {
  uint64_t rng = 0xABCDEF0123456789ULL;
  for (int game = 0; game < 2000; ++game) {
    auto p = Position::initial();
    while (true) {
      REQUIRE(p.key == zobrist_full(p));
      MoveList moves;
      p.legal_moves(moves);
      if (moves.n == 0) break;
      const Move mv = moves.m[next_random(rng) % moves.n];
      const Seat mover = (next_random(rng) & 1u) != 0 ? Seat::X : Seat::O;
      p = p.applied(mv, mover).value();
    }
  }
}

TEST_CASE("single-input isolation for both hashes over full population") {
  std::array<uint16_t, 9> zx{};
  std::array<uint16_t, 9> zo{};
  auto base = Position::from_parts(zx, zo, 4, TieState::O).value();
  for (int b = 0; b < 9; ++b) {
    for (int c = 0; c < 9; ++c) {
      auto mx = zx;
      mx[b] = uint16_t(1u << c);
      auto px = Position::from_parts(mx, zo, 4, TieState::O).value();
      auto mo = zo;
      mo[b] = uint16_t(1u << c);
      auto po = Position::from_parts(zx, mo, 4, TieState::O).value();
      CHECK(px.key != base.key);
      CHECK(po.key != base.key);
      CHECK(zobrist_tag_full(px) != zobrist_tag_full(base));
      CHECK(zobrist_tag_full(po) != zobrist_tag_full(base));
      CHECK(px.key != po.key);
      CHECK(zobrist_tag_full(px) != zobrist_tag_full(po));
    }
  }

  std::array<uint64_t, 10> keys{};
  std::array<uint32_t, 10> tags{};
  for (int f = 0; f < 9; ++f) {
    auto p = Position::from_parts(zx, zo, int8_t(f), TieState::O).value();
    keys[f] = p.key;
    tags[f] = zobrist_tag_full(p);
  }
  auto any = Position::from_parts(zx, zo, kForcedAny, TieState::O).value();
  keys[9] = any.key;
  tags[9] = zobrist_tag_full(any);
  for (int i = 0; i < 10; ++i) {
    for (int j = i + 1; j < 10; ++j) {
      CHECK(keys[i] != keys[j]);
      CHECK(tags[i] != tags[j]);
    }
  }

  auto tn = Position::from_parts(zx, zo, 4, TieState::NullFirstMove).value();
  auto tx = Position::from_parts(zx, zo, 4, TieState::X).value();
  auto to = Position::from_parts(zx, zo, 4, TieState::O).value();
  CHECK(tn.key != tx.key);
  CHECK(tn.key != to.key);
  CHECK(tx.key != to.key);
  CHECK(zobrist_tag_full(tn) != zobrist_tag_full(tx));
  CHECK(zobrist_tag_full(tn) != zobrist_tag_full(to));
  CHECK(zobrist_tag_full(tx) != zobrist_tag_full(to));
}
