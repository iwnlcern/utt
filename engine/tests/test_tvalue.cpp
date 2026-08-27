#include "doctest/doctest.h"
#include "search/tvalue.hpp"
#include "support/test_rational.hpp"

#include <cmath>
#include <limits>

using namespace uttt;

namespace {

constexpr double kInf = std::numeric_limits<double>::infinity();

void check_encloses(const TestRational& exact, double lo, double hi) {
  CAPTURE(lo);
  CAPTURE(hi);
  CHECK(exact.inside(lo, hi));
}

}  // namespace

TEST_CASE("tvalue directed primitives step one ulp and enclose exact results") {
  const double x = 0.1;
  const double y = 0.2;
  const TestRational rx = TestRational::from_double(x);
  const TestRational ry = TestRational::from_double(y);

  const double add_lo = add_down(x, y);
  const double add_hi = add_up(x, y);
  CHECK(add_lo == std::nextafter(x + y, -kInf));
  CHECK(add_hi == std::nextafter(x + y, kInf));
  check_encloses(rx + ry, add_lo, add_hi);

  const double sub_lo = sub_down(x, y);
  const double sub_hi = sub_up(x, y);
  CHECK(sub_lo == std::nextafter(x - y, -kInf));
  CHECK(sub_hi == std::nextafter(x - y, kInf));
  check_encloses(rx - ry, sub_lo, sub_hi);

  const double mul_lo = mul_down(x, y);
  const double mul_hi = mul_up(x, y);
  CHECK(mul_lo == std::nextafter(x * y, -kInf));
  CHECK(mul_hi == std::nextafter(x * y, kInf));
  check_encloses(rx * ry, mul_lo, mul_hi);

  const double div_lo = div_down(x, y);
  const double div_hi = div_up(x, y);
  CHECK(div_lo == std::nextafter(x / y, -kInf));
  CHECK(div_hi == std::nextafter(x / y, kInf));
  check_encloses(rx / ry, div_lo, div_hi);
}

TEST_CASE("tvalue ordered backup encloses F(0, 0.5) within four ulps") {
  const TInterval result = f_backup({0.0, 0.0}, {0.5, 0.5});
  const double one_third = 1.0 / 3.0;
  const double ulp = std::nextafter(one_third, kInf) - one_third;

  CHECK(TestRational{1, 3}.inside(result.lo, result.hi));
  CHECK(width(result) <= 4.0 * ulp);
  CHECK(contains(result, one_third));
}

TEST_CASE("tvalue ordered backup encloses the Sterbenz boundary a = 0.5") {
  const TInterval result = f_backup({0.5, 0.5}, {0.5, 0.5});

  CHECK(TestRational{1, 2}.inside(result.lo, result.hi));
  CHECK(contains(result, 0.5));
}

TEST_CASE("tvalue ordered backup clips its interval to the unit range") {
  const TInterval result = f_backup({0.0, 1.0}, {0.0, 1.0});

  CHECK(result.lo == 0.0);
  CHECK(result.hi == 1.0);
  CHECK(0.0 <= result.lo);
  CHECK(result.lo <= result.hi);
  CHECK(result.hi <= 1.0);
  CHECK(width(result) == 1.0);
  CHECK_FALSE(contains(result, -std::numeric_limits<double>::denorm_min()));
  CHECK_FALSE(contains(result, std::nextafter(1.0, kInf)));
}

TEST_CASE("tvalue ordered backup preserves a finite subnormal boundary") {
  const double minimum = std::numeric_limits<double>::denorm_min();
  const TInterval result = f_backup({0.5, 1.0}, {minimum, minimum});

  CHECK(0.0 <= result.lo);
  CHECK(result.lo <= result.hi);
  CHECK(result.hi <= 1.0);
  CHECK(TestRational{1}.inside(result.lo, result.hi));
}
