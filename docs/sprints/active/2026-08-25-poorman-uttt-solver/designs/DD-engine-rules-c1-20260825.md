# DD-engine-rules-c1-20260825 — Engine rules core, representation, adapter boundary, and test seam

DESIGN_DOC_ID: DD-engine-rules-c1-20260825
Status: awaiting Implementer DESIGN-REVIEW.
Author: engine.planner.
Lock boundary: this document is the COMPLETE lock surface for the engine rules core, representation, adapter boundary, and test seam.
Search math (backup operator implementation, cutoff/bound derivation, TT entry contract, selective-widening soundness) is NOT part of this document or its lock; it arrives as a successor DESIGN_DOC_ID after theory's findings and Stage-1 solver land, with its own grill delta, DESIGN-REVIEW, and lock (orchestrator amendment, `.relays/s1/engine-c1/DESIGN-orchestrator-planner-20260825-073330.md`).
A non-normative working draft of that gated material is parked at `../notes/engine-gated-math-draft-20260825.md`; it is not a design record and grants nothing.
Dispatch lineage: `.relays/s1/engine-c1/DESIGN-orchestrator-planner-20260825-065713.md` (GRILL_REQUIRED: yes — grill run, GRILL_LOCK folded in below) as amended by `DESIGN-orchestrator-planner-20260825-073330.md` (split ruling, option a).
Inputs: design spec `docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md` @ main 2aef375 (rulings R1–R4 baked in; tie-state paragraph amended to the actual-mover transition); RECONCILE.md section c1-audits as corrected by `.relays/s1/reconcile-c1/RECONCILE-orchestrator-reviewer-20260825-070120.md`; both engine-c1 audit returns.
Supersedes: the withdrawn single-document draft DD-engine-c1-20260825 (never reviewed; removed in the same commit that adds this file).

## 1. Scope

Locked by this document: Position representation, local lookup table, move encoding and generation, terminal detection, Zobrist key inputs and collision policy, protocol adapter boundary, budget seam, test seam, toolchain, repository surface (`engine/`), and the value-quality metadata SHAPE.
Out of scope (successor DD, theory-gated): backup operator implementation, cutoff/bound math, TT entry/value semantics, widening policy.
Out of scope (other owners): protocol schema content (harness), fixture schema content (theory), ML evaluator, multithreading.

## 2. Position representation (evidence E2)

Decision: per-player `std::array<uint16_t, 9>` cell masks, plus cached derived state.

```c++
struct Position {
  std::array<uint16_t, 9> x, o;     // bit c = cell c of local board b
  std::array<uint32_t, 9> tern;     // incremental ternary code per local board (index into LocalTable)
  uint16_t macro_x, macro_o;        // locals won per player
  uint16_t closed;                  // locals closed (won or full)
  int8_t   forced;                  // 0..8, or ANY = -1
  TieState tie;                     // X, O, or NULL_FIRST_MOVE (R1)
};
```

Semantics are value-copy immutable: search copies the parent and applies one move; there is no make/unmake.
`tie` is the deterministic tie owner; the transition is `tie' = opponent(actual mover)` (spec as amended @ 2aef375); `NULL_FIRST_MOVE` is valid only before the first mark (R1).
Budgets never enter Position; they live in a root-side `BudgetContext` (section 9).

Evidence (E2, microbenchmark, Homebrew clang 22.1.8 `-O3 -std=c++2c`, Apple M5 Pro, 2026-08-25):
this layout and a `__uint128_t`-per-player layout are indistinguishable — both `sizeof == 80` with the shared caches, random-playout cost 23–24 ns/ply including movegen + value copy + apply + table lookup (~42M plies/s single-thread), pure copy fully vectorized (≈0), free-choice movegen over 81 cells ≈ 26 ns.
Decision therefore falls to simplicity: direct per-local indexing, no 128-bit shift/extract code, natural fixture/debug reads.
Benchmark source was throwaway design evidence at `/tmp/uttt_bench/bench.cpp`; methodology and numbers are recorded here and are trivially reproducible from this section.
The 30 s/move budget is confirmed non-binding for the rules core; search strength will be bounded by tree shape and cutoff math (successor DD), not representation.

## 3. Local lookup table

One global table of 19,683 entries (3^9 local-board states), indexed by the incrementally maintained ternary code `tern[b]` (cell c contributes `{empty:0, X:1, O:2} * 3^c`).
Each entry packs into one `uint64_t` (total 157 KB, L2-resident):

- `status` (2 bits): open / X-won / O-won / full-draw.
- `empties` (9 bits): empty-cell mask.
- `win_x`, `win_o` (9 bits each): cells completing a local three-in-a-row for that player now (immediate wins; doubles as the opponent's block mask).
- `fork_x`, `fork_o` (9 bits each): cells creating two-or-more simultaneous local threats for that player.

Rationale: the same lookup that closes boards feeds move ordering (macro wins, local wins, blocks, forks) without per-node recomputation.
Cells of closed boards never change, so `tern[b]` of a closed board is frozen and its entry stays valid.

## 4. Moves, movegen, terminal detection

Move = `{uint8_t board, uint8_t cell}`; move lists are fixed `std::array<Move, 81>` with a count, no heap in the search path.
Movegen: if `forced != ANY` and that board is open, enumerate `empties` of that board; otherwise enumerate `empties` of every open board.
Apply-move updates, in order: cell mask, `tern[b]`, table lookup; on close: `closed`/`macro_*`; then mover macro-win check (win masks over `macro_*`); then all-closed check; then `forced` = target cell's board, remapped to ANY if that board is closed; then `tie = opponent(mover)`.
Terminal kinds exposed: `none`, `macro_win(player)`, `all_closed`.
`all_closed` resolution (chip comparison, ½–½ on exact equality) happens OUTSIDE Position, in the layer that owns budgets; Position never answers "who won a chip-count draw".
The first move is forced into the center local board (`forced = 4` in the initial state) per canonical rule 5.

## 5. Zobrist key inputs and collision policy

64-bit Zobrist over exactly the state that identifies a search node: 81 cells × 2 players (162 randoms), forced state (10 randoms: 0..8, ANY), tie state (3 randoms: X, O, NULL_FIRST_MOVE).
This matches the spec's budget-independence claim for `T(s, h)`; if theory's findings overturn budget independence, the key gains budget inputs and the successor DD addresses it — TT VALUE semantics are entirely successor-DD territory.
Collision policy: play mode stores a 32-bit secondary verification tag per entry (different Zobrist fold); acceptance/fixture mode runs with full-key verification (complete Position compared on probe) so oracle-equality tests can never be polluted by a silent collision.

## 6. Protocol adapter boundary

The adapter is a separate translation unit/library; the search core has zero JSON and zero I/O dependencies.
JSON handling uses a vendored, pinned single-header library, linked ONLY into the adapter (operator decision, 2026-08-25; candidate and license check at PLAN).
Flow: read one JSONL line → strict parse → validate (schema version, board shape, coordinate ranges, budget ranges per R3, `last_mover` consistency, forced-board consistency) → canonicalize into Position + BudgetContext → engine → serialize `{bid, move}` → single line out, flushed.
stdout carries protocol lines only; all diagnostics go to stderr.
The engine is stateless per request, which satisfies R2's re-request semantics structurally; a re-requested auction is just another request.
Bit-identical replies on re-request are NOT promised (the time budget makes search depth wall-clock-dependent); R2 requires a legal reply, not a reproducible one.
Malformed input, out-of-range values, or EOF produce a diagnostic on stderr and (for malformed requests) no stdout line — faulting is the referee's judgment, not the engine's.
The schema itself is harness-owned and consumed as-is; any change need routes through s1.orchestrator-planner.

## 7. Test seam

- Fixture ingestion: a test-only reader consumes theory's fixture schema verbatim (theory publishes the schema section early; engine requests it via the orchestrator if absent when needed). Fixtures drive: legal-move sets, closure/forced/ANY routing, terminal outcomes, and (small cases) expected thresholds and critical bids.
- Property tests: movegen equals a naive 81-cell reference implementation on random positions; closed boards never accept marks; forced-routing including closed-board→ANY; `tern[b]` always equals recomputation from masks; macro/closed caches always equal recomputation.
- Perft-style move counts at fixed depths from named positions, pinned as regression values.
- Deterministic clock: search takes an injected clock interface; tests use a fake clock, so soft-stop logic is testable without wall time.
- Threshold-equality tests vs theory's Stage-1 oracle use |T_engine − T_exact| ≤ 1e-9 on T (±1 fixed-point unit of the 10^9 combined-budget scale; operator decision, 2026-08-25), run in full-key TT verification mode (section 5). This tolerance is an ENGINE-SIDE ACCEPTANCE-TEST PARAMETER ONLY: the authoritative `p = T` classification and the approximation-tolerance/exact-fallback contract are theory-owned obligations (orchestrator amendment note, per reviewer correction 1); if theory's contract lands stricter or shaped differently, theory wins and this parameter is re-derived from it.

## 8. Toolchain and repo surface

Homebrew LLVM 22.1.8 (`/opt/homebrew/opt/llvm`) primary; `-std=c++2c`; CMake + Ninja; feature-tested library facilities only (`std::simd` and `<inplace_vector>` are absent here — E2-probed; NEON intrinsics or plain uint64 bit-ops only if profiling later justifies SIMD).
Single-threaded, deterministic-given-clock search first; concurrency is a later, separately designed step.
Surface: `engine/` per the roadmap; layout details (targets, test framework) are PLAN decisions.
License gate (standing): nelhage/ultimattt and TheGustafson/ai-ultimate-tictactoe are reference-study only; no code reuse without fresh orchestrator/operator authorization.

## 9. Budget plumbing (seam only)

`BudgetContext { int64 own_units, opp_units }` lives beside — never inside — Position.
Search maps `(Position, tie) → T` (and analysis metadata); the root layer alone combines T with BudgetContext to produce the integer bid per R3/R4.
Unsigned/64-bit-safe arithmetic for sums and products of unit counts (`__int128` intermediates where products demand it).
The interior of the search (how T is computed and pruned) is successor-DD territory.

## 10. Value-quality metadata shape (cross-domain contract)

Locked shape (operator decision 2026-08-25; consumed by harness game log + ui analysis view; the CARRIER is harness-owned — this section defines engine-side semantics only):

- `quality`: one of `exact` | `bound` | `estimate`; `exact` only when full-width search reached terminal resolution.
- `lo`, `hi`: present when `quality = bound`.
- `depth`: completed-iteration search depth for the reported value.
- `complete`: whether the reported iteration finished before the clock.

A value from any node whose move set was not exhausted can never be reported `exact`.
Iterative deepening publishes only the last fully completed iteration (implementer-audit recommendation, adopted).
The routing note for the carrier goes to s1.orchestrator-planner with this design's completion report.

## 11. GRILL_LOCK

GRILL_LOCK_ID: GRILL-engine-c1-20260825
GRILL_REQUIRED: yes
GRILL_SOURCE:
- plan/design/audit relay read: DESIGN dispatch 065713 + amendment 073330; both engine-c1 AUDIT returns; RECONCILE.md c1-audits + reviewer corrections 070120; design spec @ 2aef375
- code/docs inspected: greenfield repo; /tmp layout microbenchmark (E2); toolchain probes from AUDIT (E2)
- questions answered from codebase: Position layout (benchmark: layouts equal → simplicity wins); copy-vs-make/unmake (copy ≈ free at 80 B); table width (uint64 packing fits 47 bits); R2 idempotence (spec text: re-request is a fresh stateless request; bit-identical replies not required); TT verification need (acceptance criteria demand oracle-grade exactness → full-key mode in tests)
- questions asked operator: 4 (design-direction approval; JSON dependency; value-quality metadata shape; oracle-match tolerance)

Resolved decisions:
- Position layout — array<uint16_t,9> per player + caches — benchmark-equal to uint128, simpler — source code (E2 benchmark)
- JSON in adapter — vendored pinned single-header lib, adapter-only — robustness at a load-bearing fault boundary (R2) — source operator
- Value-quality metadata — quality + [lo,hi] + depth + complete flag — honest analysis without over-claiming; ui/harness consumable — source operator
- Oracle-match tolerance — ±1 fixed-point unit (1e-9 on T) as an engine-side acceptance-test parameter only; authoritative p=T and tolerance/fallback contracts remain theory-owned — source operator (scoped by orchestrator amendment 073330)
- TT collision policy — 64-bit Zobrist + 32-bit tag in play; full-key verify in acceptance tests — acceptance criteria demand exactness — source default (recorded, unchallenged)
- Value copy over make/unmake — benchmark — source code (E2)

Rejected alternatives:
- __uint128_t masks — no measured win; extraction complexity — rejected on evidence
- Hand-rolled JSONL parser — edge-case burden at the fault boundary — rejected by operator
- quality-enum-only and no-metadata analysis — degrade the named analysis-view goal — rejected by operator
- Exact-double and 1e-6 tolerances — brittle vs masking, respectively — rejected by operator
- Single mixed-lock design document — mechanically ambiguous lock boundary — rejected by orchestrator per reviewer correction 5

Still operator-owned:
- none for this lock; ui-facing display choices are batched at ui DESIGN per RECONCILE; p=T semantics are theory-owned, not operator-open here

Design-lock impact:
- Sections 2–10 are the lock surface this GRILL_LOCK feeds; the successor gated-math DD runs its own grill delta if new decisions arise.

## 12. Boundary contracts

Writes: engine analysis metadata semantics (section 10) — carrier harness-owned; routing note to orchestrator.
Reads: harness protocol schema v1 (consumer, as-is); theory fixture schema v1 (consumer, verbatim); theory findings doc (gates the successor DD, not this one).
Target entity: `engine/` rules core + adapter + tests conforming to this document.
Downstream consumer: referee (spawns engine), ui (analysis view), theory (cross-validation).
Contract: JSONL per harness spec; fixtures per theory spec; tolerance per section 7 (theory-subordinated).
Proof: E2 fixture/property/perft tests; E2 oracle-equality at tolerance (once Stage-1 lands); E3 referee round-trip later.
No-consumer action: not applicable — consumers named.

## 13. Acceptance criteria (for the eventual rules-core PLAN)

1. All theory fixtures pass (legal moves, closures, routing, terminals) — E2.
2. Property tests and pinned perft counts pass — E2.
3. Adapter round-trips the harness conformance corpus; stdout discipline verified — E2.
4. Rules-core microbench regression guard: playout ≤ 2× the 24 ns/ply design-evidence number on this laptop — E2.
5. Oracle threshold equality within section 7's parameter on every Stage-1-reachable position — E2 — listed for continuity but EXECUTABLE only after theory's Stage-1 and the successor search DD land; it does not gate the rules-core PLAN.

## 14. Risks

- Theory could overturn budget independence or reshape zugzwang: Zobrist inputs reopen via the successor DD; contained because no value semantics are locked here.
- Vendored JSON lib license must be verified before vendoring (PLAN step).
- The fork-mask definition (section 3) has edge cases (double-threat counting on near-full boards); property tests against a naive recomputation are the guard.
