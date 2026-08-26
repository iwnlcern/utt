#pragma once

#include <array>
#include <cstdint>

#include "core/types.hpp"

namespace uttt {

inline constexpr std::array<uint32_t, 9> kPow3 = {1, 3, 9, 27, 81, 243, 729, 2187, 6561};

uint32_t ternary_code(uint16_t x, uint16_t o);

class LocalTable {
 public:
  static const LocalTable& instance();
  bool valid(uint32_t code) const;
  LocalStatus status(uint32_t code) const;
  uint16_t empties(uint32_t code) const;
  uint16_t win(uint32_t code, Seat s) const;
  uint16_t fork(uint32_t code, Seat s) const;

 private:
  LocalTable();
  std::array<uint64_t, 19683> e_{};
};

}  // namespace uttt
