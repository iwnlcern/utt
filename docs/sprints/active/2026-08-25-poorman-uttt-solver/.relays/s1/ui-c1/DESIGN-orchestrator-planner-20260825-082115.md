ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: ui-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design phase; the ui-specific UX operator questions are asked by you at grill time
GRILL_REQUIRED: yes
FROM: s1.orchestrator-planner
TO: ui.planner
CC: ui.implementer, s1.orchestrator-reviewer
SUBJECT: DESIGN dispatch — ui domain, cycle ui-c1 (protocol lock satisfied; scope narrowed per R5)

## Context

The harness protocol/game-log design is locked: DD-harness-c1-20260825 @ sha256 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440, approved by harness-c1-design-review-3.
Operator ruling R5 (grill GRILL-harness-c1): the browser bridge is deferred from v1 — s1 ui scope is REPLAY + ANALYSIS-FROM-LOGS only; play-vs-engine is a later additive supplement (recorded in the spec and ROADMAP at main@26a1630).
Your pair's audit returns and their dispositions (RECONCILE.md section c1-audits) are the finding inventory; UI-A02/A04 analysis-carrier needs were resolved by the harness lock (optional info object in engine replies, referee-logged verbatim).

## Scope — ui DESIGN

Design (Superpowers brainstorming owns the how):
1. Replay view: import/open versioned game logs per the locked harness log schema; auction-oriented timeline (both revealed bids, both intents, resolution reason incl. tie/coin and R2 fault outcomes, payments, post-budgets, forced board); keyboard navigation; board rendering with closure/forced affordances.
2. Analysis view: per-turn metrics from logged info payloads — threshold T, share p, signed margin, critical bid (fraction and units), both conditional best moves with distinct affordances, value-quality metadata (exact/bound/estimate) surfaced honestly; per-game T/p chart.
3. App shell + fixture-driven test seam: consume theory fixture conventions (forced integer-or-null, canonical coordinates) and harness conformance/log fixtures; integer math end-to-end in TS for budgets (BigInt or ints < 2^53).
4. Layout inspiration only from ultimate-ttt@0c05c6f per your audits; no code reuse.
Out of scope: play-vs-engine, bridge client, sealed-bid entry UX (deferred with R5); any protocol/log schema change (harness-owned; route through this seat).

## Process

GRILL_REQUIRED: yes — run the design grill before lock; batch your audit's operator UX items there (reveal policy for losing intents, budget display units, conditional-move rendering, metric perspective/precision, replay acquisition/persistence; the live-analysis and human-clock questions are moot under R5).
Author the design doc at docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-<YYYYMMDD>.md with a DESIGN_DOC_ID; design-review request TO ui.implementer; on approve, report design completion to s1.orchestrator-planner and await PROCEED-TO-PLAN.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time; no work actions claimed by this relay
