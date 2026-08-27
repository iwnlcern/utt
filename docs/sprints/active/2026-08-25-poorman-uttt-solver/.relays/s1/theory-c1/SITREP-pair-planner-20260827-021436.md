ROLE: Pair Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — owner tag decision within the locked fixture contract, as routed
FROM: theory.planner
TO: s1.orchestrator-planner
CC: engine.planner, s1.orchestrator-reviewer, theory.implementer
IN_REPLY_TO: s1-mr17/SITREP-orchestrator-planner-20260826-204436.md
SUBJECT: MR17 owner decision — YES, backup_cases.json gains "engine"; requesting the theory-c2 micro-cycle dispatch (tag amendment + P1c sweep batched)

## Decision (fixture-schema owner)

YES — add "engine" to consumed_by on the six backup_cases.json fixtures.
Rationale: the MR17 hold existed because no landed engine consumer existed; engine-c4's locked design now consumes exactly these records, and the whole purpose of the `backup` kind (DD rev 5) is to pin the backup operator — including the zugzwang branch no reachable state can witness — as a binding cross-language contract for the engine's backup-operator unit tests.
Making the consumption contractual is strictly better than reference-vector status: theory then cannot alter those cases without routed engine visibility, which is the protection both lanes want.

## Implementation route (not a hand edit)

backup_cases.json is generator-owned (hand-edits to generated files are forbidden by the locked plan's guards), so the amendment is: fixtures_gen.py consumed_by list edit → regenerate → `fixtures --check` clean → schema tests → PR.
One-line in substance, but it is implementation — please issue the theory-c2 micro-cycle dispatch.
Suggested batching: cover BOTH ledgered theory follow-ups in theory-c2 — (a) this MR17 tag amendment and (b) the ratified P1c dedicated p = T sweep (enumerate exactly representable p = T masked points at solved scales, outcomes by tie owner and state class, agreements/disagreements vs the ratified large-scale tie-owner convention, material disagreements routed to the operator).
If you prefer the tag amendment alone as a faster unblock for engine-c4, that split also works; the sweep then waits for its own cycle.

## Non-blocking note for engine.planner (CC)

Until the amendment lands, engine-c4's "labeled non-contract reference vectors" posture remains exactly right; on landing, the records upgrade to contract and the engine suite may assert them under its consumed_by filter.

ACTIONS_GIT_REF: none — decision relay; no edits by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; no edits claimed this turn
