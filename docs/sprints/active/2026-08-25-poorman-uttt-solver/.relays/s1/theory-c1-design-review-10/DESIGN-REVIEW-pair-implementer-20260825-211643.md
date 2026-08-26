ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-10
PARENT_DISPATCH_ID: theory-c1-design-10
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — canonical bytes are sound; the remaining grill-record and transition-order gaps require owner/orchestrator routing, not an operator product decision
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-10/DESIGN-planner-20260825-211158.md
SUBJECT: DESIGN-REVIEW must-revise — canonical closure bytes survive; GRILL_LOCK and transition ordering remain open
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 3e0f1192f65749528ab9b40a12f0d0d743cb77a12ecdf09f0e0a27f0c50e84d5
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

The canonical closure/routing byte shape is sound and should be preserved: required top-level `move_by`; information-preserving ordered closure records; integer-or-null forced routing; canonical terminal kind rather than the engine's lossy boolean projection; and normative `consumed_by` filtering for every consumer. This cleanly resolves the ownership question raised by the third implementation STOP.

The exact DD bytes are not yet lockable. `GRILL_REQUIRED: yes` is carried by the addressed relay, but the folded GRILL_LOCK still ends at D7 and does not record the third hold, the two landed consumers, the canonical-shape decision, rejected alternatives, or transition-order question. Separately, the DD and incoming relay imply that early publication unblocks/resolves consumers even though the addressed orchestrator ruling on repair authorization and publication ordering has not landed.

These are narrow correct-forward repairs. This review does not reopen the canonical shape, authorize consumer edits, approve the staged plan, resume implementation, publish fixtures, or authorize push, PR, merge, release, or P1–P3 ratification.

## Design-grill result

Questions resolved sequentially from repository evidence; no operator question was required:

1. Is the canonical shape information-preserving and implementable? Yes. It matches the harness runtime's closure records and terminal kinds; the engine can deterministically reduce closure records to local indices and terminal kind to terminal/nonterminal.
2. Is `consumed_by` filtering required to make split consumer coverage safe? Yes. The engine already honors it; the harness currently does not and therefore requires an owning-seat repair.
3. Did the mandatory durable grill record capture this decision? No. GRILL_SOURCE lines 196–198 omit theory-c1-impl-blocker-2 and the harness consumer inspection; Resolved decisions stop at D7; Design-lock impact still says D1–D7 and MR1–MR14 only.
4. Has transition ordering been authorized? No. `theory-c1/SITREP-pair-planner-20260825-211157.md` is an addressed request TO s1.orchestrator-planner. It requests consumer-repair authorization and an ordering ruling; it is not that ruling.
5. Can the DD truthfully say the collision is resolved and early publication unblocks engine now? Not yet. Canonical semantics are resolved, but landed consumer compatibility and publication order remain pending.

Recommended answer: preserve the ruled shape; fold it as D8 into GRILL_LOCK; make publication conditional on the addressed orchestrator ordering ruling (or cite that ruling if it lands); then request re-review.

## Must-revise findings

### MR20 — Complete the mandatory GRILL_LOCK for the third hold

Fold theory-c1-impl-blocker-2, both landed consumer files, and the 211157 routing request into GRILL_SOURCE. Add the answered questions: why one object cannot satisfy both current consumers, why terminal kind/closure records preserve required information, why `consumed_by` is normative, and why theory cannot authorize consumer repairs.

Add a resolved decision D8 for the exact canonical bytes and filtering rule, including source and rationale. Record rejected alternatives: engine's index/bool/default-mover shape; unreviewed dual fields; split fixtures without harness filtering; and omitting closure/routing coverage. Update Design-lock impact from D1–D7/MR1–MR14 to include D8 and the third-hold amendment.

### MR21 — Do not pre-decide publication ordering while its ruling is pending

DD status line 7 says the amendment resolves the landed collision, line 124 says early publication unblocks engine, and the incoming relay says theory will publish once its own design/plan/dispatch chain closes. The addressed 211157 relay instead asks the orchestrator to decide whether publication may precede consumer repairs.

Distinguish semantic resolution from operational compatibility. Required boundary:

- no successor implementation dispatch may authorize publication until an addressed s1.orchestrator-planner ordering ruling lands;
- if the ruling allows publication-first with known-red consumer suites, cite that explicit authorization and record the pending consumer obligations honestly;
- otherwise require the engine/harness repairs to land before publication;
- state the no-ruling action: hold the early publication slice, without blocking unrelated read-only review work.

If the addressed ruling lands before the successor, fold its exact path and ordering into the DD/PLAN carrier. CC visibility or the theory-owned request alone is not authorization.

## Preserved decisions and boundaries

- Preserve `move_by: X|O`, ordered `{local,result}` closure records, `expected_forced: int|null`, and terminal kind `null|macro_win|all_closed`.
- Preserve normative `consumed_by`; engine repair is move_by/record/kind consumption and harness repair is filtering.
- Preserve all prior revision-5 decisions: C11/D7/P3, synthetic C3 validation, backup discriminator and theory-only default, P1–P3 operator ownership, C9/LPEscalation stops, N=24 gate, and separate merge authority.
- The held implementation worktree remains clean at `theory/stage1-c1@f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`; theory.implementer made no source/test/design/plan edits.

## Successor acceptance

Return a successor DESIGN carrier with the same DESIGN_DOC_ID, fresh digest, and `IN_REPLY_TO` naming this review. Close MR20 and MR21 while preserving the canonical byte shape. On exact closure, the design is approvable for a successor PLAN review; implementation remains separately gated.

## Evidence

- Routed integrity: SHA-256 `3e0f1192f65749528ab9b40a12f0d0d743cb77a12ecdf09f0e0a27f0c50e84d5` matches the addressed carrier; staged-plan SHA-256 is `0201863488a618dc873550dff57c901c6eb8a162b661dfaa7df2449fdf2f7b8c`.
- Exact incoming relay lint with freshness disabled: clean. Root lint remains red from inherited INDEX chronology, unrelated foreign relay claims, and the expected superseded blocker digest.
- `git diff --check` for the design and staged-plan bytes: clean.
- Current-main consumer evidence remains exactly the third STOP: engine `test_fixtures.cpp` lines 81–100/153–156 and harness `test_fixture_loader.py` lines 70–91.
- No addressed orchestrator response to the 211157 transition-order request was present at review time.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay submitted through daemon admission; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? .superpowers/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-contract-rulings-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-10/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-blocker-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-7/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-planner-20260825-211157.md
?? ui/
