ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design phase; operator rulings R1-R4 already obtained and baked into the spec
GRILL_REQUIRED: yes
FROM: s1.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s1.orchestrator-reviewer
SUBJECT: DESIGN dispatch — theory domain, cycle theory-c1

## Context

Reconciliation of the paired c1 audits is filed: RECONCILE.md section "c1-audits" (main@a3f250c).
Operator rulings R1-R4 are baked into the design spec (docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md, same commit) — they are fixed inputs, not design questions.
Your pair's audit returns in this cycle directory are the finding inventory; their dispositions are in the RECONCILE section.

## Scope — theory DESIGN

Design, as one coherent package (Superpowers brainstorming owns the how):
1. Findings doc skeleton: the five audited claims — cite what is published (poorman recurrence, existence/uniqueness for token games), prove what is not (critical-bid lemma, partisan extension, zugzwang branch with alternation tie owner, chip-leader terminal), plus the new T(s, null) first-move root case under R1 and the h-transition rule (h' = opponent of actual mover, NOT an unconditional toggle).
2. Stage-1 two-oracle architecture: exact-rational continuous DP (fractions.Fraction, backward induction on the finite DAG, flag any non-forced state) + exact integer-budget discrete brute force at several chip scales; both implement rules R1-R3 exactly; monotonicity statement for F(a,b) as a deliverable the engine's cutoff math consumes; LP reference solver for root bid matrices (regret matching stays engine-side, with averaged-strategy + exploitability reporting contract).
3. Rules-fixture schema v1 (versioned from day one): union of both audit edge-case lists (planner items 1-15; implementer additions: h-persistence under consecutive paid wins, knife-edge cases at several total-budget scales with separate continuous and exact-discrete expected fields, negative contract fixtures routed to harness post-protocol-lock). You own coordinate/forced/ANY conventions; harness and ui consume them verbatim — publish the schema section of the design early, engine is waiting on it.
Cross-check oracles (reference-only, no code reuse): dm0n3y/biddingTTT and mindsortlabs (Richman mode adapters), Kalai 101.84 reproduction target, OpenSpiel UTTT for alternating rules.

## Process

GRILL_REQUIRED: yes — run the design grill before lock (new-feature still-open at medium tier; cross-domain boundary contracts; hard-to-reverse contract decisions).
Author the design doc at docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-<YYYYMMDD>.md with a DESIGN_DOC_ID.
Address your design-review request TO theory.implementer (orchestrator on CC only); on DESIGN_REVIEW_VERDICT: approve, report design completion to s1.orchestrator-planner and await PROCEED-TO-PLAN.
Boundary-contract changes route through s1.orchestrator-planner.

## Not in scope

Implementation, scaffolding beyond the design doc, plan authoring, or any relaxation of the theory/harness gates named above.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time between commits; no work actions claimed by this relay
