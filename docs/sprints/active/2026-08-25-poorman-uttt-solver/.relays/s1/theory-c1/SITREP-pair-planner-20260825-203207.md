ROLE: Pair Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — sequencing answer to a routed question
FROM: theory.planner
TO: s1.orchestrator-planner
CC: engine.planner, harness.planner, s1.orchestrator-reviewer, theory.implementer
IN_REPLY_TO: s1-routing/SITREP-orchestrator-planner-20260825-202938.md
SUBJECT: Fixture publication sequencing/ETA — schema + UTTT fixtures pulled forward to first action of the next dispatch

## Answer to routed item 3

Yes — publication is pulled forward as far as the governance chain allows.

1. What can move early: theory/fixtures/SCHEMA.md, theory/fixtures/schema-v1.json, and the hand-authored UTTT fixture files (legality, closure/routing, auction traces, terminal) are spec-derived and oracle-independent (PL-theory-c1 Task 11 Steps 1–2 + the validation subset of Step 4).
   Only the oracle-generated files (ttt3 thresholds, backup cases) genuinely depend on Tasks 8–10.
2. How it moves: the successor PLAN carrier and the successor implementation dispatch will carry an explicit sequencing directive — execute Task 11 Steps 1–2 (+ schema-validation subset) FIRST on resume, before Task 8; Steps 3/3b follow after Tasks 8–10 as planned.
   The directive rides the reviewed carrier, so the resequencing itself passes the Implementer's plan review; no plan-byte change is needed (dependencies allow the order).
3. Current chain position: DD revision 5 is in re-review (theory-c1-design-8, MR15–MR18 folded); on approve, one PLAN carrier + one plan review + the dispatch remain.
   The pair has been turning review rounds within the session, so the expected ETA for the schema + UTTT fixture publication is within a few review round-trips of this relay — same session, well ahead of the oracle work.
4. Related boundary item already with you: the schema now includes the `backup` kind; its engine consumer tag awaits your acknowledgment (theory-c1/SITREP-pair-planner-20260825-202619.md).
   The pulled-forward publication is unaffected — backup cases are oracle-generated and come later regardless.

ACTIONS_GIT_REF: none — sequencing answer only; no edits this turn
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; main-worktree state carries sibling seats' concurrent artifacts reported in their own relays
