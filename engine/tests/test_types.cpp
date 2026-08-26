#include "doctest/doctest.h"
#include "core/types.hpp"
#include "core/budget.hpp"
#include "core/clock.hpp"

using namespace uttt;

TEST_CASE("types basics") {
  CHECK(opponent(Seat::X) == Seat::O);
  CHECK(opponent(Seat::O) == Seat::X);
  MoveList ml;
  ml.push({4, 2});
  CHECK(ml.n == 1);
  CHECK(ml.m[0] == Move{4, 2});
  CHECK(kWinLines.size() == 8);
}

TEST_CASE("all-closed chip resolution: production seam, all three margins") {
  CHECK(winner_on_chips(500000001, 499999999) == ChipResult::XWins);
  CHECK(winner_on_chips(499999999, 500000001) == ChipResult::OWins);
  CHECK(winner_on_chips(1, 1) == ChipResult::Draw);
  CHECK(winner_on_chips(0, 0) == ChipResult::Draw);
}

TEST_CASE("fake clock contract") {
  FakeClock c;
  CHECK(c.now_ms() == 0);
  c.t = 27000;
  CHECK(c.now_ms() == 27000);
}
