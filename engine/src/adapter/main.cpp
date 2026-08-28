#include "adapter/policy.hpp"
#include "adapter/wire.hpp"
#include "core/clock.hpp"
#include "eval/eval.hpp"
#include "nlohmann/json.hpp"
#include "root/p2_gate.hpp"
#include "search/search.hpp"

#include <array>
#include <cstdint>
#include <expected>
#include <iostream>
#include <limits>
#include <string>

namespace {

enum class ReadResult { Line, Oversize, Eof };

ReadResult read_bounded_line(std::istream &input, std::string &line) {
  line.clear();
  bool saw_byte = false;
  bool oversize = false;
  char ch = 0;
  while (input.get(ch)) {
    saw_byte = true;
    if (ch == '\n')
      return oversize ? ReadResult::Oversize : ReadResult::Line;
    if (line.size() < uttt::wire::kMaxLineBytes) {
      line.push_back(ch);
    } else {
      oversize = true;
    }
  }
  if (!saw_byte)
    return ReadResult::Eof;
  return oversize ? ReadResult::Oversize : ReadResult::Line;
}

using json = nlohmann::json;

std::expected<uttt::Tie, std::string> parse_tie(const json &value) {
  if (!value.is_string())
    return std::unexpected("tie must be X or O");
  const std::string tie = value.get<std::string>();
  if (tie == "X")
    return uttt::Tie::X;
  if (tie == "O")
    return uttt::Tie::O;
  return std::unexpected("tie must be X or O");
}

std::expected<uttt::Position, std::string> parse_parts(const json &parts) {
  if (!parts.is_object() || !parts.contains("x") || !parts.contains("o") ||
      !parts.contains("forced") || !parts.contains("tie"))
    return std::unexpected("parts must contain x, o, forced, and tie");
  if (!parts.at("x").is_array() || !parts.at("o").is_array() ||
      parts.at("x").size() != 9 || parts.at("o").size() != 9)
    return std::unexpected("parts x/o must be nine-element arrays");
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  for (std::size_t i = 0; i < 9; ++i) {
    if (!parts.at("x").at(i).is_number_integer() ||
        !parts.at("o").at(i).is_number_integer())
      return std::unexpected("parts masks must be integers");
    const int64_t xi = parts.at("x").at(i).get<int64_t>();
    const int64_t oi = parts.at("o").at(i).get<int64_t>();
    if (xi < 0 || xi > 0x1ff || oi < 0 || oi > 0x1ff)
      return std::unexpected("parts masks must be in 0..511");
    x[i] = static_cast<uint16_t>(xi);
    o[i] = static_cast<uint16_t>(oi);
  }
  int8_t forced = uttt::kForcedAny;
  if (!parts.at("forced").is_null()) {
    if (!parts.at("forced").is_number_integer())
      return std::unexpected("parts forced must be null or an integer");
    const int64_t raw = parts.at("forced").get<int64_t>();
    if (raw < 0 || raw > 8)
      return std::unexpected("parts forced must be null or in 0..8");
    forced = static_cast<int8_t>(raw);
  }
  uttt::TieState tie = uttt::TieState::NullFirstMove;
  if (!parts.at("tie").is_null()) {
    const auto parsed = parse_tie(parts.at("tie"));
    if (!parsed)
      return std::unexpected(parsed.error());
    tie = *parsed == uttt::Tie::X ? uttt::TieState::X : uttt::TieState::O;
  }
  const auto position = uttt::Position::from_parts(x, o, forced, tie);
  if (!position)
    return std::unexpected("Position::from_parts rejected parts");
  return *position;
}

bool endpoint_equals_share(double endpoint, uint64_t numerator,
                           uint64_t total) {
  if (total == 0)
    return false;
  if (endpoint == 0.0)
    return numerator == 0;
  if (endpoint == 1.0)
    return numerator == total;
  const auto binary = uttt::p2_detail::decompose_unit_endpoint(endpoint);
  const unsigned shift = static_cast<unsigned>(-binary.exponent);
  if (uttt::p2_detail::shift_overflows_wide(numerator, shift))
    return false;
  return uttt::p2_detail::shifted_wide(numerator, shift) ==
         uttt::p2_detail::scaled_mantissa(binary, total);
}

const char *quality_name(uttt::Quality quality) {
  switch (quality) {
  case uttt::Quality::Exact:
    return "exact";
  case uttt::Quality::Bound:
    return "bound";
  case uttt::Quality::Estimate:
    return "estimate";
  }
  return "estimate";
}

int run_analyze() {
  std::string line;
  std::size_t line_number = 0;
  while (true) {
    const ReadResult read = read_bounded_line(std::cin, line);
    if (read == ReadResult::Eof)
      return 0;
    ++line_number;
    if (read == ReadResult::Oversize) {
      std::cout << json{{"error", "analyze line exceeds 32768 bytes"},
                        {"line", line_number}}
                       .dump()
                << '\n'
                << std::flush;
      continue;
    }
    try {
      const json request = json::parse(line);
      if (!request.is_object() || !request.contains("parts") ||
          !request.contains("h") || !request.contains("bx") ||
          !request.contains("bo") || !request.contains("depth"))
        throw std::invalid_argument("analyze request missing required field");
      const auto position = parse_parts(request.at("parts"));
      const auto tie = parse_tie(request.at("h"));
      if (!position || !tie)
        throw std::invalid_argument(position ? tie.error() : position.error());
      if (!request.at("bx").is_number_integer() ||
          !request.at("bo").is_number_integer() ||
          !request.at("depth").is_number_integer())
        throw std::invalid_argument("analyze budgets/depth must be integers");
      const int64_t bx = request.at("bx").get<int64_t>();
      const int64_t bo = request.at("bo").get<int64_t>();
      const int64_t depth = request.at("depth").get<int64_t>();
      if (depth > 12)
        throw std::invalid_argument("analyze depth exceeds 12");
      if (bx < 0 || bo < 0 || depth < 0 ||
          static_cast<uint64_t>(bx) + static_cast<uint64_t>(bo) >
              std::numeric_limits<uint32_t>::max())
        throw std::invalid_argument("analyze budgets/depth outside domain");

      uttt::Search<uttt::UtttModel> search;
      constexpr uint64_t kAnalyzeNodeCap = 5'000'000;
      uint64_t used_nodes = 0;
      uttt::SearchResult result{};
      bool have_completed = false;
      for (int iteration = 0; iteration <= depth; ++iteration) {
        const uint64_t remaining = kAnalyzeNodeCap - used_nodes;
        const uttt::SearchResult attempted = search.solve(
            *position, *tie, {iteration, remaining, false, true, 12, 256, {}});
        used_nodes += search.nodes_searched();
        if (!attempted.complete) {
          if (!have_completed)
            result = attempted;
          result.complete = false;
          break;
        }
        result = attempted;
        have_completed = true;
      }
      const uint64_t total =
          static_cast<uint64_t>(bx) + static_cast<uint64_t>(bo);
      const bool equality = endpoint_equals_share(result.t.lo, bx, total) &&
                            endpoint_equals_share(result.t.hi, bx, total);
      json output{
          {"t_lo", result.t.lo},
          {"t_hi", result.t.hi},
          {"quality", quality_name(result.quality)},
          {"depth", result.depth},
          {"complete", result.complete},
          {"equality_label", equality ? json("convention") : json(nullptr)},
          {"features", uttt::eval_features(*position)}};
      std::cout << output.dump() << '\n' << std::flush;
    } catch (const std::exception &error) {
      std::cout << json{{"error", error.what()}, {"line", line_number}}.dump()
                << '\n'
                << std::flush;
    }
  }
}

} // namespace

int main(int argc, char **argv) {
  if (argc == 2 && std::string_view(argv[1]) == "analyze")
    return run_analyze();
  if (argc != 1) {
    std::cerr << "usage: uttt_engine [analyze]\n";
    return 2;
  }
  uttt::PlaceholderPolicy policy;
  uttt::SteadyClock clock;
  std::string line;
  while (true) {
    const ReadResult read = read_bounded_line(std::cin, line);
    if (read == ReadResult::Eof)
      break;
    if (read == ReadResult::Oversize) {
      std::cerr << "oversize line rejected before JSON parsing\n";
      continue;
    }
    try {
      auto type = uttt::wire::classify(line);
      if (!type) {
        std::cerr << type.error() << '\n';
        continue;
      }
      if (*type == uttt::wire::MsgType::Hello) {
        auto request = uttt::wire::parse_hello(line);
        if (!request) {
          std::cerr << request.error() << '\n';
          continue;
        }
        std::cout << uttt::wire::serialize_hello() << '\n' << std::flush;
        continue;
      }
      if (*type == uttt::wire::MsgType::Turn) {
        auto request = uttt::wire::parse_turn(line);
        if (!request || request->legal.empty()) {
          std::cerr << (request ? "turn has no legal move" : request.error())
                    << '\n';
          continue;
        }
        std::cout << uttt::wire::serialize_reply(policy.choose(*request, clock))
                  << '\n'
                  << std::flush;
        continue;
      }
      auto valid = uttt::wire::validate_game_end(line);
      if (!valid) {
        std::cerr << valid.error() << '\n';
        continue;
      }
      return 0;
    } catch (const std::exception &error) {
      std::cerr << "message rejected: " << error.what() << '\n';
    } catch (...) {
      std::cerr << "message rejected: unknown exception\n";
    }
  }
  return 0;
}
