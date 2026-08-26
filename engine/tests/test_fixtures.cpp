#include "doctest/doctest.h"
#include "core/budget.hpp"
#include "core/position.hpp"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace uttt;
using json = nlohmann::json;

namespace {

std::expected<Position, std::string> position_from_state(const json& state) {
  if (!state.contains("board") || !state["board"].is_array() || state["board"].size() != 9) {
    return std::unexpected("state.board must contain nine local-board strings");
  }
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  for (int b = 0; b < 9; ++b) {
    if (!state["board"][b].is_string()) return std::unexpected("board entry is not a string");
    const std::string row = state["board"][b].get<std::string>();
    if (row.size() != 9) return std::unexpected("board entry length is not nine");
    for (int c = 0; c < 9; ++c) {
      if (row[c] == 'X') x[b] |= uint16_t(1u << c);
      else if (row[c] == 'O') o[b] |= uint16_t(1u << c);
      else if (row[c] != '.') return std::unexpected("invalid board cell");
    }
  }
  const int8_t forced = state.at("forced").is_null()
      ? kForcedAny
      : static_cast<int8_t>(state.at("forced").get<int>());
  TieState tie = TieState::NullFirstMove;
  if (!state.at("h").is_null()) {
    tie = state.at("h").get<std::string>() == "X" ? TieState::X : TieState::O;
  }
  auto imported = Position::from_parts(x, o, forced, tie);
  if (!imported) return std::unexpected("Position::from_parts rejected fixture state");
  return imported.value();
}

std::vector<Move> sorted_moves(const json& values) {
  std::vector<Move> result;
  for (const auto& value : values) {
    result.push_back({value.at(0).get<uint8_t>(), value.at(1).get<uint8_t>()});
  }
  std::sort(result.begin(), result.end(), [](Move a, Move b) {
    return a.board < b.board || (a.board == b.board && a.cell < b.cell);
  });
  return result;
}

bool has_recognized_expectation(const json& fixture) {
  if (fixture.contains("expected_legal_moves") || fixture.contains("expected_result")) return true;
  return fixture.contains("move") &&
      (fixture.contains("expected_forced") || fixture.contains("expected_closures") ||
       fixture.contains("expected_terminal"));
}

void run_engine_fixture(const json& fixture) {
  const auto imported = position_from_state(fixture.at("state"));
  REQUIRE(imported.has_value());
  const Position p = imported.value();
  if (fixture.contains("expected_legal_moves")) {
    MoveList actual_list;
    p.legal_moves(actual_list);
    std::vector<Move> actual(actual_list.m.begin(), actual_list.m.begin() + actual_list.n);
    std::sort(actual.begin(), actual.end(), [](Move a, Move b) {
      return a.board < b.board || (a.board == b.board && a.cell < b.cell);
    });
    REQUIRE(actual == sorted_moves(fixture.at("expected_legal_moves")));
  }
  if (fixture.contains("move")) {
    const auto& raw_move = fixture.at("move");
    const Move move{raw_move.at(0).get<uint8_t>(), raw_move.at(1).get<uint8_t>()};
    Seat mover = Seat::X;
    if (fixture.contains("mover") && fixture.at("mover") == "O") mover = Seat::O;
    const auto child = p.applied(move, mover);
    REQUIRE(child.has_value());
    if (fixture.contains("expected_forced")) {
      const int expected = fixture.at("expected_forced").is_null()
          ? kForcedAny
          : fixture.at("expected_forced").get<int>();
      CHECK(child->forced == expected);
    }
    if (fixture.contains("expected_closures")) {
      std::vector<int> actual_closed;
      for (int b = 0; b < 9; ++b) {
        if (((child->closed ^ p.closed) >> b) & 1u) actual_closed.push_back(b);
      }
      CHECK(actual_closed == fixture.at("expected_closures").get<std::vector<int>>());
    }
    if (fixture.contains("expected_terminal")) {
      CHECK((child->terminal() != TerminalKind::None) == fixture.at("expected_terminal").get<bool>());
    }
  }
  if (fixture.contains("expected_result")) {
    const std::string result = fixture.at("expected_result").get<std::string>();
    const auto terminal = p.terminal();
    if (result == "X") CHECK((terminal == TerminalKind::MacroWinX || terminal == TerminalKind::AllClosed));
    if (result == "O") CHECK((terminal == TerminalKind::MacroWinO || terminal == TerminalKind::AllClosed));
    if (result == "draw") CHECK(terminal == TerminalKind::AllClosed);
    if (fixture.contains("chip_margin")) {
      const int64_t margin = fixture.at("chip_margin").get<int64_t>();
      const ChipResult expected = margin > 0 ? ChipResult::XWins
          : margin < 0 ? ChipResult::OWins : ChipResult::Draw;
      CHECK(winner_on_chips(std::max<int64_t>(margin, 0), std::max<int64_t>(-margin, 0)) == expected);
    }
  }
}

}  // namespace

TEST_CASE("fixture consumption requires a recognized engine expectation") {
  json fixture = {{"state", json::object()}};
  CHECK_FALSE(has_recognized_expectation(fixture));

  fixture["move"] = {0, 0};
  CHECK_FALSE(has_recognized_expectation(fixture));

  fixture["expected_forced"] = nullptr;
  CHECK(has_recognized_expectation(fixture));

  fixture.erase("move");
  CHECK_FALSE(has_recognized_expectation(fixture));

  fixture["expected_result"] = "draw";
  CHECK(has_recognized_expectation(fixture));
}

TEST_CASE("theory schema-v1 UTTT fixtures") {
  const char* override_dir = std::getenv("UTTT_FIXTURES_DIR");
  const std::filesystem::path directory = override_dir == nullptr
      ? std::filesystem::path("../../theory/fixtures")
      : std::filesystem::path(override_dir);
  std::vector<std::filesystem::path> files;
  if (std::filesystem::is_directory(directory)) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      if (entry.is_regular_file() && entry.path().extension() == ".json" &&
          entry.path().filename() != "schema-v1.json") files.push_back(entry.path());
    }
  }
  int consumed = 0;
  for (const auto& path : files) {
    std::ifstream input(path);
    const json root = json::parse(input);
    if (root.value("schema_version", 0) != 1 || root.value("game", "") != "uttt") continue;
    for (const auto& fixture : root.at("fixtures")) {
      if (fixture.contains("consumed_by") &&
          std::find(fixture.at("consumed_by").begin(), fixture.at("consumed_by").end(), "engine") ==
              fixture.at("consumed_by").end()) continue;
      CAPTURE(path.string());
      CAPTURE(fixture.at("id").get<std::string>());
      REQUIRE_MESSAGE(has_recognized_expectation(fixture),
                      "fixture has no recognized engine expectation key");
      run_engine_fixture(fixture);
      ++consumed;
    }
  }
  if (consumed == 0) {
    if (std::getenv("UTTT_ALLOW_MISSING_FIXTURES") != nullptr &&
        std::string(std::getenv("UTTT_ALLOW_MISSING_FIXTURES")) == "1") {
      WARN("theory fixtures not present — acceptance criterion 1 is unmet until theory publishes");
    } else {
      FAIL("theory fixtures not present — acceptance criterion 1 is unmet until theory publishes");
    }
  }
}

TEST_CASE("from_parts rejects every ImportError class") {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  x[0] = 1;
  o[0] = 1;
  CHECK(Position::from_parts(x, o, kForcedAny, TieState::X).error() == ImportError::OverlappingMasks);

  x = {};
  o = {};
  x[0] = 0b000000111;
  o[0] = 0b000111000;
  CHECK(Position::from_parts(x, o, kForcedAny, TieState::X).error() == ImportError::InvalidLocal);
  x[0] = 0b000111111;
  o[0] = 0;
  CHECK(Position::from_parts(x, o, kForcedAny, TieState::X).error() == ImportError::InvalidLocal);

  x = {};
  o = {};
  CHECK(Position::from_parts(x, o, 9, TieState::X).error() == ImportError::ForcedOutOfDomain);
  x[0] = 0b000000111;
  CHECK(Position::from_parts(x, o, 0, TieState::X).error() == ImportError::ForcedToClosedBoard);
  CHECK(Position::from_parts(x, o, kForcedAny, TieState::NullFirstMove).error() ==
        ImportError::TieNullAfterFirstMark);
  x = {};
  CHECK(Position::from_parts(x, o, kForcedAny, static_cast<TieState>(99)).error() ==
        ImportError::TieOutOfDomain);
}
