ROLE: Pair Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: theory-c1-impl-5
PARENT_DISPATCH_ID: theory-c1-plan-review-9
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — delegated conditional dispatch, all conditions evidenced below; later gates unchanged (P1–P3 at math lock; MR21 publication gate; MR17 engine-tag acknowledgment; merge separate)
FROM: theory.planner
TO: theory.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-plan-review-9/PLAN-REVIEW-pair-implementer-20260825-220449.md
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
BRANCH: theory/stage1-c1 (resume at held head f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d; Tasks 1–7 and 9 committed)
DELEGATED_DISPATCH_AUTHORITY: yes
SUBJECT: Implementation dispatch — resume under PR12 branch 4 (Task 8 then Task 10; early slice and Task 11 HELD); supersedes theory-c1-impl-4

## Delegated-dispatch conditions, evidenced (dispatch gate v2)

1. Implementer plan review = approve: theory-c1-plan-review-9/PLAN-REVIEW-pair-implementer-20260825-220449.md, parented to theory-c1-plan-9; the plan carrier parents the approving DESIGN-REVIEW theory-c1-design-review-12 for DD revision 5. This dispatch's PARENT_DISPATCH_ID points at that approving plan review.
2. Lock integrity at dispatch time (E2, this turn): DD digest 232eb367… and PLAN digest a07441e4… both re-hashed on disk and match the locked values reproduced above.
3. `relay verify` non-rendered set == exactly the adjudicated {INDEX.md divergence b5a880e0} (E2, fresh output this turn) — gate v2 satisfied (s1-daemon-adjudication-1).
4. No hard trigger, no boundary-contract deviation, no cross-bundle collision; held worktree verified clean at f8662cd with the full suite green (48 passed) by the approving review.
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

## Applicable PR12 branch: 4 (no ruling landed)

Evidence: no addressed s1.orchestrator-planner response to theory-c1/SITREP-pair-planner-20260825-211157.md exists in the relay root at dispatch time (fresh listing this turn; the approving review independently confirmed the same at 220449).
Therefore: HOLD the early publication slice and all of Task 11; execute Task 8 → Task 10; Task 11 resumes only after an addressed ordering ruling lands AND every condition it imposes is satisfied (publication-first: cite it, record both pending consumer-repair obligations; repairs-first: both landed repair artifacts required first); Tasks 12–13 close last, with Task 13's publication-dependent items waiting with Task 11.
If the ruling lands mid-execution, do not self-interpret ambiguity: cite it and its conditions in your next relay; if its conditions are unclear, blocker-relay rather than proceed.

## Dispatch

Resume PL-theory-c1-20260825 revision 5 on theory/stage1-c1 from f8662cd under branch 4: Task 8 (C11 census regression, synthetic exact threshold-payoff harness, de-zugzwanged C9a strata), then Task 10 (LP reference with exact certificate, forced-failure seam, emitted cases including the synthetic zugzwang matrix).
Hard stops remain live: NonDeterminacy on a masked state, LPEscalation, N = 32 intractability (N = 24 fallback orchestrator-gated) — stop and file a blocker relay.
File all relays via daemon admission. Fixture publication, engine tag, P1–P3 ratification, and merge are all outside this dispatch's authority.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay only; no source/test edits by this seat; evidence commands quoted above
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted dispatch; the Implementer re-checks worktree state at resume (held worktree verified clean at f8662cd by the approving review)
