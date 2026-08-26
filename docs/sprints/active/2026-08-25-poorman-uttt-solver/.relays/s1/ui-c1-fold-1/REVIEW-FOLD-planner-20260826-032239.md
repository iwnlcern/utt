ROLE: Planner
PHASE: REVIEW-FOLD
AUTHORITY: review-only
DISPATCH_ID: ui-c1-fold-1
PARENT_DISPATCH_ID: ui-c1-impl-report-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold-in is implementer-owned under the standing implementation authority; merge stays separately gated
FROM: ui.planner
TO: ui.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-impl-report-1/IMPL-pair-implementer-20260826-024104.md
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
SUBJECT: REVIEW-FOLD — panel synthesis for PR 4: 5 must-fix (3 a11y/lock + 2 design-owner rulings), optionals at your discretion

## Panel record

PANEL_CHOSEN: custom
DEFAULT_ROLES_CHANGED: yes
WHY_THIS_PANEL: non-trivial PR (79 files/11k insertions) on a static local replay UI — no backend/hot paths, so the default performance lens was swapped for an accessibility/UI-semantics lens; a11y and the six grill decisions are locked acceptance criteria
ROLES:
- security — hostile log-file input, storage, rendering surfaces
- test-coverage — locked obligations proven, false-green hunt (independently re-ran suite + owner validators)
- correctness — DD rev 7 / PL rev 4 conformance, landed-schema fidelity
- accessibility-ui-semantics — a11y criteria + grill-decision fidelity + React idioms

Lens verdicts: security approve; test-coverage approve; correctness optional; accessibility must-fix. Consolidated: REVIEW-FOLD required. No blockers.

## Must-fix (fold before merge)

1. Tie rows: exact integer units inline (a11y F1; correctness F6 deduped into it) — grill decision 2 lists ties among always-full-units resolution points; today tie_last_mover/tie_coin rows carry only labels + percent, units confined to a title tooltip (Timeline.tsx:53-58,144-146). Render the tied bids' exact units inline in tie-resolution rows and add a test. Design-owner note: the payment-equals-tied-amount reading was arguable; this ruling closes it — ties show tied-bid units inline.
2. Fold `showLosingIntent` and `preferredAnalysisSeat` into the single reducer (a11y F2) — DD §2 mandates one reducer for `{cursor, toggles}`; both live in useState (GameView.tsx:28-29). Mechanical refactor + existing tests keep passing.
3. Timeline row accessible names (a11y F3) — DD §7 requires screen-reader names for timeline rows; row articles are unnamed (Timeline.tsx:132-137,179). Add an aria-label per row (ply + outcome, the existing resolutionLabel helper serves) and assert it.
4. DESIGN-OWNER RULING (correctness F1, bid percent denominator): bids display as percent-of-COMBINED budget, matching the locked share semantics (DD §3 p = B_X/(B_X+B_O)) and the critical-bid display — one denominator app-wide. Remove the invented `n/a — seat budget is zero` state (Timeline.tsx:26-40); the only n/a remains M2's both-zero. Update the affected tests.
5. DESIGN-OWNER RULING (correctness F2, T at both-zero): logged `t` is budget-independent and remains meaningful — show verbatim `t` (and bound intervals) in percent form at both-zero; the not-applicable state covers only `p`, margin, and any units-of-combined conversion (MetricsPanel.tsx:124-128, dualShare path). Update the both-zero metric tests accordingly.

## Directed optionals (cheap, fold with the above)

6. Gate the conditional-ghost legend on PV_PIN.pinned (a11y F5 = correctness F4, deduped) — one line (GameView.tsx:212-217).
7. Add owner-contract citations (DD-harness-c1 §8 / protocol doc lines) as code comments on each extra fail-closed derive invariant (correctness F3) — documents that no contract-conformant log can be rejected; the added strictness is hereby recorded at the lock layer by this relay, so no upward relay is needed beyond it.

## Implementer-discretion optionals (not required for merge)

- File-size cap before readAsText; recents length/size cap (security F1/F2).
- CI execution of manifest validator commands; `budget_margin === 0` assertion for both-zero; consolidate the float-equality grep on the self-tested variant (tests F1/F2/F3).
- Presentational board cells vs disabled buttons (a11y F4) — defer unless cheap; tests pin getByRole('button').
- Hover-only exact units elsewhere (a11y F6): ruling — title-on-hover satisfies decision 2's wording outside resolution points; must-fix 1 covers the resolution-point gap. No further change required.

## Nits (do not block)

- `__proto__`/`constructor` fixture regression test; recents dedupe content-hash tiebreak (security F3/F4).
- Stable toggle label + aria-pressed; drop the double aria-label on the seat select; delete the dead `transition: points` CSS (a11y F7/F8/F9).

## Resolved without code change

- Correctness F5 (knife-edge label): the implementation's `no displayed edge` wins over PL Task 11's `knife-edge at p = T` wording — DD §3 forbids asserting exact equality not read from the log and the displayed margin is rounded. This relay is the required plan-vs-DD disagreement record; no plan reissue needed for display copy.

## Fold mechanics

Fold under the standing implementation authority (ui-c1-impl-2 refreshed at plan digest d083f61e…): write your FOLD_SCOPE artifact before editing (rows for every file you will touch, all -> in against the findings above), fold, run the full suite + build, push, and return your REVIEW-FOLD report with FOLD_SCOPE above ACTIONS_GIT_REF. After your fold report, I run a targeted quick check (must-fix items only), not a full panel, unless the fold changes design/blast radius. Merge remains a separate orchestrator/operator gate.

ACTIONS_GIT_REF: none — review synthesis relay; no edits by this seat; panel evidence in the four lens reports (E2-verified independently by the tests and correctness lenses, incl. suite re-runs and owner-validator executions)
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/SEATS.md
(daemon-rendered projections plus foreign concurrent work; no edits by this seat this turn)
