## Team theory — theory-c3: PROCEED TO PLAN

ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c3-proceed-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: merge remains the standing operator gate
FROM: s2.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/RECONCILE-orchestrator-planner-20260829-044814.md
PARENT_DISPATCH_ID: s2-theory-c2
APPROVED_DESIGN_DOC_ID: DD-theory-c2-20260828
APPROVING_DESIGN_REVIEW_DISPATCH_ID: s2-theory-c2-design-review-4
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@7c254bf
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision.
SUBJECT: PROCEED-TO-PLAN — theory-c3 independent checker (c3 slice of DD-theory-c2)

## Sequencing

theory-c2 closed at merge 4df0bbe (RECONCILE.md R4); the locked corpus your checker consumes is on main.
The governing design remains DD-theory-c2-20260828 (approved by s2-theory-c2-design-review-4; grill decisions G1–G8 standing) — its §8 independence boundary, §9 gate/mutant tests, and §11 c3 sequencing already lock the checker's what; no new DESIGN phase is required for this bundle.
No design lock rides this sequencing relay; your gated PLAN carries it.

Proceed to the gated design-doc PLAN for theory-c3:

- You author the PHASE: PLAN relay FROM theory.planner with DESIGN_LOCK_ID: DD-theory-c2-20260828, DESIGN_RECORD_KIND: design-doc, and PARENT_DISPATCH_ID: s2-theory-c2-design-review-4.
- Bundle scope = DD-theory-c2 §11 c3 as approved: the independent checker in C3 (grill decision of record; newest stable release, idiomatic style), implementing the game rules fresh per ruling R-s2-3 (no linked/generated/translated game-logic code shared with the emitter or engine/src/core, per R-s2-2); parser, canonical state + D4 identity, streaming/chunk verification, and the full gate battery — the checker must reject all 43 landed mutants for their intended reasons and accept the byte-pinned positive vectors.
- Fold the carried theory-c2 backlog into this plan's scope (RECONCILE.md R4): multi-reply OPPONENT positive P4 + dropped-reply mutant; catalogue count pin; R17 reason wording.
- Census availability (boundary note, CC solver.planner): solver-census-v1 ply-10 CSV + xxh128 sidecar is committed on branch solver/c3-proof-engine @ 62c8c83 and lands on main with the solver-c3 merge; theory may consume it read-only from the branch for proof-size estimation (obligation O6), citing the branch sha — do not copy it into theory surfaces.
- Plan review is the plan gate: request PLAN-REVIEW from theory.implementer; on approve, the delegated conditional dispatch authority above applies — run the SCOPE_DIFF mechanically before issuing the token; any OUT row or hard trigger re-engages this seat (the standing cross-pair contract trigger routes here exactly as it did for solver-c3).
- Merge remains a separate operator/orchestrator gate; IMPL exits with branch/PR/report.

ACTIONS_GIT_REF: none — sequencing relay; committed by the orchestrator turn (see repo log)
FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay; the orchestrator turn report carries post-commit repo state
