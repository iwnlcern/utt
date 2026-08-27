#include "doctest/doctest.h"
#include "search/search.hpp"
#include "support/test_rational.hpp"
#include "support/ttt3_continuous.hpp"
#include "support/ttt3_model.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

using namespace uttt;

namespace {

constexpr uint8_t kNoMove = std::numeric_limits<uint8_t>::max();

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
    if (std::count(board.begin(), board.end(), '.') != 3)
      continue;
    result.push_back(Ttt3State::from_board(board, Tie::X));
  }
  return result;
}

void check_valid(TInterval interval) {
  CHECK(0.0 <= interval.lo);
  CHECK(interval.lo <= interval.hi);
  CHECK(interval.hi <= 1.0);
}

void check_subset(TInterval inner, TInterval outer) {
  CHECK(outer.lo <= inner.lo);
  CHECK(inner.hi <= outer.hi);
}

TestRational abs_rational(TestRational value) {
  return value < TestRational{0} ? TestRational{0} - value : value;
}

} // namespace

TEST_CASE("search maps all terminal kinds to their locked point intervals") {
  Search<Ttt3Model> search;

  const SearchResult x_win = search.solve(
      Ttt3State::from_board("XXX......", Tie::O), Tie::O, {9, 100});
  const SearchResult o_win = search.solve(
      Ttt3State::from_board("O..O..O..", Tie::X), Tie::X, {9, 100});
  const SearchResult all_closed = search.solve(
      Ttt3State::from_board("XOXXOOOXX", Tie::X), Tie::X, {9, 100});

  CHECK(x_win.t.lo == 0.0);
  CHECK(x_win.t.hi == 0.0);
  CHECK(o_win.t.lo == 1.0);
  CHECK(o_win.t.hi == 1.0);
  CHECK(all_closed.t.lo == 0.5);
  CHECK(all_closed.t.hi == 0.5);
  CHECK(x_win.quality == Quality::Exact);
  CHECK(o_win.quality == Quality::Exact);
  CHECK(all_closed.quality == Quality::Exact);
  CHECK(x_win.best_x == kNoMove);
  CHECK(x_win.best_o == kNoMove);
}

TEST_CASE("search depth zero is a completed estimate with no invented moves") {
  Search<Ttt3Model> search;
  const SearchResult result =
      search.solve(Ttt3State::from_board(".........", Tie::X), Tie::X, {0, 1});

  CHECK(result.t.lo == 0.0);
  CHECK(result.t.hi == 1.0);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.depth == 0);
  CHECK(result.complete);
  CHECK(result.best_x == kNoMove);
  CHECK(result.best_o == kNoMove);
}

TEST_CASE(
    "A3 full-width search encloses exact values on every three-empty state") {
  const auto family = three_marks_remaining_family();
  REQUIRE(family.size() == 5376);

  std::size_t conditional_states = 0;
  for (Ttt3State state : family) {
    for (Tie tie : {Tie::X, Tie::O}) {
      state.tie = tie;
      Search<Ttt3Model> search;
      const SearchResult actual = search.solve(state, tie, {3, 1000});
      const TestRational exact = solve_continuous(state, tie).T;

      CAPTURE(state.dense_code());
      CAPTURE(tie);
      CHECK(exact.inside(actual.t.lo, actual.t.hi));
      CHECK(actual.quality == Quality::Exact);
      CHECK(actual.depth == 3);
      CHECK(actual.complete);
      ++conditional_states;
    }
  }
  CHECK(conditional_states == 10752);
}

TEST_CASE("A3 a deeper full-width limit only narrows the three-empty family") {
  const auto family = three_marks_remaining_family();
  REQUIRE(family.size() == 5376);

  std::size_t conditional_states = 0;
  for (Ttt3State state : family) {
    for (Tie tie : {Tie::X, Tie::O}) {
      state.tie = tie;
      Search<Ttt3Model> shallow_search;
      Search<Ttt3Model> deeper_search;
      const SearchResult shallow = shallow_search.solve(state, tie, {1, 1000});
      const SearchResult deeper = deeper_search.solve(state, tie, {2, 1000});

      CAPTURE(state.dense_code());
      CAPTURE(tie);
      check_subset(deeper.t, shallow.t);
      CHECK(shallow.complete);
      CHECK(deeper.complete);
      CHECK(shallow.depth == 1);
      CHECK(deeper.depth == 2);
      ++conditional_states;
    }
  }
  CHECK(conditional_states == 10752);
}

TEST_CASE(
    "A2 both conditional roots contain one half within every fixture unit") {
  constexpr std::array<int64_t, 3> kFixtureScales{8, 16, 32};
  const TestRational exact{1, 2};

  for (Tie tie : {Tie::X, Tie::O}) {
    Search<Ttt3Model> search;
    const Ttt3State root = Ttt3State::from_board(".........", tie);
    const SearchResult actual = search.solve(root, tie, {2, 100000});
    const TestRational oracle = solve_continuous(root, tie).T;
    const TestRational midpoint = (TestRational::from_double(actual.t.lo) +
                                   TestRational::from_double(actual.t.hi)) /
                                  TestRational{2};

    CAPTURE(tie);
    CHECK(oracle == exact);
    CHECK(oracle.inside(actual.t.lo, actual.t.hi));
    for (const int64_t scale : kFixtureScales) {
      CAPTURE(scale);
      CHECK(abs_rational(midpoint - exact) * TestRational{scale} <=
            TestRational{1});
    }
    CHECK(actual.complete);
    CHECK(actual.depth == 2);
  }
}

TEST_CASE("null root is only the envelope of both conditional games") {
  const Ttt3State root = Ttt3State::from_board("X........", Tie::NullFirstMove);
  Search<Ttt3Model> x_search;
  Search<Ttt3Model> o_search;
  Search<Ttt3Model> null_search;
  const SearchResult conditional_x = x_search.solve(root, Tie::X, {3, 100000});
  const SearchResult conditional_o = o_search.solve(root, Tie::O, {3, 100000});
  const SearchResult envelope =
      null_search.solve(root, Tie::NullFirstMove, {3, 200000});

  CHECK(envelope.t.lo == std::min(conditional_x.t.lo, conditional_o.t.lo));
  CHECK(envelope.t.hi == std::max(conditional_x.t.hi, conditional_o.t.hi));
  CHECK(envelope.best_x == conditional_x.best_x);
  CHECK(envelope.best_o == conditional_x.best_o);
  CHECK(envelope.complete);
}

TEST_CASE("node cap aborts without publishing a partial iteration") {
  Search<Ttt3Model> search;
  const SearchResult result =
      search.solve(Ttt3State::from_board(".........", Tie::X), Tie::X, {3, 1});

  CHECK(result.t.lo == 0.0);
  CHECK(result.t.hi == 1.0);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.depth == 0);
  CHECK_FALSE(result.complete);
  CHECK(result.best_x == kNoMove);
  CHECK(result.best_o == kNoMove);
}

TEST_CASE("search chooses deterministic extrema and UTTT opening moves") {
  Search<Ttt3Model> ttt3_search;
  const SearchResult ttt3 = ttt3_search.solve(
      Ttt3State::from_board(".........", Tie::X), Tie::X, {1, 1000});
  CHECK(ttt3.best_x == 0);
  CHECK(ttt3.best_o == 8);

  Search<UtttModel> uttt_search;
  const SearchResult opening =
      uttt_search.solve(Position::initial(), Tie::NullFirstMove, {2, 100000});
  check_valid(opening.t);
  CHECK(opening.complete);
  CHECK(opening.depth == 2);
  CHECK(opening.quality == Quality::Estimate);
  CHECK(opening.best_x >= 36);
  CHECK(opening.best_x <= 44);
  CHECK(opening.best_o >= 36);
  CHECK(opening.best_o <= 44);
}
