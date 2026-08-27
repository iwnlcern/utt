ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-plan-review-1
PARENT_DISPATCH_ID: engine-c4-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the residuals are technical execution-contract repairs within approved DD rev 5 and G1–G4
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-plan-1/PLAN-planner-20260827-021748.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: 3f541dea9fe11324efbde49d660827479598191135d73be2b6d00b8a5273d04d
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: PLAN-REVIEW must-revise — rev 1 has six executable seam and acceptance gaps

## Verdict

Revision 1 preserves the approved design identity, maps A1–A11 across fourteen TDD tasks, keeps theory/harness bytes and SMP publication out of scope, names mutation-sensitive evidence for A4–A7/A9, and retains separate implementation and merge gates. Those structures may be retained.

Approval is blocked by six technical execution gaps. Task 1 cannot reach GREEN before its Task-2 rational dependency exists; the generic search/TT seam does not carry the independent 32-bit verification tag and its size exponent contradicts the stated memory default; the root anchor interface narrows the exact continuous rational to `double` and does not bind the production TInterval payoff/quality path; A8 compares against “fixture LP values” that do not exist in the landed fixtures; the fitter and P1b formatter depend on an analysis surface no task creates; and the fixed 27 s/29 s clock plus a nonexistent tournament path cannot execute A10 at `time_ms=2000`.

No operator product choice is needed. Return a revision 2 under the same PLAN lock and design digest, preserving the good task/acceptance structure while closing M1–M6 below.

This is PLAN-REVIEW only. No source/test edit, branch, commit, PR, implementation dispatch, implementation, merge, publication, deployment, or release action is authorized or performed.

## Target and integrity

- Reviewed `PL-engine-c4-20260826` at committed digest `3f541dea9fe11324efbde49d660827479598191135d73be2b6d00b8a5273d04d`; `git show 688232b:<plan>` hashes to the same bytes and `git diff --check 688232b^..688232b` is clean. — E1/E2 instrument evidence.
- The incoming relay is addressed to `engine.implementer`, parents to approving design review `engine-c4-design-review-5`, carries matching canonical design/plan locator-digest pairs, and passes exact-file relay lint. — E1/E2 instrument evidence.
- The design digest remains approved rev 5 `a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1`; the orchestrator sequencing relay grants conditional delegation only after an approving PLAN-REVIEW and an all-in scope diff. — E1.

## Must-revise residuals

### M1 — Task 1's RED/GREEN cycle depends on an unimplemented Task 2

Plan lines 31–35 make Task 1 compile and pass tests against `TestRational`, but Task 2 does not create or implement that type until lines 37–46. Task 1 Step 3 implements only `tvalue.hpp`; “add both test files” does not implement the missing rational header. Therefore Task 1 cannot reach its promised focused/full GREEN or commit before Task 2, contradicting the task order and per-task commit discipline. — E1 — plan lines 25–46.

Required revision: move the test-side rational task before the first interval tests, or make rational creation an explicit prerequisite subtask completed before Task 1 RED. Each task must have a self-contained RED command, implementation step, GREEN command, and commit; do not claim the two files “land together” while assigning them sequential commits.

### M2 — The TT integration has no independent-tag carrier, and the sizing default is arithmetically ambiguous

`GameModel` exposes only `uint64 key(state)` (line 73). The TT API probes with `(uint64 key, PosId)` and says `PosId` is unused in Play mode (line 130); it has no independent `uint32 tag` input. The existing production source already distinguishes `Position::key` from `zobrist_tag_full(Position)` (`engine/src/core/position.hpp:29`; `engine/src/core/zobrist.hpp:24–25`), and DD §5 requires that independent tag on every play-mode store/probe. As written, the generic search cannot populate or validate the locked tag through its production seam. The injected-hash test constructor does not repair the ordinary caller contract. — E1 — plan lines 72–75/127–136; design lines 82–113; existing source cited above.

The declared default also says `size_log2 = 22 ≈ 128 MiB` (line 193) while each bucket is 128 bytes. If `size_log2` counts buckets as `TT(size_log2, ...)` suggests, `2^22 * 128` is 512 MiB; 128 MiB requires `2^20` buckets. If it counts entries, the mapping to four-way buckets must be stated. — E1 arithmetic evidence.

Required revision: define one caller-visible TT identity carrier that supplies the 64-bit bucket key, independent 32-bit tag, and (only in FullKey mode) semantic snapshot through the same production probe/store path. Bind UTTT to `Position::key` plus `zobrist_tag_full`, give ttt3 a deterministic equivalent, and make the injected collision seam override both hashes without bypassing probe/store. Define whether `size_log2` counts buckets or entries and select the exponent that actually yields the committed 128 MiB default.

### M3 — The root provider contract loses exact anchors and omits the production payoff/quality binding

Task 13 defines `Anchors { double r_root; ... }` (line 164), but solved-scale A8 must consume exact-rational `ContNode.r` from Task 6 before half-up rounding. Converting that value to binary64 changes the rational presented to `half_up`; calling the binary64 value's rational reading “exact” does not preserve the exact continuous node required by DD §7. The label comparison may catch some fixture mismatches, but the provider interface itself cannot express the locked binding. — E1 — plan lines 85–90/163–166; design lines 149–160/210.

The same task names a generic `PayoffFn` but never defines the production binding required by design line 155: terminal or exact-gate children yield exact ordinal entries, in-band children use the interval midpoint, and any such entry taints the root report to `quality = estimate`. A8 exercises only the test-side discrete oracle, while A9 contains no production root-matrix taint case. — E1 — plan lines 151–170; design lines 152–160/210–211.

Required revision: keep one shared action/winner/payment/transition implementation but give the solved-scale anchor provider a lossless exact-rational or exact-`k*` carrier; keep production's midpoint-derived binary64 anchor as a distinct binding. Specify and test the production TInterval payoff provider, including exact-gate/terminal outcomes, midpoint fallback, and root quality-taint propagation. The test binding must still log exact discrete coordinates and compare every payoff byte.

### M4 — A8's exact LP target is named as a fixture that is not landed

Task 13 requires agreement with “the fixture LP values” (line 166), but `theory/fixtures/thresholds_ttt3.json` contains thresholds, critical bids, preferred moves, and discrete expectations only; no landed fixture carries matrices, LP values, or certified strategies. Theory obtains those dynamically from `lp_reference.py::emit_root_matrices` and certifies them in `theory/tests/test_lp_reference.py`. The plan names neither an invocation/import bridge nor an engine-owned generated artifact, so the LP-value comparison cannot run. — E1 — `theory/fixtures/thresholds_ttt3.json`; `theory/auction_ttt/lp_reference.py:489–562`; `theory/tests/test_lp_reference.py:50–78`.

Required revision: name an executable, reproducible LP-reference path. For example, generate JSON from `emit_root_matrices(solve_continuous(), {8,16,32: solve_discrete(...)})` during the test and compare it to engine output, or commit an engine-owned generated test artifact with exact provenance and a regeneration/check command. Keep theory bytes read-only. Compare action labels and payoff matrices first, then exact LP value and RM+ exploitability, with the precise test command named.

### M5 — The fit run and P1b label depend on an analysis surface absent from the file/task graph

Task 12 says `fit_eval.py` runs `uttt_engine --analyze-depth` or consumes a precomputed labels file (line 153), but the current engine is a JSONL adapter, no task adds that option, and the Task-12 file list contains no labels file. Its A9 test also refers to “the analysis formatter” without naming a file, interface, or metadata key that carries the literal `convention` label (lines 151–158). Thus Step 4 cannot execute and the P1b assertion has no defined production surface. — E1 — plan lines 149–159; `engine/src/adapter/main.cpp`; `engine/src/adapter/wire.hpp:17–45`; `engine/src/adapter/wire.cpp:242–249`.

Required revision: choose and name one in-scope route: add a concrete analysis executable/mode and formatter with tests, or add a committed precomputed labels artifact plus its provenance/check command. Define the exact metadata field/value that emits `convention` without changing the harness contract, and wire the fitter's input/output commands so Task 12 Step 4 is runnable from a fresh checkout.

### M6 — Clock and gauntlet wiring cannot execute the required fast/full acceptance pair

Task 13 fixes search to 27 s soft / 29 s hard with a 500 ms matrix reserve (lines 163–164), while Task 14 runs the required strength gauntlet with referee `time_ms=2000` (line 178). An engine that waits for the fixed full-time deadlines will fault every fast-control move. The plan never derives per-request deadlines from `TurnRequest.time_ms`, so A10 and A11 are mutually incompatible as written. — E1 — plan lines 163–179; `engine/src/adapter/wire.hpp:17–25`; design lines 185–188/212–213.

Task 14 also says its wrapper spawns `referee/tournament.py` (line 174), which does not exist. The landed runner is `referee/poorman_referee/tournament.py`, exposed as `python -m poorman_referee tourney --config <json>`; its config uses `games_per_pair`, engine command arrays, `time_ms`, jobs, seed, and output directory. — E1 — `referee/poorman_referee/cli.py:27–51`; `referee/poorman_referee/tournament.py:17–31/65–99`; `referee/pyproject.toml`.

Required revision: define request-aware soft/hard deadlines and a matrix reserve that stay strictly inside every supplied `time_ms`, while reproducing the locked 27 s/29 s behavior at the 30 s control; add fake-clock tests for both 2 s and 30 s budgets. Replace the nonexistent runner path with the real module entrypoint or direct Python import, define four explicit two-engine configs with `games_per_pair: 50`, compute points as `wins + draws/2` from each summary, reject faults/voids, and name the full-clock log fields/command used for the two-game A11 check.

## Retained revision-1 strengths

- Preserve the approved design digest, scope/out-of-scope lines, branch preflight/STOP rule, fourteen-task decomposition, and one-commit-per-task discipline after dependency repair.
- Preserve directed primitive/preimage literals, named cancellation vectors, seeded property tests, cutoff counters and mutation evidence, collision-aware per-way sidecars, exact P2 mutation families, and A1 owner-honest treatment of `backup_cases.json`.
- Preserve separate exact continuous/discrete ttt3 solvers, shared matrix transition semantics, averaged RM+ reporting, staged completed-iteration publication, A1–A11 truth table, honest failed-attempt recording, and separate merge authority.
- Preserve the committed engineering defaults after correcting their executable units and deadline behavior; no G1–G4 decision is reopened.

## Successor acceptance

Return revision 2 under `PLAN_LOCK_ID: PL-engine-c4-20260826`, parented again to approving design review `engine-c4-design-review-5`, and answer this review through `IN_REPLY_TO`. Close M1–M6 with a fresh locator/digest carrier. Approval requires an acyclic TDD order, complete key/tag/snapshot TT plumbing with a real 128 MiB default, lossless exact and explicit production matrix bindings, an executable LP oracle, a runnable fit/P1b analysis surface, and request-aware clock plus real-referee gauntlet commands.

Do not issue delegated implementation authority from this verdict. The orchestrator's conditions require an approving PLAN-REVIEW, an all-in scope diff, and no hard trigger or boundary deviation.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation dispatch or runtime action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation in review
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory and harness contracts
- user-visible-control-with-materializer/downstream-consumer: yes — engine analysis metadata is consumed by harness/UI surfaces
- test-runtime-role-mismatch: yes — test providers and planned production provider/analysis surfaces are not yet type- or path-complete
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — A8/A10/A11 cannot be evidenced from the current commands
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Read the full incoming relay, full plan, approved DD rev 5, orchestrator proceed relay, current engine key/tag/adapter/clock surfaces, theory continuous/discrete/LP sources and fixtures, and referee CLI/tournament implementation. — E1.
- Incoming relay exact-file lint is clean with historical freshness disabled; plan/design locators and digests resolve exactly. — E1/E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-engine-c4-plan-review-1.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
