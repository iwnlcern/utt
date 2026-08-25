ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-plan-review-4
PARENT_DISPATCH_ID: theory-c1-plan-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — Planner can correct the stale Spec lock from the already-approved revision-4 design; P1/P2 remain the later operator math-lock gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-plan-4/PLAN-planner-20260825-144958.md
SUBJECT: PLAN-REVIEW must-revise — Task 5 is corrected but the plan body still pins DD revision 3
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 453f6da94b2abf9652f3f64cfcd167a56d5e6b15923e710c54912019b73ed287
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: 8de912175f64b2f066564590294a63591531ecf8dd9939c598ef9f71312159d0
PLAN_REVIEW_VERDICT: must-revise

## Verdict

The Task 5 Richman-convention erratum is technically sound and closes the implementation hold: the plan now specifies `solve_richman(draw: str)`, independently asserts draw-to-O = 133/256 and draw-half = 1/2, keeps canonical auction terminal semantics isolated, and applies the STOP-on-disagreement guard to both results. The acceptance line is corrected accordingly, and Tasks 1–4 remain reusable at the verified held branch head.

The exact plan bytes are not yet approvable because the plan's own governing `Spec` line still pins DD revision 3 at the superseded SHA-256 `943d25db26fce8d9891ee5989cbd5d8be5946a63bd7cf498f738a02a8c6d8cbe`, while the carrier, approved DESIGN-REVIEW, revision banner, Task 5, and folded grill record all pin DD revision 4 at `453f6da94b2abf9652f3f64cfcd167a56d5e6b15923e710c54912019b73ed287`. That leaves two contradictory design-byte locks inside one implementation plan.

## Fold disposition

- Task 5 erratum: accepted. The two explicit conventions match DD revision 4 and the exact diagnostic that produced the hold.
- Acceptance update: accepted. “Both Richman self-checks pass” is the correct executable criterion.
- PR1–PR6: remain closed. Their task definitions, scope, evidence bundle, reachability construction, and hard escalation triggers are unchanged.
- DD revision-4 approval boundaries: otherwise preserved. P1/P2 remain operator-owned; C9 non-determinacy remains a hard stop; the N = 24 fallback remains orchestrator-gated; fixture publication cites 080619; merge remains out of scope.

## Must-revise finding

### PR7 — Advance the plan body's governing Spec pin to DD revision 4

At plan line 13, replace only the stale parenthetical `revision 3, sha256 943d25db26fce8d9891ee5989cbd5d8be5946a63bd7cf498f738a02a8c6d8cbe` with `revision 4, sha256 453f6da94b2abf9652f3f64cfcd167a56d5e6b15923e710c54912019b73ed287`.

Preserve the accepted Task 5 and acceptance changes and every unrelated byte. Return a successor PLAN carrier with the same `PLAN_LOCK_ID`, a fresh `PLAN_SHA256`, `PARENT_DISPATCH_ID: theory-c1-design-review-6`, and `IN_REPLY_TO` naming this review. On exact closure of PR7, the plan is approvable for a successor addressed implementation dispatch.

## Evidence and boundaries

- Routed PLAN integrity: SHA-256 `8de912175f64b2f066564590294a63591531ecf8dd9939c598ef9f71312159d0` matches the addressed relay.
- Routed DESIGN integrity: SHA-256 `453f6da94b2abf9652f3f64cfcd167a56d5e6b15923e710c54912019b73ed287` matches the approved revision-4 design and the carrier.
- Lock contradiction: plan lines 5 and 284 identify DD revision 4, while line 13 identifies revision 3 and its superseded digest.
- Held branch continuity: `/Users/jack/Programming/utt-theory-stage1-c1` is on `theory/stage1-c1` at `b589dc45b3c7ece219814e14f6f456470621e61c`; Tasks 1–4 remain committed, and only the two diagnosed Task 5 files are untracked.
- Exact incoming relay lint with freshness disabled: clean. Root lint remains red from inherited INDEX ordering; it neither repairs PR7 nor authorizes implementation.
- No source, test, design, plan, branch, commit, push, PR, merge, publication, or runtime action is authorized or taken by this review.

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

ACTIONS_GIT_REF: governance-only PLAN-REVIEW relay submitted through the daemon; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-ui-c1-20260825.md
