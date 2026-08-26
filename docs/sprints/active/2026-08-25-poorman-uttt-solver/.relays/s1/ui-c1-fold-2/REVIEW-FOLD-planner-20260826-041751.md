ROLE: Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: ui-c1-fold-2
PARENT_DISPATCH_ID: ui-c1-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the requested scope correction is this Planner's to grant; the omission was this relay author's, not a new product question
FROM: ui.planner
TO: ui.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-fold-1/REVIEW-FOLD-pair-implementer-20260826-040120.md
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
SUBJECT: FOLD SCOPE CORRECTION — acceptance.test.ts admitted for the must-fix-5 expectation update; deviation cured

## Scope correction (deviation accepted as this Planner's omission)

Your deviation stop was correct and the cause is mine: must-fix 5's ruling (verbatim logged `t` and bound intervals stay visible as percentages at both-zero; not-applicable covers only `p`, margin, and units-of-combined conversions) necessarily updates the composed acceptance expectation at `ui/test/acceptance.test.ts:354`, and my fold-1 findings list omitted that file. The fresh E2 reproduction you supplied shows the implementation rendering exactly the ruled copy (`T: 62.50%`, `interval [50.00%, 75.00%]`, with `p`/margin/critical-bid conversion n/a) against the stale expectation.

Corrected admitted scope for the remainder of the fold (all prior admissions unchanged, one addition):

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

Bounds on the newly admitted path: the acceptance-test change is the narrow expectation update to the ruled both-zero copy (T and interval verbatim percent; p, margin, and critical-bid units-conversion remain n/a; exact `0 / 0` units visible) — RED/GREEN per your normal cycle. No other assertion in that file is in scope under this correction; if closing must-fix 5 turns out to require touching any other assertion, that is a fresh deviation — stop and relay again.

Then finish as planned: directed optional 7 (owner-contract citation comments in derive.ts, already admitted), full verification battery (suite + lint + build), push, and return the fold report with FOLD_SCOPE above ACTIONS_GIT_REF. Your three landed in-scope commits (5cfa10e, b748575, 9af5dcb) stand. My targeted quick check follows the report; merge remains separately gated.

ACTIONS_GIT_REF: none — scope-correction relay; no edits by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay authored from the primary worktree while sibling seats churn concurrently; this seat made no edits this turn (implementation worktree state is the Implementer's report of record)
