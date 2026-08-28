ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-design-review-2
PARENT_DISPATCH_ID: s2-theory-c2-design-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the Planner can close both gaps without changing operator decisions G1-G8
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-pair-planner-20260828-022430.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: a97664537b93615f04e55a1bcb85d8feec2f30845a6392b682033c8d8ee55e5c
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-theory-c2 — sound proof model; incomplete wire contract and fail-closed matrix

## Scope and identity

Reviewed `docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-theory-c2-20260828.md` at commit `6ebb8e34599aad8e966a3368df4c8d9f73d031b1`. Its working bytes match the pinned SHA-256 and its Git diff is empty. I reviewed the target entity, boundary contract, acceptance criteria, rulings R-s2-1..4, GRILL decisions G1-G8, rejected alternatives, and open/operator items. No source, test, fixture, design, branch, commit, or PR change was made.

## Acceptance criteria

1. Emit and verify from this document alone: **FAIL — MR-1**. State and fixed-record bytes are precise; the enclosing certificate and verdict-manifest bytes are not.
2. Every leaf class names its trust base and verification obligation: **PASS** (`DD` lines 121-145, 201-216).
3. Draw semantics survive adversarial read: **PASS**. Separate full `NOLOSS_X` and `NOLOSS_O` artifacts prevent one drawing move from proving the opponent cannot win (`DD` lines 105-119, 234-245).
4. Canonical state byte agreement: **PASS** (`DD` lines 23-100). Independent derivation reproduced all eight D4 tables as distinct bijections closed under composition. Across 11,208 legal playout states, all eight transforms preserved results, legal moves, and transitions: 89,664 transform-state checks passed (E2).

## MR-1 — self-contained cross-pair wire contract

Evidence: §4.3 lists header fields but defers their byte packing to theory-c3 (`DD` lines 174-191). The manifest digest covers undefined "serialized manifest rows" (185-187); the header, chunk envelope, and manifest rows have no packing/framing; RULE `u16`/`u32` byte order is unstated (218-221); and the verdict manifest has fields but no encoding/path/framing rules (234-245). XXH3-128 also lacks a selected seed, canonical 16-byte representation, and exact byte coverage except for the Appendix-A digest input.

This blocks approval because the schema, not the checker implementation, must be the shared format authority. Otherwise an independent solver emitter cannot implement from the design alone.

Required revision:

- Specify complete header, chunk-envelope, manifest-row, RULE-section, and verdict-manifest encodings.
- Specify widths/endianness, enum tags, string/path encoding and bounds, framing/alignment, offset origin, ordinal/order constraints, totals semantics, and unknown/trailing-field handling.
- Specify the exact XXH3-128 variant/seed, 16-byte representation, and coverage for every digest.
- Add one byte-exact minimal-terminal certificate and verdict-manifest golden vector.

Implementation details may remain in theory-c3, but no cross-pair wire choice may remain there.

## MR-2 — align named mutants with the fail-closed claim

Evidence: §§8-9 claim one named mutant for every fail-closed path (`DD` lines 247-264), but MUT-01..15 omit explicit obligations including unsupported schema version, ruleset-digest mismatch, unknown symmetry id, non-empty v0 `rule_ids_used`, manifest-digest mismatch, nonzero reserved byte, non-`0xFF` non-PROVER payload, manifest offset/overlap/order/totals faults, trailing bytes, and verdict-member root/ruleset/schema/symmetry mismatches. `MUT-10` cannot exercise malformed-witness or unknown-version validation while v0 refuses any rule use before streaming.

Required revision: add a normative requirement-to-positive/mutant trace table, then add missing mutants or narrow "every fail-closed path" to an exact v0 surface. Gate future validator mutants to the minor version that first registers a rule; retain a v0 header-refusal mutant.

## Confirmed surfaces

- Appendix A matches the ROADMAP rule lock and engine lifecycle ordering (`ROADMAP.md` lines 14-25; `engine/src/core/position.cpp` lines 28-88).
- Full canonical bytes, not hashes, carry identity; digest collision does not enter proof soundness.
- Dual-boolean claims do not hard-code the value conjecture and correctly close the draw trap.
- Empty v0 rule registry, fresh C3 rules, fixtures-only sharing, and checker-never-searches preserve independence.
- c3→c4→c5→c6 sequencing respects ROADMAP gates 1-2.

## Required successor

Amend `DD-theory-c2-20260828` forward, retain its `DESIGN_DOC_ID`, publish the successor digest, and reissue the DESIGN-REVIEW request with incremented lineage. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none — G1-G8 stand; both findings close within the selected architecture.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only design review
- migration/backfill/destructive-write/canonical-data-repair: no — no data actions
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifact only
- AI-or-automation-acts-downstream: no — static certificate validation only
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — binding theory-checker/solver-emitter schema
- user-visible-control-with-materializer/downstream-consumer: no — none in this cycle
- test-runtime-role-mismatch: no — independence boundary is explicit
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — this verdict refuses residual format ambiguity
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: design digest matches; design diff empty at HEAD `6ebb8e34599aad8e966a3368df4c8d9f73d031b1`.
- E1: incoming DESIGN relay exact-file lint passes with historical freshness disabled; index lint passes.
- E2: independent D4 table/group and legal-transition covariance checks pass as above.
- Root-wide relay lint has inherited boot/draft/report-format findings outside this response; exact-file lint and daemon verification are the response gates.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this review relay for daemon filing; design diff empty at HEAD 6ebb8e34599aad8e966a3368df4c8d9f73d031b1
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-fold-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-report-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-p1c-judgment-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-p1c-judgment-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-3/
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260828-022430.md
