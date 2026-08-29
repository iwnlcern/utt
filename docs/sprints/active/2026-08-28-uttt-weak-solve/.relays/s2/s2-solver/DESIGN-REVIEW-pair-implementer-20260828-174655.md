ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c2-design-review-1
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the Planner can close the named contract and soundness gaps without changing the operator's GRILL decisions; any decision to keep the objective-switch constant operator-owned must instead be surfaced explicitly
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-pair-planner-20260828-173611.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 39cf01f6dbd9d03b17671d5c018966faad5bb1f3d2f95e1837adbcfee45685d3
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c2
OWNER: solver pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-solver-c2 rev1 — architecture retained; objective, spike, persistence, census, and locked-seam gaps

## Scope and identity

Reviewed the complete 168-line `DD-solver-c2-20260828` at SHA-256 `39cf01f6dbd9d03b17671d5c018966faad5bb1f3d2f95e1837adbcfee45685d3`, its GRILL_LOCK, the reconciled A1–A7/O4–O5/R-s2-1 inputs, both solver c1 audits, the current engine rules/AltSolver interfaces, and the already-approved theory contract. No source, test, fixture, design, branch, commit, or PR change was made. The selected pluggable-kernel/local-first architecture and G1–G9 operator choices are not rejected.

## Verdict

DESIGN_REVIEW_VERDICT: must-revise

The design has the right architecture shape but does not yet satisfy the dispatch's falsifiable-spike, persistent-byte/crash-story, census-format, or locked-boundary acceptance criteria. The following revisions are blocking and bounded.

## MR-1 — consume the locked theory contract exactly and correct state identity

Blocking evidence:

- Solver §2 says the identity is 38 bytes. The normative theory serialization is 37 bytes: 18 bytes X + 18 bytes O + one forced byte (`DD-theory-c2-20260828.md:47-56`). R-s2-1 excludes TieState and a stored side.
- Solver §2/§9 still says D4/schema bind "once theory locks" and gives `CertificateSink` a D4 transform annotation and full reply set. The contract was already approved and byte-stable before this design request (`RECONCILE.md:64-72`); theory pins `d4-lexmin-v1`, stores moves in the canonical frame, and expressly carries no edge transform annotation (`DD-theory-c2-20260828.md:80-91`). OPPONENT records carry no move/reply payload because the checker enumerates replies (`:122-134`); claims are WIN_X, WIN_O, NOLOSS_X, NOLOSS_O (`:104-120`).
- `Position::from_parts` requires a TieState and rejects `NullFirstMove` after any mark, but it does not enforce s2 parity (`engine/src/core/position.cpp:95-134`). The adapter must therefore specify the unique derived compatibility TieState and make wrong-mover application inexpressible while keeping TieState outside identity.

Required revision:

Pin the approved theory design (`DD-theory-c2-20260828`, SHA-256 `629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c`, commit `d692ac2`), RULESET_ID/version and SYMMETRY_ID. Use the exact 37-byte state. Replace the generic sink with a claim/node-kind seam matching the locked contract: canonical state; claim; PROVER move or no payload for OPPONENT/TERMINAL; no serialized transform annotation or full-reply list. Define root/non-root compatibility TieState construction and an apply API that derives the mover from parity.

## MR-2 — close the dual-objective completion state machine

Blocking evidence: solver §3 permits suspending WIN_X before it is disproven, switching to NOLOSS_X, and says nothing about resuming the first objective. A completed NOLOSS_X proof alone cannot distinguish X-win from draw. Theory requires draw to carry both NOLOSS_X and NOLOSS_O (`DD-theory-c2-20260828.md:108-120`). "Objectives share the store; no work is discarded" is also unproven because facts are objective-keyed and cross-objective implications are unspecified.

Required revision:

Lock the exact outcome machine and extraction mapping:

- WIN_X proven -> WIN_X certificate and X-win verdict.
- WIN_X disproven -> NOLOSS_O certificate.
- NOLOSS_X proven -> NOLOSS_X certificate; draw only after NOLOSS_O also exists.
- NOLOSS_X disproven -> WIN_O certificate and O-win verdict.

An early scheduling switch with WIN_X incomplete must retain and later resume the missing obligation; it cannot close a verdict. Define the ProvenStore key by canonical state plus claim/objective, list the only sound cross-claim implications, and narrow "no work discarded" to what is actually reusable. The design currently calls the switch ratio pre-committed while GRILL_LOCK leaves its constant open. Either lock a default/calibration protocol before outcome-bearing runs, or describe it only as a scheduling heuristic with no completion semantics.

## MR-3 — make the A-vs-B spike falsifiable and exact

Blocking evidence: §4 mixes three root-orbit positions "solved to fixed sub-depths" with sampled positions "solved to terminal," then compares median time-to-proof. A fixed-depth nonterminal search is not an exact proof without a named exact boundary oracle. The three-day fallback "shrink probe depths" mutates the pre-registered set, and no rule covers timeouts/censored pairs. "Identical stores" also does not say whether the second kernel inherits the first kernel's facts.

Required revision:

Pre-register exact terminal-solvable subgames (or a separately validated exact boundary table), the reachability/canonical filtering and PRNG algorithm/seed, duplicate/terminal rejection, fresh byte-identical store snapshots per kernel, run order/repetitions, cache state, timeout, and censored-result handling. Define the >=2x statistic over completed paired observations and the no-decision/default outcome when coverage is insufficient. A fallback may select a predeclared easier probe tier; it may not silently turn horizon values into proofs or rewrite the comparison after observing results.

## MR-4 — finish the persistent-byte and crash-publication contract

Blocking evidence:

- The 48-byte ProvenStore record is not byte-owned: the document does not assign exact offsets/widths/tags, endian, checksum algorithm/scope, reserved-byte rules, duplicate/conflict behavior, or witness frame. The 38-byte identity error also makes its arithmetic ambiguous.
- "Truth lives only in ProvenStore" and "nothing exact lives in the TT un-logged" conflict with threshold-K facts that are exact but deliberately unlogged (§3/§5). "In the OS buffer" is not the stated fsync durability boundary, yet callers may observe the claim before the batch is durable.
- Torn-log truncation is covered, but crash-safe layer compaction/publication, checksums for immutable layers, fsync/rename/directory ordering, superseded-layer retention, and manifest replacement are absent. "Background" compaction also conflicts with single-threaded c3 unless its execution model is named.
- The checkpoint hashes only compiled `.cpp` sources, omitting headers, compiler/configuration, executable identity, record-format version, and the now-locked theory contract. Its singular objective conflicts with the objective-switch/shared-store design, and "completed epoch is a no-op" has no durable completion marker.
- A 16-byte entry format does not itself make a 128-bit store portable or tear-free; synchronization must specify alignment/platform requirements and a correctness-preserving fallback independent of the wire layout.

Required revision:

Define every byte and state transition for log, compacted layer, and manifest; separate ephemeral exact results from durable proven facts; make above-K claims observable only after the declared durability barrier; define atomic compaction publication and conflict refusal; and bind resume to the full rules dependency/build identity, record/canonicalizer versions, approved theory contract identity, root/claim set, and a durable epoch-completion record. Keep the 16-byte TT layout if desired, but specify a non-lock-free fallback and never make exactness depend on TT persistence.

## MR-5 — specify the census contract rather than a field sketch

Blocking evidence: §8 says "one JSON/CSV format" and lists field names, but chooses neither encoding nor schema/version, gives no integer widths or exact definitions, and omits the dispatch's explicit raw/legal counts. At this scale `path_count` can exceed ordinary JSON integer precision. Theory cannot independently consume an ambiguous transposition ratio or cumulative sum.

Required revision:

Choose one canonical machine format and define schema id/version, row ordering, integer encoding, and exact meanings for raw paths, legal paths, reachable distinct states, terminal-by-kind states, transposed duplicates, D4 distinct states, per-layer/cumulative values, and ratios. Include ruleset/symmetry/build pins plus output digest and deterministic rerun criteria. Retain the audited ply-6/8 anchors, but state which fields each anchor checks.

## MR-6 — do not use AltSolver horizon estimates as exact WDL truth

Blocking evidence: §10 requires solver WDL to equal AltSolver "on all positions to fixed shallow horizons." AltSolver returns `Quality::Estimate` at a nonterminal horizon (`engine/src/root/alt_solver.hpp:132-166`) and propagates that quality upward (`:172-188`). It is exact only for fully terminal-resolved subtrees. Likewise, kernel A/B agreement cannot be asserted for timed-out probe members.

Required revision:

Split the oracle gate: bit-match legal moves, apply, terminal, and parity on all shallow positions; compare WDL only where AltSolver returns complete+Exact or where a separately exact terminal-solvable corpus exists; require estimates never enter ProvenStore or certificate extraction. Define cross-kernel agreement over completed exact probe pairs and report incomplete/out-of-resource pairs separately.

## Acceptance-criteria disposition

1. Falsifiable A-vs-B procedure: FAIL — MR-3.
2. Every persistent byte owned with a crash story: FAIL — MR-4.
3. Census output format theory can consume: FAIL — MR-5.
4. Traceability to audits/rulings/measurements: PARTIAL PASS — citations are extensive, but MR-1 and MR-6 contradict the locked/current evidence they cite.
5. Design identity, GRILL_LOCK, rejected alternatives, non-goals, and boundary consumers: PASS as structure; boundary payload remains must-revise under MR-1/MR-2.

## Required successor

Amend `DD-solver-c2-20260828` forward without changing `DESIGN_DOC_ID`; preserve the selected architecture and GRILL decisions, float `DESIGN_SHA256`, and reissue the DESIGN-REVIEW request with incremented lineage. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none — all six revisions enforce existing rulings and operator choices. If the Planner elects to keep the objective-switch ratio as a fresh operator-owned prerequisite instead of defining a nonbinding calibration protocol, the successor must set the human gate and route that one question explicitly.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only game-solver design review
- migration/backfill/destructive-write/canonical-data-repair: no — no repository or production-data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts only
- AI-or-automation-acts-downstream: no — no runtime action is authorized
- worker/scheduler/queue/retry/async-side-effect: yes — c4 threads and background persistent-store compaction require an explicit crash/publication contract
- cross-repo/service-contract/generated-schema/shared-API-event: yes — the theory checker/solver emitter schema and D4 convention are shared contracts
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control in this cycle
- test-runtime-role-mismatch: yes — AltSolver Estimate results are currently described as an exact WDL oracle
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — this verdict refuses the gaps; it accepts no residual risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming DESIGN relay exact-file lint passed; requested design digest exactly matches current bytes.
- E1: `RECONCILE.md` R2 pins the approved theory contract at commit `d692ac2`, SHA-256 `629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c`.
- E1: theory §2 proves the state is 37 bytes and pins D4/move-frame semantics; theory §3/§7 pins the four claim/verdict combinations.
- E1: engine source proves `from_parts`' TieState/forced checks and missing parity check; AltSolver source proves horizon estimates are not exact.
- E1: daemon is ready, active, with zero conflicts and zero pending renders; root lint has inherited/foreign historical errors plus unresolved uncommitted artifact locators, reported separately and not repaired by this seat.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this DESIGN-REVIEW relay for daemon filing; reviewed design remains untracked at the pinned digest
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
