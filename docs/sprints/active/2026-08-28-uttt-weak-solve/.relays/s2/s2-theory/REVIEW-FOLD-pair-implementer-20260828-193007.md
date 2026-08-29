ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c2-fold-scope-2
PARENT_DISPATCH_ID: s2-theory-c2-fold-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — all planned edits are directly named by the addressed must-fix findings; merge remains separately gated
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/REVIEW-FOLD-pair-planner-20260828-192509.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c2-20260828
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: theory/certschema-c2@45994d31f2b2ca09b08b13352a2a2644eb427663
TARGET_BRANCH: main
BRANCH: theory/certschema-c2
PR: https://github.com/iwnlcern/utt/pull/12
SUBJECT: PRE-EDIT FOLD SCOPE — theory-c2 MF-1 isolation armor and MF-2 converse kind/terminality mutant

## Accepted findings

This scope accepts MF-1 and MF-2 from the addressed successor fold direction. The strongly recommended and discretionary optionals are deferred: no optional-only artifact or behavior is included. Read-only inspection confirmed the three stated test escape routes at PR head `45994d31f2b2ca09b08b13352a2a2644eb427663` and confirmed that the p1 terminal record can be changed to OPPONENT while remaining side-to-move consistent, isolating the converse terminality/kind check. No branch bytes have been edited before this artifact.

FOLD_SCOPE:
- theory/certschema/vectors.py -> in
- theory/certschema/catalogue.json -> in
- theory/certschema/mutants/MUT-R21.bin -> in
- theory/tests/test_certschema_mutants.py -> in
FOLD_SCOPE_RESULT: all-in

Boundary contract: the fold changes only the dispatched certificate-corpus generator, its deterministic generated catalogue/new mutant, and the isolation regression tests. The locked DD, generator framework, existing checked-in vectors and mutants, fixtures, checker implementation, solver, UI, merge, deployment, and release remain out of scope.

Verification contract: RED/GREEN for MF-1 and MF-2; deterministic regeneration with clean second pass; exact generated-artifact census; targeted certificate-schema tests; full theory suite; repository full suite from `referee/`; `git diff --check`; exact four-path worktree scope before commit; clean worktree after commit and push.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no credential or permission surface
- migration/backfill/destructive-write/canonical-data-repair: no — deterministic local test corpus only
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game/certificate state only
- AI-or-automation-acts-downstream: yes — the corpus constrains a downstream automated certificate checker
- worker/scheduler/queue/retry/async-side-effect: no — no asynchronous runtime behavior
- cross-repo/service-contract/generated-schema/shared-API-event: yes — generated certificate artifacts implement the existing locked cross-pair contract
- user-visible-control-with-materializer/downstream-consumer: no — no UI or deployed consumer
- test-runtime-role-mismatch: no — checked-in bytes and parser-level target fields are exercised directly
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — must-fixes and four paths are explicit; E2 is required
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none — pre-edit scope artifact; no branch edits made
FINAL_GIT_STATUS_SHORT: none — clean tree
