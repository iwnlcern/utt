ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-plan-review-1
PARENT_DISPATCH_ID: theory-c1-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — theory.planner can revise the plan; P1/P2 ratification remains the later operator gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/theory-c1/PLAN-planner-20260825-082457.md
SUBJECT: PLAN-REVIEW must-revise — repair DAG order, reachable-state proof, LP ladder evidence, file hygiene, and completion handoff
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 943d25db26fce8d9891ee5989cbd5d8be5946a63bd7cf498f738a02a8c6d8cbe
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: 29ffb86f7083780624b359649019f1dfb83158b80c0b13bd52e4c7478d7d9b83
PLAN_REVIEW_VERDICT: must-revise

## Verdict

The plan has the right locked design, target surface, boundary encoding, TDD shape, hard stops, and downstream consumers, but it is not safely executable yet. Five mechanical gaps can produce a broken implementation or evidence that does not establish the approved claims. Revise the same PLAN_LOCK_ID and float the digest forward in a successor PLAN parented again to theory-c1-design-review-3; no implementation is authorized by this review.

Current scope: read-only review of PL-theory-c1-20260825 against approved DD revision 3 and theory-c1-proceed-1. Not in scope: source/test edits, dependency resolution, branch creation, commits, PRs, scaffolding, prototypes, delegated dispatch, merge, or operator ratification.

Duplicate/already-built check: no `theory/` directory, `theory/stage1-c1` branch, or prior implementation history exists in the current checkout, so this is new work rather than a rebuild.

## Must-revise findings

### PR1 — Reverse both DAG traversals

Tasks 4 and 7 say to process boards by descending empty-cell count while claiming children are already computed (plan lines 237 and 309). Every move removes one empty cell, so a child has `E-1`; child-first backward induction must traverse ascending empty-cell count (or use explicit recursive memoization with a proven post-order). Descending order visits the empty root before any child and cannot satisfy the stated table dependency.

Revise both algorithms and add a regression assertion that every nonterminal lookup observes already-materialized child entries. Keep marks-only DAG induction and terminal-first initialization explicit.

### PR2 — Represent reachability and scale semantics explicitly

The locked DD limits C8/C9 claims to states reachable from initial `(bx, bo) = (N, N)` with legal payments, moves, tie-owner transitions, and terminal stopping (design lines 44, 48, 89, 92). Tasks 7–9 instead materialize every board/budget Cartesian point and raise `NonDeterminacy` on any inequality without defining a reachable-state set (plan lines 303–314, 329–346, 383–394). That can turn an unreachable synthetic point into a false hard stop and makes occupancy/cross-check statistics differ from the approved population.

Add an exact reachability enumerator or mask per starting scale and specify that C8/C9 acceptance, anomaly statistics, and hard escalation consume only that mask. A superset table may remain an internal optimization, but an off-mask disagreement is diagnostic, not the C9 hard trigger. Define terminal-state treatment so `d/w(s)` never divides by zero when `E(s)=0`, while retaining the recorded `M=0` exclusion. For N=64/128, name an on-demand/targeted evaluation path that can solve the selected reachable knife-edge/zugzwang points without ambiguously requiring the full `(N+1)^2` table for every board.

### PR3 — Make LP fallback and emitted-case evidence non-vacuous

The constructed duplicate-row matrix test allows either `exact-verified` or `exact-fallback-verified` (plan lines 434–438), so it can pass without exercising the perturbation/exact-simplex failure path that DD section 4 requires. Introduce a deterministic seam that forces the initial support certificate to fail, then assert the retry/fallback path and final exact certificate. Separately enumerate the root bid matrices emitted for C10 and require every one to pass exact certification; the current three unit matrices do not establish “every emitted case.” Preserve `LPEscalation` as an uncaught hard stop.

### PR4 — Close environment, scope, and commit hygiene

Task 1 runs `uv sync`, which writes `theory/uv.lock` and a project environment, but neither `uv.lock` nor an ignore policy is in the file list. The repository `.gitignore` contains only `*.swp` and `.DS_Store`; Python `__pycache__` output is therefore also unprotected. The broad `git add theory` can stage generated environment/cache bytes, while Tasks 2–12 call `git commit` without staging their new files at all (plan lines 65–66, 145–155, and each later commit step).

Add `theory/uv.lock` and a scoped ignore file to the plan, lock the dependency versions through uv, and replace every commit instruction with explicit intended-path staging plus a staged-diff check. Do not depend on a machine-global ignore file. Include the new files in the PLAN relay's in-scope list so the later mechanical SCOPE_DIFF can truthfully be `all-in`.

### PR5 — Add the required completion/report boundary

The approved DD requires a math-lock report relay with E2 evidence and an explicit P1/P2 ratification ask (design line 151). The plan ends after the FINDINGS commit and contains no final evidence bundle, PR creation, implementation report relay, or ratification handoff (plan lines 467–498). Add a final task that runs the complete pytest/sweep/fixture-regeneration/LP verification set, records exact commands and result paths, checks scope and git status, opens the implementation PR without merge, and files the theory Implementer report/math-lock handoff. If C8a is undischarged, that relay must carry the already-locked canonical in-band unavailability and 3x3-to-UTTT residual-risk wording; it must request operator ratification rather than imply it.

## Preserved strengths and gates

- Integrity and lineage are correct: the plan digest and DD revision-3 digest match their routed fields; theory-c1-design-review-3 is the approving design parent; theory-c1-proceed-1 grants conditional delegated-dispatch authority.
- Scope stays within `theory/`; engine, harness, and UI source remain out of scope. The integer-or-null `forced` convention and 080619 acknowledgment are carried correctly.
- Golden expectations remain spec-derived; C9 non-determinacy and `LPEscalation` remain hard stops; the N=24 fallback remains orchestrator-gated; P1/P2 remain proposals; C8a remains attempt-and-record.
- The relay-root INDEX is still red, so even a later approving review cannot be exercised through delegated dispatch until the run-level inversion block is cleared or operator-waived.

## Successor acceptance

Return revision 2 under the same PLAN_LOCK_ID with a fresh PLAN_SHA256 and `IN_REPLY_TO` naming this review. Close PR1–PR5 without changing DD revision 3, the theory-only boundary, the E2 target, or the standing human/math-lock and INDEX gates. The successor should remain a review request; it must not carry or imply a live implementation token.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — theory-only local package
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-theory analysis only
- AI-or-automation-acts-downstream: no — no autonomous external action
- worker/scheduler/queue/retry/async-side-effect: no — local batch solver only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — generated fixture schema is consumed by three sibling domains
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this bundle
- test-runtime-role-mismatch: no — Python implementation and Python E2 verification use the same local role
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — P1/P2 residual-risk acceptance remains operator-owned at the later math lock
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested by this review
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: governance-only PLAN-REVIEW relay and INDEX row authored; no source/test/plan/design edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-083256.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-082602.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-083444.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-082457.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/
