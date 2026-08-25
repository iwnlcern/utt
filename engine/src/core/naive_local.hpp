#pragma once

#include <bit>
#include <cassert>
#include <cstdint>

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

inline bool reachable_winner(uint16_t mask) {
  for (int c = 0; c < 9; ++c) {
    const uint16_t bit = uint16_t(1u << c);
    if ((mask & bit) == 0) continue;
    bool on_every_line = true;
    bool saw_line = false;
    for (uint16_t line : kWinLines) {
      if ((mask & line) != line) continue;
      saw_line = true;
      if ((line & bit) == 0) on_every_line = false;
    }
    if (saw_line && on_every_line && !completed(uint16_t(mask & ~bit))) return true;
  }
  return false;
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
  const bool valid = !x_won && !o_won
      ? true
      : x_won && o_won ? false
                        : detail::reachable_winner(x_won ? x : o);
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
