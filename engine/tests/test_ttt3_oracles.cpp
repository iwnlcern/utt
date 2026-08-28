#include "doctest/doctest.h"
#include "nlohmann/json.hpp"
#include "support/ttt3_discrete.hpp"

#include "support/ttt3_continuous.hpp"

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <limits>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

using namespace uttt;
using json = nlohmann::json;

namespace {

TestRational parse_rational(std::string_view text) {
  const std::size_t slash = text.find('/');
  if (slash == std::string_view::npos) {
    return TestRational{std::stoll(std::string{text})};
  }
  return TestRational{std::stoll(std::string{text.substr(0, slash)}),
                      std::stoll(std::string{text.substr(slash + 1)})};
}

Tie fixture_tie(const json &state) {
  const std::string owner = state.at("h").get<std::string>();
  REQUIRE((owner == "X" || owner == "O"));
  return owner == "X" ? Tie::X : Tie::O;
}

std::vector<json> engine_threshold_rows() {
  const char *override_dir = std::getenv("UTTT_FIXTURES_DIR");
  const std::filesystem::path directory =
      override_dir == nullptr ? std::filesystem::path("../../theory/fixtures")
                              : std::filesystem::path(override_dir);
  std::ifstream input(directory / "thresholds_ttt3.json");
  REQUIRE_MESSAGE(input.good(), "required ttt3 threshold fixture is missing");
  const json root = json::parse(input);
  REQUIRE(root.at("schema_version") == 1);
  REQUIRE(root.at("game") == "ttt3");

  std::vector<json> result;
  for (const auto &fixture : root.at("fixtures")) {
    const auto &consumers = fixture.at("consumed_by");
    if (std::find(consumers.begin(), consumers.end(), "engine") !=
        consumers.end()) {
      result.push_back(fixture);
    }
  }
  return result;
}

Outcome fixture_outcome(std::string_view result) {
  if (result == "X")
    return XWin;
  if (result == "O")
    return OWin;
  REQUIRE(result == "draw");
  return Draw;
}

uint64_t discrete_position_key(const Ttt3State &state, Tie tie, int bx,
                               int bo) {
  return uint64_t{state.x} | (uint64_t{state.o} << 9) |
         (uint64_t{tie == Tie::O} << 18) | (static_cast<uint64_t>(bx) << 19) |
         (static_cast<uint64_t>(bo) << 26);
}

struct ReachableDiscretePosition {
  Ttt3State state;
  Tie tie;
  int bx;
  int bo;
};

std::unordered_set<uint64_t> reachable_discrete_positions(int scale) {
  std::unordered_set<uint64_t> reached;
  std::deque<ReachableDiscretePosition> pending;
  const auto enqueue = [&](Ttt3State state, Tie tie, int bx, int bo) {
    state.tie = tie;
    const uint64_t key = discrete_position_key(state, tie, bx, bo);
    if (reached.insert(key).second) {
      pending.push_back({state, tie, bx, bo});
    }
  };

  const Ttt3State root = Ttt3State::from_board(".........", Tie::X);
  enqueue(root, Tie::X, scale, scale);
  enqueue(root, Tie::O, scale, scale);

  while (!pending.empty()) {
    const ReachableDiscretePosition current = pending.front();
    pending.pop_front();
    if (Ttt3Model::terminal(current.state) != TerminalKind::None)
      continue;

    const auto x_children = Ttt3Model::children_x(current.state);
    const auto o_children = Ttt3Model::children_o(current.state);
    for (int bid_x = 0; bid_x <= current.bx; ++bid_x) {
      for (int bid_o = 0; bid_o <= current.bo; ++bid_o) {
        const bool x_moves =
            bid_x > bid_o || (bid_x == bid_o && current.tie == Tie::X);
        if (x_moves) {
          for (const auto &child : x_children) {
            enqueue(child.state, Tie::O, current.bx - bid_x, current.bo);
          }
        } else {
          for (const auto &child : o_children) {
            enqueue(child.state, Tie::X, current.bx, current.bo - bid_o);
          }
        }
      }
    }
  }
  return reached;
}

std::string board_from_dense(uint32_t code) {
  std::string board(9, '.');
  for (char &cell : board) {
    const uint32_t digit = code % 3;
    code /= 3;
    if (digit == 1)
      cell = 'X';
    if (digit == 2)
      cell = 'O';
  }
  return board;
}

} // namespace

TEST_CASE(
    "exact continuous ttt3 oracle matches all four engine threshold rows") {
  const auto fixtures = engine_threshold_rows();
  REQUIRE(fixtures.size() == 4);

  std::vector<std::string> ids;
  for (const auto &fixture : fixtures) {
    ids.push_back(fixture.at("id").get<std::string>());
  }
  std::sort(ids.begin(), ids.end());
  CHECK(ids ==
        std::vector<std::string>{"threshold-a-equals-b-zero-critical",
                                 "threshold-a-less-than-b-after-x-opening",
                                 "threshold-root-conditional-h-o",
                                 "threshold-root-conditional-h-x"});

  for (const auto &fixture : fixtures) {
    const std::string id = fixture.at("id").get<std::string>();
    CAPTURE(id);
    const json &state = fixture.at("state");
    const Tie tie = fixture_tie(state);
    const std::string tie_name = tie == Tie::X ? "X" : "O";
    const ContNode actual = solve_continuous(
        Ttt3State::from_board(state.at("board").get<std::string>(), tie), tie);

    CHECK(actual.T ==
          parse_rational(
              fixture.at("expected_T").at(tie_name).get<std::string>()));
    if (fixture.contains("expected_critical_bid")) {
      CHECK(actual.r ==
            parse_rational(
                fixture.at("expected_critical_bid").get<std::string>()));
    }
    CHECK(actual.best_x ==
          fixture.at("expected_best_move").at("if_X_wins").get<uint8_t>());
    CHECK(actual.best_o ==
          fixture.at("expected_best_move").at("if_O_wins").get<uint8_t>());
  }
}

TEST_CASE("exact continuous ttt3 retains the first move for tied extrema") {
  const ContNode tied =
      solve_continuous(Ttt3State::from_board("O..OXXXXO", Tie::X), Tie::X);

  CHECK(tied.best_x == 1);
  CHECK(tied.best_o == 1);
}

TEST_CASE("exact continuous ttt3 terminals are zero one and one half") {
  const ContNode x_win =
      solve_continuous(Ttt3State::from_board("XXX......", Tie::O), Tie::O);
  const ContNode o_win =
      solve_continuous(Ttt3State::from_board("O..O..O..", Tie::X), Tie::X);
  const ContNode draw =
      solve_continuous(Ttt3State::from_board("XOXXOOOXX", Tie::X), Tie::X);

  CHECK(x_win.T == TestRational{0});
  CHECK(o_win.T == TestRational{1});
  CHECK(draw.T == TestRational{1, 2});
}

TEST_CASE("exact discrete ttt3 matches fixture equality witnesses") {
  const auto fixtures = engine_threshold_rows();
  std::size_t checked = 0;

  for (const auto &fixture : fixtures) {
    const std::string id = fixture.at("id").get<std::string>();
    if (id != "threshold-a-equals-b-zero-critical" &&
        id != "threshold-root-conditional-h-x" &&
        id != "threshold-root-conditional-h-o") {
      continue;
    }
    const json &state = fixture.at("state");
    const Tie tie = fixture_tie(state);
    const Ttt3State board =
        Ttt3State::from_board(state.at("board").get<std::string>(), tie);
    for (const auto &expectation : fixture.at("discrete_expectations")) {
      const int bx = expectation.at("bx").get<int>();
      const int bo = expectation.at("bo").get<int>();
      const int scale = expectation.at("scale").get<int>();
      CAPTURE(id);
      CAPTURE(tie);
      CAPTURE(bx);
      CAPTURE(bo);
      CAPTURE(scale);
      CHECK(solve_discrete(board, tie, bx, bo) ==
            fixture_outcome(expectation.at("result").get<std::string>()));
      ++checked;
    }
  }

  CHECK(checked == 9);
}

TEST_CASE("exact discrete ttt3 agrees outside the continuous N4 band") {
  constexpr int scale = 4;
  const auto reachable = reachable_discrete_positions(scale);
  std::size_t checked = 0;

  for (uint32_t code = 0; code < 19683; ++code) {
    const std::string board_text = board_from_dense(code);
    for (Tie tie : {Tie::X, Tie::O}) {
      const Ttt3State state = Ttt3State::from_board(board_text, tie);
      if (Ttt3Model::terminal(state) != TerminalKind::None)
        continue;
      const TestRational threshold = solve_continuous(state, tie).T;
      for (int bx = 0; bx <= scale; ++bx) {
        for (int bo = 0; bo <= scale; ++bo) {
          const int total = bx + bo;
          if (total == 0 ||
              !reachable.contains(discrete_position_key(state, tie, bx, bo))) {
            continue;
          }
          const TestRational ratio{bx, total};
          const TestRational distance =
              ratio < threshold ? threshold - ratio : ratio - threshold;
          const TestRational band{Ttt3Model::empties(state), total};
          if (distance <= band)
            continue;

          CAPTURE(board_text);
          CAPTURE(tie);
          CAPTURE(bx);
          CAPTURE(bo);
          const Outcome expected = ratio > threshold ? XWin : OWin;
          CHECK(solve_discrete(state, tie, bx, bo) == expected);
          ++checked;
        }
      }
    }
  }

  CHECK(checked == 6060);
}

TEST_CASE("exact discrete ttt3 rejects totals beyond its bounded scale") {
  const Ttt3State terminal = Ttt3State::from_board("XXX......", Tie::X);

  CHECK_THROWS_AS(solve_discrete(terminal, Tie::X, 65, 0),
                  std::invalid_argument);
  CHECK_THROWS_AS(solve_discrete(terminal, Tie::X,
                                 std::numeric_limits<int>::max(),
                                 std::numeric_limits<int>::max()),
                  std::invalid_argument);
}
