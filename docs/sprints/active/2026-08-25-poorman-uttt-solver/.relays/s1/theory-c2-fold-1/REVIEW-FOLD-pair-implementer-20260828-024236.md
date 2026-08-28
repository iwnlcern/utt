ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: theory-c2-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — F1 is a bounded test pin plus its required local mutation proof on the existing PR branch; merge remains separately gated
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c2-fold-1/REVIEW-FOLD-pair-implementer-20260828-024003.md
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
SUBJECT: CORRECTED PRE-EDIT FOLD SCOPE — include F1 local mutation target before mutation

## Scope correction before branch edits

No implementation-worktree file has been edited since the initial scope artifact. The requested F1 evidence requires the new report-path test to fail against a deliberately broken local mutation. Under the path-total scope rule, that temporary mutation means `theory/auction_ttt/knife_edge.py` must be admitted before it is touched even though it will be restored byte-for-byte and excluded from the fold commit. This successor corrects the scope before either file is edited.

F1 is accepted. O1 and O2 remain deferred for the reasons recorded in the preceding artifact: their current bytes have independent corroboration and neither is required to close the mandatory report-path retention pin.

FOLD_SCOPE:
- theory/tests/test_knife_edge.py -> in
- theory/auction_ttt/knife_edge.py -> in
FOLD_SCOPE_RESULT: all-in

Scope mapping:
- `theory/tests/test_knife_edge.py` receives the sole committed change: the weakref report-path regression test.
- `theory/auction_ttt/knife_edge.py` is admitted only for the required deliberate materialization mutation used to obtain RED. It must be restored to the exact pre-fold bytes before GREEN, staging, and commit.

Out of scope remains unchanged: artifact generation, `theory/results/knife-edge-sweep.json`, `theory/results/EVIDENCE.md`, fixtures, engine, harness, UI, dependencies, design, plan, merge, deployment, and release. Any newly required path stops the fold before that path is edited.

Boundary contract: no committed production or cross-owner contract changes. The test exercises the real `report(...)` spot path with local solver doubles and observes object lifetime via weak references; the final commit changes tests only.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local theory test and reversible mutation only
- migration/backfill/destructive-write/canonical-data-repair: no — no data writes or regeneration
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game analysis test only
- AI-or-automation-acts-downstream: no — no external action
- worker/scheduler/queue/retry/async-side-effect: no — synchronous local test
- cross-repo/service-contract/generated-schema/shared-API-event: no — final production and fixture contracts remain unchanged
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — the test enters the real report aggregation path; the mutation directly breaks that consumption seam
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — both paths are required by the directed RED/GREEN proof and final committed scope remains one test file
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none

ACTIONS_GIT_REF: no implementation edits claimed; theory/c2@e63fc3851ef54981fb1470fd6c518713204b1db7; final linked-worktree git status --short = none — clean tree
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree

Next action: add the F1 test, temporarily mutate `report(...)` to retain spot points through accumulator finish and capture the expected weakref failure, restore `knife_edge.py` byte-for-byte, run clean GREEN and the full suite, confirm protected artifacts unchanged, then commit and push only the test file. Merge remains separately gated.
