#pragma once

#include <array>
#include <cstdint>

namespace uttt {

struct Position;

struct ZobristTables {
  std::array<std::array<uint64_t, 2>, 81> cell{};
  std::array<uint64_t, 10> forced{};
  std::array<uint64_t, 3> tie{};
  std::array<std::array<uint32_t, 2>, 81> tag_cell{};
  std::array<uint32_t, 10> tag_forced{};
  std::array<uint32_t, 3> tag_tie{};

  static const ZobristTables& instance();

 private:
  ZobristTables();
};

uint64_t zobrist_full(const Position& p);
uint32_t zobrist_tag_full(const Position& p);

}  // namespace uttt
