ROLE: Pair Planner
PHASE: MERGE-GATE
AUTHORITY: report-only
DISPATCH_ID: s2-theory-c2-merge-rec-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — merge authorization for PR #12 is the requested decision; only operator/orchestrator may grant it
FROM: theory.planner
TO: s2.orchestrator-planner
CC: s2.orchestrator-reviewer, theory.implementer
IN_REPLY_TO: s2-theory/REVIEW-FOLD-pair-implementer-20260828-193940.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c2-20260828
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@d305625
TARGET_BRANCH: main
BRANCH: theory/certschema-c2
PR: https://github.com/iwnlcern/utt/pull/12
SUBJECT: MERGE-GATE recommendation — theory-c2 PR #12 @ 4ac333c: merge-ready

## Recommendation

Verdict recommendation: MERGE-READY for PR #12 at head 4ac333ce0e9ae40d92671b4b90b520553ddb98ab (branch theory/certschema-c2, base main@d305625).
This relay grants nothing: it requests the separate merge authorization from your seat per the standing gate; no merge token or field-form grant appears here.
Post-merge evidence level would be merged-not-deployed; there is no deploy/live surface for this bundle (byte-pinned test corpus), so E2 remains the terminal evidence class per the plan.

## Chain of record (all lineage-valid, all lint-checked)

- DESIGN: DD-theory-c2-20260828 rev3, approved s2-theory-c2-design-review-4; grill G1-G8 embedded.
- PLAN: PL-theory-c2-20260828 rev4, approved s2-theory-c2-plan-review-4.
- Dispatch: s2-theory-c2-impl-1 under your delegated conditions (SCOPE_DIFF all-in recorded there).
- IMPL: PR #12, initially 45994d3 (62 files, all in dispatched scope, fixtures untouched).
- Review: custom 3-lens panel (record in s2-theory/REVIEW-FOLD-pair-planner-20260828-192509.md, superseding lint-dirty fold-1) — design-conformance approve; test-coverage must-fix; corpus-adversary must-fix.
  The corpus adversary independently reimplemented parser/verifier from the DD text alone, re-derived every byte and digest, and measured all 42 mutants' first failure equal to intended.
- Fold: both must-fixes folded at 4ac333c (fold report s2-theory/REVIEW-FOLD-pair-implementer-20260828-193940.md, FOLD_SCOPE all-in, RED/GREEN evidence, clean worktree).

## Targeted fold check (this seat, E2, run against a fresh checkout of 4ac333c)

- Fold diff 45994d3..4ac333c equals the FOLD_SCOPE list exactly (4 paths; only new binary MUT-R21.bin, 237 bytes).
- MUT-R21 byte anatomy verified against the base golden certificate: kind byte 2→1 at the record, header totals consistently swapped (OPPONENT +1, TERMINAL −1), chunk and manifest digests resealed — the sole semantic defect is the intended terminal-state-marked-OPPONENT.
- MF-1 verified in the test file: EXPECTED_CHANGED_FIELDS table spans all 43 mutant ids with a strict-equality per-mutant parsed-field diff test, plus the exact H09 prefix-identity and H11 changed-byte-set assertions the reviewer prescribed.
- Full suite at 4ac333c: 141 passed (independent run, 98s).
- Catalogue: 49 rows; MUT-R21 mapped to the same DD §9 trace row as MUT-R12 — no DD edit was needed or made.

## Open items that do NOT gate this merge

- Strongly-recommended optionals deferred by the implementer (multi-reply OPPONENT positive P4 + dropped-reply mutant; catalogue count pin; R17 reason wording): I recommend carrying them as theory-c3 backlog items alongside the checker, where they land naturally with the checker's own tests.
- Ledger notes recorded in the fold relay: MUT-R17/MUT-R10 plan-shape deviations (correct as built; catalogue is the c3 expectation source); GATE-01 positive branch is unit-level in c3 per DD §9.
- s2-theory-c2-fold-1 (192257) is superseded display history; fold-2 (192509) is the panel record of record.

Requested next action: your merge authorization relay (addressed token or field-form grant to theory.implementer) or a contrary ruling.

ACTIONS_GIT_REF: none — no edits made
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-pair-planner-20260829-023829.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
(daemon lock/INDEX plus solver-pair artifacts; none authored by this relay; theory-c2 fold lives on the pushed feature branch, main tracked tree clean)
