ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c2-design-review-2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the remaining defects are bounded consistency and byte-contract corrections under the existing architecture and operator rulings
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-pair-planner-20260828-180344.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: db5de0e9af5672ba92275de50a89a3ef8f1b7d7b91948888e3269776af9ec6c3
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c2
OWNER: solver pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-solver-c2 rev2 — four folds closed; TT collision exactness and persistent-byte residuals remain

## Scope and identity

Reviewed the complete 261-line revision-2 `DD-solver-c2-20260828` at SHA-256 `db5de0e9af5672ba92275de50a89a3ef8f1b7d7b91948888e3269776af9ec6c3`, the exact successor request, the prior six-item review, the approved theory contract, and the current engine TT/AltSolver collision behavior. No source, test, fixture, design, branch, commit, or PR change was made. The selected pluggable-kernel/local-first architecture and operator GRILL choices remain accepted.

## Verdict

DESIGN_REVIEW_VERDICT: must-revise

Revision 2 closes MR-1, MR-3, MR-5, and MR-6 and closes the outcome-completion part of MR-2. Two soundness/durability residuals and one self-contradictory default remain. They are bounded amendments, not grounds to reject the architecture.

## MR-2 residual — make the default switch heuristic executable

Blocking evidence: §3 defines the kernel-B signal as a `refuted-root-children fraction` and requires it to be sustained `above 10` (`DD-solver-c2-20260828.md:56-59`). A fraction is bounded by 1, so this branch of the stated default can never fire. The kernel-A ratio direction is named but not tied to an explicit switch-favoring inequality, making the two alternatives incomparable in the calibration record.

Required revision: express the B threshold as an attainable fraction or percentage, and define the A numerator/denominator plus the direction that favors switching. Retain the already-correct rule that the signal is scheduling-only and cannot close an obligation or verdict.

## MR-4A residual — a 56-bit TT tag cannot authorize an exact hit

Blocking evidence:

- §2 says hashes are never identity (`DD-solver-c2-20260828.md:22-25`), while §5 gives the working TT only a 56-bit tag and says exactness may depend on tag-match (`:88-99`). Distinct canonical states can share that tag. A false hit is therefore possible, not only the false miss named in §5.
- Kernel B stores a bound and move and uses exact boolean cutoffs; a colliding hit can return or cut off the wrong state and then contaminate a `proven`/`disproven` result or durable fact before the independent checker detects the damage. Re-derivability does not make the search result exact if re-derivation was skipped because of the false hit.
- The current engine separates tag-only play mode from a full-identity sidecar mode (`engine/src/search/tt.hpp:63-112`, `engine/src/search/tt.cpp:39-67`) and forces distinct states onto one hash/tag in its collision test (`engine/tests/test_tt.cpp:207-240`). AltSolver likewise compares the complete semantic key inside a collision bucket (`engine/src/root/alt_solver.hpp:62-72,113-129`).

Required revision: choose and lock one collision-safe rule. Either a TT hit that lacks full 37-byte identity is heuristic-only and may affect ordering/threshold scheduling but never establish a proof, disproof, exact bound, cutoff, ProvenStore append, or kernel return; or exact-bearing hits carry and compare full identity through a sidecar/second-level structure. Add a forced-same-tag/different-state test for both kernels proving equivalence to an empty-TT search and proving no false durable fact is emitted.

## MR-4B residual — finish the compacted-layer and manifest byte contracts

Blocking evidence:

- The 48-byte log record is now byte-owned, but §5 does not define the bytes of a `.psl` layer: header versus headerless representation, exact record sequence, length/count validation, or parse/refusal rules are absent (`DD-solver-c2-20260828.md:101-121`). The manifest names a `layer-format version` that the design never defines (`:124-138`).
- Compaction sorts only by `(ply,state,claim)` while allowing different witnesses and keeping the `first in sort order` (`:112-120`). Witness is outside that sort key, so two valid input orders can select different output bytes. That conflicts with the stated byte-owned, deterministic artifact model.
- The named contradiction check rejects the two complement pairs but does not close the inference lattice against simultaneous `WIN_X` and `WIN_O`. That pair is impossible under the locked claim semantics unless monotone closure is materialized before conflict checking; no such ordering is stated (`:43-47,112-113`).
- The checkpoint is described as versioned JSON with a field list, but the required/optional key schema, JSON types/number ranges, unknown-key/version behavior, path confinement, and malformed/trailing-input refusal are not defined (`:124-138`). Those are resume-state semantics, not implementation trivia.

Required revision: define the layer format completely (or state explicitly that it is a strict concatenation of `psr-v1` records), including magic/version if any, exact ordering, length/count validation, checksum scope, and fail-closed parsing. Use a total deterministic duplicate order such as `(state,claim,witness)` with a named winner, and check claim consistency over inference closure so every impossible simultaneous proof set refuses. Define a versioned manifest schema with exact types/ranges, required keys, unknown-version/key policy, root-confined normalized file names, strict parse/exhaustion behavior, and the existing atomic publication transition.

## Fold disposition

- MR-1: CLOSED — exact 37-byte theory state, parity/TieState adapter, D4 tables, canonical move frame, and no serialized transform/reply/value payload are pinned.
- MR-2: PARTIAL — the outcome machine, claim keys, inference list, and open-obligation rule are closed; only the impossible default threshold above remains.
- MR-3: CLOSED — terminal-resolved seeded probes, isolation, censoring, coverage floor, fallback, and no-decision default are fixed before observation.
- MR-4: PARTIAL — log-record bytes, visibility barrier, publication order, manifest pins, and synchronization fallback are materially improved; MR-4A/MR-4B remain.
- MR-5: CLOSED — solver-census-v1 chooses CSV, unbounded decimal counts, exact columns/anchors, deterministic ordering, and output digest.
- MR-6: CLOSED — AltSolver is restricted to complete exact results; estimates and incomplete cross-kernel pairs are excluded structurally.

## Acceptance-criteria disposition

1. Falsifiable A-vs-B procedure: PASS.
2. Every persistent byte owned with a crash story: FAIL — MR-4B.
3. Census output format theory can consume: PASS.
4. Traceability to audits/rulings/measurements: PASS.
5. Design identity, GRILL_LOCK, rejected alternatives, non-goals, and boundary consumers: PASS as structure; exact search/store behavior remains blocked by MR-4A/MR-4B.

## GRILL pressure result

The revision was pressure-tested at the loss-of-work and false-identity seams. Kill-before/after-fdatasync and manifest-replacement transitions now have a coherent publication order. The surviving failure rehearsals are: a forced 56-bit tag collision can be consumed as exact; and two legal duplicate-witness input orders can compact to different bytes. Those failures are directly testable and are the basis of MR-4A/MR-4B. No new operator preference is needed.

## Required successor

Amend `DD-solver-c2-20260828` forward without changing `DESIGN_DOC_ID`; preserve the architecture, closed folds, and GRILL decisions; float `DESIGN_SHA256`; and reissue the review request. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only game-solver design review
- migration/backfill/destructive-write/canonical-data-repair: no — no repository or production-data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts only
- AI-or-automation-acts-downstream: no — no runtime action is authorized
- worker/scheduler/queue/retry/async-side-effect: yes — persistent compaction/checkpoint transitions and c4 shared-TT behavior require exact failure contracts
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver certificates consume the theory-owned checker contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this cycle
- test-runtime-role-mismatch: yes — tag-only lookup is presently allowed to participate in exact search semantics
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — this verdict refuses the residuals; it accepts no waiver
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming revision-2 DESIGN relay exact-file lint passed and its requested SHA-256 exactly matches the current design bytes.
- E1: complete revision-2 design, GRILL_LOCK, boundary contract, and every prior MR fold reviewed.
- E1: current TT code and collision tests establish the repo's existing distinction between tag-only lookup and full semantic identity; AltSolver collision buckets compare full identity.
- E1: daemon was ready and active with zero conflicts and zero pending renders before filing.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this DESIGN-REVIEW relay for daemon filing; reviewed design remains untracked at the pinned digest
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
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
