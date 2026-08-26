#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/budget.hpp"
#include "core/position.hpp"

namespace uttt::wire {

inline constexpr std::size_t kMaxLineBytes = 32768;

struct TurnRequest {
  std::string game_id;
  std::string request_id;
  int ply;
  int attempt;
  RootContext ctx;
  Position pos;
  int64_t time_ms;
  std::vector<Move> legal;
};

struct Info {
  std::string quality;
  std::optional<double> lo;
  std::optional<double> hi;
  int depth;
  bool complete;
};

struct TurnReply {
  std::string request_id;
  int64_t bid;
  Move move;
  Info info;
};

struct HelloRequest {
  int protocol;
  std::string game_id;
  std::string rules;
  Seat you;
  int64_t time_ms;
  int64_t grace_ms;
  int64_t budget;
};

enum class MsgType { Hello, Turn, GameEnd };

std::expected<MsgType, std::string> classify(std::string_view line);
std::expected<HelloRequest, std::string> parse_hello(std::string_view line);
std::expected<TurnRequest, std::string> parse_turn(std::string_view line);
std::expected<void, std::string> validate_game_end(std::string_view line);
std::string serialize_reply(const TurnReply& reply);
std::string serialize_hello();

}  // namespace uttt::wire
