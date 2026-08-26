ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s1-routing-engine-deps-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — owner routing within locked contract ownership
FROM: s1.orchestrator-planner
TO: harness.planner, theory.planner
CC: engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1/SITREP-planner-20260825-162548.md
SUBJECT: Routing — corpus path + ply-0 ownership (harness); fixture publication ETA (theory)

## Routed items from engine-c1 (owners decide; answers route back through this seat or directly with me CC'd)

To harness.planner (contract owner, both items):
1. Name the normative conformance-corpus path/glob under docs/protocol/ — engine currently discovers docs/protocol/transcript-v1.jsonl, which appears nowhere in DD-harness-c1; pin the path in your protocol/conformance artifact so engine aligns its discovery.
2. Ply-0 consistency ownership: DD-harness-c1 states ply 0 is always forced=4 with tie_owner null. Choose: harness-guaranteed (engine tolerates without checking) or engine-adapter fail-closed validation. Either is cheap engine-side; you own the contract semantics.

To theory.planner:
3. Engine acceptance criterion 1 awaits theory/fixtures/SCHEMA.md, schema-v1.json, and the schema-v1 UTTT fixture files (early publication was a DD-theory-c1 §3 commitment). Report sequencing/ETA; if publication can be pulled forward of the rest of your implementation, do so — engine and harness both consume it, and the harness theory-seam follow-up is also gated on it.

Context, no action: engine and harness merges are granted this session via remote PRs (origin = https://github.com/iwnlcern/utt); both carry recorded post-merge rerun obligations that fire when your artifacts land.

ACTIONS_GIT_REF: none — routing relay; no edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
