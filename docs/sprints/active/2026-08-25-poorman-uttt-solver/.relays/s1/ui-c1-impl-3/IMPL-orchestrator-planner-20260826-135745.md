ROLE: Orchestrator Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: ui-c1-impl-3
PARENT_DISPATCH_ID: ui-c1-merge-1
RUN_ID: s1
SUBJECT: DIRECT IMPLEMENTATION DISPATCH — one-file census repair in ui/test/acceptance.test.ts on local head c8d087f
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — bounded test-only repair of a stale consumer census against the landed owner corpus; merge remains gated by ui-c1-merge-2
FROM: s1.orchestrator-planner
TO: ui.implementer
CC: ui.planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-merge-1/MERGE-GATE-pair-implementer-20260826-135331.md

## Authority and scope

Your merge-blocked halt was correct. This dispatch grants exactly the repair you scoped:
SCOPE_DIFF:
- ui/test/acceptance.test.ts -> in
SCOPE_DIFF_RESULT: all-in
On local composed head c8d087f: align the conformance-fixture discovery with the owner's event-log selection (root referee/tests/fixtures/*.jsonl plus parity-*/*.jsonl; exclude the separately owned engine-stdin/** request streams), retaining the exact 19-log inventory assertion and all ten fault-class assertions. Focused RED/GREEN first, then the full battery + lint + build + owner 13/13 validation. A second file is a deviation — halt and relay.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay; no edits claimed by this relay
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted dispatch relay; repo state carried in the orchestrator turn report
