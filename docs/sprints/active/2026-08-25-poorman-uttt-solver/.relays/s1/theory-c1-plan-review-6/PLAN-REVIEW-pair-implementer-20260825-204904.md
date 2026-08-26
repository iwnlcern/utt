ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-plan-review-6
PARENT_DISPATCH_ID: theory-c1-plan-6
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining defects are executable-plan and stale-invariant repairs; P1–P3 remain operator-owned at the later math lock
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-plan-6/PLAN-planner-20260825-204316.md
SUBJECT: PLAN-REVIEW must-revise — early fixture slice lacks a closed test/commit contract; revision-5 invariants remain stale
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: eed3832d05a2c4317e211bb11d2846634b8fc5664cc53dc98e08e6954a71ac1d
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: ff30d77ccc12b7928b487d83069a2a2d1b94843021bef814573108029d23fe7d
PLAN_REVIEW_VERDICT: must-revise

## Verdict

The revision-5 theory deltas are technically coherent: Task 8's C11 census plus synthetic exact threshold-payoff harness, Task 9's explicitly empty zugzwang selector component, Task 10's synthetic LP case, the discriminated backup fixture branch, and Tasks 12–13's C1–C11/P1–P3 obligations all match the approved design.

The exact PLAN package is not yet executable under its own sequencing and commit-hygiene rules. The newly routed early-publication slice asks for a passing subset and a commit before oracle-generated fixtures exist, but neither the plan nor carrier defines that subset's test seam, exact staged paths, or partial-to-final transition. Three stale global/hygiene lines also contradict the revision-5 carrier and actual held branch.

This review does not authorize implementation, fixture publication, branch mutation, push, PR, merge, P1–P3 ratification, or `consumed_by: ["engine"]`.

## Must-revise findings

### PR8 — Make the pulled-forward Task 11 slice independently executable

Carrier lines 48–50 require Task 11 Steps 1–2 plus a `schema-validation subset` and commit before Task 8. Plan Step 1 instead defines a failing test that requires every fixture file; Step 4 defines only the final full test plus `fixtures --check`, which depends on the not-yet-complete generator/subcommand and oracle-generated files. No named test, selector, command, or exact partial commit paths exist for the early slice.

Define a closed early slice in the successor carrier or plan:

- exact created/modified paths: SCHEMA.md, schema-v1.json, the four hand-authored UTTT category files, test_fixtures.py, and fixtures_gen.py only if the permitted free-choice helper is actually used;
- the exact focused validation command and assertions that pass without thresholds_ttt3.json or backup_cases.json;
- the exact explicit-path `git add`, status check, and early commit subject;
- how Steps 3/3b later expand that same test to all fixture files and when `fixtures --check` becomes mandatory.

Preserve TDD: demonstrate the focused validator failing before the early fixture files exist and passing after them. Do not silently weaken the final all-files validation.

### PR9 — Complete the Task 11 file manifest and hygiene count

Task 11's Files list at lines 597–600 omits `theory/fixtures/backup_cases.json`, although Step 3b creates it. The Step 5 status guard at line 617 still says the fixtures directory contains seven intended files. Revision 5 contains eight: SCHEMA.md, schema-v1.json, legality.json, closure_routing.json, auction_traces.json, terminal.json, thresholds_ttt3.json, and backup_cases.json.

Add backup_cases.json to the file manifest and change the guard to exactly eight intended files. If the early slice creates only six fixture-directory files, name that intermediate count separately rather than reusing the final guard.

### PR10 — Carry P3 into the global proposal gate

Global Constraint line 23 still says only P1/P2 are proposals. Replace it with P1–P3 and require all three to remain `proposed-for-ratification`; P3 must retain the no-reachable-witness gap and synthetic-proof basis. Task 12 and acceptance already have the correct revision-5 wording.

### PR11 — Reconcile the plan's branch base with the carrier and actual history

Global Constraint line 24 says create `theory/stage1-c1` from `main@b26a90a`, while this carrier declares `BASE: main@d89f006` and the actual first theory commit `c91dbc9` has parent `d89f006`. Correct the plan to the actual held base `main@d89f006` (or explicitly distinguish an obsolete original intent from the effective base). The final scope/PR check cannot consume two different base pins.

## Preserved plan decisions and gates

- Preserve the exact approved design digest and current plan deltas for C11/D7/P3, synthetic terminals, backup fixtures, and consumer tagging.
- Preserve the resequencing goal: publish the oracle-independent schema and UTTT fixtures first after a live successor implementation dispatch.
- Preserve theory-only backup emission unless an addressed orchestrator authorization for the successor engine-search consumer lands.
- Preserve C9 and LPEscalation hard stops, the N=24 orchestrator gate, C8a/P2 risk wording, P1–P3 operator ownership, explicit-path staging, and separate merge authority.
- Tasks 1–7 and 9 remain committed; the held implementation worktree is clean at `theory/stage1-c1@f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`.

## Successor acceptance

Return a successor PLAN carrier with the same PLAN_LOCK_ID, approved DESIGN digest, fresh PLAN digest if plan bytes change, and `IN_REPLY_TO` naming this review. Close PR8–PR11 without changing the reviewed math or consumer boundaries. On exact closure, the plan is approvable for a separate successor implementation dispatch.

## Evidence

- Routed integrity: design SHA-256 `eed3832d05a2c4317e211bb11d2846634b8fc5664cc53dc98e08e6954a71ac1d` and plan SHA-256 `ff30d77ccc12b7928b487d83069a2a2d1b94843021bef814573108029d23fe7d` match the addressed carrier.
- Exact incoming relay lint with freshness disabled: clean. Root lint remains red from inherited INDEX chronology; it does not change this exact-file verdict.
- `git diff --check` for the current plan bytes: clean.
- Fresh held-branch baseline: `uv run pytest -q` completed with `48 passed in 50.43s`; worktree status is clean.
- Git history proves the effective base: `c91dbc966e1e34437c0788fe359af7ea33519722` has parent `d89f0066381c21d7d4a569461af51bb0932faa8c`.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — theory-only local package
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-theory analysis only
- AI-or-automation-acts-downstream: no — no autonomous external action
- worker/scheduler/queue/retry/async-side-effect: no — local batch solver only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — fixture schema and seed fixtures are consumed by sibling domains
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this bundle
- test-runtime-role-mismatch: no — Python implementation and local E2 verification use the same role
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — P1–P3 residual-risk acceptance remains operator-owned at the math lock
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested by this review
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: governance-only PLAN-REVIEW relay submitted through daemon admission; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-4/
