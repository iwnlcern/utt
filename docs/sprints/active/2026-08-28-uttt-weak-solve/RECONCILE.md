# Sprint s2 — Reconciliation Projection

RUN_ID: s2

Durable reconciliation projection for sprint s2 (weakly solving standard alternating UTTT).
PHASE: RECONCILE relays cite the exact section they append here, or state projection-pending.

## Sections

### R1 — paired-audit reconciliation, s2-theory-c1 + s2-solver-c1 (2026-08-28)

Returns reconciled (all four addressed to s2.orchestrator-planner, read against cited evidence):

1. theory.planner — s2-theory/AUDIT-pair-planner-20260828-002956.md
2. theory.implementer — s2-theory/AUDIT-pair-implementer-20260828-003801.md
3. solver.planner — s2-solver/AUDIT-pair-planner-20260828-005841.md
4. solver.implementer — s2-solver/AUDIT-pair-implementer-20260828-003733.md

#### Agreement (no disagreements requiring resolution were found)

- A1 Reject/narrow gates do not fire.
  No published solve of the rule-lock variant exists (both theory seats, E1 per checked source; arXiv:2006.02353 differs at first-move freedom, closure, routing-to-won, and termination, and its strategy provably breaks under won-means-closed).
  The s1 rules core expresses the rule-lock game exactly with zero surgery (both solver seats, E1 file:line, independently corroborated by theory.planner's own conformance pass — three independent passes, no mismatch).
- A2 AltSolver is an oracle, not the solver: keep verbatim as a shallow cross-check; rebuild the production solver (all four seats).
- A3 Scale: full-width enumeration walls at ~ply 10–12 (two independent E2 measurement sets; d6 memo 486,838 reproduced exactly by both solver seats; ply-8 census 26.9M distinct cross-checks the d8 memo 30.9M cumulative).
  Brute force is not a plan; the solve must be proof-search shaped, with D4 (measured ≈×8 by ply 7–8) and eventually theory-gated rule leaves.
- A4 GHI is structurally absent (marks only accumulate → leveled acyclic DAG; value is a pure function of (marks, forced, side)); residual issues are efficiency (PN double-counting) and certificate deduplication, not soundness.
- A5 Value conjecture: X win at low-medium confidence (0.55 / 0.62); the certificate semantics must stay three-valued and never hard-code X-win; an exact-draw result requires a dual proof shape.
- A6 New work lands in a new top-level `solver/` surface consuming the engine rules core as a library; no engine/ edits (s1 engine scope stays closed).
- A7 Certificate practice: streaming/chunked, hint-rich (checker never searches), content-addressed, solver-independent; checkers-2007-style partial verification is the anti-pattern; Qubic/Gasser/SAT-certificate practice is the model.

#### Disposition ledger (each finding mapped exactly once)

| # | Finding (source returns) | Disposition | Target artifact / gate | Evidence checked |
|---|---|---|---|---|
| O1 | Certificate schema + canonical state/D4 spec + exact-draw proof form + independence boundary is the load-bearing shared contract (1,2,3,4) | owned obligation | DD-theory-c2 design doc via DESIGN dispatch s2-theory-c2 (theory.planner) | all four returns, ROADMAP gates 1–2 |
| O2 | Independent checker (parser, fresh rules, D4, streaming, named-mutant suite) before accepting solver output (1,2) | owned obligation | theory post-design cycles (c3+), sequenced in DD-theory-c2 | returns 1 §5, 2 §5 |
| O3 | Strategy-rule language + machine-checked soundness framework; gates solver leaf closure (1,2) | owned obligation | theory post-design cycle; ROADMAP gate 2 unchanged | returns 1 §4, 2 §4 |
| O4 | Solver architecture spike (df-pn vs exact-WDL negamax), canonicalization scheme, proof-store/TT split, checkpoint model, solver/ build surface (3,4) | owned obligation | DD-solver-c2 design doc via DESIGN dispatch s2-solver-c2 (solver.planner) | returns 3 §4–5, 4 §4–5 |
| O5 | Alternating perft baseline + reachable/D4 layer census (theory R1 mitigation; solver both recommend) (1,3,4) | owned obligation | solver early implementation cycle, committed in DD-solver-c2 sequencing | returns 1 R1, 3 §3, 4 §3 |
| O6 | Value-conjecture memo, updated from solver probe data (1,2) | owned obligation | theory continuous thread, format fixed in DD-theory-c2 | returns 1 §2, 2 §2 |
| O7 | ROADMAP misattributes arXiv:2006.02353 authors (1,2,4) | verified closure | ROADMAP.md corrected this reconcile (Bertholon, Géraud-Stewart, Kugelmann, Lenoir, Naccache) | arXiv listing quoted in returns 1 and 2 |
| O8 | Q1: canonical s2 state definition unpinned (no side-to-move field; auction TieState relic at root) (1,3,4) | verified closure by ruling | R-s2-1 below; normative encoding lands in DD-theory-c2 | position.hpp:20-31, game_model.hpp:21-27 cites in returns |
| O9 | Q2: checker-language question if certificate lands ≥1e9 nodes (1) | verified closure by ruling | R-s2-2 below | return 1 §5 throughput analysis |
| O10 | Q3: checker reuse of referee rules.py vs fresh rules (1,2) | verified closure by ruling | R-s2-3 below | returns 1 Q3, 2 §5 independence list |
| O11 | Q5: fixtures schema v1 freeze question (1,2) | verified closure by ruling | R-s2-4 below | return 1 asset B, return 2 asset table |
| O12 | Solver pair-internal reconcile asks (rebuild-vs-extend, wall, surface) (3 §Questions) | verified closure | both solver returns agree on all three (A2, A3, A6) | cross-read of returns 3 and 4 |
| O13 | Escalation-scan trigger-present rows (cross-pair schema contract; checker/solver independence) (2,4) | owned obligation (standing) | no downgrade requested or granted; tier stays large; contract routes through orchestrator (ROADMAP gate 1) | scan blocks in returns 2 and 4 |
| O14 | 2swap diagrams are asserted-by-exhaustion, not verified — rule language must carry its own verifier (1,2) | owned obligation | folded into O3 acceptance criteria | transcript cites in returns 1 §4, 2 §1 |

Rejected findings: none.

#### Orchestrator rulings (consistent with operator rulings of 2026-08-28; operator may veto)

- R-s2-1 Canonical s2 state: identity = (nine X local masks, nine O local masks, forced ∈ {0..8, ANY}); side-to-move is derived from mark parity (X to move when counts are equal); root = empty board, forced=4, X to move; auction-era TieState and budgets are excluded from s2 state identity.
  The D4 canonicalization convention is defined normatively in DD-theory-c2 and binds both pairs.
- R-s2-2 Checker independence is defined by properties, not a named language: different pair, non-C++, no linked/generated/translated game-logic code shared with the emitter or engine/src/core.
  Language selection happens in DD-theory-c2 with a checker-throughput justification against the expected certificate size bracket.
- R-s2-3 The checker implements the game rules fresh; theory fixtures (v1 plus the s2 additive profile) are the shared conformance truth; referee/poorman_referee/rules.py may serve as a third cross-check only, never linked or copied.
- R-s2-4 Fixtures schema v1 is frozen; s2 needs (alternating profile, certificate fixtures) land additively.

Next lifecycle step: DESIGN dispatches s2-theory-c2 and s2-solver-c2 (GRILL_REQUIRED: yes on both — new-feature at large tier, cross-domain boundary contract, hard-to-reverse format/architecture decisions).

### R2 — theory-c2 design-completion reconciliation (2026-08-28)

Report: s2-theory/SITREP-pair-planner-20260828-112649.md (theory.planner).
Verified by the orchestrator against disk/git before sequencing:

- designs/DD-theory-c2-20260828.md sha256 = 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c (matches the relay's pin) at commit d692ac2 on main.
- Approving review s2-theory/DESIGN-REVIEW-pair-implementer-20260828-051431.md: DISPATCH_ID s2-theory-c2-design-review-4, DESIGN_REVIEW_VERDICT approve, DESIGN_DOC_ID DD-theory-c2-20260828, parent s2-theory-c2-design-3, FROM theory.implementer TO theory.planner — lineage valid for the gated design-doc PLAN.
- Grill ran per GRILL_REQUIRED; operator decisions G1–G8 (doc §10) are standing run rulings: dual boolean certificates; D4-only identity (color-swap out); partial certificates never deliverables; fully materialized certificate; XXH3-128 integrity-only digests; verdict manifest + N single-claim certificates; checker in C3 (newest stable, idiomatic).
- Boundary effect: the schema seam DD-solver-c2 consumes is review-approved and byte-stable; relayed to solver.planner via CC on the proceed relay.

Disposition: O1 (certificate schema + canonical/D4 spec) verified closed at design level; PROCEED-TO-PLAN issued (s2-theory-c2-proceed-2, superseding the lint-dirty proceed-1) with delegated conditional dispatch authority.

### R3 — solver-c2 design-completion reconciliation (2026-08-29)

Report: s2-solver/SITREP-pair-planner-20260829-021238.md (solver.planner).
Verified by the orchestrator against disk/relays before sequencing:

- designs/DD-solver-c2-20260828.md rev 6 sha256 = e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2 (matches the relay pin; was untracked — committed in this orchestrator batch so the PLAN can pin its source commit).
- Approving review s2-solver/DESIGN-REVIEW-pair-implementer-20260829-013446.md: DISPATCH_ID s2-solver-c2-design-review-6, DESIGN_REVIEW_VERDICT approve, DESIGN_DOC_ID and DESIGN_SHA256 match current bytes, parent s2-solver-c2 — lineage valid; five must-revise rounds folded forward.
- Grill lock GRILL-solver-c2-20260828: nine operator decisions of record (hand-rolled storage; D4-only; local-first scale seam; pluggable-kernel architecture with pre-registered ≥2×-or-B spike rule and no-decision default B; 3-day spike timebox; census ply 10 committed / 11 optioned; work-threshold durability; WIN_X-first with scheduling-only switch; single-threaded c3).
  Still operator-owned: cloud vendor/box sizing at deep-run commissioning; ply-12 census extension on theory request.
- Theory contract consumed at pinned digest 629801c4… — matches the byte-stable seam recorded in R2; obligation O5 (census, ply 10) is committed in doc §11 sequencing.

Disposition: O4 verified closed at design level (and O5 committed into c3 scope); PROCEED-TO-PLAN s2-solver-c2-proceed-1 issued with delegated conditional dispatch authority.
Observed in passing (not this section's subject): theory-c2 has advanced through gated PLAN (4 revisions), delegated IMPL (both seats), and REVIEW-FOLD relays — its merge-gate report is the next expected theory event.
