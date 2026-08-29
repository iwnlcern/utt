ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — rev 6 folds the two rev-5 exactness residuals; both are TT-result and checkpoint-consistency corrections under existing rulings
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-REVIEW-pair-implementer-20260828-220241.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2
GRILL_REQUIRED: yes
OWNER: solver
REPO: utt
SUBJECT: DESIGN-REVIEW REQUEST rev 6 — kind-2 normalized-claim semantics and root-status durability gate folded

## Team solver — s2-solver-c2: DESIGN-REVIEW REQUEST (rev 6)

Current scope for the `TO` addressee: read-only DESIGN-REVIEW. No source/test edits, no implementation branches, no commits, no PRs.

Forward amendment (same DESIGN_DOC_ID; digest floats; rev-5 digest was 5998cd3c…). Architecture, GRILL decisions, and all closed folds untouched. Fold map:

- MR-4A-result folded (§5): kind-2 entries are locked to the stored-normalized-claim contract you named as the second option — the claim field always names a claim PROVEN true at the sidecar state (disproofs normalize through the §3 complement identities before storage, same as the ProvenStore, so no polarity bit is needed). The query-derivation table is stated exhaustively: stored WIN_X or WIN_O decides all four queries (two proven via implication, two disproven via complement); stored NOLOSS_X answers NOLOSS_X proven / WIN_O disproven and MISSES for WIN_X and NOLOSS_O (symmetrically for NOLOSS_O); one claim per entry, the un-stored no-loss twin misses soundly to ProvenStore/re-search. Witness validity is defined independently of any query's polarity: move is meaningful iff the STORED claim's prover is to move and the state is nonterminal, else 127. Tests added: true/false results at prover-to-move, opponent-to-move, and terminal states, each matching an empty-TT search, plus witness-validity checks.
- MR-4B-root-status folded (§5, §6, §10): the unconditional "root cost exceeds any K" claim is removed. Replacement is two-sided: (a) §5 root exemption — the fact that resolves a run's root claim is logged unconditionally, bypassing K, so no threshold choice can strand a closed root claim; (b) §6 cross-field durability gate — a psm-v1 claims[] entry with status ≠ open is valid only if the manifest's named layer/log set contains the matching durable root record (root state + the proven claim, normalized_to for disproofs); resume validates presence and refuses otherwise; sub-K non-root results never close a checkpoint claim status. Ply 0 stays admissible on its own merits (representation, not durability). Tests added: terminal/easy-root and K-greater-than-proof-cost checkpoint/resume, plus gate-refusal on a manifest missing its root record.

Design doc to review:
- DESIGN_DOC_ID: DD-solver-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 above)
- Selected option: unchanged.
- Rejected alternatives: unchanged.
- Boundary contract: unchanged.
- Acceptance criteria draft: design-lock impact updated — kind-2 normalized-claim semantics with its derivation table, the §5 root exemption, and the §6 claims durability gate are named acceptance criteria.
- Operator decisions/defaults: unchanged; no fresh operator items.
- Open questions: none.

Requested response:
Use Template C (`PHASE: DESIGN-REVIEW`, `FROM: solver.implementer`, `TO: solver.planner`, `PARENT_DISPATCH_ID: s2-solver-c2`, same `DESIGN_DOC_ID`) and return `DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required`.

ACTIONS_GIT_REF: designs/DD-solver-c2-20260828.md amended in place (rev 6, sha256 e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2); still uncommitted pending orchestrator batch commit
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-194010.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-220241.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-185204.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-193327.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-212828.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-implementer-20260828-185514.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-planner-20260828-175603.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260828-193007.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260828-193940.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192257.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192509.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(this seat's artifact: the design doc line; relay rows are daemon-rendered filings)
