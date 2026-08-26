ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: harness-c2
RUN_ID: s1
SUBJECT: PLAN dispatch — harness-c2 (engine-stdin export, protocol-doc corpus pins, conditional theory-seam flip)
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — continuation cycle under the locked harness DD executing an adopted ruling; merge stays separately gated
FROM: s1.orchestrator-planner
TO: harness.planner
CC: harness.implementer, engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-contract-rulings-2/SITREP-planner-20260825-205131.md
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
BUNDLE_ID: harness-c2
OWNER: harness pair
REPO: /Users/jack/Programming/utt (origin https://github.com/iwnlcern/utt)
BASE: main@e92caff
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision; dispatch gate v2 (no verify events beyond the adjudicated set; daemon-admitted filing).

## Scope (locked, per your contract-rulings-2 request)

1. Implement the engine-stdin export in the fixture-generation path and commit the generated stream corpus at referee/tests/fixtures/engine-stdin/*.jsonl, exactly per your pin: one file per game per seat, <log-fixture-stem>.<seat>.jsonl, raw replayable stdin bytes (hello_request, every turn_request incl. re-request attempts, game_end), emitted by the same seeded runs as the paired log fixtures, byte-stable under the fake clock.
2. Protocol-doc corpus section in docs/protocol/poorman-uttt-protocol-v1.md pinning all three locations (game-event log globs, engine-stdin stream glob, normative transcript), replacing the previously owed single sentence.
3. Conditional: if theory/fixtures lands on main within this cycle's window, flip the theory-seam skip to required, run, and report; otherwise the obligation stays standing and this cycle closes without it.
In-scope surfaces: referee/ fixture-generation and test files, generated referee/tests/fixtures/engine-stdin/ artifacts, docs/protocol/poorman-uttt-protocol-v1.md. Anything else is a deviation.

## Design basis

No fresh DESIGN phase: this cycle continues under DD-harness-c1-20260825 @ c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e with the adopted corpus ruling (s1-corpus-ruling-1) and your own pin (harness-c1-contract-rulings-2) as the owner records of what to build. Your gated PLAN locks that DD per the usual lineage.

Acceptance: stream corpus regenerates byte-identically with the paired logs; every stream file round-trips through the referee's own reader; doc section pins all three locations; suite green; engine's criterion-3 consumer becomes satisfiable at the pinned glob (their alignment is engine-c2's, not yours). Exit with branch + PR + report; merge under a separate addressed grant per the standing remote+PR policy.
FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time; no work actions claimed by this relay
