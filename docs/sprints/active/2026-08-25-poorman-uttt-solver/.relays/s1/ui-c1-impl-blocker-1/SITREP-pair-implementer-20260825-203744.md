ROLE: Pair Implementer
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: ui-c1-impl-blocker-1
PARENT_DISPATCH_ID: ui-c1-impl-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — a bounded plan/owner-contract reconciliation is required; reopening owner terminal semantics would require a separate orchestrator decision
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-impl-1/IMPL-orchestrator-planner-20260825-202651.md
DESIGN_LOCK_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
PLAN_SHA256: 6309f3ef52351e8f967575b876e948ff808d48552defb67ad73b17c5c0782503
BUNDLE_ID: ui-c1
OWNER: ui
REPO: /Users/jack/Programming/utt
BASE: origin/main@15621830ee5f87820aa5a7e11dd0c2531a6a4490
TARGET_BRANCH: main
BRANCH: ui-c1-impl@ab7276f9ee147b477236f696a7f7c76813306432
SUBJECT: BLOCKED at Task 2 — legal both-zero transcript cannot have chip_count with zero margin

## Status

Implementation is blocked at the Task 2 owner-legality gate. Task 1 is complete, committed, rebased onto the landed harness merge, and E2-green. No Task 2 fixture, manifest, log type, parser, replay, analysis, or component byte has been authored.

The harness schema and conformance artifacts have landed through PR #1 at `origin/main@15621830ee5f87820aa5a7e11dd0c2531a6a4490`, so the availability gate itself is open. The stop is a contract contradiction in the locked plan's required legal fixture.

## Root cause and reproduction

The plan requires `both-zero.jsonl` to be a legal referee-emittable transcript where both budgets reach zero mid-game and play continues to `chip_count` with zero margin (`PL-ui-c1-20260825.md` line 120).

The landed harness owner contract requires the opposite terminal classification for equal budgets:

- `referee/poorman_referee/referee.py` lines 105–114 returns `chip_count` only for a strict budget leader and returns `draw` / `exact_tie_draw` for equality.
- `docs/protocol/poorman-uttt-protocol-v1.md` line 213 states the same normative rule.
- `referee/tests/test_referee_e2e.py` lines 307–323 provides the owner E2 assertion that equal terminal budgets produce `exact_tie_draw`.

An owner-generator reproduction using `GameConfig` + `play_game` with both landed stub engines bidding `all_in` reached `{X:0,O:0}` after ply 1, continued to 64 plies, and emitted terminal `{result:"draw", reason:"exact_tie_draw", budget_margin:0}`. Once both budgets are zero, later legal bids and payments cannot create a strict leader, so no legal continuation can end `chip_count` at zero margin.

This is not safely repairable in UI fixture bytes. Labeling that transcript `chip_count` would violate the harness owner contract; changing harness terminal semantics would be cross-owner work outside `ui/**`; silently substituting a different fixture outcome would depart from the locked exact plan.

## Requested correction

Reissue the bounded plan requirement so `both-zero.jsonl` reaches `{X:0,O:0}` mid-game, continues legally, and terminates `exact_tie_draw` with `budget_margin:0`. Preserve the fixture's UI purpose: typed not-applicable percent/share behavior while exact `0 / 0` units remain visible.

If `chip_count` at zero margin is an intended product requirement instead, route an explicit owner-semantics ruling through `s1.orchestrator-planner`; it conflicts with the landed protocol and E2 suite and cannot be decided by the UI pair.

Because the current direct implementation dispatch locks exact plan digest `6309f3ef…`, Tasks 2–15 remain held until a legitimate successor plan/review and authority acknowledgment resolve this delta. Task 1 bytes are preserved on the branch.

## Task 1 evidence

- Worktree: `/Users/jack/Programming/utt/.worktrees/ui-c1-impl`, isolated branch `ui-c1-impl`.
- RED: `test/smoke.test.tsx` failed because the generated app lacked heading `Poorman UTTT Replay`.
- GREEN after minimal app change: Vitest 1 file / 1 test passed; ESLint passed; TypeScript + Vite production build passed.
- Exact pinned toolchain is recorded in `ui/package-lock.json`; runtime dependencies are only `react@19.2.8` and `react-dom@19.2.8`.
- Task-1 commit was rebased onto the harness merge and is now `ab7276f9ee147b477236f696a7f7c76813306432`.

## Boundary contract

Writes: Task-1 private UI scaffold only; no fixture or cross-owner artifact written.
Reads: landed harness schemas, protocol, referee terminal logic, conformance fixtures, and E2 tests.
Target entity: rendered UI replay/analysis views — not reached beyond the scaffold.
Downstream consumer: human replay user.
Contract: UI consumes harness log semantics verbatim and performs no rules re-execution.
Proof: owner source + protocol + E2 test + fresh generator reproduction agree on `exact_tie_draw` for equal budgets.
No-consumer action: stop and reroute; do not manufacture a non-owner legal transcript.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local UI scaffold and contract inspection only
- migration/backfill/destructive-write/canonical-data-repair: no — no data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — replay UI takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — no UI worker or external side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — the blocker is a harness-owned terminal contract mismatch
- user-visible-control-with-materializer/downstream-consumer: yes — the eventual replay materializes human-visible terminal semantics
- test-runtime-role-mismatch: no — owner runtime, protocol, and E2 test agree
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — choosing between plan correction and owner semantic change exceeds UI implementation authority
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF:
- branch=`ui-c1-impl@ab7276f9ee147b477236f696a7f7c76813306432`
- commit=`ab7276f9ee147b477236f696a7f7c76813306432` — Task 1 scaffold only, rebased onto `origin/main@15621830ee5f87820aa5a7e11dd0c2531a6a4490`
- PR=none — implementation blocked before the plan's completion/PR exit gate

FINAL_GIT_STATUS_SHORT: none — `/Users/jack/Programming/utt/.worktrees/ui-c1-impl` clean at `ab7276f9ee147b477236f696a7f7c76813306432`
