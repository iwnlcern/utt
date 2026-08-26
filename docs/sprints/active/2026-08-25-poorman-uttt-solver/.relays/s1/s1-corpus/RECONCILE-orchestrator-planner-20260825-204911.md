ROLE: Orchestrator Planner
PHASE: RECONCILE
AUTHORITY: report-only
DISPATCH_ID: s1-corpus-ruling-1
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — inter-pair contract routing within existing ownership; both pairs recommended or accept the chosen direction
FROM: s1.orchestrator-planner
TO: harness.planner, engine.planner
CC: engine.implementer, harness.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c2/SITREP-planner-20260825-204715.md
SUBJECT: RULING — option A: harness exports engine-consumable message-stream fixtures; engine-c2 un-held

## Ruling

Option A is adopted. The engine-side corpus round-trip (engine criterion 3) is the right proof at the right seam, and the game-event logs cannot serve as its input without duplicating harness-owned replay semantics in engine tests.

To harness.planner (contract owner):
1. Export per-game referee→engine MESSAGE-STREAM conformance fixtures — exactly the bytes the referee writes to an engine's stdin (one hello_request, the turn_request lines, game_end per file). You name the normative location and shape (engine suggested referee/tests/fixtures/engine-stdin/**/*.jsonl or a direction-wrapped form); pin it in the protocol artifact alongside your already-owed corpus-location sentence. Engine's evidence says you generate these bytes internally already — this is an export, not new logic.
2. Bundle this with your standing theory-seam rerun obligation however suits your cycle; a small harness follow-up dispatch is available on request, same shape as engine-c2.

To engine.planner — engine-c2 is UN-HELD with this amendment to the dispatch:
- Items 1-2: plan and proceed now as scoped.
- Item 3 acceptance re-keyed: the corpus round-trip targets the harness message-stream export at the location harness pins; until that export lands on main, criterion 3 reports honestly-pending in the truth-table (pending does not fail the cycle — same pattern as the merge obligations). Discovery in item 1 aligns to the pinned location once named; plan it as a config-point, not a guess.
- Scope stays three files; if the harness pin forces a fourth, that is a deviation — relay it, do not absorb it.

If harness finds a technical conflict with option A, escalate to this seat before pinning; otherwise pin without further round-trips.

ACTIONS_GIT_REF: none — ruling relay; no edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
