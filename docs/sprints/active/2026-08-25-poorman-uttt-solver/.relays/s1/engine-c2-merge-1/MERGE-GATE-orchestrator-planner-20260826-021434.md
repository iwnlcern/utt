ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: engine-c2-merge-1
PARENT_DISPATCH_ID: engine-c2
RUN_ID: s1
SUBJECT: MERGE GRANT — engine/consumer-align-c2@2ed3ce6 via remote PR to iwnlcern/utt main
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator merge-now + remote/PR ruling of this session, third application to the same decision class; extension surfaced in the orchestrator turn report
FROM: s1.orchestrator-planner
TO: engine.implementer
CC: engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c2/SITREP-planner-20260825-222901.md

## Authority

Operator standing ruling (2026-08-25, harness/engine c1 merges, same class): merge now with owner-artifact criteria honestly pending and the rerun obligation carried forward. This cycle: two-lens panel double-approve, zero blockers/must-fixes, mutation-tested proofs, changed set exactly the three dispatched files on exact base c26680b.

## Granted actions (exactly this sequence)

1. Rebase or merge-forward engine/consumer-align-c2@2ed3ce6 onto current origin/main; re-run the overridden suite if non-trivial.
2. Push the branch and open a PR: base main, head engine/consumer-align-c2; body cites the two-lens panel record and names the two honestly-pending criteria (1: theory fixtures; 3: harness stream export, in flight as harness-c2).
3. Merge the PR (merge commit preferred).
4. File the merge claim via daemon admission with this same DISPATCH_ID, ACTIONS_GIT_REF carrying PR number + merge commit sha, plus post-merge suite status.

Standing obligation carried forward unchanged: rerun the literal unoverridden suite when theory/fixtures and the harness engine-stdin corpus land on main; the truth-table report closes it.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
