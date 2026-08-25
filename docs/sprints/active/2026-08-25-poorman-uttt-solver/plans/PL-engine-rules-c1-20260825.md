# Engine Rules Core Implementation Plan (PL-engine-rules-c1-20260825)

Revision 4 (2026-08-25) folds PLAN-REVIEW `engine-c1-plan-review-4` (relay 100509), all three residuals: MR1-R Task 1 commit lists every literal file (placeholder .cpp included) and the staged-set proof is exact sorted equality, not subset membership, with the pattern binding every later commit; MR4-R the `validate_game_end` interface comment states strict owner-schema validation and the test battery names one negative case per required-field class plus 32 KiB boundary tests proving pre-parse rejection; MR5-R Task 12 has an explicit three-commit topology (harness source → baseline.json only → candidate-verdict.json durable artifact on PASS, never on FAIL).

Revision 3 (2026-08-25) folds PLAN-REVIEW `engine-c1-plan-review-3` (relay 094459), all five groups: MR1 base preflight replaced with object-existence + ancestry + engine-surface-diff checks (main-equality was self-invalid) and mechanical staged-set proof via `git diff --cached --name-only`; MR2 CMake pin comment corrected, Task 2 test includes its seam headers, and a mechanical all-closed totality fixture added beside the macro-win case; MR3 Zobrist sensitivity rebuilt as single-input isolation over the full population for BOTH the 64-bit key and the independent 32-bit tag; MR4 strict `game_end` required-field/enum/budget validation, the 32 KiB line ceiling, and a full binary round-trip corpus test validating emitted replies; MR5 the reference baseline is committed BEFORE any candidate run, with identity refusal preserving the same-session proof.

Revision 2 (2026-08-25) folds PLAN-REVIEW `engine-c1-plan-review-1` (relay 090632), all eight groups: PR1 exact base lock + gitignore-first + explicit staging; PR2 normative bit order with final literals, pinned macro-win sequence, exact RefPosition, from_parts as the mechanical builder; PR3 both dependencies pinned by version/URL/sha256 and vendored in Task 1 before any consumer, JSON boundary restated; PR4 RootContext + winner_on_chips production seam and the DD clock seam brought IN scope (Task 2); PR5 independent secondary Zobrist table + full-population sensitivity tests; PR6 complete ImportError classes, all three message types validated, conformance-corpus consumption with honest pending state; PR7 single-session acceptance benchmark with identity capture, warmup separate from ≥ 10 measured runs, and a refusing --verify mode; PR8 acceptance truth table replacing the overclaiming sweep.

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Implement the C++26 rules core, local tables, Zobrist keying, test seam, benchmark harness, and JSONL protocol adapter locked by DD-engine-rules-c1-20260825 — no search math.

**Architecture:** Value-copy immutable `Position` (per-player `array<uint16_t,9>` + caches) over a 19,683-entry packed semantic local table; naive reference implementations exist beside the optimized path for property tests, the exhaustive table proof, and the benchmark baseline; a JSON-free core behind a thin adapter that consumes harness protocol v1 with a placeholder policy until the theory-gated search DD lands.

**Tech Stack:** C++26 (`-std=c++2c`), Homebrew LLVM 22.1.8, CMake ≥ 3.28 + Ninja, doctest (vendored single header, MIT), nlohmann/json (vendored single header, MIT, adapter-only).

**Spec:** docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-rules-c1-20260825.md @ sha256 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47 (locked; approving review engine-c1-design-review-4).
Consumed owner contracts: harness protocol v1 = DD-harness-c1-20260825 @ 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440; theory fixture schema v1 = DD-theory-c1-20260825 §3 (`theory/fixtures/SCHEMA.md`, `schema-v1.json`).

## Global Constraints

- Compiler: Homebrew LLVM 22.1.8 (`/opt/homebrew/opt/llvm/bin/clang++`), `-std=c++2c`; feature-tested library facilities only (`std::simd` and `<inplace_vector>` are absent — do not include them).
- JSON boundary (PR3 clarification): the PRODUCTION core (`engine/src/core/`, target `uttt_core`) has zero JSON and zero I/O dependencies; the adapter target and TEST-ONLY fixture ingestion may use the pinned vendored header.
- Dependency pins (PR3; verified 2026-08-25, MIT license bytes confirmed): doctest v2.4.12 from https://raw.githubusercontent.com/doctest/doctest/v2.4.12/doctest/doctest.h sha256 94029a7d32da24a56249658147dbd2b33ff0b9ed665295cbbaf19aafff5b0ced, LICENSE sha256 0fe0b331fa1513dcce8604ff1fa925f32d1cea17d8aeb1c2471fad40d291adc5; nlohmann/json v3.12.0 from https://github.com/nlohmann/json/releases/download/v3.12.0/json.hpp sha256 aaf127c04cb31c406e5b04a63f1ae89369fccde6d8fa7cdda1ed4f32dfc5de63, LICENSE.MIT (from https://raw.githubusercontent.com/nlohmann/json/v3.12.0/LICENSE.MIT) sha256 46a65cffd1ea955132d95a8dd921640714a8d6b537d2e4e482d31145ae95b603. Vendoring verifies these digests with `shasum -a 256` and records them in a comment in CMakeLists; a mismatch aborts and escalates.
- Bit order (PR2, normative everywhere): bit c = cell c, cells row-major within a local board — bit 0 = row 0 col 0 … bit 8 = row 2 col 2; identical to the DD's `3^c` ternary convention. `kWinLines` values are order-symmetric and unchanged; every mask literal in this plan is written in this convention and is final.
- No search math: no backup operator, no cutoff/bound math, no TT, no widening. `zobrist` keying IS in scope (it keys state identity, not values).
- License gate: nelhage/ultimattt and TheGustafson/ai-ultimate-tictactoe are reference-study only; copying code from them is prohibited.
- Vendored deps are pinned single headers with their LICENSE text committed beside them; both chosen libs are MIT (verify the license file content on vendoring; abort and escalate if not MIT).
- stdout of the engine binary carries protocol lines only; all diagnostics go to stderr; malformed requests produce no stdout line.
- Rules follow the design spec canonical rules AS LOCKED in the DD; if any task step seems to contradict the DD, the DD wins and the conflict is a blocker relay, not a local fix.
- Long .md files put full sentences on their own lines.
- Commits: small, per task step as marked; never claim a fix without the named test output.
- Branch (PR1 as corrected by MR1 — main-equality is unusable because sprint-document commits legitimately advance main): at IMPL start prove the dispatched BASE object is still safe with exactly these three checks — (1) `git cat-file -e 63b4b7b^{commit}` succeeds; (2) `git merge-base --is-ancestor 63b4b7b main` succeeds; (3) `git diff --name-only 63b4b7b..main -- engine/` prints NOTHING (the engine surface is untouched since the dispatched base). If any check fails, STOP and relay to s1.orchestrator-planner for a successor BASE — never float the base silently. On success: `git switch -c engine/rules-core-c1 63b4b7b`. Never commit to `main`.
- Staging discipline (PR1/MR1/MR1-R, mechanical EQUALITY): stage the exact FILES named in each commit step (`git add <file> <file> ...`, no bare directories); then prove the staged set is EXACTLY the expected set — `git diff --cached --name-only | sort` must byte-equal the step's sorted expected-file list (`diff <(git diff --cached --name-only | sort) <(printf '%s\n' <expected...> | sort)` prints nothing) — a strict subset fails; additionally REQUIRE `git diff --cached --name-only | grep -c '^engine/build/'` prints 0. `engine/bench/baseline.json` is intentionally trackable.

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
  src/core/budget.hpp                # RootContext + winner_on_chips (pure, header-only; PR4)
  src/core/clock.hpp                 # Clock interface + FakeClock (header-only; PR4 — DD §7 locked seam)
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
  bench/bench_playout.cpp            # --reference --out baseline.json | --candidate --baseline <path> --out-verdict candidate-verdict.json
  bench/baseline.json                # committed reference artifact (Step 2 of Task 12)
  bench/candidate-verdict.json       # committed candidate PASS evidence (Step 3 of Task 12)
  bench/README.md                    # methodology: identity, warmup vs measured runs, commit topology
```

Coordinate conventions everywhere: local boards 0–8 row-major across the macro grid; cells 0–8 row-major within a local board; matches the harness wire and theory fixtures.

---

### Task 1: Scaffold, gitignore, toolchain lock, and BOTH vendored dependencies

**Files:**
- Create: `engine/.gitignore` (content: `build/`)
- Create: `engine/CMakeLists.txt`
- Create: `engine/third_party/doctest/doctest.h`, `engine/third_party/doctest/LICENSE.txt`
- Create: `engine/third_party/nlohmann/json.hpp`, `engine/third_party/nlohmann/LICENSE.MIT`
- Create: `engine/tests/test_smoke.cpp` (deleted in Task 2 when real tests exist — keep if you prefer; it is one assertion)

**Interfaces:**
- Produces: build commands used by every later task: `cmake -S engine -B engine/build -G Ninja -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ -DCMAKE_BUILD_TYPE=RelWithDebInfo` then `ninja -C engine/build` then `ctest --test-dir engine/build --output-on-failure`.

- [ ] **Step 0: Write engine/.gitignore FIRST (PR1)**

Content is the single line `build/`.
Verify: `git check-ignore engine/build/x` prints the path after the file exists.

- [ ] **Step 1: Vendor BOTH pinned dependencies (PR3)**

Download exactly the four pinned URLs from Global Constraints, verify each file's sha256 against the pinned digests with `shasum -a 256` (abort on any mismatch), confirm both license files are MIT by reading their first lines, and place them at the paths above.
Record versions + digests in a comment block at the top of CMakeLists.
nlohmann/json is vendored NOW so Task 9's fixture reader (its first consumer) never precedes it.

- [ ] **Step 2: Write CMakeLists**

```cmake
cmake_minimum_required(VERSION 3.28)
project(uttt_engine CXX)
# Vendored (Task 1, pinned): doctest v2.4.12 sha256 94029a7d32da24a56249658147dbd2b33ff0b9ed665295cbbaf19aafff5b0ced
#                            nlohmann/json v3.12.0 sha256 aaf127c04cb31c406e5b04a63f1ae89369fccde6d8fa7cdda1ed4f32dfc5de63
# nlohmann is linked into adapter/test targets ONLY, never uttt_core.
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

- [ ] **Step 5: Commit (literal files only; exact-equality staged-set proof — MR1-R)**

```bash
git add engine/.gitignore engine/CMakeLists.txt \
  engine/third_party/doctest/doctest.h engine/third_party/doctest/LICENSE.txt \
  engine/third_party/nlohmann/json.hpp engine/third_party/nlohmann/LICENSE.MIT \
  engine/tests/test_smoke.cpp \
  engine/src/core/local_table.cpp engine/src/core/position.cpp engine/src/core/zobrist.cpp
diff <(git diff --cached --name-only | sort) <(printf '%s\n' \
  engine/.gitignore engine/CMakeLists.txt \
  engine/third_party/doctest/doctest.h engine/third_party/doctest/LICENSE.txt \
  engine/third_party/nlohmann/json.hpp engine/third_party/nlohmann/LICENSE.MIT \
  engine/tests/test_smoke.cpp \
  engine/src/core/local_table.cpp engine/src/core/position.cpp engine/src/core/zobrist.cpp | sort)
# REQUIRE: the diff prints nothing (exact set equality), and:
git diff --cached --name-only | grep -c '^engine/build/'   # REQUIRE: 0
git commit -m "engine: scaffold + gitignore + pinned vendored doctest 2.4.12 and nlohmann/json 3.12.0 (MIT)"
```

Every later commit step follows the same pattern: its named file list IS the expected set for the equality check (Global Constraints).

---

### Task 2: Core types + budget and clock seams (PR4)

**Files:**
- Create: `engine/src/core/types.hpp`
- Create: `engine/src/core/budget.hpp`
- Create: `engine/src/core/clock.hpp`
- Create: `engine/tests/test_types.cpp` (add to `uttt_tests` sources; do the same for every later test file)

**Additional produced interfaces (locked DD seams):**

```c++
// budget.hpp — the production all-closed resolver (DD §4/§9): pure, core-resident.
namespace uttt {
struct RootContext { Seat seat; int64_t budget_x; int64_t budget_o; };
enum class ChipResult : uint8_t { XWins, OWins, Draw };
constexpr ChipResult winner_on_chips(int64_t budget_x, int64_t budget_o) {
  return budget_x > budget_o ? ChipResult::XWins
       : budget_x < budget_o ? ChipResult::OWins : ChipResult::Draw;
}
}
// clock.hpp — the DD §7 injected-clock seam (PR4: locked bytes, not deferred).
namespace uttt {
struct Clock { virtual int64_t now_ms() = 0; virtual ~Clock() = default; };
struct FakeClock final : Clock { int64_t t = 0; int64_t now_ms() override { return t; } };
struct SteadyClock final : Clock { int64_t now_ms() override; };  // steady_clock in .hpp inline
}
```

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
#include "core/budget.hpp"
#include "core/clock.hpp"
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
TEST_CASE("all-closed chip resolution: production seam, all three margins (PR4)") {
  CHECK(winner_on_chips(500000001, 499999999) == ChipResult::XWins);   // +margin
  CHECK(winner_on_chips(499999999, 500000001) == ChipResult::OWins);   // -margin
  CHECK(winner_on_chips(1, 1) == ChipResult::Draw);                    // exact equality (spec ½–½)
  CHECK(winner_on_chips(0, 0) == ChipResult::Draw);
}
TEST_CASE("fake clock contract") {
  FakeClock c; CHECK(c.now_ms() == 0); c.t = 27000; CHECK(c.now_ms() == 27000);
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

- [ ] **Step 1: Write failing tests — every class from the DD (literals FINAL in the normative bit order: bit c = cell c, bit 0 = r0c0 … bit 8 = r2c2; PR2)**

```c++
#include "doctest/doctest.h"
#include "core/naive_local.hpp"
using namespace uttt;
// bit c = cell c, row-major: row r = bits {3r, 3r+1, 3r+2}; col k = bits {k, k+3, k+6}.
static constexpr uint16_t rowmask(int r) { return uint16_t(0b111u << (3 * r)); }
TEST_CASE("naive local eval") {
  auto empty = naive::eval_local(0, 0);
  CHECK(empty.valid); CHECK(empty.status == LocalStatus::Open);
  CHECK(empty.empties == 0x1FF); CHECK(empty.win_x == 0);

  // single-line win (X row 0 = bits 0,1,2; O two stray cells 7,8): valid
  auto onewin = naive::eval_local(rowmask(0), 0b110000000);
  CHECK(onewin.valid); CHECK(onewin.status == LocalStatus::XWon);
  CHECK(onewin.empties == 0);

  // dual-winner: invalid (MR1 class 1)
  auto dual = naive::eval_local(rowmask(0), rowmask(1));
  CHECK_FALSE(dual.valid); CHECK(dual.status == LocalStatus::FullDraw);
  CHECK(dual.empties == 0); CHECK(dual.win_x == 0); CHECK(dual.fork_o == 0);

  // same-player disjoint lines (X rows 0 and 1): invalid (MR1 class 2)
  auto disjoint = naive::eval_local(uint16_t(rowmask(0) | rowmask(1)), 0);
  CHECK_FALSE(disjoint.valid);

  // same-player two lines sharing the closing mark: VALID.
  // X = row 0 (bits 0,1,2) + col 0 (bits 0,3,6) = 0b001001111;
  // bit 0 lies on both lines and removing it kills both.
  auto shared = naive::eval_local(0b001001111, 0);
  CHECK(shared.valid); CHECK(shared.status == LocalStatus::XWon);

  // immediate win mask: X holds cells 0,1 -> cell 2 completes row 0
  auto near = naive::eval_local(0b000000011, 0);
  CHECK((near.win_x & 0b000000100) != 0);

  // fork: X at cells 0 (r0c0) and 2 (r0c2), candidate center cell 4:
  // placing X at 4 completes nothing, and afterwards cells {1 (row0),
  // 8 (main diag 0,4,8), 6 (anti-diag 2,4,6)} are distinct immediate wins (>= 2).
  auto forky = naive::eval_local(0b000000101, 0);
  CHECK((forky.fork_x & 0b000010000) != 0);

  // full draw: XXO/OOX/XXO -> x = cells {0,1,5,6,7} = 0b011100011,
  // o = cells {2,3,4,8} = 0b100011100; hand-checked: no line either side, board full.
  auto draw = naive::eval_local(0b011100011, 0b100011100);
  CHECK(draw.valid); CHECK(draw.status == LocalStatus::FullDraw);
}
```

The bit order is normative for the whole codebase (Global Constraints); document it in a comment in `types.hpp` and use it in fixture/wire conversion.
These literals are final; do not re-derive them at IMPL.

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

  // Mechanical import/fixture builder (PR2/PR6; lives here, NOT in Task 9).
  // ImportError is declared beside Position in position.hpp:
  // enum class ImportError : uint8_t { OverlappingMasks, InvalidLocal,
  //   ForcedOutOfDomain, ForcedToClosedBoard, TieOutOfDomain, TieNullAfterFirstMark };
  // recomputes tern/macro/closed/key from the masks and REJECTS every locked
  // inconsistency class (ImportError enum): OverlappingMasks, InvalidLocal
  // (table validity bit), ForcedOutOfDomain (not in {0..8, kForcedAny}),
  // ForcedToClosedBoard (non-ANY forced naming a closed board — this alone
  // covers the all-closed-with-forced contradiction, since every board is
  // closed then), TieOutOfDomain, TieNullAfterFirstMark (NullFirstMove with
  // any cell already marked). Terminal imports (macro win / all-closed) are
  // otherwise accepted; their consistency is fully enforced by the rules above
  // because closed/macro state is recomputed, never trusted from the caller.
  static std::expected<Position, ImportError>
  from_parts(const std::array<uint16_t, 9>& x, const std::array<uint16_t, 9>& o,
             int8_t forced, TieState tie);
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
TEST_CASE("terminal totality (M2): macro win") {
  auto t = make_macro_win_x();
  CHECK(t.terminal() == TerminalKind::MacroWinX);
  MoveList ml; t.legal_moves(ml);
  CHECK(ml.n == 0);
  CHECK(t.applied({8, 8}, Seat::O).error() == ApplyError::TerminalParent);
}
TEST_CASE("terminal totality (M2/MR2): all-closed, mechanical fixture, no theory dependency") {
  // Every local board is the hand-verified valid full-draw from Task 3:
  // x = 0b011100011, o = 0b100011100 (XXO/OOX/XXO). Nine full-draw locals ->
  // closed == 0x1FF, no macro line, terminal AllClosed. forced must be ANY
  // (every board closed); tie X arbitrary.
  std::array<uint16_t, 9> ax{}, ao{};
  ax.fill(0b011100011); ao.fill(0b100011100);
  auto t = Position::from_parts(ax, ao, kForcedAny, TieState::X).value();
  CHECK(t.terminal() == TerminalKind::AllClosed);
  MoveList ml; t.legal_moves(ml);
  CHECK(ml.n == 0);
  CHECK(t.applied({0, 0}, Seat::X).error() == ApplyError::TerminalParent);
}
```

`make_macro_win_x()` is this EXACT pinned sequence (PR2; X wins every auction — legal under bidding; forced-state comments verified by REQUIRE on `forced` after each step):

```c++
static Position make_macro_win_x() {
  auto p = Position::initial();                       // forced 4
  auto step = [&](Move m) { p = p.applied(m, Seat::X).value(); };
  step({4, 0});  REQUIRE(p.forced == 0);              // cell 0 -> board 0
  step({0, 0});  REQUIRE(p.forced == 0);
  step({0, 1});  REQUIRE(p.forced == 1);
  step({1, 0});  REQUIRE(p.forced == 0);
  step({0, 2});  REQUIRE((p.closed >> 0) & 1);        // board 0: row 0 done
                 REQUIRE(p.forced == 2);
  step({2, 1});  REQUIRE(p.forced == 1);
  step({1, 1});  REQUIRE(p.forced == 1);
  step({1, 2});  REQUIRE((p.closed >> 1) & 1);        // board 1: row 0 done
                 REQUIRE(p.forced == 2);
  step({2, 0});  REQUIRE(p.forced == kForcedAny);     // cell 0 -> board 0 closed
  step({2, 2});                                       // board 2: row 0 done -> macro row {0,1,2}
  return p;
}
```

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
// scans all cells for movegen, no caches. Same Move/Seat types. Timing-free.
struct RefPosition {
  std::array<uint8_t, 81> cells{};  // index 9*board + cell; 0 empty, 1 X, 2 O
  int8_t forced = 4;
  TieState tie = TieState::NullFirstMove;
  static RefPosition initial();
  TerminalKind terminal() const;
  void legal_moves(MoveList& out) const;
  RefPosition applied_unchecked(Move mv, Seat mover) const;  // caller passes legal moves only
};
}
```

This interface is final (PR2 — the earlier `Cell81` placeholder is gone).

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
// PRIMARY: 175 uint64 randoms: cell[81][2 seats], forced[10] (0..8 + ANY), tie[3].
// SECONDARY (PR5): an INDEPENDENT 175-entry uint32 table over the SAME identity
// inputs, generated from a DIFFERENT fixed seed — never derived from the primary
// key, so a primary collision does not imply a tag collision.
// Both generated at startup from FIXED seeds (primary: splitmix64 stream seeded
// 0x9E3779B97F4A7C15; secondary: splitmix64 stream seeded 0xA5A5A5A5DEADBEEF),
// stable across runs and platforms.
struct ZobristTables { /* both table sets */ static const ZobristTables& instance(); };
uint64_t zobrist_full(const Position& p);   // primary recompute from identity fields
uint32_t zobrist_tag_full(const Position& p);  // secondary recompute (future TT verify tag)
}
```

- `applied()` updates `key`: XOR out old forced and tie randoms, XOR in the placed cell random, XOR in new forced and tie randoms. (`key` stores the primary; the secondary is recompute-only until the TT exists.)

- [ ] **Step 1: Write failing tests (full declared input population — PR5)**

```c++
TEST_CASE("incremental key == full recompute along random games") { /* reuse Task 6 loop; REQUIRE p.key == zobrist_full(p) each ply */ }
TEST_CASE("single-input isolation, BOTH hash functions, FULL population (MR3)") {
  // Every comparison below differs in EXACTLY ONE declared identity input.
  std::array<uint16_t, 9> zx{}, zo{};
  // Common frame for cell/seat isolation: forced 4, tie O, held constant.
  auto base = Position::from_parts(zx, zo, 4, TieState::O).value();
  for (int b = 0; b < 9; ++b) for (int c = 0; c < 9; ++c) {
    auto mx = zx; mx[b] = uint16_t(1u << c);
    auto px = Position::from_parts(mx, zo, 4, TieState::O).value();   // + X mark only
    auto mo = zo; mo[b] = uint16_t(1u << c);
    auto po = Position::from_parts(zx, mo, 4, TieState::O).value();   // + O mark only
    // cell-entry isolation: base vs marked (tie and forced identical)
    CHECK(px.key != base.key);
    CHECK(po.key != base.key);
    CHECK(zobrist_tag_full(px) != zobrist_tag_full(base));
    CHECK(zobrist_tag_full(po) != zobrist_tag_full(base));
    // seat-entry isolation: same cell, same tie, same forced, owner differs
    CHECK(px.key != po.key);
    CHECK(zobrist_tag_full(px) != zobrist_tag_full(po));
  }
  // Forced isolation: identical empty board + tie O; all ten forced values
  // pairwise for BOTH functions (every board open, so 0..8 and ANY all import).
  std::array<uint64_t, 10> k{}; std::array<uint32_t, 10> g{};
  for (int f = 0; f < 9; ++f) {
    auto p = Position::from_parts(zx, zo, int8_t(f), TieState::O).value();
    k[f] = p.key; g[f] = zobrist_tag_full(p);
  }
  { auto p = Position::from_parts(zx, zo, kForcedAny, TieState::O).value();
    k[9] = p.key; g[9] = zobrist_tag_full(p); }
  for (int i = 0; i < 10; ++i) for (int j = i + 1; j < 10; ++j) {
    CHECK(k[i] != k[j]); CHECK(g[i] != g[j]);
  }
  // Tie isolation: identical EMPTY board + forced 4; all three tie values
  // pairwise for BOTH functions (empty board admits NullFirstMove, and X/O
  // on an empty board import — only Null-with-marks is rejected).
  auto tn = Position::from_parts(zx, zo, 4, TieState::NullFirstMove).value();
  auto tx = Position::from_parts(zx, zo, 4, TieState::X).value();
  auto to = Position::from_parts(zx, zo, 4, TieState::O).value();
  CHECK(tn.key != tx.key); CHECK(tn.key != to.key); CHECK(tx.key != to.key);
  CHECK(zobrist_tag_full(tn) != zobrist_tag_full(tx));
  CHECK(zobrist_tag_full(tn) != zobrist_tag_full(to));
  CHECK(zobrist_tag_full(tx) != zobrist_tag_full(to));
}
```

`from_parts` (Task 5) is the mechanical builder — no routing invention (PR2); every pairing above isolates exactly one input, and both the 64-bit key and the independent 32-bit tag are exercised over all 162 cell/seat entries, all ten forced entries, and all three tie entries (MR3).

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

The Position-from-fixture builder is `Position::from_parts` (Task 5).
This task adds rejection tests for EVERY ImportError class (PR6): overlapping masks; invalid local (dual-winner AND same-player-disjoint-lines imports); forced out of domain; non-ANY forced naming a closed board (build a closed board 0 via masks, import with forced = 0); NullFirstMove with a mark present; tie out of domain.

- [ ] **Step 2: Run against theory/fixtures if present; else verify the explicit-fail path fires.** **Step 3: Build + ctest.** **Step 4: Commit** `"engine: theory fixture ingestion (legality/routing/terminal) + import validation"`.

---

### Task 10: Wire types + adapter (JSON already vendored in Task 1)

**Files:**
- Create: `engine/src/adapter/wire.hpp`, `engine/src/adapter/wire.cpp`, `engine/src/adapter/policy.hpp`, `engine/src/adapter/main.cpp`
- Create: `engine/tests/test_wire.cpp`
- Modify: `engine/CMakeLists.txt` (add `uttt_engine` binary; nlohmann include ONLY on adapter + test targets, never `uttt_core`)

**Interfaces:**
- Consumes: harness protocol v1 (DD-harness-c1 @ 11ac4efc85…): referee→engine `hello` `{type, protocol, game_id, you, rules, time_ms, grace_ms, budget}`; engine→referee hello `{type, protocol, name, version, author?}`; turn request `{type, protocol, game_id, request_id, ply, attempt, you, board[9 strings], forced: 0-8|null, legal: [[b,c]...], budgets: {X, O}, tie_owner: "X"|"O"|null, time_ms}`; turn reply `{type, protocol, request_id, bid, move: [b,c], info?}`; `game_end` (read and ignored beyond loop exit bookkeeping).
- Produces:

```c++
namespace uttt::wire {
// The DD-locked canonicalization target is Position + RootContext (PR4).
struct TurnRequest { std::string game_id, request_id; int ply, attempt;
  RootContext ctx;               // seat from `you`, canonical X/O budgets
  Position pos;                  // via Position::from_parts (all ImportError classes fail-closed)
  int64_t time_ms;
  std::vector<Move> legal; };
struct Info { std::string quality;  // "exact"|"bound"|"estimate"
  std::optional<double> lo, hi; int depth; bool complete; };
struct TurnReply { std::string request_id; int64_t bid; Move move; Info info; };
struct HelloRequest { int protocol; std::string game_id, rules; Seat you;
  int64_t time_ms, grace_ms, budget; };
// Strict parse + validate, ALL THREE referee->engine message types (PR6);
// unknown keys ignored; missing/type-invalid required keys ->
// std::unexpected(error string) (fail-closed, no stdout).
enum class MsgType { Hello, Turn, GameEnd };
std::expected<MsgType, std::string> classify(std::string_view line);
std::expected<HelloRequest, std::string> parse_hello(std::string_view line);
std::expected<TurnRequest, std::string> parse_turn(std::string_view line);
std::expected<void, std::string> validate_game_end(std::string_view line);  // STRICT owner-schema validation: required result, enum reason, canonical in-range X/O budgets all fail-closed; unknown keys ignored (MR4-R)
std::string serialize_reply(const TurnReply&);
std::string serialize_hello();
}
namespace uttt {
struct Policy { virtual wire::TurnReply choose(const wire::TurnRequest&, Clock&) = 0; virtual ~Policy() = default; };
// Placeholder until the search DD lands: bid 0 (legal per R3), move = first
// entry of the request's legal list, info{quality:"estimate", depth:0, complete:true}.
// Takes Clock so the seam exists end-to-end; the placeholder reads now_ms() once
// and ignores it (documented as deliberate).
struct PlaceholderPolicy final : Policy { wire::TurnReply choose(const wire::TurnRequest&, Clock&) override; };
}
```

- Validation in `parse_turn` (DD §6): protocol major == 1; board strings shape 9×9 over `./X/O`; budgets in [0, 10^9] ints; `tie_owner` null only at ply 0; `forced` null ↔ `kForcedAny`; build Position via `Position::from_parts` (every ImportError class fails closed); CROSS-CHECK the request's `legal` list against own `legal_moves` — mismatch is fail-closed (return error, log diff to stderr).
- Validation in `parse_hello` (PR6): protocol major == 1; `you` ∈ {"X","O"}; required numeric fields present and non-negative.
- `validate_game_end` (PR6 as corrected by MR4): STRICT on required fields per the owner schema — `type`, `protocol`, required `result`, `reason` from the schema's enum, canonical X/O `budgets` in range — each missing/type-invalid required key fails closed; unknown keys remain ignored. A failing game_end is a stderr diagnostic + no stdout, not a crash.
- Line ceiling (MR4, owner framing rule): the read loop rejects any input line exceeding 32 KiB before parsing (fail-closed, stderr diagnostic, no stdout).
- `main.cpp` loop: read lines; classify; `hello` → validate, reply hello; `turn` → parse, `policy.choose(req, clock)` with a `SteadyClock`, serialize (echo `request_id`), write single line, flush; `game_end` → validate, exit 0; parse error → stderr diagnostic, NO stdout line, continue; EOF → exit 0.
- Conformance corpus ROUND-TRIP (PR6 as corrected by MR4): the harness-owned schemas/normative transcript land under `docs/protocol/` per the harness DD. The corpus test (extends `test_engine_e2e.py`) drives the BINARY: feed every referee→engine line of the normative transcript in order on stdin; capture every engine→referee line from stdout; validate each EMITTED line against the owner reply schema (required keys/types, `request_id` echo matching the corresponding request, `move` ∈ that request's `legal`, `bid` in range) and each INCOMING line through classify/parse; assert stdout discipline throughout (exactly one line per hello/turn request, none otherwise). Only this full round-trip may report green. If the corpus is absent, the test FAILS with "harness conformance corpus not present — criterion 3 pending-blocked" unless env `UTTT_ALLOW_MISSING_CORPUS=1` (same honesty pattern as Task 9; acceptance runs unset it).

- [ ] **Step 1: Write failing wire tests** — round-trip the harness DD's literal ply-0 turn-request line (forced 4, tie_owner null); parse_hello on the DD's hello line; legal-list cross-check failure on a doctored list; missing `request_id` fails closed; tie_owner "X" at ply 1 parses, tie_owner null at ply 1 fails; reply serialization echoes request_id and always emits `move`; unknown keys ignored; corpus test wired per the note above. `validate_game_end` negative battery (MR4-R), one named case per required-field class: happy case passes; wrong `type` fails; missing `result` fails; type-invalid `result` fails; `reason` absent fails; `reason` outside the owner enum fails; `budgets` missing fails; `budgets` keyed other than canonical X/O fails; either budget out of [0, 10^9] fails; an extra unknown key still passes. Line-framing boundary tests (MR4-R): a line of exactly the 32 KiB maximum is accepted into parsing; a line one byte over is rejected BEFORE JSON parsing (prove via a deliberately malformed oversize payload that would throw in the parser — rejection must come from the length check) with a stderr diagnostic and no stdout.
- [ ] **Step 2: Run to verify failure. Implement wire.cpp + policy + main.** **Step 3: Build + ctest.** **Step 4: Commit** `"engine: protocol v1 adapter (hello/turn/game_end), fail-closed validation, placeholder policy"`.

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

**Interfaces (DD §2 ordering rule + §13 criterion 4; PR7 session mechanics as corrected by MR5 — the baseline is COMMITTED before any candidate acceptance):**
- Identity block (shared by both modes, baked via a CMake-configured header): own executable sha256 (hash argv[0] bytes), `__VERSION__`, compile flags, `sysctl machdep.cpu.brand_string`, fixed seed list, session UUID.
- `uttt_bench --reference --measured-runs 10 --warmup-runs 2 --out engine/bench/baseline.json`: measures the NAIVE path only — warmup runs discarded and ADDITIONAL to the ≥ 10 measured runs (PR7), each run ≥ 2,000,000 plies of fixed-seed full playouts, `asm volatile` sinks — then writes `baseline.json` = {median_ref_ns_per_ply, per-run values, identity block, fresh session UUID}. It contains no candidate code path.
- `uttt_bench --candidate --baseline engine/bench/baseline.json`: opens the baseline READ-ONLY; recomputes the identity block and REFUSES (exit nonzero, no measurement) on any mismatch of executable digest, compiler, flags, or CPU string — same binary/flags/harness/environment is thereby proven, and the session continuity is the baseline's UUID echoed into the verdict output; measures the optimized Position path with the same warmup/measured discipline; verdict: PASS iff median_cand ≤ 1.0 × median_ref AND median_cand ≤ 100.0, both medians and the UUID printed, exit nonzero on FAIL. This mode cannot write the baseline (no write path exists in it).
- ORDER (MR5, mechanical): `--reference` → `git add engine/bench/baseline.json` + commit → `--candidate` back-to-back against the just-committed artifact. The baseline is durably in history BEFORE the candidate gate, so a candidate FAIL leaves the reference evidence committed for diagnosis; the identity refusal preserves the DD's same-session/same-binary proof across the intervening docs-only commit.

Commit topology (MR5-R, three commits, exact bytes each):

- [ ] **Step 1: Write bench_playout.cpp (modes share the playout driver via a template on the position type; identity header configured by CMake); COMMIT the source first** — staged set exactly {`engine/bench/bench_playout.cpp`, `engine/CMakeLists.txt`} (equality proof per Global Constraints): `"engine: benchmark harness source (reference/candidate modes, identity capture)"`.
- [ ] **Step 2: Build; run --reference; COMMIT exactly {`engine/bench/baseline.json`}** `"engine: benchmark reference baseline (naive path, warmup+10 measured, identity captured)"` — this commit precedes any candidate run and touches nothing else.
- [ ] **Step 3: Run `--candidate --baseline engine/bench/baseline.json --out-verdict engine/bench/candidate-verdict.json`; the mode writes the DURABLE VERDICT ARTIFACT (both medians, both predicate results, baseline session UUID, identity block) — it still cannot write the baseline. On PASS, COMMIT exactly {`engine/bench/candidate-verdict.json`}** `"engine: benchmark candidate PASS vs committed baseline (ratio + absolute ceiling)"` **with the verdict lines pasted in the body.** If FAIL: stop, do not tune the predicate, do not commit the verdict; profile, fix (source fixes are ordinary task commits), re-run --candidate against the unchanged committed baseline; the predicate changes only via a design amendment.
- [ ] **Step 4: Write bench/README.md** (own commit, staged set exactly {`engine/bench/README.md`}) documenting the methodology (identity capture + refusal rules, warmup separate from ≥ 10 measured runs, seeds, flags, cpu capture, source→baseline→verdict commit topology, fixture/corpus env vars) in full sentences on their own lines.

---

### Task 13: Acceptance truth table and completion evidence (PR8)

- [ ] **Step 1: Full clean build + ctest with ALL honesty overrides UNSET** (`UTTT_ALLOW_MISSING_FIXTURES`, `UTTT_ALLOW_MISSING_CORPUS`).
- [ ] **Step 2: Produce the acceptance TRUTH TABLE — one row per DD §13 criterion, each row exactly one of `green-E2` (artifact present, consumed, suite green; cite the ctest/bench output) or `pending-blocked` (owner artifact absent; cite the exact absence command + output). Overridden, skipped, or copied-example runs may NEVER be summarized as green-E2:**

```text
criterion 1 (theory fixtures)        -> green-E2 | pending-blocked (ls theory/fixtures output)
criterion 2 (properties/table/perft) -> green-E2 (no owner dependency)
criterion 3 (adapter + corpus)       -> green-E2 | pending-blocked (ls docs/protocol output)
criterion 4 (benchmark predicate)    -> green-E2 (committed engine/bench/candidate-verdict.json vs the earlier-committed baseline)
criterion 5                          -> deferred by design (theory Stage-1 + successor DD)
```

- [ ] **Step 3: Commit any stragglers; produce the IMPL report relay per protocol carrying the truth table verbatim, with ACTIONS_GIT_REF `engine/rules-core-c1@<sha>`. A report claiming completion while any row is pending-blocked must say so in its first line.**

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
- Any file outside `engine/` (the sprint relay tree excepted for the report relay).
- (The injected-clock seam is IN scope — Task 2 — per the locked DD §7; the earlier deferral was a design deviation and is withdrawn per PLAN-REVIEW PR4.)

## Anti-half-fix guards

- The exhaustive table proof and the randomized cross-implementation properties may not be weakened (no sampling the 19,683, no reducing N below 2,000 games) without a design amendment.
- A failing fixture is never skipped ad hoc: it either reveals an engine bug (fix), or a fixture/schema question (blocker relay to the orchestrator for theory routing).
- The benchmark predicate is immutable at PLAN level (design amendment required); a FAIL is a performance bug, not a threshold negotiation.
- No placeholder-policy cleverness: the placeholder stays trivially-legal-and-dumb so search-strength claims cannot leak in early.

## Verification target

E2 per criterion via the Task 13 truth table (PR8): criteria 2 and 4 are executable now with no owner dependency; criteria 1 and 3 are green-E2 only when the theory fixtures and the harness conformance corpus are respectively present and consumed — otherwise they are reported pending-blocked with absence evidence, never summarized as complete; criterion 5 explicitly deferred (theory Stage-1 + successor DD).

## Operator-judgment items

None. All choices herein are locked-design consequences or mechanical.
