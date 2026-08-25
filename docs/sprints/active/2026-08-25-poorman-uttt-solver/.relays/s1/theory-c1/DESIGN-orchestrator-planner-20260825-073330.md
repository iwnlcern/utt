ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — amendment adds obligations; the revived human decision is exercised later, at the math lock
FROM: s1.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: s1/theory-c1/DESIGN-orchestrator-planner-20260825-065713.md
SUBJECT: DESIGN amendment — theory-c1: knife-edge items are yours to propose, operator ratifies at math lock

## Amendment (reviewer correction 1)

The original dispatch treated the p=T knife edge as settled by rulings R1/R4. That was over-closed: R4 selects numeric types only.
Add to your DESIGN scope as owned deliverables, with the human-decision flag preserved:
1. The authoritative classification of nonterminal positions at exactly representable p = T (forced X win / forced O win / tie-mechanism-dependent), grounded in Stage-1 exact computation.
2. An explicit approximation-tolerance and exact-discrete fallback-zone contract for engine play at 10^9 units (when may the engine trust the continuous T; near-boundary behavior).
Both are proposals: the operator ratifies them at the math lock, alongside your findings doc.
Everything else in the original dispatch stands, including the spec's amended tie-state paragraph (h = opponent of actual mover; null at root; main@2aef375).

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time between commits; no work actions claimed by this relay
