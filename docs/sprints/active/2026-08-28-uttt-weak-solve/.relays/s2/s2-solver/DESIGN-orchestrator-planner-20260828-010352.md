ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — design phase proceeds under standing lifecycle; the grill (required below) is where fresh operator decisions surface
GRILL_REQUIRED: yes
FROM: s2.orchestrator-planner
TO: solver.planner
CC: solver.implementer, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/AUDIT-pair-planner-20260828-005841.md
SUBJECT: DESIGN s2-solver-c2 — search architecture, canonicalization, proof store, checkpointing, solver/ surface

## Design dispatch

Reconciliation s2-reconcile-1 (RECONCILE.md section R1) closed your c1 audit with full pair agreement on rebuild-vs-extend, the ~ply-10–12 full-width wall, and the solver/ surface; obligations O4 and O5 land here.
Superpowers brainstorming owns the how; this dispatch locks the what.

### Scope — DD-solver-c2 must decide and lock

1. Search architecture: settle the A-vs-B spike plan (df-pn/PN2 vs exact-WDL negamax/null-window with first-winning-child cutoff), including the measurement protocol that decides (identical canonical rules layer, identical storage, same probe positions), the three-valued objective decomposition (boolean proofs "X ≥ draw" / "X wins"), and DAG-aware proof accounting (double-count mitigation). Hybrid rule-leaf closure (C) stays staged behind ROADMAP gate 2.
2. Canonicalization scheme per ruling R-s2-1 and theory's forthcoming D4 spec: transform tables, canonical-keying strategy for hashing (transform-then-hash vs symmetry-composed Zobrist), plug-in point (TT boundary only), color-swap position (align with theory's decision).
3. Proof store vs play-TT split: append-only proven-fact log as checkpoint + certificate source; working-store technology choice (packed records + external sort vs embedded KV); eviction legality rules (nothing proven is lost before logging); RAM/disk budgets tied to the measured 55–140 B/entry numbers.
4. Checkpoint/resume model: what binds a checkpoint (rules digest, canonicalizer version, objective, schema digest), crash-safety (WAL/snapshot/epochs), idempotent resume.
5. solver/ build surface: new top-level CMake project consuming a narrow engine rules-core target (per A6 — no engine/ edits); oracle harness wiring (AltSolver + theory fixtures as conformance gates, ROADMAP gate 3).
6. Certificate emission interface as a consumer of theory's DD-theory-c2 draft schema: define the emitter seam abstractly now (canonical node, chosen move, full reply set, transform annotations), bind bytes only after the theory schema locks; schema-seam questions route to the orchestrator, not pair-to-pair lock.
7. Early-cycle commitment (O5, serves theory's R1): alternating perft baseline, reachable/D4 layer census with raw/legal/reachable/terminal/transposed counts, and first proof-growth curves from sub-solves (e.g. one root orbit) — sequence these into the cycle plan.
8. Cloud/run shape: single big-RAM box first; sharding deferred with named trigger conditions.

### Constraints (binding)

- Rulings R-s2-1..4 (RECONCILE.md R1) are input; theory owns the certificate schema and the normative D4 convention — your design consumes them and flags conflicts to the orchestrator.
- No engine/ edits; s1 engine scope stays closed.
- Compute budget unlimited (operator ruling 2026-08-28), but every long-running design must be checkpointed and resumable; "more cloud" is never the answer to an unbounded artifact (audit A3/R2).

### Grill

GRILL_REQUIRED: yes — new-feature at large tier, cross-domain certificate contract, hard-to-reverse storage/architecture decisions, cloud spend shape.
Run the design-grill against the operator before DESIGN_LOCK_ID.

### Not in scope

No production implementation (spike/measurement code per your audit discipline is fine — throwaway, out of tree, commands recorded); no certificate byte-format decisions (theory-owned); no strategy-rule design.

### Deliverable and lineage

DD-solver-c2-20260828 under the sprint designs/ tree; PHASE: DESIGN relay carrying DESIGN_DOC_ID; design-review request addressed TO solver.implementer (orchestrator CC only); on DESIGN_REVIEW_VERDICT: approve, report completion to the orchestrator for PROCEED-TO-PLAN.

### Acceptance criteria

- The A-vs-B decision procedure is falsifiable: named metrics, named probe set, a decision rule written before the spike runs.
- Every persistent byte has an owner (proof store, TT, checkpoint, certificate seam) and a crash story.
- The census deliverable (item 7) has a defined output format theory can consume for proof-size estimation.
- A reviewer can trace each design decision to a c1 audit finding, a ruling, or a measured number.

Boundary contract: Writes: (eventually) certificate artifact per theory schema; this cycle writes DD-solver-c2 only. Reads: rule lock, rulings R-s2-1..4, theory draft schema when available. Target entity: DD-solver-c2 design doc. Downstream consumer: theory checker (via certificate), theory proof-size estimation (via census). Contract: theory-owned schema at the emission seam. Proof: design review approve + grill record. No-consumer action: not applicable — consumers are commissioned.

Operator-judgment items expected to surface in the grill: cloud box sizing/provider, color-swap use, spike budget/timebox, census depth targets.
