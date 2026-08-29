ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-plan-review-2
PARENT_DISPATCH_ID: s2-theory-c2-plan-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can make the remaining test-contract corrections without changing the approved design or G1-G8
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-pair-planner-20260828-134929.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 362647d8a617aad909b5c70764b9a476c555631e2e74ec7506caa7a52e0ddd0b
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@42224dd
TARGET_BRANCH: main
SUBJECT: PLAN-REVIEW theory-c2 rev2 — must-revise residual fixture consumer and V03 contracts

## Scope and identity

Reviewed the exact 1,015-line revision-2 `PL-theory-c2-20260828` bytes at SHA-256 `362647d8a617aad909b5c70764b9a476c555631e2e74ec7506caa7a52e0ddd0b`. The plan is the sole artifact amended by current-main commit `42224dd248e52330dc61a359f2c5222f2b1d119c`; `git diff --check 709dbca..42224dd` passes. The relay correctly re-parents the PLAN to approving design review `s2-theory-c2-design-review-4`, pins design SHA-256 `629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c`, and carries the preceding must-revise review in `IN_REPLY_TO`.

No implementation, source, test, fixture, design, plan, branch, commit, PR, merge, publication, or deployment action was taken by this seat.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

Revision 2 materially folds the prior MR-1..4: the fixture envelope and compatibility matrix are now concrete; the verdict-mutant directory gains an adjacent valid member; V02 is rebuilt at the correct length; V07 uses a valid alternate root; H10/R14/R11 have single intended constructions; and Tasks 3/6 explicitly stage their new tests. Those accepted repairs should be preserved.

Implementation dispatch remains held because Task 3 still contradicts the normative fixtures-v1 consumer contract and does not prove the stated exclusion for the actual legality fixture, while Task 6 gives two different absent-member paths for MUT-V03.

## MR-5 — honor `consumed_by` by filtering, not asserting universal membership

Plan line 321 correctly states the normative `theory/fixtures/SCHEMA.md` rule: consumers MUST skip rows not naming themselves. But `_rows()` at lines 365–368 does this instead:

```python
rows = json.loads((FIX / name).read_text())["fixtures"]
assert all("theory" in r["consumed_by"] for r in rows)
return rows
```

That is not consumer filtering. It makes a valid non-theory fixture fail the theory suite, and the completeness assertions then require theory to classify a row it is required to skip. All current rows happen to name `theory`, so this defect is latent in today's frozen bytes; it is still a direct contradiction of the shared fixture API and of the incoming relay's claim that `consumed_by` is honored.

Required revision: make `_rows()` return only rows with `"theory" in consumed_by`; make completeness range over that consumable subset. If the plan also wants a schema sanity check, validate that `consumed_by` is present/non-empty without requiring universal theory membership.

## MR-6 — assert the actual excluded legality row's stated s2 disposition

The matrix says `legality-forced-closed-means-free-choice` is EXCLUDED because its `forced=4` names a closed board and therefore the state is ill-formed under DD §2.6. The test suite only puts its id in `EXCLUDED_LEGALITY`; it never decodes that fixture and asserts `not state.well_formed()`.

The direct replacement at lines 405–412 checks reachable closure-routing children and does not prove the stated property of the excluded legality row. Independent decoding of the actual fixture confirms board 4 is closed and the row advertises six auction-profile free-choice moves, so the missing assertion is non-vacuous.

Required revision: add a test over `EXCLUDED_LEGALITY` which loads the actual row and asserts the documented s2 disposition (`not _state(fx["state"]).well_formed()`). Keep the direct reachable-child/ANY coverage as a separate replacement-semantics test. This makes acceptance criterion 3's claim that every excluded row is asserted true for the legality exclusion as well as the three terminal exclusions.

## MR-7 — give MUT-V03 one absent target name

Task 6 mutation rule 3 (line 791) directs MUT-V03 to `"missing.utc"`. The normative mutant table (line 833), Task 7 emitted-artifact test (lines 950–951), and incoming relay instead use `"golden-terminal-abset.utc"`.

Both names can be syntactically valid and absent, but a locked implementation plan cannot prescribe both. Following the general rule makes the Task 7 test fail; following the table/test violates the general implementation rule.

Required revision: choose one spelling everywhere. The already-specific table, incoming relay, and Task 7 test consistently favor `golden-terminal-abset.utc`; retain that unless intentionally changing all three surfaces. Preserve the adjacent valid `golden-terminal-winx.utc` strategy for V04/V05/V07/V08.

## Acceptance-criteria disposition

1. Golden vectors byte-equal Appendix B: PASS at plan level.
2. Regeneration determinism: PASS as an approach, including the adjacent verdict-member artifact.
3. Rules conformance against consumable fixtures-v1: FAIL — MR-5/MR-6.
4. Exact 42-mutant catalogue plus GATE rows: FAIL as a single executable instruction set — MR-7. The prior V02/V07/H10/R14 construction defects are otherwise folded.
5. Full theory suite and scope fence: PASS at plan level; explicit staging repairs are present. The unchanged pre-review theory baseline remains `99 passed in 96.68s`.

## Required successor

Amend `PL-theory-c2-20260828` forward while retaining `PLAN_LOCK_ID`, float `PLAN_SHA256`, and preserve every accepted rev2 repair. Reissue the Pair-Planner PLAN with a unique successor dispatch id, parent it again to `s2-theory-c2-design-review-4`, and carry this review in `IN_REPLY_TO`. Do not issue delegated implementation dispatch on this verdict.

OPERATOR_ITEMS: none — all three corrections make the approved shared-test contract internally executable; they do not alter schema bytes, DD semantics, G1-G8, or scope.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only plan review
- migration/backfill/destructive-write/canonical-data-repair: no — no data actions
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof test corpus only
- AI-or-automation-acts-downstream: no — generator execution remains future implementation scope
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — fixtures and corpus are shared truth for theory checker and solver emitter
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — emitter tooling and future independent checker roles remain explicit
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — verdict refuses rather than accepts the residual contradictions
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming PLAN relay exact-file lint passes; relay PLAN/DESIGN digests equal the exact artifacts.
- E1: `HEAD == main == 42224dd248e52330dc61a359f2c5222f2b1d119c`; the commit changes only the plan; prospective diff check passes.
- E1: lineage, `TO`, `CC`, phase, authority, design-review parent, and prior-review reply pointer inspected literally.
- E2: all 15 current rows in legality/closure_routing/terminal name `theory`, proving why the incorrect universal assertion is currently latent rather than green evidence that filtering was implemented.
- E2: the excluded legality fixture decodes with `forced=4`, closed board 4, and six fixture-profile expected moves; no Task 3 test asserts that actual row is ill-formed under s2.
- E2: exact-plan search finds `missing.utc` at line 791 and `golden-terminal-abset.utc` at lines 833 and 951.

ACTIONS_GIT_REF: no implementation/source/test/fixture/design/plan/branch/commit/PR/merge action claimed; authored only this review draft for daemon filing against current-main plan commit 42224dd248e52330dc61a359f2c5222f2b1d119c
FINAL_GIT_STATUS_SHORT: inherited root contains `.claude/scheduled_tasks.lock`, daemon-rendered `INDEX.md`, and prior daemon-rendered relay files as uncommitted noise; no product or plan bytes changed by this review
