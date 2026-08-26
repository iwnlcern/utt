ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c2-plan-review-6
PARENT_DISPATCH_ID: harness-c2-plan-5
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no - the pair Planner can correct the reissue threading header without a fresh ruling; implementation remains held
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-plan-5/PLAN-planner-20260826-021545.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
PLAN_SHA256: d2801b4900d29de55643b396a8d14d67287e0e11eb9b6ecbbaf86f4000ae19cd
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c2
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: main@4fe2709a83ead0e8694ec2710addd8cb9929815a observed during review; dispatch-time rebase remains required
TARGET_BRANCH: main
BRANCH: none - PLAN-REVIEW is read-only
SUBJECT: PLAN-REVIEW must-revise - substantive plan accepted; reissue threading edge must point to review-5

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The plan's implementation content is accepted. The addressed reset ruling closes MR2-A under its five conditions, and MR2-C now preserves literal argv paths and isolates both generator modes. One carrier-only defect remains: plan-5's IN_REPLY_TO points to the ruling instead of the must-revise PLAN-REVIEW that this reissue answers. Implementation dispatch remains held until a plan-6 successor corrects that edge.

## Required revision - reissue threading

Protocol requires a reissued gated design-doc PLAN to keep PARENT_DISPATCH_ID on the approving DESIGN-REVIEW and carry the must-revise review it answers in IN_REPLY_TO. Plan-5 correctly keeps PARENT_DISPATCH_ID: harness-c1-design-review-4, but line 15 sets IN_REPLY_TO to harness-c2-reset-ruling-1/RECONCILE-orchestrator-planner-20260825-222434.md.

Reissue with:

IN_REPLY_TO: harness-c2-plan-review-5/PLAN-REVIEW-pair-implementer-20260825-221146.md

Keep harness-c2-reset-ruling-1 cited in the body and reset contract. The plan artifact need not change for this header-only correction; carry the same PLAN_LOCK_ID and d2801b49 digest if its bytes stay unchanged.

## Accepted substantive closure

- The addressed governing-seat ruling explicitly authorizes the one-time nineteen-log elapsed-field reset and binds reversibility, field-scoped diff, stream-invariance, one-time/no-precedent, and plan-citation conditions.
- Both bare and check modes generate under a temporary repo-shaped cwd, with exact path-preserving script copies derived from manifest argv.
- Bare mode promotes only the expected allowlist after success; check mode promotes nothing; committed fixtures are never transient engine state.
- The unchanged-argv execution test, tree-hash check, independent manifest mutation test, exact inventory, literal sidecars, reader/canonical-byte proof, delivery/sink rules, theory-seam STOP, and separate merge gate are accepted.
- No additional human judgment is open after harness-c2-reset-ruling-1; the trigger remains authorized only for the enumerated reset.

## Forward correction of review-5 carrier

The immutable rendered review-5 contains a stale duplicated tail at lines 104-134 after its complete first ending at line 103. That duplication resulted from this Implementer seat's draft-materialization defect and repeats the same must-revise disposition; it adds no contrary verdict or authority. This review records the defect forward and is the operative response to plan-5. Do not rewrite review-5.

## Evidence

- Incoming relay SHA256 c2b17a834b21b7e2db322d86a984760aa983fa0e27e6f73ea1182c1c3067ffc3.
- Plan SHA256 d2801b4900d29de55643b396a8d14d67287e0e11eb9b6ecbbaf86f4000ae19cd; design SHA256 c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e.
- Ruling SHA256 d726e5aac7eebf760806c6f348945451bc5b802d6734b8a152223f5b01652275; exact-file lint clean.
- Incoming exact-file and selected-file root-context lint are clean. Whole-root lint remains red from inherited INDEX inversions and two immutable historical merge-claim carriers.
- Current theory/fixtures census is empty, so Task 4 remains on its no-edit branch unless dispatch-time main changes.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no - local fixture/runtime plan review
- migration/backfill/destructive-write/canonical-data-repair: yes - authorized one-time canonical corpus reset under harness-c2-reset-ruling-1
- money/inventory/orders/planning/accounting/trust-critical-state: no - simulated budgets only
- AI-or-automation-acts-downstream: yes - referee drives subprocess engines
- worker/scheduler/queue/retry/async-side-effect: yes - subprocess recovery and marker effects isolated in temp
- cross-repo/service-contract/generated-schema/shared-API-event: yes - raw-byte corpus consumed by engine-c2
- user-visible-control-with-materializer/downstream-consumer: no - no deployed UI in scope
- test-runtime-role-mismatch: no - merged referee surfaces inspected
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no - reset scope and conditions are addressed by the ruling; no live-verify skip
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no - no ceremony downgrade requested; governing-seat ruling closes the enumerated trigger
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none - review-only; no source, test, design, plan, branch, commit, PR, or integration action; only this daemon draft/admission is authored
FINAL_GIT_STATUS_SHORT:
?? .pr-body-engine-c2.md

Next requested action: reissue harness-c2-plan-6 with IN_REPLY_TO corrected to review-5 and unchanged accepted plan bytes, then request PLAN-REVIEW. Do not dispatch implementation from this verdict.
