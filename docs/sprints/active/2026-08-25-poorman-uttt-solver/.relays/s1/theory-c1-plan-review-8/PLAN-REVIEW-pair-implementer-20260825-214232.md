ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-plan-review-8
PARENT_DISPATCH_ID: theory-c1-plan-8
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — one carrier gate correction remains; the addressed orchestrator ordering ruling is still pending
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-plan-8/PLAN-planner-20260825-213548.md
SUBJECT: PLAN-REVIEW must-revise — repairs-first cannot collapse into ruling-present
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd
PLAN_REVIEW_VERDICT: must-revise

## Verdict

Must revise the PLAN carrier before issuing a successor implementation dispatch. The underlying PL-theory-c1-20260825 bytes at SHA-256 `a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd` preserve the reviewed D8 schema, MR21 hold, scope, tests, and hard stops; the remaining defect is the carrier's execution-order branch.

The carrier says that if the ordering ruling has landed, the early slice executes first. That collapses two materially different rulings. A publication-first ruling permits the early slice with the pending repair obligations recorded. A repairs-first ruling requires the early slice and Task 11 to remain held until the engine and harness consumer repairs have actually landed. Mere existence of a repairs-first ruling is not publication authority.

The plan document itself states the correct contract at lines 609–610: publication-first or repairs-first (wait for them), and no ruling holds the early slice and Task 11. The successor carrier and any later implementation dispatch must preserve all three operational states rather than reducing them to ruling-present versus no-ruling.

This review does not authorize implementation, fixture publication, consumer repair, push, PR, merge, or release activity. The prior implementation token ended at the third STOP; no live implementation token is present here.

## Required revision

### PR12 — Make the execution-order branch ruling-sensitive

Reissue the PLAN carrier with the same approved design and plan lock IDs/digests unless the underlying bytes change, and replace the current binary ordering statement with these explicit branches:

1. Publication-first ruling landed: cite the exact addressed ruling; execute early slice → Task 8 → Task 10 → remaining Task 11 → Tasks 12–13; record both pending consumer-repair obligations honestly.
2. Repairs-first ruling landed but either consumer repair remains incomplete: cite the exact ruling; hold the early slice and Task 11; proceed with Task 8 → Task 10; resume Task 11 only after exact completion evidence for both owning-seat repairs is available; Tasks 12–13 close last.
3. Repairs-first ruling landed and both repairs are complete: cite the ruling and both completion artifacts, then execute the normal reviewed order.
4. No ruling landed: hold the early slice and Task 11; proceed with Task 8 → Task 10; Task 11 resumes only after a ruling and any conditions it imposes are satisfied; Tasks 12–13 close last.

The successor implementation dispatch must repeat the applicable branch and its evidence. Do not treat a request, CC visibility, or a repairs-first ruling without completed repairs as permission to publish.

## Preserved plan findings

- The PLAN correctly parents the approved design-review-12 relay and carries matching design/plan digests.
- Task 11 carries the canonical D8 byte shape and normative `consumed_by` rule.
- Tasks 8 and 10 remain independently executable while Task 11 is held.
- Task 12 and Task 13 retain the P1–P3 operator-ratification asks, publication-dependent waits, final E2 commands, explicit-path staging, PR-open/unmerged boundary, and math-lock handoff.
- `NonDeterminacy`, `LPEscalation`, and the N=24 fallback remain hard escalation gates.
- Engine/harness/ui code and merge remain out of scope.

## Evidence

- Routed integrity: design SHA-256 `232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689` and plan SHA-256 `a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd` match the carrier and current artifacts.
- Exact incoming relay lint with freshness disabled: clean.
- `git diff --check` for the current design and plan bytes: clean.
- Fresh held-branch verification: `uv run pytest -q` completed with `48 passed in 46.13s`; worktree remains clean at `theory/stage1-c1@f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`.
- Effective base remains `main@d89f0066381c21d7d4a569461af51bb0932faa8c`; the first theory commit parent is unchanged.
- No addressed response to the 211157 ordering request was present in the relay root at review time.
- Root-wide lint/verification is not claimed green; inherited/concurrent relay-root state is separate from this exact-file verdict.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — theory-only local package
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-theory analysis only
- AI-or-automation-acts-downstream: no — no autonomous external action
- worker/scheduler/queue/retry/async-side-effect: no — local batch solver only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — fixture schema and seed fixtures are consumed by sibling domains
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this bundle
- test-runtime-role-mismatch: no — Python implementation and local E2 verification use the same role
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — ordering and P1–P3 residual-risk acceptance remain externally gated
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested by this review
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: governance-only PLAN-REVIEW relay submitted through daemon admission; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M .superpowers/sdd/PL-ui-c1-20260825/progress.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-8/
