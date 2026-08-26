ROLE: Orchestrator Planner
PHASE: RECONCILE
AUTHORITY: report-only
DISPATCH_ID: s1-closure-ruling-1
PARENT_DISPATCH_ID: theory-c1
RUN_ID: s1
SUBJECT: ORDERING RULING — theory lane unblocked publication-first; consumer repairs authorized; theory merge gates on repairs landed
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — inter-pair sequencing and bounded repair authorization within ruled contract ownership
FROM: s1.orchestrator-planner
TO: theory.planner, engine.planner, harness.planner
CC: theory.implementer, engine.implementer, harness.implementer, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1/SITREP-pair-planner-20260825-211157.md

## 1. Schema ruling routed and binding

Theory's canonical closure-fixture shapes (theory-c1/SITREP-pair-planner-20260825-211157.md; DD-theory-c1 rev 5 @ 3e0f1192…) are hereby routed to both consumers and binding: required move_by; closure records {local, result} ordered by local; expected_forced integer-or-null; expected_terminal null|macro_win|all_closed (kind, not bool); consumed_by NORMATIVE for every consumer. Theory owns the schema; consumers reduce internally, never the reverse.

## 2. Ordering ruling (the MR21 gate resolves as follows)

- Theory lane: UNBLOCKED NOW, publication-first. Issue your successor dispatch this turn; Tasks 11-13 proceed to the PR and math-lock handoff without waiting on consumers.
- Merge sequencing: theory's publication PR MERGES only after both consumer repairs below are merged to main (or in the same merge window), so main is never red against the new bytes. Theory's merge grant will verify both landed; everything up to the PR is unconstrained.

## 3. Consumer repairs authorized (each a bounded small-tier item; plan-lite + pair review + delegated dispatch under gate v2; merge via PR under a separate grant)

- engine.planner — engine-fixture-repair: scope EXACTLY engine/tests/test_fixtures.cpp — read required move_by (drop optional mover default); parse closure records (assert locals; results at your option); reduce terminal kind internally; consumed_by already conforms. May develop against theory's branch bytes pre-merge. DISPATCH_ID engine-c3.
- harness.planner — loader-repair: scope EXACTLY referee/tests/test_fixture_loader.py — honor consumed_by (skip fixtures whose consumed_by excludes the harness suite); other shapes already conform. Sequence relative to harness-c2 at your discretion (separate micro-cycle DISPATCH_ID harness-c3; do NOT absorb into harness-c2's locked scope).
A second file in either repair is a deviation — relay it, do not absorb.

ACTIONS_GIT_REF: none — ruling relay; no edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; repo state carried in the orchestrator turn report
