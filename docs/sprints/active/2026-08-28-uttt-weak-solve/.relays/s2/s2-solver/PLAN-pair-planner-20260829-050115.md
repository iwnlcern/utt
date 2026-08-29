ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-solver-c3-plan-3
PARENT_DISPATCH_ID: s2-solver-c2-design-review-6
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan gate is the Implementer's PLAN-REVIEW; the standing cross-pair trigger keeps dispatch orchestrator-routed as already committed
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-REVIEW-pair-implementer-20260829-045713.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 83ed3bd1a3232d8e6d1dd5ff5f261af8a6dc064c721fe085ef1c2809e30cef55
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN s2-solver-c3-plan-3 — rev 3 folds the three plan-review-2 residuals; PLAN-REVIEW requested

## Team solver — solver-c3: PLAN rev 3 + PLAN-REVIEW REQUEST

Current scope for the `TO` addressee: read-only PLAN-REVIEW. No source/test edits, no branches, no commits, no PRs; no live dispatch token exists.

Reissued gated PLAN (same PLAN_LOCK_ID; digest floats; rev-2 digest 59b0f288…), re-parented to the same approving DESIGN-REVIEW; your must-revise rides IN_REPLY_TO. All accepted rev-2 folds, design digest, source fence, fixed spike protocol, and merge hold preserved. Fold map:

- MR-1 residual folded: the crash-matrix and resume-e2e `add_test` registrations move to Task 15 where `solver_solve` is created (your generate-time `No target` reproduction accepted); Task 12 authors and unit-tests script/manifest bytes only. Target membership is now explicit both as a standing rule in Global Constraints (src→solver_core, tests→solver_tests, tools→their executables with creation tasks named) and per task for T7, T9–T14, T16, T17, so the per-task ctest gate generates and compiles at every step.
- MR-2 residual folded: counters are `BigUint` (hand-rolled arbitrary-precision unsigned, header `src/store/biguint.hpp` — add/compare/decimal only; zero new dependencies), tested with exact decimal strings above 2^128, satisfying DD §8's unbounded-width contract verbatim with no CLI narrowing. Census checkpointing is now atomic: `census-ckpt.json` (temp→fdatasync→rename→dirsync) binds completed_ply + partial-row digest + frontier digest in one record; resume verifies both digests, truncates the partial CSV to the covered length, and falls back to the predecessor checkpoint on any mismatch; interruption tests cover mid-ply and both publication edges (append-vs-publish, frontier-vs-publish) via a `--pause-before-ckpt` hook, each proving byte-identical final output.
- MR-4 residual folded: fresh `AltSolver` instance per corpus row (memo persistence across solve() acknowledged as the accumulation hazard), plus an enforceable memory hard bound — the row's stop callback fires at memo_entries > 20e6 (~2.8 GiB at the audited ~140 B/entry; the memo IS the retained allocation, so the entry budget is a direct bound). Memo-cap exits classify with node/wall incompletes, never compared; predeclared minimums stay red on shortfall; one test feeds a high-empties state and asserts incomplete exit with ru_maxrss growth under 4 GiB, proving the bound enforceable. Seeds, empties ≤ 10 rule, node/wall caps, Exact-only comparison unchanged; kernel A shares the identical lifecycle.

Locked design: DD-solver-c2-20260828 @ e5cd2ae3… (main@8bb972b); approving review s2-solver-c2-design-review-6 (parent).
Plan of record: docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md rev 3 @ sha256 above (17 tasks, census-first order unchanged).

Dispatched path set (SCOPE_DIFF basis, unchanged from rev 2): solver/** (incl. untracked gitignored solver/build/** and scratch); sprint census/**; sprint spike/**; plans/PL-solver-c3-20260829.md (reissues only); .relays/s2 filings. engine/**, theory/**, ui/** OUT.

Out of scope, boundary contract, acceptance criteria (7, updated for BigUint/checkpoint-edges/memory-lifecycle), verification target E2: per the plan's sections. Ceremony downgrade: none. Operator-judgment items: none.

Dispatch consequence (standing, unchanged): the scan below remains trigger-present on the cross-pair census/schema contract, so on your approve I will NOT issue an implementation token; the approved plan routes to s2.orchestrator-planner for direct dispatch or condition amendment.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game-solver code and artifacts only
- migration/backfill/destructive-write/canonical-data-repair: no — new immutable local formats and disposable gitignored scratch
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — c3 single-threaded; local file-processing recovery tested in-suite
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver-census-v1 is theory-consumed and solver consumes the pinned theory contract; orchestrator routing per ROADMAP gate 1
- user-visible-control-with-materializer/downstream-consumer: no — no UI surface
- test-runtime-role-mismatch: no — registrations now follow target creation; the per-task gate generates and runs at every step; oracle compares complete+Exact rows under a tested memory lifecycle
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — unbounded-width counters honored verbatim; checkpoint publication edges tested; no residual accepted
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or residual-risk acceptance requested; the trigger consequence is the orchestrator-dispatch route above
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Requested response: PHASE: PLAN-REVIEW, FROM: solver.implementer, TO: solver.planner, PARENT_DISPATCH_ID: s2-solver-c3-plan-3, same PLAN_LOCK_ID, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plans/PL-solver-c3-20260829.md amended in place to rev 3 by this seat (tracked-modified, sha256 83ed3bd1a3232d8e6d1dd5ff5f261af8a6dc064c721fe085ef1c2809e30cef55); no other edits, commits, branches, or PRs; git status --short below
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-REVIEW-pair-implementer-20260829-045713.md
(this seat's artifact: the plans/PL-solver-c3 line; INDEX/lock are daemon/harness-owned; the PLAN-REVIEW row is the implementer's filing)
