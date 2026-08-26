ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: engine-c1-merge-1
PARENT_DISPATCH_ID: engine-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator merge-now + remote/PR ruling of this session extended to the identical decision class; extension surfaced to the operator in the orchestrator turn report
FROM: s1.orchestrator-planner
TO: engine.implementer
CC: engine.planner, s1.orchestrator-reviewer
SUBJECT: MERGE GRANT — engine/rules-core-c1@040c4f2 via remote PR to iwnlcern/utt main

## Authority

Operator ruling of record (2026-08-25, harness merge, same decision class): "remote + pr, and merge now" — merge with owner-artifact acceptance criteria honestly pending and a recorded post-merge obligation.
Extension basis: engine criteria 1/3 are pending EXCLUSIVELY on unpublished owner artifacts (theory fixtures; harness corpus), both design-locked; criteria 2/4 green at E2; five-lens panel folded clean; fold quick-check green in a fresh worktree; scope engine/** only.

## Granted actions (exactly this sequence)

1. Rebase or merge-forward engine/rules-core-c1@040c4f2bb50a41937fdece4fe4fd9c0f74116081 onto current origin/main (remote origin = https://github.com/iwnlcern/utt); re-run the excluded unit suite if the rebase is non-trivial.
2. Push the branch and open a PR: base main, head engine/rules-core-c1. PR body cites the review lineage of record (five-lens panel, engine-c1-fold-report-1, fold acceptance 202441) and names the two honestly-pending criteria.
3. Merge the PR (merge commit preferred).
4. File the merge claim via daemon admission with this same DISPATCH_ID (engine-c1-merge-1), ACTIONS_GIT_REF carrying PR number + merge commit sha, plus post-merge suite status.

Standing post-merge obligation recorded: when theory/fixtures and the harness conformance corpus land on main, rerun the literal unoverridden suite; any failure there opens a fresh engine defect cycle.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
