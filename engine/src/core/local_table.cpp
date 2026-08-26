#include "core/local_table.hpp"

#include <bit>
#include <cassert>

namespace uttt {
namespace {

bool completed(uint16_t mask) {
  for (uint16_t line : kWinLines) {
    if ((mask & line) == line) return true;
  }
  return false;
}

bool reachable_winner(uint16_t mask) {
  for (int c = 0; c < 9; ++c) {
    const uint16_t bit = uint16_t(1u << c);
    if ((mask & bit) == 0) continue;
    bool saw_line = false;
    bool on_every_line = true;
    for (uint16_t line : kWinLines) {
      if ((mask & line) != line) continue;
      saw_line = true;
      if ((line & bit) == 0) on_every_line = false;
    }
    if (saw_line && on_every_line && !completed(uint16_t(mask & ~bit))) return true;
  }
  return false;
}

uint16_t wins(uint16_t own, uint16_t empties) {
  uint16_t result = 0;
  for (int c = 0; c < 9; ++c) {
    const uint16_t bit = uint16_t(1u << c);
    if ((empties & bit) != 0 && completed(uint16_t(own | bit))) result |= bit;
  }
  return result;
}

uint16_t forks(uint16_t own, uint16_t occupied, uint16_t empties) {
  uint16_t result = 0;
  for (int c = 0; c < 9; ++c) {
    const uint16_t bit = uint16_t(1u << c);
    if ((empties & bit) == 0) continue;
    const uint16_t next = uint16_t(own | bit);
    if (completed(next)) continue;
    const uint16_t next_empty = uint16_t((~(occupied | bit)) & 0x1FFu);
    if (std::popcount(wins(next, next_empty)) >= 2) result |= bit;
  }
  return result;
}

uint64_t pack(bool valid, LocalStatus status, uint16_t empty, uint16_t win_x,
              uint16_t win_o, uint16_t fork_x, uint16_t fork_o) {
  return uint64_t(valid)
      | (uint64_t(status) << 1)
      | (uint64_t(empty) << 3)
      | (uint64_t(win_x) << 12)
      | (uint64_t(win_o) << 21)
      | (uint64_t(fork_x) << 30)
      | (uint64_t(fork_o) << 39);
}

uint64_t derive(uint16_t x, uint16_t o) {
  const bool x_won = completed(x);
  const bool o_won = completed(o);
  const bool valid = !x_won && !o_won
      ? true
      : x_won && o_won ? false : reachable_winner(x_won ? x : o);
  if (!valid) return pack(false, LocalStatus::FullDraw, 0, 0, 0, 0, 0);
  if (x_won) return pack(true, LocalStatus::XWon, 0, 0, 0, 0, 0);
  if (o_won) return pack(true, LocalStatus::OWon, 0, 0, 0, 0, 0);
  const uint16_t occupied = uint16_t(x | o);
  const uint16_t empty = uint16_t((~occupied) & 0x1FFu);
  if (empty == 0) return pack(true, LocalStatus::FullDraw, 0, 0, 0, 0, 0);
  return pack(true, LocalStatus::Open, empty, wins(x, empty), wins(o, empty),
              forks(x, occupied, empty), forks(o, occupied, empty));
}

}  // namespace

uint32_t ternary_code(uint16_t x, uint16_t o) {
  assert((x & o) == 0);
  uint32_t code = 0;
  for (int c = 0; c < 9; ++c) {
    if ((x >> c) & 1u) code += kPow3[c];
    if ((o >> c) & 1u) code += 2 * kPow3[c];
  }
  return code;
}

LocalTable::LocalTable() {
  for (uint32_t code = 0; code < e_.size(); ++code) {
    uint16_t x = 0;
    uint16_t o = 0;
    uint32_t tmp = code;
    for (int c = 0; c < 9; ++c) {
      const uint32_t value = tmp % 3;
      tmp /= 3;
      if (value == 1) x |= uint16_t(1u << c);
      if (value == 2) o |= uint16_t(1u << c);
    }
    e_[code] = derive(x, o);
  }
}

const LocalTable& LocalTable::instance() {
  static const LocalTable table;
  return table;
}

bool LocalTable::valid(uint32_t code) const { return (e_[code] & 1u) != 0; }

LocalStatus LocalTable::status(uint32_t code) const {
  return static_cast<LocalStatus>((e_[code] >> 1) & 0x3u);
}

uint16_t LocalTable::empties(uint32_t code) const {
  return uint16_t((e_[code] >> 3) & 0x1FFu);
}

uint16_t LocalTable::win(uint32_t code, Seat s) const {
  return uint16_t((e_[code] >> (s == Seat::X ? 12 : 21)) & 0x1FFu);
}

uint16_t LocalTable::fork(uint32_t code, Seat s) const {
  return uint16_t((e_[code] >> (s == Seat::X ? 30 : 39)) & 0x1FFu);
}

}  // namespace uttt
