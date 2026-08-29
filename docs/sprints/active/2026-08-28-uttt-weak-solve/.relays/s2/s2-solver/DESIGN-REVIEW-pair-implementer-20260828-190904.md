ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c2-design-review-3
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the remaining defects are bounded algorithm-direction, concurrency-publication, and manifest-schema corrections under existing rulings
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-pair-planner-20260828-185204.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 7db7df40044ffd1fc0a9bfe14f7e22c822f5ca7f75601a6ad0e43e5dafbb844e
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c2
OWNER: solver pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-solver-c2 rev3 — layer contract closes; PN direction, TT pair publication, and manifest recovery remain

## Scope and identity

Reviewed all 286 lines of revision-3 `DD-solver-c2-20260828` at SHA-256 `7db7df40044ffd1fc0a9bfe14f7e22c822f5ca7f75601a6ad0e43e5dafbb844e`, the exact successor request, both prior Implementer reviews, the approved theory contract, the existing FullKey/AltSolver collision patterns, and the complete GRILL_LOCK/boundary contract. No source, test, fixture, design, branch, commit, or PR change was made. The architecture, closed folds, psl-v1 layer format, deterministic witness dedupe, and claim-lattice refusal are accepted.

## Verdict

DESIGN_REVIEW_VERDICT: must-revise

Revision 3 materially closes the persistent-layer half of MR-4B and makes full identity mandatory for exact TT use. Three narrow contradictions remain: the df-pn switch direction is inverted, the lock-free entry/sidecar pair has an exact false-hit interleaving, and psm-v1 still lacks both a literal complete schema and the recovery mechanism its previous-manifest claim requires.

## MR-2 residual — the df-pn signal points away from switching

Blocking evidence: §3 defines `DN(root) / PN(root) ≥ 10` and says larger favors suspending the WIN_X proof attempt (`DD-solver-c2-20260828.md:56-63`). In proof-number search, PN is the estimated leaf work to prove the goal and DN is the estimated leaf work to disprove it; a large DN/PN therefore says disproof is harder relative to proof and favors continuing the proof, the opposite of the kernel-B signal. The standard definitions and terminal convention are summarized in Kishimoto et al., *Game-Tree Search Using Proof Numbers: The First Twenty Years* (ICGA Journal 35(3), 2012).

Required revision: use the proof-hardness direction, for example `PN(root) / DN(root) ≥ 10`, or an overflow-safe equivalent comparison, and define handling for 0/infinity/saturation. Retain the two-checkpoint rule and the already-correct scheduling-only/nonbinding semantics.

## MR-4A residual — entry-last publication is not a collision-safe atomic pair

Blocking evidence: §5 stores the exact payload in one 16-byte atomic entry and the 37-byte identity in a separate sidecar, then says writing the sidecar first and entry last makes a torn pair read as a miss (`DD-solver-c2-20260828.md:92-100`). Consider replacement of state A by distinct state B with the same forced 56-bit tag: after sidecar B is visible but before entry B publishes, a B reader can observe sidecar B plus old entry A, accept the full-identity check, and consume A's payload as exact. Reading/writing the non-atomic 37-byte sidecar concurrently is also not made race-free by atomicity of the separate entry. The mandated same-tag test does not yet require this replacement interleaving.

Required revision: make entry plus sidecar one race-free publication protocol. The simple accepted option is taking the same stripe lock for both readers and writers in every mode, even when the 16-byte entry itself is lock-free. A lock-free option must name a version/seqlock or equivalent validation protocol, atomic/race-free sidecar accesses, memory orders, and the double-check that rejects every mixed generation. Extend the forced-collision test to pause a replacement between sidecar and entry publication and prove both A and B probes can only hit a matched generation or miss.

## MR-4B residual — psm-v1 is not yet a complete or recoverable manifest contract

Blocking evidence:

- §6 says every key “named in the pin list” is required, but the list is prose rather than a literal JSON topology: it does not assign exact key names and nesting for compiler/build identity, claim-status objects, layers/segments, digests, offsets, or normalized targets (`DD-solver-c2-20260828.md:145-160`). “Objects/arrays only where a list is pinned” is not an independently implementable schema, and most numeric domains are described only as “far below 2^53,” not by exact inclusive ranges.
- The design uses one manifest path replaced by `rename`, yet promises that on any refusal the previous durable manifest remains authoritative and that publication guarantees one always exists (`:147-163`). Replacing a single pathname unlinks the previous generation; after restart, a refused successor has no retained prior manifest to select. The retained old store files do not reconstruct the lost manifest.

Required revision: write the complete psm-v1 schema as literal key names, exact scalar types/enums/ranges, nested object/array member shapes, ordering/canonical-byte rule if byte determinism is claimed, and cross-field constraints such as unique file names, layer ply matching, digest algorithm/length, durable-offset bounds, and normalized-target consistency. Then either retain immutable versioned manifests plus an atomically published current pointer with a defined newest-valid fallback rule, or retain a crash-safe previous generation explicitly; name the first-checkpoint/no-previous case and test corrupt-new/valid-old recovery. A single overwrite path may instead be retained only if the “previous remains authoritative” recovery claim is removed and the resulting fail-stop behavior is explicitly accepted by the owner.

## Fold disposition

- MR-1: CLOSED — exact theory identity, adapter, D4, move frame, and certificate seam remain correct.
- MR-2: PARTIAL — outcome completion and kernel-B threshold are closed; df-pn switch direction remains inverted.
- MR-3: CLOSED — exact falsifiable spike procedure remains fixed.
- MR-4A: PARTIAL — full sidecar equality is correctly required for exact use; paired concurrent publication remains unsound.
- MR-4B: PARTIAL — psl-v1 bytes, deterministic dedupe, and complete claim-set consistency are closed; psm-v1 topology and prior-generation recovery remain open.
- MR-5: CLOSED — census contract remains consumable and anchored.
- MR-6: CLOSED — AltSolver exactness boundary and Estimate exclusion remain correct.

## Acceptance-criteria disposition

1. Falsifiable A-vs-B procedure: PASS.
2. Every persistent byte owned with a crash story: FAIL — psm-v1 topology and fallback publication are incomplete.
3. Census output format theory can consume: PASS.
4. Traceability to audits/rulings/measurements: PASS.
5. Design identity, GRILL_LOCK, rejected alternatives, non-goals, and boundary consumers: PASS as structure; exact concurrent TT behavior remains blocked by MR-4A.

## GRILL pressure result

The required pressure pass replayed the two adversarial boundaries introduced by this revision. The same-tag A→B replacement produces a mixed sidecar/payload generation under entry-last publication; a corrupt or schema-refused renamed manifest leaves no previous pathname despite the recovery promise. Both are codebase-answerable and require no new operator preference. The psl-v1 record/layer path survived the pressure pass.

## Required successor

Amend `DD-solver-c2-20260828` forward without changing `DESIGN_DOC_ID`; preserve the architecture and all closed folds; float `DESIGN_SHA256`; and reissue the review request. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only game-solver design review
- migration/backfill/destructive-write/canonical-data-repair: no — no repository or production-data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts only
- AI-or-automation-acts-downstream: no — no runtime action is authorized
- worker/scheduler/queue/retry/async-side-effect: yes — c4 shared-TT publication and checkpoint replacement require exact concurrency/crash contracts
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver certificates consume the theory-owned checker contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this cycle
- test-runtime-role-mismatch: yes — the present TT publication claim permits a mixed-generation exact hit
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
- E1: incoming revision-3 DESIGN relay exact-file lint passed; its requested SHA-256 exactly matches the current 286-line design.
- E1: complete revision-3 design, GRILL_LOCK, boundary contract, and prior fold lineage reviewed.
- E1: proof-number literature confirms PN is proof effort and DN is disproof effort; the stated ratio direction contradicts the intended switch meaning.
- E1: the same-tag replacement interleaving and single-path rename recovery were checked directly against §§5–6's stated state transitions.
- E1: daemon was ready and active with zero conflicts and zero pending renders before filing.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this DESIGN-REVIEW relay for daemon filing; reviewed design remains untracked at the pinned digest
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-185204.md
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
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
