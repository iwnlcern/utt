ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: engine-c3-impl-report-1
PARENT_DISPATCH_ID: engine-c3-impl-1
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — engine.planner can reconcile this report; merge remains a separate orchestrator/operator gate
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c3-impl-1/IMPL-planner-20260826-150913.md
PLAN_LOCK_ID: PL-engine-c3-20260826
PLAN_ARTIFACT: PL-engine-c3-20260826
PLAN_SHA256: 9523a2162a5896e2b14a42c24d105ca99a3aae86046fd5a78cc112a3060b9914
BUNDLE_ID: engine-c3
OWNER: engine pair
REPO: utt (origin https://github.com/iwnlcern/utt)
BASE: a8bd38c79ee833b0aecf773950451026ffc8aba2
TARGET_BRANCH: main
BRANCH: engine/fixture-repair-c3
PR: https://github.com/iwnlcern/utt/pull/8 — OPEN, non-draft, MERGEABLE/CLEAN at exact head 5a56019fa5dfffbd9d68e9f992855c0ee6f205f9
SUBJECT: IMPLEMENTATION REPORT — engine-c3 canonical fixture reader complete at 5a56019; all local rows green, standing main-fixture row pending

## Summary

PL-engine-c3 rev 2 is implemented on `engine/fixture-repair-c3` at `5a56019fa5dfffbd9d68e9f992855c0ee6f205f9`, pushed to origin, and open as PR #8 against main. The exact base-to-head diff is the single authorized file `engine/tests/test_fixtures.cpp`.

The runner now requires canonical `move_by` with no default, parses strictly ordered `{local,result}` closure records and checks both the ordered local list and X/O/full child-position facet, maps terminal kinds to `null | "macro_win" | "all_closed"`, and rejects boolean terminal values as producer defects. Pure `std::expected<T,std::string>` helpers carry exact named errors; direct negative tests prove those errors while `run_engine_fixture` converts them to fatal `REQUIRE_MESSAGE` failures for real defective fixtures. Existing legal-move, expected-result, chip-margin, and consumed-by behavior is preserved.

No producer fixture, theory byte, second engine file, harness file, core file, schema, contract, merge, publication, deployment, or release action was performed.

## Acceptance truth table

```text
c3-A1 required move_by/no default       -> green-E2 (direct missing/invalid exact-error negatives; validation mutation made the focused test red)
c3-A2 ordered closure records/facets    -> green-E2 (ordered/disordered direct helper tests; exact theory fixtures exercise X and full facets; ordering mutation made the focused test red)
c3-A3 canonical terminal kinds          -> green-E2 (None/MacroWinX/MacroWinO/AllClosed synthetic mapping; boolean exact-error negative; validation mutation made the focused test red)
c3-A4 full unit plus honesty split       -> green-E2 (override unit 30/30 cases, 3,134,563/3,134,563 assertions; literal run 29/30 with its sole failure the unchanged fixture-absence message)
c3-A5 exact theory-head writer-reader    -> green-E2 (non-main branch bytes at d421e14533bd4cc572578d6e483b2291b90a6694; fixture case 1/1 and 69/69, full unit 30/30 and 3,134,632/3,134,632)
c3-A6 exact one-file scope               -> green-E1 (base-to-head diff and staged set each exactly engine/tests/test_fixtures.cpp; diff-check clean)
standing engine criterion 1 (main theory fixtures) -> pending-blocked (literal main-state unit fails only: theory fixtures not present — acceptance criterion 1 is unmet until theory publishes)
```

No c3-A row is pending. The exact-theory-head proof is explicitly non-main branch-byte evidence and does not mark the standing main-fixture criterion green.

## TDD, verification, and review evidence

- Initial test-first compile failed on the three absent locked helpers, confirming the new tests preceded helper implementation. After adding the helper interfaces while leaving the legacy runner untouched, the required behavioral RED executed 5 cases: 4 passed, 1 failed; 34 assertions: 32 passed, 2 failed, exactly at old closure-vector and boolean-terminal decoding. — E2.
- Focused GREEN after wiring the runner: 5/5 cases and 36/36 assertions passed. — E2.
- Mutation guard: replacing missing `move_by` rejection with the old X default made the move-by negative fail; disabling closure ordering validation made the disorder negative fail; accepting the boolean terminal value made the terminal negative fail. All three intended validations were restored and the full suite rerun green. — E2.
- Fresh post-commit absence-override unit: 30/30 cases and 3,134,563/3,134,563 assertions passed. — E2.
- Fresh literal unit with fixture override and fixture path unset: exit 1; 29/30 cases passed and 3,134,563/3,134,564 assertions passed; the only failure is the unchanged `theory fixtures not present — acceptance criterion 1 is unmet until theory publishes` honesty message. — E2, standing criterion pending.
- Theory writer-reader worktree was clean at exact head `d421e14533bd4cc572578d6e483b2291b90a6694`. Focused theory fixture case passed 69/69 assertions; full unit passed 30/30 cases and 3,134,632/3,134,632 assertions. — E2, non-main branch bytes.
- Fresh repository CTest with only the theory-fixture absence override: unit passed; e2e remained red solely on the pre-existing out-of-scope harness stream `fault-eof_or_crash.X.jsonl` (`stream must contain exactly one hello`). That base failure existed before engine-c3 edits and no harness/e2e byte is in this scope. — E2, unrelated retained baseline.
- `git diff --check a8bd38c..5a56019` exited 0. The exact committed and pushed path set is `engine/tests/test_fixtures.cpp`; worktree status is empty. — E1/E2.
- Independent read-only whole-diff review found no Critical, Important, or Minor issues and assessed the branch ready for the separately authorized merge step. The reviewer independently confirmed theory head/schema bytes and exact one-file scope. — E1.
- Remote branch resolves to exact head `5a56019fa5dfffbd9d68e9f992855c0ee6f205f9`; PR #8 is OPEN, non-draft, MERGEABLE/CLEAN against main at that head. — E1.

## Boundary contract proof

Writes: canonical fixture-consumer behavior and regression proof in `engine/tests/test_fixtures.cpp` only.
Reads: theory's required `move_by`, ordered closure records with result, terminal kind, and normative `consumed_by` contract.
Target entity: the engine fixture runner's interpretation and assertion of theory-owned UTTT fixture bytes.
Downstream consumer: engine unit/fixture regression suite and the theory publication merge fence.
Contract: chip-margin, expected-result, legal-move, and consumed-by behavior remains byte-unchanged; only the theory-owned move/closure/terminal shapes are reduced internally.
Proof: helper-negative E2, behavioral RED/GREEN, mutation guards, owner-independent unit E2, and exact-theory-head writer-reader E2.
No-consumer action: producer bytes were read only; absent main fixture publication remains pending rather than copied or treated as green.

## Scope, risks, and next action

SCOPE_DIFF:
- engine/tests/test_fixtures.cpp -> in
SCOPE_DIFF_RESULT: all-in

Remaining risk: the standing engine criterion 1 stays pending until theory publishes the exact fixture bytes to main and the literal unoverridden unit suite is rerun there. Separately, origin/main's harness-stream e2e baseline remains red on `fault-eof_or_crash.X.jsonl`; engine-c3 neither caused nor repairs that foreign surface.

Next requested action: engine.planner reconcile c3-A1 through c3-A6 as green and route the exact head/PR to the governing seats for a separately addressed merge gate. Preserve the worktree and branch for review; do not infer merge authority from this report or PR state.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local fixture consumer test only
- migration/backfill/destructive-write/canonical-data-repair: no — no data or producer fixture rewrite
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic simulated-game tests only
- AI-or-automation-acts-downstream: no — test fixture consumption only; no acting runtime changed
- worker/scheduler/queue/retry/async-side-effect: no — no worker or asynchronous side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the theory-owned fixture schema
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — exact producer-byte proof exercises the real engine test binary
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — standing main-fixture publication/rerun remains pending and foreign e2e baseline remains outside scope
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested; the governing ruling assigned this bounded cross-contract repair
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: engine/fixture-repair-c3@5a56019fa5dfffbd9d68e9f992855c0ee6f205f9; exact diff and staged set are `engine/tests/test_fixtures.cpp` only; branch pushed to origin and PR https://github.com/iwnlcern/utt/pull/8 opened at the exact head; no merge, publication, deployment, or release action claimed
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree
