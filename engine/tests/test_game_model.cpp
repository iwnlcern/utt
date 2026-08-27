#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "doctest/doctest.h"
#include "nlohmann/json.hpp"
#include "search/game_model.hpp"
#include "support/ttt3_model.hpp"

using namespace uttt;
using json = nlohmann::json;

static_assert(GameModel<UtttModel>);
static_assert(GameModel<Ttt3Model>);

namespace {

constexpr uint8_t encode_move(Move move) {
  return static_cast<uint8_t>(9 * move.board + move.cell);
}

TieState fixture_tie(const json& state) {
  if (state.at("h").is_null()) return TieState::NullFirstMove;
  return state.at("h").get<std::string>() == "X" ? TieState::X : TieState::O;
}

Position fixture_position(const json& state) {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  for (int board = 0; board < 9; ++board) {
    const std::string cells = state.at("board").at(board).get<std::string>();
    for (int cell = 0; cell < 9; ++cell) {
      if (cells[cell] == 'X') x[board] |= uint16_t(1u << cell);
      if (cells[cell] == 'O') o[board] |= uint16_t(1u << cell);
    }
  }
  const int8_t forced =
      state.at("forced").is_null()
          ? kForcedAny
          : static_cast<int8_t>(state.at("forced").get<int>());
  auto imported = Position::from_parts(x, o, forced, fixture_tie(state));
  REQUIRE(imported.has_value());
  return imported.value();
}

std::vector<json> engine_rows(std::string_view filename) {
  const char* override_dir = std::getenv("UTTT_FIXTURES_DIR");
  const std::filesystem::path directory =
      override_dir == nullptr ? std::filesystem::path("../../theory/fixtures")
                              : std::filesystem::path(override_dir);
  std::ifstream input(directory / filename);
  REQUIRE_MESSAGE(input.good(), "required theory fixture file is missing");
  const json root = json::parse(input);
  REQUIRE(root.at("game") == "uttt");

  std::vector<json> result;
  for (const auto& fixture : root.at("fixtures")) {
    const auto& consumers = fixture.at("consumed_by");
    if (std::find(consumers.begin(), consumers.end(), "engine") !=
        consumers.end()) {
      result.push_back(fixture);
    }
  }
  return result;
}

std::vector<uint8_t> encoded_moves(const json& expected) {
  std::vector<uint8_t> result;
  for (const auto& move : expected) {
    result.push_back(static_cast<uint8_t>(9 * move.at(0).get<int>() +
                                          move.at(1).get<int>()));
  }
  return result;
}

std::vector<uint8_t> child_moves(
    const std::vector<ModelChild<Position>>& children) {
  std::vector<uint8_t> result;
  for (const auto& child : children) result.push_back(child.move);
  return result;
}

Ttt3State ttt3(std::string_view board, TieState tie = TieState::X) {
  return Ttt3State::from_board(board, tie);
}

}  // namespace

TEST_CASE(
    "UtttModel child enumeration delegates exactly to core movegen on "
    "three positions") {
  std::array<Position, 3> positions;
  positions[0] = Position::initial();
  positions[1] = positions[0].applied({4, 4}, Seat::X).value();
  positions[2] = positions[1].applied({4, 0}, Seat::O).value();

  CHECK(UtttModel::empties(positions[0]) == 81);
  CHECK(UtttModel::empties(positions[1]) == 80);
  CHECK(UtttModel::empties(positions[2]) == 79);

  for (const Position& position : positions) {
    MoveList moves;
    position.legal_moves(moves);
    const auto x_children = UtttModel::children_x(position);
    const auto o_children = UtttModel::children_o(position);

    REQUIRE(x_children.size() == moves.n);
    REQUIRE(o_children.size() == moves.n);
    for (int i = 0; i < moves.n; ++i) {
      CAPTURE(i);
      CHECK(x_children[i].move == encode_move(moves.m[i]));
      CHECK(o_children[i].move == encode_move(moves.m[i]));
      CHECK(x_children[i].state.identity_equal(
          position.applied(moves.m[i], Seat::X).value()));
      CHECK(o_children[i].state.identity_equal(
          position.applied(moves.m[i], Seat::O).value()));
      CHECK(x_children[i].state.tie == TieState::O);
      CHECK(o_children[i].state.tie == TieState::X);
    }
  }
}

TEST_CASE(
    "UtttModel consumes all seven actual engine-tagged legality and "
    "terminal rows") {
  const auto legality = engine_rows("legality.json");
  const auto terminal = engine_rows("terminal.json");
  REQUIRE(legality.size() == 3);
  REQUIRE(terminal.size() == 4);

  for (const auto& fixture : legality) {
    CAPTURE(fixture.at("id").get<std::string>());
    const Position state = fixture_position(fixture.at("state"));
    const auto expected = encoded_moves(fixture.at("expected_legal_moves"));
    CHECK(child_moves(UtttModel::children_x(state)) == expected);
    CHECK(child_moves(UtttModel::children_o(state)) == expected);
    CHECK(UtttModel::terminal(state) == TerminalKind::None);
  }

  for (const auto& fixture : terminal) {
    CAPTURE(fixture.at("id").get<std::string>());
    const Position state = fixture_position(fixture.at("state"));
    const std::string expected =
        fixture.at("expected_result").get<std::string>();
    if (fixture.at("id") == "terminal-x-macro-win-outright") {
      CHECK(UtttModel::terminal(state) == TerminalKind::MacroWinX);
    } else {
      CHECK(UtttModel::terminal(state) == TerminalKind::AllClosed);
      const int bx = fixture.at("state").at("budgets").at("x").get<int>();
      const int bo = fixture.at("state").at("budgets").at("o").get<int>();
      const int expected_sign = expected == "X" ? 1 : expected == "O" ? -1 : 0;
      CHECK(UtttModel::chip_sign(state, bx, bo) == expected_sign);
    }
  }
}

TEST_CASE(
    "Ttt3Model engine-authored hand states pin ascending legality and "
    "terminals") {
  const auto opening = ttt3(".........", TieState::X);
  const auto opening_x = Ttt3Model::children_x(opening);
  const auto opening_o = Ttt3Model::children_o(opening);
  REQUIRE(opening_x.size() == 9);
  REQUIRE(opening_o.size() == 9);
  for (uint8_t cell = 0; cell < 9; ++cell) {
    CHECK(opening_x[cell].move == cell);
    CHECK(opening_o[cell].move == cell);
    CHECK(opening_x[cell].state.tie == TieState::O);
    CHECK(opening_o[cell].state.tie == TieState::X);
  }

  const auto one_empty = ttt3(".OXXOOOXX", TieState::O);
  const auto one_empty_children = Ttt3Model::children_x(one_empty);
  REQUIRE(one_empty_children.size() == 1);
  CHECK(one_empty_children.front().move == 0);
  CHECK(Ttt3Model::terminal(one_empty) == TerminalKind::None);

  const auto x_win = ttt3("XXX......", TieState::O);
  CHECK(Ttt3Model::terminal(x_win) == TerminalKind::MacroWinX);
  CHECK(Ttt3Model::children_o(x_win).empty());

  const auto o_win = ttt3("O..O..O..", TieState::X);
  CHECK(Ttt3Model::terminal(o_win) == TerminalKind::MacroWinO);
  CHECK(Ttt3Model::children_x(o_win).empty());

  const auto draw = ttt3("XOXXOOOXX", TieState::X);
  CHECK(Ttt3Model::terminal(draw) == TerminalKind::AllClosed);
  CHECK(Ttt3Model::empties(draw) == 0);
  CHECK(Ttt3Model::chip_sign(draw, 6, 5) == 1);
  CHECK(Ttt3Model::chip_sign(draw, 5, 6) == -1);
  CHECK(Ttt3Model::chip_sign(draw, 5, 5) == 0);
}

TEST_CASE(
    "GameModel key and identity seams include independent tags and tie "
    "identity") {
  const Position position =
      Position::initial().applied({4, 2}, Seat::X).value();
  const TTKey key = UtttModel::tt_key(position);
  CHECK(key.key == position.key);
  CHECK(key.tag == zobrist_tag_full(position));
  const PosId id = UtttModel::pos_id(position);
  CHECK(id.x == position.x);
  CHECK(id.o == position.o);
  CHECK(id.forced == position.forced);
  CHECK(id.tie == position.tie);

  const Ttt3State board_x = ttt3("X.O......", TieState::X);
  const Ttt3State board_o = ttt3("X.O......", TieState::O);
  const TTKey ttt3_key = Ttt3Model::tt_key(board_x);
  CHECK(ttt3_key.key == board_x.dense_code());
  CHECK(ttt3_key.tag == 0xEF431A44u);
  CHECK(ttt3_key.tag != static_cast<uint32_t>(ttt3_key.key));
  CHECK(Ttt3Model::tt_key(board_o) == ttt3_key);
  const PosId board_x_id = Ttt3Model::pos_id(board_x);
  CHECK(board_x_id.x[0] == board_x.x);
  CHECK(board_x_id.o[0] == board_x.o);
  CHECK(board_x_id.tie == TieState::X);
  CHECK(board_x_id != Ttt3Model::pos_id(board_o));
}
