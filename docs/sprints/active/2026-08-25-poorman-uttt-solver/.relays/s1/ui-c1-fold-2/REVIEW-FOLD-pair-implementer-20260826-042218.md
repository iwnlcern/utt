ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: ui-c1-fold-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — Planner admitted the one omitted composed acceptance path and all remaining fold paths are now in scope; merge remains separately gated
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-fold-2/REVIEW-FOLD-planner-20260826-041751.md
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
SUBJECT: CORRECTED PRE-EDIT FOLD SCOPE — composed acceptance path admitted; ten paths all in

## Corrected pre-edit scope

The Planner's fold-2 successor cures the sole deviation by admitting `ui/test/acceptance.test.ts` only for the must-fix-5 both-zero expectation update. The file remains untouched at this artifact. The three prior fold commits stand, and directed optional 7 remains pending.

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
- ui/test/acceptance.test.ts -> in
FOLD_SCOPE_RESULT: all-in

New-path bound: update only the composed both-zero expectations so logged T and bound intervals remain percentages, while p, margin, critical-bid combined conversion, and exact zero budget units retain their ruled states. Any other assertion change in that file is a new deviation.

Remaining work: resume Task 3 RED/GREEN on the newly admitted expectation; complete comment-only Task 4 in `derive.ts`; run the full UI/acceptance, lint, build, referee, and owner-validation battery; push the existing PR branch; return the fold report. No merge authority is present.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static replay UI only
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent canonical data writes
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game-log display state only
- AI-or-automation-acts-downstream: no — replay rendering performs no downstream action
- worker/scheduler/queue/retry/async-side-effect: no — no worker or async side-effect change
- cross-repo/service-contract/generated-schema/shared-API-event: no — existing log/protocol contracts are consumed unchanged
- user-visible-control-with-materializer/downstream-consumer: no — controls affect only in-memory replay presentation
- test-runtime-role-mismatch: no — the composed jsdom assertion directly covers the rendered consumer surface
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the owning Planner resolved the sole scope omission without changing product semantics
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none

ACTIONS_GIT_REF: none — no edits made under this corrected pre-edit scope relay
FINAL_GIT_STATUS_SHORT: none — clean linked worktree

Next action: perform the narrow composed-acceptance RED/GREEN update and finish the remaining admitted fold work. Merge remains separately gated.
