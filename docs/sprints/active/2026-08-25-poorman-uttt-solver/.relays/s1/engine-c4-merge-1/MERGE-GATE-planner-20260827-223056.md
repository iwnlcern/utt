ROLE: Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: engine-c4-merge-1
PARENT_DISPATCH_ID: engine-c4-impl-report-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: yes — merge authority is the orchestrator/operator gate; this relay only reconciles and recommends
FROM: engine.planner
TO: s1.orchestrator-planner
CC: s1.orchestrator-reviewer, engine.implementer, theory.planner
IN_REPLY_TO: engine-c4-impl-report-3/IMPL-pair-implementer-20260827-222210.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: f229caf42f1bfb8b64dbe9f8a3fd8e1e4b467a74b8a5194c1926d1492e239acf
BUNDLE_ID: engine-c4
OWNER: engine pair
REPO: utt (origin https://github.com/iwnlcern/utt)
BASE: origin/main@e3798aa22460600977e4626e0b6ac56b8e544d69
TARGET_BRANCH: main
BRANCH: engine/search-math-c4
PR: https://github.com/iwnlcern/utt/pull/10 — OPEN, non-draft, MERGEABLE at exact head 5b147b80ff39587c65dada6851a250dd6c93ba34, title refreshed to DD rev 13
SUBJECT: MERGE-GATE recommendation — engine-c4 reconciled at 5b147b8 after the full adversarial cycle; recommend merge of PR #10

## Reconciliation

The refreshed A1–A11 ledger of engine-c4-impl-report-3 is reconciled as reported: A1–A9 green-E2, A10 green-E3 (100% sweep of all four baselines, ≥90% G4 bar), A11 green-E2+E3 (83 attempts validated, staging metadata clean).
This head is the panel-hardened successor of the original PR #10 head: the four-lens adversarial panel's 1 Critical + 8 Important findings were folded through DD revs 11–13 (three delta design reviews, one operator-corrected zero-total policy, one review-caught unsound certificate carrier) and plan revs 6–7, then implemented as Task 16 with 19 recorded run-and-revert mutations.

## Independent planner verification (fresh this session, detached worktree at the exact head)

- Task 16 scope diff: exactly the eleven reported paths (CMakeLists untouched as honestly disclosed); `git diff --check` clean — E1/E2.
- PR state re-queried live: OPEN, non-draft, MERGEABLE at exactly 5b147b80ff39587c65dada6851a250dd6c93ba34; title carries DD rev 13 — E1.
- The panel's CRITICAL verified FIXED at source: `decision.move` is seeded from the staged conditional best move (policy.hpp lines 277–279) before every matrix-stage early return — the `legal.front()` path is gone.
- Fresh build + full ctest at the head: 2/2, zero failures (51 s) — E2.
- FULLY INDEPENDENT A10 rerun (fresh referee venv, this seat's own invocation): random 50/50, zero 50/50, fraction 50/50, allin_tactical 50/50, zero faults/voids — the 100% sweep reproduces — E3.

Review tier: the four-lens adversarial panel ran on the pre-fold head; the fold changed exactly the panel's must-fix surface under three approving delta design reviews and two plan reviews, so the post-fold check was the protocol's targeted quick-check (above) rather than a fresh panel.

## Recommendation

RECOMMENDED_VERDICT: merge — merge PR #10 at exact head 5b147b80ff39587c65dada6851a250dd6c93ba34 into main.
Merge authority remains with the orchestrator/operator; nothing in this relay or the PR state confers it.

What this merge lands: the complete engine-c4 search-math bundle — directed-interval threshold search with first-class cutoffs, the 32-byte collision-aware TT, selective widening with quality honesty, the exact-integer P2 gate, the theory-parity root matrix with graded estimate payoffs (G5), the zero-total alternation solver (G6), the certified/heuristic forced-action split, the fair-coin ply-0 model, request-aware clocks, the bounded analyze ABI, and the full A1–A11 acceptance surface including two independently reproduced 100% gauntlet sweeps.

Post-merge notes (non-blocking):
1. The panel's implementer-discretion list (perf/idiom items: comparator decoration, TT store pollution, per-turn TT rebuild constants, RM+ allocations, FLT_EVAL_METHOD assert, etc.) remains available for a small hardening cycle if the operator wants one; none affects correctness or the acceptance surface.
2. The branch's committed copies of the DD/plan are historical revisions; the specs of record (DD rev 13, plan rev 7) live on main and merge cleanly.
3. engine-c4 closure ends the engine pair's s1 scope unless the operator commissions further cycles.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game engine only
- migration/backfill/destructive-write/canonical-data-repair: no
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — deterministic game engine; referee consumes replies
- worker/scheduler/queue/retry/async-side-effect: no — local tournament subprocesses only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory/referee contracts consumed read-only; analyze JSONL is engine-owned
- user-visible-control-with-materializer/downstream-consumer: yes — published bids/moves and analysis metadata feed referee/ui/fitter
- test-runtime-role-mismatch: no — E3 used the real referee and the production binary; the panel's mismatch findings are closed
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the full ledger is closed; residual items are disclosed discretionary polish
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested; both triggers are the known, ruled contract-consumption facts
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none — recommend-only relay; verification ran in a detached throwaway worktree (removed); no branch movement, merge, publication, deployment, or release action (filing commit named in the turn report)
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay authored before the filing commit; exact post-commit `git status --short` in the engine.planner turn report
