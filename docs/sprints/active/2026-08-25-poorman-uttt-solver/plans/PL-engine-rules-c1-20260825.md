# Engine Rules Core Implementation Plan (PL-engine-rules-c1-20260825)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement the C++26 rules core, local tables, Zobrist keying, test seam, benchmark harness, and JSONL protocol adapter locked by DD-engine-rules-c1-20260825 — no search math.

**Architecture:** Value-copy immutable `Position` (per-player `array<uint16_t,9>` + caches) over a 19,683-entry packed semantic local table; naive reference implementations exist beside the optimized path for property tests, the exhaustive table proof, and the benchmark baseline; a JSON-free core behind a thin adapter that consumes harness protocol v1 with a placeholder policy until the theory-gated search DD lands.

**Tech Stack:** C++26 (`-std=c++2c`), Homebrew LLVM 22.1.8, CMake ≥ 3.28 + Ninja, doctest (vendored single header, MIT), nlohmann/json (vendored single header, MIT, adapter-only).

**Spec:** docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-rules-c1-20260825.md @ sha256 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47 (locked; approving review engine-c1-design-review-4).
Consumed owner contracts: harness protocol v1 = DD-harness-c1-20260825 @ 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440; theory fixture schema v1 = DD-theory-c1-20260825 §3 (`theory/fixtures/SCHEMA.md`, `schema-v1.json`).

## Global Constraints

- Compiler: Homebrew LLVM 22.1.8 (`/opt/homebrew/opt/llvm/bin/clang++`), `-std=c++2c`; feature-tested library facilities only (`std::simd` and `<inplace_vector>` are absent — do not include them).
- The search core (everything under `engine/src/core/`) has zero JSON and zero I/O dependencies; JSON appears only under `engine/src/adapter/`.
- No search math: no backup operator, no cutoff/bound math, no TT, no widening. `zobrist` keying IS in scope (it keys state identity, not values).
- License gate: nelhage/ultimattt and TheGustafson/ai-ultimate-tictactoe are reference-study only; copying code from them is prohibited.
- Vendored deps are pinned single headers with their LICENSE text committed beside them; both chosen libs are MIT (verify the license file content on vendoring; abort and escalate if not MIT).
- stdout of the engine binary carries protocol lines only; all diagnostics go to stderr; malformed requests produce no stdout line.
- Rules follow the design spec canonical rules AS LOCKED in the DD; if any task step seems to contradict the DD, the DD wins and the conflict is a blocker relay, not a local fix.
- Long .md files put full sentences on their own lines.
- Commits: small, per task step as marked; never claim a fix without the named test output.
- Branch: create `engine/rules-core-c1` from `main` at IMPL start; do not commit to `main`.

## File Structure

```
engine/
  CMakeLists.txt                     # targets: uttt_core (lib), uttt_tests, uttt_bench, uttt_engine (adapter binary)
  third_party/doctest/doctest.h      # vendored, MIT, pinned version noted in CMake comment
  third_party/doctest/LICENSE.txt
  third_party/nlohmann/json.hpp      # vendored, MIT, pinned version noted in CMake comment (adapter target only)
  third_party/nlohmann/LICENSE.MIT
  src/core/types.hpp                 # Seat, TieState, LocalStatus, TerminalKind, ApplyError, Move, MoveList
  src/core/local_table.hpp           # packed-entry accessors + table access
  src/core/local_table.cpp           # table construction (built once at startup)
  src/core/naive_local.hpp           # independent naive local-board evaluator + reachability oracle (header-only)
  src/core/position.hpp              # Position, initial(), terminal(), legal_moves(), applied()
  src/core/position.cpp
  src/core/zobrist.hpp               # zobrist tables + full recompute
  src/core/zobrist.cpp
  src/core/naive_position.hpp        # naive 81-cell reference movegen + naive apply (header-only, used by tests and bench reference path)
  src/adapter/wire.hpp               # TurnRequest/TurnReply/Hello structs + parse/validate/serialize (nlohmann inside .cpp only)
  src/adapter/wire.cpp
  src/adapter/policy.hpp             # Policy interface + PlaceholderPolicy
  src/adapter/main.cpp               # uttt_engine: hello/turn/game_end loop
  tests/test_naive_local.cpp
  tests/test_local_table.cpp
  tests/test_position.cpp
  tests/test_lifecycle.cpp
  tests/test_zobrist.cpp
  tests/test_perft.cpp
  tests/test_fixtures.cpp
  tests/test_wire.cpp
  tests/test_engine_e2e.py           # driven by ctest via python3; spawns the binary
  bench/bench_playout.cpp            # --reference | --candidate --baseline <path>; emits/checks bench/baseline.json
  bench/README.md                    # methodology: warmup, runs, flags, cpu capture
```

Coordinate conventions everywhere: local boards 0–8 row-major across the macro grid; cells 0–8 row-major within a local board; matches the harness wire and theory fixtures.

---

### Task 1: Scaffold and toolchain lock

**Files:**
- Create: `engine/CMakeLists.txt`
- Create: `engine/third_party/doctest/doctest.h`, `engine/third_party/doctest/LICENSE.txt`
- Create: `engine/tests/test_smoke.cpp` (deleted in Task 2 when real tests exist — keep if you prefer; it is one assertion)

**Interfaces:**
- Produces: build commands used by every later task: `cmake -S engine -B engine/build -G Ninja -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ -DCMAKE_BUILD_TYPE=RelWithDebInfo` then `ninja -C engine/build` then `ctest --test-dir engine/build --output-on-failure`.

- [ ] **Step 1: Vendor doctest**

Download the latest 2.4.x single header from the doctest GitHub release, save as `engine/third_party/doctest/doctest.h`, save its LICENSE.txt beside it, and confirm the license text is MIT (first line of LICENSE.txt).
Record the version in a one-line comment at the top of CMakeLists.

- [ ] **Step 2: Write CMakeLists**

```cmake
cmake_minimum_required(VERSION 3.28)
project(uttt_engine CXX)
# doctest 2.4.x vendored; nlohmann/json vendored in Task 10.
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-std=c++2c -Wall -Wextra -Werror)

add_library(uttt_core
  src/core/local_table.cpp
  src/core/position.cpp
  src/core/zobrist.cpp)
target_include_directories(uttt_core PUBLIC src)

add_executable(uttt_tests
  tests/test_smoke.cpp)
target_link_libraries(uttt_tests PRIVATE uttt_core)
target_include_directories(uttt_tests PRIVATE third_party)

enable_testing()
add_test(NAME unit COMMAND uttt_tests)
```

Until Task 2 adds real sources, create empty placeholder .cpp files for the library sources listed (each containing only a namespace comment) so the target links; they are filled by their tasks.

- [ ] **Step 3: Write the smoke test**

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
TEST_CASE("toolchain sanity") {
  static_assert(__cplusplus >= 202400L);
  CHECK(1 + 1 == 2);
}
```

- [ ] **Step 4: Build and run**

Run the three build commands from Interfaces above.
Expected: configure, build, and `ctest` all succeed with 1 passing test.

- [ ] **Step 5: Commit**

```bash
git add engine/
git commit -m "engine: scaffold CMake + Ninja + Homebrew LLVM c++2c toolchain, vendor doctest (MIT)"
```

---

### Task 2: Core types

**Files:**
- Create: `engine/src/core/types.hpp`
- Create: `engine/tests/test_types.cpp` (add to `uttt_tests` sources; do the same for every later test file)

**Interfaces:**
- Produces (exact, used by every later task):

```c++
#pragma once
#include <array>
#include <cstdint>
namespace uttt {
enum class Seat : uint8_t { X = 0, O = 1 };
constexpr Seat opponent(Seat s) { return s == Seat::X ? Seat::O : Seat::X; }
enum class TieState : uint8_t { X = 0, O = 1, NullFirstMove = 2 };
enum class LocalStatus : uint8_t { Open = 0, XWon = 1, OWon = 2, FullDraw = 3 };
enum class TerminalKind : uint8_t { None, MacroWinX, MacroWinO, AllClosed };
enum class ApplyError : uint8_t { TerminalParent, CellOccupied, BoardClosed, WrongForcedBoard, OutOfRange };
struct Move { uint8_t board; uint8_t cell; bool operator==(const Move&) const = default; };
struct MoveList { std::array<Move, 81> m; uint8_t n = 0; void push(Move mv) { m[n++] = mv; } };
inline constexpr int8_t kForcedAny = -1;
inline constexpr std::array<uint16_t, 8> kWinLines = {
    0b111000000, 0b000111000, 0b000000111,
    0b100100100, 0b010010010, 0b001001001,
    0b100010001, 0b001010100};
}  // namespace uttt
```

- [ ] **Step 1: Write failing test**

```c++
#include "doctest/doctest.h"
#include "core/types.hpp"
using namespace uttt;
TEST_CASE("types basics") {
  CHECK(opponent(Seat::X) == Seat::O);
  CHECK(opponent(Seat::O) == Seat::X);
  MoveList ml;
  ml.push({4, 2});
  CHECK(ml.n == 1);
  CHECK(ml.m[0] == Move{4, 2});
  CHECK(kWinLines.size() == 8);
}
```

- [ ] **Step 2: Run to verify it fails** (compile error: header absent). **Step 3: Add the header exactly as in Interfaces.** **Step 4: Build + ctest, expect pass.** **Step 5: Commit** `git commit -m "engine: core types"`.

---

### Task 3: Naive local evaluator and reachability oracle

The INDEPENDENT implementation the exhaustive table proof compares against (DD §3/§7).
Written FIRST and from the rules text only — do not consult `local_table.cpp` while writing it.

**Files:**
- Create: `engine/src/core/naive_local.hpp`
- Create: `engine/tests/test_naive_local.cpp`

**Interfaces:**
- Produces:

```c++
namespace uttt::naive {
struct LocalEval {
  bool valid;          // locally reachable per DD §3 rule
  LocalStatus status;
  uint16_t empties, win_x, win_o, fork_x, fork_o;
};
// x, o: 9-bit masks, must be disjoint (callers guarantee; assert).
LocalEval eval_local(uint16_t x, uint16_t o);
}
```

- Semantics (implement literally):
  - `completed(m)`: any of `kWinLines` fully inside mask m.
  - `valid`: if neither side has a completed line → true. If both sides have completed lines → false. If exactly one side (say mask w) has completed lines → true iff ∃ bit c ∈ w such that c lies on EVERY completed line of w and `completed(w & ~bit(c))` is false.
  - `status`: XWon/OWon if that side has a completed line (valid or not — invalid entries still get the FullDraw convention below); for INVALID entries force `status = FullDraw`, masks zeroed, per DD §3 defined-bytes rule. For valid: XWon/OWon on a completed line, FullDraw if all 9 cells filled with no line, else Open.
  - `empties`: `~(x|o) & 0x1FF` for valid open boards; 0 for closed or invalid.
  - `win_p` (only for valid Open boards; else 0): bits c ∈ empties where `completed(p | bit(c))`.
  - `fork_p` (only for valid Open boards; else 0): bits c ∈ empties where NOT `completed(p | bit(c))` AND the board after placing p at c has ≥ 2 distinct immediate-win cells for p.

- [ ] **Step 1: Write failing tests — every class from the DD**

```c++
#include "doctest/doctest.h"
#include "core/naive_local.hpp"
using namespace uttt;
static uint16_t rowmask(int r) { return uint16_t(0b111 << (3 * (2 - r))); }
TEST_CASE("naive local eval") {
  auto empty = naive::eval_local(0, 0);
  CHECK(empty.valid); CHECK(empty.status == LocalStatus::Open);
  CHECK(empty.empties == 0x1FF); CHECK(empty.win_x == 0);

  // single-line win: valid
  auto onewin = naive::eval_local(rowmask(0), 0b000000011);
  CHECK(onewin.valid); CHECK(onewin.status == LocalStatus::XWon);
  CHECK(onewin.empties == 0);

  // dual-winner: invalid (MR1 class 1)
  auto dual = naive::eval_local(rowmask(0), rowmask(1));
  CHECK_FALSE(dual.valid); CHECK(dual.status == LocalStatus::FullDraw);
  CHECK(dual.empties == 0); CHECK(dual.win_x == 0); CHECK(dual.fork_o == 0);

  // same-player disjoint lines: invalid (MR1 class 2, XXX/XXX/...)
  auto disjoint = naive::eval_local(uint16_t(rowmask(0) | rowmask(1)), 0);
  CHECK_FALSE(disjoint.valid);

  // same-player two lines sharing the closing mark: VALID
  // X on row0 + col0 (cells r0c0,r0c1,r0c2,r1c0,r2c0), corner r0c0 on both lines,
  // removing it kills both.
  uint16_t two = uint16_t(rowmask(0) | 0b100100100);
  auto shared = naive::eval_local(two, 0);
  CHECK(shared.valid); CHECK(shared.status == LocalStatus::XWon);

  // immediate win mask: X holds r0c0,r0c1 -> r0c2 completes
  auto near = naive::eval_local(0b110000000, 0);
  CHECK((near.win_x & 0b001000000) != 0);

  // fork: X at two corners r0c0,r2c2 with center free — center creates
  // the two diagonals? center completes nothing yet; after center X has
  // main-diag threat needing r0c0..: hand-check a known fork instead:
  // X at r0c0 and r0c2 and r2c0: playing center (r1c1) yields diag+col threats >= 2.
  auto forky = naive::eval_local(0b101000100, 0);
  CHECK((forky.fork_x & 0b000010000) != 0);

  // full draw
  auto draw = naive::eval_local(0b101110010, 0b010001101);
  CHECK(draw.valid); CHECK(draw.status == LocalStatus::FullDraw);
}
```

Verify the fork/draw literals by hand against the bit convention (bit 8 = r0c0 … bit 0 = r2c2 or your chosen order) BEFORE running — pick ONE bit order (recommended: bit index = 3*row + col, i.e. bit 0 = r0c0 … bit 8 = r2c2) and rewrite the literals above consistently with it; the values shown assume MSB-first and MUST be adapted to the chosen order.
The chosen bit order is part of the produced interface: document it in a comment in `types.hpp` and use it in every later task and in fixture/wire conversion.

- [ ] **Step 2: Run to verify failure.** **Step 3: Implement `eval_local` per the semantics block.** **Step 4: Build + ctest, expect pass.** **Step 5: Commit** `"engine: naive local evaluator + MR1 reachability oracle"`.

---

### Task 4: LocalTable — packed entries + exhaustive proof

**Files:**
- Create: `engine/src/core/local_table.hpp`, fill `engine/src/core/local_table.cpp`
- Create: `engine/tests/test_local_table.cpp`

**Interfaces:**
- Produces:

```c++
namespace uttt {
// Packed entry (DD §3, 48 bits): bit 0 valid; bits 1-2 status;
// bits 3-11 empties; 12-20 win_x; 21-29 win_o; 30-38 fork_x; 39-47 fork_o.
class LocalTable {
 public:
  static const LocalTable& instance();          // built once, thread-safe static
  bool valid(uint32_t code) const;
  LocalStatus status(uint32_t code) const;
  uint16_t empties(uint32_t code) const;
  uint16_t win(uint32_t code, Seat s) const;
  uint16_t fork(uint32_t code, Seat s) const;
 private:
  std::array<uint64_t, 19683> e_;
};
// ternary code helpers (cell c contributes {empty:0, X:1, O:2} * 3^c)
uint32_t ternary_code(uint16_t x, uint16_t o);
inline constexpr std::array<uint32_t, 9> kPow3 = {1,3,9,27,81,243,729,2187,6561};
}
```

- Construction: for each of the 19,683 codes decode (x, o) masks, call a PRIVATE re-derivation (not `naive::eval_local` — the table builder derives independently from `kWinLines`; the naive header is the test-side oracle) and pack.

- [ ] **Step 1: Write failing tests**

```c++
TEST_CASE("exhaustive table proof vs naive oracle") {
  const auto& t = LocalTable::instance();
  for (uint32_t code = 0; code < 19683; ++code) {
    uint16_t x = 0, o = 0; uint32_t tmp = code;
    for (int c = 0; c < 9; ++c) { auto v = tmp % 3; tmp /= 3;
      if (v == 1) x |= uint16_t(1u << c); if (v == 2) o |= uint16_t(1u << c); }
    auto n = naive::eval_local(x, o);
    CAPTURE(code);
    REQUIRE(t.valid(code) == n.valid);
    REQUIRE(t.status(code) == n.status);
    REQUIRE(t.empties(code) == n.empties);
    REQUIRE(t.win(code, Seat::X) == n.win_x);
    REQUIRE(t.win(code, Seat::O) == n.win_o);
    REQUIRE(t.fork(code, Seat::X) == n.fork_x);
    REQUIRE(t.fork(code, Seat::O) == n.fork_o);
  }
}
TEST_CASE("ternary code round-trip") {
  CHECK(ternary_code(0, 0) == 0);
  CHECK(ternary_code(1, 0) == 1);   // X at cell 0
  CHECK(ternary_code(0, 1) == 2);   // O at cell 0
  CHECK(ternary_code(0b10, 0b01) == 2 + 3);
}
```

- [ ] **Step 2: Run to verify failure.** **Step 3: Implement table + packing.** **Step 4: Build + ctest (the exhaustive case runs in well under a second).** **Step 5: Commit** `"engine: 19683-entry packed local table + exhaustive naive-oracle proof"`.

---

### Task 5: Position — initial state, apply, movegen, terminals

**Files:**
- Create: `engine/src/core/position.hpp`, fill `engine/src/core/position.cpp`
- Create: `engine/tests/test_position.cpp`

**Interfaces:**
- Produces:

```c++
namespace uttt {
struct Position {
  std::array<uint16_t, 9> x{}, o{};
  std::array<uint32_t, 9> tern{};
  uint16_t macro_x = 0, macro_o = 0, closed = 0;
  int8_t forced = 4;                       // 0..8 or kForcedAny
  TieState tie = TieState::NullFirstMove;
  uint64_t key = 0;                        // incrementally maintained Zobrist (Task 7 wires it)

  static Position initial();               // forced=4, tie=NullFirstMove, key = zobrist_full(*this)
  TerminalKind terminal() const;           // derived from macro_x/macro_o/closed
  void legal_moves(MoveList& out) const;   // EMPTY at any terminal (DD §4)
  std::expected<Position, ApplyError> applied(Move mv, Seat mover) const;
  bool identity_equal(const Position& p) const;  // fieldwise x,o,forced,tie ONLY (DD §5)
};
}
```

- `applied` order (DD §4): validate (terminal parent → `TerminalParent`; range; forced match — a move into board b when `forced != kForcedAny && forced != b` → `WrongForcedBoard`; closed board → `BoardClosed`; occupied cell → `CellOccupied`), then on a copy: set cell mask; update `tern[b]`; table lookup; on close update `closed`/`macro_*`; mover macro-win check via `kWinLines` over the mover's macro mask; all-closed check; `forced = closed-bit(mv.cell) ? kForcedAny : mv.cell`; `tie = opponent(mover)` mapped into TieState; update `key` incrementally (Task 7 adds this line; until then key stays 0).
- `terminal()`: MacroWin{X,O} if the macro mask holds a line (check both; construction makes dual impossible), else AllClosed if `closed == 0x1FF`, else None.

- [ ] **Step 1: Write failing tests**

```c++
TEST_CASE("initial state") {
  auto p = Position::initial();
  CHECK(p.forced == 4);
  CHECK(p.tie == TieState::NullFirstMove);
  MoveList ml; p.legal_moves(ml);
  CHECK(ml.n == 9);
  for (int i = 0; i < ml.n; ++i) CHECK(ml.m[i].board == 4);
}
TEST_CASE("apply validation") {
  auto p = Position::initial();
  CHECK(p.applied({3, 0}, Seat::X).error() == ApplyError::WrongForcedBoard);
  CHECK(p.applied({4, 9}, Seat::X).error() == ApplyError::OutOfRange);
  auto q = p.applied({4, 4}, Seat::X).value();
  CHECK(q.applied({4, 4}, Seat::O).error() == ApplyError::CellOccupied);
  CHECK(q.forced == 4);                      // center cell -> board 4 (open)
  CHECK(q.tie == TieState::O);               // opponent(actual mover)
  CHECK(p.tie == TieState::NullFirstMove);   // parent untouched
}
TEST_CASE("closure, routing, closed-board ANY") {
  // X wins local 4 with cells 0,1,2 (row) while sending O around;
  // build by alternating forced-legal moves; assert closed bit 4, macro_x bit 4,
  // then a move TARGETING cell 4 (closed board) yields forced == kForcedAny.
  auto p = Position::initial();
  p = p.applied({4, 0}, Seat::X).value();   // forced -> board 0
  p = p.applied({0, 4}, Seat::O).value();   // forced -> board 4
  p = p.applied({4, 1}, Seat::X).value();   // forced -> board 1
  p = p.applied({1, 4}, Seat::O).value();   // forced -> board 4
  p = p.applied({4, 2}, Seat::X).value();   // completes row in board 4 -> closes it
  CHECK((p.closed >> 4) & 1);
  CHECK((p.macro_x >> 4) & 1);
  CHECK(p.forced == 2);                      // cell 2 -> board 2 (open)
  p = p.applied({2, 4}, Seat::O).value();    // targets closed board 4
  CHECK(p.forced == kForcedAny);
  MoveList ml; p.legal_moves(ml);
  for (int i = 0; i < ml.n; ++i) CHECK(((p.closed >> ml.m[i].board) & 1) == 0);
}
TEST_CASE("terminal totality (M2)") {
  // Fabricate a macro win via legal play on three local boards
  // (win locals 0,1,2 for X using the routing dance above per board),
  // then: terminal() == MacroWinX, legal_moves empty, applied -> TerminalParent.
  // Build helper make_macro_win_x() inside the test file that performs
  // the scripted legal sequence and returns the terminal Position.
  auto t = make_macro_win_x();
  CHECK(t.terminal() == TerminalKind::MacroWinX);
  MoveList ml; t.legal_moves(ml);
  CHECK(ml.n == 0);
  CHECK(t.applied({8, 8}, Seat::O).error() == ApplyError::TerminalParent);
}
```

`make_macro_win_x()` is written in the test file as an explicit scripted sequence of `applied` calls (every move legal under routing; same dance as the closure test repeated for locals 0, 1, 2 — the writer works the routing out once and asserts value() at each step).
Note the row convention: with bit order 3*row+col, cells {0,1,2} are row 0.

- [ ] **Step 2: Run to verify failure.** **Step 3: Implement.** **Step 4: Build + ctest, expect pass.** **Step 5: Commit** `"engine: Position apply/movegen/terminals with total terminal legality"`.

---

### Task 6: Naive reference position + randomized property tests

**Files:**
- Create: `engine/src/core/naive_position.hpp`
- Create: `engine/tests/test_lifecycle.cpp`

**Interfaces:**
- Produces:

```c++
namespace uttt::naive {
// Naive 81-cell reference: recomputes everything from scratch each call.
// Independent implementation: derives closure/status from eval_local (Task 3),
// scans all cells for movegen, no caches. Same Move/Seat types.
struct RefPosition {
  std::array<Cell81, 1> board;  // implement as std::array<uint8_t, 81>: 0 empty, 1 X, 2 O
  int8_t forced; TieState tie;
  static RefPosition initial();
  TerminalKind terminal() const;
  void legal_moves(MoveList& out) const;
  RefPosition applied_unchecked(Move mv, Seat mover) const;  // caller passes legal moves only
};
uint64_t playout_ns_probe(...);  // NOT here — bench owns timing; this header stays timing-free
}
```

(Adjust the struct literally: `std::array<uint8_t, 81> cells;` with index `9*board+cell`; drop the `Cell81` placeholder shown above.)

- [ ] **Step 1: Write failing property tests**

```c++
// xorshift64 with fixed seed; play N=2000 random full games; at every ply:
//   REQUIRE optimized legal_moves == naive legal_moves (as sets)
//   REQUIRE optimized terminal() == naive terminal()
//   REQUIRE tern[b] == recomputed ternary_code(x[b], o[b]) for all b
//   REQUIRE macro/closed caches == recomputation from local statuses
//   REQUIRE parent identity fields unchanged after applied (immutability)
//   REQUIRE tie == opponent(last mover) at every step and NullFirstMove only at ply 0
// Random mover each ply (either seat may move consecutively — bidding reality).
```

Write the loop concretely: collect both move lists, sort by (board, cell), compare; on mismatch `CAPTURE(ply, seed)`.

- [ ] **Step 2: Run to verify failure** (naive header absent). **Step 3: Implement `naive_position.hpp`.** **Step 4: Build + ctest.** **Step 5: Commit** `"engine: naive reference position + randomized cross-implementation property tests"`.

---

### Task 7: Zobrist keying

**Files:**
- Create: `engine/src/core/zobrist.hpp`, fill `engine/src/core/zobrist.cpp`
- Modify: `engine/src/core/position.cpp` (initial() and applied() maintain `key`)
- Create: `engine/tests/test_zobrist.cpp`

**Interfaces:**
- Produces:

```c++
namespace uttt {
// 175 randoms: cell[81][2 seats], forced[10] (0..8 + ANY), tie[3].
// Generated at build time from a FIXED seed (splitmix64 from 0x9E3779B97F4A7C15)
// so keys are stable across runs and platforms.
struct ZobristTables { /* arrays as above */ static const ZobristTables& instance(); };
uint64_t zobrist_full(const Position& p);   // recompute from identity fields
// 32-bit secondary tag: independent fold, e.g. splitmix64(key) >> 32 — used by the
// future TT (successor DD); expose now as: uint32_t zobrist_tag(uint64_t key);
}
```

- `applied()` updates `key`: XOR out old forced and tie randoms, XOR in the placed cell random, XOR in new forced and tie randoms.

- [ ] **Step 1: Write failing tests**

```c++
TEST_CASE("incremental key == full recompute along random games") { /* reuse Task 6 loop; REQUIRE p.key == zobrist_full(p) each ply */ }
TEST_CASE("key sensitivity: every input changes the key") {
  auto p = Position::initial();
  auto q = p.applied({4, 4}, Seat::X).value();
  CHECK(p.key != q.key);                             // cell + tie + forced moved
  // forced-only difference: construct two positions equal in cells/tie differing
  // only in forced (reach the same cell set via different move orders per Task 5's
  // routing dance) and CHECK different keys; likewise tie-only via mover swap.
  // X-mark vs O-mark on the same cell:
  auto r = p.applied({4, 4}, Seat::O).value();
  CHECK(q.key != r.key);
}
```

The forced-only / tie-only constructions are scripted explicitly in the test (two short move sequences each; the writer verifies the intended field diff with REQUIRE on the fields before comparing keys).

- [ ] **Step 2: Run to verify failure.** **Step 3: Implement tables + full recompute + incremental maintenance.** **Step 4: Build + ctest.** **Step 5: Commit** `"engine: stable-seed Zobrist keying, incremental == recompute, per-input sensitivity"`.

---

### Task 8: Both-mover perft with pinned counts

**Files:**
- Create: `engine/tests/test_perft.cpp`

**Interfaces:**
- Consumes: `Position`, `MoveList`.
- Produces: `uint64_t perft(const Position& p, int depth)` (test-local function): if `depth == 0` or terminal → 1; else sum over every legal move mv of `perft(applied(mv, X)) + perft(applied(mv, O))` — DD §7's both-mover expansion.

- [ ] **Step 1: Write the perft function and a shape test**

```c++
TEST_CASE("perft shape") {
  auto p = Position::initial();
  CHECK(perft(p, 1) == 18);   // 9 legal cells x 2 movers
}
```

- [ ] **Step 2: Run depths 2–4 once, print counts, PIN them**

Run a temporary `MESSAGE`-style print of `perft(initial, d)` for d = 2, 3, 4.
Copy the printed numbers into `CHECK(perft(p, 2) == <printed>)` etc., delete the print.
These are regression pins (they detect movegen/apply drift), not external truths; the comment in the test says exactly that.

- [ ] **Step 3: Build + ctest (keep total perft runtime under ~5 s; drop to depth 3 if 4 exceeds it).** **Step 4: Commit** `"engine: both-mover perft regression pins"`.

---

### Task 9: Theory fixture ingestion

**Files:**
- Create: `engine/tests/test_fixtures.cpp`

**Interfaces:**
- Consumes: theory fixture schema v1 (DD-theory-c1 §3): files under `theory/fixtures/*.json`, shape `{"schema_version": 1, "game": "uttt" | "ttt3", "fixtures": [...]}`; categories consumed NOW: legality (`expected_legal_moves`), closure/routing (`move` + expected closure set + next `forced` + terminal flag), terminal (expected result + chip margin — engine checks board-side facts only; chip margins are budget-layer, asserted as pure comparisons on the fixture's stated margins: positive → X, negative → O, zero → draw). Threshold and auction-trace categories are OUT of scope (search/budget math is successor-DD).
- Fixture JSON parsing: this is a TEST target, not core; use the vendored nlohmann header (test target may link it; core still cannot).

- [ ] **Step 1: Write the reader + runner**

```c++
// Discover files: env UTTT_FIXTURES_DIR, default "../../theory/fixtures" relative
// to the test binary dir (document in bench/README.md too).
// If the directory is absent or contains no schema_version==1 uttt files:
//   FAIL the test with message "theory fixtures not present — acceptance
//   criterion 1 is unmet until theory publishes" UNLESS env
//   UTTT_ALLOW_MISSING_FIXTURES=1, in which case WARN and pass.
//   (CI/acceptance runs unset it; local mid-development runs may set it.)
// For legality fixtures: build Position from the fixture's position encoding
// per SCHEMA.md (board strings + forced + h), run legal_moves, compare as sets.
// For closure/routing: apply the given move (mover from the fixture), compare
// closure set, next forced (null <-> kForcedAny), terminal flag.
// For terminal: compare TerminalKind and, for nine-closed cases, assert the
// margin-sign rule on the fixture's stated margin.
```

The Position-from-fixture builder validates via a `Position::from_parts(...)` factory added in this task (same file as position.cpp): it takes cell masks/forced/tie, recomputes tern/macro/closed/key, and REJECTS (returns `std::expected` error) any local board whose table entry is invalid (DD §6 import validation) or whose forced/tie values are out of domain — with tests using a hand-built invalid dual-winner import.

- [ ] **Step 2: Run against theory/fixtures if present; else verify the explicit-fail path fires.** **Step 3: Build + ctest.** **Step 4: Commit** `"engine: theory fixture ingestion (legality/routing/terminal) + import validation"`.

---

### Task 10: Vendored JSON + wire types + adapter

**Files:**
- Create: `engine/third_party/nlohmann/json.hpp`, `engine/third_party/nlohmann/LICENSE.MIT`
- Create: `engine/src/adapter/wire.hpp`, `engine/src/adapter/wire.cpp`, `engine/src/adapter/policy.hpp`, `engine/src/adapter/main.cpp`
- Create: `engine/tests/test_wire.cpp`
- Modify: `engine/CMakeLists.txt` (add `uttt_engine` binary; nlohmann include ONLY on adapter + test targets)

**Interfaces:**
- Consumes: harness protocol v1 (DD-harness-c1 @ 11ac4efc85…): referee→engine `hello` `{type, protocol, game_id, you, rules, time_ms, grace_ms, budget}`; engine→referee hello `{type, protocol, name, version, author?}`; turn request `{type, protocol, game_id, request_id, ply, attempt, you, board[9 strings], forced: 0-8|null, legal: [[b,c]...], budgets: {X, O}, tie_owner: "X"|"O"|null, time_ms}`; turn reply `{type, protocol, request_id, bid, move: [b,c], info?}`; `game_end` (read and ignored beyond loop exit bookkeeping).
- Produces:

```c++
namespace uttt::wire {
struct TurnRequest { std::string game_id, request_id; int ply, attempt;
  Seat you; Position pos; int64_t budget_x, budget_o; int64_t time_ms;
  std::vector<Move> legal; };
struct Info { std::string quality;  // "exact"|"bound"|"estimate"
  std::optional<double> lo, hi; int depth; bool complete; };
struct TurnReply { std::string request_id; int64_t bid; Move move; Info info; };
// Parse + validate one line; unknown keys ignored; missing/type-invalid
// required keys -> std::unexpected(ParseError) (fail-closed, no stdout).
std::expected<TurnRequest, std::string> parse_turn(std::string_view line);
std::string serialize_reply(const TurnReply&);
std::string serialize_hello();
}
namespace uttt {
struct Policy { virtual wire::TurnReply choose(const wire::TurnRequest&) = 0; virtual ~Policy() = default; };
// Placeholder until the search DD lands: bid 0 (legal per R3), move = first
// entry of the request's legal list, info{quality:"estimate", depth:0, complete:true}.
struct PlaceholderPolicy final : Policy { wire::TurnReply choose(const wire::TurnRequest&) override; };
}
```

- Validation in `parse_turn` (DD §6): protocol major == 1; board strings shape 9×9 over `./X/O`; budgets in [0, 10^9] ints; `tie_owner` null only at ply 0; `forced` null ↔ free choice; build Position via `Position::from_parts` (Task 9) which rejects invalid locals; CROSS-CHECK the request's `legal` list against own `legal_moves` — mismatch is fail-closed (return error, log diff to stderr).
- `main.cpp` loop: read lines; `hello` → reply hello; `turn` → parse, policy, serialize (echo `request_id`), write single line, flush; `game_end` → exit 0; parse error → stderr diagnostic, NO stdout line, continue; EOF → exit 0.

- [ ] **Step 1: Vendor nlohmann/json single header (latest 3.x), verify MIT license text, commit vendoring separately** `"engine: vendor nlohmann/json 3.x (MIT), adapter-only"`.
- [ ] **Step 2: Write failing wire tests** — round-trip a literal turn-request line copied from the harness DD example (ply 0, forced 4, tie_owner null); assert parsed fields; assert legal-list cross-check failure on a doctored list; assert missing `request_id` fails closed; assert reply serialization echoes request_id and emits `move` always; assert unknown keys ignored.
- [ ] **Step 3: Run to verify failure. Implement wire.cpp + policy + main.** **Step 4: Build + ctest.** **Step 5: Commit** `"engine: protocol v1 adapter (hello/turn/game_end), fail-closed validation, placeholder policy"`.

---

### Task 11: End-to-end binary test

**Files:**
- Create: `engine/tests/test_engine_e2e.py`
- Modify: `engine/CMakeLists.txt` (add `add_test(NAME e2e COMMAND python3 ...)` passing the binary path)

- [ ] **Step 1: Write the script**

```python
# Spawns build/uttt_engine; sends hello, asserts hello reply on stdout (one line, valid JSON);
# sends the DD-harness example ply-0 turn request; asserts exactly one stdout line,
# request_id echoed, move in the legal list, bid == 0, info.quality == "estimate";
# sends a malformed line; asserts NO stdout output within 200 ms and process alive;
# sends game_end; asserts clean exit 0.
# stderr is captured and may be non-empty; stdout must contain ONLY the two protocol lines.
```

- [ ] **Step 2: Run, expect pass (fix flushing if the one-line assertions hang).** **Step 3: Commit** `"engine: e2e adapter test — stdout discipline, fault silence, request_id echo"`.

---

### Task 12: Benchmark harness + named baseline artifact + predicate

**Files:**
- Create: `engine/bench/bench_playout.cpp`, `engine/bench/README.md`
- Modify: `engine/CMakeLists.txt` (target `uttt_bench`, RelWithDebInfo/-O3)

**Interfaces (DD §2 ordering rule + §13 criterion 4, verbatim discipline):**
- `uttt_bench --reference --runs 10 --out engine/bench/baseline.json`: measures the NAIVE path (`naive::RefPosition` playouts), fixed seed set, per run: full random playouts until ≥ 2,000,000 plies; records median ns/ply over runs plus environment `{cpu: sysctl machdep.cpu.brand_string, compiler: __VERSION__, flags, date}`; volatile/`DoNotOptimize`-style sinks on every loop result.
- `uttt_bench --candidate --runs 10 --baseline engine/bench/baseline.json`: measures the optimized `Position` path the same way, then prints PASS/FAIL of BOTH predicates — median candidate ≤ 1.0 × median reference AND median candidate ≤ 100 ns/ply — exiting nonzero on FAIL.
- ORDER (anti-tautology): the reference run is executed and `baseline.json` COMMITTED before the candidate mode is ever run for acceptance; candidate never writes the baseline file.

- [ ] **Step 1: Write bench_playout.cpp** (both modes share the playout driver via a template on the position type; sink pattern: `asm volatile("" :: "r"(value));`).
- [ ] **Step 2: Build; run --reference; commit baseline** `"engine: benchmark baseline artifact (naive reference path, median of 10 runs, env captured)"`.
- [ ] **Step 3: Run --candidate; record PASS output in the commit message body; commit** `"engine: benchmark candidate PASS vs committed baseline (ratio + absolute ceiling)"`. If FAIL: stop, do not tune the predicate; profile, fix, re-run; the predicate itself only changes via a design amendment.
- [ ] **Step 4: Write bench/README.md** documenting the methodology (runs, seeds, warmup = first run discarded, flags, cpu capture, fixture-dir env vars) in full sentences on their own lines.

---

### Task 13: Acceptance sweep and completion evidence

- [ ] **Step 1: Full clean build + ctest** — all tests pass, including fixture tests with real `theory/fixtures` if published (unset `UTTT_ALLOW_MISSING_FIXTURES`; if theory has not landed, record criterion 1 as pending with the env-var evidence line).
- [ ] **Step 2: Map results to DD §13:** criterion 1 (fixtures) → ctest fixture suite; criterion 2 (properties/table proof/lifecycle/Zobrist/perft) → ctest; criterion 3 (adapter round-trip + stdout discipline) → ctest e2e; criterion 4 (benchmark predicate) → bench PASS output. Paste the ctest summary and bench verdict into the implementation report.
- [ ] **Step 3: Commit any stragglers; produce the IMPL report relay per protocol with ACTIONS_GIT_REF `engine/rules-core-c1@<sha>`.**

---

## Boundary contract (from DD §12, carried verbatim)

Writes: engine analysis metadata semantics (DD §10) inside the harness-owned `info` carrier.
Reads: harness protocol v1 @ pinned digest; theory fixture schema v1; no other cross-surface reads.
Target entity: `engine/` rules core + adapter + tests conforming to DD-engine-rules-c1-20260825.
Downstream consumer: referee (spawns binary), ui (analysis view), theory (cross-validation).
Proof: E2 (ctest suites + bench verdict); E3 (referee round-trip) belongs to a later cycle.
No-consumer action: not applicable.

## Out of scope (hard lines)

- Search math of any kind (backup, cutoffs, TT, widening, eval, bid matrix) — successor DD.
- Protocol schema or fixture schema changes — owner-routed only.
- Multithreading, SIMD, ML.
- Any file outside `engine/` (the sprint relay/plan tree excepted for the report relay + INDEX row).
- The injected-clock seam (DD §7): it exists to make the search soft-stop testable, no search loop exists in this plan to consume it, and it ships with the search PLAN; the adapter's `time_ms` is parsed and carried but unused by the placeholder policy.

## Anti-half-fix guards

- The exhaustive table proof and the randomized cross-implementation properties may not be weakened (no sampling the 19,683, no reducing N below 2,000 games) without a design amendment.
- A failing fixture is never skipped ad hoc: it either reveals an engine bug (fix), or a fixture/schema question (blocker relay to the orchestrator for theory routing).
- The benchmark predicate is immutable at PLAN level (design amendment required); a FAIL is a performance bug, not a threshold negotiation.
- No placeholder-policy cleverness: the placeholder stays trivially-legal-and-dumb so search-strength claims cannot leak in early.

## Verification target

E2 for every acceptance criterion executable now (DD §13 criteria 1–4; criterion 1 conditionally pending theory fixture publication); criterion 5 explicitly deferred (theory Stage-1 + successor DD).

## Operator-judgment items

None. All choices herein are locked-design consequences or mechanical.
