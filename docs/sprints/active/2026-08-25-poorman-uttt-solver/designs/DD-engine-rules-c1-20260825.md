# DD-engine-rules-c1-20260825 — Engine rules core, representation, adapter boundary, and test seam

DESIGN_DOC_ID: DD-engine-rules-c1-20260825
Status: revision 3, awaiting successor Implementer DESIGN-REVIEW.
Revision 3 (2026-08-25) folds review `.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-081432.md` and the orchestrator boundary routing `.relays/s1/engine-c1/SITREP-orchestrator-planner-20260825-082114.md`: MR1 mechanical local-reachability rule replacing the dual-winner shorthand (§3, §7); MR2 grill ledger made internally truthful (48-bit payload, collapsible copy probe, make/unmake never measured) and non-self-referential baseline ordering rule (§2, §11, §13); consistency: RootContext naming unified (§2); §6 now consumes the LOCKED harness schema as-is at its approved digest, with the addressed alignment routing satisfying the pre-PLAN acknowledgment gate (§6, §12).
Revision 2 (2026-08-25) folds all five must-revise findings from `.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-075718.md`: M1 adapter narrowed to an abstract boundary (§6, §9 RootContext); M2 total terminal legality (§4); M3 table validity bit + mechanical fork definition + exhaustive proof (§3, §7); M4 lifecycle/Zobrist/budget-terminal/perft test obligations and the Position→T notation fix (§7, §9); M5 benchmark evidence rescoped to directional, durable baseline moved to a committed PLAN deliverable (§2, §13).
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
Budgets never enter Position; they live in the root-side `RootContext` (section 9).

Evidence (E2, DIRECTIONAL microbenchmark, Homebrew clang 22.1.8 `-O3 -std=c++2c`, Apple M5 Pro, 2026-08-25):
this layout and a `__uint128_t`-per-player layout are indistinguishable on the random-playout probe — both `sizeof == 80` with the shared caches, 23–24 ns/ply including movegen + value copy + apply + table lookup (~42M plies/s single-thread).
Evidence scope correction (must-revise M5): the throwaway bench's pure-copy and repeated-movegen probes were optimizable (the copy loop's state perturbation had identical conditional arms, permitting collapse), so no "copy ≈ 0" claim is made; the playout number is directional layout-comparison evidence only, not a durable baseline.
Decision (stands on the playout comparison plus simplicity): direct per-local indexing, no 128-bit shift/extract code, natural fixture/debug reads.
Durable performance evidence is a PLAN deliverable with an explicit ordering rule (must-revise MR2): the committed benchmark harness under `engine/` (dead-code-elimination guards such as `DoNotOptimize`/volatile sinks; documented warmup/iteration counts, compiler flags, CPU state) first measures a FIXED REFERENCE PATH — the naive 81-cell reference implementation the property tests already require — and that measurement is stored as a named baseline artifact (value + environment) BEFORE the optimized implementation under test is evaluated; the acceptance criterion compares candidate to that stored artifact. Measuring the candidate and calling the same measurement its own baseline is tautological and prohibited.
The 30 s/move budget is still assessed non-binding for the rules core (directional evidence + prior art); search strength will be bounded by tree shape and cutoff math (successor DD), not representation.

## 3. Local lookup table

One global table of 19,683 entries (3^9 local-board states), indexed by the incrementally maintained ternary code `tern[b]` (cell c contributes `{empty:0, X:1, O:2} * 3^c`).
Each entry packs into one `uint64_t` (total 157 KB, L2-resident):

- `valid` (1 bit): 1 iff the assignment is LOCALLY REACHABLE under immediate closure, defined mechanically (must-revise MR1): an open or full-draw board is reachable iff it contains NO completed line; a won board is reachable iff exactly one player holds completed lines AND at least one of that player's marks lies on EVERY completed line and its removal leaves no completed line (the closing mark completed all existing lines simultaneously — a board closes at its first completed line, so nothing is marked afterward). This excludes dual-winner boards AND same-player multi-line boards with no common closing mark (e.g. two disjoint X rows). Sufficiency holds because line-completion is monotone in the mark set and bidding lets either player mark consecutively, so any prefix order of a reachable board avoids premature completion. Invalid entries carry defined bytes (`status = full-draw` convention, masks zeroed) but are never consulted by play; the adapter's import validation rejects any external position whose local board maps to an invalid entry.
- `status` (2 bits): open / X-won / O-won / full-draw.
- `empties` (9 bits): empty-cell mask.
- `win_x`, `win_o` (9 bits each): cells completing a local three-in-a-row for that player now (immediate wins; doubles as the opponent's block mask).
- `fork_x`, `fork_o` (9 bits each), defined mechanically (must-revise M3): cell c is a fork cell for player P iff c is empty, placing P at c does NOT complete a line (that cell is `win_p`, not fork), and in the resulting board the set of cells that would then be immediate wins for P has size ≥ 2 (distinct cells, regardless of how many lines each completes).

Rationale: the same lookup that closes boards feeds move ordering (macro wins, local wins, blocks, forks) without per-node recomputation.
Cells of closed boards never change, so `tern[b]` of a closed board is frozen and its entry stays valid.
Table proof obligation: an exhaustive test compares ALL 19,683 entries — validity (per the MR1 reachability rule, independently reimplemented), status, empties, win masks, fork masks — against an independent naive evaluator (section 7), with named fixtures for both invalidity classes: dual-winner boards and same-player disjoint-line boards.
Packed entry width is 48 bits (valid 1 + status 2 + empties 9 + win masks 18 + fork masks 18), comfortably inside the uint64 entry.

## 4. Moves, movegen, terminal detection

Move = `{uint8_t board, uint8_t cell}`; move lists are fixed `std::array<Move, 81>` with a count, no heap in the search path.
Movegen: if `forced != ANY` and that board is open, enumerate `empties` of that board; otherwise enumerate `empties` of every open board.
Apply-move updates, in order: cell mask, `tern[b]`, table lookup; on close: `closed`/`macro_*`; then mover macro-win check (win masks over `macro_*`); then all-closed check; then `forced` = target cell's board, remapped to ANY if that board is closed; then `tie = opponent(mover)`.
Terminal kinds exposed: `none`, `macro_win(player)`, `all_closed`.
Terminal legality is total (must-revise M2): `legal_moves(p) = ∅` for BOTH terminal kinds — a macro win ends the game even with open cells remaining — and `apply` rejects every move on a terminal parent as a checked precondition, without mutating the parent.
`all_closed` resolution (chip comparison, ½–½ on exact equality) happens OUTSIDE Position, in the layer that owns budgets; Position never answers "who won a chip-count draw".
The first move is forced into the center local board (`forced = 4` in the initial state) per canonical rule 5.

## 5. Zobrist key inputs and collision policy

64-bit Zobrist over exactly the state that identifies a search node: 81 cells × 2 players (162 randoms), forced state (10 randoms: 0..8, ANY), tie state (3 randoms: X, O, NULL_FIRST_MOVE).
This matches the spec's budget-independence claim for `T(s, h)`; if theory's findings overturn budget independence, the key gains budget inputs and the successor DD addresses it — TT VALUE semantics are entirely successor-DD territory.
Collision policy: play mode stores a 32-bit secondary verification tag per entry (different Zobrist fold); acceptance/fixture mode runs with full-key verification so oracle-equality tests can never be polluted by a silent collision.
Full-key verification is a FIELDWISE comparison over the identity-bearing Position fields (`x`, `o`, `forced`, `tie`) — never a padding-sensitive `memcmp`, and never over derived caches (`tern`, `macro_*`, `closed`) unless cache consistency has been separately verified (reviewer clarification, adopted).

## 6. Protocol adapter boundary (abstract boundary only; concrete wire bytes deferred)

The owner schema is now design-locked and this section consumes it AS-IS: DD-harness-c1-20260825, Implementer-APPROVED at digest 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440 (`.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-081315.md`), routed to this consumer by the addressed orchestrator alignment relay `.relays/s1/engine-c1/SITREP-orchestrator-planner-20260825-082114.md` — that routing satisfies the pre-PLAN owner/consumer acknowledgment gate named on both sides (must-revise M1; consistency correction, review 081432).
Concrete consumption obligations, from the approved schema: hello/turn/game_end message envelopes (not bare per-line requests); canonical X/O marks and budget keys with seat delivery via `you` (engine derives mine/theirs once); explicit `tie_owner` field — `last_mover` does not exist on the wire; `request_id` echo in every turn reply; the request's complete `legal` move list cross-checked against own movegen (divergence is a diagnostic + fail-closed condition); reply `move` always a member of `legal`; the optional `info` object carries section 10's value-quality metadata and is logged verbatim by the referee.
Schema questions route to harness.planner via s1.orchestrator-planner; the schema is not this pair's to change.
Remaining PLAN-time work is implementation and the conformance corpus, not contract shape.

Locked boundary properties (schema-independent):
- The adapter is a separate translation unit/library; the search core has zero JSON and zero I/O dependencies.
- JSON handling uses a vendored, pinned single-header library, linked ONLY into the adapter (operator decision, 2026-08-25; candidate and license check at PLAN).
- Flow shape: read one protocol line → strict parse → validate against the owner schema of record (fail-closed on missing/type-invalid required keys; unknown keys ignored per the owner's forward-compatibility rule) → canonicalize into Position + RootContext (section 9) → engine → serialize the owner-schema reply → single line out, flushed.
- Validation includes internal-consistency checks on the imported state: board shape and coordinate ranges, budget ranges per R3, forced-board consistency, tie-state consistency (NULL only at ply 0), and rejection of invalid local-board states per section 3's validity bit.
- stdout carries protocol lines only; all diagnostics go to stderr.
- The engine is stateless per request, which satisfies R2's re-request semantics structurally; a re-requested auction is just another request.
- Bit-identical replies on re-request are NOT promised (the time budget makes search depth wall-clock-dependent); R2 requires a legal reply, not a reproducible one.
- Malformed input, out-of-range values, or EOF produce a diagnostic on stderr and (for malformed requests) no stdout line — faulting is the referee's judgment, not the engine's.
- The engine's value-quality metadata (section 10) rides the owner schema's optional analysis carrier (`info` in the current harness draft); the metadata SEMANTICS are engine-owned, the carrier is harness-owned.

## 7. Test seam

- Fixture ingestion: a test-only reader consumes theory's fixture schema verbatim (theory publishes the schema section early; engine requests it via the orchestrator if absent when needed). Fixtures drive: legal-move sets, closure/forced/ANY routing, terminal outcomes, and (small cases) expected thresholds and critical bids.
- Property tests: movegen equals a naive 81-cell reference implementation on random positions; closed boards never accept marks; forced-routing including closed-board→ANY; `tern[b]` always equals recomputation from masks; macro/closed caches always equal recomputation.
- Table proof (M3/MR1): exhaustive comparison of all 19,683 LocalTable entries (validity per the section 3 reachability rule, status, empties, win masks, fork masks) against an independent naive evaluator that reimplements the reachability rule from its definition; named fixtures for dual-winner and same-player disjoint-line invalid boards.
- Terminal and rejection tests (M2): named fixtures proving `legal_moves = ∅` at both terminal kinds (macro win with open cells remaining; all-closed), and apply-rejection without parent mutation for post-terminal, occupied-cell, closed-board, out-of-range, and wrong-forced-board moves; adapter rejection fixtures for invalid local states and inconsistent caches/closure/forced/tie imports.
- State-lifecycle tests (M4): initial state is `forced = 4` with `tie = NULL_FIRST_MOVE`; `tie' = opponent(actual mover)` including consecutive marks by the same player; NULL_FIRST_MOVE is gone after the first mark and never reappears; parent Position is byte-identical across identity fields after any child apply (value-copy immutability).
- Zobrist completeness (M4): the incrementally maintained key equals full recomputation on random positions, and changing ANY single key input (any cell, forced state, tie state) changes the key.
- Budget-layer terminal outcomes (M4): all-closed resolution tested for X-relative chip margins positive, negative, and exactly zero (½–½).
- Perft, defined unambiguously (M4): perft is a BOTH-MOVER expansion matching the search shape — from each position, every legal cell is expanded twice, once as an X mark and once as an O mark (2× branching), with terminal cutoffs; counts at fixed depths from named positions are pinned as regression values. A single-mover schedule is NOT assumed anywhere, because under bidding either player can mark consecutively.
- Deterministic clock: search takes an injected clock interface; tests use a fake clock, so soft-stop logic is testable without wall time.
- Threshold-equality tests vs theory's Stage-1 oracle use |T_engine − T_exact| ≤ 1e-9 on T (±1 fixed-point unit of the 10^9 combined-budget scale; operator decision, 2026-08-25), run in full-key TT verification mode (section 5). This tolerance is an ENGINE-SIDE ACCEPTANCE-TEST PARAMETER ONLY: the authoritative `p = T` classification and the approximation-tolerance/exact-fallback contract are theory-owned obligations (orchestrator amendment note, per reviewer correction 1); if theory's contract lands stricter or shaped differently, theory wins and this parameter is re-derived from it.

## 8. Toolchain and repo surface

Homebrew LLVM 22.1.8 (`/opt/homebrew/opt/llvm`) primary; `-std=c++2c`; CMake + Ninja; feature-tested library facilities only (`std::simd` and `<inplace_vector>` are absent here — E2-probed; NEON intrinsics or plain uint64 bit-ops only if profiling later justifies SIMD).
Single-threaded, deterministic-given-clock search first; concurrency is a later, separately designed step.
Surface: `engine/` per the roadmap; layout details (targets, test framework) are PLAN decisions.
License gate (standing): nelhage/ultimattt and TheGustafson/ai-ultimate-tictactoe are reference-study only; no code reuse without fresh orchestrator/operator authorization.

## 9. Root context and budget plumbing (seam only)

`RootContext { Seat seat /* X or O, from the owner schema's `you` */; int64 budget_x, budget_o /* canonical X/O units */ }` lives beside — never inside — Position (M1: the harness delivers canonical X/O budgets plus a seat; the engine derives own/theirs itself, killing the perspective-flip bug class).
Search maps `Position → T` (tie state is inside Position; the earlier `(Position, tie)` notation was redundant — M4); T is X's critical fraction of the combined budget, exactly as the spec defines it, regardless of which seat this engine holds.
The root layer alone combines T with RootContext — converting to own-seat perspective via `seat` — to produce the integer bid per R3/R4, plus the analysis metadata of section 10.
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
- questions answered from codebase: Position layout (playout benchmark: the two copy-based layouts directionally indistinguishable → simplicity selected the array layout; the isolated copy probe was collapsible and proves nothing — MR2 correction); table width (uint64 packing fits the 48-bit payload: valid 1 + status 2 + empties 9 + win 18 + fork 18 — MR2 correction); R2 idempotence (spec text: re-request is a fresh stateless request; bit-identical replies not required); TT verification need (acceptance criteria demand oracle-grade exactness → full-key mode in tests)
- questions asked operator: 4 (design-direction approval; JSON dependency; value-quality metadata shape; oracle-match tolerance)

Resolved decisions:
- Position layout — array<uint16_t,9> per player + caches — playout-benchmark directionally equal to uint128, simpler — source code (E2 directional benchmark)
- JSON in adapter — vendored pinned single-header lib, adapter-only — robustness at a load-bearing fault boundary (R2) — source operator
- Value-quality metadata — quality + [lo,hi] + depth + complete flag — honest analysis without over-claiming; ui/harness consumable — source operator
- Oracle-match tolerance — ±1 fixed-point unit (1e-9 on T) as an engine-side acceptance-test parameter only; authoritative p=T and tolerance/fallback contracts remain theory-owned — source operator (scoped by orchestrator amendment 073330)
- TT collision policy — 64-bit Zobrist + 32-bit tag in play; full-key verify in acceptance tests — acceptance criteria demand exactness — source default (recorded, unchallenged)
- Value copy over make/unmake — value-copy chosen for contract simplicity; the playout benchmark (whose per-ply path includes a full Position copy) showed no representation bottleneck at 23–24 ns/ply; make/unmake was NEVER measured and no comparative claim is made (MR2 correction) — source code (E2 directional) + default

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
Reads: harness protocol schema v1 — the APPROVED owner record DD-harness-c1-20260825 @ digest 11ac4efc85…7890440 (consumer, as-is; concrete adapter acknowledgment orchestrator-routed pre-PLAN); theory fixture schema v1 (consumer, verbatim); theory findings doc (gates the successor DD, not this one).
Target entity: `engine/` rules core + adapter + tests conforming to this document.
Downstream consumer: referee (spawns engine), ui (analysis view), theory (cross-validation).
Contract: JSONL per harness spec; fixtures per theory spec; tolerance per section 7 (theory-subordinated).
Proof: E2 fixture/property/perft tests; E2 oracle-equality at tolerance (once Stage-1 lands); E3 referee round-trip later.
No-consumer action: not applicable — consumers named.

## 13. Acceptance criteria (for the eventual rules-core PLAN)

1. All theory fixtures pass (legal moves, closures, routing, terminals) — E2.
2. Property tests, the exhaustive 19,683-entry table proof, terminal/rejection/lifecycle/Zobrist tests, and pinned both-mover perft counts pass — E2.
3. Adapter round-trips the harness conformance corpus (against the APPROVED owner schema, per section 6); stdout discipline verified — E2.
4. Rules-core microbench regression guard: candidate playout compared against the NAMED BASELINE ARTIFACT produced per section 2's ordering rule (fixed reference path measured first by the committed harness; artifact stores value + environment; the /tmp design-evidence number is not the baseline and the candidate is never its own baseline) — E2.
5. Oracle threshold equality within section 7's parameter on every Stage-1-reachable position — E2 — listed for continuity but EXECUTABLE only after theory's Stage-1 and the successor search DD land; it does not gate the rules-core PLAN.

## 14. Risks

- Theory could overturn budget independence or reshape zugzwang: Zobrist inputs reopen via the successor DD; contained because no value semantics are locked here.
- Vendored JSON lib license must be verified before vendoring (PLAN step).
- The fork-mask definition (section 3) has edge cases (double-threat counting on near-full boards); the exhaustive 19,683-entry naive-evaluator comparison in section 7 is the guard.
- The adapter consumes the approved harness schema at a pinned digest (section 6); if the owner schema is later amended, this document's §6 requires a matching supplement/revision with its own review — the risk is bounded by the pin, not open-ended.
