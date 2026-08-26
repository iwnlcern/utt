#include "adapter/wire.hpp"

#include "nlohmann/json.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <set>

namespace uttt::wire {
namespace {

using json = nlohmann::json;

std::expected<json, std::string> parse_json(std::string_view line) {
  if (line.size() > kMaxLineBytes) return std::unexpected("oversize line");
  try {
    json value = json::parse(line);
    if (!value.is_object()) return std::unexpected("message must be an object");
    return value;
  } catch (const std::exception& error) {
    return std::unexpected(std::string("invalid JSON: ") + error.what());
  }
}

bool string_field(const json& value, const char* key, std::string& out) {
  if (!value.contains(key) || !value.at(key).is_string()) return false;
  out = value.at(key).get<std::string>();
  return true;
}

bool integer_value(const json& value, int64_t minimum, int64_t maximum, int64_t& out) {
  if (!value.is_number_integer()) return false;
  try {
    out = value.get<int64_t>();
  } catch (const std::exception&) {
    return false;
  }
  return out >= minimum && out <= maximum;
}

bool integer_field(const json& value, const char* key, int64_t minimum,
                   int64_t maximum, int64_t& out) {
  return value.contains(key) && integer_value(value.at(key), minimum, maximum, out);
}

bool constant_fields(const json& value, std::string_view type) {
  int64_t protocol = 0;
  return value.contains("type") && value.at("type").is_string() &&
      value.at("type").get<std::string>() == type &&
      integer_field(value, "protocol", 1, 1, protocol);
}

std::expected<Seat, std::string> parse_seat(const json& value) {
  if (!value.is_string()) return std::unexpected("seat is not a string");
  const std::string seat = value.get<std::string>();
  if (seat == "X") return Seat::X;
  if (seat == "O") return Seat::O;
  return std::unexpected("seat is outside X/O");
}

std::expected<Move, std::string> parse_move(const json& value) {
  if (!value.is_array() || value.size() != 2) return std::unexpected("move must have two cells");
  int64_t board = 0;
  int64_t cell = 0;
  if (!integer_value(value.at(0), 0, 8, board) || !integer_value(value.at(1), 0, 8, cell)) {
    return std::unexpected("move coordinate outside 0..8");
  }
  return Move{static_cast<uint8_t>(board), static_cast<uint8_t>(cell)};
}

std::expected<RootContext, std::string> parse_context(const json& value, Seat seat,
                                                      bool exact_keys) {
  if (!value.is_object() || !value.contains("X") || !value.contains("O")) {
    return std::unexpected("budgets must contain canonical X/O keys");
  }
  if (exact_keys && value.size() != 2) return std::unexpected("budgets has noncanonical keys");
  int64_t x = 0;
  int64_t o = 0;
  if (!integer_value(value.at("X"), 0, 1000000000, x) ||
      !integer_value(value.at("O"), 0, 1000000000, o)) {
    return std::unexpected("budget is not an integer in range");
  }
  return RootContext{seat, x, o};
}

std::expected<Position, std::string> parse_position(const json& value, int ply) {
  if (!value.contains("board") || !value.at("board").is_array() ||
      value.at("board").size() != 9) {
    return std::unexpected("board must contain nine strings");
  }
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  for (int b = 0; b < 9; ++b) {
    const auto& local = value.at("board").at(b);
    if (!local.is_string()) return std::unexpected("local board must be a string");
    const std::string cells = local.get<std::string>();
    if (cells.size() != 9) return std::unexpected("local board must contain nine cells");
    for (int c = 0; c < 9; ++c) {
      if (cells[c] == 'X') x[b] |= uint16_t(1u << c);
      else if (cells[c] == 'O') o[b] |= uint16_t(1u << c);
      else if (cells[c] != '.') return std::unexpected("board contains an invalid cell");
    }
  }

  if (!value.contains("forced")) return std::unexpected("forced missing");
  int8_t forced = kForcedAny;
  if (!value.at("forced").is_null()) {
    int64_t raw = 0;
    if (!integer_value(value.at("forced"), 0, 8, raw)) return std::unexpected("forced invalid");
    forced = static_cast<int8_t>(raw);
  }
  if (ply == 0 && forced != 4) {
    return std::unexpected("ply-0 turn must carry forced 4");
  }
  if (!value.contains("tie_owner")) return std::unexpected("tie_owner missing");
  TieState tie = TieState::NullFirstMove;
  if (value.at("tie_owner").is_null()) {
    if (ply != 0) return std::unexpected("null tie_owner only allowed at ply 0");
  } else {
    if (ply == 0) return std::unexpected("ply-0 turn must carry null tie_owner");
    auto seat = parse_seat(value.at("tie_owner"));
    if (!seat) return std::unexpected(seat.error());
    tie = *seat == Seat::X ? TieState::X : TieState::O;
  }
  auto position = Position::from_parts(x, o, forced, tie);
  if (!position) return std::unexpected("position import rejected");
  return *position;
}

}  // namespace

std::expected<MsgType, std::string> classify(std::string_view line) {
  auto parsed = parse_json(line);
  if (!parsed) return std::unexpected(parsed.error());
  if (!parsed->contains("type") || !parsed->at("type").is_string()) {
    return std::unexpected("type missing or invalid");
  }
  const std::string type = parsed->at("type").get<std::string>();
  if (type == "hello") return MsgType::Hello;
  if (type == "turn") return MsgType::Turn;
  if (type == "game_end") return MsgType::GameEnd;
  return std::unexpected("unknown message type");
}

std::expected<HelloRequest, std::string> parse_hello(std::string_view line) {
  auto parsed = parse_json(line);
  if (!parsed) return std::unexpected(parsed.error());
  if (!constant_fields(*parsed, "hello")) return std::unexpected("invalid hello type/protocol");
  HelloRequest result{};
  result.protocol = 1;
  if (!string_field(*parsed, "game_id", result.game_id) ||
      !string_field(*parsed, "rules", result.rules) || result.rules != "poorman-uttt-v1") {
    return std::unexpected("invalid hello string field");
  }
  if (!parsed->contains("you")) return std::unexpected("hello you missing");
  auto seat = parse_seat(parsed->at("you"));
  if (!seat) return std::unexpected(seat.error());
  result.you = *seat;
  if (!integer_field(*parsed, "time_ms", 0, std::numeric_limits<int64_t>::max(), result.time_ms) ||
      !integer_field(*parsed, "grace_ms", 0, std::numeric_limits<int64_t>::max(), result.grace_ms) ||
      !integer_field(*parsed, "budget", 0, 1000000000, result.budget)) {
    return std::unexpected("invalid hello numeric field");
  }
  return result;
}

std::expected<TurnRequest, std::string> parse_turn(std::string_view line) {
  auto parsed = parse_json(line);
  if (!parsed) return std::unexpected(parsed.error());
  if (!constant_fields(*parsed, "turn")) return std::unexpected("invalid turn type/protocol");
  TurnRequest result{};
  if (!string_field(*parsed, "game_id", result.game_id) ||
      !string_field(*parsed, "request_id", result.request_id)) {
    return std::unexpected("invalid turn identifier");
  }
  int64_t ply = 0;
  int64_t attempt = 0;
  if (!integer_field(*parsed, "ply", 0, std::numeric_limits<int>::max(), ply) ||
      !integer_field(*parsed, "attempt", 1, 3, attempt) ||
      !integer_field(*parsed, "time_ms", 0, std::numeric_limits<int64_t>::max(), result.time_ms)) {
    return std::unexpected("invalid turn numeric field");
  }
  result.ply = static_cast<int>(ply);
  result.attempt = static_cast<int>(attempt);
  if (!parsed->contains("you")) return std::unexpected("turn you missing");
  auto seat = parse_seat(parsed->at("you"));
  if (!seat) return std::unexpected(seat.error());
  if (!parsed->contains("budgets")) return std::unexpected("budgets missing");
  auto context = parse_context(parsed->at("budgets"), *seat, false);
  if (!context) return std::unexpected(context.error());
  result.ctx = *context;
  auto position = parse_position(*parsed, result.ply);
  if (!position) return std::unexpected(position.error());
  result.pos = *position;
  if (!parsed->contains("legal") || !parsed->at("legal").is_array()) {
    return std::unexpected("legal list missing or invalid");
  }
  for (const auto& value : parsed->at("legal")) {
    auto move = parse_move(value);
    if (!move) return std::unexpected(move.error());
    result.legal.push_back(*move);
  }
  auto supplied = result.legal;
  auto less = [](Move a, Move b) {
    return a.board < b.board || (a.board == b.board && a.cell < b.cell);
  };
  std::sort(supplied.begin(), supplied.end(), less);
  MoveList own_list;
  result.pos.legal_moves(own_list);
  std::vector<Move> own(own_list.m.begin(), own_list.m.begin() + own_list.n);
  std::sort(own.begin(), own.end(), less);
  if (supplied != own) return std::unexpected("legal list differs from engine move generation");
  return result;
}

std::expected<void, std::string> validate_game_end(std::string_view line) {
  auto parsed = parse_json(line);
  if (!parsed) return std::unexpected(parsed.error());
  if (!constant_fields(*parsed, "game_end")) return std::unexpected("invalid game_end type/protocol");
  if (!parsed->contains("result") || !parsed->at("result").is_string()) {
    return std::unexpected("result missing or invalid");
  }
  const std::set<std::string> results = {"X", "O", "draw", "void"};
  if (!results.contains(parsed->at("result").get<std::string>())) {
    return std::unexpected("result outside enum");
  }
  if (!parsed->contains("reason") || !parsed->at("reason").is_string()) {
    return std::unexpected("reason missing or invalid");
  }
  const std::set<std::string> reasons = {"macro_win", "chip_count", "exact_tie_draw",
      "hello_fault", "recovery_fault", "triple_double_fault_void"};
  if (!reasons.contains(parsed->at("reason").get<std::string>())) {
    return std::unexpected("reason outside enum");
  }
  if (!parsed->contains("budgets")) return std::unexpected("budgets missing");
  auto context = parse_context(parsed->at("budgets"), Seat::X, true);
  if (!context) return std::unexpected(context.error());
  return {};
}

std::string serialize_reply(const TurnReply& reply) {
  json info = {{"quality", reply.info.quality}, {"depth", reply.info.depth},
               {"complete", reply.info.complete}};
  if (reply.info.lo) info["lo"] = *reply.info.lo;
  if (reply.info.hi) info["hi"] = *reply.info.hi;
  return json{{"type", "turn"}, {"protocol", 1}, {"request_id", reply.request_id},
              {"bid", reply.bid}, {"move", {reply.move.board, reply.move.cell}},
              {"info", std::move(info)}}.dump();
}

std::string serialize_hello() {
  return json{{"type", "hello"}, {"protocol", 1}, {"name", "poorman-uttt"},
              {"version", "0.1.0"}}.dump();
}

}  // namespace uttt::wire
