#pragma once

#include <bit>
#include <array>
#include <cassert>
#include <cstdint>
#include <queue>

#include "core/types.hpp"

namespace uttt::naive {

struct LocalEval {
  bool valid;
  LocalStatus status;
  uint16_t empties;
  uint16_t win_x;
  uint16_t win_o;
  uint16_t fork_x;
  uint16_t fork_o;
};

namespace detail {

inline bool completed(uint16_t mask) {
  for (uint16_t line : kWinLines) {
    if ((mask & line) == line) return true;
  }
  return false;
}

inline uint32_t code_for(uint16_t x, uint16_t o) {
  constexpr std::array<uint32_t, 9> powers = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};
  uint32_t code = 0;
  for (int c = 0; c < 9; ++c) {
    if (((x >> c) & 1u) != 0) code += powers[c];
    if (((o >> c) & 1u) != 0) code += 2 * powers[c];
  }
  return code;
}

inline const std::array<bool, 19683>& reachable_codes() {
  static const auto reachable = [] {
    struct State {
      uint16_t x;
      uint16_t o;
    };
    std::array<bool, 19683> seen{};
    std::queue<State> frontier;
    seen[0] = true;
    frontier.push({0, 0});
    while (!frontier.empty()) {
      const State state = frontier.front();
      frontier.pop();
      const uint16_t occupied = uint16_t(state.x | state.o);
      if (completed(state.x) || completed(state.o) || occupied == 0x1FFu) continue;
      for (int c = 0; c < 9; ++c) {
        const uint16_t bit = uint16_t(1u << c);
        if ((occupied & bit) != 0) continue;
        for (Seat mover : {Seat::X, Seat::O}) {
          State next = state;
          (mover == Seat::X ? next.x : next.o) |= bit;
          const uint32_t code = code_for(next.x, next.o);
          if (seen[code]) continue;
          seen[code] = true;
          frontier.push(next);
        }
      }
    }
    return seen;
  }();
  return reachable;
}

inline uint16_t immediate_wins(uint16_t own, uint16_t empties) {
  uint16_t result = 0;
  for (int c = 0; c < 9; ++c) {
    const uint16_t bit = uint16_t(1u << c);
    if ((empties & bit) != 0 && completed(uint16_t(own | bit))) result |= bit;
  }
  return result;
}

inline uint16_t forks(uint16_t own, uint16_t occupied, uint16_t empties) {
  uint16_t result = 0;
  for (int c = 0; c < 9; ++c) {
    const uint16_t bit = uint16_t(1u << c);
    if ((empties & bit) == 0) continue;
    const uint16_t next = uint16_t(own | bit);
    if (completed(next)) continue;
    const uint16_t next_empties = uint16_t((~(occupied | bit)) & 0x1FFu);
    if (std::popcount(immediate_wins(next, next_empties)) >= 2) result |= bit;
  }
  return result;
}

}  // namespace detail

inline LocalEval eval_local(uint16_t x, uint16_t o) {
  assert((x & o) == 0);
  const bool x_won = detail::completed(x);
  const bool o_won = detail::completed(o);
  const bool valid = detail::reachable_codes()[detail::code_for(x, o)];
  if (!valid) return {false, LocalStatus::FullDraw, 0, 0, 0, 0, 0};
  if (x_won) return {true, LocalStatus::XWon, 0, 0, 0, 0, 0};
  if (o_won) return {true, LocalStatus::OWon, 0, 0, 0, 0, 0};

  const uint16_t occupied = uint16_t(x | o);
  const uint16_t empties = uint16_t((~occupied) & 0x1FFu);
  if (empties == 0) return {true, LocalStatus::FullDraw, 0, 0, 0, 0, 0};

  return {true,
          LocalStatus::Open,
          empties,
          detail::immediate_wins(x, empties),
          detail::immediate_wins(o, empties),
          detail::forks(x, occupied, empties),
          detail::forks(o, occupied, empties)};
}

}  // namespace uttt::naive
