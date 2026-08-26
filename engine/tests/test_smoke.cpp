#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

TEST_CASE("toolchain sanity") {
  static_assert(__cplusplus >= 202400L);
  CHECK(1 + 1 == 2);
}
