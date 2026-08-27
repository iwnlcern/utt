#pragma once

#include "search/game_model.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <stdexcept>
#include <vector>

namespace uttt {

inline constexpr uint8_t kTTOccupied = 1u << 0;
inline constexpr uint8_t kTTQualityMask = 3u << 1;
inline constexpr uint8_t kTTQualityExact = 0u << 1;
inline constexpr uint8_t kTTQualityBound = 1u << 1;
inline constexpr uint8_t kTTQualityEstimate = 2u << 1;
inline constexpr uint8_t kTTComplete = 1u << 3;
inline constexpr uint8_t kTTHull = 1u << 4;

struct alignas(16) TTEntry {
  uint32_t tag;
  uint32_t smp_word;
  double lo;
  double hi;
  uint8_t move_x;
  uint8_t move_o;
  uint8_t depth;
  uint8_t gen;
  uint8_t flags;
  uint8_t pad[3];
};

static_assert(sizeof(TTEntry) == 32);
static_assert(alignof(TTEntry) == 16);
static_assert(offsetof(TTEntry, tag) == 0);
static_assert(offsetof(TTEntry, smp_word) == 4);
static_assert(offsetof(TTEntry, lo) == 8);
static_assert(offsetof(TTEntry, hi) == 16);
static_assert(offsetof(TTEntry, move_x) == 24);
static_assert(offsetof(TTEntry, move_o) == 25);
static_assert(offsetof(TTEntry, depth) == 26);
static_assert(offsetof(TTEntry, gen) == 27);
static_assert(offsetof(TTEntry, flags) == 28);
static_assert(offsetof(TTEntry, pad) == 29);

struct alignas(64) TTBucket {
  TTEntry way[4];
};

static_assert(sizeof(TTBucket) == 128);
static_assert(alignof(TTBucket) == 64);
static_assert(offsetof(TTBucket, way) == 0);

struct CollisionStats {
  uint64_t collisions = 0;
  uint64_t hits = 0;
  uint64_t misses = 0;
  uint64_t stores = 0;
};

class TT {
public:
  enum class Mode : uint8_t { Play, FullKey };

  static constexpr uint8_t kDefaultEntriesLog2 = 22;
  static constexpr uint8_t kMinEntriesLog2 = 2;
  static constexpr uint8_t kMaxEntriesLog2 =
      static_cast<uint8_t>(std::numeric_limits<std::size_t>::digits - 6);

  explicit TT(uint8_t entries_log2 = kDefaultEntriesLog2,
              Mode mode = Mode::Play);

  TT(const TT &) = delete;
  TT &operator=(const TT &) = delete;
  TT(TT &&) noexcept = default;
  TT &operator=(TT &&) noexcept = default;

  std::optional<TTEntry> probe(TTKey key, const PosId &id);
  void store(TTKey key, const PosId &id, TTEntry entry);

  std::size_t entry_count() const { return buckets_.size() * 4; }
  std::size_t bucket_count() const { return buckets_.size(); }
  std::size_t sidecar_entry_count() const { return sidecars_.size(); }
  Mode mode() const { return mode_; }

  static constexpr std::size_t bytes_for_entries_log2(uint8_t entries_log2) {
    if (entries_log2 < kMinEntriesLog2 || entries_log2 > kMaxEntriesLog2)
      throw std::invalid_argument("TT entries_log2 is out of range");
    return (std::size_t{1} << entries_log2) * sizeof(TTEntry);
  }

  CollisionStats stats{};

private:
  std::size_t bucket_index(uint64_t key) const {
    return static_cast<std::size_t>(key) & (buckets_.size() - 1);
  }

  std::size_t sidecar_index(std::size_t bucket, std::size_t way) const {
    return bucket * 4 + way;
  }

  bool identity_matches(std::size_t bucket, std::size_t way,
                        const PosId &id) const;
  void write(std::size_t bucket, std::size_t way, TTKey key, const PosId &id,
             TTEntry entry);

  Mode mode_;
  std::vector<TTBucket> buckets_;
  std::vector<PosId> sidecars_;
};

} // namespace uttt
