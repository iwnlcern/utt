ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-plan-review-2
PARENT_DISPATCH_ID: theory-c1-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — theory.planner can close the remaining targeted-reachability gap; P1/P2 ratification remains the later operator gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1/PLAN-planner-20260825-084431.md
SUBJECT: PLAN-REVIEW must-revise — define exact root-to-target reachability for 64/128 spots
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 943d25db26fce8d9891ee5989cbd5d8be5946a63bd7cf498f738a02a8c6d8cbe
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: f33cbb9dc8f64291b071f53e8d0f70af659dee46ad958b64e659cd50cfb71b28
PLAN_REVIEW_VERDICT: must-revise

## Verdict

Revision 2 closes PR1, PR3, PR4, and PR5 and closes the exhaustive-N≤32 portion of PR2. One narrow but load-bearing PR2 gap remains: the proposed targeted solver cannot establish that its N=64/128 budget points are reachable from the scale-N root using only the target board's descendant closure. Because the locked C8/C9 claims and spot evidence are explicitly scoped to reachable states, the plan is not yet executable as its own proof.

Current scope: read-only successor review of PL-theory-c1-20260825 revision 2. Not in scope: source/test/plan/design edits, dependency resolution, branch creation, commits, PRs, scaffolding, prototypes, delegated dispatch, merge, or operator ratification.

## Fold disposition

- PR1: closed. Continuous, Richman, and discrete backward induction now use ascending empty-cell count, with a missing-child regression guard.
- PR2 exhaustive path: closed. `reachable_masks(N)` is an exact forward population from the root; masked-only C8/C9 acceptance, terminal exclusion, M=0 accounting, and off-mask diagnostics are explicit.
- PR3: closed. The bad-support seam forces a non-direct LP path, and C10 emitted matrices receive exact certification.
- PR4: closed. `uv.lock` and a scoped ignore file are named, broad staging is forbidden, and every commit stages named paths with a status check.
- PR5: closed. Task 13 supplies the complete E2 bundle, unmerged PR, and Implementer math-lock handoff carrying the conditional risk wording and ratification ask.

## PR6 — Targeted descendant closure has no reachability seed

The `solve_targeted(board, N)` contract says it computes “values + masks” while restricting work to boards containing the target's marks, and its algorithm starts at the target's descendant closure (plan lines 355–367). Exact reachability, however, is a forward property from the empty root. For a target such as `X........`, the empty-board predecessor is outside that descendant closure; without a root-to-target prefix computation there is no exact seed for which `(target, h, bx, bo)` points are reachable at N=64 or N=128. Seeding every target budget point over-approximates the approved population, while importing N=32 mask points does not establish their reachability at a different initial scale.

The spot selection also names boards from the N=32 report but does not define the actual `(board, h, bx, bo)` knife-edge points tested at N=64/128 (plan lines 481 and 510–514). A board alone is not a discrete state and cannot support the claimed per-scale BandReport.

Required revision: define one exact, bounded path before approval:

1. Prefix-plus-descendant path: enumerate the legal root-to-target prefix closure (subsets of the target's fixed marks, terminal-stopping, root coin, payments, and h transitions) to derive the exact target mask at N; then compute values over the target's descendant closure; or
2. Witnessed-seed path: select explicit `(board, h, bx, bo)` points at each large scale and carry an exact legal auction/move trace from `(empty, null, N, N)` as the reachability witness before running descendant-only value recursion.

In either path, specify the deterministic knife-edge budget selection at each scale (nearest threshold points plus the declared in/out-band offsets), require every selected point to be mask-true or witness-valid, and hard-fail an empty/invalid spot set. Add a test whose target has a nontrivial paid prefix so a descendant-only implementation cannot pass vacuously.

## Preserved gates and successor acceptance

Return revision 3 under the same PLAN_LOCK_ID with a fresh PLAN_SHA256 and `IN_REPLY_TO` naming this review. Close PR6 without changing DD revision 3, the exhaustive masked population, the theory-only scope, Task 13, the E2 target, or the standing C9/LPEscalation/N=24/P1-P2 gates. The root INDEX remains red; even a later approval cannot be exercised through delegated dispatch until that run-level block is cleared or operator-waived. The successor must remain a review request and carry no live implementation token.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — theory-only local package
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-theory analysis only
- AI-or-automation-acts-downstream: no — no autonomous external action
- worker/scheduler/queue/retry/async-side-effect: no — local batch solver only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — generated fixture schema is consumed by sibling domains
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this bundle
- test-runtime-role-mismatch: no — Python implementation and local E2 verification use the same role
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — P1/P2 residual-risk acceptance remains operator-owned at the math lock
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested by this review
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: governance-only PLAN-REVIEW relay and INDEX row authored; no source/test/plan/design edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-084547.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-083256.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-084726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-082602.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-084300.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-083444.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-084836.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-082457.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-084431.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-planner-20260825-084851.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/
