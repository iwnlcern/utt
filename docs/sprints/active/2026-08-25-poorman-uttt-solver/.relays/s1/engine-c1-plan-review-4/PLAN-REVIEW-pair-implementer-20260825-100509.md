ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-plan-review-4
PARENT_DISPATCH_ID: engine-c1-plan-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining blockers are narrow mechanical plan corrections
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1-plan-3/PLAN-planner-20260825-095316.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
PLAN_SHA256: cd1673ebab3fd0ba2304e1b4dcf0395b07e1f8ffad571bf118a8abe18f902ea8
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW must-revise — rev 3 closes MR2/MR3 but retains mechanical staging, adapter-test, and benchmark-evidence contradictions

## Verdict

Revision 3 preserves the locked identities and materially closes the terminal-totality and Zobrist findings.
The retained BASE is now mechanically safe: the object exists, is an ancestor of current `main`, and `git diff --name-only 63b4b7b..main -- engine/` is empty.
Approval is still blocked by three narrow implementation-plan contradictions: Task 1 still stages directories and proves only an allowed subset, the literal adapter interface/test bytes do not pin the new strict `game_end` and 32 KiB rules, and Task 12 requests a candidate-verdict commit when no post-candidate bytes exist to commit.

This verdict is review-only.
It authorizes no source/test/plan/design edit by this seat, dependency download, branch creation or movement, commit, PR, implementation, delegated dispatch, merge, publication, deployment, or release.

## Must-revise findings

### MR1-R — The global staged-set rule is still contradicted by Task 1 and proves only a subset

Global Constraints now correctly prohibit bare directories and require comparison with each step's named FILE list.
Task 1 nevertheless still commands `git add engine/.gitignore engine/CMakeLists.txt engine/third_party engine/tests/test_smoke.cpp engine/src`, so the claimed removal of `engine/third_party` and `engine/src` did not land.
The proposed proof also says only that every printed cached path belongs to the allowed list; that accepts a strict subset and therefore does not prove that every required file was staged.

Required revision: replace both directory operands with all literal Task 1 paths, including the three placeholder core `.cpp` files named by CMake, and compare a sorted `git diff --cached --name-only` result for exact equality with the task's expected file set.
Retain the separate zero-count `engine/build/` refusal.
Make each later commit step either enumerate its exact staged paths or point to an equally mechanical per-task expected-set command.

Evidence: E1 — plan lines 32, 77–82, 111–126, and 144–149; successor relay claims the bare-directory adds were removed, but line 147 retains them.

### MR4-R — Literal adapter interface and tests still permit the old weak contract

The Task 10 behavior prose now requires strict `game_end` fields and a 32 KiB pre-parse ceiling, and the binary corpus round-trip is materially corrected.
But the literal interface still declares `validate_game_end(...); // type/protocol checked, payload tolerated`, directly contradicting the new required-field behavior.
The named failing tests cover only a happy case and a generic wrong-type case; they do not force failures for missing/invalid `result`, an out-of-enum `reason`, missing/noncanonical/out-of-range budgets, or an oversize input line.
An implementation can therefore follow the literal interface/test steps and recreate the exact weakness MR4 required closed.

Required revision: make the interface comment state strict owner-schema validation and name executable negative cases for every required `game_end` class.
Add line-framing boundary tests that pin the accepted maximum and the first oversize byte, and prove rejection happens before JSON parsing with no stdout.
Retain unknown-key tolerance and the full corpus binary round-trip gate.

Evidence: E1 — plan lines 743–748 and 761–768; harness design lines 21–25 and 70–75.

### MR5-R — The reference-first order is corrected, but the candidate evidence step is not executable as written

The reference artifact is now committed before candidate measurement, candidate mode is read-only, and identity refusal precedes measurement; those parts close the original ordering defect.
However, Step 3 says to paste the candidate verdict into a commit message after running candidate mode, while Step 3 creates or modifies no tracked file and specifies no empty commit.
Step 1 also creates the benchmark source/configuration without a named commit/staged set, while Step 2 says to commit the baseline immediately, leaving the exact bytes in that reference commit ambiguous.

Required revision: state the exact Task 12 commit topology and staged paths.
Commit the benchmark executable source/configuration before measuring; then commit only `baseline.json` before the candidate gate.
After candidate PASS, either create a named durable candidate-verdict artifact and commit it, explicitly authorize a named empty evidence commit, or route the verdict directly into the later IMPL relay without claiming a nonexistent Step 3 commit.
Whichever form is chosen must preserve the committed baseline unchanged and carry both medians plus the baseline session UUID.

Evidence: E1 — plan lines 794–809.

## Closed or retained portions

- The plan and design SHA-256 values match the current artifacts; the successor PLAN exact-file lint is clean when historical freshness is disabled.
- MR1's object-existence, ancestry, and empty engine-surface checks all pass against current `main`; no successor BASE is presently required.
- MR2 is closed: exact dependency comments and seam includes are corrected, and the hand-verified all-closed fixture asserts `AllClosed`, empty movegen, and `TerminalParent` rejection independent of owner artifacts.
- MR3 is closed: both hashes exercise the full 162 cell/seat, ten forced, and three tie populations with constant surrounding identity fields; incremental-primary versus full recomputation remains named.
- MR4's strict behavior prose, unknown-key rule, 32 KiB pre-parse placement, and owner-schema binary corpus round-trip are retained, subject only to the literal interface/test correction above.
- MR5's reference-before-candidate order, candidate read-only rule, identity refusal, and baseline UUID continuity are retained, subject only to an executable durable-evidence sequence.
- All prior closed scope boundaries, pins, oracles, perft shape, owner routing, truth-table honesty, benchmark predicate, and search-math exclusion remain accepted.

## Standing dispatch gate, independent of this verdict

The 2.9.2 daemon is `ready`, epoch `active`, and `pending_renders = 0`, but `relay verify` remains `ok: false` with one recorded `INDEX.md` divergence/conflict.
No delegated dispatch is available while that verify result remains red, even after a future plan approval.

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
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approval would accept mechanically under-pinned execution/evidence steps
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
Close MR1-R, MR4-R, and MR5-R without reopening MR2/MR3 or weakening the locked designs, owner contracts, truth table, or benchmark predicate.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-pair-implementer-20260825-100306.md` — daemon admission pending at authoring
- source/tests/plan/design=none — review-only; no source, test, plan, design, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-pair-implementer-20260825-095726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-implementer-20260825-100211.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-4/
