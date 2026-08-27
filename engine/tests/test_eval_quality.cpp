#include "doctest/doctest.h"

#include "eval/eval.hpp"
#include "nlohmann/json.hpp"
#include "search/search.hpp"
#include "support/ttt3_continuous.hpp"

#include <array>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace uttt;

namespace {

struct WideState {
  uint8_t kind = 0;
  TieState tie = TieState::X;
};

struct WideModel {
  using State = WideState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    if (state.kind != 0 && state.kind != 3) return {};
    std::vector<ModelChild<State>> result;
    for (uint8_t move = 0; move < 13; ++move)
      result.push_back({{1, TieState::O}, move});
    return result;
  }
  static std::vector<ModelChild<State>> children_o(const State &state) {
    if (state.kind != 0 && state.kind != 3) return {};
    std::vector<ModelChild<State>> result;
    for (uint8_t move = 0; move < 13; ++move)
      result.push_back({{2, TieState::X}, move});
    return result;
  }
  static TerminalKind terminal(const State &state) {
    if (state.kind == 1) return TerminalKind::MacroWinX;
    if (state.kind == 2) return TerminalKind::MacroWinO;
    return TerminalKind::None;
  }
  static int empties(const State &) { return 13; }
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
  static bool free_choice(const State &state) { return state.kind == 0; }
};

struct TwoPlyState {
  uint8_t ply = 0;
  TieState tie = TieState::X;
};

struct TwoPlyModel {
  using State = TwoPlyState;
  static std::vector<ModelChild<State>> children_x(const State &state) {
    return state.ply < 2
               ? std::vector<ModelChild<State>>{{{uint8_t(state.ply + 1), TieState::O}, 0}}
               : std::vector<ModelChild<State>>{};
  }
  static std::vector<ModelChild<State>> children_o(const State &state) {
    return state.ply < 2
               ? std::vector<ModelChild<State>>{{{uint8_t(state.ply + 1), TieState::X}, 0}}
               : std::vector<ModelChild<State>>{};
  }
  static TerminalKind terminal(const State &) { return TerminalKind::None; }
  static int empties(const State &) { return 1; }
  static TTKey tt_key(const State &state) { return {state.ply, state.ply}; }
  static PosId pos_id(const State &state) {
    PosId id{};
    id.x[0] = state.ply;
    id.tie = state.tie;
    return id;
  }
  static int chip_sign(const State &, int64_t bx, int64_t bo) {
    return (bx > bo) - (bx < bo);
  }
};

std::string slurp(const std::filesystem::path &path) {
  std::ifstream input(path);
  return {std::istreambuf_iterator<char>(input),
          std::istreambuf_iterator<char>()};
}

} // namespace

TEST_CASE("eval quality A9 widening is bound and force-exhaust flips exact") {
  Search<WideModel> widened;
  const SearchResult bounded = widened.solve({0, TieState::X}, Tie::X,
                                              {1, 1000, false, true, 12});
  Search<WideModel> exhausted;
  const SearchResult exact = exhausted.solve({0, TieState::X}, Tie::X,
                                              {1, 1000, false, true, 13});
  Search<WideModel> forced;
  const SearchResult forced_exact = forced.solve(
      {3, TieState::X}, Tie::X, {1, 1000, false, true, 12});

  REQUIRE(bounded.complete);
  CHECK(bounded.quality == Quality::Bound);
  CHECK(bounded.quality != Quality::Exact);
  CHECK(exact.quality == Quality::Exact);
  CHECK(forced_exact.quality == Quality::Exact);
  CHECK(bounded.t.lo == doctest::Approx(exact.t.lo));
  CHECK(bounded.t.hi == doctest::Approx(exact.t.hi));
}

TEST_CASE("eval quality A9 estimate taint propagates through two plies") {
  Search<TwoPlyModel> search;
  const SearchResult result =
      search.solve({0, TieState::X}, Tie::X, {2, 1000});
  REQUIRE(result.complete);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.t.lo == 0.0);
  CHECK(result.t.hi == 1.0);
}

TEST_CASE("eval quality A9 full-width cutoff keeps its sound side tight") {
  const Ttt3State state = Ttt3State::from_board("XXO.X.X..", Tie::X);
  const TestRational truth = solve_continuous(state, Tie::X).T;
  Search<Ttt3Model> search;
  const SearchResult result = search.solve(
      state, Tie::X, {4, 10000, false, false, 12},
      Window{{0.4, 0.6}, 0.0});
  REQUIRE(result.complete);
  CHECK(result.quality == Quality::Bound);
  CHECK(truth.inside(result.t.lo, result.t.hi));
  CHECK(result.t.hi - result.t.lo < 1e-13);
}

TEST_CASE("eval feature extractor is the budget-independent production input") {
  const Position position = Position::initial();
  const EvalFeatures features = eval_features(position);
  CHECK(features.size() == kEvalFeatureCount);
  const double estimate = eval_estimate(position);
  CHECK(estimate > 0.0);
  CHECK(estimate < 1.0);
}

TEST_CASE("eval fitter candidates are parameterized for the production sigmoid") {
  std::filesystem::path script =
      std::filesystem::current_path() / ".." / "tools" / "fit_eval.py";
  if (!std::filesystem::exists(script))
    script = std::filesystem::current_path() / "tools" / "fit_eval.py";
  const std::string command = "python3 \"" + script.string() + "\" --self-test";
  CHECK(std::system(command.c_str()) == 0);
}

TEST_CASE("eval quality A9 analyze emits convention and null through real executable") {
  using json = nlohmann::json;
  const std::filesystem::path stem =
      std::filesystem::temp_directory_path() /
      ("uttt-analyze-a9-" + std::to_string(getpid()));
  const auto input_path = stem.string() + ".in";
  const auto output_path = stem.string() + ".out";
  const auto error_path = stem.string() + ".err";

  const std::array<uint16_t, 9> xs = {227, 227, 227, 227, 227, 227, 227, 227, 227};
  const std::array<uint16_t, 9> os = {284, 284, 284, 284, 284, 284, 284, 284, 284};
  auto request = [&](int64_t bx, int64_t bo) {
    return json{{"parts", {{"x", xs}, {"o", os}, {"forced", nullptr}, {"tie", "X"}}},
                {"h", "X"}, {"bx", bx}, {"bo", bo}, {"depth", 6}};
  };
  {
    std::ofstream input(input_path);
    input << request(500000000, 500000000).dump() << '\n';
    input << request(400000000, 600000000).dump() << '\n';
  }
  std::filesystem::path engine = std::filesystem::current_path() / "uttt_engine";
  if (!std::filesystem::exists(engine))
    engine = std::filesystem::current_path() / "build" / "uttt_engine";
  const std::string command = "\"" + engine.string() + "\" analyze < \"" +
                              input_path + "\" > \"" + output_path +
                              "\" 2> \"" + error_path + "\"";
  REQUIRE(std::system(command.c_str()) == 0);

  std::ifstream output(output_path);
  std::string first_line;
  std::string second_line;
  REQUIRE(static_cast<bool>(std::getline(output, first_line)));
  REQUIRE(static_cast<bool>(std::getline(output, second_line)));
  const json positive = json::parse(first_line);
  const json negative = json::parse(second_line);
  CHECK(positive.at("equality_label") == "convention");
  CHECK(negative.at("equality_label").is_null());
  CHECK(positive.at("features").size() == kEvalFeatureCount);
  CHECK(positive.at("t_lo") == 0.5);
  CHECK(positive.at("t_hi") == 0.5);
  CHECK(slurp(error_path).empty());

  {
    std::ofstream input(input_path);
    json missing_budget = request(500000000, 500000000);
    missing_budget.erase("bo");
    input << missing_budget.dump() << '\n';
  }
  CHECK(std::system(command.c_str()) != 0);
  CHECK(slurp(error_path).find("missing required field") != std::string::npos);

  std::filesystem::remove(input_path);
  std::filesystem::remove(output_path);
  std::filesystem::remove(error_path);
}
