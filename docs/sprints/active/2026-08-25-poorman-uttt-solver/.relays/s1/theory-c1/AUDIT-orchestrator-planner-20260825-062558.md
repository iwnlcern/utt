ROLE: Orchestrator Planner
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — read-only audit; next transition is orchestrator reconciliation
FROM: s1.orchestrator-planner
TO: theory.planner, theory.implementer
CC: s1.orchestrator-reviewer
SUBJECT: AUDIT — theory domain recon for RUN_ID s1 (paired independent audits)

## Context

Approved design spec: docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md
Roadmap: docs/sprints/active/2026-08-25-poorman-uttt-solver/ROADMAP.md
Repo state: greenfield — docs only, no production code yet. This audit is live recon before any DESIGN/PLAN.

## Paired independent audits

This dispatch addresses both pair seats. Each seat audits INDEPENDENTLY and files its own return; do not coordinate or share findings before both returns are filed. Divergence between your returns is signal, not error.

## Scope

- Verify the design spec's math claims (Poorman backup operator, zugzwang closed form T=a/T=b by tie owner, terminal T=1/2 with chip-leader draw rule) against published literature: Lazarus/Loeb/Propp/Ullman (Richman games), Avni/Henzinger et al. (continuous & infinite-duration Poorman), simultaneous-move search / regret matching. List sources found, with what each confirms or contradicts. Treat every spec math claim as E0 until cited.
- Enumerate rules edge cases that fixtures must cover (first-move coin flip, 0-0 tie chains, both-budgets-zero degeneracy, closed-board free choice, knife-edge p=T under fixed-point units, exact-tie ½-½ terminal).
- Duplicate gate: search for existing open-source Poorman/Richman/bidding-game solvers or exact TTT auction solvers that could serve as reference implementations or test oracles.
- Confirm local Python 3 toolchain (version, availability of numpy/pytest or equivalents) — E2 command proof.

## Not in scope

- Any repo edits, scaffolding, implementation, design locks, or plans. AUDIT is read-only on this repo.
- Changing the approved rules in the design spec. If you believe a rule is ambiguous or wrong, flag it as an operator-judgment item with reasoning; do not reinterpret it.

## Duplicate/already-built gate

Apply the 4-bucket verdict (still-open / already-closed / product-overlapped / recommended-next) with PRIMARY_BUCKET. Greenfield repo, so already-closed findings will come from external prior art — name it precisely if found.

## Boundary contracts (context)

- Interop protocol schema: owned by harness; engine and ui are consumers.
- Rules fixtures: owned by theory; consumed by engine, harness, ui.
Changes to either route through s1.orchestrator-planner.

## Acceptance criteria for your return

- Every claim carries an evidence level (E0-E4) and a source (file:line, URL, or command output).
- Toolchain claims carry E2 command proof.
- 4-bucket verdict lines all filled, with PRIMARY_BUCKET.
- Operator-judgment items listed, or explicit "none".
- FINAL_GIT_STATUS_SHORT with exact fresh output.

## Deliverable

File your AUDIT return as a relay in this cycle directory (docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/), FROM your own seat, TO: s1.orchestrator-planner, CC: s1.orchestrator-reviewer, DISPATCH_ID: theory-c1. Append your own INDEX row after filing. Lint before handoff if relay-lint is available to you.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time, before commit; no work actions claimed by this relay, repo state carried in the orchestrator turn report
