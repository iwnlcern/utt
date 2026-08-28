#include "adapter/policy.hpp"
#include "doctest/doctest.h"
#include "nlohmann/json.hpp"
#include "root/alt_solver.hpp"
#include "root/bid_matrix.hpp"
#include "root/rmplus.hpp"
#include "support/test_rational.hpp"
#include "support/ttt3_continuous.hpp"
#include "support/ttt3_discrete.hpp"

#include "core/clock.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using namespace uttt;
using json = nlohmann::json;

namespace {

struct Coordinate {
  uint16_t x;
  uint16_t o;
  Tie h;
  int64_t bx;
  int64_t bo;
  auto operator<=>(const Coordinate &) const = default;
};

struct TickClock final : Clock {
  int64_t value = 0;
  int64_t step = 100;
  int64_t now_ms() override {
    const int64_t result = value;
    value += step;
    return result;
  }
};

struct CountingClock final : Clock {
  std::size_t polls = 0;
  int64_t now_ms() override {
    ++polls;
    return 0;
  }
};

struct GateClock final : Clock {
  std::size_t free_polls = 0;
  std::size_t polls = 0;
  int64_t now_ms() override { return polls++ < free_polls ? 0 : 2'000; }
};

double rational_as_double(const std::string &text) {
  const std::size_t slash = text.find('/');
  if (slash == std::string::npos)
    return std::stod(text);
  return std::stod(text.substr(0, slash)) / std::stod(text.substr(slash + 1));
}

std::vector<RootAction> actions_from_json(const json &value) {
  std::vector<RootAction> result;
  for (const auto &entry : value) {
    result.push_back(
        {entry.at("bid").get<int64_t>(), entry.at("move").get<uint8_t>()});
  }
  return result;
}

const json &root_record(const json &artifact, Tie h, int scale) {
  const std::string id = std::string("root-") + (h == Tie::X ? "X" : "O") +
                         "-N" + std::to_string(scale);
  for (const auto &record : artifact.at("records")) {
    if (record.at("id") == id)
      return record;
  }
  throw std::runtime_error("missing root LP record " + id);
}

Ttt3State child_for(const Ttt3State &state, Seat mover, uint8_t move) {
  const auto children = mover == Seat::X ? Ttt3Model::children_x(state)
                                         : Ttt3Model::children_o(state);
  const auto found =
      std::find_if(children.begin(), children.end(),
                   [move](const auto &child) { return child.move == move; });
  REQUIRE(found != children.end());
  return found->state;
}

Position uttt_all_terminal_root() {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  x[0] = 0b000000111;
  x[4] = 0b000000111;
  o[2] = 0b000000111;
  o[5] = 0b000000111;
  // The sole open cell is 8. X completes local diagonal 0-4-8 and macro
  // diagonal 0-4-8; O completes local column 2-5-8 and macro column 2-5-8.
  x[8] = static_cast<uint16_t>((1u << 0) | (1u << 1) | (1u << 4) | (1u << 6));
  o[8] = static_cast<uint16_t>((1u << 2) | (1u << 3) | (1u << 5) | (1u << 7));
  auto result = Position::from_parts(x, o, 8, Tie::X);
  REQUIRE(result.has_value());
  return *result;
}

std::set<Coordinate> expected_coordinates(const Ttt3State &state, Tie h,
                                          int64_t bx, int64_t bo,
                                          const std::vector<RootAction> &rows,
                                          const std::vector<RootAction> &cols) {
  std::set<Coordinate> result;
  for (const RootAction row : rows) {
    for (const RootAction column : cols) {
      if (row.bid > column.bid || (row.bid == column.bid && h == Tie::X)) {
        const Ttt3State child = child_for(state, Seat::X, row.move);
        result.insert({child.x, child.o, Tie::O, bx - row.bid, bo});
      } else {
        const Ttt3State child = child_for(state, Seat::O, column.move);
        result.insert({child.x, child.o, Tie::X, bx, bo - column.bid});
      }
    }
  }
  return result;
}

} // namespace

TEST_CASE("root matrix candidate bids use exact rounded anchors") {
  CHECK(candidate_bids(8, 8) == std::vector<int64_t>{0, 2, 4, 6, 7, 8});
  CHECK(candidate_bids(32, 17) ==
        std::vector<int64_t>{0, 8, 15, 16, 17, 18, 19, 24, 32});
  CHECK(exact_half_up_product(0.5, 17) == 9);
  CHECK(exact_half_up_product(std::nextafter(0.5, 0.0), 17) == 8);
  CHECK(exact_half_up_product(std::nextafter(0.5, 1.0), 17) == 9);
  CHECK(exact_half_up_product(std::numeric_limits<double>::denorm_min(),
                              1'000'000'000) == 0);
  CHECK(production_anchors({0.0, 0.0}, {1.0, 1.0}, 17, 2, 7).k_star == 9);
  CHECK(production_anchors({0.75, 0.75}, {0.25, 0.25}, 17, 2, 7).k_star == 0);
}

TEST_CASE("root matrix A8 action labels payoffs and provider coordinates match "
          "theory") {
  std::ifstream input(UTTT_TEST_LP_REFERENCE);
  REQUIRE_MESSAGE(input.good(), "committed ttt3 LP reference is missing");
  const json artifact = json::parse(input);
  CHECK(artifact.at("provenance").at("generator") ==
        "engine/tools/gen_lp_reference.py");

  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  for (Tie h : {Tie::X, Tie::O}) {
    for (int scale : {8, 16, 32}) {
      CAPTURE(h);
      CAPTURE(scale);
      const json &reference = root_record(artifact, h, scale);
      const ContNode continuous = solve_continuous(state, h);
      const Anchors anchors{
          half_up_int(continuous.r * TestRational{2 * scale}),
          continuous.best_x,
          continuous.best_o,
      };
      std::set<Coordinate> requested;
      PayoffFn<Ttt3State> payoff = [&](Ttt3State child, Tie h2, int64_t bx2,
                                       int64_t bo2) {
        requested.insert({child.x, child.o, h2, bx2, bo2});
        return PayoffResult{
            static_cast<double>(
                static_cast<int>(solve_discrete(
                    child, h2, static_cast<int>(bx2), static_cast<int>(bo2))) -
                1),
            true,
        };
      };
      const auto matrix = build_bid_matrix<Ttt3Model>(
          state, h, scale, scale, anchors, std::move(payoff));
      const auto expected_rows = actions_from_json(reference.at("row_actions"));
      const auto expected_columns =
          actions_from_json(reference.at("column_actions"));
      CHECK(matrix.row_actions == expected_rows);
      CHECK(matrix.column_actions == expected_columns);
      CHECK(matrix.payoffs ==
            reference.at("matrix").get<std::vector<std::vector<double>>>());
      CHECK(requested == expected_coordinates(state, h, scale, scale,
                                              expected_rows, expected_columns));
      CHECK(matrix.all_exact);

      const RMPlusResult solved = solve_rmplus(matrix.payoffs, 10'000);
      CHECK(solved.iterations == 10'000);
      CHECK(solved.exploitability <= 0.02);
      CHECK(std::abs(
                solved.value -
                rational_as_double(reference.at("value").get<std::string>())) <=
            1.0 / static_cast<double>(scale));
      CHECK(solved.average_regret <= 0.02);
      std::cout << "[root-matrix] " << reference.at("id").get<std::string>()
                << " value=" << solved.value
                << " exploitability=" << solved.exploitability
                << " average_regret=" << solved.average_regret << '\n';
    }
  }
}

TEST_CASE("root matrix graded production payoff carries magnitude and taint") {
  const Ttt3State child = Ttt3State::from_board(".........", Tie::X);
  SearchResult midpoint{{0.0, 1.0}, 0, 0, Quality::Estimate, 4, true};
  midpoint.t_est = 0.5;

  const PayoffResult x_side =
      production_payoff<Ttt3Model>(child, Tie::X, 51, 49, midpoint);
  const PayoffResult o_side =
      production_payoff<Ttt3Model>(child, Tie::O, 49, 51, midpoint);
  const PayoffResult equality =
      production_payoff<Ttt3Model>(child, Tie::X, 50, 50, midpoint);
  const PayoffResult x_saturated =
      production_payoff<Ttt3Model>(child, Tie::X, 5, 3, midpoint);
  const PayoffResult o_saturated =
      production_payoff<Ttt3Model>(child, Tie::O, 3, 5, midpoint);
  CHECK(x_side.value == doctest::Approx(0.08));
  CHECK(o_side.value == doctest::Approx(-0.08));
  CHECK(equality.value == 0.0);
  CHECK(x_saturated.value == 1.0);
  CHECK(o_saturated.value == -1.0);
  CHECK_FALSE(x_side.exact);
  CHECK_FALSE(o_side.exact);
  CHECK_FALSE(equality.exact);

  const Anchors anchors{50, 0, 0};
  PayoffFn<Ttt3State> in_band = [&](Ttt3State next, Tie h2, int64_t bx2,
                                    int64_t bo2) {
    return production_payoff<Ttt3Model>(next, h2, bx2, bo2, midpoint);
  };
  const auto matrix = build_bid_matrix<Ttt3Model>(child, Tie::X, 50, 50,
                                                  anchors, std::move(in_band));
  CHECK_FALSE(matrix.all_exact);
  CHECK(matrix_quality(Quality::Exact, matrix.all_exact) == Quality::Estimate);
  CHECK_THROWS_AS(production_payoff<Ttt3Model>(child, Tie::X, 0, 0, midpoint),
                  std::invalid_argument);
}

TEST_CASE("root matrix rejects nonfinite and out-of-range double payoffs") {
  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  const Anchors anchors{0, 0, 0};
  for (double invalid :
       {std::numeric_limits<double>::quiet_NaN(),
        std::numeric_limits<double>::infinity(), 1.01, -1.01}) {
    CAPTURE(invalid);
    PayoffFn<Ttt3State> payoff = [invalid](Ttt3State, Tie, int64_t, int64_t) {
      return PayoffResult{invalid, false};
    };
    CHECK_THROWS_AS(build_bid_matrix<Ttt3Model>(state, Tie::X, 1, 1, anchors,
                                                std::move(payoff)),
                    std::logic_error);
  }
}

TEST_CASE(
    "root matrix extraction filters by maximum averaged probability first") {
  RootMatrix<Ttt3State> matrix;
  matrix.row_actions = {{9, 8}, {3, 7}, {3, 2}};
  matrix.column_actions = {{8, 8}, {4, 7}, {4, 1}};
  matrix.payoffs = {{1.0, 1.0, 1.0}, {0.4, 0.2, 0.2}, {0.4, 0.2, 0.2}};
  RMPlusResult solution;
  solution.row_strategy = {0.01, 0.495, 0.495};
  solution.column_strategy = {0.01, 0.495, 0.495};

  CHECK(select_root_action(matrix, solution, Seat::X) == 2);
  CHECK(select_root_action(matrix, solution, Seat::O) == 2);
}

TEST_CASE(
    "root matrix extraction compares payoff within equal max probability") {
  RootMatrix<Ttt3State> matrix;
  matrix.row_actions = {{5, 8}, {5, 7}, {3, 6}, {3, 2}};
  matrix.column_actions = {{5, 8}, {5, 7}, {3, 6}, {3, 2}};
  matrix.payoffs = {{0.0, 0.0, 0.0, 0.0},
                    {0.5, 0.5, 0.5, 0.5},
                    {-0.5, -0.5, -0.5, -0.5},
                    {-0.5, -0.5, -0.5, -0.5}};
  RMPlusResult solution;
  solution.row_strategy = {0.4, 0.4, 0.1, 0.1};
  solution.column_strategy = {0.4, 0.4, 0.1, 0.1};
  CHECK(select_root_action(matrix, solution, Seat::X) == 1);

  matrix.payoffs = {{0.0, 0.5, -0.5, -0.5},
                    {0.0, 0.5, -0.5, -0.5},
                    {0.0, 0.5, -0.5, -0.5},
                    {0.0, 0.5, -0.5, -0.5}};
  CHECK(select_root_action(matrix, solution, Seat::O) == 0);

  matrix.payoffs.assign(4, std::vector<double>(4, 0.0));
  CHECK(select_root_action(matrix, solution, Seat::X) == 1);
  CHECK(select_root_action(matrix, solution, Seat::O) == 1);

  solution.row_strategy = {0.4, 0.1, 0.4, 0.1};
  solution.column_strategy = {0.4, 0.1, 0.4, 0.1};
  CHECK(select_root_action(matrix, solution, Seat::X) == 2);
  CHECK(select_root_action(matrix, solution, Seat::O) == 2);
}

TEST_CASE("root matrix ply-zero bid ties are fair coin expectations") {
  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  int x_calls = 0;
  int o_calls = 0;
  PayoffFn<Ttt3State> payoff = [&](Ttt3State, Tie h2, int64_t bx2,
                                   int64_t bo2) {
    if (h2 == Tie::O) {
      ++x_calls;
      CHECK(bx2 == 0);
      CHECK(bo2 == 0);
      return std::optional<PayoffResult>{{1.0, true}};
    }
    ++o_calls;
    CHECK(bx2 == 0);
    CHECK(bo2 == 0);
    return std::optional<PayoffResult>{{-0.5, false}};
  };
  const auto matrix = build_bid_matrix<Ttt3Model>(
      state, Tie::NullFirstMove, 0, 0, Anchors{0, 0, 0}, std::move(payoff));
  REQUIRE(matrix.complete);
  REQUIRE(matrix.payoffs.size() == 4);
  for (const auto &row : matrix.payoffs)
    for (double value : row)
      CHECK(value == 0.25);
  CHECK(x_calls == 16);
  CHECK(o_calls == 16);
  CHECK_FALSE(matrix.all_exact);

  PayoffFn<Ttt3State> reverse = [](Ttt3State, Tie h2, int64_t, int64_t) {
    return std::optional<PayoffResult>{
        h2 == Tie::O ? PayoffResult{-1.0, false}
                     : PayoffResult{0.5, true}};
  };
  const auto reverse_matrix = build_bid_matrix<Ttt3Model>(
      state, Tie::NullFirstMove, 0, 0, Anchors{0, 0, 0},
      std::move(reverse));
  REQUIRE(reverse_matrix.complete);
  for (const auto &row : reverse_matrix.payoffs)
    for (double value : row)
      CHECK(value == -0.25);
  CHECK_FALSE(reverse_matrix.all_exact);

  PayoffFn<Ttt3State> both_exact = [](Ttt3State, Tie h2, int64_t, int64_t) {
    return std::optional<PayoffResult>{
        h2 == Tie::O ? PayoffResult{1.0, true}
                     : PayoffResult{-1.0, true}};
  };
  const auto exact_matrix = build_bid_matrix<Ttt3Model>(
      state, Tie::NullFirstMove, 0, 0, Anchors{0, 0, 0},
      std::move(both_exact));
  REQUIRE(exact_matrix.complete);
  CHECK(exact_matrix.all_exact);
}

TEST_CASE("root matrix cancellation never stores a partial coin expectation") {
  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  int payoff_calls = 0;
  int stop_polls = 0;
  PayoffFn<Ttt3State> payoff = [&](Ttt3State, Tie, int64_t, int64_t) {
    ++payoff_calls;
    return std::optional<PayoffResult>{{1.0, true}};
  };
  const auto matrix = build_bid_matrix<Ttt3Model>(
      state, Tie::NullFirstMove, 0, 0, Anchors{0, 0, 0}, std::move(payoff),
      [&] { return ++stop_polls == 2; });
  CHECK_FALSE(matrix.complete);
  CHECK(matrix.entries_evaluated == 0);
  CHECK(payoff_calls == 1);
}

TEST_CASE("root matrix validates both coin branches before averaging") {
  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  int calls = 0;
  PayoffFn<Ttt3State> payoff = [&](Ttt3State, Tie, int64_t, int64_t) {
    return std::optional<PayoffResult>{{++calls % 2 == 1 ? 2.0 : -2.0,
                                        true}};
  };
  CHECK_THROWS_AS(build_bid_matrix<Ttt3Model>(
                      state, Tie::NullFirstMove, 0, 0, Anchors{0, 0, 0},
                      std::move(payoff)),
                  std::logic_error);
}

TEST_CASE("root critical fraction enclosure contains exact scalar") {
  const Aggregates aggregates{{0.2, 0.3}, {0.6, 0.7}, true, true};
  const TInterval enclosure = critical_r_enclosure(aggregates.a, aggregates.b);
  const TestRational a{1, 4};
  const TestRational b{2, 3};
  const TestRational exact = (b - a) / (TestRational{1} - a + b);
  CHECK(exact.inside(enclosure.lo, enclosure.hi));

  std::mt19937_64 rng{0xC8A13u};
  for (int index = 0; index < 200; ++index) {
    double point_a = static_cast<double>(rng() >> 11) * 0x1.0p-54;
    double point_b = 0.5 + static_cast<double>(rng() >> 11) * 0x1.0p-54;
    if (point_a > point_b)
      std::swap(point_a, point_b);
    const TInterval point =
        critical_r_enclosure({point_a, point_a}, {point_b, point_b});
    const TestRational exact_a = TestRational::from_double(point_a);
    const TestRational exact_b = TestRational::from_double(point_b);
    const TestRational exact_r =
        (exact_b - exact_a) / (TestRational{1} - exact_a + exact_b);
    CAPTURE(index);
    CAPTURE(point_a);
    CAPTURE(point_b);
    CHECK(exact_r.inside(point.lo, point.hi));
  }
}

TEST_CASE(
    "root forced certificate fails closed and publishes only proven actions") {
  using policy_detail::CertifiedChild;
  const auto exact = [](uint8_t move, TInterval t) {
    return CertifiedChild{move, t, Quality::Exact, true};
  };

  const std::vector<CertifiedChild> dominant_x{exact(1, {0.1, 0.1}),
                                               exact(2, {0.4, 0.4})};
  const std::vector<CertifiedChild> dominant_o{exact(3, {0.6, 0.6}),
                                               exact(4, {0.3, 0.3})};
  const Aggregates stable{{0.1, 0.1}, {0.6, 0.6}, true, true};
  const auto x = policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 10, 10, stable, dominant_x, dominant_o, true);
  const auto o = policy_detail::certified_forced_action(
      OForced, Seat::O, Tie::O, 10, 10, stable, dominant_x, dominant_o, true);
  REQUIRE(x.has_value());
  REQUIRE(o.has_value());
  CHECK(*x == RootAction{4, 1});
  CHECK(*o == RootAction{4, 3});

  const Aggregates midpoint_witness{{0.0, 0.0}, {0.0, 0.3}, true, true};
  CHECK_FALSE(policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 100, 100, midpoint_witness,
      {exact(1, {0.0, 0.0})}, {exact(2, {0.0, 0.3})}, true));

  const double third = 1.0 / 3.0;
  const Aggregates ceiling_straddle{
      {0.0, 0.0},
      {std::nextafter(third, 0.0), std::nextafter(third, 1.0)},
      true,
      true};
  CHECK_FALSE(policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 4, 4, ceiling_straddle, {exact(1, {0.0, 0.0})},
      {exact(2, ceiling_straddle.b)}, true));

  const std::vector<CertifiedChild> overlapping_x{exact(1, {0.1, 0.2}),
                                                  exact(2, {0.15, 0.3})};
  const Aggregates overlap{{0.1, 0.2}, {0.6, 0.6}, true, true};
  CHECK_FALSE(policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 1, 1, overlap, overlapping_x, dominant_o,
      true));

  CHECK_FALSE(policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 10, 3, stable, dominant_x, dominant_o, true));
  CHECK_FALSE(policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 10, 10, stable, dominant_x, dominant_o, false));

  const Aggregates zugzwang{{0.8, 0.8}, {0.2, 0.2}, true, true};
  const auto zero = policy_detail::certified_forced_action(
      XForced, Seat::X, Tie::X, 10, 10, zugzwang,
      {exact(5, {0.8, 0.8}), exact(6, {0.9, 0.9})}, {exact(7, {0.2, 0.2})},
      true);
  REQUIRE(zero.has_value());
  CHECK(*zero == RootAction{0, 5});
}

TEST_CASE("root production payoff preserves the half-boundary discriminator") {
  const Ttt3State child = Ttt3State::from_board(".........", Tie::X);
  SearchResult below{{0.0, 1.0}, 0, 0, Quality::Estimate, 1, true};
  SearchResult above = below;
  below.t_est = std::nextafter(0.5, 1.0);
  above.t_est = std::nextafter(0.5, 0.0);
  const double below_value =
      production_payoff<Ttt3Model>(child, Tie::X, 9, 7, below).value;
  const double above_value =
      production_payoff<Ttt3Model>(child, Tie::X, 9, 7, above).value;
  CHECK(std::bit_cast<uint64_t>(below_value) ==
        std::bit_cast<uint64_t>(0x1.ffffffffffff0p-2));
  CHECK(std::bit_cast<uint64_t>(above_value) ==
        std::bit_cast<uint64_t>(0x1.0000000000004p-1));
}

TEST_CASE("root matrix zero-total alternation is finite for both tie owners") {
  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  for (Tie h : {Tie::X, Tie::O}) {
    AltSolver<Ttt3Model> alternate;
    PayoffFn<Ttt3State> payoff =
        [&](Ttt3State child, Tie h2, int64_t bx2,
            int64_t bo2) -> std::optional<PayoffResult> {
      REQUIRE(bx2 == 0);
      REQUIRE(bo2 == 0);
      const AltResult result = alternate.solve(
          child, h2, AltLimits{Ttt3Model::empties(child), 100'000, 1, {}});
      if (!result.complete)
        return std::nullopt;
      return PayoffResult{result.value, result.quality == Quality::Exact};
    };
    const auto matrix = build_bid_matrix<Ttt3Model>(
        state, h, 0, 0, Anchors{0, 0, 0}, std::move(payoff));
    REQUIRE(matrix.complete);
    CHECK(matrix.all_exact);
    CHECK(alternate.unique_root_searches() == 4);
    const RMPlusResult solution = solve_rmplus(matrix.payoffs, 10'000);
    CHECK(std::isfinite(solution.value));
    CHECK(std::isfinite(solution.exploitability));
    const std::size_t selected =
        select_root_action(matrix, solution, h == Tie::X ? Seat::X : Seat::O);
    CHECK(selected < (h == Tie::X ? matrix.row_actions.size()
                                  : matrix.column_actions.size()));
  }
}

TEST_CASE("root matrix incomplete payoff stores no partial value for RM plus") {
  const Ttt3State state = Ttt3State::from_board(".........", Tie::X);
  int calls = 0;
  PayoffFn<Ttt3State> payoff = [&](Ttt3State, Tie, int64_t,
                                   int64_t) -> std::optional<PayoffResult> {
    ++calls;
    return std::nullopt;
  };
  const auto matrix = build_bid_matrix<Ttt3Model>(
      state, Tie::X, 0, 0, Anchors{0, 0, 0}, std::move(payoff));
  CHECK_FALSE(matrix.complete);
  CHECK(matrix.entries_evaluated == 0);
  CHECK(calls == 1);
}

TEST_CASE("root matrix UTTT production binding covers in-band and exact P2 "
          "children") {
  const Position root = Position::initial();
  const SearchResult midpoint{{0.5, 0.5}, 0, 0, Quality::Bound, 2, true};
  PayoffFn<Position> in_band = [&](Position child, Tie h2, int64_t bx2,
                                   int64_t bo2) {
    return production_payoff<UtttModel>(child, h2, bx2, bo2, midpoint);
  };
  const auto matrix = build_bid_matrix<UtttModel>(
      root, Tie::X, 100, 100, Anchors{0, 36, 44}, std::move(in_band));
  CHECK_FALSE(matrix.all_exact);
  CHECK(matrix_quality(Quality::Bound, matrix.all_exact) == Quality::Estimate);

  const SearchResult x_threshold{{0.0, 0.0}, 0, 0, Quality::Bound, 2, true};
  const SearchResult o_threshold{{1.0, 1.0}, 0, 0, Quality::Bound, 2, true};
  CHECK(production_payoff<UtttModel>(root, Tie::X, 1'000, 0, x_threshold) ==
        PayoffResult{1, true});
  CHECK(production_payoff<UtttModel>(root, Tie::O, 0, 1'000, o_threshold) ==
        PayoffResult{-1, true});

  PayoffFn<Position> forced = [&](Position child, Tie h2, int64_t bx2,
                                  int64_t bo2) {
    const SearchResult threshold =
        bx2 > UtttModel::empties(child) ? x_threshold : o_threshold;
    return production_payoff<UtttModel>(child, h2, bx2, bo2, threshold);
  };
  const auto forced_matrix = build_bid_matrix<UtttModel>(
      root, Tie::X, 1'000, 1'000, Anchors{0, 36, 44}, std::move(forced));
  CHECK(forced_matrix.all_exact);
}

TEST_CASE(
    "root matrix terminal children produce exact ordinals without taint") {
  const Position state = uttt_all_terminal_root();
  const SearchResult unused{};
  PayoffFn<Position> terminal = [&](Position child, Tie h2, int64_t bx2,
                                    int64_t bo2) {
    return production_payoff<UtttModel>(child, h2, bx2, bo2, unused);
  };
  const auto matrix = build_bid_matrix<UtttModel>(
      state, Tie::X, 4, 4, Anchors{0, 80, 80}, std::move(terminal));
  CHECK(matrix.all_exact);
  CHECK(matrix_quality(Quality::Exact, matrix.all_exact) == Quality::Exact);
  CHECK(matrix.payoffs.size() == matrix.row_actions.size());
}

TEST_CASE(
    "root matrix request-aware deadlines fit 2s and 30s request budgets") {
  for (int64_t time_ms : {2'000, 30'000}) {
    CAPTURE(time_ms);
    const RequestDeadlines deadlines = request_deadlines(1'000, time_ms);
    CHECK(deadlines.soft == 1'000 + (27 * time_ms) / 30);
    CHECK(deadlines.hard == 1'000 + (29 * time_ms) / 30);
    CHECK(deadlines.reserve == std::min<int64_t>(500, time_ms / 6));
    CHECK(deadlines.search_stop == deadlines.soft - deadlines.reserve);
    CHECK(deadlines.search_stop <= deadlines.soft);
    CHECK(deadlines.soft <= deadlines.hard);
    CHECK(deadlines.hard <= 1'000 + time_ms);
  }
  const RequestDeadlines saturated =
      request_deadlines(std::numeric_limits<int64_t>::max() - 4, 30'000);
  CHECK(saturated.hard == std::numeric_limits<int64_t>::max());
}

TEST_CASE("root matrix A11 hard abort publishes prior completed iteration and "
          "still runs matrix") {
  FakeClock clock;
  const RequestDeadlines deadlines = request_deadlines(0, 2'000);
  int matrix_calls = 0;
  const int published = run_root_stages<int>(
      7, 1, 4, clock, deadlines,
      [&](int depth, int64_t hard) -> std::optional<int> {
        if (depth == 1) {
          clock.t = 100;
          return 11;
        }
        clock.t = hard;
        return std::nullopt;
      },
      [&](int staged, int64_t hard) {
        ++matrix_calls;
        CHECK(staged == 11);
        CHECK(hard == deadlines.hard);
        return staged + 1;
      });
  CHECK(published == 12);
  CHECK(matrix_calls == 1);

  FakeClock soft_clock;
  const int soft_published = run_root_stages<int>(
      3, 1, 2, soft_clock, deadlines,
      [&](int, int64_t) -> std::optional<int> {
        soft_clock.t = deadlines.soft;
        return 99;
      },
      [](int staged, int64_t) { return staged; });
  CHECK(soft_published == 3);
}

TEST_CASE("root matrix production Search cancellation is checked inside DFS") {
  int polls = 0;
  Search<UtttModel> search(10);
  const SearchResult cancelled =
      search.solve(Position::initial(), Tie::NullFirstMove,
                   Limits{6, std::numeric_limits<uint64_t>::max(), false, true,
                          12, 1, [&] { return ++polls == 4; }});
  CHECK_FALSE(cancelled.complete);
  CHECK(cancelled.depth == 0);
  CHECK(polls == 4);

  int initial_polls = 0;
  const SearchResult initially_cancelled = search.solve(
      Position::initial(), Tie::X,
      Limits{6, std::numeric_limits<uint64_t>::max(), false, true, 12, 0, [&] {
               ++initial_polls;
               return true;
             }});
  CHECK_FALSE(initially_cancelled.complete);
  CHECK(initial_polls == 1);
}

TEST_CASE("root matrix policy seam emits a legal in-band UTTT reply") {
  wire::TurnRequest request;
  request.request_id = "root-policy";
  request.ply = 0;
  request.ctx = {Seat::X, 100, 100};
  request.pos = Position::initial();
  request.time_ms = 2'000;
  MoveList legal;
  request.pos.legal_moves(legal);
  request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);

  EnginePolicy policy;
  FakeClock clock;
  const wire::TurnReply reply = policy.choose(request, clock);
  CHECK(reply.request_id == request.request_id);
  CHECK(reply.bid >= 0);
  CHECK(reply.bid <= request.ctx.budget_x);
  CHECK(std::find(request.legal.begin(), request.legal.end(), reply.move) !=
        request.legal.end());
  CHECK(reply.info.quality == "estimate");
  CHECK(reply.info.complete);
  REQUIRE(reply.info.lo.has_value());
  REQUIRE(reply.info.hi.has_value());
  Search<UtttModel> reference;
  const SearchResult null_report =
      reference.solve(request.pos, Tie::NullFirstMove,
                      Limits{reply.info.depth, 25'000, true, true, 12});
  CHECK(*reply.info.lo == null_report.t.lo);
  CHECK(*reply.info.hi == null_report.t.hi);
  const PolicyDiagnostics in_band = policy.last_diagnostics();
  CHECK(in_band.p2_checked);
  CHECK(in_band.root_class == InBand);
  CHECK(in_band.matrix_constructed);
  CHECK(in_band.matrix_complete);
  CHECK(in_band.matrix_solved);
  CHECK(in_band.matrix_action_published);
  CHECK(in_band.rm_iterations == 10'000);

  request.request_id = "forced-policy";
  request.ply = 1;
  request.ctx = {Seat::X, 1'000, 0};
  request.pos = uttt_all_terminal_root();
  request.time_ms = 2'000;
  request.legal.clear();
  request.pos.legal_moves(legal);
  request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);
  const wire::TurnReply forced_reply = policy.choose(request, clock);
  CHECK(std::find(request.legal.begin(), request.legal.end(),
                  forced_reply.move) != request.legal.end());
  const PolicyDiagnostics forced = policy.last_diagnostics();
  CHECK(forced.p2_checked);
  CHECK(forced.root_class == XForced);
  CHECK(forced.certificate_attempted);
  CHECK_FALSE(forced.certificate_published);
  CHECK_FALSE(forced.matrix_bypassed);
  CHECK(forced.matrix_constructed);
  CHECK(forced.matrix_solved);
  CHECK(forced.matrix_action_published);
}

TEST_CASE("root policy play window enables a production UTTT cutoff") {
  std::mt19937 rng{0xC416u};
  bool witnessed = false;
  for (int attempt = 0; attempt < 24 && !witnessed; ++attempt) {
    Position position = Position::initial();
    Seat mover = Seat::X;
    for (int ply = 0; ply < 70; ++ply) {
      if (UtttModel::terminal(position) != TerminalKind::None)
        break;
      MoveList legal;
      position.legal_moves(legal);
      REQUIRE(legal.n > 0);
      const Move move = legal.m[rng() % legal.n];
      position = position.applied(move, mover).value();
      mover = mover == Seat::X ? Seat::O : Seat::X;
      if (UtttModel::terminal(position) != TerminalKind::None ||
          UtttModel::empties(position) > 24)
        continue;

      wire::TurnRequest request;
      request.request_id = "window-cut";
      request.ply = ply + 1;
      request.ctx = {Seat::X, 600, 400};
      request.pos = position;
      request.time_ms = 2'000;
      position.legal_moves(legal);
      request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);
      EnginePolicy policy;
      FakeClock clock;
      static_cast<void>(policy.choose(request, clock));
      const CutCounters cuts = policy.last_diagnostics().root_cuts;
      witnessed = cuts.min_dominance + cuts.max_dominance + cuts.window_lo +
                      cuts.window_hi + cuts.precision >
                  0;
      if (witnessed)
        break;
    }
  }
  CHECK(witnessed);
}

TEST_CASE("root matrix zero-time policy fallback is legal and incomplete") {
  wire::TurnRequest request;
  request.request_id = "zero-time";
  request.ply = 0;
  request.ctx = {Seat::O, 100, 100};
  request.pos = Position::initial();
  request.time_ms = 0;
  MoveList legal;
  request.pos.legal_moves(legal);
  request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);

  EnginePolicy policy;
  FakeClock clock;
  const wire::TurnReply reply = policy.choose(request, clock);
  CHECK(reply.info.depth == 0);
  CHECK_FALSE(reply.info.complete);
  CHECK(std::find(request.legal.begin(), request.legal.end(), reply.move) !=
        request.legal.end());
}

TEST_CASE("root matrix production cancellation preserves the reserved slice") {
  wire::TurnRequest request;
  request.request_id = "cancelled-root";
  request.ply = 0;
  request.ctx = {Seat::X, 100, 100};
  request.pos = Position::initial();
  request.time_ms = 2'000;
  MoveList legal;
  request.pos.legal_moves(legal);
  request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);

  EnginePolicy policy;
  TickClock clock;
  clock.step = 5;
  const wire::TurnReply reply = policy.choose(request, clock);
  CHECK(reply.info.complete);
  CHECK(reply.info.depth > 0);
  const PolicyDiagnostics diagnostics = policy.last_diagnostics();
  CHECK(diagnostics.root_search_cancelled);
  CHECK(diagnostics.matrix_constructed);
  CHECK(diagnostics.matrix_entries > 0);
  CHECK_FALSE(diagnostics.matrix_complete);
  CHECK_FALSE(diagnostics.matrix_solved);
  CHECK_FALSE(diagnostics.matrix_action_published);
}

TEST_CASE("root matrix zero-iteration RM preserves the staged fallback") {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  x[0] = x[1] = 0b000000111;
  for (int board = 3; board < 9; ++board) {
    x[board] = 227;
    o[board] = 284;
  }
  x[2] = static_cast<uint16_t>((1u << 3) | (1u << 4));
  o[2] = static_cast<uint16_t>((1u << 1) | (1u << 2));
  const auto witness = Position::from_parts(x, o, 2, Tie::X);
  REQUIRE(witness.has_value());

  wire::TurnRequest request;
  request.request_id = "rm-zero";
  request.ply = 0;
  request.ctx = {Seat::X, 100, 100};
  request.pos = *witness;
  request.time_ms = 2'000;
  MoveList legal;
  request.pos.legal_moves(legal);
  request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);

  EnginePolicy calibration_policy;
  CountingClock calibration_clock;
  static_cast<void>(calibration_policy.choose(request, calibration_clock));
  REQUIRE(calibration_policy.last_diagnostics().rm_iterations == 10'000);
  REQUIRE(calibration_clock.polls >= 10'001);

  std::optional<wire::TurnReply> stopped_reply;
  PolicyDiagnostics stopped_diagnostics;
  for (const std::size_t trailing_polls :
       {std::size_t{10'001}, std::size_t{10'000}}) {
    EnginePolicy stopped_policy;
    GateClock stopped_clock;
    stopped_clock.free_polls = calibration_clock.polls - trailing_polls;
    const wire::TurnReply reply = stopped_policy.choose(request, stopped_clock);
    const PolicyDiagnostics diagnostics = stopped_policy.last_diagnostics();
    if (diagnostics.matrix_complete && diagnostics.rm_iterations == 0) {
      stopped_reply = reply;
      stopped_diagnostics = diagnostics;
      break;
    }
  }

  REQUIRE(stopped_reply.has_value());
  CHECK(stopped_diagnostics.matrix_constructed);
  CHECK(stopped_diagnostics.matrix_complete);
  CHECK_FALSE(stopped_diagnostics.matrix_solved);
  CHECK_FALSE(stopped_diagnostics.matrix_action_published);
  CHECK(stopped_reply->bid == 0);
  Search<UtttModel> reference(16);
  const SearchResult staged = reference.solve(
      request.pos, request.pos.tie,
      Limits{stopped_reply->info.depth, 25'000, true, true, 12},
      make_play_window(request.ctx.budget_x,
                       request.ctx.budget_x + request.ctx.budget_o,
                       UtttModel::empties(request.pos)));
  const uint8_t expected =
      policy_detail::legal_preferred(request, staged.best_x);
  CHECK(stopped_reply->move == policy_detail::unflatten(expected));
}

TEST_CASE("root zero-total policy keeps its threshold TT unchanged") {
  wire::TurnRequest request;
  request.request_id = "zero-total-policy";
  request.ply = 0;
  request.ctx = {Seat::X, 0, 0};
  request.pos = Position::initial();
  request.time_ms = 2'000;
  MoveList legal;
  request.pos.legal_moves(legal);
  request.legal.assign(legal.m.begin(), legal.m.begin() + legal.n);

  EnginePolicy policy;
  FakeClock clock;
  const wire::TurnReply reply = policy.choose(request, clock);
  CHECK(reply.bid == 0);
  const PolicyDiagnostics diagnostics = policy.last_diagnostics();
  CHECK(diagnostics.matrix_constructed);
  CHECK(diagnostics.matrix_complete);
  CHECK(diagnostics.alternation_searches > 0);
  CHECK(diagnostics.alternation_memo_entries > 0);
  CHECK(diagnostics.threshold_tt_before_alt.collisions ==
        diagnostics.threshold_tt_after_alt.collisions);
  CHECK(diagnostics.threshold_tt_before_alt.hits ==
        diagnostics.threshold_tt_after_alt.hits);
  CHECK(diagnostics.threshold_tt_before_alt.misses ==
        diagnostics.threshold_tt_after_alt.misses);
  CHECK(diagnostics.threshold_tt_before_alt.stores ==
        diagnostics.threshold_tt_after_alt.stores);
}
