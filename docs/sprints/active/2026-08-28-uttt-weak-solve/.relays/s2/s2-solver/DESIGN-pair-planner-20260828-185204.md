ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — rev 3 folds the three rev-2 residuals; all are consistency/byte-contract corrections under existing rulings
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 7db7df40044ffd1fc0a9bfe14f7e22c822f5ca7f75601a6ad0e43e5dafbb844e
GRILL_REQUIRED: yes
OWNER: solver
REPO: utt
SUBJECT: DESIGN-REVIEW REQUEST rev 3 — MR-2 residual, MR-4A, MR-4B folded

## Team solver — s2-solver-c2: DESIGN-REVIEW REQUEST (rev 3)

Current scope for the `TO` addressee: read-only DESIGN-REVIEW. No source/test edits, no implementation branches, no commits, no PRs.

Forward amendment (same DESIGN_DOC_ID; digest floats; rev-2 digest was db5de0e9…). Architecture, GRILL decisions, and the closed folds (MR-1, MR-3, MR-5, MR-6, MR-2 outcome machine) untouched. Fold map:

- MR-2 residual folded (§3): both default switch signals are now executable with explicit switch-favoring direction — kernel A: DN(root)/PN(root) ≥ 10 (numerator/denominator named); kernel B: refuted-root-children fraction ≥ 0.8; either sustained across two consecutive checkpoint reviews. Still scheduling-only, nonbinding, verdict-irrelevant.
- MR-4A folded (§5, §10): locked collision-safety rule — the working TT gains a full-identity sidecar (engine FullKey pattern); any exact use of a hit (proof, disproof, exact bound, cutoff, kernel return, ProvenStore append) requires 37-byte sidecar equality; tag-only matches are heuristic-only (ordering/threshold scheduling) and are misses for exact purposes; sidecar writes ordered so a torn pair reads as a miss; TT budget re-stated (~56 B/way combined). Required tests added: forced same-tag/different-state probes per kernel, equivalence to empty-TT search, no false durable fact.
- MR-4B folded (§5, §6, §10): psl-v1 layer format fully byte-owned (magic/version/ply/reserved/record_count header, strict 48 B-record concatenation, ascending full-record-byte order, one record per (state, claim), fail-closed refusal list incl. size arithmetic, popcount≠ply, order/duplicate, digest); deterministic dedupe — total order (state, claim, witness), smallest-witness winner, permuted inputs compact to byte-identical layers (test added); claim consistency closed over the inference lattice — surviving claim set must be a subset of one of the three truth-assignment sets {WIN_X, NOLOSS_X}, {NOLOSS_X, NOLOSS_O}, {WIN_O, NOLOSS_O}, so {WIN_X, WIN_O} and every closure-implied impossible refuses via E-PSR-CONTRADICTION; psm-v1 manifest schema locked (version-first refusal, required keys/types, digest hex forms, number domains < 2^53 with range refusals, strict unknown-key refusal, trailing-bytes refusal, root-confined file-name grammar, previous durable manifest stays authoritative on any refusal); manifest pins name psl-v1.

Design doc to review:
- DESIGN_DOC_ID: DD-solver-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 above)
- Selected option: unchanged.
- Rejected alternatives: unchanged.
- Boundary contract: unchanged (pinned theory contract).
- Acceptance criteria draft: design-lock impact updated — §5 collision-safety, psl-v1 contract + deterministic dedupe, and §6 psm-v1 refusal rules are now named acceptance criteria alongside the rev-2 set.
- Operator decisions/defaults: unchanged; no fresh operator items (your OPERATOR_ITEMS: none confirmed).
- Open questions: none.

Requested response:
Use Template C (`PHASE: DESIGN-REVIEW`, `FROM: solver.implementer`, `TO: solver.planner`, `PARENT_DISPATCH_ID: s2-solver-c2`, same `DESIGN_DOC_ID`) and return `DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required`.

ACTIONS_GIT_REF: designs/DD-solver-c2-20260828.md amended in place (rev 3, sha256 7db7df40044ffd1fc0a9bfe14f7e22c822f5ca7f75601a6ad0e43e5dafbb844e); still uncommitted pending orchestrator batch commit
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
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
(this seat's artifact: the design doc line; relay rows are daemon-rendered filings)
