ROLE: Pair Implementer
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: engine-c4-impl-blocker-1
PARENT_DISPATCH_ID: engine-c4-impl-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — engine design owners can issue/review a successor while preserving G4; changing the bar would require a separate operator gate
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c4-impl-1/IMPL-planner-20260827-033148.md
SUBJECT: IMPL STOP — locked production matrix loses A10 information; design ruling required
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: 12044c07af257146563be24f74cfeb93b5300f826015f39465cc682d32dc1b48
BLOCKER: A10 strength conflicts with information retained by the locked production in-band payoff/extraction seam

## Stop

Implementation stops at Task 14 with A10 RED. A1-A9 remain green at E2 and A11 is green at E2+E3, but the locked product does not reach 90% points against every baseline. The plan requires one clean four-baseline pass before commit/push/PR. No Task 14 commit, push, PR, merge, publication, or design-semantic change occurred.

The reviewed Task 13 product remains at `afb91d529258f1e38e27484007b9d5a090b796af`. The only retained working-tree additions are five engine-owned gauntlet configs and `engine/tools/run_gauntlet.py`, intentionally uncommitted while acceptance is red.

## Fresh evidence

All acceptance commands explicitly unset `UTTT_ALLOW_MISSING_FIXTURES`, `UTTT_ALLOW_MISSING_CORPUS`, `UTTT_FIXTURES_DIR`, `UTTT_FIXTURE_ROOT`, and `UTTT_CONTRACT_FIXTURE_ROOT`.

- Full unit: 124/124 cases, 3,667,708 assertions, zero failed/skipped.
- CTest: 2/2 (`unit` 27.68 s, `e2e` 21.62 s), 49.30 s total.
- A11 real full-clock: two seat-swapped games, faults 0, voids 0; all 94 engine attempts valid and `complete: true`, depths 3-4, maximum elapsed 40 ms <= 30,000 ms.
- Restored-product comparisons for policy, root-matrix test, and weights against `afb91d5` are empty.
- Fresh adversarial review independently reproduced exact-five-unset unit 124/124 and CTest 2/2, inspected the retained latest A10/A11 logs and six-file runner/config fence, and returned `Approved-blocked` with no Critical/Important/Minor findings. Attempts 1-3 were overwritten by the fixed output directories and remain implementer-report evidence only; the retained latest fraction 0/50 red is independently sufficient to block A10.

Four honest A10 attempts used 50 paired games per reached baseline and stopped at the first red matchup:

1. Locked product: random 50/50; zero 28/50 RED; faults/voids 0.
2. Bid-1 known-opponent-tie experiment: random 50; zero 50; fraction 0/50 RED; faults/voids 0.
3. Known-opponent-tie S/4 experiment: random 50; zero 50; fraction 25/50 RED; faults/voids 0.
4. Any-known-tie S/4 experiment: random 50; zero 50; fraction 0/50 RED; faults/voids 0.

Every experiment and test was removed after failure. A disclosed fitted-weight candidate left the initial profile unchanged and scored 0/10 against fraction; it was not adopted. On restored Task 13 bytes, an independent 50-game allin_tactical diagnostic scored 50/50 with faults/voids 0; that partial diagnostic does not turn A10 green.

## Locked-seam collision

DD section 7 and Plan Task 13 require each production in-band child payoff to be `sign(post-payment X share - child sound-interval midpoint)`. At the real opening, child sound intervals are `[0,1]`, hence midpoint 0.5. An isolated run-and-revert diagnostic on the real 28x28 matrix found:

- four X bid-0 rows each have payoff range `[0,+1]` and strategy mass `0.24996720779220796`;
- positive X bids admit `-1`, while full-stack rows are `[-1,-1]`;
- O is symmetric, with about 99.98% total mass on four bid-0 actions;
- all four bid-0 moves have equal maximum probability, so extraction selects the first/preferred move;
- fitted weights do not alter the sound `[0,1]` midpoint or this profile.

RM+ solves the coarse matrix it receives, and sampling its averaged profile still yields bid 0 almost always. The failures are not faults, voids, clock overruns, nondeterminism, runner defects, or a tunable fit miss. Clearing A10 requires richer production payoff/move guidance or an explicitly authorized non-profile pressure/opponent model. Either changes locked semantics; choosing locally would exceed this Implementer's authority and violate the no-silent-workaround line.

## Requested resolution

Please route a successor design ruling that:

1. chooses richer production in-band guidance or an explicit policy outside the RM+ profile;
2. preserves exact solved-scale A8 labels/payoffs/coordinates and operator-locked G4;
3. identifies the exact source/test fence and mutation-sensitive production witnesses;
4. grants successor PLAN/IMPL authority at `engine/search-math-c4@afb91d529258f1e38e27484007b9d5a090b796af` plus the six uncommitted Task 14 tool paths; and
5. requires focused/full E2, one clean four-baseline A10, A11 rerun, then commit/push/exact-title PR only if all rows are green.

The full local evidence/truth table is at `.superpowers/sdd/PL-engine-c4-20260826/task-14-implementer-report.md`; it is not substituted for this carrier.

## Preserved state

- Theory, referee, harness, docs, fixture, protocol, and product source/test bytes remain unchanged from approved Task 13.
- Only the five configs and runner remain uncommitted.
- A10 is RED; A11 is GREEN. No override, partial matchup, or diagnostic is summarized complete.
- No Task 14 commit, push, PR, merge, deployment, or release occurred.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game engine only
- migration/backfill/destructive-write/canonical-data-repair: no
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated budgets only
- AI-or-automation-acts-downstream: no
- worker/scheduler/queue/retry/async-side-effect: no — local tournament only
- cross-repo/service-contract/generated-schema/shared-API-event: no — referee consumed read-only
- user-visible-control-with-materializer/downstream-consumer: no
- test-runtime-role-mismatch: yes — E2 sound payoff lacks information for locked E3 strength
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — local remedy would invent production policy; A10 stays red
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — addressed successor design ruling required
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: engine/search-math-c4@afb91d529258f1e38e27484007b9d5a090b796af; six Task 14 paths uncommitted; no push/PR/merge
FINAL_GIT_STATUS_SHORT:
?? engine/tools/gauntlet/
?? engine/tools/run_gauntlet.py
