ROLE: Pair Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: theory-c1-impl-6
PARENT_DISPATCH_ID: theory-c1-plan-review-9
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — delegated conditional dispatch under gate v2 and PR12 branch 1 (the addressed ordering ruling landed); later gates unchanged (P1–P3 at math lock; theory MERGE gated on both consumer repairs per the ruling; MR17 engine-tag acknowledgment still pending)
FROM: theory.planner
TO: theory.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1-closure/RECONCILE-orchestrator-planner-20260826-022721.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd
BUNDLE_ID: theory-c1
OWNER: theory
REPO: /Users/jack/Programming/utt
BASE: main@d89f006
TARGET_BRANCH: main
BRANCH: theory/stage1-c1 (resume at 480b6818abee7ade39e49950e2e6242fe701e03a; Tasks 1–10 minus 11-early complete)
DELEGATED_DISPATCH_AUTHORITY: yes
SUBJECT: Implementation dispatch — publication-first under the landed ordering ruling: early slice, Task 11 completion, Tasks 12–13 (supersedes theory-c1-impl-5, whose authorized slice is complete and reconciled)

## Applicable PR12 branch: 1 — publication-first ruling landed

Ruling citation: s1-closure/RECONCILE-orchestrator-planner-20260826-022721.md (s1-closure-ruling-1, FROM s1.orchestrator-planner, addressed TO theory.planner among others) — theory lane UNBLOCKED publication-first; Tasks 11–13 proceed to the PR and math-lock handoff without waiting on consumers.
Pending consumer-repair obligations, recorded honestly per branch 1 (authorized by the same ruling; owned by their seats, NOT theory's): engine-c3 (engine/tests/test_fixtures.cpp — move_by, closure records, terminal-kind reduction) and harness-c3 (referee/tests/test_fixture_loader.py — honor consumed_by).
Merge sequencing from the ruling, carried into Task 13: theory's PR opens unmerged as planned, and its merge grant will additionally verify BOTH consumer repairs are merged (or same-window); record this in the math-lock handoff.
The ruling binds the canonical shapes of DD rev 5 as routed at digest 3e0f1192… (the 211157-era bytes); the current approved digest 232eb367… carries those shapes byte-identically plus the later MR20–MR22 governance folds — no shape delta.

## Delegated-dispatch conditions, evidenced (dispatch gate v2)

1. Implementer plan review = approve: theory-c1-plan-review-9 (approve at the exact current digests), parented to theory-c1-plan-9 → design-review-12; its execution-gate 3 anticipated exactly this dispatch form ("If a publication-first ruling later lands, the dispatch ... must cite it and record both pending repair obligations honestly"). This dispatch's PARENT_DISPATCH_ID points at that approving review.
2. Lock integrity at dispatch time (E2, this turn): DD 232eb367… and PLAN a07441e4… re-hashed on disk, matching.
3. `relay verify` non-rendered set == exactly the adjudicated {INDEX.md divergence b5a880e0} (E2, fresh this turn).
4. No hard trigger, no boundary deviation, no collision; the impl-5 slice was reconciled green by this seat (SITREP-pair-planner-20260826-021519.md) and the worktree verified clean at 480b681.
5. Filed via daemon admission, addressed solely to theory.implementer.

SCOPE_DIFF:
- theory/pyproject.toml -> in
- theory/.gitignore -> in
- theory/uv.lock -> in
- theory/auction_ttt/ -> in
- theory/tests/ -> in
- theory/fixtures/ -> in
- theory/results/ -> in
- theory/FINDINGS.md -> in
- docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/ -> in
- docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md -> in
SCOPE_DIFF_RESULT: all-in

## Dispatch

Resume PL-theory-c1-20260825 revision 5 on theory/stage1-c1 from 480b681, in the branch-1 order:
1. Task 11 EARLY SLICE (schema-v1.json + SCHEMA.md citing 080619 + the four hand-authored UTTT category files under the D8 canonical shapes + focused red/green test + explicit-path early commit). The early-commit relay records the two pending consumer repairs.
2. Task 11 Steps 3/3b/4/5 (oracle-generated thresholds + backup cases — backup emits `consumed_by: ["theory"]`; the MR17 engine-tag acknowledgment has NOT landed, re-verify in the relay tree before emitting; full fixture test module + mandatory `fixtures --check`; final eight-file commit).
3. Tasks 12–13 as written: FINDINGS (C1–C11, C8a and C11-lemma attempt-and-record, P1–P3 proposed-for-ratification), evidence bundle, push + OPEN UNMERGED PR, math-lock handoff with the P1–P3 asks, the backup consumer state, and the ruling's merge-sequencing note (theory merge awaits both consumer repairs).
Hard stops remain live: NonDeterminacy on a masked state, LPEscalation — stop and blocker-relay.
File all relays via daemon admission. Merge authority is not granted by this dispatch.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay only; no source/test edits by this seat; evidence commands quoted above
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted dispatch; the Implementer re-checks worktree state at resume (verified clean at 480b681 by this seat's reconciliation)
