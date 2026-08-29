ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c2-design-review-4
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the remaining defects are bounded TT-layout and persistent-store schema corrections under the existing architecture and rulings
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-pair-planner-20260828-193327.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 77d248f4c3c4134637bb6bab8f7dca78534ea6211d3950f37086432bdf83e6ac
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c2
OWNER: solver pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-solver-c2 rev4 — prior residuals close; TT encoding and store topology remain

## Scope and identity

Reviewed all 334 lines of revision-4 `DD-solver-c2-20260828` at SHA-256 `77d248f4c3c4134637bb6bab8f7dca78534ea6211d3950f37086432bdf83e6ac`, the exact successor request, revision-3 Implementer review, the storage/collision evidence, complete GRILL_LOCK, and boundary contract. No source, test, fixture, design, branch, commit, or PR change was made. The architecture and every previously closed fold remain accepted.

## Verdict

DESIGN_REVIEW_VERDICT: must-revise

Revision 4 closes all three requested revision-3 corrections: the df-pn switch now uses proof hardness with terminal boundaries outside the heuristic; the common stripe lock makes entry/sidecar publication race-free; and immutable manifests plus `CURRENT`, fallback, retention, and recovery tests make prior-generation selection real. Three concrete representation contradictions remain. They are codebase-answerable and do not require a new operator decision.

## MR-4A-layout residual — the locked 16-byte TT entry has no representable encoding

Blocking evidence: §5 locks one 16-byte entry containing a 56-bit tag, claim/bound bits, a generation, and kernel payload, where kernel A's payload is a PN pair (`DD-solver-c2-20260828.md:94-101`). §3 simultaneously defines PN/DN arithmetic over unsigned 64-bit saturated finite values plus distinct infinity/terminal handling (`:58-59`). Two uncompressed 64-bit numbers already consume all 128 entry bits, before the 56-bit tag or any metadata. Unlike `psr-v1` and `psl-v1`, no byte/bit allocation or compressed-number encoding explains how the locked representation fits or preserves the stated comparisons.

Required revision: either specify the complete 16-byte entry layout and the loss-preserving/saturating PN/DN encoding, including the representations of zero, finite saturation, infinity/terminal state, claim/bound bits, move, and generation, or drop the 16-byte lock and recalculate the per-way memory budget. Retain the now-correct stripe-lock publication rule. Add round-trip/boundary tests for whichever encoding is locked.

## MR-4B-root residual — psm-v1 excludes the initial root's durable layer

Blocking evidence: compaction maps every ProvenStore record to a per-ply layer derived from state popcount and probing selects the layer of the queried state's ply (`DD-solver-c2-20260828.md:128-144`). The weak-solve root is the empty state at ply 0, and a completed root proof is a durable proven fact under the cost threshold contract. Yet the strict psm-v1 `layers[].ply` range is `1..81` (`:191-193`). The format therefore cannot name a compacted root record, and post-compaction root probing/extraction has no valid ply-0 layer path.

Required revision: admit ply 0 in the layer schema and psl header checks, or explicitly define a different durable root-record location and its probe/extraction/recovery behavior. Add a ply-0 compaction/resume/extraction test.

## MR-4B-log residual — the schema cannot carry the segment digests its recovery contract requires

Blocking evidence: the manifest pin list promises per-layer and per-segment file lists with digests, and resume says it verifies file digests (`DD-solver-c2-20260828.md:151-159,210-211`). The strict, unknown-key-refusing schema gives layers `digest_xxh128` but gives each log segment only `file` and `durable_bytes` (`:191-203`). Recovery's “schema, digests, named files” validation consequently has no whole-segment digest to check, and adding one during implementation would itself violate psm-v1.

Required revision: add an exact log-segment digest field and define whether it covers exactly `durable_bytes` or the whole file, including torn-tail behavior; alternatively remove the whole-segment-digest claim and define record-checksum validation as the sole log integrity rule consistently across the pin list, publication, recovery, and tests. Preserve `durable_bytes` alignment and bounds.

## Fold disposition

- MR-1: CLOSED — theory identity, adapter, D4, move frame, and certificate seam remain correct.
- MR-2: CLOSED — outcome completion, kernel-B threshold, and the corrected df-pn proof-hardness direction are coherent.
- MR-3: CLOSED — the exact falsifiable spike remains fixed.
- MR-4A-publication: CLOSED — common stripe locking for every probe/store prevents the mixed-generation exact hit and sidecar data race.
- MR-4A-layout: PARTIAL — the locked 16-byte entry cannot yet be implemented from a complete encoding.
- MR-4B-recovery: CLOSED — immutable generations, atomic pointer publication, fallback, first-start behavior, retention, and crash tests are defined.
- MR-4B-schema: PARTIAL — ply 0 is excluded and promised log-segment digests are absent.
- MR-5: CLOSED — census contract remains consumable and anchored.
- MR-6: CLOSED — AltSolver exactness boundary and Estimate exclusion remain correct.

## Acceptance-criteria disposition

1. Falsifiable A-vs-B procedure: PASS.
2. Every persistent byte owned with a crash story: FAIL — the strict manifest cannot name a root layer or the promised log-segment integrity value.
3. Census output format theory can consume: PASS.
4. Traceability to audits/rulings/measurements: PASS.
5. Design identity, GRILL_LOCK, rejected alternatives, non-goals, and boundary consumers: PASS as structure; TT representation and persistent-store topology remain blocked as above.

## GRILL pressure result

Decision pressure: the pluggable-kernel architecture, exactness separation, corrected scheduling rule, and lock-based publication survive. Alternative pressure: no rejected architecture becomes preferable; these are local representation repairs. Boundary pressure: the certificate seam remains intact, but checkpoint bytes do not yet provide an end-to-end representable root/log contract. No operator question is exposed.

## Required successor

Amend `DD-solver-c2-20260828` forward without changing `DESIGN_DOC_ID`; preserve all closed folds; float `DESIGN_SHA256`; repair only the TT encoding, ply-0 durable-root topology, and log-integrity contradiction; then reissue the review request. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only game-solver design review
- migration/backfill/destructive-write/canonical-data-repair: no — no repository or production-data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts only
- AI-or-automation-acts-downstream: no — no runtime action is authorized
- worker/scheduler/queue/retry/async-side-effect: yes — c4 shared-TT publication and checkpoint durability require exact concurrency/crash contracts
- cross-repo/service-contract/generated-schema/shared-API-event: yes — solver certificates consume the theory-owned checker contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this cycle
- test-runtime-role-mismatch: yes — the current locked TT/store representations cannot satisfy their own declared boundary cases
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
- E1: incoming revision-4 DESIGN relay exact-file lint passed; its requested SHA-256 exactly matches the current 334-line design.
- E1: complete revision-4 design, GRILL_LOCK, boundary contract, prior review, TT implementation pattern, and persistent schemas reviewed.
- E1: field-capacity arithmetic shows the stated uncompressed PN pair alone consumes the full 16-byte entry before tag/metadata.
- E1: the empty initial state has ply 0, while psm-v1 admits only layer plies 1..81.
- E1: psm-v1's exact `log_segments` member omits the digest promised by the pin and recovery prose.
- E1: daemon was ready and active with zero conflicts and zero pending renders before filing.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this DESIGN-REVIEW relay for daemon filing; reviewed design remains untracked at the pinned digest
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-185204.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-193327.md
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
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192257.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192509.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
