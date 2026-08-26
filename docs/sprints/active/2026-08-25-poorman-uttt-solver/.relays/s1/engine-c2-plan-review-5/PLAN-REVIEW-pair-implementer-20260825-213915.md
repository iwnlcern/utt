ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c2-plan-review-5
PARENT_DISPATCH_ID: engine-c2-plan-4
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the recursive owner pin is accepted; the three blockers are narrow PLAN corrections within engine.planner authority
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, harness.planner
IN_REPLY_TO: engine-c2-plan-4/PLAN-planner-20260825-213123.md
PLAN_LOCK_ID: PL-engine-c2-20260825
PLAN_ARTIFACT: PL-engine-c2-20260825
PLAN_SHA256: 0a57c7fbed6757215d74e9bcbb1f03d1de6751f68d1a4981d78fd067e464a2c6
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: Delta PLAN-REVIEW must-revise — make rev 4 internally recursive, current-owner-attributed, and seat-consistent

## Verdict

The rev-4 direction is mechanically sound: the owner-corrected recursive glob, recursive-selection proof, exclusion sentinels, mutation expectation, unchanged strict shape/pending/failure semantics, Task 1 carry-forward, and three-file fence are accepted. The delta is not implementation-ready at this digest because three operative bytes remain inconsistent with that direction.

This is a read-only delta review. It authorizes no source, test, plan, design, branch, commit, PR, integration, publication, or deployment action by this seat. The Task 2/3 hold remains in force pending a corrected same-lock successor, its approval, and the separately promised `engine-c2-impl-2` implementation dispatch.

## Must-revise findings

### MR1 — The operative discovery instruction still says single-level

Task 2's discovery bullet says to match the pinned "single-level glob" even though the new constant and owner ruling require recursive traversal. That instruction directly conflicts with the rev-4 delta and could reintroduce the exact defect the revision is intended to close. — E1 — committed plan line 59; rulings-3 lines 24–28.

Required revision: replace "single-level glob" with unambiguous recursive wording, such as "recursive glob over the engine-stdin subtree," while retaining lexicographic sorting.

### MR2 — The source comment attributes the active pin to superseded rulings-2

The exact comment block the plan instructs the Implementer to add still labels the corpus locations as pinned by `harness-c1-contract-rulings-2`. Rulings-3 explicitly supersedes rulings-2's flat glob and is the active owner pin. The inline constant comment is current, but the leading normative attribution is not. — E1 — committed plan lines 48–55; rulings-3 lines 22–28.

Required revision: attribute the active corpus-location pin to `harness-c1-contract-rulings-3`; rulings-2 may be mentioned only as superseded history if useful.

### MR3 — The proposed nested `.O` stream reuses X-seat literals

The nested selftest path is `engine-stdin/parity-even/deep.O.jsonl`, but the plan directs both valid streams to reuse the file's known-good literals. Those existing HELLO and TURN literals carry `"you": "X"`; the owner mapping makes the `.X`/`.O` suffix the stream seat. A purported valid `.O` owner stream carrying X-seat messages makes the positive proof internally inconsistent. — E1 — committed plan line 63; current `engine/tests/test_engine_e2e.py` lines 11–30; rulings-3 line 24.

Required revision: either name the nested fixture `deep.X.jsonl`, or pin O-consistent HELLO and TURN literals for `deep.O.jsonl`. Keep the flat and nested positions and all existing recursive/exclusion/mutation assertions.

## Retained delta and execution state

- Recursive `CORPUS_GLOB = "referee/tests/fixtures/engine-stdin/**/*.jsonl"` is accepted exactly as owner-pinned. — E1.
- Flat plus nested positive discovery, `.stderr` exclusion, outside-subtree `.jsonl` exclusion, strict stream shape, real-subprocess replay, and flat-glob mutation RED remain accepted. — E1.
- Task 1 is unaffected and was completed under `engine-c2-impl-1` at commit `9d6ae53957fe0b81d373a0d3891c2d49db0e0d72`. — E2.
- The rev-3 Task 2 implementation exists only as held branch commit `12b7c94e1b5362488ad1ff38ab852273e16b535e`; it is not claimed current, approved, reported, or integrated. No source/test edits were made after this hold was discovered. — E1.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local PLAN delta review
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game protocol only
- AI-or-automation-acts-downstream: no — no implementation authority in this review
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the harness-owned stream contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control changed
- test-runtime-role-mismatch: no — the selftest uses the real local engine subprocess
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — all findings are mechanical corrections under the existing owner pin
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `PLAN_LOCK_ID` with a fresh digest and close MR1–MR3 without changing the accepted recursive owner pin or reopening rev-3 content. Then request a delta PLAN-REVIEW. Only after approval may engine.planner issue the promised successor `engine-c2-impl-2` for Tasks 2–3. The current hold remains active meanwhile.

ACTIONS_GIT_REF: none — read-only delta PLAN-REVIEW; only daemon draft/admission authored; no product, test, plan, design, branch, commit, PR, integration, publication, or deployment action in this phase

FINAL_GIT_STATUS_SHORT: dirty only from concurrent foreign sprint/relay work in the shared main worktree; implementation worktree clean at `12b7c94e1b5362488ad1ff38ab852273e16b535e`
