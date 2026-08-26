ROLE: Pair Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: theory-c1-merge-rec-1
PARENT_DISPATCH_ID: theory-c1-fold-report-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — merge authority is the orchestrator's separate grant; this relay is the pair's recommendation with the fence evidence
FROM: theory.planner
TO: s1.orchestrator-planner
CC: s1.orchestrator-reviewer, theory.implementer, engine.planner
IN_REPLY_TO: theory-c1-fold-report-4/REVIEW-FOLD-pair-implementer-20260826-140722.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd
BUNDLE_ID: theory-c1
OWNER: theory
REPO: /Users/jack/Programming/utt
TARGET_BRANCH: main
BRANCH: theory/stage1-c1
PR: https://github.com/iwnlcern/utt/pull/7 (OPEN, non-draft, CLEAN, UNMERGED at d421e14533bd4cc572578d6e483b2291b90a6694)
SUBJECT: MERGE RECOMMENDATION — theory-c1 PR #7: merge-blocked solely on the engine-c3 fence; grant recommended the moment the fence clears

## Verdict

merge-blocked — on exactly one condition: the s1-closure-ruling-1 §2 consumer fence. engine-c3 is NOT landed (re-verified by both pair seats this session: origin/main engine/tests/test_fixtures.cpp lacks move_by, parses closure records as vector<int>, reduces expected_terminal to bool; no engine-c3 PR exists). harness-c3 is landed (f1c146b).
Recommendation: grant theory's merge the moment engine-c3 merges or shares the merge window — every theory-side condition is complete.

## Theory-side completeness (all E2, planner-verified on disk, not implementer-reported)

- PR #7 head d421e14: full suite 85 passed; `fixtures --check` diff-clean; diff confined to theory/ from base d89f006.
- Adversarial panel (custom four-lens): math lens APPROVE with full independent reproduction (all 22,186 thresholds via an independent solver; census; Richman 133/256 and 1/2; Kalai; crosscheck-N8 rebuilt from scratch; C8a induction numerically verified at >1M budget points, zero violations). All five must-fix findings + two C8a wording must-haves folded at 34da9c8 and verified by this seat; four optionals accepted, two declined with recorded reasons.
- MATH LOCK CLOSED: C1–C11 discharged/recorded (C8a discharged with validated proof; C11 lemma honestly undischarged, census-pinned at 22,186/11,093); operator ratification obtained and folded at d421e14 — P1 ratified as amended (P1a oracle-authoritative at solved scales; P1b large-scale tie-owner convention, labeled convention; P1c non-blocking side sweep, theory-c2 candidate), P2 and P3 ratified as proposed.
- Fixture publication complete under the D8 canonical bytes; harness compatibility empirically verified (main's loader consumed all fixtures, zero errors); backup cases remain consumed_by ["theory"] pending the MR17 acknowledgment.

## For engine-c3's scope (fence-clearing item, engine.planner CC'd)

The panel's engine build against the new bytes characterized the full repair surface: parse required `move_by` (the landed `mover` default silently mis-attributes O moves to X), accept closure records {local, result}, reduce terminal kind internally. The one permanently incompatible fixture (forced-to-closed legality pin) no longer carries the engine tag — after engine-c3, the engine suite should pass against theory/fixtures cleanly.

## Standing items after merge (non-blocking, for the ledger)

- P1c dedicated p=T sweep — theory-c2 candidate.
- MR17 backup engine-tag acknowledgment — outstanding; theory-only until addressed.
- Post-merge rerun obligations recorded by the run (harness theory-seam integration fires on theory's landing; engine criterion-1 consumes theory/fixtures).

ACTIONS_GIT_REF: none — recommendation relay; verification commands quoted; no edits by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; the implementation worktree is clean at d421e14 (verified this turn)
