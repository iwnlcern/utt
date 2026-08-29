ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-plan-review-4
PARENT_DISPATCH_ID: s2-theory-c2-plan-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan approved; the Pair Planner may separately evaluate delegated conditional dispatch after mechanical SCOPE_DIFF all-in, while merge remains operator/orchestrator gated
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-pair-planner-20260828-154729.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 4558eeda83cdc6e94b2bf45e93161856b7ab1cda6ac53706926504103d4bd5f7
PLAN_REVIEW_VERDICT: approve
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@d305625
TARGET_BRANCH: main
SUBJECT: PLAN-REVIEW theory-c2 rev4 — approve locked implementation plan

## Scope and identity

Reviewed the exact 1,026-line revision-4 `PL-theory-c2-20260828` bytes at SHA-256 `4558eeda83cdc6e94b2bf45e93161856b7ab1cda6ac53706926504103d4bd5f7`. The plan is the sole artifact amended by current-main commit `d3056259101796cceaabea502e6c7ef8c3d4d84b`, whose parent is reviewed rev3 commit `68a6b278d3f6d98fdd2f5f3daf77d779459e2671`; `git diff --check` passes. The PLAN relay is addressed to this seat, re-parents to approving design review `s2-theory-c2-design-review-4`, and carries round-3 review `s2-theory-c2-plan-review-3` in `IN_REPLY_TO`.

No implementation, source, test, fixture, design, plan, branch, commit, PR, merge, publication, or deployment action was taken by this seat.

## Verdict

PLAN_REVIEW_VERDICT: approve

Revision 4 exactly folds MR-8: both the Task 3 normative prose and acceptance criterion 3 now scope completeness to theory-consumable rows, explicitly skip non-theory rows, and remain byte-consistent with `_rows()` and `test_matrix_covers_every_consumable_fixture_exactly_once`. The two-line documentation replacement changes no code snippet and preserves every accepted rev2/rev3 repair.

The full locked plan is executable within the approved design and theory-c2 boundary:

- deterministic emitter-side `certschema` package and generated corpus;
- exact Appendix B golden bytes and XXH3-128 identity;
- concrete fixtures-v1 compatibility/exclusion matrix with non-vacuous s2 assertions;
- positive P1/P2/P3 and verdict constructions;
- 42 named mutants plus GATE rows with first-failure isolation and resolvable later-step verdict members;
- regeneration determinism, exact catalogue census, full-suite verification, explicit task staging, and no checker/fixture/DD edits.

## Acceptance-criteria disposition

1. Golden certificate/verdict byte equality: APPROVE at plan level — exact constants and builder tests are named.
2. Generator determinism: APPROVE at plan level — fresh-versus-committed artifact comparison is complete.
3. Rules conformance: APPROVE — compatible and excluded theory-consumable rows are explicit; non-theory rows are skipped; the actual forced-closed exclusion and auction/s2 terminal differences are asserted.
4. Corpus completeness: APPROVE — 42 mutant ids and both GATE rows are enumerated with deterministic catalogue coverage.
5. Full suite and scope fence: APPROVE — task file lists, explicit staging commands, final suite, and status check are present.

## Boundary contract

Writes: deterministic certificate/verdict positive vectors, mutant corpus, and `catalogue.json` under `theory/certschema/`.

Reads: approved DD revision 3 and frozen fixtures-v1 theory-consumable rows.

Target entity: shared conformance corpus consumed independently by the future theory-c3 checker and solver emitter.

Downstream consumer: theory-c3 checker and solver certificate emitter.

Contract: DD-pinned schema/ruleset/symmetry/digests, canonical 37-byte states, exact member-claim/path semantics, catalogue ids, and deterministic file bytes.

Proof: E2 named tests for golden equality, fixtures compatibility, positive constructions, mutant/catalogue census, and fresh regeneration.

No-consumer action: not applicable — both downstream consumers are commissioned; checker implementation remains explicitly out of scope here.

## Authority and next gate

This approval is PLAN-REVIEW only. It authorizes no implementation by itself. The Pair Planner may issue a distinct, correctly parented implementation relay only after the delegated conditions from `s2-theory-c2-proceed-2` are mechanically satisfied, including `SCOPE_DIFF_RESULT: all-in`, and only a live bare own-line dispatch token in that addressed relay starts implementation. Merge, publication, deployment, and release remain separate gates.

OPERATOR_ITEMS: none — no product-semantic ambiguity, scope expansion, or accepted residual risk remains in the reviewed plan.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only plan review
- migration/backfill/destructive-write/canonical-data-repair: no — no data actions
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof test corpus only
- AI-or-automation-acts-downstream: no — generated artifacts are test/conformance inputs, not acting automation
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — corpus is shared truth for checker and emitter; large ceremony and boundary contract retained
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — emitter tooling and checker implementation remain independent by locked design
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — plan scope and semantics are explicit; no risk waiver used
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming relay exact-file lint passes; PLAN/DESIGN digests equal exact artifacts; `HEAD == main == d305625`.
- E1: rev4 changes only the plan, exactly 2 insertions/2 deletions; prospective diff check passes; INDEX carries the plan-4 row with the correct design-review parent.
- E2: focused consumer probe confirms 15 current theory-consumable rows, skips a synthetic engine-only row, and confirms the actual excluded legality row is closed and ill-formed.
- E2: plan scan confirms MR-8 qualification in normative prose and acceptance, zero `missing.utc`, and only the intended valid/absent verdict member names.
- E2: `cd theory && uv run pytest -q` — 99 passed in 95.24s.

ACTIONS_GIT_REF: no implementation/source/test/fixture/design/plan/branch/commit/PR/merge action claimed; authored only this approval draft for daemon filing against current-main plan commit d3056259101796cceaabea502e6c7ef8c3d4d84b
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
