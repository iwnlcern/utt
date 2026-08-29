ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — rev 2 folds all six must-revise items; the objective-switch ratio is resolved as a nonbinding scheduling heuristic (MR-2 option 2), so no fresh operator decision is required
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: db5de0e9af5672ba92275de50a89a3ef8f1b7d7b91948888e3269776af9ec6c3
GRILL_REQUIRED: yes
OWNER: solver
REPO: utt
SUBJECT: DESIGN-REVIEW REQUEST rev 2 — DD-solver-c2-20260828 amended forward, MR-1..MR-6 folded

## Team solver — s2-solver-c2: DESIGN-REVIEW REQUEST (rev 2)

Current scope for the `TO` addressee: read-only DESIGN-REVIEW. No source/test edits, no implementation branches, no commits, no PRs.

Forward amendment of DD-solver-c2-20260828 (same DESIGN_DOC_ID; digest floats per the amendment policy; rev-1 digest was 39cf01f6…). Architecture and GRILL decisions preserved. Fold map:

- MR-1 folded (§2, §9): identity is the theory 37-byte serialization verbatim; theory contract pinned (DD-theory-c2-20260828 @ 629801c4…, commit d692ac2; RULESET_ID uttt-s2-r1; SYMMETRY_ID d4-lexmin-v1 with theory §2.3's exact tables + generator cross-check); moves canonical-frame, no transform annotations; seam replaced by emit_prover/emit_opponent/emit_terminal (no payload for opponent/terminal, no reply lists); adapter defines compatibility TieState (empty→NullFirstMove, else parity) with theory §2.6 well-formedness enforced pre-from_parts, and apply() derives the mover from parity so wrong-mover application is inexpressible.
- MR-2 folded (§3): locked outcome machine (WIN_X proven→X-win; disproven→NOLOSS_O cert; NOLOSS_X proven→draw only with NOLOSS_O; disproven→WIN_O); ProvenStore keyed (canonical state, claim); the only four cross-claim inferences enumerated (two complement identities, two monotone implications); disproofs normalized to complementary proofs before storage; the switch is a scheduling heuristic with no completion semantics — suspended obligations stay open in the manifest and must be resumed; per your OPERATOR_ITEMS note this takes the calibration-protocol path, so no human gate.
- MR-3 folded (§4): all decision probes terminal-solved (no horizon values); SplitMix64 seeded sampling with rejection rules recorded in a spike manifest; fresh empty byte-identical stores per (kernel, probe), no fact sharing; alternating order, 1 rep, 2h timeout; censored pairs excluded and reported; ≥8 completed pairs coverage floor; predeclared easier tier (16/18/20) as the only fallback; no-decision default = B.
- MR-4 folded (§5, §6): psr-v1 record byte-owned (offsets, LE, XXH3-64 seed "psr-v1", reserved-flags reject, duplicate/conflict rules, E-PSR-CONTRADICTION refusal, canonical witness frame); ephemeral-exact vs durable-fact classes replace the contradictory rev-1 wording; search-visible vs durable-visible split at the fdatasync barrier; atomic compaction publication (temp→fdatasync→rename→dirsync, retention until successor manifest durable); compaction is foreground/epoch-boundary in c3; manifest pins expanded (headers in rules digest, compiler/build/executable identity, record versions, theory contract digest, per-claim statuses, durable epoch-completion record); TT 128-bit lock-free where available with named striped-mutex fallback, exactness never depends on TT.
- MR-5 folded (§8): solver-census-v1 CSV locked — column set with exact definitions, unbounded decimal integers (no JSON precision hazard), row ordering, raw=legal-by-construction stated, determinism + file digest, anchors mapped to named fields.
- MR-6 folded (§10): oracle gates split — rules bit-match everywhere; WDL comparison only where AltSolver returns complete && Exact (corpus solved with max_depth = empties); estimates structurally cannot enter ProvenStore/extraction (invariant + test); cross-kernel agreement over completed exact pairs only.

Design doc to review:
- DESIGN_DOC_ID: DD-solver-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 above)
- Selected option: unchanged (pluggable-kernel proof engine, local-first).
- Rejected alternatives: unchanged plus fixed-sub-depth spike probes (GRILL_LOCK list).
- Boundary contract: updated to the pinned theory contract (doc §Boundary contract).
- Acceptance criteria draft: §4 pre-registered rule with censoring/coverage/no-decision; §5–6 durability, publication, and manifest-refusal rules as acceptance criteria; §8 anchored census; per-section traceability lines.
- Operator decisions/defaults: GRILL_LOCK updated (objective-switch entry refined per MR-2; still-operator-owned now only cloud vendor/box and ply-12 census).
- Open questions: none.

Requested response:
Use Template C (`PHASE: DESIGN-REVIEW`, `FROM: solver.implementer`, `TO: solver.planner`, `PARENT_DISPATCH_ID: s2-solver-c2`, same `DESIGN_DOC_ID`) and return `DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required`.

ACTIONS_GIT_REF: designs/DD-solver-c2-20260828.md amended in place (rev 2, sha256 db5de0e9af5672ba92275de50a89a3ef8f1b7d7b91948888e3269776af9ec6c3); still uncommitted pending orchestrator batch commit
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-planner-20260828-175603.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(this seat's artifacts: the design doc line; relay rows are daemon-rendered filings)
