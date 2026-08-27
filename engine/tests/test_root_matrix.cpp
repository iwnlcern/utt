#include "adapter/policy.hpp"
#include "doctest/doctest.h"
#include "nlohmann/json.hpp"
#include "root/bid_matrix.hpp"
#include "root/rmplus.hpp"
#include "support/test_rational.hpp"
#include "support/ttt3_continuous.hpp"
#include "support/ttt3_discrete.hpp"

#include "core/clock.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
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
            static_cast<int>(solve_discrete(child, h2, static_cast<int>(bx2),
                                            static_cast<int>(bo2))) -
                1,
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
            reference.at("matrix").get<std::vector<std::vector<int>>>());
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

TEST_CASE("root matrix production midpoint ordinal taints in-band entries") {
  const Ttt3State child = Ttt3State::from_board(".........", Tie::X);
  const SearchResult midpoint{{0.5, 0.5}, 0, 0, Quality::Bound, 4, true};

  const PayoffResult x_side =
      production_payoff<Ttt3Model>(child, Tie::X, 6, 4, midpoint);
  const PayoffResult o_side =
      production_payoff<Ttt3Model>(child, Tie::O, 4, 6, midpoint);
  const PayoffResult equality =
      production_payoff<Ttt3Model>(child, Tie::X, 5, 5, midpoint);
  CHECK(x_side == PayoffResult{+1, false});
  CHECK(o_side == PayoffResult{-1, false});
  CHECK(equality == PayoffResult{0, false});

  const Anchors anchors{5, 0, 0};
  PayoffFn<Ttt3State> in_band = [&](Ttt3State next, Tie h2, int64_t bx2,
                                    int64_t bo2) {
    return production_payoff<Ttt3Model>(next, h2, bx2, bo2, midpoint);
  };
  const auto matrix = build_bid_matrix<Ttt3Model>(child, Tie::X, 5, 5, anchors,
                                                  std::move(in_band));
  CHECK_FALSE(matrix.all_exact);
  CHECK(matrix_quality(Quality::Exact, matrix.all_exact) == Quality::Estimate);
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
  CHECK(forced.matrix_bypassed);
  CHECK_FALSE(forced.matrix_constructed);
  CHECK_FALSE(forced.matrix_solved);
  CHECK_FALSE(forced.matrix_action_published);
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
  CHECK(diagnostics.child_search_cancelled);
  CHECK(diagnostics.matrix_constructed);
  CHECK(diagnostics.matrix_entries == 0);
  CHECK_FALSE(diagnostics.matrix_complete);
  CHECK_FALSE(diagnostics.matrix_solved);
  CHECK_FALSE(diagnostics.matrix_action_published);
}

TEST_CASE("root matrix zero-iteration RM preserves the staged fallback") {
  wire::TurnRequest request;
  request.request_id = "rm-zero";
  request.ply = 0;
  request.ctx = {Seat::X, 100, 100};
  request.pos = Position::initial();
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
  CHECK(stopped_reply->move == request.legal.front());
}
