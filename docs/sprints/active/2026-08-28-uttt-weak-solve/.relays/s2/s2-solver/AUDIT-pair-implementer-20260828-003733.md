ROLE: Pair Implementer
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: s2-solver-c1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — read-only return; architecture selection remains later DESIGN
FROM: solver.implementer
TO: s2.orchestrator-planner
CC: solver.planner, s2.orchestrator-reviewer
STATUS: returned
IN_REPLY_TO: s2-solver/AUDIT-orchestrator-planner-20260828-001407.md
SUBJECT: AUDIT return — reuse sound, AltSolver oracle-only, shallow exponential wall measured

ACTIONS_GIT_REF: daemon-only onboarding rendered s2-seat/BOOT-pair-implementer-20260828-001841.md and updated projections; no engine/source/test/prototype repository files were edited by this seat
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-2/
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-boot/SITREP-orchestrator-reviewer-20260828-002232.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-seat/

Snapshot: `git status --short` at 2026-08-28T00:26:56-0700. s1 files, reviewer SITREP, and other-seat artifacts are inherited/concurrent. This seat's pre-return tracked contribution is its daemon boot plus SEATS/INDEX projections.

## Executive verdict

Reuse `engine/src/core` behind a solver-owned alternating adapter. Put s2 work under top-level `solver/`; do not reopen closed s1 engine scope. Keep `AltSolver` as a small exact-WDL oracle, but rebuild production around compact canonical identity, solver-specific storage, checkpointing, and proof extraction.

Shortlist, not lock: PN-DFPN/DFPN; exact WDL alpha-beta/null-window plus extraction; hybrid proof search plus measured late tables and theory-approved positional leaves. Carry the hybrid and simpler alpha-beta oracle into DESIGN.

## 1. Rule-lock reuse

PRIMARY_BUCKET: already-closed
still-open: make side-to-move explicit, fix X at root, exclude auction tie/chip identity.
already-closed: board, 3^9 table, closure, routing, movegen, structural terminals.
product-overlapped: s1 `TieState`/`chip_sign` surrounds the reusable kernel.
recommended-next: solver adapter plus independent perft/layer and fixture parity; no engine edits.

- Nine X/O 9-bit masks (`engine/src/core/position.hpp:20-29`); all 19,683 ternary locals (`engine/src/core/local_table.hpp:10-25`, `.cpp:83-105`), exhaustively checked (`engine/tests/test_local_table.cpp:7-28`).
- X-first/alternation are not enforced by `applied(Move, Seat)` (`position.hpp:31-35`, `position.cpp:54-72`); AltSolver chooses mover and toggles (`engine/src/root/alt_solver.hpp:167-181`). Adapter seam, not surgery.
- Center board: `forced=4` (`position.hpp:27-28`), forced movegen (`position.cpp:35-50`), nine-move test (`engine/tests/test_position.cpp:22-29`) and owner fixture (`theory/fixtures/legality.json:6-16`).
- Routing/closed target: successor forced logic (`position.cpp:73-81`), ANY skips closed (`:45-51`), fixtures cover already-closed and self-close (`theory/fixtures/closure_routing.json:30-51`).
- Won/full closes (`local_table.cpp:65-78`), propagates (`position.cpp:72-78`), then rejects play (`:54-63`).
- Macro win precedes all-closed (`position.cpp:28-32`), stops successors (`:35-38`), ordering fixture (`closure_routing.json:66-75`).
- All-closed is `AllClosed` (`position.cpp:28-32`) and maps exactly to draw 0 (`alt_solver.hpp:152-162`). Unequal-chip fixtures are auction-only; equal chips draw (`theory/fixtures/terminal.json:33-39`).

Factor/pin: Position identity and hashes include `TieState` (`position.hpp:27-29`, `position.cpp:79-93`, `zobrist.cpp:39-55`); `PosId` carries it and `GameModel` requires budget `chip_sign` (`game_model.hpp:21-27,35-50,81-87`). Budgets are otherwise absent from board transitions.

Reject/narrow: not triggered. Minimal factoring is solver-owned `{marks, forced, side}` identity.

## 2. AltSolver

PRIMARY_BUCKET: still-open
still-open: production storage, depth-independent solved reuse, persistence, symmetry, proof edges, exact-to-terminal driver.
already-closed: mover min/max, exact WDL terminals, collision buckets, cancellation/caps, small oracle.
product-overlapped: fixed TT/Zobrist cover primitives, not proof records.
recommended-next: retain oracle; rebuild production shell.

Binary64 is exact-safe for completed {-1,0,1} min/max (`alt_solver.hpp:152-185`); horizon uses evaluator and `Quality::Estimate` (`:132-165`). Buckets compare full PosId/mover/depth (`:61-72,108-129`), with forced-collision/depth tests (`engine/tests/test_alt_solver.cpp:143-163`). Cancellation is incomplete and does not cache parent (`alt_solver.hpp:144-150,174-189`; test `:165-174`).

Depth is memo identity, duplicating iterative depths (`alt_solver.hpp:61-67,95-121`). Unbounded `unordered_map<size_t, vector<Entry>>` has no eviction/serialization (`:69-78,126-129`). `AltResult` has no move, all-reply edges, transform, witness, dependency, or artifact ID (`:18-22,61-67`). Children copy positions (`game_model.hpp:89-101`). Verdict: rebuild production; promote only oracle.

## 3. Scale

PRIMARY_BUCKET: still-open
still-open: no reachable denominator/D4 layer census.
already-closed: fast rules kernel; retained state/tree is first wall.
product-overlapped: benchmark/TT sizing help locally only.
recommended-next: canonical layer census reporting raw/legal/reachable/terminal/transposed/D4 counts.

Host: Apple M5 Pro, 15 CPUs, 51,539,607,552 RAM (`uname -a; sysctl -n hw.memsize hw.ncpu machdep.cpu.brand_string`). Committed ten-run 2M-ply benchmark: 50.47830225 ns/ply, ~19.81M random applications/s (`engine/bench/candidate-verdict.json:4-19,21-37`).

Throwaway C++26 stdin harness under `mktemp /tmp/uttt-s2-audit.XXXXXX` linked `libuttt_core.a`, prewarmed LocalTable, solved center root with X, timed, counted memo, sampled RSS. Shape:

```sh
/opt/homebrew/opt/llvm/bin/clang++ -std=c++2c -O3 -Wall -Wextra -Werror -I engine/src -x c++ - -x none engine/build/libuttt_core.a -o "$probe_dir/probe"
for depth in 0 1 2 3 4 5 6; do "$probe_dir/probe" "$depth"; done
unlink "$probe_dir/probe"; rmdir "$probe_dir"
```

Exact successful output; depth 7 same harness literal 7:

```text
d0 entries=1
d1 entries=10
d2 entries=90 rss_delta=32768
d3 entries=794 rss_delta=196608
d4 entries=6886 sec=0.001 rss_delta=1130496
d5 entries=58574 sec=0.013 rate=4533035.929/s rss_delta=8454144 bytes/entry=144.333
d6 entries=486838 sec=0.176 rate=2759101.832/s rss_delta=68272128 bytes/entry=140.236
d7 complete=true quality=2 value=0.000 entries=3934422 sec=2.241 rate=1756040.932/s rss_delta=548290560 bytes/entry=139.357
sizeof(Position)=88 sizeof(PosId)=38 sizeof(AltResult)=16
```

`quality=2` is Estimate (`engine/src/search/search.hpp:19-20`): not proven nodes/value. Focused suite: 6/6, 66,669 assertions. Full `ctest --test-dir engine/build --output-on-failure`: unit 37.94s, e2e 25.62s, 100%, total 63.57s.

Measured depth 7 costs 522.9 MiB. Extrapolated only: log fit depths 2-6 = 8.58x/ply; at 140.236 B/entry, ~41 GiB depth 9, 351 GiB depth 10, 3.0 TiB depth 11. Later terminals/transpositions/symmetry/selectivity alter this; it locates current wall only.

48 GiB holds <=3.68e8 current entries before overhead (~fit d9); 256 GiB/1 TiB hold 1.96e9/7.84e9 (fit d9.85/d10.5). 4 TB holds 8.33e10-1.67e11 compact 48-24B records before indexes; random I/O dominates. Scale-up buys shallow plies, not game.

Reachable fraction: **unknown**; no census, so numeric fraction would be fabricated. Loose representation bound `10*3^81` precedes legality/turn/closure/terminal/symmetry; D4 <=8x. Resolve by census.

## 4. Architecture, GHI, D4

PRIMARY_BUCKET: recommended-next
still-open: production proof search/canonicalizer/graph-safe accounting/late tables.
already-closed: finite deterministic children and exact terminal WDL.
product-overlapped: engine search/TT and external UTTT PN-DFPN.
recommended-next: benchmark hybrid PN-DFPN/null-window and alpha-beta oracle identically.

1. DFPN/PN2/two-level PN-DFPN: X OR/O AND for X-win; natural weak proof (one X edge, all O replies: `ROADMAP.md:27-34`; Connect4 transcript `Beating Connect 4 with Graph Theory.txt:70-84`). Dual search if X-win unproven.
2. Exact WDL alpha-beta/null-window: simpler reference/bounded DFS; TT cutoffs are not certificate, so extraction replays legal moves and exact opponent children.
3. Retrograde late layers: sequential/deterministic; only if census finds tractable slice.
4. Hybrid: proof front, D4 TT, measured tables, theory-approved leaves, deterministic extraction. Recommended, not lock.

GHI: every edge adds permanent mark (`position.cpp:66-72`), closure is irreversible (`:73-81`), terminal has no successor (`:35-38`); graph is acyclic. With `{marks,forced,side}` and no repetition/history draw, value is path-independent, so classic GHI is absent. Multiple parents remain: use acyclic-graph-safe PN/TT and deduplicated extraction. Reference: Schijf/Allis/Uiterwijk, https://doi.org/10.3233/ICG-1994-17203 .

D4: center-board opening invariant (`ROADMAP.md:18-20,27-34`). Precompute `transform[8][512]`, `board_perm[8][9]`; transform masks/forced, retain side, select lexical minimum, retain inverse for moves. Root cells collapse center/edge/corner. Reduction <=8x; benchmark. Inspiration: `I Solved Connect 4.txt:139-147`.

Independent directional evidence: Nelson Elhage reports UTTT PN-DFPN/shared TT, ~20-ply positions in hours, full estimate hundreds of millions CPU-hours: https://minimax.dev/docs/ultimate/ . His free 81-cell opening differs: https://minimax.dev/docs/ultimate/the-game/ .

## 5. Certificate/operations

PRIMARY_BUCKET: still-open
still-open: theory schema, emitter, proof identity/extraction, durable store/frontier, checkpoint, cloud runbook.
already-closed: roadmap locks writer/reader and weak-DAG intent.
product-overlapped: engine `certificate_*` certifies auction root action, not proof DAG (`engine/src/adapter/policy.hpp:27-43,280-293`; `engine/tests/test_root_matrix.cpp:418-442`).
recommended-next: mutable search store separate from immutable content-addressed proof.

AltSolver has no edges/witnesses (`alt_solver.hpp:18-22,61-67,174-189`). Schema seam proposals, subordinate to theory: rules/schema/build fingerprinted root; collision-safe state bytes/ID+side; terminal or edges; one X edge; complete O move set; orientation transform; deterministic chunks/hashes/manifest; theory-defined dual shapes for draw/loss.

Checkpoint binds rules, canonicalizer, objective/root, source/executable, TT version, schema digest; refuses mismatch. WAL + immutable checksummed snapshots/manifests, monotonic epochs, idempotent resume. Persist solved status, proof/disproof/work/witness/frontier leases; separate exact solved from replaceable bounds.

TB candidates: sorted/mmap layers; custom buckets + hot RAM index; RocksDB/LMDB baseline only; immutable proof chunks outside TT. Cloud: single large shared-TT box first; shard only canonical frontier with disjoint objectives, immutable manifests, independent checkpoints/subproofs, leases/idempotent keys. Theory soundness gates nonterminal strategy leaves (`ROADMAP.md:46-50`).

## 6. Duplicate gate

PRIMARY_BUCKET: product-overlapped
still-open: no solver/, D4, proof search, persistent store, emitter, checkpoint, s2 certificate schema/fixtures.
already-closed: core/table/Position/Zobrist, 32B TT, AltSolver, C++26 infra, current conformance fixtures.
product-overlapped: primitives auction-coupled; certificate diagnostic is name-only overlap.
recommended-next: adapter reuse; new solver/proof/storage surfaces after gates.

- TT is 32B, 4-way, evicting, optional full-key sidecar (`engine/src/search/tt.hpp:22-52,63-113`; `.cpp:39-49,88-163`), default 128 MiB pinned (`engine/tests/test_tt.cpp:147-169`); no durable proof records.
- Zobrist is deterministic 64+32 board/forced/tie (`zobrist.hpp:10-25`, `.cpp:19-63`); derive side/no-auction key; certificates never trust hash alone.
- C++26/O3/tests/bench/CTest exist (`engine/CMakeLists.txt:1-10,19-89`); add independent solver build.
- Engine consumes all tagged legality/terminal rows (`engine/tests/test_game_model.cpp:52-70,129-160`); s2 schema/checker fixtures absent and theory-owned (`ROADMAP.md:38-49`).

No flag, dead path, alternate executable, or hidden emitter closes s2.

## Cycle decomposition — not plan

1. Adapter, independent perft/layer census, D4 measurements.
2. Bounded alpha-beta and DFPN kernels on identical positions.
3. Production TT/checkpoint crash/resume; evidence-based kernel choice.
4. Theory-schema lock, emitter/extractor, checker fixtures.
5. Measured tables or theory-proven leaves.
6. Laptop soak, single-box cloud, optional frontier shards, final verified certificate.

## Boundary

Writes: eventual weak-solution DAG; audit writes no production object.
Reads: rule lock and theory schema/checker once locked.
Target entity: verified root W/D/L proof for center-board-forced alternating rule-9 game.
Downstream consumer: theory non-C++ checker.
Contract: unlocked; item 5 subordinate to theory.
Proof: eventual E2 emitter/checker plus fixture parity; audit E1/E2 only.
No-consumer action: defer emitter until addressed theory contract locks.

Operator-judgment items: none. Architecture is DESIGN; schema normal orchestrator gate.

Prior-art warning: Bertholon et al. 2020 uses free opening field and continued play in won fields (https://arxiv.org/abs/2006.02353), so it does not certify this lock. ROADMAP shorthand `Bertholon/Gehanne/Grimaud` does not match that paper's authors; reconcile citation.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only audit
- migration/backfill/destructive-write/canonical-data-repair: no — no mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game proof
- AI-or-automation-acts-downstream: no — no runtime action
- worker/scheduler/queue/retry/async-side-effect: no — recommendations only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory schema/checker boundary
- user-visible-control-with-materializer/downstream-consumer: no — no UI
- test-runtime-role-mismatch: no — native unit/e2e passed
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — unknown remains unknown
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver is requested; the trigger is the known theory schema/checker boundary
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Acceptance: six items cited/E2; lock itemized; measured/extrapolated separated; four buckets each; shortlist/decomposition/operator/status present; source/tests untouched.

Next requested action: reconcile with independent planner audit, map findings once, then dispatch DESIGN questions without treating shortlist as lock.
