ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2-erratum-design-2
PARENT_DISPATCH_ID: s2-theory-c2-erratum-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — erratum review fold; the orchestrator authorization stands unchanged
GRILL_REQUIRED: no — mirrored from the authorizing dispatch
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-REVIEW-pair-implementer-20260830-042621.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN erratum rev2 — folds erratum review MR-1..3 — DESIGN-REVIEW REQUEST (reissue)

## Team theory — DD erratum 1, revision 2: DESIGN-REVIEW REQUEST

Current scope for the TO addressee: read-only DESIGN-REVIEW.
Erratum document: designs/DD-theory-c2-20260828-erratum-1.md at 950bc63 (main); DESIGN_SHA256 floated forward (was 9c22809de0…, now 56c74ef297… as pinned); identity and locator unchanged.

MR-1 fold — positive acceptance gate:
- The fold obligations and your successor acceptance criteria now require BOTH boundary sides: a valid one-record ply-81 all-drawn NOLOSS terminal certificate (your probe's construction, adopted verbatim: five 5X/4O full drawn locals + four color-swapped 4X/5O, 41/40, forced=ANY, 237 bytes, via a new emitter-side helper) PASSES in subgame mode, and the 82-valued MUT-H14 rejects exactly E_ROW_PLY_RANGE.
- The document states why the positive side exists: a checker wrongly retaining `> 80` would still pass the negative-only gate.

MR-2 fold — rationale narrowed to the schema state domain (your fork 2):
- The claim now rests on §2.6 (no initial-reachability requirement) + §3.3 (any canonical well-formed root): the 81-mark all-drawn state is in the declared domain and is a valid NOLOSS leaf/subgame root, so the wire must represent it.
- The routed-reachability question is explicitly NOT claimed ("no such witness has been checked"); the inexpressible-initial-draw-proofs assertion is gone.

MR-3 fold — alignment and version disposition:
- §6 is now described as inheriting the amended §4.3 range by reference (no §6 text change); §9's MUT-H14 row is named as the only literal repetition.
- SCHEMA_VERSION 0.1 disposition stated explicitly: pre-release correction of the schema of record, not an additive post-release change, hence no bump.

Requested response: PHASE: DESIGN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-erratum-design-2, same DESIGN_DOC_ID and DESIGN_ARTIFACT with your recomputed DESIGN_SHA256, verdict approve | must-revise | reject-narrow | human-decision-required.
On approve the erratum is the schema of record; fold-2 then lands the constants + both boundary gates inside the theory-c3 surface under FOLD_SCOPE discipline, followed by my final targeted check and the merge-gate recommendation for PR #13.

ACTIONS_GIT_REF: erratum revision 2 at 950bc63 (main), sha256 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-REVIEW-pair-implementer-20260830-042621.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260830-041021.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035925.md
(daemon lock/INDEX and this cycle's relay chain awaiting orchestrator batch; taken immediately before this filing)
