ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-11
PARENT_DISPATCH_ID: theory-c1-design-11
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — one textual consistency repair remains; the separate orchestrator ordering ruling is still pending
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-11/DESIGN-planner-20260825-212001.md
SUBJECT: DESIGN-REVIEW must-revise — MR20 closes; MR21 leaves one stale publication-prerequisite claim
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: d3717183c0c56b59334b5de4390d1721b1207f64925413aaa83e6a115f1f86d3
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

Must revise DD-theory-c1-20260825 revision 5 at SHA-256 `d3717183c0c56b59334b5de4390d1721b1207f64925413aaa83e6a115f1f86d3` before the successor gated PLAN carrier.

MR20 is closed. The mandatory GRILL_LOCK now names the third implementation hold, both landed consumer surfaces and their line ranges, the 211157 routing request, and design-review-10; it records all four code-answerable questions, D8's canonical byte shape and filtering rule, the rejected alternatives, and the D1–D8 / MR1–MR21 lock impact.

MR21 is closed in the STATUS line, the section-3 publication-ordering gate, and the staged PLAN's EARLY SLICE. One contradictory acceptance sentence remains: DD section 4 still says `the publication prerequisite is satisfied` while the addressed ordering ruling is explicitly pending. That stale assertion makes the design internally inconsistent about whether publication is presently dispatchable.

The canonical closure/routing schema remains approved in substance and must not change: required `move_by`; ordered closure records `{local, result}`; integer-or-null `expected_forced`; terminal kind `null | "macro_win" | "all_closed"`; and normative `consumed_by` filtering for every consumer.

This review does not authorize implementation, consumer repair, fixture publication, push, PR, merge, or release activity. With no addressed s1.orchestrator-planner response to the 211157 request present at review time, the early publication slice remains held.

## Design-grill result

Questions resolved sequentially from repository evidence; no operator question was required:

1. Is the mandatory GRILL_LOCK complete for the third hold? Yes. GRILL_SOURCE, code-answerable questions, D8, rejected alternatives, and Design-lock impact contain the MR20-required material.
2. Does the canonical byte-shape decision preserve all information required by the landed consumers? Yes. Records and terminal kinds preserve the referee runtime shape; the engine can reduce them deterministically; normative `consumed_by` makes split coverage safe after the owning-seat repair.
3. Is publication ordering correctly gated in the design body and staged plan? Mostly. DD lines 125–126 and PLAN lines 609–610 hold publication until the addressed ruling, and the carrier does not pre-decide the ruling.
4. Does any remaining design text contradict that gate? Yes. DD line 172 says the publication prerequisite is satisfied without narrowing the claim to the already-closed forced-encoding prerequisite.

Recommended answer: retain all D8 and MR20/MR21 gate bytes, correct the single stale acceptance parenthetical, float the design digest in a successor DESIGN carrier, and request re-review.

## Required revision

### MR22 — Qualify the satisfied prerequisite and retain the pending ordering gate

In DD section 4's domain-acceptance bullet, replace the claim `the publication prerequisite is satisfied` with an explicit split: the forced integer-or-null encoding prerequisite is satisfied by the 080619 acknowledgment, while the MR21 publication-ordering and consumer-compatibility prerequisite remains pending the addressed response to the 211157 request.

Preserve the section-3 gate and PLAN EARLY SLICE verbatim in substance: publication-first requires the addressed ruling and honest pending-repair obligations; repairs-first waits; no ruling holds the early slice without blocking read-only work.

Return a successor DESIGN carrier with the same DESIGN_DOC_ID, a fresh digest, and `IN_REPLY_TO` naming this review. On exact closure, the design is approvable for a successor PLAN review; implementation remains separately gated.

## Evidence

- Routed integrity: design SHA-256 `d3717183c0c56b59334b5de4390d1721b1207f64925413aaa83e6a115f1f86d3` and staged-plan SHA-256 `80b22b7b7783a18dea463717d4a05789e9271cc5e3c5b6ef467cda8d6d6f5180` match the addressed carrier.
- Exact incoming relay lint with freshness disabled: clean.
- `git diff --check` for the design and staged-plan bytes: clean.
- No addressed orchestrator ordering response was present in the relay root at review time.
- Fresh implementation-worktree status: clean on `theory/stage1-c1` at `f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`.
- Root-wide lint was not claimed green; inherited/concurrent relay-root state remains outside this exact-file verdict.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay submitted through daemon admission; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? .superpowers/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-contract-rulings-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-10/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-11/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-10/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-blocker-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-7/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-planner-20260825-211157.md
?? ui/
