ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-erratum-design-review-2
PARENT_DISPATCH_ID: s2-theory-c2-erratum-design-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — this approval closes the pair design gate; fold, implementation, and merge remain separately authorized and gated
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-pair-planner-20260830-151704.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac
DESIGN_REVIEW_VERDICT: approve
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN-REVIEW erratum 1 rev2 — approve; MR-1 through MR-3 closed

## Scope and identity

Reviewed the complete 43-line revision-2 erratum at `main@950bc63c46521a300b78b2574cfe4ecdf1fa4967`. The working bytes and the Git object both recompute to SHA-256 `56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac`; commit `950bc63` changes only the erratum artifact from its reviewed revision-1 ancestor `eb07107`. The `DESIGN_DOC_ID` and erratum locator are unchanged, while the floated digest correctly identifies the amended bytes.

The immutable base DD is byte-unchanged between `eb07107` and `950bc63`; Appendix A and RULESET_DIGEST `16e85b36332493470fa1006516128579` therefore remain unchanged. No source, test, fixture, plan, branch, PR, merge, publication, deployment, or release change was made in this review.

## Verdict

DESIGN_REVIEW_VERDICT: approve

Revision 2 closes all three findings from `s2-theory/DESIGN-REVIEW-pair-implementer-20260830-042621.md` without changing the authorized `0..81` correction or introducing an operator-decision item.

## Finding dispositions

MR-1 — closed. The design now requires both boundary sides: a canonical, well-formed one-record ply-81 all-drawn NOLOSS terminal certificate passes in subgame mode, and a well-framed row with ply 82 rejects exactly `E_ROW_PLY_RANGE`. It explicitly explains why the positive side is necessary to catch a checker that incorrectly retains the old `> 80` guard. The emitter-side helper is bounded to battery-input tooling and does not add a committed golden artifact.

MR-2 — closed. The rationale is narrowed to the schema state domain anchored in base DD §§2.6 and 3.3. It establishes the admitted 81-mark terminal state without claiming a legal routed 81-move history from the canonical initial position, and it explicitly states that no routed witness was checked.

MR-3 — closed. The alignment text now accurately says §6 inherits §4.3's range by reference and identifies §9 MUT-H14 as the sole literal repetition. It explicitly retains `SCHEMA_VERSION 0.1` as a pre-release correction before checker merge or downstream emitter existence, rather than claiming an additive post-release compatibility change.

## Acceptance criteria status

1. PASS at design level — normative row range is `0..81`; `E_ROW_PLY_RANGE` means `ply > 81` and keeps its first-failure position.
2. PASS at design level — the fold must prove positive ply-81 acceptance and exact-code ply-82 rejection; the paired gates prevent the identified false green.
3. PASS at design level — MUT-H14 value/catalogue/digest regeneration is explicit, while all other committed vectors, mutants, and digests remain byte-identical.
4. PASS at design level — Appendix A and RULESET_DIGEST remain immutable.
5. PASS — the rationale disclaims routed reachability, the §6 locator matches the base DD, and schema-version disposition is explicit.
6. PASS — solver code remains out of scope; the later emitter consumes the approved cap.

Implementation-level E2 proof of these fold obligations is still required by the subsequent authorized fold; this design approval is not that proof and grants no implementation or merge authority.

## Boundary contract

Writes: the approved erratum overlay for the existing certificate manifest-row `ply` range; after separate fold authority, checker/corpus/test constants and the positive boundary helper.

Reads: immutable base DD rev3, current theory certificate schema/checker surfaces, and the later solver emitter boundary.

Target entity: `DD-theory-c2-20260828` as amended by `DD-theory-c2-20260828-erratum-1`.

Downstream consumers: theory checker and later solver emitter.

Contract: accept manifest-row plies 0 through 81 inclusive; reject values above 81 before row ordering; retain schema/ruleset identities and unrelated artifact bytes.

Proof: this approving design-review lineage plus the later fold's E2 positive-81/negative-82 checker gates and deterministic artifact census.

No-consumer action: not applicable — checker and emitter are commissioned, while solver implementation remains outside this erratum fold.

OPERATOR_ITEMS: none — the revision stays within the orchestrator-authorized value and surface and removes, rather than accepts, the unsupported reachability claim

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static schema review
- migration/backfill/destructive-write/canonical-data-repair: no — no data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifact only
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — shared theory-checker/solver-emitter certificate contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — paired fold gates exercise the checker boundary consumed by the emitter
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — exact one-value erratum with explicit non-claims and deferred implementation proof
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — large ceremony retained; no downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or used

Verification:
- E1: incoming DESIGN relay exact-file lint returned zero errors and zero warnings. Relay-root lint remains inherited-red on artifact probe-root resolution, including this erratum locator; manual Git-object, worktree, and SHA-256 checks independently match the pinned bytes.
- E1: `main`, `HEAD`, and the reviewed artifact resolve to `950bc63`; `eb07107` is its ancestor; the revision-2 commit changes only the erratum and the base DD is unchanged.
- E1: base DD §§2.6, 3.3, 4.1, 4.3, 6, and 9 agree with the revised state-domain rationale, by-reference §6 alignment, `SCHEMA_VERSION 0.1`, and MUT-H14 disposition.
- E2 incidental: a corrected parser-based in-memory probe on the theory-c3 worktree constructed a canonical, well-formed 41-X/40-O all-drawn NOLOSS terminal certificate with `forced=ANY`, one manifest row at ply 81, and 237 total bytes. This validates the proposed positive fixture construction but does not claim routed initial reachability or implementation completion.
- E2 infrastructure: `relay verify --root .` returned `ok: true`; no relay conflict was reported.

The Planner may now treat the erratum as the schema of record and proceed only through the already-authorized fold-2 path under FOLD_SCOPE discipline. This approval does not authorize source/test edits by this seat, a new implementation dispatch, PR mutation outside that fold, merge, publication, deployment, or release.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/merge/publication/deployment/release change claimed; reviewed main@950bc63c46521a300b78b2574cfe4ecdf1fa4967 and authored only this DESIGN-REVIEW draft for daemon filing; final status below attributes existing scheduler, daemon, and relay bytes
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-REVIEW-pair-implementer-20260830-042621.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260830-041021.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260830-151704.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035925.md
(daemon lock/INDEX and this cycle's relay chain awaiting orchestrator batch; taken immediately before this filing)
