ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: engine-c2
RUN_ID: s1
SUBJECT: PLAN dispatch — engine-c2 consumer alignment (corpus discovery + ply-0 fail-closed) at small tier
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — mechanical consumer alignment to owner rulings already on record; merge stays separately gated
FROM: s1.orchestrator-planner
TO: engine.planner
CC: engine.implementer, s1.orchestrator-reviewer, harness.planner
IN_REPLY_TO: engine-c1/SITREP-planner-20260825-203943.md
PLAN_LOCK_ID: PL-engine-c2-20260825
PLAN_ARTIFACT: PL-engine-c2-20260825
BUNDLE_ID: engine-c2
OWNER: engine pair
REPO: /Users/jack/Programming/utt (origin https://github.com/iwnlcern/utt)
BASE: main@c26680b
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision; dispatch gate v2 (no verify events beyond the adjudicated set; daemon-admitted filing).

## Scope (locked)

1. Align e2e corpus discovery to harness ruling 1: exactly docs/protocol/transcript-v1.jsonl PLUS glob referee/tests/fixtures/**/*.jsonl (.jsonl only; .stderr companions excluded). Test-only edit.
2. Fail-closed ply-0 validation per harness ruling 2: adapter rejects forced != 4 or tie_owner != null at ply 0 (tie_owner != null at ply > 0 already validated); wire tests for both rejection classes. No deeper defensive branching.
3. Close the standing rerun obligation: criterion-3 corpus round-trip against the real corpus now; criterion-1 fixture suite when theory publishes; truth-table report either way (an honestly-still-pending criterion 1 does not fail this cycle).
In-scope files: engine/src/adapter/wire.cpp, engine/tests/test_wire.cpp, engine/tests/test_engine_e2e.py. Anything else is a deviation.
No DESIGN phase: the owner rulings on record (harness-c1-contract-rulings-1; DD-engine-rules-c1 §6) are the design; your PLAN carries no DESIGN_LOCK_ID.

## Ceremony downgrade record (medium → small)

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game engine, no auth surface
- migration/backfill/destructive-write/canonical-data-repair: no — code + tests only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets
- AI-or-automation-acts-downstream: no — no downstream actor
- worker/scheduler/queue/retry/async-side-effect: no — none touched
- cross-repo/service-contract/generated-schema/shared-API-event: no — consumer-side alignment to pinned owner rulings; zero schema/contract edits in scope
- user-visible-control-with-materializer/downstream-consumer: no — wire validation and tests only
- test-runtime-role-mismatch: no — tests run in the same role as the code
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — three named files, rulings pinned, truth-table verification
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none
WHY_DOWNGRADE_IS_SAFE:
- consumer-only alignment; three-file blast radius; no contract, core, schema, or bench changes; direct E2 verification via wire tests + corpus round-trip + truth-table report

Acceptance: discovery matches the ruled pair exactly; both ply-0 rejection classes proven by wire tests; corpus round-trip green against the real corpus; truth-table report filed; merge via PR under a separate addressed grant.
FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time; no work actions claimed by this relay
