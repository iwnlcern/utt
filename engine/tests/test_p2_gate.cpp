#include "doctest/doctest.h"
#include "root/p2_gate.hpp"
#include "support/test_rational.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <random>
#include <stdexcept>

using namespace uttt;

namespace {

RootClass exact_p2_classify(TInterval t, int64_t bx, int64_t total,
                            int empties) {
  const TestRational budget{bx};
  const TestRational margin{empties};
  const TestRational scale{total};
  if (budget - margin > TestRational::from_double(t.hi) * scale) {
    return XForced;
  }
  if (TestRational::from_double(t.lo) * scale - budget > margin) {
    return OForced;
  }
  return InBand;
}

}  // namespace

TEST_CASE("A7 P2 exact margins are strict at M 1e9") {
  constexpr int64_t kM = 1'000'000'000;
  constexpr int kEmpties = 81;
  const TInterval half{0.5, 0.5};

  CHECK(p2_classify(half, 500'000'081, kM, kEmpties) == InBand);
  CHECK(p2_classify(half, 500'000'082, kM, kEmpties) == XForced);
  CHECK(p2_classify(half, 499'999'919, kM, kEmpties) == InBand);
  CHECK(p2_classify(half, 499'999'918, kM, kEmpties) == OForced);
}

TEST_CASE("A7 P2 binary64 1e-9 cliff does not understate the upper endpoint") {
  CHECK(p2_classify({0.0, 1e-9}, 1, 1'000'000'000, 0) == InBand);
}

TEST_CASE("A7 P2 uses the adverse endpoint independently on each side") {
  CHECK(p2_classify({0.4, 0.5}, 450'000'081, 1'000'000'000, 81) ==
        InBand);
  CHECK(p2_classify({0.5, 0.6}, 549'999'919, 1'000'000'000, 81) ==
        InBand);
}

TEST_CASE("A7 P2 literal zero endpoint branches preserve their asymmetry") {
  CHECK(p2_classify({0.0, 0.0}, 82, 1'000'000'000, 81) == XForced);
  CHECK(p2_classify({0.0, 0.5}, 0, 1'000'000'000, 0) == InBand);
}

TEST_CASE("A7 P2 literal one endpoint branches preserve their asymmetry") {
  CHECK(p2_classify({0.0, 1.0}, 1'000'000'000, 1'000'000'000, 0) ==
        InBand);
  CHECK(p2_classify({1.0, 1.0}, 0, 1'000'000'000, 0) == OForced);
}

TEST_CASE("A7 P2 X shift overflow is decided by positive magnitude") {
  const double subnormal = std::numeric_limits<double>::denorm_min();
  CHECK(p2_classify({subnormal, subnormal}, 1, 1'000'000'000, 0) ==
        XForced);
}

TEST_CASE("A7 P2 O shift overflow cannot force classification") {
  const double subnormal = std::numeric_limits<double>::denorm_min();
  CHECK(p2_classify({subnormal, 1.0}, 0, 1'000'000'000, 1) == InBand);
}

TEST_CASE("A7 P2 unresolved interval takes the in-band fallback") {
  CHECK(p2_classify({0.4, 0.6}, 500'000'000, 1'000'000'000, 81) ==
        InBand);
}

TEST_CASE("A7 P2 matches exact TestRational arithmetic on 200 seeded cases") {
  std::mt19937_64 rng{0xA7C411ULL};
  int x_forced_count = 0;
  int o_forced_count = 0;
  int in_band_count = 0;
  for (int index = 0; index < 200; ++index) {
    const auto unit = [&]() {
      return static_cast<double>(rng() >> 11) * 0x1.0p-53;
    };
    const int64_t total =
        1'000 + static_cast<int64_t>(rng() % 999'999'001);
    const int empties = static_cast<int>(rng() % 82);
    double lo = 0.0;
    double hi = 0.0;
    int64_t bx = 0;
    if (index % 3 == 0) {
      lo = unit() * 0.25;
      hi = unit() * 0.25;
      if (hi < lo) std::swap(lo, hi);
      bx = total;
    } else if (index % 3 == 1) {
      lo = 0.75 + unit() * 0.25;
      hi = 0.75 + unit() * 0.25;
      if (hi < lo) std::swap(lo, hi);
      bx = 0;
    } else {
      lo = unit() * 0.25;
      hi = 0.75 + unit() * 0.25;
      bx = total / 2;
    }
    const TInterval interval{lo, hi};

    CAPTURE(index);
    CAPTURE(lo);
    CAPTURE(hi);
    CAPTURE(bx);
    CAPTURE(total);
    CAPTURE(empties);
    const RootClass expected = exact_p2_classify(interval, bx, total, empties);
    CHECK(p2_classify(interval, bx, total, empties) == expected);
    if (expected == XForced) ++x_forced_count;
    if (expected == OForced) ++o_forced_count;
    if (expected == InBand) ++in_band_count;
  }
  CHECK(x_forced_count == 67);
  CHECK(o_forced_count == 67);
  CHECK(in_band_count == 66);
}

TEST_CASE("P2 gate rejects inputs outside the locked UTTT domain") {
  constexpr int64_t kTooLarge = int64_t{1} << 32;
  const double nan = std::numeric_limits<double>::quiet_NaN();
  const double inf = std::numeric_limits<double>::infinity();

  CHECK_THROWS_AS(p2_classify({0.6, 0.4}, 1, 2, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({-0.1, 0.4}, 1, 2, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 1.1}, 1, 2, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({nan, 0.4}, 1, 2, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, inf}, 1, 2, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 0.6}, 0, -1, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 0.6}, 0, kTooLarge, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 0.6}, -1, 10, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 0.6}, 11, 10, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 0.6}, 1, 10, -1),
                  std::invalid_argument);
  CHECK_THROWS_AS(p2_classify({0.4, 0.6}, 1, 10, 82),
                  std::invalid_argument);
}

TEST_CASE("P2 gate keeps canonical zero-total play in band") {
  const double subnormal = std::numeric_limits<double>::denorm_min();
  CHECK(p2_classify({0.0, 0.0}, 0, 0, 0) == InBand);
  CHECK(p2_classify({0.0, 1.0}, 0, 0, 0) == InBand);
  CHECK(p2_classify({1.0, 1.0}, 0, 0, 0) == InBand);
  CHECK(p2_classify({subnormal, subnormal}, 0, 0, 0) == InBand);
  CHECK(p2_classify({0.25, 0.75}, 0, 0, 81) == InBand);
  CHECK(p2_classify({-0.0, 0.0}, 0, 0, 81) == InBand);
}

TEST_CASE("P2 gate accepts signed zero and the maximum locked budget") {
  constexpr int64_t kMaxM = (int64_t{1} << 32) - 1;
  CHECK(p2_classify({-0.0, 0.0}, 1, kMaxM, 0) == XForced);
  CHECK(p2_classify({1.0, 1.0}, kMaxM, kMaxM, 81) == InBand);
}
