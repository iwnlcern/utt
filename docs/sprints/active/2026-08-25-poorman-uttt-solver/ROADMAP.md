# Sprint s1 — Poorman Auction-Bidding UTTT Solver

RUN_ID: s1
Sprint doc root: docs/sprints/active/2026-08-25-poorman-uttt-solver/
Design spec: docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md (approved by operator 2026-08-25)
Relay substrate: `.relays/s1/` under this sprint root; **tracked in git** (operator may flip to gitignored; record any change here).

**Daemon cutover (operator-directed, 2026-08-25):** the relay engine daemon governs this root (epoch active; 90 hand relays imported as `hand-authored-import`; seats registered with keys under `.engine/seats/`).
Seats file via `relay submit` drafts under `.engine/drafts/<seat>/`; hand INDEX appends are retired.
Root health instruments: `relay status` / `relay verify` / `relay lint --relay-root`; the standalone `relay-lint --index` monotonic check no longer applies to the daemon-rendered INDEX.
The engine README's "not adopted" status line is ruled stale for kit 2.9.1 by the operator.
Known permanent records: one historical `divergence` event marking the pre-cutover hand INDEX (archived), and non-monotonic time cells across the alphabetical import batch in the rendered INDEX.
The `.engine/` ledger is machine-local (self-gitignored); the daemon must be running for the root to operate — restart with `relay daemon start --root <root> --run-id s1 --seat s1.orchestrator-planner`.

## Teams

| Pair | Domain | Surfaces |
|------|--------|----------|
| theory | Literature review (gates engine math lock); exact Stage-1 3×3 auction-TTT solver (Python, independent implementation); canonical rules fixtures | `theory/` |
| engine | C++26 rules core, threshold search, evaluator, protocol I/O | `engine/` |
| harness | Interop protocol spec (owner), referee, baseline bots, tournament runner (Python) | `referee/`, `docs/protocol/` |
| ui | Vite/React web UI: replay + analysis-from-logs (play-vs-engine deferred post-s1 per operator ruling R5 in GRILL-harness-c1) | `ui/` |

Orchestrator Reviewer seat: `s1.orchestrator-reviewer`.

## Gates and sequencing

1. theory lit review + Stage-1 exact solver gate the engine pair's search-math design lock.
2. harness protocol lock gates ui work and friend-engine interop.
3. Shared contracts: protocol schema (harness-owned), rules fixtures (theory-owned).
   All contract changes route through the orchestrator.
4. ML distillation is out of scope for s1; later cycle only if engine strength disappoints (≤ $10 GPU).

## Constraints

- ≤ 30 s/move on a laptop, referee-enforced.
- Target $0 GPU spend in s1.
