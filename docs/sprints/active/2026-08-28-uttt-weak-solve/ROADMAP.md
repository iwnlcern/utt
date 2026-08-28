# Sprint s2 — Weakly Solving Standard Alternating UTTT

RUN_ID: s2
Sprint doc root: docs/sprints/active/2026-08-28-uttt-weak-solve/
Relay substrate: `.relays/s2/` under this sprint root; **tracked in git** (same selection as s1; operator may flip — record any change here).
Daemon governance: this root is engine-managed from birth (kit 2.9.2 relay daemon, per the operator-directed s1 cutover convention).
Non-standard directory declaration: `notes/` holds method reference material (2swap Connect-4 transcripts supplied by the operator 2026-08-28).

## Objective

Weakly solve the standard alternating-turn ultimate tic-tac-toe game exactly as this repo defines it, and ship a **verified certificate**: a proof/strategy artifact that an independent checker — different language, different pair — verifies end to end.
Operator rulings (2026-08-28): rule set is the repo rule-9 game verbatim; evidence bar is the verified certificate; compute budget is unlimited (laptop + cloud as needed).

## Target game (s2 rule lock)

Derived from the canonical rules in docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md (rules 1, 5, 6, 7, 8, 9), with bidding stripped per rule 9's degenerate case:

1. 3×3 macro grid of 3×3 local boards. Players X and O alternate; X moves first.
2. The first move is forced into the center local board (any of its 9 cells).
3. A mark in cell c of a local board forces the next move into local board c, unless board c is closed, in which case the next move may be any empty cell in any open board.
4. A local board is closed when it is won or completely full. Cells of a closed board are dead (no further play there).
5. Completing macro three-in-a-row of won local boards wins the game outright.
6. If all nine local boards close with no macro winner, the game is a draw (the auction game's equal-chips ½–½ at zero budgets).

Any ambiguity discovered against the s1 engine/theory rules implementations is an orchestrator escalation, not a pair-local decision.

## Method inspiration (2swap's Connect 4 weak solve)

Transcripts: notes/2swap-connect4-transcripts/.
Transferable ideas the pairs must evaluate rather than assume:
- Weak solution DAG: one chosen move at our-to-move nodes, all opponent replies; exploit choice-freedom among winning/drawing moves to pick the most compressible tree.
- Rule-based leaf closure ("steady state diagrams" / claimeven / parity-zugzwang threat calculus): a provable positional language that closes entire subtrees without search; leaves of the opening DAG carry verified strategy rules instead of subtrees.
- Symmetry fusing (UTTT: D4 board symmetry, preserved by the center-forced first move) into a canonicalized DAG.
- The certificate is the deliverable, not the solver run.

## Teams

| Pair | Domain | Surfaces |
|------|--------|----------|
| theory | Prior art (Bertholon/Gehanne/Grimaud 2020 UTTT solve and rule-variant delta; Allis-style knowledge methods), value conjecture, UTTT strategy-rule language + soundness proofs, **certificate schema (owner)**, independent certificate checker (Python or other non-C++) | `theory/` (s2 additions), certificate schema doc |
| solver | C++26 solving engine: search architecture (DFPN/PN/retrograde — design phase decides), symmetry reduction, at-scale transposition/memo storage, proof-DAG emission, checkpoint/resume, long-run + cloud execution | `solver/` (or engine/ extension — audit decides), reuse of engine/src/core rules kernels |

Orchestrator Reviewer seat: `s2.orchestrator-reviewer`.
Deferred pair: viz (2swap-style solution-DAG explorer, likely extending `ui/`) — boots after the certificate schema locks.

## Gates and sequencing

1. Certificate schema (theory-owned) is the shared contract; solver emits it, checker verifies it; changes route through the orchestrator.
2. Theory's strategy-rule soundness proofs gate any solver use of rule-based leaf closure; until then solver leaves close only on terminals.
3. Solver rules-core reuse and brute-force subgame solving may start immediately after its audit/design cycle; it must bit-match the s1 rules core and theory fixtures where applicable.
4. s1 leftovers (theory-c2 P1c sweep, MR17 ack) stay parked in s1; they do not fold into s2.

## Constraints

- Compute budget unlimited (operator ruling 2026-08-28); prefer checkpointed, resumable runs.
- Certificate must be independently verifiable end to end by machine; human-legibility of the compressed DAG is a stretch goal, not the bar.
