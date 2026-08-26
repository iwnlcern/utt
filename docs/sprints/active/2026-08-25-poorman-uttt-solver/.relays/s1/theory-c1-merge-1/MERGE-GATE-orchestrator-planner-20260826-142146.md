ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: theory-c1-merge-1
PARENT_DISPATCH_ID: theory-c1-merge-rec-1
RUN_ID: s1
SUBJECT: MERGE GRANT (fence-conditioned) — theory/stage1-c1 via PR 7, executable the moment engine-c3 lands
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator standing merge-now + remote/PR ruling; the math-lock operator ratifications (P1a/P1b/P1c, P2, P3) are already folded at the PR head per the pair's record
FROM: s1.orchestrator-planner
TO: theory.implementer
CC: theory.planner, engine.planner, harness.planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-merge-rec-1/MERGE-GATE-pair-planner-20260826-141758.md

## Authority

Theory-side completeness accepted per the planner's E2-verified recommendation: PR 7 head d421e14533bd4cc572578d6e483b2291b90a6694, suite 85 passed, four-lens panel with full independent math reproduction, math lock CLOSED with operator ratifications folded, D8-canonical fixtures published and empirically consumed by main's loader.

## Execution condition (hard — the s1-closure-ruling-1 §2 fence)

This grant may be executed ONLY once engine-c3 is merged to origin/main, verified mechanically by you at execution time: origin/main's engine/tests/test_fixtures.cpp contains the move_by token (and no engine-c3 merge-window arrangement supersedes this check). Until then this grant is inert; do not merge.

## Granted actions (exactly this sequence, post-condition)

1. Verify the fence check above on a fresh fetch; rebase or merge-forward theory/stage1-c1 onto current origin/main if needed, re-running the theory suite if non-trivial.
2. Merge PR 7 (merge commit preferred), body citing the panel/fold lineage and the folded operator ratifications.
3. File the merge claim via daemon admission with this same DISPATCH_ID, ACTIONS_GIT_REF carrying PR number + merge sha + post-merge suite status.

## Ledger effects on landing (for the CC'd owners, no new authority)

- harness: the theory-seam integration obligation FIRES — flip the skip to required, run, report.
- engine: the criterion-1 fixture-suite rerun becomes actionable under the standing obligation; with engine-c3's repairs the suite should pass cleanly per the theory panel's characterization.
- Standing non-blocking items recorded: P1c dedicated p=T sweep (theory-c2 candidate); MR17 backup engine-tag acknowledgment.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
