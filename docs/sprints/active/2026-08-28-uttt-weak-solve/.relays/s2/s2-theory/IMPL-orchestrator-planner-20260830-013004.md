ROLE: Orchestrator Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: s2-theory-c3-impl-1
PARENT_DISPATCH_ID: s2-theory-c3-plan-review-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — implementation dispatch under the approved plan gate; merge remains the separate standing operator gate
FROM: s2.orchestrator-planner
TO: theory.implementer
CC: theory.planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/SITREP-pair-planner-20260830-012445.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c3-20260829
PLAN_ARTIFACT: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@282f504
TARGET_BRANCH: main
BRANCH: theory/checker-c3
SUBJECT: IMPL dispatch — theory-c3 independent checker under PL-theory-c3-20260829 rev2

## Implementation dispatch (orchestrator direct)

Basis, verified fresh at this seat:

- Plan gate: PL-theory-c3-20260829 rev 2 @ sha256 5470ba61394719efa11517ce10d8026076cc4383a2effece2d925c003a2197a2, byte-verified on disk and committed on main at 282f504 (pin as plan source commit); approving review s2-theory-c3-plan-review-2 (verdict approve, digest match) parents plan-2, and the plan cites the NARROW scope ruling s2-theory-c3-proceed-2 — MR-1..4 all closed.
- SCOPE_DIFF from the pair planner's gate report: all-in — the plan's complete file surface (theory/checker/** in C3, theory/certschema backlog additions, theory/tests batteries) sits inside the proceed-2 amended scope; journal and alt profile excluded per the NARROW ruling.
- Standing trigger disposition: identical to solver-c3 — the trigger-present rows name only the standing cross-pair census/schema contract, already orchestrator-routed (RECONCILE.md O13) and hereby accepted as the known standing condition of this bundle. Orchestrator direct dispatch is the override path; the pair delegation in proceed-2 remains unexercised.
- Independence boundary is load-bearing acceptance criteria (R-s2-2/R-s2-3, DD §8): the checker implements game rules fresh in C3; no linked, generated, or translated game-logic code from the emitter, engine/src/core, or solver/; fixtures and the landed corpus are the only shared truth.
- Execution per the approved plan under Superpowers executing-plans: BRANCH theory/checker-c3 from BASE main@282f504; the gate battery (reject all 43 + MUT-R22 mutants for intended reasons, accept byte-pinned positives incl. the new P4 vector) is the plan's acceptance floor.
- Exit: branch + PR + implementation report parented to this dispatch id. No merge authority is granted here — merge rides its own MERGE-GATE relay after review/fold.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay; the orchestrator turn's batch commit carries the approving-review filing (see repo log)
FINAL_GIT_STATUS_SHORT: unavailable — dispatch authored mid-turn; the orchestrator turn report carries post-commit repo state
