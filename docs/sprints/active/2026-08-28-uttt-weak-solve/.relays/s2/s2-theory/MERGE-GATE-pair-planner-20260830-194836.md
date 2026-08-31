ROLE: Pair Planner
PHASE: MERGE-GATE
AUTHORITY: report-only
DISPATCH_ID: s2-theory-c3-merge-rec-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — merge authorization for PR #13 is the requested decision; only operator/orchestrator may grant it
FROM: theory.planner
TO: s2.orchestrator-planner
CC: s2.orchestrator-reviewer, theory.implementer
IN_REPLY_TO: s2-theory/REVIEW-FOLD-pair-implementer-20260830-180918.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@282f504
TARGET_BRANCH: main
BRANCH: theory/checker-c3
PR: https://github.com/iwnlcern/utt/pull/13
SUBJECT: MERGE-GATE recommendation — theory-c3 PR #13 @ df9a90a: merge-ready (erratum landed)

## Recommendation

Verdict recommendation: MERGE-READY for PR #13 at head df9a90a5cfe39c397896406f9a3032fe465a7375 (branch theory/checker-c3, base main@282f504).
This relay grants nothing; it requests the separate merge authorization per the standing gate.
The erratum-before-merge sequencing your authorizing dispatch asked for is satisfied: the approved erratum's constants and paired boundary gates are IN this head (fold-2), so no named condition needs to ride the grant.
Post-merge evidence class: merged-not-deployed, E2 terminal (offline checker + byte-pinned corpus; no deploy/live surface).

## Chain of record (all lineage-valid, all lint-checked)

- Governing design: DD-theory-c2-20260828 rev3 + approved erratum 1 rev2 @ 950bc63 (schema of record; approving review s2-theory-c2-erratum-design-review-2); scope narrowed by your NARROW ruling s2-theory-c3-proceed-2.
- PLAN: PL-theory-c3-20260829 rev2, approved s2-theory-c3-plan-review-2.
- Dispatch: your direct s2-theory-c3-impl-1 (cross-pair trigger routing honored; my SCOPE_DIFF all-in on record at s2-theory-c3-dispatch-req-1).
- IMPL: PR #13 initially @ 3f3a1f0 (22 files, exact plan surface, frozen surfaces untouched).
- Review: custom 4-lens panel (record in s2-theory-c3-fold-1) — design-conformance optional, test-false-green approve (5 planted checker defects all caught), adversarial-input optional (~85 attacks, zero wrong-accepts, zero crashes, XXH3 differential clean to 65536 bytes), idiomaticity must-fix (CLI path-argument panic).
- Fold-1 @ 675ef22: MF-1 + all directed optionals; my targeted check passed (panic repro gone: exit 1, one JSON line, EV_PARSE; 197 green reproduced).
- Erratum cycle: flag → your authorization → doc rev2 (review rounds: must-revise then approve) → fold-2 @ df9a90a with a full RED chain, including the false-green proof (the valid ply-81 probe rejected under the old guard) and proof the old MUT-H14 artifact no longer pinned the boundary.

## Final targeted check (this seat, fresh checkout of df9a90a)

- Fold-2 diff 675ef22..df9a90a equals the five FOLD_SCOPE paths exactly; the wire.c3 change is the single one-value guard (>80 → >81).
- Direct boundary probes run by me against the freshly built checker: committed MUT-H14 rejects exactly E_ROW_PLY_RANGE; the emitter-built 237-byte ply-81 all-drawn NOLOSS probe passes.
- Full suite reproduced: 198 pytest green (incl. 55-check battery) and c3c-test clean under pinned c3c 0.8.3.

## Bundle outcome vs acceptance criteria (plan rev2)

All six criteria E2-verified: 4 positives + verdict modes; all 44 mutants exact-code rejections; XXH3 vectors + corpus anchor (+ reference cross-check to 65536 B and >240 B battery probe); fixtures-v1 matrix in C3; deterministic P4/MUT-R22/catalogue-51 backlog; suites green with exact scope.
Open ledger items that do NOT gate this merge: c6 (external-memory verified-set, resume, scale), as-needed alt fixture profile, implementer-declined discretionary idiom optionals (fault-channel unification, verified/indices merge — noted for c6).

Requested next action: your merge authorization relay (addressed token or field-form grant to theory.implementer) pinned to head df9a90a, or a contrary ruling.

ACTIONS_GIT_REF: none — no edits made
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-180918.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260830-175144.md
(daemon lock/INDEX and the fold-2 relays awaiting orchestrator batch; taken immediately before this filing)
