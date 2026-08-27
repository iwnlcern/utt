#include "doctest/doctest.h"
#include "search/backup.hpp"
#include "support/test_rational.hpp"

#include <array>
#include <cmath>
#include <cstdint>
#include <limits>

using namespace uttt;

namespace {

struct BackupVector {
  TInterval a;
  TInterval b;
  Tie tie;
  std::int64_t t_numerator;
  std::int64_t t_denominator;
  std::int64_t r_numerator;
  std::int64_t r_denominator;
};

TInterval enclose_nearest(double nearest) {
  return {
      std::nextafter(nearest, -std::numeric_limits<double>::infinity()),
      std::nextafter(nearest, std::numeric_limits<double>::infinity()),
  };
}

const std::array<BackupVector, 6> kBackupVectors{{
    // non-contract reference: fixture:backup-zugzwang-three-quarters-h-x (consumed_by theory-only pending MR17)
    {{0.75, 0.75}, {0.25, 0.25}, Tie::X, 3, 4, 0, 1},
    // non-contract reference: fixture:backup-zugzwang-three-quarters-h-o (consumed_by theory-only pending MR17)
    {{0.75, 0.75}, {0.25, 0.25}, Tie::O, 1, 4, 0, 1},
    // non-contract reference: fixture:backup-zugzwang-two-thirds-h-x (consumed_by theory-only pending MR17)
    {enclose_nearest(2.0 / 3.0), enclose_nearest(1.0 / 3.0), Tie::X, 2, 3, 0, 1},
    // non-contract reference: fixture:backup-zugzwang-two-thirds-h-o (consumed_by theory-only pending MR17)
    {enclose_nearest(2.0 / 3.0), enclose_nearest(1.0 / 3.0), Tie::O, 1, 3, 0, 1},
    // non-contract reference: fixture:backup-equal-children-zero-critical (consumed_by theory-only pending MR17)
    {{0.5, 0.5}, {0.5, 0.5}, Tie::X, 1, 2, 0, 1},
    // non-contract reference: fixture:backup-ordered-children-positive-critical (consumed_by theory-only pending MR17)
    {{0.25, 0.25}, {0.75, 0.75}, Tie::O, 1, 2, 1, 3},
}};

void check_contains_interval(TInterval outer, TInterval inner) {
  CHECK(outer.lo <= inner.lo);
  CHECK(inner.hi <= outer.hi);
}

void check_eight_ulp_enclosure(
    TInterval interval, std::int64_t numerator, std::int64_t denominator) {
  const TestRational exact{numerator, denominator};
  const double nearest = static_cast<double>(numerator) / static_cast<double>(denominator);
  const double ulp = std::nextafter(nearest, std::numeric_limits<double>::infinity()) - nearest;

  CHECK(exact.inside(interval.lo, interval.hi));
  CHECK(width(interval) <= 8.0 * ulp);
}

}  // namespace

TEST_CASE("backup aggregation initializes missing sides and folds endpoints independently") {
  Aggregates aggregates{{0.0, 0.0}, {1.0, 1.0}, false, false};

  fold_x(aggregates, {0.70, 0.90});
  CHECK(aggregates.has_x);
  CHECK(aggregates.a.lo == 0.70);
  CHECK(aggregates.a.hi == 0.90);
  fold_x(aggregates, {0.60, 0.95});
  fold_x(aggregates, {0.65, 0.80});
  CHECK(aggregates.a.lo == 0.60);
  CHECK(aggregates.a.hi == 0.80);

  fold_o(aggregates, {0.10, 0.20});
  CHECK(aggregates.has_o);
  CHECK(aggregates.b.lo == 0.10);
  CHECK(aggregates.b.hi == 0.20);
  fold_o(aggregates, {0.05, 0.50});
  fold_o(aggregates, {0.35, 0.45});
  CHECK(aggregates.b.lo == 0.35);
  CHECK(aggregates.b.hi == 0.50);
}

TEST_CASE("backup A1 copied non-contract reference vectors enclose exact results") {
  for (std::size_t index = 0; index < kBackupVectors.size(); ++index) {
    CAPTURE(index);
    const BackupVector& vector = kBackupVectors[index];
    Aggregates aggregates{};
    fold_x(aggregates, vector.a);
    fold_o(aggregates, vector.b);

    check_eight_ulp_enclosure(
        backup_node(aggregates, vector.tie), vector.t_numerator, vector.t_denominator);
    check_eight_ulp_enclosure(
        critical_r(aggregates), vector.r_numerator, vector.r_denominator);
  }
}

TEST_CASE("backup equality seam selects ordered result where both branches coincide") {
  Aggregates aggregates{};
  fold_x(aggregates, {0.5, 0.5});
  fold_o(aggregates, {0.5, 0.5});
  const TInterval ordered = f_backup(aggregates.a, aggregates.b);
  const TInterval for_x = backup_node(aggregates, Tie::X);
  const TInterval for_o = backup_node(aggregates, Tie::O);

  CHECK(for_x.lo == ordered.lo);
  CHECK(for_x.hi == ordered.hi);
  CHECK(for_o.lo == ordered.lo);
  CHECK(for_o.hi == ordered.hi);
  CHECK(contains(for_x, 0.5));
  CHECK(contains(for_o, 0.5));
  CHECK(contains(critical_r(aggregates), 0.0));
}

TEST_CASE("backup hull contains ordered and X-tie branch results when intervals straddle") {
  Aggregates aggregates{};
  fold_x(aggregates, {0.2, 0.8});
  fold_o(aggregates, {0.4, 0.6});
  const TInterval ordered = f_backup(aggregates.a, aggregates.b);
  const TInterval result = backup_node(aggregates, Tie::X);

  check_contains_interval(result, ordered);
  check_contains_interval(result, aggregates.a);
}

TEST_CASE("backup hull contains ordered and O-tie branch results when intervals straddle") {
  Aggregates aggregates{};
  fold_x(aggregates, {0.4, 0.6});
  fold_o(aggregates, {0.2, 0.8});
  const TInterval ordered = f_backup(aggregates.a, aggregates.b);
  const TInterval result = backup_node(aggregates, Tie::O);

  check_contains_interval(result, ordered);
  check_contains_interval(result, aggregates.b);
}
