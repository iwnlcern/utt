ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: theory-c2-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — F1 is a bounded test-only pin on the existing open PR branch; merge remains separately gated
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c2-fold-1/REVIEW-FOLD-planner-20260828-022012.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c2-20260827
PLAN_ARTIFACT: PL-theory-c2-20260827
PLAN_SHA256: 713d726191d0a5214047ccb76a93626f6ee431cd78b7958bff9de86cb8ff11fa
BUNDLE_ID: theory-c2
OWNER: theory
REPO: /Users/jack/Programming/utt
BASE: main@dce510ef5a8be18638fd6dc4e5718e67941a7a56
TARGET_BRANCH: main
BRANCH: theory/c2 @ e63fc3851ef54981fb1470fd6c518713204b1db7
PR: https://github.com/iwnlcern/utt/pull/11 — OPEN and UNMERGED at e63fc3851ef54981fb1470fd6c518713204b1db7
SUBJECT: PRE-EDIT FOLD SCOPE — F1 report-path KnifePoint retention pin in one test file

## Accepted finding

F1 is technically sound against the current implementation. `report(...)` currently streams targeted points into one `_ScaleFold` accumulator per scale, but the existing weakref lifecycle test calls `spot_sweep(...)` directly. The fold will add a report-path weakref regression test that proves a produced `KnifePoint` is collectable before `report(...)` returns. Because the compliant implementation already streams, RED will be demonstrated only against a deliberately broken local mutation that materializes the spot stream; that mutation will be removed before the clean GREEN run and commit.

O1 is deferred: the panel independently reproduced all stream digests, so a content assertion is useful future hardening but not required to close F1. O2 is deferred: exhaustive missing-mask behavior is already implemented and the panel classed the absent test as low risk. Neither optional changes the admitted file set, but this fold stays limited to the mandatory report-path pin.

FOLD_SCOPE:
- theory/tests/test_knife_edge.py -> in
FOLD_SCOPE_RESULT: all-in

Out of scope remains unchanged: production code, artifact generation, `theory/results/knife-edge-sweep.json`, `theory/results/EVIDENCE.md`, fixtures, engine, harness, UI, dependencies, design, plan, merge, deployment, and release. Any newly required file stops the fold before that file is edited.

Boundary contract: no production or cross-owner contract changes. The test exercises the real `report(...)` spot path with local solver doubles and observes object lifetime via weak references; committed report behavior and artifact bytes remain unchanged.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local theory test only
- migration/backfill/destructive-write/canonical-data-repair: no — no data writes or regeneration
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game analysis test only
- AI-or-automation-acts-downstream: no — no external action
- worker/scheduler/queue/retry/async-side-effect: no — synchronous local test
- cross-repo/service-contract/generated-schema/shared-API-event: no — production and fixture contracts remain unchanged
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — the test enters the real `report(...)` aggregation path and replaces only expensive solver boundaries
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — one named mandatory test pin; operator-judgment evidence remains unchanged
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none

ACTIONS_GIT_REF: no implementation edits claimed; theory/c2@e63fc3851ef54981fb1470fd6c518713204b1db7; final linked-worktree git status --short = none — clean tree
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree

Next action: add only the admitted F1 test, prove RED against a deliberately broken local report mutation, restore the compliant implementation, prove GREEN and the fresh full suite, confirm the two protected artifacts are byte-identical, commit and push the existing PR branch, then file the REVIEW-FOLD report. Merge remains separately gated.
