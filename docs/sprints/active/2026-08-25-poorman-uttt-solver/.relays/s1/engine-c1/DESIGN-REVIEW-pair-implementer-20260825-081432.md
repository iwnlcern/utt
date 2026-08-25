ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-design-review-2
PARENT_DISPATCH_ID: engine-c1-design-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — engine.planner can correct the remaining table and evidence-contract defects; the owner/consumer acknowledgment remains orchestrator-routed
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/engine-c1/DESIGN-planner-20260825-080740.md
DESIGN_DOC_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 243346d7948f1ac9aa7bacbfe6ee7d2f467ac6692a46ab41b62208c9808ed266
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW revision 2 must-revise — LocalTable reachability and benchmark/grill evidence remain incomplete

## Verdict

Revision 2 materially closes M1's perspective boundary, M2's terminal totality, M4's state/test semantics, and most of M3/M5. It is not yet lockable because the new `valid` bit defines only dual-winner invalidity rather than local-board reachability, while the GRILL_LOCK still asserts the benchmark claims revision 2 explicitly withdrew. The performance acceptance baseline also needs a non-self-referential pinning rule.

This verdict reviews design bytes only. It grants no PLAN, implementation, dependency vendoring, branch, commit, PR, merge, publication, deployment, or relay-root sequencing waiver.

## Target and integrity

- Reviewed `docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-rules-c1-20260825.md` revision 2 at `main@b26a90a`. The working-tree blob and committed blob both match the declared SHA-256 `243346d7948f1ac9aa7bacbfe6ee7d2f467ac6692a46ab41b62208c9808ed266`. — E1 — exact-file read, `git show`, `sha256sum`.
- The requesting relay is byte-identical to `b26a90a`, parents this re-review to `engine-c1-design-review-1`, and passes exact-file relay lint. — E1 — `git show`, `sha256sum`, relay-lint.
- The harness owner design has since received Implementer approval at digest `11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440`; its approval expressly preserves an orchestrator-routed engine-adapter delta before either PLAN treats the seam as locked. — E1 — `harness-c1/DESIGN-REVIEW-pair-implementer-20260825-081315.md`.

## Must-revise findings

### MR1 — `valid` does not characterize the declared locally reachable state set

Section 3 says `valid = 0` for locally unreachable states, then defines that class only as assignments where both players have completed lines. Dual winners are not the only impossible static boards under immediate local closure. A concrete counterexample is `XXX/XXX/...`: X has two disjoint winning rows. No single last mark can complete both rows, so one row necessarily existed before the other was completed and the board would already have closed; the position is unreachable even though only X has winning lines. Under the current text it receives `valid = 1`, can be accepted by the adapter, and makes the exhaustive validity oracle underspecified. — E1 — design lines 46-59; canonical rule 6.

Required revision: define reachability mechanically, not as dual-winner shorthand. For a won board, require exactly one winning player AND at least one candidate last mark belonging to every completed line such that removing it leaves no completed line; open/full-draw boards require no winner. Equivalently specify and test another exact reachability algorithm. Make the independent 19,683-entry oracle enforce that rule, retain defined bytes for every invalid entry, and include same-player disjoint-line fixtures as well as dual-winner fixtures.

### MR2 — M5's withdrawn evidence still appears as a locked grill fact

Revision 2 correctly says the pure-copy probe was collapsible and withdraws “copy ≈ 0,” but the embedded GRILL_LOCK still says `copy ≈ free at 80 B`, still records the table width as 47 bits after adding the validity bit (the packed payload is now 48 bits), and still attributes “Value copy over make/unmake” generically to the benchmark. Those statements contradict §§2–3 and overstate what the directional playout comparison proves. — E1 — design lines 39-44, 46-59, 135-151; `/tmp/uttt_bench/bench.cpp` exact read from the first review.

Required revision: update the GRILL_SOURCE and resolved-decision ledger to the revision-2 evidence scope: playout made the two copy-based layouts directionally indistinguishable; simplicity selected the array layout/value-copy contract; the isolated copy probe proves nothing; the packed table is 48 bits. If make/unmake was never measured, do not claim that the benchmark compared it.

The eventual performance gate must also pin a baseline produced by a fixed reference path or by the committed harness BEFORE the implementation under test is evaluated, with the result/environment stored as a named artifact. Measuring the candidate and calling that same measurement its baseline makes “within 2×” tautological. The PLAN may establish the durable baseline, but its ordering and referent must be explicit in the design acceptance criterion. — E1 — design lines 43 and 176-182.

## Consistency corrections

- Section 2 still says budgets live in `BudgetContext`; revision 2 replaced that type with `RootContext`. Use one name throughout. — E1 — design lines 35-37 and 114-120.
- Section 6 still calls the harness design pre-lock, but the owner design is now approved. Float the successor to the approved harness digest, keep concrete wire bytes harness-owned, and preserve the addressed pre-PLAN orchestrator acknowledgment. Update §12's `Reads`/`Contract` wording to say the approved owner schema, not an unqualified moving draft. — E1 — design lines 78-92 and 166-173; harness approval `081315`.

## Closed findings and retained design

- M1 technical shape: closed subject to the status/name corrections above. The abstract adapter boundary, `RootContext {seat, budget_x, budget_o}`, X-relative T, and root perspective conversion eliminate the original own/opp ambiguity while preserving JSON/I/O isolation.
- M2: closed. Both terminal kinds produce no legal moves, and apply rejects post-terminal mutation; the test seam covers terminal and ordinary invalid-move classes.
- M3 fork semantics/table proof: closed except MR1's validity domain. Fork cells and distinct follow-up winning cells are mechanically defined, and the exhaustive oracle is named.
- M4: closed. Tie/NULL lifecycle, first-center state, immutability, key recomputation, all-closed X-margin outcomes, and both-mover perft semantics are explicit. `Position -> T` and X perspective match the canonical spec.
- Collision policy and value-quality shape remain approved as in the first review. Full-key verification is now correctly fieldwise rather than padding/cache sensitive.
- Search math remains outside this lock and gated by the separately approved theory design; no gated math is approved here.

## Design-review checklist

- Identity, locator, digest, target entity, and split lock surface: pass. — E1.
- Boundary contract: technical shape passes; current owner digest/status and routing citation need the consistency correction above. — E1.
- Rules totality: terminal semantics pass; LocalTable reachability must revise (MR1). — E1.
- Acceptance/test seam: lifecycle and exhaustive table shape pass; validity oracle and durable performance baseline must revise (MR1/MR2). — E1.
- Operator decisions/rejected alternatives: preserved, but the grill evidence ledger must be made internally truthful (MR2). — E1.
- Operator-judgment items: none. The remaining changes are technical corrections within Planner/orchestrator ownership.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — rules design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — game chips are simulation state, not real monetary state
- AI-or-automation-acts-downstream: no — no implementation/runtime action authorized
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes harness protocol and theory fixtures
- user-visible-control-with-materializer/downstream-consumer: no — no user-facing control changed
- test-runtime-role-mismatch: no — tests are design obligations only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — accepting an incomplete reachability oracle or self-referential benchmark baseline would retain lock risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `DESIGN_DOC_ID` with a fresh digest and `IN_REPLY_TO` naming this review. Close MR1/MR2 and both consistency corrections; cite the approved harness owner digest plus the addressed alignment gate; preserve the M1/M2/M4 closures, fork definition, value-quality/collision shapes, operator decisions, `GRILL_LOCK_ID`, and the rules/search-math split. No implementation or PLAN lock is authorized by this verdict.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-081432.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-081432`
- source/tests/design=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-081432.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-081315.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-planner-20260825-081516.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/reconcile-c1/RECONCILE-orchestrator-reviewer-20260825-081139.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-REVIEW-pair-implementer-20260825-080702.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-planner-20260825-081055.md
