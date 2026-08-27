#include "search/tt.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace uttt {
namespace {

constexpr std::size_t kWays = 4;
constexpr uint8_t kStoredFlags =
    kTTQualityMask | kTTComplete | kTTHull | kTTOccupied;

bool occupied(const TTEntry &entry) { return (entry.flags & kTTOccupied) != 0; }

double interval_width(const TTEntry &entry) { return entry.hi - entry.lo; }

bool preferable_victim(const TTEntry &candidate, const TTEntry &current,
                       uint8_t incoming_generation) {
  const bool candidate_stale = candidate.gen != incoming_generation;
  const bool current_stale = current.gen != incoming_generation;
  if (candidate_stale != current_stale)
    return candidate_stale;
  if (candidate.depth != current.depth)
    return candidate.depth < current.depth;
  return interval_width(candidate) > interval_width(current);
}

uint8_t worse_quality(uint8_t lhs, uint8_t rhs) {
  return std::max(static_cast<uint8_t>(lhs & kTTQualityMask),
                  static_cast<uint8_t>(rhs & kTTQualityMask));
}

} // namespace

TT::TT(uint8_t entries_log2, Mode mode) : mode_(mode) {
  constexpr uint8_t kSizeBits =
      static_cast<uint8_t>(std::numeric_limits<std::size_t>::digits);
  if (entries_log2 < 2 || entries_log2 >= kSizeBits - 5)
    throw std::invalid_argument("TT entries_log2 is out of range");

  const std::size_t entries = std::size_t{1} << entries_log2;
  buckets_.resize(entries / kWays);
  if (mode_ == Mode::FullKey)
    sidecars_.resize(entries);
}

bool TT::identity_matches(std::size_t bucket, std::size_t way,
                          const PosId &id) const {
  return mode_ == Mode::Play || sidecars_[sidecar_index(bucket, way)] == id;
}

std::optional<TTEntry> TT::probe(TTKey key, const PosId &id) {
  const std::size_t bucket = bucket_index(key.key);
  for (std::size_t way = 0; way < kWays; ++way) {
    const TTEntry &entry = buckets_[bucket].way[way];
    if (!occupied(entry) || entry.tag != key.tag)
      continue;
    if (!identity_matches(bucket, way, id)) {
      ++stats.collisions;
      continue;
    }
    ++stats.hits;
    return entry;
  }
  ++stats.misses;
  return std::nullopt;
}

void TT::write(std::size_t bucket, std::size_t way, TTKey key, const PosId &id,
               TTEntry entry) {
  entry.tag = key.tag;
  entry.smp_word = 0;
  entry.flags =
      static_cast<uint8_t>((entry.flags & kStoredFlags) | kTTOccupied);
  entry.pad[0] = 0;
  entry.pad[1] = 0;
  entry.pad[2] = 0;
  if (entry.move_x > 80)
    entry.move_x = std::numeric_limits<uint8_t>::max();
  if (entry.move_o > 80)
    entry.move_o = std::numeric_limits<uint8_t>::max();
  buckets_[bucket].way[way] = entry;
  if (mode_ == Mode::FullKey)
    sidecars_[sidecar_index(bucket, way)] = id;
  ++stats.stores;
}

void TT::store(TTKey key, const PosId &id, TTEntry incoming) {
  const std::size_t bucket = bucket_index(key.key);
  std::optional<std::size_t> empty_way;
  std::optional<std::size_t> same_way;

  for (std::size_t way = 0; way < kWays; ++way) {
    const TTEntry &resident = buckets_[bucket].way[way];
    if (!occupied(resident)) {
      if (!empty_way)
        empty_way = way;
      continue;
    }
    if (resident.tag != key.tag)
      continue;
    if (!identity_matches(bucket, way, id)) {
      ++stats.collisions;
      continue;
    }
    same_way = way;
    break;
  }

  if (same_way) {
    const TTEntry resident = buckets_[bucket].way[*same_way];
    if (incoming.depth < resident.depth &&
        interval_width(incoming) > interval_width(resident))
      return;

    if (incoming.depth == resident.depth) {
      TTEntry merged = resident;
      const double merged_lo = std::max(resident.lo, incoming.lo);
      const double merged_hi = std::min(resident.hi, incoming.hi);
      if (merged_lo <= merged_hi) {
        merged.lo = merged_lo;
        merged.hi = merged_hi;
      }
      merged.gen = incoming.gen;
      merged.flags = static_cast<uint8_t>(
          worse_quality(resident.flags, incoming.flags) |
          ((resident.flags & incoming.flags) & kTTComplete) |
          ((resident.flags | incoming.flags) & kTTHull));
      if (merged.move_x > 80)
        merged.move_x = incoming.move_x;
      if (merged.move_o > 80)
        merged.move_o = incoming.move_o;
      write(bucket, *same_way, key, id, merged);
      return;
    }

    write(bucket, *same_way, key, id, incoming);
    return;
  }

  if (empty_way) {
    write(bucket, *empty_way, key, id, incoming);
    return;
  }

  std::size_t victim = 0;
  for (std::size_t way = 1; way < kWays; ++way) {
    if (preferable_victim(buckets_[bucket].way[way],
                          buckets_[bucket].way[victim], incoming.gen))
      victim = way;
  }
  write(bucket, victim, key, id, incoming);
}

} // namespace uttt
