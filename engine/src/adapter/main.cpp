#include "adapter/policy.hpp"
#include "adapter/wire.hpp"
#include "core/clock.hpp"

#include <iostream>
#include <string>

int main() {
  uttt::PlaceholderPolicy policy;
  uttt::SteadyClock clock;
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.size() > uttt::wire::kMaxLineBytes) {
      std::cerr << "oversize line rejected before JSON parsing\n";
      continue;
    }
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
  }
  return 0;
}
