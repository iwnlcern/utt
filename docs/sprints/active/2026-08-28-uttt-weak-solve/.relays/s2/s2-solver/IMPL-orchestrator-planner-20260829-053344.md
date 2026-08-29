ROLE: Orchestrator Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: s2-solver-c3-impl-1
PARENT_DISPATCH_ID: s2-solver-c3-plan-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — implementation dispatch under the approved plan gate; merge remains the separate standing operator gate
FROM: s2.orchestrator-planner
TO: solver.implementer
CC: solver.planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/SITREP-pair-planner-20260829-052843.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@334d096
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: DISPATCH IMPL — solver-c3 proof-engine bundle under PL-solver-c3-20260829 rev4

## Implementation dispatch (orchestrator direct)

Basis, verified fresh at this seat:

- Plan gate: PL-solver-c3-20260829 rev 4 @ sha256 0c12e1723a1e91191bfe807fc8a7cea9f8332100608f3d8f50e9782e477b891e (byte-verified on disk, committed at main@334d096 — pin this as the plan source commit); approving review s2-solver-c3-plan-review-4 (verdict approve, all seven acceptance criteria APPROVE, digest match), parented to plan-4, which parents the approving design review s2-solver-c2-design-review-6 per the gated design-doc rule.
- SCOPE_DIFF from the pair planner's gate report: all-in (solver/**, sprint census/**, sprint spike/**, plan reissues, .relays/s2 filings). That path fence is the dispatch's in-scope list; anything else is a deviation that stops work and relays back.
- Standing trigger disposition: both seats' scans are trigger-present solely on the standing cross-pair census/schema contract (solver-census-v1 → theory; pinned theory schema ← solver). That contract is already orchestrator-routed (RECONCILE.md O13, ROADMAP gate 1); it is hereby accepted as the known standing condition of this bundle, not a new risk. This orchestrator direct dispatch is the override path; the pair's delegated conditions in proceed-1 remain unexercised and unchanged.
- Execution per the approved plan under Superpowers executing-plans: work on BRANCH solver/c3-proof-engine from BASE main@334d096; oracle conformance gates (AltSolver shallow horizons, theory fixtures) are plan acceptance criteria; census output lands in the theory-consumable format.
- Exit: branch + PR + implementation report parented to this dispatch id. No merge authority is granted here — merge rides its own MERGE-GATE relay after review/fold.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay; plan batch commit 334d096 is the orchestrator turn's edit (see repo log)
FINAL_GIT_STATUS_SHORT: unavailable — dispatch authored immediately after the plan batch commit; the orchestrator turn report carries post-commit repo state
