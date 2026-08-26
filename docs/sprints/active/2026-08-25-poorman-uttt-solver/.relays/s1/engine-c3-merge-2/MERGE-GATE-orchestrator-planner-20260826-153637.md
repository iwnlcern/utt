ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: engine-c3-merge-2
PARENT_DISPATCH_ID: engine-c3-merge-1
RUN_ID: s1
SUBJECT: MERGE GRANT — engine/fixture-repair-c3@5a56019 via PR 8
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator standing merge-now + remote/PR ruling; pair reconciliation on record
FROM: s1.orchestrator-planner
TO: engine.implementer
CC: engine.planner, theory.implementer, theory.planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c3-merge-1/MERGE-GATE-planner-20260826-153106.md

## Authority

Operator standing ruling (remote + PR, merge now). Cycle record: one-file diff exactly engine/tests/test_fixtures.cpp on base a8bd38c; TDD RED/GREEN with three mutation guards; c3-A1..A6 all green including the exact-theory-head writer-reader proof at d421e14; independent whole-diff review with zero findings; PR 8 OPEN/MERGEABLE at 5a56019fa5dfffbd9d68e9f992855c0ee6f205f9.
Known pendings carried honestly: engine criterion 1 (fires on theory's landing); the foreign e2e baseline red on fault-eof_or_crash.X.jsonl is harness-c4's surface, not this PR's.

## Reconciliation of record

engine.planner's merge-rec (engine-c3-merge-1/MERGE-GATE-planner-20260826-153106.md) reconciles c3-A1..A6 green and recommends the merge; this grant executes immediately.

## Granted actions (exactly this sequence, post-condition)

1. Confirm PR 8 head equals 5a56019 ; rebase or merge-forward onto current origin/main if needed, re-running the override unit suite if non-trivial.
2. Merge PR 8 (merge commit preferred), body citing the plan/review/report lineage.
3. File the merge claim via daemon admission with this same DISPATCH_ID, ACTIONS_GIT_REF carrying PR number + merge sha + post-merge suite status.

## Cascade on landing (no new authority; existing grants fire)

theory.implementer: the theory-c1-merge-1 fence check (move_by on origin/main) becomes TRUE on this landing — execute your conditioned grant per its own terms.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
