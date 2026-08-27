#include "doctest/doctest.h"

#include "root/alt_solver.hpp"
#include "search/tt.hpp"
#include "support/ttt3_discrete.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

using namespace uttt;

namespace {

bool has_win(uint16_t marks) {
  constexpr std::array<uint16_t, 8> lines{
      0b000000111, 0b000111000, 0b111000000, 0b001001001,
      0b010010010, 0b100100100, 0b100010001, 0b001010100,
  };
  return std::any_of(lines.begin(), lines.end(),
                     [marks](uint16_t line) { return (marks & line) == line; });
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

struct ProbeState {
  uint8_t kind = 0;
  TieState tie = TieState::X;
};

struct ProbeModel {
  using State = ProbeState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    if (state.kind != 0)
      return {};
    return {{{1, TieState::O}, 0}, {{2, TieState::O}, 1}};
  }
  static std::vector<ModelChild<State>> children_o(const State &state) {
    if (state.kind != 0)
      return {};
    return {{{2, TieState::X}, 0}, {{1, TieState::X}, 1}};
  }
  static TerminalKind terminal(const State &state) {
    if (state.kind == 1)
      return TerminalKind::MacroWinX;
    if (state.kind == 2)
      return TerminalKind::MacroWinO;
    return TerminalKind::None;
  }
  static int empties(const State &) { return 1; }
  static TTKey tt_key(const State &state) { return {state.kind, state.kind}; }
  static PosId pos_id(const State &state) {
    PosId id{};
    id.x[0] = state.kind;
    id.tie = state.tie;
    return id;
  }
  static int chip_sign(const State &, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }
};

static_assert(GameModel<ProbeModel>);

AltLimits limits(int depth) {
  return {depth, std::numeric_limits<uint64_t>::max(), 1, {}};
}

} // namespace

TEST_CASE("alternation solver matches discrete scale-zero on all 22186 nodes") {
  AltSolver<Ttt3Model> solver;
  std::size_t checked = 0;
  for (uint32_t code = 0; code < 19683; ++code) {
    const std::string board = board_from_dense(code);
    const Ttt3State probe = Ttt3State::from_board(board, Tie::X);
    if (has_win(probe.x) || has_win(probe.o) ||
        board.find('.') == std::string::npos)
      continue;
    for (Tie tie : {Tie::X, Tie::O}) {
      const Ttt3State state = Ttt3State::from_board(board, tie);
      const AltResult actual =
          solver.solve(state, tie, limits(Ttt3Model::empties(state)));
      CAPTURE(board);
      CAPTURE(tie);
      REQUIRE(actual.complete);
      CHECK(actual.quality == Quality::Exact);
      CHECK(actual.value ==
            static_cast<int>(solve_discrete(state, tie, 0, 0)) - 1);
      ++checked;
    }
  }
  CHECK(checked == 22'186);
}

TEST_CASE(
    "alternation solver uses mover-only children and opposite polarities") {
  AltSolver<ProbeModel> solver;
  const AltResult x = solver.solve({0, TieState::X}, Tie::X, limits(1));
  const AltResult o = solver.solve({0, TieState::O}, Tie::O, limits(1));
  REQUIRE(x.complete);
  REQUIRE(o.complete);
  CHECK(x.value == 1.0);
  CHECK(o.value == -1.0);
}

TEST_CASE(
    "alternation horizon maps evaluator estimates into X-centric values") {
  const auto evaluate = [](const ProbeState &state) {
    constexpr std::array<double, 7> values{-1.0, 0.0, 0.25, 0.5,
                                           0.75, 1.0, 2.0};
    return values.at(state.kind - 3);
  };
  constexpr std::array<double, 7> expected{1.0,  1.0,  0.5, 0.0,
                                           -0.5, -1.0, -1.0};
  AltSolver<ProbeModel> solver(evaluate);
  for (uint8_t index = 0; index < expected.size(); ++index) {
    const uint8_t kind = static_cast<uint8_t>(index + 3);
    const AltResult result =
        solver.solve({kind, TieState::X}, Tie::X, limits(0));
    CAPTURE(kind);
    REQUIRE(result.complete);
    CHECK(result.quality == Quality::Estimate);
    CHECK(result.value == doctest::Approx(expected[index]));
  }
}

TEST_CASE(
    "alternation memo is collision-safe depth-sensitive and reuses roots") {
  const auto evaluate = [](const ProbeState &state) {
    return state.kind == 3 ? 0.25 : 0.75;
  };
  AltSolver<ProbeModel> solver(
      evaluate, [](const PosId &, Tie, int) { return std::size_t{0}; });

  const AltResult first = solver.solve({3, TieState::X}, Tie::X, limits(0));
  const AltResult second = solver.solve({4, TieState::X}, Tie::X, limits(0));
  const AltResult repeated = solver.solve({3, TieState::X}, Tie::X, limits(0));
  CHECK(first.value == 0.5);
  CHECK(second.value == -0.5);
  CHECK(repeated.value == first.value);
  CHECK(solver.unique_root_searches() == 2);

  const AltResult shallow = solver.solve({0, TieState::X}, Tie::X, limits(0));
  const AltResult deeper = solver.solve({0, TieState::X}, Tie::X, limits(1));
  CHECK(shallow.value == -0.5);
  CHECK(deeper.value == 1.0);
}

TEST_CASE("alternation solver leaves the threshold TT bit-identical") {
  TT table(2, TT::Mode::FullKey);
  PosId id{};
  id.x[0] = 7;
  id.tie = Tie::X;
  TTEntry entry{};
  entry.lo = 0.25;
  entry.hi = 0.75;
  entry.move_x = 2;
  entry.move_o = 6;
  entry.depth = 4;
  entry.flags = kTTQualityBound | kTTComplete;
  table.store({7, 11}, id, entry);
  const auto stored = table.probe({7, 11}, id);
  REQUIRE(stored.has_value());
  const CollisionStats before = table.stats;

  AltSolver<ProbeModel> solver;
  REQUIRE(solver.solve({0, TieState::X}, Tie::X, limits(1)).complete);
  CHECK(table.stats.collisions == before.collisions);
  CHECK(table.stats.hits == before.hits);
  CHECK(table.stats.misses == before.misses);
  CHECK(table.stats.stores == before.stores);
  const auto after = table.probe({7, 11}, id);
  REQUIRE(after.has_value());
  CHECK(after->lo == stored->lo);
  CHECK(after->hi == stored->hi);
  CHECK(after->move_x == stored->move_x);
  CHECK(after->move_o == stored->move_o);
  CHECK(after->depth == stored->depth);
  CHECK(after->flags == stored->flags);
  CHECK(sizeof(TTEntry) == 32);
}

TEST_CASE(
    "alternation cancellation returns incomplete and caches no partial value") {
  AltSolver<ProbeModel> solver;
  int polls = 0;
  AltLimits stopped = limits(1);
  stopped.stop_requested = [&] { return ++polls == 2; };
  const AltResult result = solver.solve({0, TieState::X}, Tie::X, stopped);
  CHECK_FALSE(result.complete);
  CHECK(solver.memo_entries() == 0);
}
