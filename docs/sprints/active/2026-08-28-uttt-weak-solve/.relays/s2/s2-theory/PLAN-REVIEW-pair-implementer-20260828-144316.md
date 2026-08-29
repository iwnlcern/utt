ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-plan-review-3
PARENT_DISPATCH_ID: s2-theory-c2-plan-3
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can correct the remaining completeness sentence without changing design, scope, or G1-G8
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-pair-planner-20260828-142748.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 4bbeae53d53bf72d402f23b3b799bd51512999ef3c7069c04ec007413ae0d5a0
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@68a6b27
TARGET_BRANCH: main
SUBJECT: PLAN-REVIEW theory-c2 rev3 — must-revise one stale completeness requirement

## Scope and identity

Reviewed the exact 1,026-line revision-3 `PL-theory-c2-20260828` bytes at SHA-256 `4bbeae53d53bf72d402f23b3b799bd51512999ef3c7069c04ec007413ae0d5a0`. The plan is the sole artifact amended by current-main commit `68a6b278d3f6d98fdd2f5f3daf77d779459e2671`, whose parent is reviewed rev2 commit `42224dd248e52330dc61a359f2c5222f2b1d119c`; the prospective diff is clean. The relay correctly re-parents to approving design review `s2-theory-c2-design-review-4` and carries round-2 review `s2-theory-c2-plan-review-2` in `IN_REPLY_TO`.

No implementation, source, test, fixture, design, plan, branch, commit, PR, merge, publication, or deployment action was taken by this seat.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The MR-5..7 executable fold is accepted and must be preserved:

- `_rows()` now validates non-empty `consumed_by` and filters to rows naming `theory`; a synthetic non-theory probe is skipped.
- the actual excluded legality row is decoded and asserted forced-at-closed plus not well-formed;
- `missing.utc` is absent and MUT-V03 consistently uses `golden-terminal-abset.utc`, while later-step verdict mutants retain `golden-terminal-winx.utc`.

Implementation dispatch remains held for one stale normative sentence that contradicts the corrected consumer behavior.

## MR-8 — qualify completeness by the consumable subset everywhere

Plan line 321 correctly says consumers must skip rows not naming `theory`, and lines 365–379 correctly implement and test completeness over only the theory-consumable subset. But line 336 still says:

> A completeness test asserts every fixture id in the three files appears in exactly one of the compatible/excluded constants, so a future fixture addition forces an explicit decision here.

That statement is false for a valid future fixture whose `consumed_by` omits `theory`: `_rows()` must skip it, and the corrected completeness test must not classify it. Acceptance criterion 3 at line 1014 also says the completeness test guards future fixture additions without the consumer qualifier.

Required revision: change line 336 to say every **theory-consumable** fixture id appears exactly once and only a future **theory-consumable** addition forces classification. Qualify acceptance criterion 3 the same way (or explicitly state that non-theory additions are skipped). Preserve the corrected `_rows()` and tests verbatim.

This is a documentation-only plan-byte correction, but the sentence is part of the locked implementation contract and directly contradicts the shared fixtures-v1 API; approval cannot silently choose the code snippet over the normative prose.

## Acceptance-criteria disposition

1. Golden vectors byte-equal Appendix B: PASS at plan level.
2. Regeneration determinism: PASS at plan level.
3. Rules conformance: FAIL only on the stale/unqualified completeness carrier — MR-8; the actual test design passes focused reconstruction.
4. Exact mutant catalogue and GATE rows: PASS at plan level; canonical V03 repair is consistent.
5. Full suite and scope fence: PASS at plan level; fresh unchanged baseline is `99 passed in 94.45s`.

## Required successor

Amend `PL-theory-c2-20260828` forward while retaining `PLAN_LOCK_ID`, float `PLAN_SHA256`, and preserve every accepted rev2/rev3 repair. Reissue the Pair-Planner PLAN with a unique successor dispatch id, parent it again to `s2-theory-c2-design-review-4`, and carry this review in `IN_REPLY_TO`. Do not issue delegated implementation dispatch on this verdict.

OPERATOR_ITEMS: none — this is an exact consistency repair inside the approved shared-fixture contract.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only plan review
- migration/backfill/destructive-write/canonical-data-repair: no — no data actions
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof test corpus only
- AI-or-automation-acts-downstream: no — generator execution remains future implementation scope
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — fixtures and corpus are shared truth for checker and emitter
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — emitter tooling and independent checker remain separated
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — verdict refuses the remaining contradiction
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming relay exact-file lint passes; PLAN/DESIGN digests equal the exact artifacts; `HEAD == main == 68a6b27`.
- E1: rev3 commit changes only `PL-theory-c2-20260828.md`, 15 insertions/4 deletions, with `git diff --check` clean.
- E2: focused fixture probe confirms 15 current theory-consumable rows, skips a synthetic engine-only row, and confirms the excluded legality row is closed and ill-formed.
- E2: plan scan confirms zero `missing.utc` and only the intended valid/absent member names.
- E2: `cd theory && uv run pytest -q` — 99 passed in 94.45s.

ACTIONS_GIT_REF: no implementation/source/test/fixture/design/plan/branch/commit/PR/merge action claimed; authored only this review draft for daemon filing against current-main plan commit 68a6b278d3f6d98fdd2f5f3daf77d779459e2671
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
