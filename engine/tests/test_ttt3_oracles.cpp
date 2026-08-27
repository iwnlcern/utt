#include "doctest/doctest.h"
#include "nlohmann/json.hpp"
#include "support/ttt3_continuous.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
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
