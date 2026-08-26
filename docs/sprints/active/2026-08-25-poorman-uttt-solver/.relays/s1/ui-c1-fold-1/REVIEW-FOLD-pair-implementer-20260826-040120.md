ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: ui-c1-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — ui.planner must admit ui/test/acceptance.test.ts in a corrected all-in fold scope or rule an alternative before that path can be touched
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-fold-1/REVIEW-FOLD-pair-implementer-20260826-034436.md
DESIGN_LOCK_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
PLAN_SHA256: d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a
BUNDLE_ID: ui-c1
OWNER: ui
REPO: /Users/jack/Programming/utt
BASE: ui-c1-impl@9af5dcbf9afa8d9921bb287ead4bde456bcbf167
TARGET_BRANCH: main
BRANCH: ui-c1-impl
PR: https://github.com/iwnlcern/utt/pull/4
SUBJECT: FOLD DEVIATION — composed acceptance test is stale but outside admitted scope

## Confirmed deviation

The must-fix 5 component RED/GREEN fold correctly leaves logged `t`, `lo`, and `hi` meaningful as percentages when both budgets are zero. Its task review found the composed acceptance test still requires the superseded `T: n/a` and interval-n/a copy and explicitly rejects the design-owner percentages.

Fresh E2 reproduction at `ui-c1-impl@9af5dcbf9afa8d9921bb287ead4bde456bcbf167`:

- Command: from `ui/`, `npm test -- --run test/acceptance.test.ts`.
- Result: 54 passed, 1 failed.
- Failure: `UI v1 composed acceptance > renders complete both-zero metric totality through the composed GameView surface` at `ui/test/acceptance.test.ts:354` expected the old interval-n/a copy, while the rendered output was `T: 62.50%` and `interval [50.00%, 75.00%]` with `p`, margin, and critical-bid conversion still n/a.
- The stale expectation path was not listed in the admitted pre-edit scope. It remains unmodified.

FOLD_SCOPE:
- ui/src/components/Timeline.tsx -> in
- ui/test/components/timeline.test.tsx -> in
- ui/src/app/GameView.tsx -> in
- ui/src/app/cursorReducer.ts -> in
- ui/test/app/cursor-reducer.test.ts -> in
- ui/test/app/game-view.test.tsx -> in
- ui/src/components/MetricsPanel.tsx -> in
- ui/test/components/metrics-panel.test.tsx -> in
- ui/src/replay/derive.ts -> in
- ui/test/acceptance.test.ts -> out
FOLD_SCOPE_RESULT: deviation-present

No edit has been made to the OUT path. The branch retains three already-reviewed in-scope commits: `5cfa10e` timeline semantics, `b748575` single reducer and pin-gated legend, and `9af5dcb` both-zero logged shares. Directed optional 7 remains pending because discovery of the OUT path stops the fold.

Requested correction: issue a successor scope that marks `ui/test/acceptance.test.ts -> in` for the narrow expectation update, or rule a different in-scope resolution. After a corrected all-in artifact, the Implementer will update the composed test RED/GREEN, finish the pending comment-only optional, run the full verification battery, and return the fold report.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static replay UI only
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent canonical data writes
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game-log display state only
- AI-or-automation-acts-downstream: no — replay rendering performs no downstream action
- worker/scheduler/queue/retry/async-side-effect: no — no worker or async side-effect change
- cross-repo/service-contract/generated-schema/shared-API-event: no — existing log/protocol contracts are consumed unchanged
- user-visible-control-with-materializer/downstream-consumer: no — controls affect only in-memory replay presentation
- test-runtime-role-mismatch: no — the composed jsdom test directly reproduces the stale expectation
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — adding an unlisted test file requires an owning Planner scope correction
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none — no edits made under this deviation relay
FINAL_GIT_STATUS_SHORT: none — clean linked worktree

Next requested action: ui.planner file a corrected all-in fold-scope successor admitting the composed acceptance-test expectation update, or rule an alternative. Merge remains separately gated.
