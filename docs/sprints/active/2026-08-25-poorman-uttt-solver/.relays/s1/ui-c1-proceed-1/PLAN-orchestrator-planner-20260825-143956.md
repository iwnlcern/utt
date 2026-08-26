ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: ui-c1-proceed-1
RUN_ID: s1
SUBJECT: PROCEED TO PLAN — ui-c1 (design approved at rev 4; M4 ruling issued this turn)
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: standing gates (dispatch gate v2; M4 pin lands in harness artifact)
FROM: s1.orchestrator-planner
TO: ui.planner
CC: ui.implementer, s1.orchestrator-reviewer
PARENT_DISPATCH_ID: ui-c1
APPROVED_DESIGN_DOC_ID: DD-ui-c1-20260825
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: dcea3f9e83a73f5d1515c8604f25f7a6298fc0d54a66600dd0cacf517b4ecb6a
APPROVING_DESIGN_REVIEW_DISPATCH_ID: ui-c1-design-review-4
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
BUNDLE_ID: ui-c1
OWNER: ui pair
REPO: /Users/jack/Programming/utt (monorepo; ui surface = ui/ per DD)
BASE: main@f8e26ae
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision; dispatch gate v2 (no verify events beyond the adjudicated set; daemon-admitted filing).

Implementer phase scope — PLAN-REVIEW after plan is drafted.
Current scope: answer design questions, review Planner's plan, findings inline.
Not in current scope: source/test edits, implementation branches, commits, PRs, scaffolding, or prototype implementation.
Implementation begins only after a daemon-admitted relay under this run's RELAY_ROOT carries the literal dispatch token bare, unfenced, un-backticked, alone on its own line, addressed to the Implementer in TO. Urgency is not dispatch; inline, quoted, fenced, CC-only, or non-addressee mentions are inert.

Approved design context:
DD-ui-c1-20260825 rev 4 approved by ui-c1-design-review-4 (parent ui-c1-design-4); digest verified this turn.
Operator grill decisions (GRILL-ui-c1-20260825) are locked in the DD; R5 scope narrowing stands (replay + analysis-from-logs).
M4 ruling s1-m4-ruling-1 (filed this turn): PV fields canonical X/O; analysis state key = harness log schema's canonical pre-state hash + (game_id, ply). Your PLAN cites the ruling as gate satisfaction; the harness pin is the eventual E1 source, and PV rendering stays 'unavailable — perspective unpinned' behind your single mapping function until the pin lands.
This relay is sequencing only and carries no design-doc lock. Author your gated PLAN FROM ui.planner with DESIGN_LOCK_ID: DD-ui-c1-20260825, DESIGN_RECORD_KIND: design-doc, PARENT_DISPATCH_ID: ui-c1-design-review-4.

FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay authored at filing time; no work actions claimed by this relay
