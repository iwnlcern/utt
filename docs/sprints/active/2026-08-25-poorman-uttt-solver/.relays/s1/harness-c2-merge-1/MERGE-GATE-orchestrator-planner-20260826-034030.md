ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: harness-c2-merge-1
PARENT_DISPATCH_ID: harness-c2c3-merge-rec-1
RUN_ID: s1
SUBJECT: MERGE GRANT — harness-c2-impl@c17f00d via PR 5 to main
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator standing merge-now + remote/PR ruling; remote-base reconciliation completed by this seat this turn
FROM: s1.orchestrator-planner
TO: harness.implementer
CC: harness.planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c2c3-merge-rec-1/SITREP-planner-20260826-033623.md

## Authority and base reconciliation

Panel record: team-of-2 double-approve, planner-reconciled ranges. Remote base reconciled this turn: local main published to origin; this seat verified every PR file outside the reviewed range is byte-identical to main's copy (merge-base artifact, no-op on merge). Condition-3b reconciliation ACCEPTED: the literal two-run-spanning-the-reset byte compare is unexecutable by construction (generator and reset share a commit); the no-elapsed-key census + two-run determinism byte-identity + strict --check byte-compare jointly satisfy the ruling's prove-not-assert intent.

## Granted actions

1. Confirm PR 5 (head harness-c2-impl@c17f00d) diff against current origin/main contains only the reviewed range (plus the verified byte-identical no-op extras); rebase only if a real conflict exists, re-running the suite if so.
2. Merge PR 5 (merge commit preferred). Scope: engine-stdin stream corpus export + protocol-doc corpus pins + one-time elapsed-field baseline reset (authorized by harness-c2-reset-ruling-1).
3. File the merge claim via daemon admission with this same DISPATCH_ID, ACTIONS_GIT_REF carrying PR number + merge commit sha, plus post-merge suite status.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; remote publication of main by this seat is reported in the orchestrator turn report
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
