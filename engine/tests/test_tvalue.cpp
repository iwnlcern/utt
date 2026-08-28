#include "doctest/doctest.h"
#include "search/tvalue.hpp"
#include "support/test_rational.hpp"

#include <algorithm>
#include <array>
#include <cfenv>
#include <cmath>
#include <cstdint>
#include <limits>
#include <random>

using namespace uttt;

namespace {

constexpr double kInf = std::numeric_limits<double>::infinity();

void check_encloses(const TestRational& exact, double lo, double hi) {
  CAPTURE(lo);
  CAPTURE(hi);
  CHECK(exact.inside(lo, hi));
}

TestRational exact_f(double a, double b) {
  const TestRational one{1};
  const TestRational ra = TestRational::from_double(a);
  const TestRational rb = TestRational::from_double(b);
  return rb / (one - ra + rb);
}

TestRational exact_x_preimage(double t, double b) {
  const TestRational one{1};
  const TestRational rt = TestRational::from_double(t);
  const TestRational rb = TestRational::from_double(b);
  return one - rb * (one - rt) / rt;
}

TestRational exact_o_preimage(double t, double a) {
  const TestRational one{1};
  const TestRational rt = TestRational::from_double(t);
  const TestRational ra = TestRational::from_double(a);
  return rt * (one - ra) / (one - rt);
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

TEST_CASE("tvalue A5 X preimage encloses the named cancellation witness") {
  const TInterval result = x_preimage({0.2, 0.2}, {0.25, 0.25});

  check_encloses(TestRational{1, 14411518807585588}, result.lo, result.hi);
  CHECK(result.lo == 0.0);
  CHECK(result.hi > 0.0);
}

TEST_CASE("tvalue A5 preimages use crossed interval endpoints") {
  const TInterval x_result = x_preimage({0.5, 0.75}, {0.25, 0.5});
  check_encloses(TestRational{1, 2}, x_result.lo, x_result.hi);
  check_encloses(TestRational{11, 12}, x_result.lo, x_result.hi);

  const TInterval o_result = o_preimage({0.25, 0.5}, {0.25, 0.5});
  check_encloses(TestRational{1, 6}, o_result.lo, o_result.hi);
  check_encloses(TestRational{3, 4}, o_result.lo, o_result.hi);
}

TEST_CASE("tvalue A5 singular point windows return full preimages without FP exceptions") {
  std::feclearexcept(FE_ALL_EXCEPT);

  const TInterval x_result = x_preimage({0.0, 0.0}, {0.25, 0.75});
  const TInterval o_result = o_preimage({1.0, 1.0}, {0.25, 0.75});

  CHECK(x_result.lo == 0.0);
  CHECK(x_result.hi == 1.0);
  CHECK(o_result.lo == 0.0);
  CHECK(o_result.hi == 1.0);
  CHECK(std::fetestexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW) == 0);
}

TEST_CASE("tvalue A5 endpoint guards are one-sided") {
  const TInterval x_result = x_preimage({0.0, 0.5}, {0.25, 0.5});
  CHECK(x_result.lo == 0.0);
  CHECK(x_result.hi < 1.0);
  check_encloses(TestRational{3, 4}, x_result.lo, x_result.hi);

  const TInterval o_result = o_preimage({0.5, 1.0}, {0.25, 0.5});
  CHECK(o_result.lo > 0.0);
  CHECK(o_result.hi == 1.0);
  check_encloses(TestRational{1, 2}, o_result.lo, o_result.hi);
}

TEST_CASE("tvalue A5 preimages clip results to the unit interval") {
  const TInterval x_result = x_preimage({0.25, 0.25}, {1.0, 1.0});
  const TInterval o_result = o_preimage({0.75, 0.75}, {0.0, 0.0});

  CHECK(x_result.lo == 0.0);
  CHECK(x_result.hi == 0.0);
  CHECK(o_result.lo == 1.0);
  CHECK(o_result.hi == 1.0);
}

TEST_CASE("tvalue A5 named F_down rounding direction encloses 2/1025") {
  const TInterval result = f_backup(
      {1.0 / 1024.0, 1.0 / 1024.0},
      {2.0 / 1024.0, 2.0 / 1024.0});

  check_encloses(TestRational{2, 1025}, result.lo, result.hi);
}

TEST_CASE("tvalue A5 seeded 500-case rational composites enclose exact endpoints") {
  constexpr int kScale = 1024;
  constexpr std::uint32_t kSeed = 0xA5C4'500u;
  std::mt19937 random(kSeed);
  std::uniform_int_distribution<int> unit(0, kScale);
  std::uniform_int_distribution<int> lower_half(1, kScale / 2);
  std::uniform_int_distribution<int> upper_half(kScale / 2, kScale - 1);

  for (int case_index = 0; case_index < 500; ++case_index) {
    CAPTURE(case_index);

    std::array<int, 4> ordered{unit(random), unit(random), unit(random), unit(random)};
    std::sort(ordered.begin(), ordered.end());
    const TInterval a{
        static_cast<double>(ordered[0]) / kScale,
        static_cast<double>(ordered[1]) / kScale,
    };
    const TInterval b{
        static_cast<double>(ordered[2]) / kScale,
        static_cast<double>(ordered[3]) / kScale,
    };
    const TInterval f_result = f_backup(a, b);
    check_encloses(exact_f(a.lo, b.lo), f_result.lo, f_result.hi);
    check_encloses(exact_f(a.hi, b.hi), f_result.lo, f_result.hi);

    std::array<int, 2> x_t_values{upper_half(random), upper_half(random)};
    std::array<int, 2> x_b_values{unit(random), unit(random)};
    std::sort(x_t_values.begin(), x_t_values.end());
    std::sort(x_b_values.begin(), x_b_values.end());
    const TInterval x_t{
        static_cast<double>(x_t_values[0]) / kScale,
        static_cast<double>(x_t_values[1]) / kScale,
    };
    const TInterval x_b{
        static_cast<double>(x_b_values[0]) / kScale,
        static_cast<double>(x_b_values[1]) / kScale,
    };
    const TInterval x_result = x_preimage(x_t, x_b);
    check_encloses(exact_x_preimage(x_t.lo, x_b.hi), x_result.lo, x_result.hi);
    check_encloses(exact_x_preimage(x_t.hi, x_b.lo), x_result.lo, x_result.hi);

    std::array<int, 2> o_t_values{lower_half(random), lower_half(random)};
    std::array<int, 2> o_a_values{unit(random), unit(random)};
    std::sort(o_t_values.begin(), o_t_values.end());
    std::sort(o_a_values.begin(), o_a_values.end());
    const TInterval o_t{
        static_cast<double>(o_t_values[0]) / kScale,
        static_cast<double>(o_t_values[1]) / kScale,
    };
    const TInterval o_a{
        static_cast<double>(o_a_values[0]) / kScale,
        static_cast<double>(o_a_values[1]) / kScale,
    };
    const TInterval o_result = o_preimage(o_t, o_a);
    check_encloses(exact_o_preimage(o_t.lo, o_a.hi), o_result.lo, o_result.hi);
    check_encloses(exact_o_preimage(o_t.hi, o_a.lo), o_result.lo, o_result.hi);

    CHECK(0.0 <= f_result.lo);
    CHECK(f_result.lo <= f_result.hi);
    CHECK(f_result.hi <= 1.0);
    CHECK(0.0 <= x_result.lo);
    CHECK(x_result.lo <= x_result.hi);
    CHECK(x_result.hi <= 1.0);
    CHECK(0.0 <= o_result.lo);
    CHECK(o_result.lo <= o_result.hi);
    CHECK(o_result.hi <= 1.0);
  }
}
