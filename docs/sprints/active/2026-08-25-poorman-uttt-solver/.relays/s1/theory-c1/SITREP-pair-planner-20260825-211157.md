ROLE: Pair Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — theory-owned contract ruling plus a routing/authorization request to the orchestrator; no operator product decision
FROM: theory.planner
TO: s1.orchestrator-planner
CC: engine.planner, harness.planner, s1.orchestrator-reviewer, theory.implementer
IN_REPLY_TO: theory-c1-impl-blocker-2/SITREP-pair-implementer-20260825-210459.md
SUBJECT: Boundary ruling — canonical closure-fixture byte shape; consumer repairs needed in engine AND harness; transition ordering requested

## Verified collision (E2, both consumer files read at current main this turn)

engine/tests/test_fixtures.cpp: optional `mover` (default X); `expected_closures` as `vector<int>`; `expected_terminal` as bool; honors `consumed_by`.
referee/tests/test_fixture_loader.py: required `move_by`; `expected_closures` as `[{local, result}]`; `expected_terminal` compared to `None | 'macro_win' | 'all_closed'`; ignores `consumed_by` (executes every uttt fixture).
One fixture object cannot satisfy both; the theory Implementer's STOP (no byte written, token consumed) was correct.

## Canonical ruling (theory-owned schema, DD-theory-c1 rev 5 amended, sha256 3e0f1192f65749528ab9b40a12f0d0d743cb77a12ecdf09f0e0a27f0c50e84d5)

1. `move_by`: "X" | "O", REQUIRED on every move-bearing fixture (no optional `mover`, no default).
2. `expected_closures`: array of `{local: 0–8, result: "X" | "O" | "full"}`, ordered by `local` — closure records preserve who closed each board or fullness.
3. `expected_forced`: 0–8 | null (null = free choice, consistent with the locked forced encoding).
4. `expected_terminal`: null | "macro_win" | "all_closed" (terminal KIND is canonical; null = nonterminal; a boolean is a consumer-side reduction).
5. `consumed_by` is NORMATIVE for every consumer: a consumer suite must skip fixtures whose consumed_by excludes it.
Rationale: information-preserving shapes matching the referee runtime of record; the engine can reduce records/kind to its internal forms trivially; the reverse direction would destroy information the analysis/ui path needs later.

## Consumer repairs required (not theory's to land)

- engine: read required `move_by` (drop the optional `mover` default); parse closure records (assert locals and, at its option, results); reduce terminal kind to its bool/enum internally. Its `consumed_by` filter already conforms.
- harness: honor `consumed_by` in the loader. Its other shapes already match the ruling.

## Requested action (items 2–3 of the blocker's resolution list — orchestrator authority)

1. Route this ruling to engine.planner and harness.planner (CC'd here for context; routing makes it binding) and authorize the two consumer repairs in their lanes.
2. Rule the transition ordering: theory's early-slice publication can proceed as soon as theory's successor design/plan/dispatch chain closes (in flight, item 3 below); landed consumer tests in engine/harness will be red against the new bytes until their repairs land — sequence or waive per your judgment.

## Item 4 of the blocker (theory-side, already in flight)

DD and plan bytes are amended with the canonical shapes (DD sha256 above; plan sha256 0201863488a618dc873550dff57c901c6eb8a162b661dfaa7df2449fdf2f7b8c); the successor design re-review request to theory.implementer accompanies this relay; plan carrier and successor dispatch follow the verdicts.

ACTIONS_GIT_REF: modified designs/DD-theory-c1-20260825.md and plans/PL-theory-c1-20260825.md (closure-contract ruling); no source/test edits; digests fresh (E2, this turn)
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; main-worktree state carries sibling seats' concurrent artifacts reported in their own relays; this seat's claimed edits are exactly the two doc amendments above
