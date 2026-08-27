#include "doctest/doctest.h"
#include "support/test_rational.hpp"

#include <cstdint>
#include <limits>

TEST_CASE("rational from_double preserves the exact binary64 value") {
  CHECK(TestRational::from_double(0.2) ==
        TestRational{3602879701896397LL, 18014398509481984LL});
  CHECK(TestRational::from_double(-0.5) == TestRational{-1, 2});
  CHECK(TestRational::from_double(-0.0) == TestRational{0});
}

TEST_CASE("rational arithmetic and ordering remain exact beyond int64") {
  const TestRational a{2, 3};
  const TestRational b{-5, 7};

  CHECK(a + b == TestRational{-1, 21});
  CHECK(a - b == TestRational{29, 21});
  CHECK(a * b == TestRational{-10, 21});
  CHECK(a / b == TestRational{-14, 15});
  CHECK(b < a);
  CHECK(a > TestRational{1, 2});
  CHECK(TestRational{2, -4} == TestRational{-1, 2});

  const int64_t max = std::numeric_limits<int64_t>::max();
  const TestRational large{max, max - 1};
  const TestRational square = large * large;
  CHECK(square / large == large);
  CHECK(square > large);
}

TEST_CASE("rational half_up_int uses floor semantics for signed ties") {
  CHECK(half_up_int(TestRational{1, 2}) == 1);
  CHECK(half_up_int(TestRational{-1, 2}) == 0);
  CHECK(half_up_int(TestRational{3, 2}) == 2);
  CHECK(half_up_int(TestRational{-3, 2}) == -1);
}

TEST_CASE("rational inside compares exact binary64 endpoints") {
  CHECK(TestRational{1, 3}.inside(0.3, 0.4));
  CHECK_FALSE(TestRational{1, 3}.inside(0.34, 0.4));

  const TestRational binary_point_two = TestRational::from_double(0.2);
  CHECK(binary_point_two.inside(0.2, 0.2));
  CHECK_FALSE(TestRational{1, 5}.inside(0.2, 0.2));
  CHECK(TestRational{1, 5}.inside(0.19999999999999998, 0.2));
}
