ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c3-plan-review-4
PARENT_DISPATCH_ID: s2-solver-c3-plan-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the plan is approved without residual risk; the standing cross-pair trigger requires direct orchestrator routing before any implementation token
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-pair-planner-20260829-051655.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 0c12e1723a1e91191bfe807fc8a7cea9f8332100608f3d8f50e9782e477b891e
PLAN_REVIEW_VERDICT: approve
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN-REVIEW solver-c3 rev4 — approve retained-generation census recovery and locked implementation plan

## Scope and identity

Reviewed all 265 lines of `PL-solver-c3-20260829` rev 4 at SHA-256 `0c12e1723a1e91191bfe807fc8a7cea9f8332100608f3d8f50e9782e477b891e`, the addressed successor relay, locked design section 8, the prior three PLAN-REVIEW returns, and current daemon lineage. The plan and design digests match their relays; the gated PLAN correctly re-parents the approving design review and carries round-3 review in `IN_REPLY_TO`; exact-file relay lint has zero errors and zero warnings.

No source, test, fixture, design, plan, branch, commit, PR, merge, census, spike, publication, deployment, or external-run change was made by this seat.

## Verdict

PLAN_REVIEW_VERDICT: approve

Revision 4 closes the last census durability residual without reopening accepted folds. Each checkpoint is now an immutable generation-scoped record binding format, completed ply, exact partial-prefix length and digest, and immutable frontier names, sizes, and digests. Referenced bytes become durable before publication; the predecessor record and frontier remain physically present until the successor record is durable; GC retains the newest two generations; resume selects the highest fully valid generation and truncates the partial file to its bound prefix.

The interruption matrix now kills after partial append, after frontier durability, after successor-record publication but before predecessor GC, and mid-ply. It also corrupts a published successor frontier and requires fallback to the retained predecessor. Every path must reproduce final CSV bytes exactly. The Task 8 commit label now records arbitrary-precision counters. These are the five required round-3 repairs.

## Acceptance-criteria disposition

1. Section 4 spike pre-registration: APPROVE at plan level — seeds, tiers, decision rule, timeouts, persistent consumed budget, censoring, floor, and default are explicit.
2. Section 5 durability/visibility and root exemption: APPROVE at plan level — active-batch visibility, checkpoint ordering, K boundary trio, and root durability are directly tested.
3. Section 5 collision safety and Kind-2 semantics: APPROVE at plan level — sidecar identity, locked two-phase publication, derivation polarity, witness rules, and both-kernel reruns are explicit.
4. Section 5 psl-v1 deterministic compaction: APPROVE at plan level — exact format refusals, closure-implied contradictions, and byte-identical permuted compaction are named.
5. Section 6 storage checkpoint/recovery: APPROVE at plan level — durable publication, corrupt-new/valid-old fallback, root-status gate, crash matrix, and newest-two retention are explicit.
6. Section 8 census: APPROVE at plan level — early ordering, audited anchors, arbitrary-precision counters, deterministic CSV/digest, immutable generation records, exact partial-prefix binding, retained frontier generations, edge interruption tests, and corrupted-successor fallback are executable and mutually consistent.
7. Section 10 oracle gates: APPROVE at plan level — fresh AltSolver lifetime per row, Exact-only comparisons, separately reported incomplete rows, red minimums, node/wall/memo-entry caps, tested RSS ceiling, and cross-kernel agreement are explicit.

## Boundary contract

Writes: solver implementation and tests under `solver/**`; the early ply-10 `solver-census-v1` CSV and digest; c3 spike state/report; later PLAN reissues and relay filings only in their named paths.

Reads: locked solver design `DD-solver-c2-20260828` at `e5cd2ae3…`, pinned theory contract and fixtures, and the s1 rules kernel without modifying `engine/**` or `theory/**`.

Target entity: local-first solver-c3 proof engine with durable truth storage, two proof kernels, and the early census deliverable.

Downstream consumer: theory consumes `solver-census-v1`; later theory checker/certificate work consumes the pinned solver seam.

Contract: 37-byte state identity, `d4-lexmin-v1`, locked claim semantics, exact persistent formats, deterministic census columns/digest, and orchestrator-routed seam changes.

Proof: E2 task gates, locked fixtures and exact-oracle corpora, checkpoint interruption matrices, full solver and engine suites, committed census bytes, and pre-registered spike report.

No-consumer action: not applicable — theory is the named commissioned consumer; any seam friction routes to the orchestrator.

## Authority and next gate

This approval is PLAN-REVIEW only. It authorizes no implementation, branch, commit, PR, merge, publication, deployment, or release. Because the plan relay's scan is trigger-present on a cross-pair census/schema contract, the Pair Planner must route this approved plan to `s2.orchestrator-planner`; the Pair Planner must not self-issue an implementation token. Work begins only on a later addressed relay carrying a live bare own-line implementation dispatch from the orchestrator or after an explicit condition amendment.

OPERATOR_ITEMS: none — no product-semantic ambiguity, scope expansion, ceremony downgrade, or residual-risk acceptance remains in the reviewed plan

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game-solver code and artifacts only
- migration/backfill/destructive-write/canonical-data-repair: no — new immutable formats and disposable gitignored scratch
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — c3 is single-threaded; local interruption recovery is tested in-suite
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver-census-v1 is theory-consumed and solver consumes the pinned theory contract; direct orchestrator routing remains required
- user-visible-control-with-materializer/downstream-consumer: no — no UI surface
- test-runtime-role-mismatch: no — registrations follow target creation; oracle rows have bounded lifetimes; retained checkpoint fallback is physically realizable and tested
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the final retained-generation defect is repaired; no waiver used
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming relay exact-file lint with `--no-freshness` reports zero errors and zero warnings; its PLAN_SHA256 equals the current 265-line plan.
- E1: SHA-256 of the locked design is `e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2`; SHA-256 of the reviewed plan is `0c12e1723a1e91191bfe807fc8a7cea9f8332100608f3d8f50e9782e477b891e`.
- E1: Task 8 lines 152-156 name immutable generation records, `partial_bytes`, exact prefix digest, immutable frontier metadata, newest-two retention, newest-valid selection, all four kill edges, and corrupted-successor fallback.
- E1: relay daemon status is ready/active with zero conflicts and zero pending renders; pre-filing `relay verify` reports `ok: true`.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/merge/census/spike/publication/deployment/external-run change claimed; authored only this approval draft for daemon filing; final status below attributes planner, daemon, scheduler, and prior-review bytes separately
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-REVIEW-pair-implementer-20260829-045713.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-REVIEW-pair-implementer-20260829-051453.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-pair-planner-20260829-050115.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-pair-planner-20260829-051655.md
