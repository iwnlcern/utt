#include "doctest/doctest.h"

#include "eval/eval.hpp"
#include "nlohmann/json.hpp"
#include "search/search.hpp"
#include "support/ttt3_continuous.hpp"

#include <array>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <spawn.h>
#include <stdexcept>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

extern char **environ;

using namespace uttt;

namespace {

struct WideState {
  uint8_t kind = 0;
  TieState tie = TieState::X;
};

struct WideModel {
  using State = WideState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    if (state.kind != 0 && state.kind != 3)
      return {};
    std::vector<ModelChild<State>> result;
    for (uint8_t move = 0; move < 13; ++move)
      result.push_back({{1, TieState::O}, move});
    return result;
  }
  static std::vector<ModelChild<State>> children_o(const State &state) {
    if (state.kind != 0 && state.kind != 3)
      return {};
    std::vector<ModelChild<State>> result;
    for (uint8_t move = 0; move < 13; ++move)
      result.push_back({{2, TieState::X}, move});
    return result;
  }
  static TerminalKind terminal(const State &state) {
    if (state.kind == 1)
      return TerminalKind::MacroWinX;
    if (state.kind == 2)
      return TerminalKind::MacroWinO;
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
               ? std::vector<ModelChild<State>>{{{uint8_t(state.ply + 1),
                                                  TieState::O},
                                                 0}}
               : std::vector<ModelChild<State>>{};
  }
  static std::vector<ModelChild<State>> children_o(const State &state) {
    return state.ply < 2
               ? std::vector<ModelChild<State>>{{{uint8_t(state.ply + 1),
                                                  TieState::X},
                                                 0}}
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

struct EstimateState {
  uint8_t kind = 0;
  TieState tie = TieState::X;
};

struct EstimateModel {
  using State = EstimateState;

  static std::vector<ModelChild<State>> children_x(const State &state) {
    switch (state.kind) {
    case 0:
      return {{{10, TieState::O}, 0}, {{11, TieState::O}, 1}};
    case 1:
      return {{{13, TieState::O}, 0}, {{14, TieState::O}, 1}};
    case 2:
      return {{{11, TieState::O}, 0}, {{12, TieState::O}, 1}};
    case 3:
      return {{{20, TieState::O}, 0}, {{13, TieState::O}, 1}};
    case 4:
      return {{{22, TieState::O}, 0}};
    default:
      return {};
    }
  }
  static std::vector<ModelChild<State>> children_o(const State &state) {
    switch (state.kind) {
    case 0:
      return {{{12, TieState::X}, 0}, {{13, TieState::X}, 1}};
    case 1:
      return {{{10, TieState::X}, 0}, {{11, TieState::X}, 1}};
    case 2:
      return {{{10, TieState::X}, 0}, {{11, TieState::X}, 1}};
    case 3:
      return {{{21, TieState::X}, 0}};
    case 4:
      return {{{3, TieState::X}, 0}};
    default:
      return {};
    }
  }
  static TerminalKind terminal(const State &state) {
    if (state.kind == 20)
      return TerminalKind::AllClosed;
    if (state.kind == 21)
      return TerminalKind::MacroWinO;
    if (state.kind == 22)
      return TerminalKind::MacroWinX;
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
  static double estimate(const State &state) {
    switch (state.kind) {
    case 10:
      return 0.2;
    case 11:
      return 0.4;
    case 12:
      return 0.6;
    case 13:
      return 0.8;
    case 14:
      return 0.9;
    default:
      return 0.5;
    }
  }
};

std::string slurp(const std::filesystem::path &path) {
  std::ifstream input(path);
  return {std::istreambuf_iterator<char>(input),
          std::istreambuf_iterator<char>()};
}

class TempFile {
public:
  explicit TempFile(std::string_view label) {
    std::string pattern = (std::filesystem::temp_directory_path() /
                           ("uttt-" + std::string(label) + "-XXXXXX"))
                              .string();
    std::vector<char> writable(pattern.begin(), pattern.end());
    writable.push_back('\0');
    const int descriptor = mkstemp(writable.data());
    if (descriptor < 0)
      throw std::runtime_error("mkstemp failed");
    close(descriptor);
    path_ = writable.data();
  }
  TempFile(const TempFile &) = delete;
  TempFile &operator=(const TempFile &) = delete;
  ~TempFile() {
    std::error_code ignored;
    std::filesystem::remove(path_, ignored);
  }
  const std::filesystem::path &path() const { return path_; }
  void write(std::string_view contents) const {
    std::ofstream output(path_, std::ios::trunc);
    if (!output)
      throw std::runtime_error("temporary file open failed");
    output << contents;
  }

private:
  std::filesystem::path path_;
};

int run_process(const std::vector<std::string> &arguments,
                const TempFile *input = nullptr,
                const TempFile *output = nullptr,
                const TempFile *error = nullptr) {
  posix_spawn_file_actions_t actions;
  if (posix_spawn_file_actions_init(&actions) != 0)
    throw std::runtime_error("posix_spawn_file_actions_init failed");
  auto destroy_actions = [&] { posix_spawn_file_actions_destroy(&actions); };
  if (input && posix_spawn_file_actions_addopen(&actions, STDIN_FILENO,
                                                input->path().c_str(), O_RDONLY,
                                                0) != 0) {
    destroy_actions();
    throw std::runtime_error("posix_spawn stdin action failed");
  }
  if (output && posix_spawn_file_actions_addopen(
                    &actions, STDOUT_FILENO, output->path().c_str(),
                    O_WRONLY | O_CREAT | O_TRUNC, 0600) != 0) {
    destroy_actions();
    throw std::runtime_error("posix_spawn stdout action failed");
  }
  if (error && posix_spawn_file_actions_addopen(
                   &actions, STDERR_FILENO, error->path().c_str(),
                   O_WRONLY | O_CREAT | O_TRUNC, 0600) != 0) {
    destroy_actions();
    throw std::runtime_error("posix_spawn stderr action failed");
  }
  std::vector<char *> argv;
  argv.reserve(arguments.size() + 1);
  for (const std::string &argument : arguments)
    argv.push_back(const_cast<char *>(argument.c_str()));
  argv.push_back(nullptr);
  pid_t pid = 0;
  const int spawn_error = posix_spawnp(&pid, arguments.front().c_str(),
                                       &actions, nullptr, argv.data(), environ);
  destroy_actions();
  if (spawn_error != 0)
    return 127;
  int status = 0;
  if (waitpid(pid, &status, 0) != pid)
    return 127;
  return WIFEXITED(status) ? WEXITSTATUS(status) : 128;
}

void check_analyze_schema(const nlohmann::json &value) {
  CHECK(value.is_object());
  CHECK(value.size() == 7);
  CHECK(value.at("t_lo").is_number());
  CHECK(value.at("t_hi").is_number());
  CHECK(value.at("quality").is_string());
  CHECK(value.at("depth").is_number_integer());
  CHECK(value.at("complete").is_boolean());
  CHECK(value.contains("equality_label"));
  CHECK(value.at("features").is_array());
  CHECK(value.at("features").size() == kEvalFeatureCount);
}

} // namespace

TEST_CASE("eval quality A9 widening is bound and force-exhaust flips exact") {
  Search<WideModel> widened;
  const SearchResult bounded =
      widened.solve({0, TieState::X}, Tie::X, {1, 1000, false, true, 12});
  Search<WideModel> exhausted;
  const SearchResult exact =
      exhausted.solve({0, TieState::X}, Tie::X, {1, 1000, false, true, 13});
  Search<WideModel> forced;
  const SearchResult forced_exact =
      forced.solve({3, TieState::X}, Tie::X, {1, 1000, false, true, 12});

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
  const SearchResult result = search.solve({0, TieState::X}, Tie::X, {2, 1000});
  REQUIRE(result.complete);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.t.lo == 0.0);
  CHECK(result.t.hi == 1.0);
}

TEST_CASE("eval quality A9 t_est propagates X-min O-max and ordered F") {
  Search<EstimateModel> search;
  const SearchResult result = search.solve({0, TieState::X}, Tie::X, {1, 1000});
  REQUIRE(result.complete);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.t_est == doctest::Approx(0.5));
}

TEST_CASE("eval quality A9 t_est uses tie-owner zugzwang branches") {
  Search<EstimateModel> x_search;
  Search<EstimateModel> o_search;
  const SearchResult x = x_search.solve({1, TieState::X}, Tie::X, {1, 1000});
  const SearchResult o = o_search.solve({1, TieState::O}, Tie::O, {1, 1000});
  REQUIRE(x.complete);
  REQUIRE(o.complete);
  CHECK(x.t_est == doctest::Approx(0.8));
  CHECK(o.t_est == doctest::Approx(0.4));
}

TEST_CASE("eval quality A9 t_est equality seam is continuous") {
  Search<EstimateModel> search;
  const SearchResult result = search.solve({2, TieState::X}, Tie::X, {1, 1000});
  REQUIRE(result.complete);
  CHECK(result.t_est == doctest::Approx(0.4));
}

TEST_CASE("eval quality A9 t_est survives an estimate-tainted cutoff") {
  Search<EstimateModel> search;
  const SearchResult result = search.solve({4, TieState::O}, Tie::O, {2, 1000},
                                           Window{{0.0, 0.1}, 0.0});
  REQUIRE(result.complete);
  CHECK(result.cuts.window_hi > 0);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.t_est == doctest::Approx(0.4));
}

TEST_CASE("eval quality A9 bound and TT hits use the interval midpoint") {
  Search<WideModel> search(8, TT::Mode::FullKey);
  const Limits bounded_limits{1, 1000, true, true, 12};
  const SearchResult first =
      search.solve({0, TieState::X}, Tie::X, bounded_limits);
  REQUIRE(first.complete);
  REQUIRE(first.quality == Quality::Bound);
  CHECK(first.t_est == doctest::Approx((first.t.lo + first.t.hi) / 2.0));
  const uint64_t hits_before = search.tt_stats()->hits;
  const SearchResult second =
      search.solve({0, TieState::X}, Tie::X, bounded_limits);
  CHECK(search.tt_stats()->hits > hits_before);
  CHECK(second.t_est == doctest::Approx((second.t.lo + second.t.hi) / 2.0));
}

TEST_CASE("eval quality A9 estimate entries are never reusable pure TT hits") {
  Search<EstimateModel> search(8, TT::Mode::FullKey);
  const Limits limits{1, 1000, true, false, 12};
  REQUIRE(search.solve({0, TieState::X}, Tie::X, limits).complete);
  const SearchResult second = search.solve({0, TieState::X}, Tie::X, limits);
  REQUIRE(second.complete);
  CHECK(second.quality == Quality::Estimate);
  CHECK(search.evaluator_calls() == 4);
  CHECK(second.t_est == doctest::Approx(0.5));
}

TEST_CASE(
    "eval quality A9 production evaluator taints real UTTT through two plies") {
  Search<UtttModel> search;
  const SearchResult result =
      search.solve(Position::initial(), Tie::X, {2, 100000});
  REQUIRE(result.complete);
  CHECK(search.evaluator_calls() > 0);
  CHECK(result.quality == Quality::Estimate);
  CHECK(result.t.lo == 0.0);
  CHECK(result.t.hi == 1.0);
}

TEST_CASE("eval quality A9 full-width cutoff keeps its sound side tight") {
  const Ttt3State state = Ttt3State::from_board("XXO.X.X..", Tie::X);
  const TestRational truth = solve_continuous(state, Tie::X).T;
  Search<Ttt3Model> search;
  const SearchResult result = search.solve(
      state, Tie::X, {4, 10000, false, false, 12}, Window{{0.4, 0.6}, 0.0});
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

TEST_CASE("eval features exclude macro lines blocked by a drawn board") {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  x[0] = 0b000000111;
  x[1] = 0b011100011;
  o[1] = 0b100011100;
  const auto position = Position::from_parts(x, o, 2, TieState::X);
  REQUIRE(position.has_value());
  const EvalFeatures features = eval_features(*position);
  CHECK(features[11] == 2.0);
  CHECK(features[12] == 0.0);
}

TEST_CASE(
    "eval features traverse forced-board legal cells to destination richness") {
  std::array<uint16_t, 9> x{};
  std::array<uint16_t, 9> o{};
  x[0] = 0b000000011;
  o[1] = 0b000000011;
  x[4] = 0b011100000;
  o[4] = 0b100011100;
  const auto position = Position::from_parts(x, o, 4, TieState::X);
  REQUIRE(position.has_value());
  const EvalFeatures features = eval_features(*position);
  CHECK(features[13] == 20.0);
  CHECK(features[14] == 20.0);
}

TEST_CASE(
    "eval fitter candidates are parameterized for the production sigmoid") {
  CHECK(std::filesystem::path(UTTT_TEST_FIT_SCRIPT).is_absolute());
  CHECK(run_process({"python3", UTTT_TEST_FIT_SCRIPT, "--self-test"}) == 0);
}

TEST_CASE("eval quality A9 analyze emits convention and null through real "
          "executable") {
  using json = nlohmann::json;
  TempFile input("analyze-in");
  TempFile output("analyze-out");
  TempFile error("analyze-err");

  const std::array<uint16_t, 9> xs = {227, 227, 227, 227, 227,
                                      227, 227, 227, 227};
  const std::array<uint16_t, 9> os = {284, 284, 284, 284, 284,
                                      284, 284, 284, 284};
  auto request = [&](const auto &px, const auto &po, json forced,
                     json state_tie, int64_t bx, int64_t bo, int depth) {
    return json{{"parts",
                 {{"x", px},
                  {"o", po},
                  {"forced", std::move(forced)},
                  {"tie", std::move(state_tie)}}},
                {"h", "X"},
                {"bx", bx},
                {"bo", bo},
                {"depth", depth}};
  };
  std::array<uint16_t, 9> empty{};
  std::array<uint16_t, 9> macro_x{};
  std::array<uint16_t, 9> macro_o{};
  macro_x[0] = macro_x[1] = macro_x[2] = 0b000000111;
  macro_o[0] = macro_o[1] = macro_o[2] = 0b000000111;
  constexpr int64_t max_total = 4294967295LL;

  std::vector<json> requests;
  requests.push_back(request(xs, os, nullptr, "X", 500000000, 500000000, 6));
  requests.push_back(request(xs, os, nullptr, "X", 400000000, 600000000, 6));
  requests.push_back(request(empty, empty, 4, nullptr, 0, max_total, 0));
  requests.push_back(request(macro_x, empty, 3, "X", 0, max_total, 6));
  requests.push_back(request(empty, macro_o, 3, "X", max_total, 0, 6));
  requests.push_back(request(xs, os, nullptr, "X", 0, 0, 6));
  std::string input_text;
  for (const json &value : requests)
    input_text += value.dump() + "\n";
  input.write(input_text);

  CHECK(std::filesystem::path(UTTT_TEST_ENGINE_PATH).is_absolute());
  REQUIRE(run_process({UTTT_TEST_ENGINE_PATH, "analyze"}, &input, &output,
                      &error) == 0);
  CHECK(slurp(error.path()).empty());
  std::ifstream result_stream(output.path());
  std::vector<json> replies;
  std::string line;
  while (std::getline(result_stream, line))
    replies.push_back(json::parse(line));
  REQUIRE(replies.size() == requests.size());
  for (const json &reply : replies)
    check_analyze_schema(reply);
  CHECK(replies[0].at("equality_label") == "convention");
  CHECK(replies[0].at("t_lo") == 0.5);
  CHECK(replies[0].at("t_hi") == 0.5);
  CHECK(replies[1].at("equality_label").is_null());
  CHECK(replies[2].at("t_lo") == 0.0);
  CHECK(replies[2].at("t_hi") == 1.0);
  CHECK(replies[2].at("equality_label").is_null());
  CHECK(replies[3].at("t_lo") == 0.0);
  CHECK(replies[3].at("t_hi") == 0.0);
  CHECK(replies[3].at("equality_label") == "convention");
  CHECK(replies[4].at("t_lo") == 1.0);
  CHECK(replies[4].at("t_hi") == 1.0);
  CHECK(replies[4].at("equality_label") == "convention");
  CHECK(replies[5].at("equality_label").is_null());

  json rejected = requests[0];
  rejected["depth"] = 13;
  input.write(requests[0].dump() + "\n" + rejected.dump() + "\n" +
              requests[1].dump() + "\n");
  REQUIRE(run_process({UTTT_TEST_ENGINE_PATH, "analyze"}, &input, &output,
                      &error) == 0);
  CHECK(slurp(error.path()).empty());
  std::ifstream ordered_stream(output.path());
  std::vector<json> ordered;
  while (std::getline(ordered_stream, line))
    ordered.push_back(json::parse(line));
  REQUIRE(ordered.size() == 3);
  check_analyze_schema(ordered[0]);
  CHECK(ordered[1] == json{{"error", "analyze depth exceeds 12"}, {"line", 2}});
  check_analyze_schema(ordered[2]);

  const json adversarial = request(empty, empty, 4, nullptr, 50, 50, 12);
  input.write(adversarial.dump() + "\n");
  REQUIRE(run_process({UTTT_TEST_ENGINE_PATH, "analyze"}, &input, &output,
                      &error) == 0);
  CHECK(slurp(error.path()).empty());
  const json bounded = json::parse(slurp(output.path()));
  check_analyze_schema(bounded);
  CHECK_FALSE(bounded.at("complete").get<bool>());
  CHECK(bounded.at("depth").get<int>() < 12);
}
