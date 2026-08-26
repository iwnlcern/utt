ROLE: Orchestrator Planner
PHASE: RECONCILE
AUTHORITY: report-only
DISPATCH_ID: harness-c2-reset-ruling-1
PARENT_DISPATCH_ID: harness-c2-reset-ruling-request-1
RUN_ID: s1
SUBJECT: RULING — option 1 authorized: one-time 19-log elapsed-field baseline reset under harness-c2
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — governing-seat ruling with git-history reversibility established; surfaced to the operator in the orchestrator turn report
FROM: s1.orchestrator-planner
TO: harness.planner
CC: harness.implementer, engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c2-reset-ruling-request-1/SITREP-planner-20260825-222153.md

## Ruling

Option 1 — the one-time baseline reset of all 19 committed event logs under the manifest's deterministic scripted clock — is AUTHORIZED under harness-c2.
Option 2 is rejected: freezing accidental wall-clock values as playback vectors enshrines noise as contract data and couples the manifest to the referee's internal clock-call pattern with zero consumer benefit.

## Conditions (bind the plan and the implementation report)

1. Reversibility of record: the pre-reset corpus bytes are preserved in git history on origin/main (pre-reset baseline main@29cbdf2 at this ruling); the executable restore path is `git checkout <pre-reset sha> -- referee/tests/fixtures/`. Cite both in the implementation report.
2. Field-scoped proof: the report carries a diff check proving the reset changed ONLY elapsed-time fields across exactly the 19 enumerated files — any other key changing fails the cycle.
3. Stream invariance proof: the engine-stdin stream corpus is byte-identical before/after the reset (streams carry no elapsed fields — prove it, do not assert it).
4. One-time: after the reset, gen_fixtures.py --check is a true byte-compare; any future baseline change is a fresh ruling, not a precedent.
5. The reissued harness-c2-plan-5 cites this ruling relay id; the delegated-dispatch "no hard trigger" condition is satisfied FOR THIS ENUMERATED RESET ONLY by this addressed authorization.

ACTIONS_GIT_REF: none — ruling relay; no edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
