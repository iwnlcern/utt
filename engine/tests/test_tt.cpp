#include "doctest/doctest.h"
#include "search/search.hpp"
#include "search/tt.hpp"
#include "support/ttt3_model.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace uttt;

namespace {

constexpr uint8_t kNoMove = std::numeric_limits<uint8_t>::max();

TTEntry entry(double lo, double hi, uint8_t depth, uint8_t gen,
              uint8_t move_x = kNoMove, uint8_t move_o = kNoMove,
              uint8_t flags = kTTQualityExact | kTTComplete) {
  TTEntry result{};
  result.lo = lo;
  result.hi = hi;
  result.move_x = move_x;
  result.move_o = move_o;
  result.depth = depth;
  result.gen = gen;
  result.flags = flags;
  return result;
}

struct CollidingTtt3Model : Ttt3Model {
  static TTKey tt_key(const State &) {
    return {0x0123456789ABCDEFULL, 0xC0111DEu};
  }
};

struct HullCacheState {
  uint8_t id = 0;
  Tie tie = Tie::X;
  bool operator==(const HullCacheState &) const = default;
};

struct HullCacheModel {
  using State = HullCacheState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    if (state.id == 0)
      return {{{1, Tie::O}, 2}};
    return {{{3, Tie::O}, 3}};
  }

  static std::vector<ModelChild<State>> children_o(const State &state) {
    if (state.id == 0)
      return {{{2, Tie::X}, 7}};
    return {{{4, Tie::X}, 8}};
  }

  static TerminalKind terminal(const State &state) {
    if (state.id == 3)
      return TerminalKind::MacroWinX;
    if (state.id == 4)
      return TerminalKind::AllClosed;
    return TerminalKind::None;
  }

  static int empties(const State &) { return 2; }

  static TTKey tt_key(const State &state) {
    const uint64_t identity =
        static_cast<uint64_t>(state.id) * 3 + static_cast<uint8_t>(state.tie);
    return {identity, static_cast<uint32_t>(0xACED0000u + identity)};
  }

  static PosId pos_id(const State &state) {
    PosId result;
    result.x[0] = state.id;
    result.tie = state.tie;
    return result;
  }

  static int chip_sign(const State &, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }
};

static_assert(GameModel<HullCacheModel>);

std::string board_from_dense(uint32_t code) {
  std::string board(9, '.');
  for (char &cell : board) {
    const uint32_t digit = code % 3;
    code /= 3;
    if (digit == 1)
      cell = 'X';
    if (digit == 2)
      cell = 'O';
  }
  return board;
}

std::vector<Ttt3State> three_marks_remaining_family() {
  std::vector<Ttt3State> result;
  for (uint32_t code = 0; code < 19683; ++code) {
    const std::string board = board_from_dense(code);
    if (std::count(board.begin(), board.end(), '.') == 3)
      result.push_back(Ttt3State::from_board(board, Tie::X));
  }
  return result;
}

Position seeded_position(uint64_t seed, int plies) {
  Position position = Position::initial();
  for (int ply = 0; ply < plies; ++ply) {
    if (position.terminal() != TerminalKind::None)
      break;
    MoveList moves;
    position.legal_moves(moves);
    REQUIRE(moves.n > 0);
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    const Move move = moves.m[seed % static_cast<uint64_t>(moves.n)];
    const Seat mover = ((seed >> 8) & 1u) == 0 ? Seat::X : Seat::O;
    position = position.applied(move, mover).value();
  }
  return position;
}

void check_same_search_result(const SearchResult &with_tt,
                              const SearchResult &without_tt) {
  CHECK(with_tt.t.lo == without_tt.t.lo);
  CHECK(with_tt.t.hi == without_tt.t.hi);
  CHECK(with_tt.best_x == without_tt.best_x);
  CHECK(with_tt.best_o == without_tt.best_o);
  CHECK(with_tt.quality == without_tt.quality);
  CHECK(with_tt.depth == without_tt.depth);
  CHECK(with_tt.complete == without_tt.complete);
  CHECK(with_tt.hull == without_tt.hull);
}

} // namespace

TEST_CASE(
    "A6 TT layout and entry-count sizing retain the locked byte contract") {
  CHECK(sizeof(TTEntry) == 32);
  CHECK(alignof(TTEntry) == 16);
  CHECK(offsetof(TTEntry, tag) == 0);
  CHECK(offsetof(TTEntry, smp_word) == 4);
  CHECK(offsetof(TTEntry, lo) == 8);
  CHECK(offsetof(TTEntry, hi) == 16);
  CHECK(offsetof(TTEntry, move_x) == 24);
  CHECK(offsetof(TTEntry, move_o) == 25);
  CHECK(offsetof(TTEntry, depth) == 26);
  CHECK(offsetof(TTEntry, gen) == 27);
  CHECK(offsetof(TTEntry, flags) == 28);
  CHECK(offsetof(TTEntry, pad) == 29);
  CHECK(sizeof(TTBucket) == 128);
  CHECK(alignof(TTBucket) == 64);
  CHECK(TT::bytes_for_entries_log2(TT::kDefaultEntriesLog2) ==
        128ULL * 1024ULL * 1024ULL);

  TT table{2, TT::Mode::Play};
  CHECK(table.entry_count() == 4);
  CHECK(table.bucket_count() == 1);
  CHECK(table.sidecar_entry_count() == 0);
}

TEST_CASE("TT byte sizing rejects every shift and multiplication overflow") {
  constexpr uint8_t size_bits =
      static_cast<uint8_t>(std::numeric_limits<std::size_t>::digits);
  constexpr uint8_t max_entries_log2 = static_cast<uint8_t>(size_bits - 6);

  CHECK(TT::bytes_for_entries_log2(2) == 128);
  CHECK(TT::bytes_for_entries_log2(TT::kDefaultEntriesLog2) ==
        128ULL * 1024ULL * 1024ULL);
  CHECK(TT::bytes_for_entries_log2(max_entries_log2) ==
        (std::size_t{1} << (size_bits - 1)));
  CHECK_THROWS_AS(TT::bytes_for_entries_log2(0), std::invalid_argument);
  CHECK_THROWS_AS(TT::bytes_for_entries_log2(1), std::invalid_argument);
  CHECK_THROWS_AS(
      TT::bytes_for_entries_log2(static_cast<uint8_t>(max_entries_log2 + 1)),
      std::invalid_argument);
  CHECK_THROWS_AS(TT::bytes_for_entries_log2(64), std::invalid_argument);
  CHECK_THROWS_AS(TT::bytes_for_entries_log2(255), std::invalid_argument);
}

TEST_CASE("A6 empty entries and Play tags cannot produce false hits") {
  TT table{2, TT::Mode::Play};
  const PosId ignored{};

  CHECK_FALSE(table.probe({0x10u, 0x20u}, ignored).has_value());
  table.store({0x10u, 0x20u}, ignored, entry(0.25, 0.5, 3, 1));

  const auto hit = table.probe({0x10u, 0x20u}, ignored);
  REQUIRE(hit.has_value());
  CHECK(hit->lo == 0.25);
  CHECK(hit->hi == 0.5);
  CHECK(hit->move_x == kNoMove);
  CHECK(hit->move_o == kNoMove);
  CHECK_FALSE(table.probe({0x10u, 0x21u}, ignored).has_value());
}

TEST_CASE("A6 FullKey collision stores and probes distinct per-way snapshots") {
  TT table{2, TT::Mode::FullKey};
  CHECK(table.sidecar_entry_count() == 4);

  Ttt3State first_state = Ttt3State::from_board("X.O......", Tie::X);
  Ttt3State second_state = first_state;
  second_state.tie = Tie::O;
  const TTKey first_key = CollidingTtt3Model::tt_key(first_state);
  const TTKey second_key = CollidingTtt3Model::tt_key(second_state);
  REQUIRE(first_key == second_key);
  const PosId first_id = CollidingTtt3Model::pos_id(first_state);
  const PosId second_id = CollidingTtt3Model::pos_id(second_state);
  REQUIRE(first_id.tie != second_id.tie);

  table.store(first_key, first_id, entry(0.125, 0.25, 4, 1, 2, 7));
  table.store(second_key, second_id, entry(0.75, 0.875, 4, 1, 5, 8));
  CHECK(table.stats.collisions == 1);

  const auto first = table.probe(first_key, first_id);
  REQUIRE(first.has_value());
  CHECK(first->lo == 0.125);
  CHECK(first->move_x == 2);

  table.stats = {};
  const auto second = table.probe(second_key, second_id);
  REQUIRE(second.has_value());
  CHECK(second->lo == 0.75);
  CHECK(second->move_x == 5);
  CHECK(table.stats.collisions == 1);

  PosId absent = second_id;
  absent.x[0] ^= 1u;
  CHECK_FALSE(table.probe(second_key, absent).has_value());
  CHECK(table.stats.collisions == 3);
}

TEST_CASE("TT replacement prioritizes stale generation before depth") {
  TT table{2, TT::Mode::Play};
  const PosId ignored{};
  table.store({0, 10}, ignored, entry(0.1, 0.2, 1, 2));
  table.store({0, 11}, ignored, entry(0.1, 0.2, 8, 1));
  table.store({0, 12}, ignored, entry(0.1, 0.2, 9, 1));
  table.store({0, 13}, ignored, entry(0.1, 0.2, 10, 1));

  table.store({0, 14}, ignored, entry(0.3, 0.4, 2, 2));

  CHECK(table.probe({0, 10}, ignored).has_value());
  CHECK_FALSE(table.probe({0, 11}, ignored).has_value());
  CHECK(table.probe({0, 12}, ignored).has_value());
  CHECK(table.probe({0, 13}, ignored).has_value());
  CHECK(table.probe({0, 14}, ignored).has_value());
}

TEST_CASE("TT replacement treats pre-wrap generation 255 as stale at zero") {
  TT table{2, TT::Mode::Play};
  const PosId ignored{};
  table.store({0, 40}, ignored, entry(0.0, 1.0, 1, 0));
  table.store({0, 41}, ignored, entry(0.0, 0.1, 8, 255));
  table.store({0, 42}, ignored, entry(0.0, 0.1, 9, 255));
  table.store({0, 43}, ignored, entry(0.0, 0.1, 10, 255));

  table.store({0, 44}, ignored, entry(0.4, 0.6, 2, 0));

  CHECK(table.probe({0, 40}, ignored).has_value());
  CHECK_FALSE(table.probe({0, 41}, ignored).has_value());
  CHECK(table.probe({0, 42}, ignored).has_value());
  CHECK(table.probe({0, 43}, ignored).has_value());
  CHECK(table.probe({0, 44}, ignored).has_value());
}

TEST_CASE("TT replacement chooses shallower then wider entries") {
  const PosId ignored{};
  SUBCASE("shallower precedes interval width") {
    TT table{2, TT::Mode::Play};
    table.store({0, 20}, ignored, entry(0.0, 0.1, 2, 1));
    table.store({0, 21}, ignored, entry(0.0, 1.0, 3, 1));
    table.store({0, 22}, ignored, entry(0.0, 0.2, 4, 1));
    table.store({0, 23}, ignored, entry(0.0, 0.2, 5, 1));
    table.store({0, 24}, ignored, entry(0.4, 0.6, 6, 1));
    CHECK_FALSE(table.probe({0, 20}, ignored).has_value());
    CHECK(table.probe({0, 21}, ignored).has_value());
  }
  SUBCASE("wider breaks an equal-depth tie") {
    TT table{2, TT::Mode::Play};
    table.store({0, 30}, ignored, entry(0.0, 0.1, 4, 1));
    table.store({0, 31}, ignored, entry(0.0, 0.8, 4, 1));
    table.store({0, 32}, ignored, entry(0.0, 0.3, 4, 1));
    table.store({0, 33}, ignored, entry(0.0, 0.2, 4, 1));
    table.store({0, 34}, ignored, entry(0.4, 0.6, 6, 1));
    CHECK(table.probe({0, 30}, ignored).has_value());
    CHECK_FALSE(table.probe({0, 31}, ignored).has_value());
  }
}

TEST_CASE(
    "TT same-entry protection and equal-depth merge preserve useful data") {
  TT table{2, TT::Mode::Play};
  const PosId ignored{};
  const TTKey key{0x55u, 0x66u};
  table.store(key, ignored, entry(0.2, 0.7, 6, 1, 3, 4));
  table.store(key, ignored, entry(0.1, 0.9, 5, 2, 7, 8));

  auto hit = table.probe(key, ignored);
  REQUIRE(hit.has_value());
  CHECK(hit->lo == 0.2);
  CHECK(hit->hi == 0.7);
  CHECK(hit->depth == 6);
  CHECK(hit->move_x == 3);
  CHECK(hit->move_o == 4);

  table.store(key, ignored, entry(0.3, 0.8, 6, 2, 7, 8));
  hit = table.probe(key, ignored);
  REQUIRE(hit.has_value());
  CHECK(hit->lo == 0.3);
  CHECK(hit->hi == 0.7);
  CHECK(hit->depth == 6);
  CHECK(hit->move_x == 3);
  CHECK(hit->move_o == 4);
}

TEST_CASE("TT equal-depth merges never let estimates narrow sound entries") {
  struct Case {
    const char *name;
    TTEntry resident;
    TTEntry incoming;
    double expected_lo;
    double expected_hi;
    uint8_t expected_quality;
    uint8_t expected_move_x;
  };
  const std::array cases{
      Case{"exact then estimate",
           entry(0.2, 0.7, 6, 1, 2, 3, kTTQualityExact | kTTComplete),
           entry(0.3, 0.5, 6, 2, 7, 8, kTTQualityEstimate), 0.2, 0.7,
           kTTQualityExact, 2},
      Case{"estimate then exact",
           entry(0.3, 0.5, 6, 1, 2, 3, kTTQualityEstimate),
           entry(0.2, 0.7, 6, 2, 7, 8, kTTQualityExact | kTTComplete), 0.2, 0.7,
           kTTQualityExact, 7},
      Case{"bound then estimate",
           entry(0.1, 0.8, 6, 1, 2, 3, kTTQualityBound | kTTComplete),
           entry(0.3, 0.5, 6, 2, 7, 8, kTTQualityEstimate), 0.1, 0.8,
           kTTQualityBound, 2},
      Case{"estimate then bound",
           entry(0.3, 0.5, 6, 1, 2, 3, kTTQualityEstimate),
           entry(0.1, 0.8, 6, 2, 7, 8, kTTQualityBound | kTTComplete), 0.1, 0.8,
           kTTQualityBound, 7},
      Case{"wide estimate then narrow estimate",
           entry(0.1, 0.9, 6, 1, 2, 3, kTTQualityEstimate),
           entry(0.3, 0.5, 6, 2, 7, 8, kTTQualityEstimate), 0.3, 0.5,
           kTTQualityEstimate, 7},
      Case{"narrow estimate then wide estimate",
           entry(0.3, 0.5, 6, 1, 2, 3, kTTQualityEstimate),
           entry(0.1, 0.9, 6, 2, 7, 8, kTTQualityEstimate), 0.1, 0.9,
           kTTQualityEstimate, 7},
  };

  for (const Case &test : cases) {
    TT table{2, TT::Mode::Play};
    const PosId ignored{};
    const TTKey key{0x77u, 0x88u};
    table.store(key, ignored, test.resident);
    table.store(key, ignored, test.incoming);
    const auto hit = table.probe(key, ignored);

    CAPTURE(test.name);
    REQUIRE(hit.has_value());
    CHECK(hit->lo == test.expected_lo);
    CHECK(hit->hi == test.expected_hi);
    CHECK((hit->flags & kTTQualityMask) == test.expected_quality);
    CHECK(hit->move_x == test.expected_move_x);
  }
}

TEST_CASE(
    "TT production reuse preserves the current-node hull under one node") {
  const HullCacheState root{0, Tie::X};
  Search<HullCacheModel> warm{4, TT::Mode::FullKey};
  const SearchResult primed = warm.solve(root, Tie::X, {2, 100, true});
  REQUIRE(primed.complete);
  REQUIRE(primed.quality == Quality::Exact);
  REQUIRE(primed.hull);

  const uint64_t hits_before = warm.tt_stats()->hits;
  const SearchResult reused = warm.solve(root, Tie::X, {2, 1, true});
  CHECK(reused.complete);
  CHECK(reused.hull);
  CHECK(reused.t.lo == primed.t.lo);
  CHECK(reused.t.hi == primed.t.hi);
  CHECK(reused.best_x == primed.best_x);
  CHECK(reused.best_o == primed.best_o);
  CHECK(warm.tt_stats()->hits == hits_before + 1);

  Search<HullCacheModel> fresh{4, TT::Mode::FullKey};
  CHECK_FALSE(fresh.solve(root, Tie::X, {2, 1, true}).complete);
  CHECK_FALSE(warm.solve(root, Tie::X, {2, 1, false}).complete);

  Search<HullCacheModel> estimate_control{4, TT::Mode::FullKey};
  const SearchResult estimate =
      estimate_control.solve(root, Tie::X, {1, 100, true});
  REQUIRE(estimate.complete);
  REQUIRE(estimate.quality == Quality::Estimate);
  CHECK_FALSE(estimate_control.solve(root, Tie::X, {1, 1, true}).complete);

  Search<HullCacheModel> shallow_control{4, TT::Mode::FullKey};
  REQUIRE(shallow_control.solve(root, Tie::X, {2, 100, true}).complete);
  CHECK_FALSE(shallow_control.solve(root, Tie::X, {3, 1, true}).complete);
}

TEST_CASE(
    "TT history remains sound for bounded precision and null-root search") {
  const Ttt3State bounded_state = Ttt3State::from_board("XXO.XXX..", Tie::X);
  Search<Ttt3Model> history{10, TT::Mode::FullKey};
  const SearchResult primed =
      history.solve(bounded_state, Tie::X, {3, 10000, true});
  REQUIRE(primed.complete);
  REQUIRE(primed.quality == Quality::Exact);

  const Window bounded_window{{0.4, 0.6}, 0.0};
  Search<Ttt3Model> bounded_fresh;
  const SearchResult bounded_expected =
      bounded_fresh.solve(bounded_state, Tie::X, {3, 10000}, bounded_window);
  const SearchResult bounded_cached =
      history.solve(bounded_state, Tie::X, {3, 1, true}, bounded_window);
  CHECK(bounded_cached.complete);
  CHECK(bounded_cached.quality == Quality::Exact);
  CHECK(bounded_expected.t.lo <= bounded_cached.t.lo);
  CHECK(bounded_cached.t.hi <= bounded_expected.t.hi);
  CHECK(bounded_cached.best_x == bounded_expected.best_x);
  CHECK(bounded_cached.best_o == bounded_expected.best_o);

  const Window precision_window{{0.0, 1.0}, 0.125};
  Search<Ttt3Model> precision_fresh;
  const SearchResult precision_expected = precision_fresh.solve(
      bounded_state, Tie::X, {3, 10000}, precision_window);
  const SearchResult precision_cached =
      history.solve(bounded_state, Tie::X, {3, 1, true}, precision_window);
  CHECK(precision_cached.complete);
  CHECK(width(precision_cached.t) <= precision_window.eps_node);
  CHECK(precision_expected.t.lo <= precision_cached.t.lo);
  CHECK(precision_cached.t.hi <= precision_expected.t.hi);
  CHECK(precision_cached.best_x == precision_expected.best_x);
  CHECK(precision_cached.best_o == precision_expected.best_o);

  const Ttt3State null_root =
      Ttt3State::from_board(".........", Tie::NullFirstMove);
  Search<Ttt3Model> null_history{10, TT::Mode::FullKey};
  REQUIRE(null_history.solve(null_root, Tie::NullFirstMove, {3, 100000, true})
              .complete);
  const SearchResult null_cached =
      null_history.solve(null_root, Tie::NullFirstMove, {3, 100000, true});
  Search<Ttt3Model> null_fresh;
  const SearchResult null_expected =
      null_fresh.solve(null_root, Tie::NullFirstMove, {3, 100000});
  check_same_search_result(null_cached, null_expected);
  CHECK_FALSE(null_cached.hull);
}

TEST_CASE("A6 TT-on equals TT-off across the three-empty ttt3 family") {
  const auto family = three_marks_remaining_family();
  REQUIRE(family.size() == 5376);
  Search<Ttt3Model> without_tt;
  Search<Ttt3Model> with_tt{12, TT::Mode::FullKey};

  for (Ttt3State state : family) {
    for (Tie tie : {Tie::X, Tie::O}) {
      state.tie = tie;
      const SearchResult expected = without_tt.solve(state, tie, {3, 10000});
      const SearchResult actual = with_tt.solve(state, tie, {3, 10000, true});
      CAPTURE(state.dense_code());
      CAPTURE(tie);
      check_same_search_result(actual, expected);
    }
  }
  REQUIRE(with_tt.tt_stats() != nullptr);
  CHECK(with_tt.tt_stats()->hits > 0);
}

TEST_CASE("A6 TT-on equals TT-off on seeded UTTT positions") {
  const std::array<Position, 3> positions{
      seeded_position(0x123456789ABCDEF0ULL, 5),
      seeded_position(0x0F1E2D3C4B5A6978ULL, 7),
      seeded_position(0xA5A5A5A55A5A5A5AULL, 9),
  };

  for (const Position &position : positions) {
    REQUIRE(position.tie != Tie::NullFirstMove);
    Search<UtttModel> without_tt;
    Search<UtttModel> with_tt{10, TT::Mode::FullKey};
    const SearchResult expected =
        without_tt.solve(position, position.tie, {2, 100000});
    const SearchResult actual =
        with_tt.solve(position, position.tie, {2, 100000, true});
    check_same_search_result(actual, expected);
  }
}
