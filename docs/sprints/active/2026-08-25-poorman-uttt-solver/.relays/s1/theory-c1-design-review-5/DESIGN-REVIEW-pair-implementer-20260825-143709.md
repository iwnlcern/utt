ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-5
PARENT_DISPATCH_ID: theory-c1-design-5
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — Planner can reconcile the two stale acceptance clauses from already-settled MR10/MR11 evidence; P1/P2 remain the later operator math-lock gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-5/DESIGN-planner-20260825-143245.md
SUBJECT: DESIGN-REVIEW must-revise — MR12 closed; acceptance still contradicts settled MR10/MR11 state
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 8b828cb12312a2edd6dc9af21668a4a852da66bae389d4277e1403dece515cdf
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

MR12 is closed exactly as requested: the mandatory `GRILL_LOCK` now records the implementation hold and rev-4 review sources, adds D6 with the two explicit Richman conventions, and advances Design-lock impact to revision 4 / D1–D6 / MR1–MR12. The Richman correction remains sound and isolated from canonical auction terminal semantics.

The exact design bytes are not yet lockable because section 4 retains two stale acceptance clauses that contradict already-settled MR10/MR11 state and the newly corrected lock-impact record. These are narrow record-consistency corrections; no operator decision or design reopening is required.

## Design-grill result

Questions resolved sequentially from repository evidence:

1. Was MR12 folded without changing the corrected Richman design or D1–D5/P1–P2? Yes. The new GRILL_LOCK hunks are the requested source, D6, and lock-impact additions; DD lines 104–108 and 149 remain unchanged; `git diff --check` is clean.
2. Does section 4 now agree with the folded decision history? No. Line 151 still carries the pre-MR10 `D_max` monotonicity acceptance law, and line 154 still calls the 080619 acknowledgment pending.
3. Is either discrepancy operator-owned? No. MR10 already made cross-scale rise diagnostic-only, MR11's addressed 080619 relay explicitly satisfied the publication prerequisite, and plan revision 4 already reflects both outcomes.

Recommended answer: preserve the complete MR12 fold and correct only the two stale section-4 clauses below.

## Fold disposition

- MR12: closed. `GRILL_SOURCE`, questions answered from codebase, D6, and Design-lock impact all carry the rev-4 Richman convention decision durably.
- Richman self-check design: approved as sound, conditional on the acceptance-record corrections below; no further mathematical change requested.

## Must-revise findings

### MR13 — Replace the stale `D_max` monotonicity acceptance law

DD line 151 says `D_max` must be non-increasing across doublings. That contradicts the locked MR10 design at lines 96–97: the defined statistic is `R_mis`, it is reported with in-band occupancy, and a rise on doubling is an anomaly routed to the orchestrator rather than an acceptance failure. `D_max` is otherwise absent from the current DD, and plan revision 4's acceptance criteria correctly require zero out-of-band disagreements plus `R_mis`/occupancy reporting.

Replace line 151 with the settled rule: zero out-of-band disagreements at every tested scale is the hard criterion; report `R_mis`, its attaining state, maximum in-band ratio, in-band occupancy, M=0 exclusions, and off-mask diagnostics; cross-scale rises are anomaly findings, not acceptance failures.

### MR14 — Mark the forced-encoding acknowledgment satisfied

DD line 154 says the forced-encoding acknowledgment is pending and not yet received. The addressed orchestrator relay `theory-c1/SITREP-orchestrator-planner-20260825-080619.md` locks integer-or-null across theory and harness and states that the fixture-publication prerequisite is satisfied. The prior approving review records MR11 closed on that evidence, the plan cites 080619 at lines 22/586/660, and the updated GRILL_LOCK line 205 now correctly says the acknowledgment is locked.

Replace only the stale prerequisite clause in line 154 with the settled state: fixture schema v1 and seed publication cite the 080619 acknowledgment; the prerequisite is satisfied.

## Successor acceptance and boundaries

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh digest, and `IN_REPLY_TO` naming this review. Preserve the MR12 fold, the corrected Richman lines 104–108/149, all D1–D6/P1–P2 content, and every unrelated byte. On exact closure of MR13–MR14, revision 4 is approvable for successor PLAN review.

- Routed digest: SHA-256 `8b828cb12312a2edd6dc9af21668a4a852da66bae389d4277e1403dece515cdf` matches the addressed relay.
- Exact incoming relay lint with freshness disabled: clean. Root lint remains red from inherited INDEX history and the transient prior PLAN digest; neither condition authorizes implementation.
- No source, test, design, plan, branch, commit, push, PR, merge, or runtime action is authorized or taken by this review.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay submitted through the daemon; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-design-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-plan-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-pair-implementer-20260825-095726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-5/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-implementer-20260825-100211.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-review-4/
