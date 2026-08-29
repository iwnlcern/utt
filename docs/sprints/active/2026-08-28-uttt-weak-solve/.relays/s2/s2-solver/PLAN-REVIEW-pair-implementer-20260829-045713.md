ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c3-plan-review-2
PARENT_DISPATCH_ID: s2-solver-c3-plan-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining build-graph, census-width, checkpoint-publication, and oracle-resource defects are bounded plan repairs; the standing cross-pair trigger already routes any later implementation token to the orchestrator
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-pair-planner-20260829-035142.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 59b0f28847cf7e4125170f92a1cc2ffd7cfb5ee50dd3a01f670cb79d06518712
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN-REVIEW solver-c3 rev2 — most folds accepted; three executable-contract residuals remain

## Scope and identity

Reviewed all 258 lines of `PL-solver-c3-20260829` rev 2 at SHA-256 `59b0f28847cf7e4125170f92a1cc2ffd7cfb5ee50dd3a01f670cb79d06518712`, the addressed successor relay, revision-6 locked design, prior PLAN-REVIEW, orchestrator proceed relay, CMake generator-expression behavior, and AltSolver limit/memo semantics. The plan and design digests match their relays; lineage is the required same-design-review reissue with the prior must-revise in `IN_REPLY_TO`; exact-file relay lint has no errors or warnings. No source, test, fixture, design, plan, branch, commit, PR, census, spike, or external-run change was made.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

Rev 2 materially closes the sequencing, build-main/ignore/staging, active-batch visibility, K/root, record ingress, TT interleaving/polarity, crash-case inventory, bounded near-terminal oracle shape, long-run locations, and escalation-routing defects. Preserve those folds. Implementation dispatch remains held because Task 12 makes the promised per-task CMake gate fail before Task 15 exists, the census still implements a fixed-width counter under an unbounded-width locked contract and has no atomic frontier/row checkpoint binding, and the exact-oracle gate still omits the requested enforceable memory lifecycle.

## Acceptance-criteria disposition

1. Section 4 spike pre-registration: PASS at plan level — fixed seeds/tiers/rule, 2-hour probe cap, 72-hour persisted aggregate budget, resume state, censoring/floor/default, and durable sprint paths are named.
2. Section 5 durability/visibility and root exemption: PASS at plan level — active-batch lookup, checkpoint exclusion/order pin, K-1/K/K+1, root bypass at maximum K, and ply-0 resume are named.
3. Section 5 collision safety and Kind-2 semantics: PASS at plan level — locked sidecar equality, two-phase replacement pin for both kernels, and all claim-pair/state-shape witness cases are named.
4. Section 5 psl-v1 deterministic compaction: PASS at plan level.
5. Section 6 checkpoint/recovery: PASS for the storage stack; the separate census checkpoint publication residual below keeps criterion 6 partial.
6. Section 8 census: PARTIAL — early ordering, schema, anchors, digest, scoped scratch, partial artifact, and interruption/resume test are present; fixed `unsigned __int128` is not the locked unbounded-width count contract, and partial-row checksums are not bound to the retained frontier across a boundary-publication crash.
7. Section 10 oracle gates: PARTIAL — the deterministic near-terminal corpus, node/wall caps, Exact-only comparison, incomplete accounting, and red minimums are sound; a fresh-per-row or enforceable RSS/memory lifecycle remains unspecified.

## MR-1 residual — defer the crash test registration until its executable exists

Plan line 182 registers `solver_crash_matrix` with `$<TARGET_FILE:solver_solve>` in Task 12, while line 206 first creates `solver_solve` in Task 15. This is not a test that merely stays red until Task 15: CMake generation itself refuses `$<TARGET_FILE:solver_solve>` when the named target does not exist. A minimal reproduction under the installed CMake 4.3.1 fails at generate time with `No target "solver_solve"`. It also contradicts the global constraint and verification section promising a green `ctest` gate at every task.

Required revision: Task 12 may create and unit-test the manifest/crash-script bytes, but register `solver_crash_matrix` only in Task 15 after `solver_solve` is created, or create a linkable solve target before the registration. Name the target membership for Tasks 7, 9-11, 13-14, 16, and 17 rather than relying on the global assertion alone.

## MR-2 residual — implement the census contract without silent narrowing

Locked DD section 8 requires every count to be emitted as a decimal ASCII integer of unbounded width. Plan lines 148, 151-152, and acceptance criterion 6 instead pin `unsigned __int128` and only a 2^70 test. That is a larger fixed ceiling, not the approved unbounded-width contract; the generic `--max-ply P` interface names no maximum whose mathematical count bound is proved below 2^128.

The resume description also publishes two coupled facts — the last completed CSV row and the retained frontier — without a digest/epoch binding or atomic selection rule. Per-line checksums can accept a newly appended row while a crash leaves only the predecessor frontier (or the reverse); the named mid-ply kill does not exercise that publication boundary.

Required revision: use an arbitrary-precision nonnegative integer representation for path/cumulative/derived counts (for example `boost::multiprecision::cpp_int`) with decimal tests above 2^128, or explicitly narrow the CLI and prove the locked contract permits that narrowing before implementation. Bind completed ply, partial-row digest, and frontier digest in an atomically published checkpoint record (or define an equivalent predecessor-fallback rule), and test interruption at each checkpoint publication edge as well as mid-ply.

## MR-4 residual — make the oracle memory bound executable

The prior required revision called for node, time, and memory caps. Rev 2 supplies `node_cap = 5e7` and 30 seconds per state but no RSS/memory cap and no solver-lifetime rule. `AltSolver` owns a memo map that is not cleared by `solve()`, so reusing one instance across the 600 rows accumulates entries beyond any one row's node cap; the plan does not require a fresh instance/process per row. A time callback does not bound retained allocation.

Required revision: instantiate/destroy a fresh `AltSolver` per corpus row and state an enforceable per-row RSS/memory ceiling (subprocess/resource limit, allocator budget, or another tested hard bound). Classify memory-bound exits with the other incomplete rows, never compare them, and keep the predeclared exact-row minimum red on shortfall. Preserve the current seeds, empties <= 10 rule, node/time caps, and Exact-only comparison.

## Required successor

Amend `PL-solver-c3-20260829` forward without changing `PLAN_LOCK_ID`; preserve all accepted rev-2 folds, design digest, source fence, fixed spike protocol, and merge hold; float `PLAN_SHA256`; reissue `PHASE: PLAN` with `DISPATCH_ID: s2-solver-c3-plan-3`, parented to `s2-solver-c2-design-review-6` and `IN_REPLY_TO` this review. Do not run SCOPE_DIFF or issue implementation dispatch on this verdict. After a later approve, the trigger-present scan still forbids pair-delegated dispatch; route the approved plan to `s2.orchestrator-planner` exactly as rev 2 promises.

OPERATOR_ITEMS: none — these are execution-contract repairs inside the approved design; no product-semantics ruling or waiver is requested

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game-solver code and artifacts
- migration/backfill/destructive-write/canonical-data-repair: no — new immutable artifacts and disposable scratch only
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — c3 stays single-threaded; local interruption recovery only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver-census-v1 is theory-consumed and solver consumes the pinned theory contract; direct orchestrator routing remains required
- user-visible-control-with-materializer/downstream-consumer: no — no UI surface
- test-runtime-role-mismatch: yes — Task 12 references a target not created until Task 15, so the claimed per-task CMake/ctest gate cannot run
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approving fixed-width counters, an unbound census checkpoint pair, or an accumulating oracle memo would accept residual execution risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Verification:
- E1: incoming relay exact-file lint with `--no-freshness` reports zero errors and zero warnings; its PLAN_SHA256 exactly matches the current 258-line plan.
- E1: relay daemon status is ready/active with zero conflicts and zero pending renders; `relay verify` reports `ok: true` before filing.
- E1: local CMake 4.3.1 minimal reproduction of Task 12's missing `$<TARGET_FILE:solver_solve>` fails generation with `No target "solver_solve"`.
- E1: exact design/plan comparison shows DD line 262 says unbounded width while plan line 148 pins `unsigned __int128` and a 2^70 test.
- E1: `engine/src/root/alt_solver.hpp` shows the per-instance memo persists across `solve()` calls and only node/time-stop inputs exist; rev 2 names no memory/lifetime gate.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/census/spike/external-run changes claimed; authored only this PLAN-REVIEW relay for daemon filing; concurrent scheduler/index bytes remain foreign/daemon-owned
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
