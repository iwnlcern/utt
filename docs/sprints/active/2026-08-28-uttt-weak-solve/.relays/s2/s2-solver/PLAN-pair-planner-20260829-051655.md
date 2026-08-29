ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-solver-c3-plan-4
PARENT_DISPATCH_ID: s2-solver-c2-design-review-6
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan gate is the Implementer's PLAN-REVIEW; dispatch stays orchestrator-routed per the standing trigger
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-REVIEW-pair-implementer-20260829-051453.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 0c12e1723a1e91191bfe807fc8a7cea9f8332100608f3d8f50e9782e477b891e
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN s2-solver-c3-plan-4 — rev 4 folds the census-checkpoint generation residual; PLAN-REVIEW requested

## Team solver — solver-c3: PLAN rev 4 + PLAN-REVIEW REQUEST

Current scope for the `TO` addressee: read-only PLAN-REVIEW. No source/test edits, no branches, no commits, no PRs; no live dispatch token exists.

Reissued gated PLAN (same PLAN_LOCK_ID; digest floats; rev-3 digest 83ed3bd1…), re-parented to the same approving DESIGN-REVIEW; your must-revise rides IN_REPLY_TO. Every accepted rev-2/rev-3 fold, the design digest, source fence, fixed spike protocol, and merge hold are preserved; only the census checkpoint lifecycle changed. Fold map (MR-2 final residual, all five numbered revisions):

1. Immutable generation-scoped records `census-ckpt-<ply>.json`, never overwritten; resume scans newest-to-oldest and selects the highest generation that validates end to end (your singleton-rename analysis accepted — it was the same unlink-the-predecessor class psm-v1/psc-v1 already fixed, now applied to the census).
2. Each record binds format/version, completed ply (the generation), exact `partial_bytes`, the partial-prefix XXH3-128 over exactly those bytes, and the immutable frontier file list with per-file name/size/digest; all referenced bytes are durable before the record publishes (temp→fdatasync→rename→dirsync).
3. Predecessor record and frontier files are physically retained until the successor record is durable; GC removes only generations older than the newest two.
4. Interruption matrix extended: kill after partial append, after frontier durability, after successor publication but before predecessor GC, mid-ply, plus a corrupted-successor case (post-publish frontier byte flip) — each resume selects the valid successor or the retained predecessor and reproduces byte-identical final CSV.
5. Task 8's commit label corrected to "arbitrary-precision counters".

Locked design: DD-solver-c2-20260828 @ e5cd2ae3… (main@8bb972b); approving review s2-solver-c2-design-review-6 (parent).
Plan of record: docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md rev 4 @ sha256 above (17 tasks, census-first order unchanged).

Dispatched path set (SCOPE_DIFF basis, unchanged): solver/** (incl. untracked gitignored solver/build/** and scratch); sprint census/**; sprint spike/**; plans/PL-solver-c3-20260829.md (reissues only); .relays/s2 filings. engine/**, theory/**, ui/** OUT.

Out of scope, boundary contract, acceptance criteria, verification target E2: per the plan's sections (criterion 6 already carries the checkpoint-edge matrix). Ceremony downgrade: none. Operator-judgment items: none.

Dispatch consequence (standing): the scan below remains trigger-present on the cross-pair census/schema contract; on your approve I will NOT issue an implementation token — the approved plan routes to s2.orchestrator-planner for direct dispatch or condition amendment.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game-solver code and artifacts only
- migration/backfill/destructive-write/canonical-data-repair: no — new immutable local formats and disposable gitignored scratch
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — c3 single-threaded; local file-processing recovery tested in-suite
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver-census-v1 is theory-consumed and solver consumes the pinned theory contract; orchestrator routing per ROADMAP gate 1
- user-visible-control-with-materializer/downstream-consumer: no — no UI surface
- test-runtime-role-mismatch: no — registrations follow target creation; oracle rows have bounded lifetimes; checkpoint fallback is now physically realizable and tested
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the retained-predecessor lifecycle closes the named recovery gap; no residual accepted
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or residual-risk acceptance requested; the trigger consequence is the orchestrator-dispatch route above
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Requested response: PHASE: PLAN-REVIEW, FROM: solver.implementer, TO: solver.planner, PARENT_DISPATCH_ID: s2-solver-c3-plan-4, same PLAN_LOCK_ID, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plans/PL-solver-c3-20260829.md amended in place to rev 4 by this seat (tracked-modified, sha256 0c12e1723a1e91191bfe807fc8a7cea9f8332100608f3d8f50e9782e477b891e); no other edits, commits, branches, or PRs; git status --short below
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-REVIEW-pair-implementer-20260829-045713.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-REVIEW-pair-implementer-20260829-051453.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-pair-planner-20260829-050115.md
(this seat's artifact: the plans/PL-solver-c3 line; INDEX/lock are daemon/harness-owned; PLAN-REVIEW rows are the implementer's filings)
