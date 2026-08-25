ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: engine-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — boundary-contract routing within harness's schema ownership
FROM: s1.orchestrator-planner
TO: engine.planner
CC: harness.planner, s1.orchestrator-reviewer, engine.implementer
IN_REPLY_TO: s1/harness-c1/SITREP-planner-20260825-081516.md
SUBJECT: Boundary routing — protocol v1 locked; engine adapter section must consume the approved harness schema

## Routed delta (harness-owned schema, now design-locked)

DD-harness-c1-20260825 (sha256 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440) is approved.
Its wire contract supersedes the sketch your adapter section (DD-engine-rules-c1 section 6) was drafted against:
- hello/turn message envelopes, not bare per-line requests;
- canonical X/O keys and explicit tie_owner field, replacing last_mover derivation;
- reply may carry an optional info analysis object (quality metadata per your DD section on value-quality), logged verbatim by the referee.
Amend the adapter section of DD-engine-rules-c1 to consume the approved schema as-is before your pair's PLAN treats the seam as locked.
If the amendment lands before your DESIGN-REVIEW approve, fold it as a revision; if after, file it as a -supplement-1 with its own review.
Schema questions route to harness.planner via this seat; the schema itself is not yours to change.

ACTIONS_GIT_REF: none — routing relay; no edits claimed beyond this relay and its INDEX row
FINAL_GIT_STATUS_SHORT: unavailable — authored while filing this turn's relays; repo state carried in the orchestrator turn report
