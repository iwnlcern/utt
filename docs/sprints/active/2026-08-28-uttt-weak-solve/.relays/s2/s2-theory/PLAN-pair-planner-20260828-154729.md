ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c2-plan-4
PARENT_DISPATCH_ID: s2-theory-c2-design-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan-review fold; plan gate remains the Implementer review, merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 4558eeda83cdc6e94b2bf45e93161856b7ab1cda6ac53706926504103d4bd5f7
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@d305625
TARGET_BRANCH: main
SUBJECT: PLAN s2-theory-c2 rev4 — folds plan-review MR-8 — PLAN-REVIEW REQUEST (reissue)

## Team theory — theory-c2: PLAN for review (revision 4)

Current scope for the TO addressee: read-only PLAN-REVIEW.
No implementation until dispatch; delegated conditional dispatch (s2-theory-c2-proceed-2) binds only after your approve + mechanical SCOPE_DIFF all-in.

Plan: PL-theory-c2-20260828 revision 4, committed at d305625; PLAN_SHA256 floated forward (was 4bbeae53d5…, now 4558eeda83… as pinned).
Delta is exactly MR-8 (2 insertions, 2 deletions, documentation-only):

- The Task 3 matrix prose now states completeness ranges over THEORY-CONSUMABLE fixture ids only; a future row whose `consumed_by` omits `theory` is skipped by `_rows()` per the SCHEMA.md consumer contract and never classified.
- Acceptance criterion 3 carries the same qualifier (and now also names the ill-formed forced-at-closed exclusion assertion explicitly).

All accepted rev2/rev3 repairs are preserved verbatim; no code snippet changed.

Requested response:
PHASE: PLAN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-plan-4, PLAN_LOCK_ID: PL-theory-c2-20260828, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plan revision 4 committed at d305625 (main), sha256 4558eeda83cdc6e94b2bf45e93161856b7ab1cda6ac53706926504103d4bd5f7; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
(harness lock, daemon INDEX rendering, and the cycle's relay chain awaiting orchestrator batch commit; taken immediately before this filing)
