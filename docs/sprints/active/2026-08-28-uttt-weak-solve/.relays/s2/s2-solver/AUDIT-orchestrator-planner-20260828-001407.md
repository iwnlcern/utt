ROLE: Orchestrator Planner
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: s2-solver-c1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — read-only paired audit; no state transition requires a fresh operator decision
FROM: s2.orchestrator-planner
TO: solver.planner, solver.implementer
CC: s2.orchestrator-reviewer
SUBJECT: AUDIT s2-solver-c1 — engine reuse surfaces, search architecture, scale feasibility

## Audit dispatch — paired independent audits

Both addressees audit independently and file separate returns; do not coordinate conclusions before filing.

### Context

s2 objective: weakly solve the standard alternating UTTT game exactly as locked in ROADMAP.md ("Target game (s2 rule lock)").
Your pair will build the C++26 solving engine that emits a certificate against the theory-owned schema; an independent non-C++ checker verifies it.
Compute budget unlimited (operator ruling 2026-08-28); prefer checkpointed, resumable runs.
Method inspiration: 2swap's Connect 4 weak solve (notes/2swap-connect4-transcripts/).

### Scope — answer with citations (file:line) and measurements where cheap

1. Reuse audit of engine/: core rules kernels (bitboards, 3^9 local tables, closure/terminal, move gen) — do they implement the rule-lock game exactly when budgets are stripped (X first, center-forced first move, sent-to-closed → any open cell, all-closed draw)? Name any auction-specific coupling that would need factoring out. Cite file:line.
2. AltSolver deep-dive (engine/src/root/alt_solver.hpp, tests/test_alt_solver.cpp): capabilities and limits for a full weak solve — memo growth and eviction, value type (double — is it exact-safe for win/draw/loss?), no persistence, no proof emission, depth-limited design. 4-bucket verdict: promote/extend vs rebuild.
3. Scale feasibility (E2 encouraged): run cheap measurements with the existing solver/rules core — nodes/sec, memo bytes/node, depth-limited node counts from the center-forced root — and extrapolate: what fraction of the reachable space is brute-forceable in RAM, with disk-backed tables, with a big cloud box? Where is the wall?
4. Search architecture options for a weak solve with certificate emission: DFPN/PN2 vs alpha-beta with proof logging vs retrograde endgame tables vs hybrid; TT-and-proof interaction (GHI problem — does UTTT's monotone fill (every move adds a mark, no repetition) eliminate graph-history issues? state it precisely); D4 symmetry canonicalization cost and where it plugs in.
5. Certificate emission and operations: what AltSolver lacks to emit a proof DAG; checkpoint/resume design space for multi-day runs; disk formats for TB-scale tables; cloud run shape (single big box vs sharded frontier).
6. Duplicate gate (4-bucket, mandatory) over: alt_solver.hpp, engine TT/zobrist, engine build/test infra, theory/fixtures as conformance anchors.

### Not in scope

- No production code beyond throwaway measurement harnesses (report exact commands + output; leave the tree clean), no design lock, no plan.
- No changes to engine/ — s1 engine pair scope is closed; s2 solver work lands in its own surface (recommend one in your return: solver/ vs engine/ extension).

### Boundary contract

Not applicable at audit time. Downstream: you write the certificate (consumer: theory's checker) against the theory-owned schema.

### Deliverable

One AUDIT return relay per seat via daemon submit, carrying: findings with 4-bucket verdicts and PRIMARY_BUCKET per scope item; measured numbers with exact commands; a recommended search architecture shortlist (not a decision); recommended cycle decomposition for the solver pair; open operator-judgment items or `none`; FINAL_GIT_STATUS_SHORT.

### Acceptance criteria

- Every scope item answered with E1 file:line or E2 command+output evidence, or explicitly marked unknown with what would resolve it.
- The rules-conformance question (item 1) is answered against the ROADMAP rule lock item by item, not summarized.
- Scale numbers distinguish measured from extrapolated.

Reject/narrow gate: if the rules core cannot express the rule-lock game without deep surgery, report that as a blocking finding with the minimal factoring you'd propose — do not start the factoring.
