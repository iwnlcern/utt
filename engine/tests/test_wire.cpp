#include "doctest/doctest.h"
#include "adapter/wire.hpp"

#include "nlohmann/json.hpp"

#include <string>

using namespace uttt;
using namespace uttt::wire;
using json = nlohmann::json;

namespace {

const std::string kHello = R"({"type":"hello","protocol":1,"game_id":"g","you":"X","rules":"poorman-uttt-v1","time_ms":1000,"grace_ms":250,"budget":1000000000})";
const std::string kTurn = R"({"type":"turn","protocol":1,"game_id":"g","request_id":"r0","ply":0,"attempt":1,"you":"X","board":[".........",".........",".........",".........",".........",".........",".........",".........","........."],"forced":4,"legal":[[4,0],[4,1],[4,2],[4,3],[4,4],[4,5],[4,6],[4,7],[4,8]],"budgets":{"X":1000000000,"O":1000000000},"tie_owner":null,"time_ms":1000})";

std::string game_end(json overrides = json::object()) {
  json value = {{"type", "game_end"}, {"protocol", 1}, {"result", "X"},
                {"reason", "macro_win"}, {"budgets", {{"X", 7}, {"O", 3}}}};
  for (auto& [key, replacement] : overrides.items()) {
    if (replacement.is_discarded()) value.erase(key);
    else value[key] = replacement;
  }
  return value.dump();
}

}  // namespace

TEST_CASE("hello and turn parse against protocol-v1 literals") {
  auto hello = parse_hello(kHello);
  REQUIRE(hello.has_value());
  CHECK(hello->protocol == 1);
  CHECK(hello->you == Seat::X);
  CHECK(hello->rules == "poorman-uttt-v1");

  auto turn = parse_turn(kTurn);
  REQUIRE(turn.has_value());
  CHECK(turn->request_id == "r0");
  CHECK(turn->pos.forced == 4);
  CHECK(turn->legal.size() == 9);
  CHECK(turn->ctx.budget_x == 1000000000);
}

TEST_CASE("turn validation fails closed and ignores unknown keys") {
  json value = json::parse(kTurn);
  value["legal"] = json::array({json::array({4, 0})});
  CHECK_FALSE(parse_turn(value.dump()).has_value());
  value = json::parse(kTurn);
  value.erase("request_id");
  CHECK_FALSE(parse_turn(value.dump()).has_value());
  value = json::parse(kTurn);
  value["ply"] = 1;
  value["tie_owner"] = nullptr;
  CHECK_FALSE(parse_turn(value.dump()).has_value());
  value["tie_owner"] = "X";
  CHECK(parse_turn(value.dump()).has_value());
  value["extension"] = {"future", true};
  CHECK(parse_turn(value.dump()).has_value());
}

TEST_CASE("adapter ply-0 contract") {
  auto check_rejected = [](json value, const std::string& expected_error) {
    const auto result = parse_turn(value.dump());
    CHECK_FALSE(result.has_value());
    if (!result) CHECK(result.error() == expected_error);
  };

  json value = json::parse(kTurn);
  value["forced"] = 3;
  check_rejected(value, "ply-0 turn must carry forced 4");

  value = json::parse(kTurn);
  value["forced"] = nullptr;
  check_rejected(value, "ply-0 turn must carry forced 4");

  value = json::parse(kTurn);
  value["tie_owner"] = "X";
  check_rejected(value, "ply-0 turn must carry null tie_owner");

  value = json::parse(kTurn);
  value["ply"] = 1;
  value["forced"] = nullptr;
  value["tie_owner"] = "X";
  value["legal"] = json::array();
  for (int board = 0; board < 9; ++board) {
    for (int cell = 0; cell < 9; ++cell) {
      value["legal"].push_back({board, cell});
    }
  }
  CHECK(parse_turn(value.dump()).has_value());
}

TEST_CASE("reply serialization echoes request and always emits move") {
  TurnReply reply{"abc", 0, {4, 2}, {"estimate", std::nullopt, std::nullopt, 0, true}};
  const json value = json::parse(serialize_reply(reply));
  CHECK(value.at("type") == "turn");
  CHECK(value.at("protocol") == 1);
  CHECK(value.at("request_id") == "abc");
  CHECK(value.at("move") == json::array({4, 2}));
}

TEST_CASE("strict game_end battery") {
  CHECK(validate_game_end(game_end()).has_value());
  CHECK(validate_game_end(game_end({{"extra", 1}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"type", "turn"}})).has_value());

  json value = json::parse(game_end());
  value.erase("result");
  CHECK_FALSE(validate_game_end(value.dump()).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"result", 1}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"result", "Q"}})).has_value());
  value = json::parse(game_end());
  value.erase("reason");
  CHECK_FALSE(validate_game_end(value.dump()).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"reason", 2}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"reason", "unknown"}})).has_value());
  value = json::parse(game_end());
  value.erase("budgets");
  CHECK_FALSE(validate_game_end(value.dump()).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", json::array()}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", "bad"}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", {{"x", 1}, {"o", 1}}}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", {{"X", 1.5}, {"O", 1}}}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", {{"X", "5"}, {"O", 1}}}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", {{"X", -1}, {"O", 1}}}})).has_value());
  CHECK_FALSE(validate_game_end(game_end({{"budgets", {{"X", 1000000001}, {"O", 1}}}})).has_value());
}

TEST_CASE("line ceiling rejects before parsing") {
  auto malformed_oversize = classify(std::string(kMaxLineBytes + 1, '{'));
  REQUIRE_FALSE(malformed_oversize.has_value());
  CHECK(malformed_oversize.error() == "oversize line");

  const std::string well_formed_oversize =
      json{{"type", "hello"}, {"padding", std::string(kMaxLineBytes, 'x')}}.dump();
  REQUIRE(well_formed_oversize.size() > kMaxLineBytes);
  auto well_formed_result = classify(well_formed_oversize);
  REQUIRE_FALSE(well_formed_result.has_value());
  CHECK(well_formed_result.error() == "oversize line");

  std::string boundary = kHello;
  boundary.append(kMaxLineBytes - boundary.size(), ' ');
  CHECK(classify(boundary).has_value());
}
