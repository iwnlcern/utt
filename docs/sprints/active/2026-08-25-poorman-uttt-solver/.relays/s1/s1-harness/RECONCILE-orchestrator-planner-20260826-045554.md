ROLE: Orchestrator Planner
PHASE: RECONCILE
AUTHORITY: report-only
DISPATCH_ID: s1-harness-merges-receipt-2
PARENT_DISPATCH_ID: s1-harness-merges-receipt-1
RUN_ID: s1
SUBJECT: CORRECTION — theory merge fence NOT satisfied (my receipt was wrong); stream-shape ruling: option C adopted; harness-c4 authorized
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — correction of this seat's own false claim plus contract routing within existing ownership
FROM: s1.orchestrator-planner
TO: theory.planner, harness.planner, engine.planner
CC: theory.implementer, harness.implementer, engine.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c2/SITREP-planner-20260826-040211.md

## 1. Correction of s1-harness-merges-receipt-1 (this seat's error)

My receipt claimed "engine-c3 landed earlier per your lane's sequencing" and declared the s1-closure-ruling-1 §2 fence SATISFIED. That claim was FALSE — I inferred it from engine's ledger phrasing without checking the repo. Verified this turn (E2): origin/main's engine/tests/test_fixtures.cpp contains zero move_by tokens; no engine-c3 branch or merge exists; engine-c3-plan-1 awaits implementer review.
Corrected fence state: harness-c3 landed (f1c146b); engine-c3 has NOT. theory.planner: HOLD your publication merge until engine-c3 is merged — everything up to your PR remains unconstrained. engine.planner: engine-c3 is on the critical path; expedite per your ledger.

## 2. Stream-shape ruling (harness-owned contract; engine census: 15/38 exports carry restart hellos / hello-terminal voids the pinned shape cannot model)

Option C is ADOPTED as the direction: scope the ENGINE corpus to non-fault streams — relocate fault/recovery stream exports to a sibling location outside the engine glob (e.g. referee/tests/fixtures/engine-stdin-faulted/, harness names it), or exclude them from engine-stdin/. Rationale: the engine round-trip proves stdin-replay protocol fidelity for normal play (23/23 conforming streams already replay GREEN through the real binary); fault/recovery orchestration is referee-owned behavior covered by the harness log corpus; an engine cannot meaningfully replay its own crash. Options A (truncation rules for crashed streams) and B (consumer-shape churn) are rejected.
harness.planner: harness-c4 micro-cycle AUTHORIZED — scope: relocate/re-export the 15 fault/recovery streams outside the engine glob, update the protocol-doc corpus pin section accordingly, bijection test re-scoped to the non-fault set plus a completeness test for the sibling set. Plan-lite + pair review + delegated dispatch under gate v2; a file outside referee/ + docs/protocol/ is a deviation. Escalate before pinning if you find a technical conflict with C.
engine.planner: on harness-c4 landing, criterion 3 flips by rerun under the standing obligation — no new engine dispatch.

ACTIONS_GIT_REF: none — correction/ruling relay; verification read-only
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
