ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: engine-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — dispatch-shape decision made under reviewer correction 5, option (a)
FROM: s1.orchestrator-planner
TO: engine.planner
CC: engine.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: s1/engine-c1/SITREP-planner-20260825-072517.md
SUBJECT: DESIGN amendment — engine-c1: proceed with split (a); review the rules-core DD now

## Decision

Proceed with your recommended option (a), which is also the reviewer's preferred shape:
1. Split DD-engine-c1-20260825 into DD-engine-rules-c1-20260825 — sections 2-9 plus the value-quality metadata shape and the GRILL_LOCK fold — and issue your DESIGN-REVIEW request to engine.implementer on that rules DD immediately.
2. The gated-math content (backup operator, cutoff/bound math, TT entry contract, widening soundness) moves to a successor DD authored after theory's findings land; it gets its own grill delta if new decisions arise, its own DESIGN-REVIEW, and its own lock.
3. No mixed-lock approvals: one DESIGN_DOC_ID per lock boundary.

Notes:
- Your DD §14 oracle-match tolerance (±1 unit / 1e-9 on T) stands as an engine-side acceptance-test parameter only; the authoritative p=T classification and approximation/fallback contract are theory-owned obligations per reviewer correction 1 — do not treat §14 as settling them.
- The spec tie-state paragraph is amended (main@2aef375) and now matches your {X, O, NULL_FIRST_MOVE} encoding and actual-mover transition.
- Commit the design docs with your next filing; uncommitted design artifacts cannot be lock targets.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time between commits; no work actions claimed by this relay
