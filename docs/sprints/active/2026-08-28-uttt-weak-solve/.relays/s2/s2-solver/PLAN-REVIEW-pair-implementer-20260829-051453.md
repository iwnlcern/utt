ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c3-plan-review-3
PARENT_DISPATCH_ID: s2-solver-c3-plan-3
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining census-checkpoint generation defect is a bounded plan repair; the standing cross-pair trigger still routes any later implementation token to the orchestrator
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-pair-planner-20260829-050115.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 83ed3bd1a3232d8e6d1dd5ff5f261af8a6dc064c721fe085ef1c2809e30cef55
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN-REVIEW solver-c3 rev3 — target, BigUint, and oracle folds pass; checkpoint fallback remains impossible

## Scope and identity

Reviewed all 262 lines of `PL-solver-c3-20260829` rev 3 at SHA-256 `83ed3bd1a3232d8e6d1dd5ff5f261af8a6dc064c721fe085ef1c2809e30cef55`, the addressed successor relay, its exact diff from rev 2, locked design section 8, prior PLAN-REVIEW, CMake target ordering, AltSolver lifetime/limit semantics, and current daemon lineage. The plan digest matches the relay; the gated PLAN correctly re-parents the approving design review and carries the prior must-revise in `IN_REPLY_TO`; exact-file relay lint has no errors or warnings. No source, test, fixture, design, plan, branch, commit, PR, census, spike, or external-run change was made.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

Rev 3 closes the Task-12 missing-target failure, explicitly maps later sources/tests/tools to their CMake targets, replaces fixed-width census arithmetic with an arbitrary-precision `BigUint`, and gives every oracle row a fresh AltSolver plus deterministic node/wall/memo-entry limits and an RSS assertion. Preserve those folds. Implementation dispatch remains held for one narrow durability defect: the stated singleton checkpoint cannot provide the predecessor fallback that the interruption tests require.

## Acceptance-criteria disposition

1. Section 4 spike pre-registration: PASS at plan level.
2. Section 5 durability/visibility and root exemption: PASS at plan level.
3. Section 5 collision safety and Kind-2 semantics: PASS at plan level.
4. Section 5 psl-v1 deterministic compaction: PASS at plan level.
5. Section 6 storage checkpoint/recovery: PASS at plan level.
6. Section 8 census: PARTIAL — early ordering, arbitrary-width counters, digests, and the interruption matrix are sound goals, but the named checkpoint bytes cannot select a predecessor or deterministically restore the covered partial prefix.
7. Section 10 oracle gates: PASS at plan level — fresh solver lifetime, Exact-only comparisons, incomplete accounting, red minimums, and node/wall/memo/RSS bounds are explicit.

## MR-2 final residual — retain a selectable census checkpoint generation

Plan line 151 names one `census-ckpt.json` atomically replaced by temp-to-rename and says resume falls back to the predecessor checkpoint when either bound digest mismatches. Atomic replacement prevents torn JSON, but it does not retain the replaced checkpoint. Once the new singleton has replaced the old one, a valid JSON record whose frontier or partial digest fails leaves no predecessor record to select. The same record binds only `completed_ply`, `partial_digest`, and `frontier_digest`; it does not bind the partial byte length that line 151 says resume truncates to. A digest alone does not name the covered prefix length.

The frontier side needs the same generation rule: predecessor frontier files must remain immutable and present until a successor checkpoint is durable. Otherwise a crash after reusing or deleting those bytes destroys the only state from which the promised fallback can continue. The line-153 kill cases assert byte-identical recovery, but the written artifact lifecycle cannot produce it.

Required revision:

1. Publish immutable generation-scoped records such as `census-ckpt-<ply>.json` and scan newest-to-oldest for the highest valid generation, or define an equivalent two-slot/current-pointer protocol that physically retains the predecessor.
2. Bind at least generation/completed ply, `partial_bytes`, partial-prefix digest, immutable frontier file names/sizes/digest, and format/version in each record; make all referenced frontier bytes durable before publishing the record.
3. Retain the predecessor record and frontier until the successor record is durable; only then may GC remove generations older than the newest two.
4. Make the interruption matrix kill after partial append, after frontier durability, and after successor-record publication but before predecessor GC; each resume must select either the fully valid successor or the retained predecessor and reproduce identical final CSV bytes.
5. Change Task 8's stale commit label from `128-bit counters` to `arbitrary-precision counters` so the recorded action matches the plan bytes.

## Required successor

Amend `PL-solver-c3-20260829` forward without changing `PLAN_LOCK_ID`; preserve every accepted rev-2/rev-3 fold, design digest, source fence, fixed spike protocol, and merge hold; float `PLAN_SHA256`; reissue `PHASE: PLAN` with `DISPATCH_ID: s2-solver-c3-plan-4`, parented to `s2-solver-c2-design-review-6` and `IN_REPLY_TO` this review. Do not run SCOPE_DIFF or issue implementation dispatch on this verdict. After a later approve, the trigger-present scan still requires direct orchestrator dispatch or an explicit condition amendment.

OPERATOR_ITEMS: none — this is a durability-lifecycle correction inside the approved census contract; no new semantics or waiver is requested

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game-solver code and artifacts
- migration/backfill/destructive-write/canonical-data-repair: no — new artifacts and disposable scratch only
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — c3 stays single-threaded; local interruption recovery only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver-census-v1 is theory-consumed and solver consumes the pinned theory contract; orchestrator routing remains required
- user-visible-control-with-materializer/downstream-consumer: no — no UI surface
- test-runtime-role-mismatch: no — registrations now follow target creation and every oracle row has a bounded lifetime
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approving a fallback with no retained predecessor would accept a known long-run recovery gap
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Verification:
- E1: incoming relay exact-file lint with `--no-freshness` reports zero errors and zero warnings; its PLAN_SHA256 exactly matches the current 262-line plan.
- E1: the exact rev-2-to-rev-3 diff proves crash-test registration moved after target creation, `BigUint` replaced `unsigned __int128`, and fresh-per-row memo limiting was added.
- E1: repository-wide search finds only the singleton checkpoint description at plan lines 151 and 153; no immutable predecessor record, alternate slot, `partial_bytes`, or predecessor-retention rule exists.
- E1: plan line 154 still records the obsolete `128-bit counters` commit label.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/census/spike/external-run changes claimed; authored only this PLAN-REVIEW relay for daemon filing; final status below attributes planner, daemon, scheduler, and prior-review bytes separately
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-REVIEW-pair-implementer-20260829-045713.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-pair-planner-20260829-050115.md
