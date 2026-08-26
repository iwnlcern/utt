ROLE: Pair Implementer
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: theory-c1-impl-blocker-2
PARENT_DISPATCH_ID: theory-c1-impl-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — theory/harness/engine owner routing can resolve the byte contract; no operator product-semantic choice is required
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-impl-4/IMPL-pair-planner-20260825-205947.md
SUBJECT: IMPL STOP — landed engine and harness require incompatible closure-fixture bytes
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: eed3832d05a2c4317e211bb11d2846634b8fc5664cc53dc98e08e6954a71ac1d
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: 4038719b2c8fe3bc13abfdab77e7f0488e86e5936279ff25c6a17dbd690cd079
BLOCKER: boundary-contract deviation at the first consuming task

## Stop

Implementation stops before writing the Task 11 EARLY SLICE test or any fixture/schema byte. The current landed engine and harness consumers require incompatible representations for the same theory-owned closure/routing fixture, while the dispatch requires these artifacts to be consumed immediately and asserts no boundary-contract deviation. Choosing one consumer's representation or inventing a dual schema would exceed theory.implementer's authority and make the other landed acceptance test fail.

The live token in theory-c1-impl-4 is consumed up to this STOP. No Task 11, Task 8, Task 10, fixture publication, source/test edit, commit, push, PR, merge, or P1–P3 ratification occurred.

## Exact collision

Landed engine consumer at current main:

- `engine/tests/test_fixtures.cpp:81-82` reads the mover from optional `mover` (default X).
- lines 91–96 deserialize `expected_closures` as `std::vector<int>` — local-board indices only.
- lines 98–100 deserialize `expected_terminal` as `bool`.
- lines 153–156 honor `consumed_by` and skip fixtures not tagged for engine.

Landed harness consumer at current main:

- `referee/tests/test_fixture_loader.py:70-73` iterates every `game: uttt` fixture and does not filter `consumed_by`.
- lines 80–85 require `move_by` and read the same `expected_closures` / `expected_terminal` keys.
- lines 86–91 compare `expected_closures` to `[{'local': int, 'result': 'X'|'O'|'full'}]` and compare `expected_terminal` directly to the runtime terminal value `None | 'macro_win' | 'all_closed'`.
- `referee/poorman_referee/rules.py:77-98` confirms those runtime shapes.

Therefore one fixture object cannot satisfy both consumers:

- `expected_closures` cannot simultaneously be `list[int]` and `list[{local,result}]`;
- `expected_terminal` cannot simultaneously be boolean and `null | 'macro_win' | 'all_closed'`;
- `mover` versus `move_by` is a third naming divergence (dual fields could mask this one, but that would still be an unreviewed schema choice).

Splitting fixtures by `consumed_by` does not solve the collision because the harness consumer ignores that field and executes every UTTT fixture. Omitting closure fixtures does not solve it because the approved early slice requires closure/routing coverage and both consumers are waiting on those artifacts.

## Governing-contract gap

DD-theory-c1 section 3 fixes a closure/routing fixture as `move` plus expected closure set, next `forced`, and terminal flag, but does not lock the exact closure element representation or the mover field name. The staged plan repeats the category but likewise does not resolve the landed consumer divergence. Theory owns the schema, while engine and harness explicitly consume it without local reinterpretation, so the missing byte contract must be settled and routed before publication.

## Requested resolution

Please route an addressed boundary decision through s1.orchestrator-planner to theory.planner plus the engine/harness owners that:

1. pins one canonical theory fixture shape for mover, closure records, and terminal expectation;
2. identifies the consumer repair(s) required to accept that shape, including whether harness must honor `consumed_by`;
3. authorizes the owning consumer seat(s) to land those repairs or records an ordered compatibility transition;
4. returns a successor theory design/plan/implementation carrier if the canonical schema changes the current DD/PLAN bytes.

Recommended canonical direction for owner review: `move_by: 'X'|'O'`; `expected_closures: [{local, result}]` to preserve closure meaning; `expected_forced: int|null`; `expected_terminal: bool` to match the design's terminal-flag wording. That recommendation is not implemented or treated as a ruling.

## Preserved state and evidence

- Incoming dispatch is exact-file lint clean with freshness disabled; its live token, parent approval, digests, and held-head pin all matched.
- Isolated implementation worktree is clean on `theory/stage1-c1` at `f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`.
- Fresh pre-dispatch baseline from the approving review remained `48 passed in 49.46s`; no implementation byte changed afterward.
- The incompatibility is current-main evidence from the landed engine and harness consumer tests, not a speculative future concern.
- Shared relay-root lint remains red from inherited INDEX chronology; exact-file results remain separately judgeable.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — theory-only local package
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game state only
- AI-or-automation-acts-downstream: no — no autonomous external action
- worker/scheduler/queue/retry/async-side-effect: no — local batch solver only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — three sibling domains disagree on one theory-owned fixture contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control changed
- test-runtime-role-mismatch: yes — two landed consumers interpret the same fixture keys with incompatible runtime types
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — choosing a representation locally would invent a cross-pair API decision
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — owner routing, not a waiver, is required
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: report-only blocker relay submitted through daemon admission; no source/test/design/plan edits or commits by theory.implementer; implementation worktree remains at f8662cd; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? .superpowers/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-7/
