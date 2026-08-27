ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-plan-review-2
PARENT_DISPATCH_ID: engine-c4-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the residuals are technical execution-contract repairs within approved DD rev 5 and G1–G4
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-plan-2/PLAN-planner-20260827-030739.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: 7e8dca7de8ad9e45ae0d51423098438897355b1f41816d745e455e9b1485545b
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: PLAN-REVIEW must-revise — rev 2 closes M1/M2/M4 and most of M3/M5/M6, but three executable seams remain

## Verdict

Revision 2 materially closes the prior dependency-order defect, gives the search/TT path an independent key/tag carrier with an arithmetically correct 128 MiB default, preserves solved-scale anchors losslessly as exact `k_star`, names a committed LP-reference generator/artifact, and replaces fixed clocks and the nonexistent referee path with request-aware deadlines and the real tournament entrypoint. Preserve those repairs.

Approval is still blocked by three narrow execution defects. The production in-band payoff has its X/O sign reversed; the analysis request has neither budgets nor feature values, so it cannot produce the promised P1b equality label or fit evaluator weights from the declared data; and A11 still has no named runnable full-clock command/config path despite the prior request for one.

No operator product choice is needed. Return revision 3 under the same plan lock and design digest, retaining every closed fold while repairing R1–R3 below.

This is PLAN-REVIEW only. No source/test edit, branch, commit, PR, implementation dispatch, implementation, merge, publication, deployment, or release action is authorized or performed.

## Target and integrity

- Reviewed committed `PL-engine-c4-20260826` rev 2 at SHA-256 `7e8dca7de8ad9e45ae0d51423098438897355b1f41816d745e455e9b1485545b`; the approved design remains rev 5 at SHA-256 `a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1`. — E1/E2 instrument evidence.
- Incoming `engine-c4-plan-2` is addressed to `engine.implementer`, replies to review 1, re-parents the gated plan to approving design review 5, carries matching locator/digest pairs, and passes exact-file relay lint with historical freshness disabled. — E1/E2 instrument evidence.
- Exact rev-1-to-rev-2 diff and current source/theory/referee interfaces were checked; findings below cite the current plan line numbers. — E1.

## Closed prior residuals

- M1 closed: `TestRational` is now Task 1 and TInterval consumes its committed result in Task 2; each task has an independent RED/GREEN/commit cycle. — plan lines 25–57.
- M2 closed: `GameModel::tt_key` carries independent `key` and `tag`; UTTT binds `Position::key` plus `zobrist_tag_full`; FullKey adds the semantic snapshot through the same probe/store path; `entries_log2` explicitly counts entries and 22 computes to 128 MiB. — plan lines 73–79/129–138.
- M3 anchor-carrier half closed: solved-scale exact rational `r` is converted to exact `k_star` before crossing the provider seam, while production has a distinct exact-reading-of-binary64 half-up path. — plan lines 164–170.
- M4 closed in structure: Task 13 owns a committed generated LP artifact, generator, exact regeneration command, label/payoff-first comparison, exact LP-value comparison, and RM+ exploitability check while theory bytes remain read-only. The generator implementation must use the landed callable shape `emit_root_matrices(solve_continuous(), {scale: solve_discrete(scale) for scale in (8, 16, 32)})`; the set/dict shorthand in plan line 170 is explanatory rather than executable. — E1 — plan lines 164–174; `theory/tests/test_lp_reference.py:50–53`.
- M6 fast-control half closed: deadlines scale from each `TurnRequest.time_ms`, reproduce 27 s/29 s at 30 s, and have 2 s/30 s fake-clock tests; A10 uses the real referee module, four explicit 50-game configs, summary points, and fault/void rejection. — plan lines 168–183.

## Must-revise residuals

### R1 — Production in-band payoff maps X and O backwards

Task 13 defines the production fallback ordinal as `sign(midpoint − bx2/M2)` (line 167). The locked threshold orientation is the opposite: X is forced when its budget proportion is above T (`budget_x − E > T.hi × M`), O when it is below T (`T.lo × M − budget_x > E`), and the matrix payoff convention is `OWin=-1`, `Draw=0`, `XWin=+1`. Therefore the stated expression emits a negative ordinal on the X side and a positive ordinal on the O side. — E1 — plan lines 140–149/167; design §7 lines 135–155; Task 7's `Outcome { OWin=0, Draw=1, XWin=2 }` at plan line 98.

Required revision: define the fallback as the sign of the budget proportion minus the threshold midpoint, using an exact or explicitly directed comparison that does not silently recreate the forbidden float-product gate. Add named tests on both sides of the midpoint that assert X maps to `+1`, O maps to `-1`, and any in-band fallback still sets `exact=false` and taints the root report to `quality=estimate`.

### R2 — The analysis/fitter contract lacks the data needed for both advertised consumers

The new analyze input is exactly `{"parts": ..., "h": ..., "depth": N}` (line 155). P1b equality is the relation `p = T`, where `p = bx/(bx+bo)` at a larger scale, but the request carries no `bx`, `bo`, total scale, or proportion. `Position::from_parts` imports only board masks, forced board, and tie state; it does not contain budgets. Thus the analyzer cannot decide when to emit `equality_label: "convention"`, and the named positive test cannot be constructed from the declared schema. — E1 — plan lines 153–160; design §7 line 165; `engine/src/core/position.hpp:37–41`.

The same analyzer returns only threshold/quality/depth/completeness/label. Least-squares fitting also needs the evaluator feature vector for every target midpoint, but neither the analyzer output nor the declared sampled-position corpus provides those independent variables, and the plan names no Python/C++ feature-parity implementation. A script cannot derive weights from target labels alone. — E1 — plan lines 153–161.

Required revision: add budgets (or an exact reduced proportion plus scale classification) to the analysis request and specify the exact equality predicate that produces `"convention"`. Also expose a stable ordered feature vector in analysis output, or define a named fitter-side extractor with an E2 parity test against `eval_estimate`'s production feature extraction. Update the committed corpus schema and fresh-checkout command accordingly; keep the harness protocol unchanged.

### R3 — A11 still lacks a runnable full-clock verification command

Task 14 names four committed configs, all at `time_ms: 2000`, and gives a runnable A10 command. Its A11 step then refers only to a `time_ms: 30000` “config variant” and two games, without naming a fifth config, a runner option that synthesizes it, an exact command, or the parser path that walks `auction.attempts[*].turns.{X,O}.elapsed_ms` and checks published `info.depth`/`info.complete`. The prior review explicitly required the full-clock log fields and command, so this part of M6 is not yet executable from a fresh checkout. — E1 — plan lines 176–185; `referee/poorman_referee/gamelog.py:20–44/113–125`.

Required revision: name the committed 30 s two-game config or an exact `run_gauntlet.py` option that deterministically creates it, plus the exact fresh-checkout command. Specify that the check parses every turn attempt's `elapsed_ms`, rejects validation faults/voids, asserts `elapsed_ms <= 30000`, and verifies the emitted `info.depth`/`info.complete` staging metadata on the accepted published replies.

## Successor acceptance

Return revision 3 under `PLAN_LOCK_ID: PL-engine-c4-20260826`, parented again to `engine-c4-design-review-5`, answering this relay through `IN_REPLY_TO`, with a fresh plan locator/digest carrier. Preserve all closed M1–M6 repairs. Approval requires correct X/O midpoint ordinals with taint tests, a budget-bearing P1b analyzer plus a feature-complete runnable fitter seam, and an exact full-clock A11 command/config/parser contract.

Do not issue delegated implementation authority from this verdict. The orchestrator conditions still require an approving PLAN-REVIEW, an all-in scope diff, and no hard trigger or boundary deviation.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation dispatch or runtime action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation in review
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory and referee contracts
- user-visible-control-with-materializer/downstream-consumer: yes — engine analysis metadata has declared downstream consumers
- test-runtime-role-mismatch: yes — test oracle and production payoff/analyzer/fitter paths remain semantically incomplete
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — A9 and A11 cannot be evidenced from the current declared inputs/commands
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Read the incoming relay, exact plan revision diff, approved design, current position/wire adapter, theory continuous/discrete/LP callable surfaces, and referee CLI/tournament/game-log schema. — E1.
- Incoming relay exact-file lint is clean with historical freshness disabled; current plan/design locators and digests resolve exactly. — E1/E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-engine-c4-plan-review-2.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
