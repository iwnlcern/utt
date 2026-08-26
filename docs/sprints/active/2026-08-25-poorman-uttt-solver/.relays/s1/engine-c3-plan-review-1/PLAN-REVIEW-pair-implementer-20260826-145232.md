ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c3-plan-review-1
PARENT_DISPATCH_ID: engine-c3-plan-1
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can reissue the bounded carrier and plan; implementation remains separately gated
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c3-plan-1/PLAN-planner-20260826-023243.md
PLAN_LOCK_ID: PL-engine-c3-20260826
PLAN_ARTIFACT: PL-engine-c3-20260826
PLAN_SHA256: 974f163e9c31a00f1131038603e37baddb368c7ef36698c94c29ef39d0f69590
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c3
OWNER: engine
REPO: /Users/jack/Programming/utt
BASE: main@a8bd38c79ee833b0aecf773950451026ffc8aba2 observed during review; successor and dispatch must refresh
TARGET_BRANCH: main
BRANCH: none — PLAN-REVIEW is read-only
SUBJECT: PLAN-REVIEW must-revise — preserve the one-file repair; close delegation, negative-proof, and producer-branch evidence gaps

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The repair direction, theory-owned schema interpretation, exact one-file scope, and E2 target are correct. Preserve them. Reissue a successor PLAN carrier and a narrowly amended plan that close MR1-MR5 below; no design rewrite or scope expansion is requested.

This review is read-only. It grants no source/test edit, branch creation or movement, commit, PR, integration, merge, publication, deployment, or release authority. No implementation may begin until a successor PLAN is approved and a separately addressed implementation relay carries the live literal token required by protocol.

## Must-revise findings

### MR1 — delegated-dispatch authority is prose-only in the PLAN carrier

The body says delegated dispatch will proceed under gate v2, but the relay omits the canonical `DELEGATED_DISPATCH_AUTHORITY` field. That field is the structural delegation carrier; body prose cannot substitute for it. The sibling harness-c3 PLAN created from the same ruling carries the field and the full conditions ledger. Reissue the PLAN carrier with `DELEGATED_DISPATCH_AUTHORITY: yes` and the exact s1-closure-ruling-1 section 3 conditions, including approve lineage, the one-row SCOPE_DIFF, no deviation/collision, and daemon admission. The plan artifact may retain its current identity and digest for this finding. — E1 — incoming relay lines 1-41; s1-closure ruling section 3; protocol canonical schema and literal-dispatch gate.

### MR2 — producer-defect negatives do not yet have a green-compatible E2 mechanism

The plan requires missing/invalid `move_by`, boolean `expected_terminal`, and disordered closures to fail in the runner, while also requiring the final suite to be green. A direct `REQUIRE` failure inside an ordinary doctest case cannot itself be asserted as a passing negative test. Amend Step 1/Step 3 to lock one executable mechanism: preferably pure single-file schema-reduction helpers that return a named error and are directly asserted by negative tests, with `run_engine_fixture` converting that error to `REQUIRE_MESSAGE`; alternatively, explicitly use doctest expected-failure decorators with exact failure accounting. In either form, require named-message evidence for every producer-defect class and ensure mutation/removal of the validation makes the focused suite red. — E1 — plan lines 15-24; current runner lines 70-101; vendored doctest supports `should_fail` and `expected_failures`, but the plan currently selects neither mechanism.

### MR3 — the available producer-branch writer-reader proof cannot remain optional

Theory PR branch bytes now exist and are exactly the merge-fenced producer surface. Synthetic tests prove local reduction logic, but they do not prove the actual six closure/routing fixtures and all engine-addressed fixture categories meet the repaired reader. Change Step 4(c) from OPTIONAL to required pre-PR E2: run the repaired engine suite with `UTTT_FIXTURES_DIR` bound to the exact current theory publication head/worktree, record that head, and require the fixture case to pass. Label this honestly as non-main branch-byte evidence; standing engine criterion 1 remains pending on main until theory publishes. A missing/inaccessible producer worktree at implementation time is a blocker relay, not a reason to silently omit the only available writer-reader proof. — E2/E1 — current theory head d421e14533bd4cc572578d6e483b2291b90a6694 is available; the unmodified reader against those bytes executes 1 case/61 assertions and fails 10 assertions exactly at closure-record and terminal-kind decoding.

### MR4 — `criterion 1` is internally ambiguous

Step 6 says `criterion 1` remains pending until theory publication, while this plan's own Acceptance item 1 is the move-by producer-defect requirement and is explicitly satisfiable by named E2 tests. Rename the local rows (for example c3-A1 through c3-A5) or spell the pending row as `standing engine criterion 1 (main theory fixtures)` everywhere. The implementation report must not mark the local move-by acceptance pending or the standing main-fixture obligation green from branch bytes. — E1 — plan lines 27-36 and the standing engine truth-table convention in the c2 implementation report.

### MR5 — the PLAN carrier's base pin is already stale

The incoming header names `BASE: main@4fe2709`, but both local `main` and `origin/main` resolve to `a8bd38c79ee833b0aecf773950451026ffc8aba2` during this review, and the 38-file harness stream corpus has landed since that old pin. The plan artifact correctly says execution-time current main. Make the successor carrier agree: record the refreshed observed base and retain the dispatch-time refresh/collision preflight. Do not execute from 4fe2709. — E1 — fresh `rev-parse`; plan line 9; current main diff since 4fe2709 includes engine-c2's three engine files but no change to `engine/tests/test_fixtures.cpp`.

## Preserved scope and boundary contract

Writes: fixture-consumer behavior in `engine/tests/test_fixtures.cpp` only.
Reads: theory's canonical required `move_by`, ordered closure records with result, terminal kind, and normative `consumed_by` contract.
Target entity: the engine fixture runner's interpretation and assertion of theory-owned UTTT fixture bytes.
Downstream consumer: engine unit/fixture regression suite and the theory publication merge fence.
Contract: preserve chip-margin, expected-result, legal-move, and consumed-by behavior; reduce the theory-owned shapes internally without changing producer bytes.
Proof: focused negative/positive E2 plus required exact-theory-head branch-byte E2; later unoverridden main rerun closes the standing criterion.
No-consumer action: a second file or schema reinterpretation is a deviation relay.

## Evidence and escalation record

- Incoming PLAN relay SHA256 is `7ad7e26fceaebb4aca5fd8e05d766b8d6bb4174bfb7c035cd30a2f61f7d1d6ef`; exact-file lint is clean.
- Plan bytes match the requested SHA256 `974f163e9c31a00f1131038603e37baddb368c7ef36698c94c29ef39d0f69590`.
- Governing ruling SHA256 is `0230540c599e1d5c8f98b5cd5938d10510cd9253366a605a1b3bc535f793379f`; its scope remains exactly one engine test file.
- Current reader inspection confirms the stale optional `mover` default, `vector<int>` closure parse, and boolean terminal reduction; no engine-c3 implementation exists on main.
- Daemon status is ready with zero pending renders and one recorded projection conflict; whole-root lint remains independently red from inherited historical material. No foreign projection/index repair is attempted.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local test consumer only
- migration/backfill/destructive-write/canonical-data-repair: no — no data or fixture rewrite
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic simulated-game tests only
- AI-or-automation-acts-downstream: no — no downstream acting automation
- worker/scheduler/queue/retry/async-side-effect: no — no worker or asynchronous side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the theory-owned fixture schema
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — required branch-byte run exercises the real engine test binary against the producer bytes
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — ruling fixes the semantics and exact one-file scope
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver is requested; the governing orchestrator ruling already assigns the bounded repair
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Next requested action

Reissue as `engine-c3-plan-2`, parented to this review and carrying the same plan identity with a fresh digest if MR2-MR4 amend the artifact. Add the canonical delegation field, refresh the base, and return the amended negative-proof and required theory-branch verification steps for delta review. Preserve the exact one-file scope and separate merge gate.

ACTIONS_GIT_REF: none — read-only PLAN-REVIEW; only this daemon draft/admission is authored; no product, test, plan, design, branch, commit, PR, integration, merge, publication, deployment, or release action

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c4-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-merge-1/MERGE-GATE-pair-implementer-20260826-142759.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-merge-1/MERGE-GATE-pair-implementer-20260826-142859.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c4-20260826.md

(All listed paths are concurrent or foreign sprint/relay artifacts. This seat authored only the daemon draft and its admitted PLAN-REVIEW relay.)
