ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: s2-theory-c2-merge-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — this relay is the merge decision, granted under the operator's standing remote+PR merge-now policy (s1 ruling, carried into s2; operator may revoke)
FROM: s2.orchestrator-planner
TO: theory.implementer
CC: theory.planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/MERGE-GATE-pair-planner-20260829-033317.md
PARENT_DISPATCH_ID: s2-theory-c2-merge-rec-1
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c2-20260828
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@d305625
TARGET_BRANCH: main
BRANCH: theory/certschema-c2
PR: https://github.com/iwnlcern/utt/pull/12
SUBJECT: MERGE GRANT — theory-c2 PR #12 @ 4ac333c authorized for merge to main

## Grant

Orchestrator verification before grant (fresh, this relay's turn):

- PR #12 live state: OPEN, head 4ac333ce0e9ae40d92671b4b90b520553ddb98ab (byte-equal to the recommendation), base main, MERGEABLE / CLEAN (gh pr view, E2).
- Chain of record: design and plan locks lineage-verified previously (RECONCILE.md R2, proceed-2); dispatch s2-theory-c2-impl-1 rode the delegated conditions with SCOPE_DIFF all-in; panel + fold with FOLD_SCOPE all-in; pair-planner targeted fold check at a fresh checkout, full suite 141 passed.
- Deferred optionals (P4 positive, catalogue count pin, R17 wording) are accepted as theory-c3 backlog, not merge blockers; MUT-R17/MUT-R10 ledger notes stand as recorded.

Merge PR #12 at head 4ac333ce0e9ae40d92671b4b90b520553ddb98ab into main.
If the head sha has moved from that value, stop and relay back instead of merging.
Post-merge: file the merge claim on this same DISPATCH_ID (s2-theory-c2-merge-1) with the merge commit in ACTIONS_GIT_REF; expected terminal verdict for this bundle is merged-not-deployed with E2 as the terminal evidence class (byte-pinned corpus, no deploy/live surface — per plan).

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no edits by this seat this relay
FINAL_GIT_STATUS_SHORT: unavailable — grant authored mid-turn; the orchestrator turn report carries post-commit repo state
