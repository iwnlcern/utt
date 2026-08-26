ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: ui-c1-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — all proposed fold paths are named by the five must-fixes or the two accepted directed optionals; merge remains separately gated
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-fold-1/REVIEW-FOLD-planner-20260826-032258.md
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
BASE: ui-c1-impl@837d7b2c36ec274b70fb1e3d9f0dae76f240318f
TARGET_BRANCH: main
BRANCH: ui-c1-impl
PR: https://github.com/iwnlcern/utt/pull/4
SUBJECT: PRE-EDIT FOLD SCOPE — nine paths cover five must-fixes and both directed optionals

## Pre-edit scope decision

The five must-fixes are technically consistent with the locked UI behavior: tie rows currently hide exact units in tooltips; two toggles bypass the replay reducer; timeline articles lack accessible names; bids use per-seat rather than combined-budget denominators; and both-zero suppresses logged threshold and interval percentages. I accept directed optionals 6 and 7 because they are bounded to the named GameView and derive surfaces and do not change the protocol or locked design.

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
FOLD_SCOPE_RESULT: all-in

Scope mapping:
- Timeline source/test cover must-fixes 1, 3, and 4.
- GameView, cursor reducer, and their tests cover must-fix 2 and directed optional 6.
- MetricsPanel source/test cover must-fix 5.
- Replay derive source covers directed optional 7's owner-contract comments only; no invariant behavior changes.

Out of scope remains unchanged: fixtures, schemas, protocol bytes, referee, engine, theory, build tooling, dependencies, CSS, merge, release, deployment, and remote configuration. Any newly discovered required path is a deviation and stops the fold before that path is edited.

Boundary contract: the UI continues to consume the existing validated game-log and analysis shapes and renders logged facts without rule recomputation. No writer, schema, event, or cross-owner contract changes.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static replay UI only
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent canonical data writes
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game-log display state only
- AI-or-automation-acts-downstream: no — replay rendering performs no downstream action
- worker/scheduler/queue/retry/async-side-effect: no — no worker or async side-effect change
- cross-repo/service-contract/generated-schema/shared-API-event: no — existing log/protocol contracts are consumed unchanged
- user-visible-control-with-materializer/downstream-consumer: no — controls affect only in-memory replay presentation
- test-runtime-role-mismatch: no — component tests exercise the rendered semantics under the existing jsdom contract
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the design-owner relay resolves both semantic questions and bounds the optionals
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none

ACTIONS_GIT_REF: no edits claimed; ui-c1-impl@837d7b2c36ec274b70fb1e3d9f0dae76f240318f; final linked-worktree git status --short = none — clean tree
FINAL_GIT_STATUS_SHORT: none — clean tree

Next action: perform RED/GREEN folds only on the nine all-in paths, run the full UI and acceptance suites plus lint/build and owner validators, push the existing PR branch, and return the daemon-admitted fold report. Merge remains separately gated.
