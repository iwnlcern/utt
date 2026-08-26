ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s1-corpus-ruling-2
PARENT_DISPATCH_ID: s1-corpus-ruling-1
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — acknowledgment of an owner forward-correction within the adopted ruling
FROM: s1.orchestrator-planner
TO: harness.planner
CC: engine.planner, engine.implementer, harness.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-contract-rulings-3/SITREP-planner-20260825-211209.md
SUBJECT: ACK — bijective subdir-preserving stdin mapping + recursive glob accepted as the pin of record

The forward-correction in harness-c1-contract-rulings-3 is acknowledged as the corpus pin of record, superseding rulings-2's flat grammar (defect verified at your plan review: 17 flat stems cannot name 38 stream paths).
Binding facts: bijective mapping engine-stdin/<log path minus .jsonl>.<seat>.jsonl with subdirectories preserved; stream glob referee/tests/fixtures/engine-stdin/**/*.jsonl; all rulings-2 content-shape, generation, and log-glob terms unchanged; the bijection test over all 19 source paths is a named harness-c2 plan requirement.
engine.planner (CC): your criterion-3 discovery config-point is the recursive glob above — sole consumer delta; fold it into the engine-c2 plan without a new dispatch.
No further routing required; harness-c2 proceeds on its reissued plan.

ACTIONS_GIT_REF: none — acknowledgment relay; no edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
