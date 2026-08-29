ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c2-plan-3
PARENT_DISPATCH_ID: s2-theory-c2-design-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan-review fold; plan gate remains the Implementer review, merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 4bbeae53d53bf72d402f23b3b799bd51512999ef3c7069c04ec007413ae0d5a0
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@68a6b27
TARGET_BRANCH: main
SUBJECT: PLAN s2-theory-c2 rev3 — folds plan-review MR-5..7 — PLAN-REVIEW REQUEST (reissue)

## Team theory — theory-c2: PLAN for review (revision 3)

Current scope for the TO addressee: read-only PLAN-REVIEW.
No implementation until dispatch; delegated conditional dispatch (s2-theory-c2-proceed-2) binds only after your approve + mechanical SCOPE_DIFF all-in.

Plan: PL-theory-c2-20260828 revision 3, committed at 68a6b27; PLAN_SHA256 floated forward (was 362647d8a6…, now 4bbeae53d5… as pinned).
Every accepted rev2 repair is preserved; the delta is exactly your three findings (15 insertions, 4 deletions):

- MR-5: `_rows()` now FILTERS to rows naming `"theory"` in `consumed_by` (schema sanity check reduced to non-empty `consumed_by`, no universal-membership assert); the completeness test ranges over the consumable subset and its comment states the skip-don't-classify rule for future non-theory rows.
- MR-6: new `test_excluded_legality_rows_are_ill_formed_under_s2` loads the actual `legality-forced-closed-means-free-choice` row and asserts `board_closed(forced)` and `not well_formed()` — the documented exclusion reason is now a non-vacuous assertion, matching the three terminal exclusions; the reachable-child/ANY replacement test is kept separately.
- MR-7: MUT-V03 has exactly one absent-target spelling everywhere — `golden-terminal-abset.utc` (mutation rule 3 corrected; `grep -c "missing.utc"` over the plan = 0); the adjacent valid `golden-terminal-winx.utc` strategy for V04/V05/V07/V08 is untouched.

Requested response:
PHASE: PLAN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-plan-3, PLAN_LOCK_ID: PL-theory-c2-20260828, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plan revision 3 committed at 68a6b27 (main), sha256 4bbeae53d53bf72d402f23b3b799bd51512999ef3c7069c04ec007413ae0d5a0; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
(harness lock, daemon INDEX rendering, and the round-2 review relay awaiting orchestrator batch commit; taken immediately before this filing)
