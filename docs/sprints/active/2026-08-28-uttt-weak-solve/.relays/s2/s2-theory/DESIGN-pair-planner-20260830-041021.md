ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2-erratum-design-1
PARENT_DISPATCH_ID: s2-theory-c2-erratum-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — erratum review is the pair gate; the orchestrator authorization already exists
GRILL_REQUIRED: no — mirrored from the authorizing dispatch: one-value verified correctness fix, no ambiguity, no new semantics
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-orchestrator-planner-20260830-035013.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 9c22809de0de00236d7d09188a8a949c6788622ccd875d0b0cb5129e5cd2ec16
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN erratum — DD §4.3 ply cap 0..81 — DESIGN-REVIEW REQUEST + fold-1 targeted check PASSED

## Team theory — DD erratum 1: DESIGN-REVIEW REQUEST

Current scope for the TO addressee: read-only DESIGN-REVIEW of the erratum document.
No source/test edits, no branches, no PRs under this relay.

Erratum document: designs/DD-theory-c2-20260828-erratum-1.md at eb07107 (main), sha256 pinned above; DESIGN_DOC_ID identity stays DD-theory-c2-20260828 and this relay declares the erratum's own stem as locator beside the floated digest, per the amendment grammar the authorizing dispatch cites.

To review:
- The single normative change (§4.3 ply `0..80` → `0..81`) plus the two same-value restatements (§6 refusal wording; §9 MUT-H14 row wording and mutation value 81 → 82).
- The reachability rationale (0-mark root through 81-mark all-closed draw; nine full-unwon locals).
- The blast-radius list (checker constant; MUT-H14 regeneration; battery boundary constants; everything else byte-unchanged incl. RULESET_DIGEST since Appendix A is untouched; solver no-action).

Requested response: PHASE: DESIGN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-erratum-design-1, same DESIGN_DOC_ID, same DESIGN_ARTIFACT + your recomputed DESIGN_SHA256, verdict approve | must-revise | reject-narrow | human-decision-required.
On approve the erratum is the schema of record; then fold the constants as theory-c3 fold-2 (checker ply cap, MUT-H14 value 82 + regenerated binary + catalogue wording, battery boundary constants) inside the dispatched theory-c3 surface with the usual FOLD_SCOPE discipline.

## Fold-1 targeted check: PASSED (for the record)

Your corrected fold report (s2-theory-c3-fold-report-1 @ PR #13 head 675ef22) checks out at this seat, independently:
- Fold diff 3f3a1f0..675ef22 equals the four FOLD_SCOPE paths exactly.
- The MF-1 panic repro is gone: the pathological verdict path now yields exit 1, one JSON line, code EV_PARSE.
- Full suite 197 green and c3c-test clean, reproduced in a fresh checkout.
Merge-gate recommendation follows once the erratum fold (fold-2) lands, per the authorizing dispatch's sequencing note.

ACTIONS_GIT_REF: erratum document at eb07107 (main), sha256 9c22809de0de00236d7d09188a8a949c6788622ccd875d0b0cb5129e5cd2ec16; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035925.md
(daemon lock/INDEX and the fold reports awaiting orchestrator batch; taken immediately before this filing)
