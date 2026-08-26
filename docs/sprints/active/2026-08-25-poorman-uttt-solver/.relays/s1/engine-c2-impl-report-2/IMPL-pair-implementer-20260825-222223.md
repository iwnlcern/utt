ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: engine-c2-impl-report-2
PARENT_DISPATCH_ID: engine-c2-impl-2
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — planner can reconcile this honestly-pending cycle; owner-artifact publication and any integration remain separately gated
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, harness.planner
IN_REPLY_TO: engine-c2-impl-2/IMPL-planner-20260825-220537.md
PLAN_LOCK_ID: PL-engine-c2-20260825
PLAN_ARTIFACT: PL-engine-c2-20260825
PLAN_SHA256: 54cf88aed6558e1650b2942f13843698cfb949e10ad38af02a7493458ed1c382
BUNDLE_ID: engine-c2
OWNER: engine pair
REPO: utt (origin https://github.com/iwnlcern/utt)
BASE: c26680bbae619728fbfab8622e18080e25be3934
TARGET_BRANCH: main
BRANCH: engine/consumer-align-c2
PR: none — dispatch requires branch plus report; push, PR, and integration are separately gated and were not attempted
SUBJECT: IMPLEMENTATION REPORT — rev-5 consumer alignment at 2ed3ce6; criteria 1/3 honestly pending on owner artifacts

## Summary

Tasks 1–3 are implemented and verified on the clean branch `engine/consumer-align-c2` at `2ed3ce699eab752bb4fe85438faa43d836868a70`, based on exact `c26680bbae619728fbfab8622e18080e25be3934`.

Task 1 is commit `9d6ae53957fe0b81d373a0d3891c2d49db0e0d72`: ply-0 rejects non-4/null `forced` and non-null `tie_owner` with the two locked strings, while the ply>0 regression remains valid. Task 2 is amended commit `2ed3ce699eab752bb4fe85438faa43d836868a70`: discovery uses the rulings-3 recursive config point, matched owner streams fail closed on shape defects, raw stdin bytes are replayed in isolated real engine processes, and the owner-independent selftest proves flat plus nested selection and both exclusions.

No fourth file, owner artifact, log reconstruction, schema, contract, core, benchmark, harness, or theory byte was changed. No push, PR, integration, publication, deployment, or release action was attempted.

## Acceptance truth table

```text
criterion 1 (theory fixtures)          -> pending-blocked (`test ! -d theory/fixtures` exit 0; literal unit failure: `theory fixtures not present — acceptance criterion 1 is unmet until theory publishes`)
criterion 2 (owner-independent suite)  -> green-E2 (both allowances set: CTest exit 0, 2/2 passed, 0 failed)
criterion 3 (message-stream round-trip)-> pending-blocked (active recursive glob `referee/tests/fixtures/engine-stdin/**/*.jsonl` match count = 0; literal e2e error: `harness message-stream corpus not present — criterion 3 pending-blocked`)
discovery/shape selftest (MR2)         -> green-E2 unconditionally (synthetic flat plus nested discovery/shape/replay selftest executes before the real-corpus check; overridden e2e passed)
criterion 4 (benchmark)                -> unchanged from c1 (no bench edits; committed verdict PASS at reference 883.24801025 ns/ply, candidate 50.47830225 ns/ply, ratio 0.05715076814689037, ratio predicate true, absolute 100 ns predicate true, baseline identity/session equal)
ply-0 validation (this cycle's new)    -> green-E2 (`adapter ply-0 contract`: exit 0, 1/1 case and 7/7 assertions passed)
discovery alignment (this cycle's new) -> green-E2 (rulings-3 recursive config point plus owner-independent e2e/selftest passed; active owner glob currently has zero files)
```

No overridden, skipped, copied-example, or sibling-worktree result is summarized as criterion 1 or criterion 3 acceptance-green. Both pending rows close only by rerunning the literal unoverridden suite after their owning artifacts land.

## TDD and verification

- Task 1 RED: the dedicated case executed exactly one test case; three locked rejection assertions failed while the ply>0 guard passed. GREEN: 1/1 case and 7/7 assertions passed after the two minimal adapter guards. — E2.
- Task 2 RED under the held flat constant: the revised selftest required `synthetic.X.jsonl` plus nested `parity-even/deep.X.jsonl`; discovery omitted the nested stream and the exact-set assertion failed. GREEN under the recursive constant: both streams were selected and replayed through separate real engine subprocesses; both exclusion sentinels stayed absent. — E2.
- Clean build: 42 Ninja steps rebuilt successfully before the evidence run; the final fresh build reported no work outstanding. — E2.
- Literal unoverridden CTest: exit 8, exactly 2/2 tests failed, solely from the named absent theory fixtures and absent message-stream export. Unit recorded 25/26 cases and 3,134,532/3,134,533 assertions; e2e terminated only at the exact criterion-3 pending assertion after its synthetic selftest. — E2, honestly pending.
- Fully overridden owner-independent CTest: exit 0, 2/2 registered tests passed, 0 failed. This proves owner-independent behavior only. — E2.
- Focused ply-0 test: exit 0, exactly 1/1 case and 7/7 assertions passed. — E2.
- Owner checks: `theory/fixtures` absent; active recursive harness stream glob count 0. — E2.
- Benchmark artifact verification: committed verdict `PASS`; both predicates true; reference/candidate medians and baseline identity/session agree; `jq -e` identity/predicate comparison exited 0. No benchmark path differs from the base. — E2.
- Scope and hygiene: `git diff --check c26680b..HEAD` exited 0; the exact base-to-head changed set is `engine/src/adapter/wire.cpp`, `engine/tests/test_engine_e2e.py`, and `engine/tests/test_wire.cpp`; final branch worktree status is empty. — E1/E2.
- Task review, Task 3 evidence review, and final whole-branch review each returned no Critical, Important, or Minor findings; the final assessment was ready subject only to this report and separate integration authority. — E1.

## Boundary contract proof

Writes: engine adapter validation behavior and engine-owned consumer tests only.
Reads: harness rulings-3 recursive stream-location/raw-stdin contract, the normative legacy transcript location, and theory fixture presence.
Target entity: the engine's protocol-consumer validation and corpus-discovery paths.
Downstream consumer: local CTest/e2e acceptance and later harness-export replay.
Contract: ply 0 requires `forced: 4` and `tie_owner: null`; corpus streams are bare hello/turn/game_end raw bytes under the recursive engine-stdin subtree; every match must validate and replay, and only zero matches may pend.
Proof: E2 focused wire tests, real-process synthetic recursive selftest, literal/overridden CTest split, and active owner-artifact absence measurements.
No-consumer action: the absent harness export is pending-blocked, not reconstructed, copied, or treated as green.

## Scope, risks, and next action

SCOPE_DIFF:
- engine/src/adapter/wire.cpp -> in
- engine/tests/test_wire.cpp -> in
- engine/tests/test_engine_e2e.py -> in
SCOPE_DIFF_RESULT: all-in

Remaining risk: criteria 1 and 3 remain pending exclusively on `theory/fixtures` and at least one owner-published stream matching the active recursive glob. Producer defects will fail rather than convert to pending once files exist.

Next requested action: engine.planner reconcile this report and route the owner-artifact rerun dependency. Preserve the branch pending a separately addressed integration gate.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no credential or permission surface changed
- migration/backfill/destructive-write/canonical-data-repair: no — local code/tests only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: yes — the engine adapter emits a move/bid reply through the harness protocol
- worker/scheduler/queue/retry/async-side-effect: no — synchronous local subprocess tests only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the harness-owned stream contract
- user-visible-control-with-materializer/downstream-consumer: no — no UI or deployed materializer changed
- test-runtime-role-mismatch: no — tests invoke the real local engine subprocess
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — two owner-owned acceptance artifacts remain absent and integration is separately gated
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: engine/consumer-align-c2@2ed3ce699eab752bb4fe85438faa43d836868a70; commits `9d6ae53957fe0b81d373a0d3891c2d49db0e0d72` and `2ed3ce699eab752bb4fe85438faa43d836868a70`; exact diff is the three in-scope files above; no push, PR, integration, publication, deployment, or release action claimed
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree
