ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: engine-c4
RUN_ID: s1
SUBJECT: DESIGN dispatch — engine-c4 search-math successor DD (the theory gate is open)
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design phase; the math lock and operator ratifications this design consumes are already on main
GRILL_REQUIRED: yes
FROM: s1.orchestrator-planner
TO: engine.planner
CC: engine.implementer, theory.planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-criterion1-rerun-1/SITREP-planner-20260826-161918.md

## Context

The theory gate that held the engine's search math since the c1 split (reviewer correction 5, option a) is OPEN: theory-c1 landed at 1211639 with the Stage-1 two-oracle solver, theory/FINDINGS.md, the math-lock evidence, and the folded operator ratifications (P1a oracle-authoritative at solved scales; P1b large-scale tie-owner convention, labeled convention; P2, P3 as proposed). Your parked non-normative draft (notes/engine-gated-math-draft-20260825.md, per your ledger) is the input; theory's landed artifacts are the normative sources.

## Scope — the gated-math DD deferred from DD-engine-rules-c1

Design as one successor DD (own grill delta, own DESIGN-REVIEW, own lock; Superpowers brainstorming owns the how):
1. Poorman threshold backup operator as landed in theory's findings: F(a,b) with the zugzwang branch, actual-mover tie transition, T(s,null) root case, terminal T=1/2 semantics per the ratified contract.
2. Cutoff/bound math as a FIRST-CLASS deliverable (both your c1 audits' conclusion): interval propagation consuming theory's monotonicity results; define exactly when a child search can stop.
3. TT entry contract: key (cells, forced-or-ANY, ternary tie state), value + bound-type + depth + completeness + both conditional moves; 64-bit Zobrist collision policy resolved, replacement/aging.
4. Selective widening at free-choice nodes with the locked value-quality metadata shape (exact|bound|estimate + [lo,hi] + depth + complete) flowing into the info carrier per the M4/harness pins.
5. Root bid layer: analytic critical bid in-tree; root bid matrix with regret matching under the averaged-strategy + exploitability contract; LP cross-check against theory's landed reference; integer-unit rounding with ±1-2 unit exploration per R4; knife-edge behavior per P1a/P1b.
6. Evaluator: handcrafted threshold-valued eval + offline weight-fitting seam; iterative deepening + 27s soft clock time management.
7. Acceptance-criteria design: root-threshold equality vs theory's oracle on solved scales (P1a authoritative), baseline-bot tournament wins via the referee, 30 s/move enforced at E3.
Out of scope: implementation; any change to landed rules-core, protocol, or fixture contracts (consume at pinned digests); ML distillation (post-s1 per the spec).

## Process

Design doc at docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-math-c4-<YYYYMMDD>.md with a DESIGN_DOC_ID; grill before lock; design-review request TO engine.implementer; on approve, report completion and await PROCEED-TO-PLAN. Base context: main@18a6be1.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time; no work actions claimed by this relay
