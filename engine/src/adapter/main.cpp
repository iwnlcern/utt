#include "adapter/policy.hpp"
#include "adapter/wire.hpp"
#include "core/clock.hpp"

#include <iostream>
#include <string>

namespace {

enum class ReadResult { Line, Oversize, Eof };

ReadResult read_bounded_line(std::istream& input, std::string& line) {
  line.clear();
  bool saw_byte = false;
  bool oversize = false;
  char ch = 0;
  while (input.get(ch)) {
    saw_byte = true;
    if (ch == '\n') return oversize ? ReadResult::Oversize : ReadResult::Line;
    if (line.size() < uttt::wire::kMaxLineBytes) {
      line.push_back(ch);
    } else {
      oversize = true;
    }
  }
  if (!saw_byte) return ReadResult::Eof;
  return oversize ? ReadResult::Oversize : ReadResult::Line;
}

}  // namespace

int main() {
  uttt::PlaceholderPolicy policy;
  uttt::SteadyClock clock;
  std::string line;
  while (true) {
    const ReadResult read = read_bounded_line(std::cin, line);
    if (read == ReadResult::Eof) break;
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
          std::cerr << (request ? "turn has no legal move" : request.error()) << '\n';
          continue;
        }
        std::cout << uttt::wire::serialize_reply(policy.choose(*request, clock))
                  << '\n' << std::flush;
        continue;
      }
      auto valid = uttt::wire::validate_game_end(line);
      if (!valid) {
        std::cerr << valid.error() << '\n';
        continue;
      }
      return 0;
    } catch (const std::exception& error) {
      std::cerr << "message rejected: " << error.what() << '\n';
    } catch (...) {
      std::cerr << "message rejected: unknown exception\n";
    }
  }
  return 0;
}
