ROLE: Pair Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — boundary-contract routing request to the orchestrator; no operator decision
FROM: theory.planner
TO: s1.orchestrator-planner
CC: engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-review-7/DESIGN-REVIEW-pair-implementer-20260825-163906.md
SUBJECT: Boundary decision — new `backup` fixture kind; engine consumer tag held pending your acknowledgment (MR17)

## Boundary-contract decision (theory-owned fixture schema, pre-publication)

DD-theory-c1-20260825 revision 5 adds a `"game": "backup"` fixture kind: abstract backup-operator cases (`a`, `b`, `h`, `expected_T`, `expected_critical_bid` as rational strings, no board state), carrying the `a > b` zugzwang-branch cases that C11 proves have no reachable canonical-game witness, plus `a = b` and `a < b`.

## Consumer boundary and requested action (per design-review finding MR17)

The landed engine rules-core consumes only `game: "uttt"` fixtures; the real consumer of backup cases is the theory-gated SUCCESSOR engine-search design's backup-operator unit tests.
Accordingly:
1. Publication will emit backup cases with `consumed_by: ["theory"]` by default.
2. This addressed relay requests your acknowledgment/routing so the `engine` tag may be added — either now (binding the successor engine-search design as the named future consumer) or later when that design dispatches; until an addressed acknowledgment lands, the engine tag is not emitted.
3. No-consumer action on record: theory-only evidence, engine tag deferred.
engine.planner is CC'd as the adjacent contract owner for context only; no action is required from engine.

ACTIONS_GIT_REF: modified designs/DD-theory-c1-20260825.md and plans/PL-theory-c1-20260825.md (revision-5 MR15–MR18 folds, claimed in the concurrent design re-request); no source/test edits; this relay and its admission are the only additional artifacts this turn
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; main-worktree state carries sibling seats' concurrent artifacts reported in their own relays
