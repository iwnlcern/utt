ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c2-design-review-5
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the remaining defects are bounded TT-result and checkpoint-root consistency corrections under the existing architecture and rulings
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-pair-planner-20260828-212828.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 5998cd3c6a6d6e76f6f8d9f37ec6db788b8068acc9d12a2cd13b91517783e616
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c2
OWNER: solver pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-solver-c2 rev5 — representations fit; exact-result polarity and root durability remain

## Scope and identity

Reviewed all 356 lines of revision-5 `DD-solver-c2-20260828` at SHA-256 `5998cd3c6a6d6e76f6f8d9f37ec6db788b8068acc9d12a2cd13b91517783e616`, the exact successor request, revision-4 Implementer review, kernel result semantics, complete GRILL_LOCK, and boundary contract. No source, test, fixture, design, branch, commit, or PR change was made. The architecture and every previously closed fold remain accepted.

## Verdict

DESIGN_REVIEW_VERDICT: must-revise

Revision 5 closes the three literal revision-4 representation defects: `tte-v1` now fits 128 bits while keeping the scheduling ratio at full width outside the TT; psl/psm admit the ordinary ply-0 path; and log-segment integrity consistently covers the named durable prefix. Two exactness contradictions remain: kind-2 does not define which boolean result its “exact bound” represents, and psm-v1 asserts unconditional root durability despite a finite proof and unbounded flag-tunable threshold K.

## MR-4A-result residual — kind-2 has no defined exact-result polarity

Blocking evidence: Kernel B returns `proven | disproven` for a queried claim (`DD-solver-c2-20260828.md:65-69`), and a full-key TT hit may drive an exact bound, boolean cutoff, or kernel return (`:111-114`). But kind-2 contains only `claim`, `move`, and zeroed pn/dn; no field or rule says whether the queried claim was proven or disproven (`:96-108`). `move` cannot safely encode the polarity: a true proof at opponent-to-move or a terminal has no witness move, while a false prover-to-move result can also have none (§9's payload rules, `:278-285`). If `claim` is intended to name the normalized proven claim rather than the query, that contract and its complement/implication lookup semantics are not stated.

Required revision: either allocate an explicit result/bound-polarity bit from the reserved space, or state that every kind-2 entry represents a proven normalized claim and completely define how a query derives proven, disproven, implication-only, or miss from the stored claim. Define witness validity independently of result polarity. Add round-trip and probe tests for true and false results at prover-to-move, opponent-to-move, and terminal states; each must match an empty-TT search.

## MR-4B-root-status residual — flag-tunable K defeats unconditional durable-root claims

Blocking evidence: §5 makes proofs with cost below flag-tunable K ephemeral and writes only costs at or above K to ProvenStore (`DD-solver-c2-20260828.md:121-124`). The manifest then claims a completed root proof “always” exceeds any K and is therefore durable (`:208-214`). Every game proof has finite expanded-node cost, so choosing K above that cost—or solving a terminal/easy subgame root—makes the assertion false. psm-v1 can still mark the root claim `proven` or `disproven-normalized` (`:199-206`), and completed-epoch resume is a no-op (`:232-234`), without a cross-field requirement that the corresponding durable normalized root record exists.

Required revision: remove the unconditional cost claim. For each non-open root status, either require and validate the matching durable proven-claim record in the manifest's named layer/log set (including complement normalization), or define that sub-K root results cannot close checkpoint claim status and must be recomputed after resume. Keep ply 0 admissible for initial-root facts but do not equate “root” with “always durable.” Add terminal/easy-root and K-greater-than-proof-cost checkpoint/resume tests.

## Fold disposition

- MR-1: CLOSED — theory identity, adapter, D4, move frame, and certificate seam remain correct.
- MR-2: CLOSED — outcome completion and both scheduling signals remain coherent.
- MR-3: CLOSED — the exact falsifiable spike remains fixed.
- MR-4A-publication: CLOSED — common stripe locking prevents mixed-generation exact hits.
- MR-4A-capacity: CLOSED — the tte-v1 fields fit 128 bits and A's full-width root ratio stays outside the projection.
- MR-4A-result: PARTIAL — kind-2 exact truth polarity/normalization is undefined.
- MR-4B-recovery: CLOSED — immutable generations, pointer fallback, retention, and first-start behavior remain defined.
- MR-4B-log: CLOSED — segment digest scope, durable bounds, salvage, and torn-tail handling are coherent.
- MR-4B-root: PARTIAL — ply-0 storage is representable, but manifest root status is not bound to a durable root fact under tunable K.
- MR-5: CLOSED — census contract remains consumable and anchored.
- MR-6: CLOSED — AltSolver exactness boundary and Estimate exclusion remain correct.

## Acceptance-criteria disposition

1. Falsifiable A-vs-B procedure: PASS.
2. Every persistent byte owned with a crash story: FAIL — recovered exact root status is not yet bound to persisted proof truth.
3. Census output format theory can consume: PASS.
4. Traceability to audits/rulings/measurements: PASS.
5. Design identity, GRILL_LOCK, rejected alternatives, non-goals, and boundary consumers: PASS as structure; exact Kernel-B TT results and checkpoint root truth remain blocked.

## GRILL pressure result

Decision pressure: the architecture, 128-bit capacity choice, lock-based publication, ply topology, and durable-prefix log rule survive. Alternative pressure: no rejected architecture becomes preferable; one reserved TT bit or an explicit normalized-claim contract and one root-status cross-field rule are sufficient. Boundary pressure: certificate/checker consumption remains intact, but the search-to-TT and checkpoint-to-resume truth boundaries are not yet independently decidable. No operator question is exposed.

## Required successor

Amend `DD-solver-c2-20260828` forward without changing `DESIGN_DOC_ID`; preserve all closed folds; float `DESIGN_SHA256`; define kind-2 result semantics and bind every non-open root status to durable proof evidence (or keep it open/recomputable); then reissue the review request. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only game-solver design review
- migration/backfill/destructive-write/canonical-data-repair: no — no repository or production-data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts only
- AI-or-automation-acts-downstream: no — no runtime action is authorized
- worker/scheduler/queue/retry/async-side-effect: yes — shared-TT exact cutoffs and checkpoint recovery require exact truth contracts
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver certificates consume the theory-owned checker contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this cycle
- test-runtime-role-mismatch: yes — the current TT/checkpoint representations cannot distinguish all declared runtime states
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — this verdict refuses the residuals and accepts no waiver
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming revision-5 DESIGN relay exact-file lint passed; its requested SHA-256 exactly matches the current 356-line design.
- E1: complete revision-5 design, GRILL_LOCK, boundary contract, prior review, kernel result surfaces, and persistent schemas reviewed.
- E1: enumerating kind-2's live fields finds no exact-result polarity or normalized-claim lookup rule.
- E1: move absence is shared by true opponent/terminal proofs and can therefore not substitute for result polarity.
- E1: finite proof cost plus flag-tunable K contradicts “always exceeds any K,” and psm-v1 has no matching-root-record cross-field gate.
- E1: daemon was ready and active with zero conflicts and zero pending renders before filing.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this DESIGN-REVIEW relay for daemon filing; reviewed design remains untracked at the pinned digest
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-194010.md
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
