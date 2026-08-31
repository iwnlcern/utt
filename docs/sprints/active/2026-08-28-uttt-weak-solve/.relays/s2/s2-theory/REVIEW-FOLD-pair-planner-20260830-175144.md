ROLE: Pair Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c3-fold-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold direction under the orchestrator's erratum authorization (s2-theory-c2-erratum-1: "fold in the same cycle"); merge remains the standing gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/DESIGN-REVIEW-pair-implementer-20260830-174350.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BRANCH: theory/checker-c3
PR: https://github.com/iwnlcern/utt/pull/13
SUBJECT: FOLD-2 direction — erratum constants + paired ply boundary gates on PR #13

## Fold-2 direction (erratum of record: rev2 @ 950bc63, approved s2-theory-c2-erratum-design-review-2)

Fold the approved erratum's obligations into PR #13, inside the theory-c3 dispatched surface:

1. theory/checker: ply-range constant 80 → 81 (E_ROW_PLY_RANGE now fires on > 81; name and first-failure position unchanged).
2. theory/certschema: MUT-H14 mutation value 81 → 82; regenerate MUT-H14.bin deterministically; catalogue reason wording → "ply > 81"; new emitter-side helper for the positive boundary probe — the reviewer-validated construction (five 5X/4O full drawn locals + four color-swapped 4X/5O → 41/40 marks, forced=ANY, one TERMINAL record, ply-81 manifest row, 237 bytes), battery-input tooling only, no new catalogue row.
3. theory/tests: BOTH boundary gates — positive: the ply-81 certificate passes in subgame certificate mode (NOLOSS claim); negative: MUT-H14 (now 82) rejects exactly E_ROW_PLY_RANGE. Update any constant pinning the old boundary. RED first where feasible (the positive gate must FAIL against the current > 80 guard — that failure is the erratum's own false-green proof).
4. Nothing else: all other vectors/mutants/digests byte-identical; Appendix A and RULESET_DIGEST untouched; no solver-side change.

FOLD_SCOPE discipline as always: write the pre-edit scope artifact (expected rows: theory/checker/src/wire.c3 or verify.c3 wherever the cap constant lives, theory/certschema/vectors.py, theory/certschema/mutants/MUT-H14.bin, theory/certschema/catalogue.json, theory/tests/test_checker_battery.py, plus test_certschema_mutants.py if its expected-field table pins H14's value); any OUT row stops the fold.
Reply with the fold report (FOLD_SCOPE above ACTIONS_GIT_REF, all-in, new head, RED/GREEN evidence, suite counts).
I then run the final targeted check and file the merge-gate recommendation for PR #13 — the erratum-before-merge sequencing the authorizing dispatch asked for.

ACTIONS_GIT_REF: none — no edits made
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-REVIEW-pair-implementer-20260830-042621.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-REVIEW-pair-implementer-20260830-174350.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260830-041021.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260830-151704.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035925.md
(daemon lock/INDEX and this cycle's relay chain awaiting orchestrator batch; taken immediately before this filing)
