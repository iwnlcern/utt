#include "doctest/doctest.h"
#include "core/local_table.hpp"
#include "core/naive_local.hpp"

using namespace uttt;

TEST_CASE("exhaustive table proof vs naive oracle") {
  const auto& t = LocalTable::instance();
  for (uint32_t code = 0; code < 19683; ++code) {
    uint16_t x = 0;
    uint16_t o = 0;
    uint32_t tmp = code;
    for (int c = 0; c < 9; ++c) {
      const auto v = tmp % 3;
      tmp /= 3;
      if (v == 1) x |= uint16_t(1u << c);
      if (v == 2) o |= uint16_t(1u << c);
    }
    const auto n = naive::eval_local(x, o);
    CAPTURE(code);
    REQUIRE(t.valid(code) == n.valid);
    REQUIRE(t.status(code) == n.status);
    REQUIRE(t.empties(code) == n.empties);
    REQUIRE(t.win(code, Seat::X) == n.win_x);
    REQUIRE(t.win(code, Seat::O) == n.win_o);
    REQUIRE(t.fork(code, Seat::X) == n.fork_x);
    REQUIRE(t.fork(code, Seat::O) == n.fork_o);
  }
}

TEST_CASE("ternary code round-trip") {
  CHECK(ternary_code(0, 0) == 0);
  CHECK(ternary_code(1, 0) == 1);
  CHECK(ternary_code(0, 1) == 2);
  CHECK(ternary_code(0b10, 0b01) == 2 + 3);
}
