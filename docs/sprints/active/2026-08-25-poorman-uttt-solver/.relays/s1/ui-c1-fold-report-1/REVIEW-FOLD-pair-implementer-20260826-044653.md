ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: ui-c1-fold-report-1
PARENT_DISPATCH_ID: ui-c1-fold-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — this is the requested implementation fold report; merge remains separately gated
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
TARGET_BRANCH: main
BRANCH: ui-c1-impl
PR: https://github.com/iwnlcern/utt/pull/4
SUBJECT: REVIEW-FOLD REPORT — all five must-fixes and directed optionals 6/7 folded at 9f2e5a9; fresh E2 green; PR 4 updated; merge gated

## Result

Folded the corrected `ui-c1-fold-2` scope into `ui-c1-impl` and pushed the reviewed five-commit range `837d7b2c36ec274b70fb1e3d9f0dae76f240318f..9f2e5a9f8565510959e6bc64b40ebb2158db57c7` to the existing PR 4. The remote branch and PR head both resolve to `9f2e5a9f8565510959e6bc64b40ebb2158db57c7`; the linked implementation worktree is synchronized and clean. No merge, deployment, release, or artifact-pin action was attempted.

## Finding disposition

1. Must-fix 1: exact tie units are rendered inline from the final logged attempt for both `tie_last_mover` and `tie_coin` production paths.
2. Must-fix 2: cursor, losing-intent visibility, and analysis-seat selection now share the single `cursorReducer` state/action surface without changing cursor fallback semantics.
3. Must-fix 3: timeline article names include ply plus resolved, voided, fault, or tie outcome labels.
4. Must-fix 4: ordinary and critical bid percentages use the combined pre-auction budget denominator.
5. Must-fix 5: at both-zero budgets, logged `t` and logged bounds remain verbatim percentages; only `p`, margin, and units-of-combined conversions are unavailable. The corrected acceptance expectation was changed narrowly after the fold-2 scope successor admitted that path.
6. Directed optional 6 accepted: the conditional-PV legend is rendered only when conditional analysis exists and `PV_PIN.pinned` is true. Cost if wrong: an intended unpinned preview mode would hide the legend.
7. Directed optional 7 accepted: comment-only owner citations now cover sequential/final auction shape, attempt ordinal/request-ID constraints, keyed recovery association, and the UI's fail-closed incomplete trailing-prefix extension. Cost if wrong: citation wording may need correction; no runtime strictness was added.

Task review caught the stale acceptance expectation outside the original nine-path admission; implementation stopped before that new-path edit, obtained the fold-2 correction, filed a fresh ten-path pre-edit scope artifact, and then repaired the single ruled assertion. Task 4 review also caught two overstated citations; one comment-only correction wave fixed both, and fresh scoped re-review accepted them with no findings.

The final independent exact-range review found 0 Critical and 0 Important issues and ruled the fold ready to push. Three Minor items remain deferable: use contract-valid table-driven dedicated tie fixtures, add a positive legend assertion to a pinned test, and prefer protocol heading names over section-number shorthand in citations. Production covers both tie reasons; an existing denominator test also exercises the `tie_coin` inline-unit branch. These Minors do not weaken any dispatched acceptance condition.

## Fresh controller verification at 9f2e5a9

- Node `v25.2.1`; `cd ui && npm test` -> 18/18 files, 252/252 tests passed. — E2.
- `cd ui && npm test -- test/acceptance.test.ts` -> 55/55 passed. — E2.
- `cd ui && npm run lint` -> exit 0. — E2.
- `cd ui && npm run build` -> exit 0; Vite transformed 33 modules and emitted the production bundle. — E2.
- `cd referee && .venv/bin/pytest -q` -> 184 passed, 1 skipped in 6.94s; the skip is the existing development-only theory-fixture gate. — E2.
- Owner Draft 2020-12 schemas plus `read_log`/`replay_frames` over all 13 complete UI fixtures -> `owner-schema-replay-ok files=13`. — E2.
- `cmp ui/fixtures/success-macro-win.jsonl ui/public/sample-game.jsonl` -> exit 0. — E1.
- `npm ls --omit=dev --depth=0` -> only exact `react@19.2.8` and `react-dom@19.2.8`. — E1.
- `git diff --check 837d7b2..9f2e5a9` -> exit 0; base is an ancestor; exact changed-path comparison equals the ten-path admitted set. — E1.
- Production pin audit still resolves only `PV_PIN: { pinned: false, source: null }`. Remote branch and PR head checks both resolve to `9f2e5a9`; final linked-worktree status is empty and synchronized with `origin/ui-c1-impl`. — E1/E2.

## Scope and retained gates

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

PR 4 remains open against `main`. This report requests the Planner's targeted quick check and fold reconciliation. It grants no merge, deployment, publication, release, or artifact-pin authority; the linked worktree is retained for PR feedback.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static replay UI; no credential surface changed
- migration/backfill/destructive-write/canonical-data-repair: no — no persistent data shape or repair changed
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — read-only replay UI; no action bridge
- worker/scheduler/queue/retry/async-side-effect: no — no worker or external side effect added
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI citations and verification consume harness-owned log contracts
- user-visible-control-with-materializer/downstream-consumer: yes — timeline, reducer-owned controls, metrics, and conditional legend are user-visible
- test-runtime-role-mismatch: no — production React surfaces are covered by composed jsdom tests plus production build
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — corrected scope is exact; all requested verification was rerun live
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested; normal Planner and orchestrator review routing is retained

ACTIONS_GIT_REF: ui-c1-impl@9f2e5a9f8565510959e6bc64b40ebb2158db57c7; fold commits 5cfa10e, b748575, 9af5dcb, d0fa885, 9f2e5a9 pushed normally to origin/ui-c1-impl and PR 4; no merge
FINAL_GIT_STATUS_SHORT: clean — linked worktree `/Users/jack/Programming/utt/.worktrees/ui-c1-impl` is synchronized with `origin/ui-c1-impl` at 9f2e5a9
