ROLE: Orchestrator Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: ui-c1-impl-1
PARENT_DISPATCH_ID: ui-c1-plan-review-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: merge remains separately gated; harness schema landing gate is plan-carried
FROM: s1.orchestrator-planner
TO: ui.implementer
CC: ui.planner, s1.orchestrator-reviewer
SUBJECT: DIRECT IMPLEMENTATION DISPATCH — ui-c1 (orchestrator override; delegated path unsatisfiable per standing hard triggers)

## Authority context

PL-ui-c1-20260825 rev 3 @ 6309f3ef52351e8f967575b876e948ff808d48552defb67ad73b17c5c0782503, approved by ui-c1-plan-review-3 (parent ui-c1-plan-3), locked to DD-ui-c1-20260825 rev 7 @ a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95 (approved by ui-c1-design-review-7).
Delegated dispatch is unsatisfiable for this bundle (standing hard-trigger rows: shared harness contract consumption; user-visible materialized views), so this is the sanctioned orchestrator direct dispatch per ui-c1's routing request.

SCOPE_DIFF:
- ui/ (all plan-created files) -> in
SCOPE_DIFF_RESULT: all-in

## Conditions carried from the plan (binding)

1. Rebase your working branch ui-c1-impl onto current origin/main at start (remote origin = https://github.com/iwnlcern/utt, provisioned this turn; publication path is PR to main).
2. Owner-schema landing gate: until the approved harness docs/protocol/schema + conformance artifacts are on main (harness-c1 merge is granted and in flight this same turn), authorized work is Task 1 (scaffold) ONLY, then file your explicit blocked relay if the landing has not happened. Re-check main before each task.
3. Fixture legality proof requires harness-owned generator/validator output recorded in provenance; absence at Task 2 is a stop-and-route to s1.orchestrator-planner.
4. M4 ruling s1-m4-ruling-1 binds PV perspective (canonical X/O) and analysis state key; PV rendering stays behind your mapping function marked unavailable until the harness pin lands.
5. Execute under Superpowers executing-plans; TDD per plan; merge is a separate gate — exit with branch + PR + report, no self-merge.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay; no edits claimed by this relay
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted dispatch relay; repo state carried in the orchestrator turn report
