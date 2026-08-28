## Team theory — theory-c2: PROCEED TO PLAN

ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c2-proceed-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: merge remains the standing operator gate
FROM: s2.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/SITREP-pair-planner-20260828-112649.md
PARENT_DISPATCH_ID: s2-theory-c2
APPROVED_DESIGN_DOC_ID: DD-theory-c2-20260828
APPROVING_DESIGN_REVIEW_DISPATCH_ID: s2-theory-c2-design-review-4
PLAN_LOCK_ID: PL-theory-c2-20260828
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@d692ac2
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision.
SUBJECT: PROCEED-TO-PLAN — theory-c2 gated design-doc PLAN (supersedes s2-theory-c2-proceed-1)

## Supersession note

This relay supersedes s2-theory/PLAN-orchestrator-planner-20260828-112914.md (s2-theory-c2-proceed-1), which is structurally dirty: it carried the optional DESIGN_ARTIFACT/DESIGN_SHA256 field pair, whose fail-closed root-mode locator check cannot resolve the sprint-tree designs/ path from this relay root.
The design identity and integrity references below are prose context; the gated lock rides your own PLAN relay, not this sequencing relay.

## Sequencing

Your design-completion report reconciled clean (RECONCILE.md section R2): designs/DD-theory-c2-20260828.md verified byte-for-byte at sha256 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c, committed at d692ac2 on main; approving review s2-theory-c2-design-review-4 lineage-valid; grill decisions G1–G8 recorded as standing run rulings.

Proceed to your gated design-doc PLAN:

- You author the PHASE: PLAN relay FROM theory.planner with DESIGN_LOCK_ID: DD-theory-c2-20260828, DESIGN_RECORD_KIND: design-doc, and PARENT_DISPATCH_ID: s2-theory-c2-design-review-4 (the approving DESIGN-REVIEW).
  This orchestrator relay is sequencing only and carries no design lock.
- Plan scope boundary (from the DESIGN dispatch and DD-theory-c2 as approved): normative schema/spec artifacts, golden vectors, gate/mutant test artifacts, and the c3–c6 sequencing anchor — within the theory surface; no checker implementation in this bundle (that is c3), no solver-side edits, no engine/ edits.
- Plan review is the plan gate: request PLAN-REVIEW from theory.implementer; on approve, delegated conditional dispatch authority above applies — run the SCOPE_DIFF mechanically before issuing the token; any OUT row or hard trigger re-engages this seat.
- Merge remains a separate operator/orchestrator gate; IMPL exits with branch/PR/report.

Boundary note (CC solver.planner): DD-theory-c2-20260828 @ sha256 629801c4… is the byte-stable schema seam DD-solver-c2 consumes; schema changes still route through the orchestrator (ROADMAP gate 1).

ACTIONS_GIT_REF: none — sequencing relay; RECONCILE.md section R2 edit is committed by the orchestrator turn (see repo log)
FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay authored alongside a same-turn RECONCILE.md edit; the orchestrator turn report carries post-commit repo state
