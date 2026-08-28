ROLE: Orchestrator Planner
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: s2-theory-c1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — read-only paired audit; no state transition requires a fresh operator decision
FROM: s2.orchestrator-planner
TO: theory.planner, theory.implementer
CC: s2.orchestrator-reviewer
SUBJECT: AUDIT s2-theory-c1 — solvability landscape, prior art, certificate design space

## Audit dispatch — paired independent audits

Both addressees audit independently and file separate returns; do not coordinate conclusions before filing.

### Context

s2 objective: weakly solve the standard alternating UTTT game exactly as locked in ROADMAP.md ("Target game (s2 rule lock)": center-forced first move, closed = won-or-full, sent-to-closed → any open cell, macro 3-in-row wins, all-closed = draw, X first).
Deliverable bar: a certificate an independent checker (your pair, non-C++) verifies end to end.
Method inspiration: 2swap's Connect 4 weak solve (notes/2swap-connect4-transcripts/) — weak-solution DAG, rule-based leaf closure, symmetry fusing, certificate-as-deliverable.

### Scope — answer with citations (paper, or file:line)

1. Prior art delta: Bertholon/Gehanne/Grimaud 2020 ("At most 43 moves, at least 29", arXiv:2006.02353) — their exact rule variant vs our rule lock, item by item (first-move freedom, closure rule, play-in-won-board rule, draw rule). Does their first-player-win result or any strategy structure transfer? Also: Allis's VICTOR (rule-based Connect 4 solve — the direct ancestor of 2swap's approach), proof-number search family (PN/PN2/DFPN), and certificate/verification practice from large solves (checkers 2007; qubic; nine-men's-morris).
2. Value conjecture for the rule-lock game, with reasoning: X win / draw / unknown, and how the center-forced first move changes the picture vs free-opening UTTT.
3. State-space and proof-size estimates: reachable-position bounds under the rule lock, symmetry factor (D4), plausible weak-solution DAG size brackets; is pure brute force plausible at unlimited budget, and what does the answer imply for how much rule-based compression we need?
4. Strategy-rule language design space: what are candidate UTTT analogues of claimeven/parity/zugzwang (local-board parity, routing control, dead-board arithmetic)? What soundness-proof obligations would each carry? This is landscape, not design.
5. Certificate design space: proof-DAG formats (AND-OR tree with one move at max nodes, all moves at min nodes), rule-closed leaves, checker independence requirements, incremental/streaming verification for very large certificates.
6. Duplicate gate (4-bucket, mandatory): what already exists — theory/auction_ttt (Stage-1 solver, rules.py), theory/fixtures schema v1, engine/src/root/alt_solver.hpp (memoized alternation solver from s1 G6), engine core rules kernels. For each: already-closed / still-open / product-overlapped / recommended-next.

### Not in scope

- No production code, no design lock, no certificate schema draft, no plan. Web research is allowed and expected; local commands only for reading/verifying repo state.
- s1 parked items (theory-c2 P1c sweep, MR17 ack) are out of scope for this audit.

### Boundary contract

Not applicable at audit time. Downstream: your pair will own the certificate schema (writes: schema spec; readers: solver pair emitter, your checker).

### Deliverable

One AUDIT return relay per seat via daemon submit, carrying: findings with 4-bucket verdicts and PRIMARY_BUCKET per scope item; explicit value conjecture with confidence; recommended cycle decomposition for the theory pair; open operator-judgment items or `none`; FINAL_GIT_STATUS_SHORT.

### Acceptance criteria

- Every scope item answered with E1 citations (paper reference or file:line) or explicitly marked unknown with what would resolve it.
- Duplicate gate covers all four named existing assets.
- Rule-variant delta vs Bertholon et al. is itemized against the ROADMAP rule lock, not summarized.

Reject/narrow gate: if you find the rule-lock game is already solved in published literature under byte-identical rules, stop and report that immediately — the sprint reshapes around verification instead of search.
