ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: s2-theory-c3-merge-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — this relay is the merge decision, granted under the operator's standing remote+PR merge-now policy (carried from s1; operator may revoke)
FROM: s2.orchestrator-planner
TO: theory.implementer
CC: theory.planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/MERGE-GATE-pair-planner-20260830-194836.md
PARENT_DISPATCH_ID: s2-theory-c3-merge-rec-1
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@282f504
TARGET_BRANCH: main
BRANCH: theory/checker-c3
PR: https://github.com/iwnlcern/utt/pull/13
SUBJECT: MERGE GRANT — theory-c3 PR #13 @ df9a90a authorized for merge to main

## Grant

Orchestrator verification before grant (fresh, this relay's turn):

- PR #13 live state: OPEN, head df9a90a5cfe39c397896406f9a3032fe465a7375 (byte-equal to the recommendation), base main, MERGEABLE / CLEAN (gh pr view, E2).
- Erratum sequencing satisfied: designs/DD-theory-c2-20260828-erratum-1.md rev2 digest 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac byte-verified on disk; approving review s2-theory-c2-erratum-design-review-2 verdict approve with preserved DESIGN_DOC_ID identity; the erratum constants and boundary gates are inside the PR head (fold-2, wire.c3 guard >80 → >81, MUT-H14 82, ply-81 NOLOSS probe passing) — no condition rides this grant.
- Chain of record: plan rev2 approved (plan-review-2); direct dispatch s2-theory-c3-impl-1; 4-lens panel with fold-1 (MF-1 CLI panic) and fold-2 (erratum constants) both scope-clean; pair-planner targeted checks on fresh checkouts; 198 pytest + c3c-test green under pinned c3c 0.8.3; all six plan acceptance criteria E2-verified.
- Open ledger items (c6 scale/resume work, alt profile as-needed, declined idiom optionals) are accepted as non-gating.

Merge PR #13 at head df9a90a5cfe39c397896406f9a3032fe465a7375 into main.
If the head sha has moved from that value, stop and relay back instead of merging.
Post-merge: file the merge claim on this same DISPATCH_ID (s2-theory-c3-merge-1) with the merge commit in ACTIONS_GIT_REF; expected terminal verdict is merged-not-deployed with E2 as the terminal evidence class (offline checker + byte-pinned corpus, no deploy/live surface).

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no edits by this seat this relay
FINAL_GIT_STATUS_SHORT: unavailable — grant authored mid-turn; the orchestrator turn report carries post-commit repo state
