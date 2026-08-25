# Poorman Auction-Bidding Ultimate Tic-Tac-Toe Solver — Design

Date: 2026-08-25
Status: approved by operator (Jack) in brainstorming session; math lock gated on theory pair's literature review and Stage-1 exact solver.

## Overview

A competitive engine for ultimate tic-tac-toe played under continuous, simultaneous Poorman bidding, plus the surrounding tooling: an interop protocol so a friend's engine can play it, a referee/tournament harness, an analysis-capable web UI, and research artifacts validating the bidding math.

The engine is a CPU threshold-search engine (Stockfish-shaped, not AlphaZero): the value of a position is the critical fraction of the combined remaining budget a player needs to force a win, and a Poorman backup operator replaces negamax.
This removes the continuous bid dimension from the search-tree interior entirely.

Hard constraints: no more than $10 of GPU spend (target $0), and no more than 30 seconds per move on a laptop.

## Canonical rules

These override any variant found in reference material.

1. Board: 3×3 macro grid of 3×3 local boards.
2. Budgets: both players start with 1.0, represented as 10^9 fixed-point units.
   Bids are integers in units; this is effectively continuous for an at-most-81-move game while keeping bid equality exact.
3. Each turn both players submit sealed simultaneous bids plus an intended move.
   The higher bidder pays their bid to the bank (money is destroyed — Poorman), and must place their mark (won bids must result in a move).
4. Bid ties resolve in order:
   a. The player with more chips remaining wins the tie (compared at bid time, before payment).
   b. If chips are exactly equal, the player who did not move last wins the tie.
   c. On the first move with equal chips, a coin flip decides.
   The tie winner is the auction winner: they pay the tied bid and must move.
5. The first move is forced into the center local board.
6. A local board is closed when it is won or completely full.
   A mark in cell c of a local board forces the next move — whoever wins the next auction, including the same player — into local board c, unless board c is closed, in which case the next move may be any empty cell in any open board.
7. Completing macro three-in-a-row wins the game outright.
8. If all nine local boards close with no macro winner: the player with more chips remaining wins 1–0; if chips are exactly equal, the game is ½–½.
   There is no macro-mark-count tiebreak.
9. Degenerate case: if both budgets reach exactly 0, every auction is a 0–0 tie and the tie rules alternate the mover; play continues as ordinary alternating UTTT.

Because the chip leader wins drawn boards, leftover budget has terminal value.
This is not a textbook Poorman game, and published critical-ratio results must be checked against this variant rather than assumed.

## Threshold theory

State for search purposes: `(board cells, forced-board index or ANY, tie-relevant state)`, with budgets held separately.
Let `p = B_X / (B_X + B_O)`.
Define `T(s, h)` as the critical budget fraction for X: if `p > T` X can force a win, if `p < T` O can force a win.
Terminal values: `T = 0` (X macro win), `T = 1` (O macro win), `T = 1/2` (all boards closed, no macro winner — the chip comparison at `p ≷ 1/2` decides, exact equality is ½–½).

Backup operator (subject to validation): with `a = min over X moves of T(child, ·)` and `b = max over O moves of T(child, ·)`,

- if `a ≤ b`: `T = b / (1 - a + b)`, critical bid fraction `r = (b - a) / (1 - a + b)` of the combined budget;
- if `a > b` (zero-bid zugzwang): both players bid 0 and the tie rules force a mover.

### Open math question (blocks the engine math lock)

The reference derivation modeled the tie owner as a pure alternation flag.
Under our rules the 0–0 tie is won by the chip leader, who is then forced to move, so the zugzwang branch depends on the budget comparison — i.e., on `p` itself — with the last-mover flag mattering only on the knife-edge `p = 1/2`.
The zugzwang backup must be re-derived for this variant, and the transposition-table key must carry exactly the tie-relevant state the corrected operator needs.
The theory pair resolves this via literature review plus an exact solver, before the engine pair locks its search design.

## Component 1: theory (Python)

- Literature review: Richman/Poorman bidding games (Lazarus, Loeb, Propp, Ullman; Avni/Henzinger et al. on continuous and infinite-duration Poorman games), UTTT solving attempts, simultaneous-move game-tree search and regret matching.
  Deliverable: a cited findings doc that confirms or corrects the backup operator and zugzwang handling for this variant.
- Exact reference solver for ordinary 3×3 auction tic-tac-toe under these exact rules (Stage 1): full-state-space threshold computation, cross-checked against high-resolution discrete-budget brute force.
  This is an independent implementation, in a different language from the engine, so shared misconceptions can't self-confirm.
- Canonical rules fixtures: machine-readable positions with expected legal moves, closures, terminal outcomes, and (for small cases) expected thresholds and critical bids.
  Consumed by engine, harness, and UI tests.

## Component 2: engine (C++26, CPU-only)

Target `-std=c++2c`; on macOS prefer Homebrew LLVM since Apple Clang lags.
SIMD where profiling justifies it.

- Rules core: bitboards (9 × 9-bit local boards per player), precomputed win masks and per-local-board tables over all 3^9 configurations, move generation, closure and terminal detection.
- Search: iterative deepening under a ~27 s soft clock; transposition table keyed on cells + forced board + tie-relevant state; Poorman backup operator (as locked by theory); tactical move ordering (macro wins, local wins, blocks, forks, routing quality, free-choice avoidance); tactical extensions; selective widening at free-choice nodes; interval branch-and-bound as a later optimization.
- Evaluation: handcrafted evaluator producing an estimated threshold via `sigmoid(-H(s))` over macro/local tactical features and routing/tie-owner features; weights fitted against deeper-search thresholds.
- Bidding: analytic critical bid in the tree interior; at the root, a small bid-matrix (24–48 candidate bids including 0, the critical bid, ±neighbors in fixed-point units, fractions of stack, all-in) solved with regret matching for near-threshold and losing positions.
- Protocol I/O: the JSONL interface below.
- Deferred (later cycle, only if strength disappoints): small distilled neural evaluator trained by search distillation, ≤ $10 GPU, ideally laptop-only.

## Component 3: harness (Python) — owns the interop protocol

Protocol: JSONL over stdin/stdout, stateless per request.
Referee sends the full game state; engine replies with a sealed bid and its intended move if it wins.

```jsonl
→ {"protocol": 1, "board": [[".",...,9 chars],...9 locals], "forced": 4, "budgets": {"you": 1000000000, "opp": 1000000000}, "last_mover": null, "time_ms": 30000}
← {"bid": 63410000, "move": [4, 2]}
```

Exact schema is a versioned document owned by the harness pair; engine and UI are consumers; changes gate through the orchestrator.
The protocol must be implementable by the friend's engine in an afternoon, in any language.

- Referee: spawns two engine processes, collects sealed bids, resolves auctions and every tie rule including the coin flip, enforces the move-on-win rule and time limits, emits a replayable JSON game log.
- Baseline bots: random-legal/random-bid, always-zero bidder, fixed-fraction bidders, all-in tactical bidder.
- Tournament runner: paired games alternating seats and initial coin flip, W/D/L and average budget-margin stats, engine-version regression tracking.

## Component 4: ui (Vite + React + TypeScript)

- Play against the engine through the referee's local bridge.
- Replay game logs move by move.
- Analysis view: threshold `T`, actual share `p`, budget margin, critical bid, and both conditional best moves (if-X-wins / if-O-wins), chess-engine style.
- Layout ideas borrowed from github.com/ultimate-ttt/ultimate-ttt (2021 CRA app); no code reuse — its alternating-turn state machine doesn't fit bidding.

## Decomposition and sequencing

Four agent pairs under the orchestrator protocol, monorepo layout `theory/`, `engine/`, `referee/`, `ui/`, `docs/`.

| Pair | Domain | Gates |
|------|--------|-------|
| theory | Lit review, exact Stage-1 solver, rules fixtures | Lit review + Stage-1 gate the engine math lock |
| engine | C++ rules core, threshold search, eval, protocol I/O | Rules core may start against the canonical rules above; search math waits on theory |
| harness | Protocol spec, referee, baselines, tournament runner | Protocol lock gates UI and friend-engine interop |
| ui | Web UI | Starts after protocol lock, develops against logged fixtures |

Collision control: the protocol schema (harness-owned) and rules fixtures (theory-owned) are the two shared contracts; all other surfaces are disjoint.

## Verification strategy

- E1/E2: property tests of the rules core against theory's fixtures; engine-vs-reference-solver threshold equality on small boards; protocol schema round-trip tests.
- E2: Stage-1 continuous results vs discrete brute force; zugzwang and tie-rule edge cases as named test cases.
- E3: full tournament runs vs baselines and prior engine versions; 30 s/move enforcement measured by the referee on a laptop.
- Acceptance: engine beats all baseline bots decisively, respects time control, and its root thresholds match the exact solver on every position the exact solver can reach.

## Risks

- The zugzwang re-derivation could reshape the backup operator or TT key; that is why it gates the math lock rather than being patched later.
- Free-choice nodes (up to ~70 legal cells) may blow up search depth; selective widening is the planned mitigation.
- Exact bid ties are strategically reachable under fixed-point budgets; the referee and engine must agree on tie semantics to the unit.
