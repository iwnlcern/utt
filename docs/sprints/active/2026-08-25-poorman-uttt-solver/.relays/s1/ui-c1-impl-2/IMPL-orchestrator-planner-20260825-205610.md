ROLE: Orchestrator Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: ui-c1-impl-2
PARENT_DISPATCH_ID: ui-c1-plan-review-4
RUN_ID: s1
SUBJECT: DIRECT IMPLEMENTATION DISPATCH (refresh) — ui-c1 at plan rev 4; Tasks 2-15 resume
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — digest refresh of the standing orchestrator direct dispatch; merge stays separately gated
FROM: s1.orchestrator-planner
TO: ui.implementer
CC: ui.planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1/SITREP-planner-20260825-205456.md
DESIGN_LOCK_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: direct-override
DESIGN_SHA256: a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95
DESIGN_ARTIFACT: DD-ui-c1-20260825
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
PLAN_SHA256: d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a
BUNDLE_ID: ui-c1
OWNER: ui pair
REPO: /Users/jack/Programming/utt (origin https://github.com/iwnlcern/utt)
BASE: main@7f14dda
TARGET_BRANCH: main
BRANCH: ui-c1-impl

## Authority

This supersedes ui-c1-impl-1 (which pinned superseded plan digest 6309f3ef…). Authority basis: PL-ui-c1-20260825 rev 4 @ d083f61e… approved by ui-c1-plan-review-4 (parent ui-c1-plan-4); design lock unchanged at rev 7 @ a2e80f1e…. Blocker ui-c1-impl-blocker-1 resolved at the plan layer (both-zero fixture expectation corrected to exact_tie_draw per the landed owner contract); no owner-semantics change.

SCOPE_DIFF:
- ui/ (all plan-created files) -> in
SCOPE_DIFF_RESULT: all-in

## Conditions (carried forward from ui-c1-impl-1, updated)

1. Task 1 bytes (ui-c1-impl@ab7276f9, rebased onto the harness merge) remain valid per the review; resume at Task 2 after a rebase check onto current origin/main.
2. Owner-schema gate: OPEN (harness schemas/conformance on main at 1562183).
3. Fixture legality proof via harness-owned generator/validator with provenance; absence at the consuming task is a stop-and-route.
4. M4 ruling s1-m4-ruling-1 binds PV perspective and analysis state key; PV rendering behind your mapping function until the harness pin section lands (harness-c2 carries it).
5. Execute under Superpowers executing-plans; TDD per plan; exit with branch + PR + report; merge under a separate addressed grant.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay; no edits claimed by this relay
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted dispatch relay; repo state carried in the orchestrator turn report
