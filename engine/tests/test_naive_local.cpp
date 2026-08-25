#include "doctest/doctest.h"
#include "core/naive_local.hpp"

using namespace uttt;

static constexpr uint16_t rowmask(int r) {
  return uint16_t(0b111u << (3 * r));
}

TEST_CASE("naive local eval") {
  auto empty = naive::eval_local(0, 0);
  CHECK(empty.valid);
  CHECK(empty.status == LocalStatus::Open);
  CHECK(empty.empties == 0x1FF);
  CHECK(empty.win_x == 0);

  auto onewin = naive::eval_local(rowmask(0), 0b110000000);
  CHECK(onewin.valid);
  CHECK(onewin.status == LocalStatus::XWon);
  CHECK(onewin.empties == 0);

  auto dual = naive::eval_local(rowmask(0), rowmask(1));
  CHECK_FALSE(dual.valid);
  CHECK(dual.status == LocalStatus::FullDraw);
  CHECK(dual.empties == 0);
  CHECK(dual.win_x == 0);
  CHECK(dual.fork_o == 0);

  auto disjoint = naive::eval_local(uint16_t(rowmask(0) | rowmask(1)), 0);
  CHECK_FALSE(disjoint.valid);

  auto shared = naive::eval_local(0b001001111, 0);
  CHECK(shared.valid);
  CHECK(shared.status == LocalStatus::XWon);

  auto near = naive::eval_local(0b000000011, 0);
  CHECK((near.win_x & 0b000000100) != 0);

  auto forky = naive::eval_local(0b000000101, 0);
  CHECK((forky.fork_x & 0b000010000) != 0);

  auto draw = naive::eval_local(0b011100011, 0b100011100);
  CHECK(draw.valid);
  CHECK(draw.status == LocalStatus::FullDraw);
}
