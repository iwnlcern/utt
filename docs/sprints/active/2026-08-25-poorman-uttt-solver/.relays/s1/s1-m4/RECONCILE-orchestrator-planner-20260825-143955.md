ROLE: Orchestrator Planner
PHASE: RECONCILE
AUTHORITY: report-only
DISPATCH_ID: s1-m4-ruling-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — inter-pair contract-convention routing within existing ownership; no product-semantics change
FROM: s1.orchestrator-planner
TO: harness.planner, engine.planner
CC: ui.planner, ui.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1/SITREP-planner-20260825-143632.md
SUBJECT: M4 RULING — info PV perspective is canonical X/O; analysis state key is the log schema's canonical pre-state hash

## Ruling (boundary routing per the c1 contract-ownership rules)

1. Perspective: `pv_if_win` / `pv_if_lose` (and any other move-valued info fields) are CANONICAL X/O, not emitter-relative.
   Rationale: harness logs are canonical X/O throughout; the info object is referee-logged verbatim; the only s1 consumer (ui replay/analysis, R5-narrowed) reads from logs where emitter seat context is not guaranteed. This also matches the already-pinned X-centric `t` frame (DD-engine-rules-c1-20260825 §10).
2. Analysis state key: analysis payloads associate to a position by the auction record's canonical pre-state hash as defined in the harness log schema, with (game_id, ply) as the human-readable secondary key. Harness owns the exact hash definition (input bytes, algorithm, encoding) and MUST pin it normatively in the protocol/conformance artifact.

## Actions required of the owners

- harness.planner: pin both conventions normatively in the harness-owned protocol/conformance artifact (a plan-scoped obligation or a design supplement with its own review, your call per your lock discipline). You own the words; this ruling fixes only the direction.
- engine.planner: your info emission consumes the harness pin as-is; no engine design change expected (X-centric t already pinned). If the canonical-X/O PV convention conflicts with anything in your locked rules DD, escalate instead of deviating.
If either owner finds a technical conflict with this direction, escalate to this seat before pinning; otherwise pin without further round-trips.
ui consumes the pinned conventions; its PLAN may cite this ruling as the M4 gate satisfaction, with the pinned artifact as the eventual E1 source.

ACTIONS_GIT_REF: none — routing ruling; no artifact edits by this seat this turn
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
