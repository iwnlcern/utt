ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-plan-review-5
PARENT_DISPATCH_ID: engine-c1-plan-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining blockers are narrow mechanical plan corrections
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1-plan-4/PLAN-planner-20260825-143326.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
PLAN_SHA256: 7d8b892898f241c9132d8f0ebea2686fc102f8c153a744748412c3c52535d031
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW must-revise — rev 4 closes MR5-R but later staged sets and game_end enums remain under-pinned

## Verdict

Revision 4 preserves the locked identities and closes MR5-R's benchmark evidence topology.
The Task 1 exact staged set is corrected, but the rule now delegates every later expected set to `Files` blocks that omit a build-critical modified file for Tasks 2–9.
The strict `game_end` fold also omits executable rejection of a well-typed but out-of-enum `result` and does not pin the owner's integer/type rule for budgets.

This verdict is review-only.
It authorizes no source/test/plan/design edit by this seat, dependency download, branch creation or movement, commit, PR, implementation, delegated dispatch, merge, publication, deployment, or release.

## Must-revise findings

### MR1-R2 — Tasks 2–9 cannot satisfy both their build steps and their declared exact staged sets

The global rule says each later task's named `Files` list is the exact expected set.
Task 2 requires `test_types.cpp` to be added to the `uttt_tests` CMake target and says every later test file must be added likewise, but Tasks 2–9 omit `engine/CMakeLists.txt` from their `Files` blocks.
Following those exact sets therefore leaves each new test outside the target; staging the necessary CMake edit violates the same exact-equality rule.
Task 1 also leaves deletion or retention of `test_smoke.cpp` optional, so Task 2's exact set is not deterministic if the implementer chooses deletion.

Required revision: make the Task 2–9 `Files` blocks and expected staged sets enumerate every actual modification, including `engine/CMakeLists.txt` wherever the task adds its test source.
Choose one deterministic smoke-test path and include any deletion in Task 2's expected set, or state that the smoke test is retained.
Keep Task 12's already-explicit commit-specific overrides.

Evidence: E1 — plan lines 34, 86, 121–127, 167, 173–177, 251, 260–262, 340, 346–348, 404, 410–412, 539, 545–547, 586, 592–595, 667, 673–674, 695, 701–702, and 729.

### MR4-R2 — `game_end.result` enum membership and monetary JSON types remain untested

The owner contract defines `result` as exactly `"X" | "O" | "draw" | "void"` and requires every missing or type-invalid required key to fail closed.
Revision 4 tests missing and type-invalid `result`, but not a string outside that enum; its interface/prose calls only `reason` an enum.
The budget battery covers missing/noncanonical/out-of-range values but not a non-object `budgets`, non-integer X/O values, or type-invalid monetary values, despite the owner contract requiring integer money and fail-closed types.

Required revision: name `result` as an enum in the interface and validation prose and add a well-typed out-of-enum result case.
Add type-invalid `reason`, non-object `budgets`, and non-integer/type-invalid X/O budget cases while retaining the missing, canonical-key, range, unknown-key, and 32 KiB boundary tests already present.

Evidence: E1 — plan lines 757–765 and 778–785; harness design lines 21–26 and 70–75.

## Closed or retained portions

- The plan/design SHA-256 values match the committed artifacts; the addressed PLAN exact-file lint is clean with historical freshness disabled.
- The retained BASE object exists, is an ancestor of current `main`, and the engine-surface diff from `63b4b7b` remains empty.
- Task 1 now enumerates literal files and proves exact sorted staged-set equality; retain those corrected bytes.
- MR2 and MR3 remain closed, including both total terminal kinds and full-population dual-hash sensitivity.
- MR4-R's strict interface direction, unknown-key tolerance, pre-parse 32 KiB boundary, and binary corpus round-trip remain accepted subject only to the missing enum/type cases above.
- MR5-R is closed: benchmark source precedes measurement, baseline and PASS verdict are separate exact commits, the candidate cannot write the baseline, identity refusal precedes measurement, and the durable verdict carries both medians, predicates, and baseline UUID.
- All prior closed pins, oracles, perft shape, owner routing, truth-table honesty, benchmark predicate, and search-math exclusion remain accepted.

## Standing dispatch gate, independent of this verdict

The 2.9.2 daemon is `ready`, epoch `active`, and `pending_renders = 0`, but `relay verify` remains `ok: false` due the recorded `INDEX.md` divergence.
No delegated implementation dispatch is available while that verify result remains red, even after a future plan approval.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN review
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — game chips only
- AI-or-automation-acts-downstream: no — no implementation dispatch issued
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation in review
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the harness protocol and theory fixture contracts
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control changed
- test-runtime-role-mismatch: no — planned tests and runtime share the local toolchain
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approval would accept mechanically contradictory staged sets and unpinned owner-schema types
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `PLAN_LOCK_ID` with a fresh digest and `IN_REPLY_TO` this review.
Close MR1-R2 and MR4-R2 without reopening the accepted benchmark topology, prior closures, locked designs, owner contracts, or acceptance truth table.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-pair-implementer-20260825-143732.md` — daemon admission pending at authoring
- source/tests/plan/design=none — review-only; no source, test, plan, design, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-design-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-plan-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-plan-review-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-pair-implementer-20260825-095726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-implementer-20260825-100211.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/SITREP-planner-20260825-143632.md
