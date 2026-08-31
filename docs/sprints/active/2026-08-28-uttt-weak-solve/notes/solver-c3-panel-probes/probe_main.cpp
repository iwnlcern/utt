// Adversarial proof-soundness probe for solver-c3.
// Part 1 is an INDEPENDENT re-implementation of the s2 rule lock, written
// from the ROADMAP text only (not from engine/ or solver/ sources).
// Part 2 cross-checks the solver worktree code against it.

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "canon/canon.hpp"
#include "core/state.hpp"
#include "search/kernel_a.hpp"
#include "search/kernel_b.hpp"
#include "search/tt.hpp"
#include "store/proven_store.hpp"

// ---------------------------------------------------------------------------
// Independent rules implementation (rule lock: 3x3 macro of 3x3 locals,
// X first, first move forced to center board, mark in cell c forces board c
// unless closed (then any open board), board closes when won or full, closed
// board cells dead, macro 3-in-row of won locals wins, all closed no macro
// winner = draw).
// ---------------------------------------------------------------------------
namespace indep {

struct IState {
  std::array<std::uint8_t, 81> cell{};  // 0 empty, 1 X, 2 O
  int forced = 4;                       // -1 = any, else 0..8
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

// who won local board b: 0 none, 1 X, 2 O
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

// terminal: 0 = none, 1 = X macro win, 2 = O macro win, 3 = draw
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

int side_to_move(const IState& s) {  // 1 = X, 2 = O
  int x = 0, o = 0;
  for (auto c : s.cell) { x += c == 1; o += c == 2; }
  return x == o ? 1 : 2;
}

std::vector<int> legal_moves(const IState& s) {  // move = b*9+c
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

IState root() { return IState{}; }

// exact game value from X's perspective: +1 X win, 0 draw, -1 O win
// returns 99 (abort) when the memo exceeds the budget
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

// map to/from the solver 37-byte identity
std::array<std::uint8_t, 37> to_bytes(const IState& s) {
  std::array<std::uint8_t, 37> b{};
  for (int board = 0; board < 9; ++board) {
    std::uint16_t x = 0, o = 0;
    for (int c = 0; c < 9; ++c) {
      if (s.cell[board*9 + c] == 1) x |= std::uint16_t(1u << c);
      if (s.cell[board*9 + c] == 2) o |= std::uint16_t(1u << c);
    }
    b[board*2] = std::uint8_t(x); b[board*2 + 1] = std::uint8_t(x >> 8);
    b[18 + board*2] = std::uint8_t(o); b[18 + board*2 + 1] = std::uint8_t(o >> 8);
  }
  b[36] = s.forced == -1 ? 9 : std::uint8_t(s.forced);
  return b;
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

// ---------------------------------------------------------------------------
// Cross-check harness
// ---------------------------------------------------------------------------
static int failures = 0;
#define CHECKP(cond, ...) do { if (!(cond)) { ++failures; \
  std::printf("FAIL %s:%d: ", __FILE__, __LINE__); std::printf(__VA_ARGS__); std::printf("\n"); } } while (0)

using Bytes = std::array<std::uint8_t, 37>;

static std::string hex(const Bytes& b) {
  static const char* d = "0123456789abcdef";
  std::string s;
  for (auto v : b) { s += d[v >> 4]; s += d[v & 15]; }
  return s;
}

// 1. joint perft + movegen bit-match to ply 7, distinct + canon census check
static void perft_and_census() {
  const std::uint64_t want_perft[8] = {0, 9, 80, 704, 6120, 52584, 446944, 3755040};
  std::vector<indep::IState> frontier{indep::root()};
  // also mirror through the solver's own state machine
  std::vector<solver::SolverState> sfrontier{solver::initial_root()};
  for (int ply = 1; ply <= 7; ++ply) {
    std::vector<indep::IState> next;
    std::vector<solver::SolverState> snext;
    std::unordered_set<std::string> distinct;
    std::unordered_set<std::string> canon_reps;
    for (std::size_t i = 0; i < frontier.size(); ++i) {
      const auto& s = frontier[i];
      const auto& ss = sfrontier[i];
      // movegen bit-match
      auto im = indep::legal_moves(s);
      auto sm = solver::legal_moves(ss);
      std::sort(im.begin(), im.end());
      std::vector<int> smv(sm.begin(), sm.end());
      std::sort(smv.begin(), smv.end());
      CHECKP(im == smv, "movegen mismatch at ply %d", ply);
      // terminal agreement of parents already implied (frontier is nonterminal
      // via alternating perft semantics: perft counts sequences, terminals not expanded)
      for (const int m : im) {
        auto child = indep::apply(s, m);
        auto schild_exp = solver::apply(ss, std::uint8_t(m));
        CHECKP(bool(schild_exp), "solver apply refused legal move ply %d", ply);
        const auto cb = indep::to_bytes(child);
        CHECKP(cb == solver::serialize(*schild_exp),
               "serialize mismatch after move ply %d: %s vs %s", ply,
               hex(cb).c_str(), hex(solver::serialize(*schild_exp)).c_str());
        const int it = indep::terminal(child);
        const auto st = solver::terminal(*schild_exp);
        const int stt = st == uttt::TerminalKind::None ? 0
                      : st == uttt::TerminalKind::MacroWinX ? 1
                      : st == uttt::TerminalKind::MacroWinO ? 2 : 3;
        CHECKP(it == stt, "terminal mismatch ply %d state %s (indep %d solver %d)",
               ply, hex(cb).c_str(), it, stt);
        distinct.insert(std::string(cb.begin(), cb.end()));
        const auto rep = solver::canon(cb).bytes;
        canon_reps.insert(std::string(rep.begin(), rep.end()));
        if (it == 0) { next.push_back(child); snext.push_back(*schild_exp); }
      }
    }
    // alternating perft: total sequences of length ply
    std::uint64_t seqs = 0;
    // recount: sequences = expanded children this level (alive + terminal)
    // (we pushed only alive; recompute by summing move counts)
    for (std::size_t i = 0; i < frontier.size(); ++i) {
      seqs += indep::legal_moves(frontier[i]).size();
    }
    CHECKP(seqs == want_perft[ply], "perft(%d) = %llu want %llu", ply,
           (unsigned long long)seqs, (unsigned long long)want_perft[ply]);
    std::printf("ply %d: perft=%llu distinct=%zu canon=%zu\n", ply,
                (unsigned long long)seqs, distinct.size(), canon_reps.size());
    if (ply == 4) CHECKP(distinct.size() == 6092, "ply4 distinct %zu want 6092", distinct.size());
    if (ply == 7) {
      CHECKP(distinct.size() == 3447584, "ply7 distinct %zu want 3447584", distinct.size());
      CHECKP(canon_reps.size() == 431167, "ply7 canon %zu want 431167", canon_reps.size());
    }
    frontier = std::move(next);
    sfrontier = std::move(snext);
  }
}

// 2. D4 equivariance on random playout states: transforms preserve rules,
//    canon is orbit-invariant and idempotent, move maps commute with apply.
static void equivariance() {
  std::mt19937_64 rng(0xC0FFEE);
  int states_checked = 0;
  for (int game = 0; game < 4000; ++game) {
    indep::IState s = indep::root();
    while (indep::terminal(s) == 0) {
      const Bytes raw = indep::to_bytes(s);
      // canon orbit invariance + idempotence + fixed-point membership
      const auto canonical = solver::canon(raw);
      const auto again = solver::canon(canonical.bytes);
      CHECKP(again.bytes == canonical.bytes, "canon not idempotent %s", hex(raw).c_str());
      CHECKP(canonical.bytes == solver::transform(raw, canonical.transform),
             "canon.transform does not reproduce canon bytes %s", hex(raw).c_str());
      bool rep_in_orbit = false;
      for (std::uint8_t t = 0; t < 8; ++t) {
        const Bytes tb = solver::transform(raw, t);
        if (tb == canonical.bytes) rep_in_orbit = true;
        CHECKP(solver::canon(tb).bytes == canonical.bytes,
               "canon not orbit invariant t=%u %s", t, hex(raw).c_str());
        CHECKP(canonical.bytes <= tb, "canon not lexmin t=%u %s", t, hex(raw).c_str());
        // transformed state must be a valid state with identical semantics
        const indep::IState ts = indep::from_bytes(tb);
        CHECKP(indep::terminal(ts) == indep::terminal(s),
               "terminal changes under transform t=%u %s", t, hex(raw).c_str());
        CHECKP(indep::side_to_move(ts) == indep::side_to_move(s),
               "side changes under transform t=%u", t);
        auto sp = solver::parse(tb);
        CHECKP(bool(sp), "transformed state fails solver parse t=%u %s", t, hex(raw).c_str());
        // legal move set maps bijectively
        auto lm = indep::legal_moves(s);
        std::set<int> mapped;
        for (int m : lm) mapped.insert(solver::map_move_to_canonical(std::uint8_t(m), t));
        auto tlm = indep::legal_moves(ts);
        std::set<int> tset(tlm.begin(), tlm.end());
        CHECKP(mapped == tset, "legal move set not equivariant t=%u %s", t, hex(raw).c_str());
        // apply commutes on one random legal move
        if (!lm.empty()) {
          const int m = lm[rng() % lm.size()];
          const std::uint8_t tm = solver::map_move_to_canonical(std::uint8_t(m), t);
          const Bytes lhs = solver::transform(indep::to_bytes(indep::apply(s, m)), t);
          const Bytes rhs = indep::to_bytes(indep::apply(ts, tm));
          CHECKP(lhs == rhs, "apply does not commute with transform t=%u %s m=%d", t,
                 hex(raw).c_str(), m);
          // round trip through inverse
          CHECKP(solver::map_move_from_canonical(tm, t) == m, "move map round trip t=%u", t);
        }
      }
      CHECKP(rep_in_orbit, "canonical bytes not a member of the orbit %s", hex(raw).c_str());
      ++states_checked;
      auto lm = indep::legal_moves(s);
      s = indep::apply(s, lm[rng() % lm.size()]);
    }
  }
  std::printf("equivariance: %d states x 8 transforms checked\n", states_checked);
}

// 3. Kernel A and B cross-check against the independent solver on random
//    deep positions, including under an adversarial always-colliding TT tag.
static void kernel_cross_check() {
  namespace fs = std::filesystem;
  std::mt19937_64 rng(0xDEADBEEF);
  std::unordered_map<indep::IState, int, indep::IStateHash> memo;
  int checked = 0;
  int store_id = 0;
  for (int trial = 0; trial < 4000 && checked < 60; ++trial) {
    // random playout to a target ply
    const int target = 28 + int(rng() % 27);  // ply 28..54
    indep::IState s = indep::root();
    bool ok = true;
    for (int ply = 0; ply < target; ++ply) {
      auto lm = indep::legal_moves(s);
      if (lm.empty()) { ok = false; break; }
      s = indep::apply(s, lm[rng() % lm.size()]);
    }
    if (!ok || indep::terminal(s) != 0) continue;
    // guard: only solve if the independent subgame is small enough
    memo.clear();
    const int value = indep::solve_value(s, memo, 3'000'000);
    if (value == 99) continue;  // subgame too large for the budget; skip
    const bool truth[4] = {value == 1, value == -1, value >= 0, value <= 0};

    const Bytes raw = indep::to_bytes(s);
    auto parsed = solver::parse(raw);
    CHECKP(bool(parsed), "deep state fails parse");
    if (!parsed) continue;

    for (int mode = 0; mode < 2; ++mode) {  // 0 = normal tag, 1 = colliding tag
      for (int claim_i = 0; claim_i < 4; ++claim_i) {
        const auto claim = static_cast<solver::Claim>(claim_i);
        for (int kernel_i = 0; kernel_i < 2; ++kernel_i) {
          fs::path dir = fs::path("/tmp/psa-probes/store-scratch") /
                         ("s" + std::to_string(store_id++));
          fs::create_directories(dir);
          solver::ProvenStore store(dir, 1);
          solver::WorkingTT tt(
              256, mode == 0 ? solver::WorkingTT::Tagger{}
                             : solver::WorkingTT::Tagger{[](std::span<const std::uint8_t, 37>) {
                                 return std::uint64_t{42};
                               }});
          solver::SearchCtx ctx{store, tt, 0};
          solver::SolveOutcome out;
          if (kernel_i == 0) {
            solver::KernelB kb;
            out = kb.solve(claim, *parsed, ctx, solver::Limits{});
          } else {
            solver::KernelA ka;
            out = ka.solve(claim, *parsed, ctx, solver::Limits{});
          }
          CHECKP(out.complete, "kernel %c incomplete without cap (claim %d)",
                 kernel_i ? 'A' : 'B', claim_i);
          const bool proven = out.truth == solver::Truth::Proven;
          CHECKP(proven == truth[claim_i],
                 "kernel %c mode %d claim %d: got %s, independent value %d, state %s",
                 kernel_i ? 'A' : 'B', mode, claim_i,
                 proven ? "Proven" : "Disproven", value, hex(raw).c_str());
        }
      }
    }
    ++checked;
    std::printf("  pos %d ply %d value %d ok (memo %zu)\n", checked, target, value,
                memo.size());
  }
  std::printf("kernel cross-check: %d deep positions x 4 claims x 2 kernels x 2 tag modes\n",
              checked);
}

int main(int argc, char** argv) {
  std::setvbuf(stdout, nullptr, _IONBF, 0);
  const std::string stage = argc > 1 ? argv[1] : "all";
  if (stage == "all" || stage == "perft") perft_and_census();
  if (stage == "all" || stage == "equi") equivariance();
  if (stage == "all" || stage == "kernel") kernel_cross_check();
  if (failures == 0) std::printf("ALL PROBES PASSED\n");
  else std::printf("%d PROBE FAILURES\n", failures);
  return failures == 0 ? 0 : 1;
}
