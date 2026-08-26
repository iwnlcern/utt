ROLE: Pair Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: theory-c1-impl-4
PARENT_DISPATCH_ID: theory-c1-plan-review-7
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — delegated conditional dispatch, all conditions evidenced below; later human gates unchanged (P1–P3 ratification at math lock; MR17 engine-tag acknowledgment; merge is a separate gate)
FROM: theory.planner
TO: theory.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-plan-review-7/PLAN-REVIEW-pair-implementer-20260825-205636.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: eed3832d05a2c4317e211bb11d2846634b8fc5664cc53dc98e08e6954a71ac1d
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: 4038719b2c8fe3bc13abfdab77e7f0488e86e5936279ff25c6a17dbd690cd079
BUNDLE_ID: theory-c1
OWNER: theory
REPO: /Users/jack/Programming/utt
BASE: main@d89f006
TARGET_BRANCH: main
BRANCH: theory/stage1-c1 (resume at held head f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d; Tasks 1–7 and 9 committed)
DELEGATED_DISPATCH_AUTHORITY: yes
SUBJECT: Implementation dispatch — resume theory Stage-1 with the early fixture slice, then Task 8 (supersedes theory-c1-impl-3, whose authority ended at the second hold)

## Delegated-dispatch conditions, evidenced (dispatch gate v2)

1. Implementer plan review = approve: theory-c1-plan-review-7/PLAN-REVIEW-pair-implementer-20260825-205636.md, parented to theory-c1-plan-7; that PLAN parents to the approving DESIGN-REVIEW theory-c1-design-review-9 for DD revision 5. This dispatch's PARENT_DISPATCH_ID points at that approving plan review.
2. Lock integrity at dispatch time (E2, this turn): DD digest eed3832d… and PLAN digest 4038719b… both re-hashed on disk and match the locked values reproduced above.
3. `relay verify` non-rendered set == exactly the adjudicated {INDEX.md divergence b5a880e0} (E2, fresh output this turn) — gate v2 satisfied (s1-daemon-adjudication-1).
4. No hard trigger, no boundary-contract deviation, no cross-bundle collision: same theory-only surface approved at plan-review-7; held worktree verified clean by that review with the full suite green (48 passed).
5. This dispatch is filed via daemon admission and addressed solely to theory.implementer.

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

Resume execution of PL-theory-c1-20260825 revision 5 on theory/stage1-c1 from held head f8662cd, in the reviewed order (plan-review-7 execution gates carried verbatim):
1. Task 11 EARLY SLICE first — focused fixture test red → hand-authored schema + UTTT files → green → explicit-path early commit; no `fixtures --check` claim until the final Task 11 expansion. Engine and harness consume these artifacts immediately on your early commit.
2. Then Task 8 (C11 census regression, synthetic exact threshold-payoff harness, de-zugzwanged C9a strata) and Task 10.
3. Then Task 11 Steps 3/3b/4/5 (oracle-generated files; backup cases emit `consumed_by: ["theory"]` unless the addressed MR17 orchestrator authorization has landed — re-check the theory-c1 relay tree at that point).
4. Tasks 12–13 close as written (FINDINGS C1–C11, P1–P3 proposed-for-ratification with the locked risk wording; evidence bundle; OPEN UNMERGED PR; math-lock handoff with the P1–P3 asks).
Hard stops remain live: NonDeterminacy on a masked state, LPEscalation, N = 32 intractability (N = 24 fallback orchestrator-gated) — stop and file a blocker relay.
File all relays via daemon admission. Merge authority is not granted by this dispatch.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay only; no source/test edits by this seat; evidence commands quoted above
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted dispatch; the Implementer re-checks worktree state at resume (held worktree verified clean at f8662cd by the approving review)
