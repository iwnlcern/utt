#include "doctest/doctest.h"
#include "search/search.hpp"
#include "support/test_rational.hpp"
#include "support/ttt3_continuous.hpp"
#include "support/ttt3_model.hpp"

#include <algorithm>
#include <bit>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

using namespace uttt;

namespace {

constexpr uint8_t kNoMove = std::numeric_limits<uint8_t>::max();

struct ReferenceResult {
  TInterval t{0.0, 1.0};
  uint8_t best_x = kNoMove;
  uint8_t best_o = kNoMove;
  Quality quality = Quality::Estimate;
};

Quality combine_quality(Quality lhs, Quality rhs) {
  if (lhs == Quality::Estimate || rhs == Quality::Estimate)
    return Quality::Estimate;
  if (lhs == Quality::Bound || rhs == Quality::Bound)
    return Quality::Bound;
  return Quality::Exact;
}

template <GameModel M>
ReferenceResult unpruned_node(const typename M::State &state, Tie tie,
                              int remaining_depth) {
  switch (M::terminal(state)) {
  case TerminalKind::MacroWinX:
    return {{0.0, 0.0}, kNoMove, kNoMove, Quality::Exact};
  case TerminalKind::MacroWinO:
    return {{1.0, 1.0}, kNoMove, kNoMove, Quality::Exact};
  case TerminalKind::AllClosed:
    return {{0.5, 0.5}, kNoMove, kNoMove, Quality::Exact};
  case TerminalKind::None:
    break;
  }
  if (remaining_depth == 0)
    return {};

  const auto x_children = M::children_x(state);
  const auto o_children = M::children_o(state);
  REQUIRE(!x_children.empty());
  REQUIRE(!o_children.empty());

  Aggregates aggregates{};
  ReferenceResult result;
  result.quality = Quality::Exact;
  double best_x_hi = std::numeric_limits<double>::infinity();
  for (const auto &child : x_children) {
    const ReferenceResult searched =
        unpruned_node<M>(child.state, Tie::O, remaining_depth - 1);
    fold_x(aggregates, searched.t);
    result.quality = combine_quality(result.quality, searched.quality);
    if (searched.t.hi < best_x_hi ||
        (searched.t.hi == best_x_hi && child.move < result.best_x)) {
      best_x_hi = searched.t.hi;
      result.best_x = child.move;
    }
  }

  double best_o_lo = -std::numeric_limits<double>::infinity();
  for (const auto &child : o_children) {
    const ReferenceResult searched =
        unpruned_node<M>(child.state, Tie::X, remaining_depth - 1);
    fold_o(aggregates, searched.t);
    result.quality = combine_quality(result.quality, searched.quality);
    if (searched.t.lo > best_o_lo ||
        (searched.t.lo == best_o_lo && child.move > result.best_o)) {
      best_o_lo = searched.t.lo;
      result.best_o = child.move;
    }
  }
  result.t = backup_node(aggregates, tie);
  return result;
}

template <GameModel M>
ReferenceResult unpruned_solve(typename M::State state, Tie tie, int depth) {
  if (tie != Tie::NullFirstMove)
    return unpruned_node<M>(state, tie, depth);
  const ReferenceResult x = unpruned_node<M>(state, Tie::X, depth);
  const ReferenceResult o = unpruned_node<M>(state, Tie::O, depth);
  ReferenceResult result = x;
  result.t = {std::min(x.t.lo, o.t.lo), std::max(x.t.hi, o.t.hi)};
  result.quality = combine_quality(x.quality, o.quality);
  return result;
}

bool same_bits(double lhs, double rhs) {
  return std::bit_cast<uint64_t>(lhs) == std::bit_cast<uint64_t>(rhs);
}

void check_exactly_equal(const SearchResult &actual,
                         const ReferenceResult &expected) {
  CHECK(same_bits(actual.t.lo, expected.t.lo));
  CHECK(same_bits(actual.t.hi, expected.t.hi));
  CHECK(actual.best_x == expected.best_x);
  CHECK(actual.best_o == expected.best_o);
  CHECK(actual.quality == expected.quality);
  CHECK(actual.complete);
}

void check_no_cuts(const CutCounters &cuts) {
  CHECK(cuts.min_dominance == 0);
  CHECK(cuts.max_dominance == 0);
  CHECK(cuts.window_lo == 0);
  CHECK(cuts.window_hi == 0);
  CHECK(cuts.precision == 0);
  CHECK(cuts.hull_blocked == 0);
}

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

std::vector<Ttt3State> four_empty_sample() {
  std::vector<Ttt3State> result;
  std::size_t eligible = 0;
  for (uint32_t code = 0; code < 19683; ++code) {
    const std::string board = board_from_dense(code);
    if (std::count(board.begin(), board.end(), '.') != 4)
      continue;
    if ((eligible++ % 31) == 0)
      result.push_back(Ttt3State::from_board(board, Tie::X));
  }
  return result;
}

std::vector<Position> seeded_uttt_positions() {
  Position one = Position::initial();
  Position two = one.applied({4, 4}, Seat::X).value();
  Position three = one.applied({4, 0}, Seat::X).value();
  three = three.applied({0, 4}, Seat::O).value();
  return {one, two, three};
}

struct HullState {
  uint8_t id = 0;
  bool operator==(const HullState &) const = default;
};

struct HullWitnessModel {
  using State = HullState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    if (state.id == 0)
      return {{{4}, 3}};
    if (state.id == 1)
      return {{{2}, 4}};
    return {{{2}, 0}};
  }

  static std::vector<ModelChild<State>> children_o(const State &state) {
    if (state.id == 0)
      return {{{1}, 7}};
    if (state.id == 1)
      return {{{3}, 6}};
    return {{{2}, 0}};
  }

  static TerminalKind terminal(const State &state) {
    if (state.id == 3)
      return TerminalKind::AllClosed;
    if (state.id == 4)
      return TerminalKind::MacroWinX;
    return TerminalKind::None;
  }

  static int empties(const State &) { return 1; }
  static TTKey tt_key(const State &state) { return {state.id, state.id}; }
  static PosId pos_id(const State &state) {
    PosId result;
    result.x[0] = state.id;
    result.tie = Tie::X;
    return result;
  }
  static int chip_sign(const State &, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }
};

static_assert(GameModel<HullWitnessModel>);

struct PreimageState {
  uint8_t id = 0;
  bool operator==(const PreimageState &) const = default;
};

struct PreimageWitnessModel {
  using State = PreimageState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    if (state.id == 0)
      return {{{2}, 0}};
    if (state.id == 1)
      return {{{3}, 1}, {{4}, 2}};
    return {{{4}, 0}};
  }

  static std::vector<ModelChild<State>> children_o(const State &state) {
    if (state.id == 0)
      return {{{1}, 8}};
    if (state.id == 1)
      return {{{3}, 7}};
    return {{{4}, 0}};
  }

  static TerminalKind terminal(const State &state) {
    if (state.id == 2)
      return TerminalKind::MacroWinX;
    if (state.id == 3)
      return TerminalKind::AllClosed;
    return TerminalKind::None;
  }

  static int empties(const State &) { return 1; }
  static TTKey tt_key(const State &state) { return {state.id, state.id}; }
  static PosId pos_id(const State &state) {
    PosId result;
    result.x[0] = state.id;
    result.tie = Tie::X;
    return result;
  }
  static int chip_sign(const State &, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }
};

static_assert(GameModel<PreimageWitnessModel>);

} // namespace

TEST_CASE("A4 full window and zero precision are exact-neutral to Task 8") {
  const Window full{{0.0, 1.0}, 0.0};
  const auto family = three_marks_remaining_family();
  REQUIRE(family.size() == 5376);
  for (Ttt3State state : family) {
    for (Tie tie : {Tie::X, Tie::O}) {
      state.tie = tie;
      Search<Ttt3Model> search;
      const SearchResult actual = search.solve(state, tie, {3, 1000}, full);
      const ReferenceResult expected = unpruned_solve<Ttt3Model>(state, tie, 3);
      CAPTURE(state.dense_code());
      CAPTURE(tie);
      check_exactly_equal(actual, expected);
      check_no_cuts(actual.cuts);
    }
  }

  for (const Position &state : seeded_uttt_positions()) {
    const Tie tie = state.tie == Tie::NullFirstMove ? Tie::X : state.tie;
    Search<UtttModel> search;
    const SearchResult actual = search.solve(state, tie, {2, 100000}, full);
    const ReferenceResult expected = unpruned_solve<UtttModel>(state, tie, 2);
    CAPTURE(state.key);
    check_exactly_equal(actual, expected);
    check_no_cuts(actual.cuts);
  }
}

TEST_CASE("A4 bounded windows return sound same-side bounds") {
  struct Case {
    const char *name;
    const char *board;
    Window window;
    bool below;
  };
  const Case cases[] = {
      {"one-fifth below", "XXO.XXX..", {{0.4, 0.6}, 0.0}, true},
      {"four-fifths above", "XOOOO...O", {{0.4, 0.6}, 0.0}, false},
  };

  for (const Case &test : cases) {
    const Ttt3State state = Ttt3State::from_board(test.board, Tie::X);
    const TestRational truth = solve_continuous(state, Tie::X).T;
    const ReferenceResult unpruned = unpruned_solve<Ttt3Model>(state, Tie::X, 3);
    Search<Ttt3Model> search;
    const SearchResult bounded =
        search.solve(state, Tie::X, {3, 10000}, test.window);

    CAPTURE(test.name);
    CHECK(truth.inside(bounded.t.lo, bounded.t.hi));
    if (test.below)
      CHECK(bounded.t.hi < test.window.w.lo);
    else
      CHECK(bounded.t.lo > test.window.w.hi);
    CHECK(bounded.best_x == unpruned.best_x);
    CHECK(bounded.best_o == unpruned.best_o);
    CHECK(bounded.complete);
  }
}

TEST_CASE("A4 bounded children preserve the parent interval and both moves") {
  struct Case {
    const char *board;
    Tie tie;
    Window window;
    bool parent_interval_neutral;
  };
  const Case cases[] = {
      {"XXO.X.X..", Tie::X, {{0.4, 0.6}, 0.0}, true},
      {"XOOO....O", Tie::X, {{0.4, 0.6}, 0.0}, true},
      {"OXXXX....", Tie::O, {{0.7, 0.9}, 0.0}, false},
  };
  for (const Case &test : cases) {
    const char *board = test.board;
    const Ttt3State state = Ttt3State::from_board(board, Tie::X);
    const ReferenceResult unpruned =
        unpruned_solve<Ttt3Model>(state, test.tie, 4);
    Search<Ttt3Model> search;
    const SearchResult bounded = search.solve(
        state, test.tie, {4, 100000}, test.window);

    CAPTURE(board);
    if (std::string_view(board) == "OXXXX....") {
      CHECK(unpruned.best_x == 5);
      CHECK(unpruned.best_o == 8);
    }
    if (test.parent_interval_neutral) {
      CHECK(same_bits(bounded.t.lo, unpruned.t.lo));
      CHECK(same_bits(bounded.t.hi, unpruned.t.hi));
    }
    CHECK(bounded.best_x == unpruned.best_x);
    CHECK(bounded.best_o == unpruned.best_o);
    CHECK(bounded.complete);
  }
}

TEST_CASE("A4 bounded root moves match Task 8 on deterministic sample") {
  const auto sample = four_empty_sample();
  REQUIRE(sample.size() == 131);
  for (Ttt3State state : sample) {
    for (Tie tie : {Tie::X, Tie::O}) {
      state.tie = tie;
      const ReferenceResult unpruned =
          unpruned_solve<Ttt3Model>(state, tie, 4);
      Search<Ttt3Model> search;
      const SearchResult bounded = search.solve(
          state, tie, {4, 100000}, Window{{0.7, 0.9}, 0.0});

      CAPTURE(state.dense_code());
      CAPTURE(tie);
      CHECK(bounded.best_x == unpruned.best_x);
      CHECK(bounded.best_o == unpruned.best_o);
      CHECK(bounded.complete);
    }
  }
}

TEST_CASE("A4 precision result obeys width semantics") {
  const Ttt3State state = Ttt3State::from_board("XXO.X.X..", Tie::X);
  constexpr double eps = 0.125;
  Search<Ttt3Model> search;
  const SearchResult result =
      search.solve(state, Tie::X, {4, 100000}, Window{{0.0, 1.0}, eps});

  CHECK(width(result.t) <= eps);
  CHECK(result.cuts.precision > 0);
  CHECK(result.complete);
}

TEST_CASE("A4 named positions make every cutoff counter nonzero") {
  const Ttt3State low = Ttt3State::from_board("XXO.X.X..", Tie::X);
  const Ttt3State high = Ttt3State::from_board("XOOO....O", Tie::O);

  Search<Ttt3Model> low_search;
  const SearchResult low_result = low_search.solve(
      low, Tie::X, {4, 100000}, Window{{0.4, 0.6}, 0.125});
  Search<Ttt3Model> high_search;
  const SearchResult high_result = high_search.solve(
      high, Tie::O, {4, 100000}, Window{{0.4, 0.6}, 0.125});
  Search<HullWitnessModel> hull_search;
  const SearchResult hull_result = hull_search.solve(
      HullState{}, Tie::O, {2, 100}, Window{{0.1, 0.2}, 0.0});

  CAPTURE(low_result.cuts.min_dominance);
  CAPTURE(low_result.cuts.max_dominance);
  CAPTURE(low_result.cuts.window_lo);
  CAPTURE(low_result.cuts.window_hi);
  CAPTURE(low_result.cuts.precision);
  CAPTURE(low_result.cuts.hull_blocked);
  CAPTURE(high_result.cuts.min_dominance);
  CAPTURE(high_result.cuts.max_dominance);
  CAPTURE(high_result.cuts.window_lo);
  CAPTURE(high_result.cuts.window_hi);
  CAPTURE(high_result.cuts.precision);
  CAPTURE(high_result.cuts.hull_blocked);
  CAPTURE(hull_result.cuts.hull_blocked);
  CHECK(low_result.cuts.min_dominance > 0);
  CHECK(high_result.cuts.max_dominance > 0);
  CHECK(low_result.cuts.window_lo > 0);
  CHECK(high_result.cuts.window_hi > 0);
  CHECK(low_result.cuts.precision + high_result.cuts.precision > 0);
  CHECK(hull_result.cuts.hull_blocked > 0);
}

TEST_CASE("A4 hull straddle refuses a one-branch window cut") {
  const ReferenceResult unpruned =
      unpruned_solve<HullWitnessModel>(HullState{}, Tie::O, 2);
  Search<HullWitnessModel> search;
  const SearchResult result = search.solve(
      HullState{}, Tie::O, {2, 100}, Window{{0.1, 0.2}, 0.0});

  CHECK(result.cuts.hull_blocked > 0);
  CHECK(result.cuts.window_lo == 0);
  CHECK(result.cuts.window_hi == 0);
  CHECK(same_bits(result.t.lo, unpruned.t.lo));
  CHECK(same_bits(result.t.hi, unpruned.t.hi));
  CHECK(result.best_x == unpruned.best_x);
  CHECK(result.best_o == unpruned.best_o);
  CHECK(result.complete);
}

TEST_CASE("A4 crossed preimage intersection cuts an empty child outright") {
  const ReferenceResult unpruned =
      unpruned_solve<PreimageWitnessModel>(PreimageState{}, Tie::O, 2);
  Search<PreimageWitnessModel> search;
  const SearchResult result = search.solve(
      PreimageState{}, Tie::O, {2, 10}, Window{{0.8, 0.9}, 0.0});

  CHECK(result.complete);
  CHECK(result.cuts.window_lo > 0);
  CHECK(result.t.lo <= unpruned.t.lo);
  CHECK(unpruned.t.hi <= result.t.hi);
  CHECK(result.t.hi < 0.8);
  CHECK(result.best_x == unpruned.best_x);
  CHECK(result.best_o == unpruned.best_o);

  Search<PreimageWitnessModel> mutation_search;
  const SearchResult nonempty = mutation_search.solve(
      PreimageState{}, Tie::O, {2, 10}, Window{{0.4, 0.9}, 0.0});
  CHECK_FALSE(nonempty.complete);
}

TEST_CASE("cutoff search preserves node-cap incomplete publication") {
  Search<Ttt3Model> search;
  const SearchResult result = search.solve(
      Ttt3State::from_board(".........", Tie::X), Tie::X, {4, 1},
      Window{{0.4, 0.6}, 0.125});

  CHECK(result.t.lo == 0.0);
  CHECK(result.t.hi == 1.0);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.depth == 0);
  CHECK_FALSE(result.complete);
  CHECK(result.best_x == kNoMove);
  CHECK(result.best_o == kNoMove);
}
