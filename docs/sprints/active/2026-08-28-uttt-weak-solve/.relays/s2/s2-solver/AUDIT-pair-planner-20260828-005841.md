ROLE: Pair Planner
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: s2-solver-c1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — audit return; reconciliation and next-phase authority sit with the orchestrator
FROM: solver.planner
TO: s2.orchestrator-planner
CC: s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/AUDIT-orchestrator-planner-20260828-001407.md
SUBJECT: AUDIT return s2-solver-c1 — rules core reuses clean; AltSolver is an oracle, not the solver; full-width wall at ~ply 11–12; proof-search architecture required

## Independence note

Filed without reading solver.implementer's return (s2-solver/AUDIT-pair-implementer-20260828-003733.md, visible in git status but not opened by this seat).

## Item 1 — Rules-core conformance to the s2 rule lock (E1, item by item)

Verdict: the s1 rules core implements the rule-lock game exactly when budgets are stripped. No factoring needed; the reject/narrow gate does not fire.

1. Macro grid, X/O alternation, X first: `Position` holds 9 local bitboard pairs (engine/src/core/position.hpp:22-23). The core is mover-agnostic (`applied(mv, Seat)` position.hpp:34); alternation is enforced by the caller — AltSolver flips the successor seat each ply (engine/src/root/alt_solver.hpp:173). "X first" is the caller convention `solve(initial, Tie::X)`. Expressible exactly; nothing to factor.
2. Center-forced first move: `Position::initial()` leaves `forced = 4` (position.hpp:27); legal_moves under a forced board appends only that board's empty cells (engine/src/core/position.cpp:45-47). Fixture anchor: `legality-forced-center-opening` (theory/fixtures/legality.json).
3. Sent-to-closed → any open cell: `applied` sets `next.forced = closed(cell) ? kForcedAny : cell` (position.cpp:79-81); `legal_moves` with `kForcedAny` iterates all open boards (position.cpp:49-51). Fixtures: `routing-send-to-closed-free-choice`, `routing-self-closing-send-free-choice` (theory/fixtures/closure_routing.json).
4. Closed = won-or-full, cells dead: `LocalTable::status` classifies each 3^9 local code (engine/src/core/local_table.hpp:18); `applied` sets closed/macro bits on close (position.cpp:73-78), rejects plays into closed boards (position.cpp:60), and `legal_moves` skips closed boards (position.cpp:50).
5. Macro three-in-a-row wins outright: `terminal()` checks macro lines before all-closed (position.cpp:28-33). Fixture: `closure-double-line-macro-precedes-all-closed`.
6. All-closed with no macro winner = draw: `terminal()` returns `AllClosed` at closed==0x1FF (position.cpp:31); AltSolver maps it to value 0.0 = draw (alt_solver.hpp:160-162), matching the auction game's equal-chips ½–½ at zero budgets.

Auction couplings found, none inside the rules kernel: budgets live in core/budget.hpp but are consumed only by adapter/root/tests (`grep -rln "budget.hpp\|clock.hpp" engine/src engine/tests` → adapter/wire.hpp, adapter/policy.hpp, adapter/main.cpp, tests only). Residual relics: `TieState::NullFirstMove` as the initial `tie` value (position.hpp:28) — harmless at the unique root, deterministic thereafter (position.cpp:82); the `GameModel` concept requires auction-specific `chip_sign` (engine/src/search/game_model.hpp:50) — trivially satisfiable, unused by an alternation solver; `Search` (search/search.hpp) is threshold-interval math and is not needed at all for the weak solve.

4-bucket (item 1):
PRIMARY_BUCKET: already-closed
still-open: none — every rule-lock behavior has an implementing code path cited above.
already-closed: the full rules kernel (position, local_table, types, zobrist) — reuse as-is, do not rebuild.
product-overlapped: none.
recommended-next: promote/reuse via a solver-owned build target; bit-match against theory fixtures as the conformance gate (ROADMAP gate 3).

## Item 2 — AltSolver deep-dive (E1 + E2)

Capabilities: exact alternation semantics — mover-only children, max for X / min for O (alt_solver.hpp:168-184); terminal mapping {+1, −1, 0} (alt_solver.hpp:154-162); memo keyed (PosId, mover, remaining_depth) with full-identity collision-chaining, i.e. collision-safe (alt_solver.hpp:113-124); cancellation without caching partials (test_alt_solver.cpp:165-175); validated against a 22,186-node ttt3 oracle (test_alt_solver.cpp:86-109).

Limits for a weak solve:
- `value` is double (alt_solver.hpp:19). Exact-safe for {−1, 0, 1} (all representable), but carries no win-distance, no bounds, no proof linkage; 8 wasted bytes per entry.
- Depth-keyed memo: `remaining_depth` is part of the key (alt_solver.hpp:62-66), so the same position under different horizons is re-searched and re-stored. A solve-to-terminal engine wants depth-free exact entries.
- Memo is `unordered_map<size_t, vector<Entry>>` (alt_solver.hpp:71): unbounded, no eviction, no persistence, measured ~55–133 B/entry (E2 below) vs ~40 B of payload identity alone.
- No pruning of any kind: all children are always evaluated (alt_solver.hpp:174-184) — not even a stop-at-proven-win cutoff, no move ordering, no symmetry.
- No proof emission: returns a scalar; no best move, no child provenance — cannot feed a certificate.
- Estimate leakage: below-horizon values come from `eval_estimate` (alt_solver.hpp:132-142) — fine for its s1 purpose, irrelevant to an exact solve.

4-bucket (item 2):
PRIMARY_BUCKET: still-open
still-open: the weak-solve engine (exact full solve + proof emission + persistence) does not exist anywhere in the repo.
already-closed: AltSolver as a depth-limited exact-alternation *oracle* — keep verbatim as the cross-check for the new solver's shallow horizons (its depth-6 root memo count reproduced the independent BFS census exactly: 486,838).
product-overlapped: none.
recommended-next: rebuild (new solver, new store), reusing the rules kernel and AltSolver as test oracle; do not extend AltSolver in place — every load-bearing design choice (value type, memo key, storage, no pruning) is wrong for the solve.

## Item 3 — Scale feasibility (E2 measured, extrapolation marked)

Harness: throwaway C++ in /tmp (never in the tree), compiled and run as
`/opt/homebrew/opt/llvm/bin/clang++ -std=c++26 -O3 -I engine/src /tmp/s2_audit_measure.cpp engine/build/libuttt_core.a -o /tmp/s2_audit_measure && /tmp/s2_audit_measure` (and the same for s2_audit_states.cpp). Machine: 48 GB RAM, 15-core Apple Silicon, single-threaded runs. Binaries and sources deleted after the run; full source available on request.

Measured — alternating perft from the center-forced root (X to move; leaves = length-d or terminal paths):
```text
d1 9, d2 80, d3 704, d4 6120, d5 52584, d6 446944, d7 3755040,
d8 31263816, d9 257796760, d10 2115115344, d11 17273318656 leaves
rate 77–92 M nodes/s; path branching factor ~8.2/ply and stable through d11
```
Measured — distinct states per ply (BFS census, key = (x, o, forced); side-to-move is mark-parity):
```text
ply4 6092, ply5 51688, ply6 428264, ply7 3447584, ply8 26944486 distinct
D4-canonical: ply7 431167 (ratio 7.996), ply8 3368691 (ratio 7.999)
distinct growth ~7.8×/ply; transposition savings vs paths only 8% at ply7, 14% at ply8
```
Measured — AltSolver from the root, Tie::X (node_cap 2e8 on the deeper runs):
```text
depth 6:  memo 486838 complete, 0.15 s, peak RSS 66 MB
depth 8:  memo 30878908 complete, 18.3 s, 4.1 GB → ~133 B/entry, ~1.9 M path-visits/s
depth 10: cap hit (memo 2.0e8, incomplete), 298 s, 11.1 GB → ~55 B/entry at scale
depth 12: cap hit (memo 2.0e8, incomplete), 177 s, 16.9 GB (RSS high-water includes prior run)
```
Cross-check: AltSolver's depth-6/8 memo counts equal the cumulative independent census sums exactly (486,838; 30,878,908) — two implementations agree on the reachable-state counts.

Extrapolated (marked): at ~7.8× distinct-state growth per ply, cumulative distinct states pass ~2e9 by ply 10, ~1.6e10 by ply 11, ~1.2e11 by ply 12; closure-driven tapering arrives too late to save full enumeration — with games running tens of plies, total reachable space is ≥10^18 by any extrapolation. Full-width walls (at a packed ~16 B/state, extrapolated): laptop RAM (48 GB) ≈ ply 10; big cloud box (2 TB RAM) ≈ ply 12; 100 TB disk ≈ ply 13. D4 buys exactly one ply (÷8 ≈ one 8× ply). The wall is structural: no enumerate-everything strategy (including retrograde tables — UTTT's "endgames" have MORE marks and thus more states, with no compact piece-count parameterization) reaches terminal depth. Conclusion: the weak solve must be proof-search shaped — a best-first/depth-first proof of the root value that visits a tiny fraction of the reachable space, exactly the Connect-4 precedent (4.5e12 states, solved 1988 with knowledge + search). Feasibility therefore rests on proof-tree size, which is unknown until the value conjecture and early proof runs exist — flagged as the pair's principal risk, not a blocker.

## Item 4 — Search-architecture options (shortlist, not a decision)

GHI, stated precisely: every move adds one permanent mark; marks are never removed, so mark-count strictly increases along every path and the transposition structure is a leveled DAG (edges only ply p → p+1; transpositions only within a ply). There are no cycles, no repetition rule, and no history-dependent rule in the rule-9 game (budgets stripped). A node's value is a pure function of (x, o, forced, side-to-move), side-to-move = mark-parity, and `forced` is inside PosId (game_model.hpp:21-27). Therefore TT values are path-independent and graph-history interference does not arise — DFPN's classic GHI pathologies are structurally impossible here. Residual DAG effect: proof/disproof-number double-counting through shared subgraphs (an efficiency, not soundness, issue) and the certificate DAG must deduplicate nodes.

Options:
A. DFPN/PN2 over the canonical DAG. Three-valued outcome handled as two boolean proofs ("X achieves ≥ draw", "X achieves win"); strongest when the root is decisive and the proof tree is skewed. Needs a disk-backed PN store at scale.
B. Exact depth-first negamax {−1, 0, +1} with canonical TT, first-winning-child cutoff, and append-only proof logging. Simplest correct thing; proof-DAG extraction is natural (chosen move at our-to-move nodes, all children at opponent nodes); risk is weaker best-first guidance than PN on skewed proofs.
C. Hybrid (the 2swap shape): A or B on top + theory-gated strategy-rule leaf closure to amputate subtrees below rule-certified positions. Gated by ROADMAP gate 2 (theory soundness proofs); until then leaves close only on terminals.
D. Retrograde/endgame tables: rejected — see item 3 wall.

Symmetry: D4 only (color-swap maps to a value-negated position; usable but a design call). Measured gain saturates at the full ×8 by ply 7. Canonicalization = 8 × (board permutation + 9 mask maps); with precomputed 512-entry per-symmetry mask tables this is tens of ns — small vs a memo probe. Plug-in point: canonicalize at TT/memo boundary only (search walks raw positions); the certificate either stores canonical nodes + symmetry-mapped moves or emits the canonical DAG (schema question — theory-owned). Note the s1 zobrist is orientation-bound (cell-indexed tables, engine/src/core/zobrist.hpp) — canonical keying needs transform-then-hash or symmetry-composed tables; design decides.

Recommendation: design phase spikes A vs B (both over the same canonical rules kernel + storage layer), C staged behind the theory gate. No decision here.

## Item 5 — Certificate emission and operations (design space, not a decision)

What AltSolver lacks to emit a proof DAG: chosen-move recording at winner-to-move nodes; full child coverage at loser-to-move nodes; canonical node identity; any serialization; crash-safe append; resume. Emission shape that fits the DAG: append-only log of proven facts (canonical state, value, chosen move | child-list hash), post-processed into the schema'd certificate — the log is also the checkpoint (resume = replay/reload into the TT). Checkpoint alternatives: periodic TT snapshot + frontier dump. Disk: fixed-width packed records + external sort/dedupe is likely sufficient (certificate is write-once); an embedded KV store (RocksDB/LMDB-class) is the alternative for the *working* proof store; the TT proper can stay lossy only if every proven fact is logged before eviction — proof store and play TT are different objects and must not be conflated (s1 TT eviction, tt.cpp `preferable_victim`, is legal precisely because nothing depends on retention). Cloud shape: single big-RAM box first (DFS/PN state is pointer-heavy and shard-hostile); shard only if the proof outgrows one box — frontier sharding is a known hard mode, defer.

## Item 6 — Duplicate/already-built gate (mandatory 4-bucket)

PRIMARY_BUCKET: still-open
still-open: weak-solve engine, canonicalizer, at-scale proof store, certificate emitter, checkpoint/resume, run orchestration — none exist.
already-closed: rules kernel (item 1 — reuse); AltSolver as shallow oracle (item 2 — reuse, don't extend); TT machinery — 32 B entries, 4-way buckets, and a FullKey mode with exact-identity sidecar (engine/src/search/tt.hpp:22-52, tt.cpp:47-50) — the *pattern* reuses, though the payload (double lo/hi interval) is auction-shaped and a solve TT wants a new packed entry; zobrist with incremental update (position.cpp:85-87) — reusable, with the canonical-keying caveat in item 4; build/test infra (CMake C++26 -Wall -Wextra -Werror, doctest, fixture runner engine/tests/test_fixtures.cpp) — reuse; theory fixtures as conformance anchors (legality/closure_routing/terminal pin every rule-lock behavior; auction-only fields skipped via consumed_by).
product-overlapped: none in-repo. Prior art (Bertholon/Gehanne/Grimaud 2020 and the rule-variant delta) is theory-owned per the s2 ROADMAP teams table; solver treats it as an input, not a work item.
recommended-next: proceed to solver DESIGN (c2) on the surfaces below.

## Recommended surface

New top-level `solver/` as its own CMake project consuming the engine rules kernel as a library (add_subdirectory or a narrow rules-core target listing engine/src/core sources) — engine/ s1 scope is closed and its auction layers (adapter, root/bid_matrix, search threshold math) should not be dragged along. Audit recommendation only; design decides the exact target shape.

## Recommended cycle decomposition (solver pair)

- c2 DESIGN: architecture spike A vs B (item 4), canonicalization scheme, proof-store/TT split, certificate interface stub against theory's draft schema, checkpoint model, solver/ build surface. Grill triggers apply (new-feature, large tier, cross-domain certificate contract).
- c3: canonical rules layer + exact solver core, oracle-tested — bit-match AltSolver on shallow horizons and theory fixtures (ROADMAP gate 3 allows immediate start post-design).
- c4: at-scale proof store, checkpoint/resume, parallel workers; first deep sub-solves (e.g. one opening cell) end to end.
- c5: certificate emission against the locked theory schema; checker round-trip on a sub-solve certificate.
- c6: full run (laptop → cloud), certificate handoff, live-verify = independent checker pass end to end.

## Boundary contract

Audit-time: not applicable (read-only). Downstream (noted, unowned here): Writes: certificate artifact per theory-owned schema. Reads: theory schema + soundness-gated strategy rules. Downstream consumer: theory's non-C++ checker. Proof: checker verifies a sub-solve certificate end to end before the full run (E3-class evidence at c5).

## Evidence by claim

- Rules conformance: E1 (file:line above) + E2 standing green s1 suites (fixture runner consumes theory fixtures).
- AltSolver capabilities/limits: E1 (alt_solver.hpp cites) + E2 (oracle test, measured runs).
- Scale numbers: E2 measured (exact commands above); wall positions and total-space size: extrapolated, marked.
- GHI/DAG argument: E1-grade structural argument from the move rule (applied() only ORs mark bits, position.cpp:71); no command can prove a negative — flagged as an argument, not a measurement.

## Risks / reject-or-narrow gates

- Reject gate (dispatch): does not fire — rules core expresses the rule-lock game with zero surgery.
- Principal risk: proof-tree size is unknown; feasibility of the full weak solve rests on the root being provable with best-first search + symmetry (+ later rule closure). Mitigation: c4's sub-solve milestones give early measured proof-growth curves.
- Draw-value risk: if the root is a draw, the certificate must prove both "X ≥ draw" and "O ≥ draw" — roughly doubles the artifact. Value conjecture is theory's c1/c2 deliverable.
- Rules-ambiguity escalation channel (ROADMAP): nothing found to escalate.

## Operator-judgment items

none blocking. Deferred-to-plan notes: cloud box sizing/provider; color-swap symmetry use; certificate granularity (canonical-DAG vs raw with symmetry annotations) — the last belongs to the theory schema conversation.

## Questions for Implementer/operator

For reconcile with solver.implementer's independent return: (1) agreement on the rebuild-vs-extend verdict for AltSolver; (2) agreement on the ~ply-11–12 full-width wall and its architectural consequence; (3) preferred surface (solver/ vs engine/ extension).

ACTIONS_GIT_REF: no repo edits claimed; measurement harnesses lived in /tmp and were deleted after the run; this relay draft is the only file authored by this seat
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-3/
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-boot/SITREP-orchestrator-reviewer-20260828-002232.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-seat/
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/AUDIT-pair-implementer-20260828-003733.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/AUDIT-pair-implementer-20260828-003801.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/AUDIT-pair-planner-20260828-002956.md
(all listed paths belong to other seats' s1/s2 filings, none authored by solver.planner)
