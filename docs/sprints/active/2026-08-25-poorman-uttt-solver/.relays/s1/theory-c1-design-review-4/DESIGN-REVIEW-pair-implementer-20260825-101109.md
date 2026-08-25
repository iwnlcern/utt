ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-4
PARENT_DISPATCH_ID: theory-c1-design-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — Planner can correct the stale folded grill record; P1/P2 remain the later operator math-lock gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-4/DESIGN-planner-20260825-100635.md
SUBJECT: DESIGN-REVIEW must-revise — rev-4 Richman correction is sound but the mandatory GRILL_LOCK still locks revision 3
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 4f02840e4ee48291638d3f7810b14c88f9876f456c1318a443caf558c591af47
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

The revision-4 Richman correction is mathematically and architecturally sound: the two explicit terminal conventions reproduce the published 133/256 and the complement-symmetric 1/2 result respectively, and the canonical Stage-1 auction semantics remain isolated from this machinery-only self-check. The design is not yet lockable because its mandatory folded `GRILL_LOCK` still says the PLAN must reference revision 3, records only D1–D5, and does not carry the rev-4 convention decision that this `GRILL_REQUIRED: yes` cycle is meant to make durable.

## Design-grill result

Questions resolved from repository evidence, with no operator question required:

1. Does draw-to-O reproduce the cited literature value? Yes. The prior exact-Fraction diagnostic gave 133/256 when only full-board draws changed from 1/2 to 1, matching Develin–Payne section 6.
2. Does draw-half independently constrain the machinery? Yes. X/O complement symmetry forces the swap-invariant empty board to 1/2 under the average backup.
3. Does the split alter canonical auction semantics? No. DD lines 104–108 explicitly isolate Richman mode as a self-check and retain the canonical chip-comparison terminal rule elsewhere.
4. Did any unrelated DD content move? No. The exact diff from revision 3 contains only the revision-status line, the Richman self-check bullet, and its acceptance line; `git diff --check` is clean.

Recommended answer: keep the rev-4 two-convention design exactly as written and correct the folded grill record so the design lock actually carries it.

## Must-revise finding

### MR12 — Fold the Richman erratum into `GRILL_LOCK`

The design header declares `GRILL_LOCK_ID: GL-theory-c1-20260825 (folded in below)` and this addressed DESIGN cycle carries `GRILL_REQUIRED: yes`, but the artifact at DD lines 174–204 was not advanced with revision 4:

- `GRILL_SOURCE` omits the implementation-hold relay, the exact diagnostic, and the Develin–Payne convention check.
- `Resolved decisions` ends at D5 and does not record the explicit two-convention Richman self-check.
- `Design-lock impact` says the theory PLAN must reference revision 3 carrying D1–D5.

Correct forward in the same design document:

1. Add the rev-4 design relay and `theory-c1/SITREP-pair-implementer-20260825-100211.md` to `GRILL_SOURCE`, including the exact diagnostic/literature definition under questions answered from code/docs.
2. Add D6: Richman machinery self-check convention — `draw="O"` terminal 1 asserts 133/256; `draw="half"` terminal 1/2 asserts 1/2; canonical auction terminal semantics remain unchanged — source: implementation-hold E2 diagnostic plus Develin–Payne section 6.
3. Change `Design-lock impact` to revision 4 carrying D1–D6 and name the Richman self-check convention as D6.

Do not change the corrected DD lines 104–108 or 149, broaden Richman mode into canonical gameplay, or alter any unrelated D1–D5/P1–P2 decision. Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh digest, and `IN_REPLY_TO` naming this review.

## Evidence and boundaries

- Routed digest: SHA-256 `4f02840e4ee48291638d3f7810b14c88f9876f456c1318a443caf558c591af47` matches the addressed relay.
- Revision diff: only the three announced DD hunks; no whitespace errors.
- Exact incoming relay lint with freshness disabled: clean. Root lint remains red from inherited INDEX ordering and the expected transient old PLAN digest after the staged rev-4 plan bytes; neither condition authorizes implementation.
- This review makes no source, test, design, plan, branch, commit, push, PR, merge, or runtime change.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay submitted through the daemon; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? .harness-c1-design-review-4-draft.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1-plan-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-pair-implementer-20260825-095726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-implementer-20260825-100211.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-review-4/
