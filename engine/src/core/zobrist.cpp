#include "core/zobrist.hpp"

#include "core/position.hpp"

namespace uttt {
namespace {

uint64_t splitmix64(uint64_t& state) {
  uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
  z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
  z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
  return z ^ (z >> 31);
}

int forced_index(int8_t forced) { return forced == kForcedAny ? 9 : forced; }

}  // namespace

ZobristTables::ZobristTables() {
  uint64_t primary = 0x9E3779B97F4A7C15ULL;
  uint64_t secondary = 0xA5A5A5A5DEADBEEFULL;
  for (auto& square : cell) {
    for (auto& value : square) value = splitmix64(primary);
  }
  for (auto& value : forced) value = splitmix64(primary);
  for (auto& value : tie) value = splitmix64(primary);
  for (auto& square : tag_cell) {
    for (auto& value : square) value = static_cast<uint32_t>(splitmix64(secondary));
  }
  for (auto& value : tag_forced) value = static_cast<uint32_t>(splitmix64(secondary));
  for (auto& value : tag_tie) value = static_cast<uint32_t>(splitmix64(secondary));
}

const ZobristTables& ZobristTables::instance() {
  static const ZobristTables tables;
  return tables;
}

uint64_t zobrist_full(const Position& p) {
  const auto& z = ZobristTables::instance();
  uint64_t key = z.forced[forced_index(p.forced)] ^ z.tie[static_cast<uint8_t>(p.tie)];
  for (int b = 0; b < 9; ++b) {
    for (int c = 0; c < 9; ++c) {
      const uint16_t bit = uint16_t(1u << c);
      if ((p.x[b] & bit) != 0) key ^= z.cell[9 * b + c][0];
      if ((p.o[b] & bit) != 0) key ^= z.cell[9 * b + c][1];
    }
  }
  return key;
}

uint32_t zobrist_tag_full(const Position& p) {
  const auto& z = ZobristTables::instance();
  uint32_t tag = z.tag_forced[forced_index(p.forced)] ^
      z.tag_tie[static_cast<uint8_t>(p.tie)];
  for (int b = 0; b < 9; ++b) {
    for (int c = 0; c < 9; ++c) {
      const uint16_t bit = uint16_t(1u << c);
      if ((p.x[b] & bit) != 0) tag ^= z.tag_cell[9 * b + c][0];
      if ((p.o[b] & bit) != 0) tag ^= z.tag_cell[9 * b + c][1];
    }
  }
  return tag;
}

}  // namespace uttt
