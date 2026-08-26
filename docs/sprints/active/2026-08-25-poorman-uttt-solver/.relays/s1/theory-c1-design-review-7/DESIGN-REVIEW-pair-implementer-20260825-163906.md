ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-7
PARENT_DISPATCH_ID: theory-c1-design-7
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the census adjudication and synthetic basis are sound; the Planner can correct the remaining internal and boundary-contract inconsistencies without changing P3's later operator gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-7/DESIGN-planner-20260825-162057.md
SUBJECT: DESIGN-REVIEW must-revise — D7 basis survives the grill; stale reachable-zugzwang text and backup fixture boundary remain
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 661feacd56bc089ca0ada464e013e74bb0d57c59fb82a99fe6266689daba213b
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

The central revision-5 decision is sound: the zero-zugzwang census is expected rather than evidence of an oracle defect, the design truthfully records the missing reachable witness as C11/P3, and a synthetic exact one-step harness is an appropriate computational check of C3 if it is labeled as synthetic threshold-payoff evidence rather than canonical chip-comparison play.

The exact design bytes are not yet lockable. Section 2 still mandates reachable zugzwang sampling/spots despite C11, section 3's normative top-level enum and common envelope exclude the new `backup` kind they are supposed to govern, and the new cross-pair consumer claim is not aligned with the current engine consumer. These are correct-forward consistency and boundary-contract repairs; no fresh operator decision is needed, and P3 remains operator-owned at the later math lock.

This review does not approve the staged plan, authorize implementation, resume Task 8, ratify P1–P3, change the locked rules, or authorize publication, push, PR, merge, or release activity.

## Design-grill result

Questions resolved sequentially from repository evidence; no operator question was required:

1. Is the census repeatable, and is its boundary nonempty? Yes. A fresh full solve at `f8662cd` returned 22,186 nodes, zero `a > b`, zero `.zugzwang`, and minimum `b-a = 0` at `('.OXXOOOXX', 'O')` with `a=b=1/2`, matching both seats' prior diagnostics.
2. Does the proposed synthetic matrix contract actually exhibit the claimed branch behavior? Yes, when its children are defined as synthetic exact threshold-payoff terminals. A read-only exact integer enumeration over every `(bx,bo)` grid point for `(a,b) ∈ {(3/4,1/4),(2/3,1/3),(1,0)}`, both `h`, and N in {8,16,32,64} found zero maximin/minimax inequalities, zero out-of-band prediction failures, and `(0,0)` was a pure saddle at every tested point.
3. Can those children truthfully be called chip-comparison terminals? No. Canonical chip comparison has threshold 1/2; assigned thresholds such as 3/4 and 1/4 are deliberately synthetic terminal payoff predicates. The design must name that distinction so C3 evidence cannot be misread as rules-kernel evidence or a rules reinterpretation.
4. Did revision 5 remove every reachable-zugzwang requirement? No. DD lines 95–96 and D3 at line 197 still require a zugzwang C9a stratum and N=64/128 zugzwang spots, contradicting C11/D7 and the revised acceptance section.
5. Is the new fixture contract self-consistent and already consumed as claimed? No. DD line 125 limits `game` to `uttt|ttt3`, line 140 requires canonical `state` fields for every fixture, and line 148 omits the backup category, while line 146 introduces `game: backup` with `(a,b,h)`. The current engine rules consumer explicitly processes only `game: uttt`; the backup consumer belongs to the theory-gated successor engine-search design, not the landed rules-core fixture reader.

Recommended answer: preserve C11/D7/P3 and the verified synthetic cases, then fold MR15–MR18 below before requesting re-review.

## Must-revise findings

### MR15 — Remove stale reachable-zugzwang requirements from the architecture and D3 record

DD lines 95–96 still say the N=12 C9a sample includes zugzwang states and the N=64/128 sweep performs zugzwang spot checks. D3 at line 197 repeats the same spot-check contract. Those statements are impossible under C11 and contradict the revision-5 Task 8/acceptance intent.

Replace the C9a sample strata with near-band, masked both-zero, and uniform masked states; point to the separate C11 census plus synthetic C3 harness. Restate D3's target population truthfully: deterministic in-band/knife-edge owners remain, while the formerly requested reachable-zugzwang subset is proven empty by C11 and synthetic validation is separate. Preserve N=64/128 and the no-sampling rule.

### MR16 — Define `game: backup` as a real discriminated schema branch

DD line 125's normative enum excludes `backup`; line 140's common envelope requires board/forced/budgets state for every fixture; line 146 instead describes abstract `(a,b,h)` triples; line 148's exhaustive category list omits backup. A schema implementer cannot satisfy all four statements without inventing a shape.

Make the top-level enum `uttt | ttt3 | backup`, split truly common fields (`id`, `game`, `consumed_by`, optional `notes`) from game-specific payloads, and define one exact backup shape. Recommended: `a`, `b`, `h`, `expected_T`, and `expected_critical_bid` as rational strings directly on the backup fixture, with no canonical board/forced/budgets state. Add backup to the category list and state explicitly that this pre-publication extension does not reinterpret canonical UTTT/ttt3 rules.

### MR17 — Resolve the backup fixture's downstream-consumer boundary before publication

DD line 146 says backup fixtures are consumed by engine backup-operator tests, but the current engine rules plan and landed reader consume only `game: uttt` rules fixtures; search backup math is explicitly deferred to a theory-gated successor engine design. The addressed design relay only CCs the orchestrator, and the staged plan defers the formal flag until Task 13 after fixture creation, which is too late for a new shared-contract branch labeled `consumed_by: engine`.

Name the actual consumer as the successor engine-search design and require an addressed pre-publication routing/acknowledgment before emitting `consumed_by: engine`; otherwise publish the backup cases as theory-only evidence and defer the engine tag. Record the no-consumer action explicitly. This does not require the operator to ratify P3 early.

### MR18 — Replace the canonical-terminal misnomer and complete the GRILL_SOURCE

C3 currently calls the synthetic children “chip-comparison terminals at assigned thresholds.” Canonical chip comparison has only the 1/2 threshold, so that phrase conflicts with C5 and the fixed-rule boundary. Call them synthetic exact threshold-payoff terminals and give the integer predicate; state that the harness validates the branch algebra, not reachability or the canonical rules kernel.

Add the second-hold census, the synthetic-matrix feasibility check, and the fixture-consumer inspection to GRILL_SOURCE `code/docs inspected` and `questions answered from codebase`. D7 already records the decision, but the mandatory grill record does not yet record how these revision-5 questions were resolved.

## Preserved decisions and boundaries

- C11: keep the complete zero-zugzwang census and `a=b` witness regression.
- C3: keep the one-step proof obligation, both abstract unit cases, and the synthetic exact matrix harness; the E2 grill found its proposed cases feasible.
- P3: keep operator ratification at the math lock with the UTTT no-reachable-witness gap named explicitly.
- P1/P2, C8/C8a, C9 hard stop, N=24 orchestrator gate, D1–D6, forced integer-or-null encoding, and the two Richman conventions remain closed and unchanged.
- No source/test/design/plan edits were made by theory.implementer.

## Successor acceptance

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh digest, and `IN_REPLY_TO` naming this review. Preserve every unrelated revision-5 byte. On exact closure of MR15–MR18, the design is approvable for successor PLAN review.

Plan preview only, not a PLAN-REVIEW verdict: before issuing the successor PLAN carrier, also remove its stale “named zugzwang state” interface comment at lines 550–553, make line 602's envelope agree with `game: backup`, advance the findings-reference test at line 624 to C1–C11/P1–P3, and update stale commit subjects. Task 9's target-union wording may retain the zugzwang selector only if it explicitly records that C11 makes that subset empty.

## Evidence

- Routed integrity: SHA-256 `661feacd56bc089ca0ada464e013e74bb0d57c59fb82a99fe6266689daba213b` matches the addressed relay.
- Exact incoming relay lint with freshness disabled: clean. Root lint remains red from inherited INDEX chronology plus the now-superseded PLAN digest in the prior blocker; neither condition changes this exact-file verdict or grants implementation authority.
- `git diff --check` for the current design and staged plan bytes: clean.
- Current engine consumer: `.worktrees/engine-c1-impl/engine/tests/test_fixtures.cpp:111-143` filters for schema v1 plus `game == "uttt"`; `PL-engine-rules-c1-20260825.md:720` scopes threshold and auction-trace categories out of the rules-core implementation, and the successor search design remains theory-gated.
- Implementation worktree remains clean at `theory/stage1-c1@f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay submitted through daemon admission; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-fold-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-fold-report-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-report-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-7/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-blocker-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-7/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-review-7/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-review-3/
