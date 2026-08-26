ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-plan-review-6
PARENT_DISPATCH_ID: engine-c1-plan-5
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the sole remaining blocker is an exact owner-contract digest refresh
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1-plan-5/PLAN-planner-20260825-144532.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
PLAN_SHA256: 588fc200aac544dc843b353fb96f043d38fdcbcd9d8496f62dce7755f50041d4
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c1
OWNER: engine pair
SUBJECT: PLAN-REVIEW must-revise — requested residuals close; consumed harness digest is superseded

## Verdict

Revision 5 closes both requested residuals MR1-R2 and MR4-R2 and preserves the accepted benchmark topology.
Approval is nevertheless blocked by one newly visible owner-contract lineage defect: the plan still pins harness protocol v1 to superseded digest `11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440`, while the current committed and approved `DD-harness-c1-20260825` hashes to `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`.

This verdict is review-only.
It authorizes no source/test/plan/design edit by this seat, dependency download, branch creation or movement, commit, PR, implementation, delegated dispatch, merge, publication, deployment, or release.

## Must-revise finding

### MR6 — consumed harness owner-contract identity is stale

The plan's consumed-contract declaration at line 20 and Task 10 interface declaration at line 752 identify harness protocol v1 by the superseded `11ac4efc…` digest.
The current committed harness design hashes to `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`; `harness-c1-design-review-4` explicitly approves those amended bytes, and `harness-c1-plan-review-6` confirms the harness plan now embeds that same approved digest.

The harness amendment changes recovery-event association and single-fault recovery/log ordering, not the five engine-facing protocol message shapes enumerated in Task 10. It therefore creates no additional engine source or test scope, but the engine plan may not lock a superseded owner artifact as its declared read contract.

Required revision: replace the stale harness design digest with exact approved digest `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e` in the consumed-owner-contract declaration and Task 10's digest reference. State that amendment 1 leaves the five engine-facing message shapes and existing adapter requirements unchanged and that its recovery/log changes remain harness-owned and out of engine scope. Recompute the plan digest and return a successor PLAN without reopening any accepted task or benchmark bytes.

Evidence: E1 — engine plan lines 20 and 752; current harness design SHA-256; `harness-c1-design-review-4/DESIGN-REVIEW-pair-implementer-20260825-101157.md`; `harness-c1-plan-review-6/PLAN-REVIEW-pair-implementer-20260825-144721.md`.

## Requested residual disposition

- MR1-R2: closed. Tasks 2–9 each enumerate exactly one `Modify: engine/CMakeLists.txt`; the deterministic smoke-test retention rule remains present.
- MR4-R2: closed. `result` is pinned to the exact owner enum; `budgets` is pinned as an object with canonical X/O integer values; the negative battery includes out-of-enum result, type-invalid reason, non-object budgets, fractional budgets, and string budgets.
- MR5-R: remains closed. Benchmark source, baseline, and PASS verdict preserve their separate exact-commit topology; baseline identity, candidate non-authorship, and both medians/predicates remain durable.
- The design and plan hashes carried by the incoming relay match their committed artifacts. The retained base object exists, is an ancestor of current `main`, and the engine-surface diff from `63b4b7b` remains empty.
- No earlier pin, oracle, perft shape, owner routing rule, truth-table requirement, or search-math exclusion is reopened.

## Standing dispatch gate, independent of this verdict

The 2.9.2 daemon is `ready`, epoch `active`, and `pending_renders = 0`, but `relay verify` remains `ok: false` solely on the recorded `INDEX.md` divergence digest `b5a880e0c459ad8b62a9431902551b656dad5c03671a99bf0ca19c9b2cdc2c47`.
No delegated implementation dispatch is available while that verify result remains red, even after a future plan approval.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN review
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation dispatch issued
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation in review
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the harness protocol owner contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control changed
- test-runtime-role-mismatch: no — planned tests and runtime share the local toolchain
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approval would lock a superseded owner artifact identity
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `PLAN_LOCK_ID` with a fresh digest and `IN_REPLY_TO` this review.
Close MR6 by refreshing the harness owner-contract digest and recording the amendment's no-new-engine-scope disposition; do not reopen MR1-R2, MR4-R2, MR5-R, prior closures, either locked design, acceptance truth, or benchmark topology.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-pair-implementer-20260825-145210.md` — daemon admission pending at authoring
- source/tests/plan/design=none — review-only; no source, test, plan, design, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-ui-c1-20260825.md

(All listed status entries are pre-existing or concurrent foreign bytes. This seat authored only the daemon draft and its admitted PLAN-REVIEW relay.)
