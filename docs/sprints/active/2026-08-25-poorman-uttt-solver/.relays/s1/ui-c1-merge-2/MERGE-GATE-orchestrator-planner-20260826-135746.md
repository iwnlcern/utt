ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: ui-c1-merge-2
PARENT_DISPATCH_ID: ui-c1-impl-3
RUN_ID: s1
SUBJECT: MERGE GRANT (supersedes ui-c1-merge-1) — PR 4 at the post-repair head, conditioned on the green battery
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — operator standing merge-now + remote/PR ruling; grant executable only on the named green-evidence condition
FROM: s1.orchestrator-planner
TO: ui.implementer
CC: ui.planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-merge-1/MERGE-GATE-pair-implementer-20260826-135331.md

## Authority

Supersedes ui-c1-merge-1 (its head pin 9f2e5a9 is stale after the authorized main-forward merge and the ui-c1-impl-3 repair). All ui-c1-merge-1 context carries forward, including the named post-merge E3 spot check.

## Execution condition (hard)

This grant may be executed ONLY after the ui-c1-impl-3 repair is complete with: focused test RED-then-GREEN, full battery 252/252, lint 0, build 0, owner-schema validation 13/13 — all at the exact head you will push. A red or partial battery voids this grant; relay instead.

## Granted actions (exactly this sequence, post-condition)

1. Push the repaired branch; confirm PR 4's head equals the pushed head.
2. Merge PR 4 (merge commit preferred), body citing the panel/fold lineage plus the impl-3 census repair.
3. Post-merge E3 spot check as named in ui-c1-merge-1: build from the actual merge commit, open in a real browser, load a real merged-corpus game log, step the timeline, open the analysis panel; report what rendered.
4. File the merge claim via daemon admission with this DISPATCH_ID (ui-c1-merge-2), ACTIONS_GIT_REF carrying PR number + merge sha, post-merge battery status, and the E3 result.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
