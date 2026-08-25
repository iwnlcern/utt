ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: harness-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design phase; operator rulings R1-R4 already obtained and baked into the spec
GRILL_REQUIRED: yes
FROM: s1.orchestrator-planner
TO: harness.planner
CC: harness.implementer, s1.orchestrator-reviewer
SUBJECT: DESIGN dispatch — harness domain, cycle harness-c1

## Context

Reconciliation of the paired c1 audits is filed: RECONCILE.md section "c1-audits" (main@a3f250c).
Operator rulings R1-R4 are baked into the design spec (docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md, same commit) — they are fixed inputs, not design questions.
Your pair's audit returns in this cycle directory are the finding inventory; their dispositions are in the RECONCILE section.

## Scope — harness DESIGN

Design protocol spec v1 + game-log schema v1 + referee/runner architecture (Superpowers brainstorming owns the how). Fixed inputs: operator rulings R1 (tie_owner null at move 1, hidden coin, consulted only on an actual first-move tie), R2 (fault = opponent wins auction at no cost, opponent's move applied; double-fault = re-request up to 3 attempts; 3 double-faults = game killed/voided, excluded from stats), R3 (integer bids 0..budget inclusive).
Settle the merged question list from both audit returns: framing/flush/encoding/line grammar (board as nine 9-char strings, 0-based row-major recommended by your implementer), handshake-vs-pure-stateless (afternoon bar is the tiebreak), process lifecycle (persistent per game, restart between games), seat identity (you: "X"|"O", canonical marks in logs), clock semantics (monotonic, write-flush to complete-line, logged grace, warmup allowance question), legal-action enumeration (recommend yes), game-end notification, stderr-is-free-form, seeded determinism with seed in log header, paired seat+tie swaps.
Named consumer requirements from the ui audits (travel with this dispatch, resolve pre-lock): analysis carrier — the {bid, move} reply cannot feed the analysis view; choose optional engine info field, separate analysis request, or referee log enrichment, with value-quality metadata (exact/bound/estimate) from the engine; browser bridge transport (WebSocket or HTTP+SSE sidecar) with sealed lifecycle states, atomic human intent + idempotency, resync semantics; log schema sufficient for UI replay with zero re-execution (both sealed bids, both intents, resolution reason incl. tie/coin, payments, post-budgets, timing, terminal reason incl. R2 outcomes).
Acceptance: a third party can implement a client in an afternoon in any language — normative transcript, field table or JSON Schema, ~100-line stdlib-Python reference bot doubling as executable documentation, conformance fixtures for success plus every named failure. v1 scope gate: no SPRT, no network transport beyond the local bridge, no generic tournament platform.

## Process

GRILL_REQUIRED: yes — run the design grill before lock (new-feature still-open at medium tier; cross-domain boundary contracts; hard-to-reverse contract decisions).
Author the design doc at docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-<YYYYMMDD>.md with a DESIGN_DOC_ID.
Address your design-review request TO harness.implementer (orchestrator on CC only); on DESIGN_REVIEW_VERDICT: approve, report design completion to s1.orchestrator-planner and await PROCEED-TO-PLAN.
Boundary-contract changes route through s1.orchestrator-planner.

## Not in scope

Implementation, scaffolding beyond the design doc, plan authoring, or any relaxation of the theory/harness gates named above.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time between commits; no work actions claimed by this relay
