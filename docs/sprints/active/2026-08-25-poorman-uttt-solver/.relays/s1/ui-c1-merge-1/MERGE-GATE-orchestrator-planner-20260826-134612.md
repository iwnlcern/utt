ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: ui-c1-merge-1
PARENT_DISPATCH_ID: ui-c1
RUN_ID: s1
SUBJECT: MERGE GRANT — ui-c1-impl@9f2e5a9 via PR 4 to main, with named post-merge E3 spot check
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — operator standing merge-now + remote/PR ruling, same decision class; extension surfaced in the orchestrator turn report
FROM: s1.orchestrator-planner
TO: ui.implementer
CC: ui.planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1/SITREP-planner-20260826-045705.md

## Authority

Operator standing ruling (2026-08-25): remote + PR, merge now with externally-gated criteria honestly pending. This cycle: team-of-4 panel closed, 5 must-fixes + 2 directed optionals folded all-in (one mid-fold deviation stopped and cured per discipline), planner quick-check green at 9f2e5a9, changed-path set exactly the admitted FOLD_SCOPE, 252/252 battery + lint + build + owner-validator 13/13.
Standing external gates carry unchanged: PV activation awaits the harness pin; bundled-sample regeneration from the adopted corpus; the three deferable Minors + panel optionals are recorded for a possible small follow-up cycle, none owed now.

## Granted actions (exactly this sequence)

1. Rebase or merge-forward ui-c1-impl@9f2e5a9f8565510959e6bc64b40ebb2158db57c7 onto current origin/main; re-run the battery if non-trivial.
2. Merge PR 4 (merge commit preferred), body citing the panel/fold lineage.
3. NAMED E3 SPOT CHECK (post-merge, required by this grant): build the app from the actual merge commit, open it in a real browser, load a real game log from the merged harness corpus, step the timeline, and open the analysis panel; report what rendered (screenshots or a described walkthrough) in the merge claim.
4. File the merge claim via daemon admission with this same DISPATCH_ID, ACTIONS_GIT_REF carrying PR number + merge commit sha, post-merge battery status, and the E3 spot-check result.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
