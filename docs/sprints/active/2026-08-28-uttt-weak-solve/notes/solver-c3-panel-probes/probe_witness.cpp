// Witness/fact validation probe: walks psr-v1 log segments produced by the
// kernel cross-check probe and validates a sample of recorded proven facts
// against the independent rules implementation:
//  - state parses and is canonical
//  - witness present iff prover-to-move and nonterminal
//  - witness is a legal move of the canonical state
//  - claim truth re-derived by an independent bounded solve of the state and,
//    when a witness exists, preserved at the witness child.
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <random>
#include <unordered_map>
#include <vector>

#include "canon/canon.hpp"
#include "core/state.hpp"
#include "store/record.hpp"

namespace indep {
struct IState {
  std::array<std::uint8_t, 81> cell{};
  int forced = 4;
  bool operator==(const IState&) const = default;
};
struct IStateHash {
  std::size_t operator()(const IState& s) const {
    std::size_t h = 1469598103934665603ull;
    for (auto c : s.cell) { h ^= c; h *= 1099511628211ull; }
    h ^= static_cast<std::size_t>(s.forced + 1);
    h *= 1099511628211ull;
    return h;
  }
};
const int kLines[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
int local_winner(const IState& s, int b) {
  for (auto& L : kLines) {
    const std::uint8_t a = s.cell[b*9 + L[0]];
    if (a != 0 && a == s.cell[b*9 + L[1]] && a == s.cell[b*9 + L[2]]) return a;
  }
  return 0;
}
bool local_full(const IState& s, int b) {
  for (int c = 0; c < 9; ++c) if (s.cell[b*9 + c] == 0) return false;
  return true;
}
bool local_closed(const IState& s, int b) {
  return local_winner(s, b) != 0 || local_full(s, b);
}
int terminal(const IState& s) {
  int macro[9];
  for (int b = 0; b < 9; ++b) macro[b] = local_winner(s, b);
  for (auto& L : kLines) {
    if (macro[L[0]] != 0 && macro[L[0]] == macro[L[1]] && macro[L[0]] == macro[L[2]]) {
      return macro[L[0]];
    }
  }
  for (int b = 0; b < 9; ++b) if (!local_closed(s, b)) return 0;
  return 3;
}
int side_to_move(const IState& s) {
  int x = 0, o = 0;
  for (auto c : s.cell) { x += c == 1; o += c == 2; }
  return x == o ? 1 : 2;
}
std::vector<int> legal_moves(const IState& s) {
  std::vector<int> out;
  if (terminal(s) != 0) return out;
  auto add = [&](int b) {
    if (local_closed(s, b)) return;
    for (int c = 0; c < 9; ++c) if (s.cell[b*9 + c] == 0) out.push_back(b*9 + c);
  };
  if (s.forced != -1) add(s.forced);
  else for (int b = 0; b < 9; ++b) add(b);
  return out;
}
IState apply(const IState& s, int move) {
  IState n = s;
  n.cell[move] = static_cast<std::uint8_t>(side_to_move(s));
  const int next_board = move % 9;
  n.forced = local_closed(n, next_board) ? -1 : next_board;
  return n;
}
int solve_value(const IState& s,
                std::unordered_map<IState, int, IStateHash>& memo,
                std::size_t budget) {
  const int t = terminal(s);
  if (t == 1) return 1;
  if (t == 2) return -1;
  if (t == 3) return 0;
  if (auto it = memo.find(s); it != memo.end()) return it->second;
  if (memo.size() > budget) return 99;
  const int mover = side_to_move(s);
  int best = mover == 1 ? -2 : 2;
  for (const int m : legal_moves(s)) {
    const int v = solve_value(apply(s, m), memo, budget);
    if (v == 99) return 99;
    if (mover == 1) { if (v > best) best = v; if (best == 1) break; }
    else { if (v < best) best = v; if (best == -1) break; }
  }
  memo.emplace(s, best);
  return best;
}
IState from_bytes(const std::array<std::uint8_t, 37>& b) {
  IState s;
  for (int board = 0; board < 9; ++board) {
    const std::uint16_t x = std::uint16_t(b[board*2]) | std::uint16_t(b[board*2+1]) << 8;
    const std::uint16_t o = std::uint16_t(b[18+board*2]) | std::uint16_t(b[18+board*2+1]) << 8;
    for (int c = 0; c < 9; ++c) {
      if ((x >> c) & 1u) s.cell[board*9 + c] = 1;
      if ((o >> c) & 1u) s.cell[board*9 + c] = 2;
    }
  }
  s.forced = b[36] == 9 ? -1 : b[36];
  return s;
}
}  // namespace indep

static int failures = 0;
#define CHECKP(cond, ...) do { if (!(cond)) { ++failures; \
  std::printf("FAIL: "); std::printf(__VA_ARGS__); std::printf("\n"); } } while (0)

int main() {
  namespace fs = std::filesystem;
  std::mt19937_64 rng(0xFACADE);
  std::vector<solver::ProvenFact> facts;
  std::size_t segments = 0;
  for (const auto& entry :
       fs::recursive_directory_iterator("/tmp/psa-probes/solve-stores")) {
    if (entry.path().filename() != "segment-0.psr") continue;
    ++segments;
    std::ifstream in(entry.path(), std::ios::binary);
    std::array<std::uint8_t, 48> bytes{};
    while (in.read(reinterpret_cast<char*>(bytes.data()), bytes.size())) {
      auto fact = solver::unpack(bytes);
      CHECKP(bool(fact), "segment %s holds an invalid record",
             entry.path().c_str());
      if (fact) facts.push_back(*fact);
    }
  }
  std::printf("loaded %zu facts from %zu segments\n", facts.size(), segments);
  std::shuffle(facts.begin(), facts.end(), rng);

  std::unordered_map<indep::IState, int, indep::IStateHash> memo;
  std::size_t validated = 0;
  std::size_t structural = 0;
  for (const auto& fact : facts) {
    // structural checks on every fact
    auto parsed = solver::parse(fact.state);
    CHECKP(bool(parsed), "fact state fails parse");
    CHECKP(solver::canon(fact.state).bytes == fact.state, "fact state not canonical");
    const indep::IState s = indep::from_bytes(fact.state);
    const int term = indep::terminal(s);
    const int claim_i = static_cast<int>(fact.claim);
    const int prover = (claim_i == 0 || claim_i == 2) ? 1 : 2;  // WinX/NolossX -> X
    const bool witness_required = term == 0 && prover == indep::side_to_move(s);
    CHECKP(witness_required == (fact.witness != solver::kNoWitness),
           "witness presence wrong (claim %d witness %u)", claim_i, fact.witness);
    if (fact.witness != solver::kNoWitness) {
      auto lm = indep::legal_moves(s);
      CHECKP(std::find(lm.begin(), lm.end(), int(fact.witness)) != lm.end(),
             "witness %u not a legal move", fact.witness);
    }
    ++structural;
    if (validated >= 400) continue;
    // semantic check on a sample: independently re-solve the state
    memo.clear();
    const int value = indep::solve_value(s, memo, 2'000'000);
    if (value == 99) continue;
    const bool truth[4] = {value == 1, value == -1, value >= 0, value <= 0};
    CHECKP(truth[claim_i], "recorded claim %d FALSE (independent value %d)",
           claim_i, value);
    if (fact.witness != solver::kNoWitness) {
      memo.clear();
      const int child_value =
          indep::solve_value(indep::apply(s, fact.witness), memo, 2'000'000);
      if (child_value != 99) {
        const bool child_truth[4] = {child_value == 1, child_value == -1,
                                     child_value >= 0, child_value <= 0};
        CHECKP(child_truth[claim_i],
               "witness child breaks claim %d (child value %d)", claim_i,
               child_value);
      }
    }
    ++validated;
  }
  std::printf("structural checks on %zu facts; semantic re-solve on %zu\n",
              structural, validated);
  if (failures == 0) std::printf("ALL WITNESS PROBES PASSED\n");
  else std::printf("%d WITNESS PROBE FAILURES\n", failures);
  return failures == 0 ? 0 : 1;
}
