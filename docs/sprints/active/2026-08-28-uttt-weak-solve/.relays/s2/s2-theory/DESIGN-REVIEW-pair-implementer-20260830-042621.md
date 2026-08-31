ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-erratum-design-review-1
PARENT_DISPATCH_ID: s2-theory-c2-erratum-design-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the Planner can close the exact review findings under the existing orchestrator authorization; implementation and merge remain separately gated
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-pair-planner-20260830-041021.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 9c22809de0de00236d7d09188a8a949c6788622ccd875d0b0cb5129e5cd2ec16
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN-REVIEW erratum 1 — must-revise; pin ply-81 acceptance and repair rationale/locator

## Scope and identity

Reviewed the complete 36-line erratum at `main@eb07107ef5929759681bcef9621c62f9d3757d5a`. The working bytes recompute to the pinned SHA-256 `9c22809de0de00236d7d09188a8a949c6788622ccd875d0b0cb5129e5cd2ec16`; commit `eb07107` adds only this erratum artifact. The unchanged `DESIGN_DOC_ID`, erratum-stem locator, one-value `0..80` to `0..81` correction, unchanged Appendix A bytes, and unchanged RULESET_DIGEST are coherent.

No source, test, fixture, design, plan, branch, PR, merge, publication, deployment, or release change was made in this review.

## Verdict

DESIGN_REVIEW_VERDICT: must-revise

The value correction is sound at the schema/state boundary, but the current erratum does not yet provide a false-green-resistant acceptance contract and overstates its reachability proof. The following three revisions are required; none changes the proposed `0..81` value or needs a new operator decision.

## MR-1 — require a positive ply-81 acceptance gate

The fold obligations update the checker cap to 81 and mutate MUT-H14 from 81 to 82, but the battery only pins MUT-H14's rejection code. A checker that incorrectly retains `ply > 80` rejection will still reject the new 82-valued mutant with `E_ROW_PLY_RANGE`; the described gates therefore pass while the erratum is not implemented.

Add an explicit acceptance criterion and fold obligation that a valid one-record, ply-81 terminal-draw certificate passes in subgame certificate mode. An emitter-side helper is sufficient; no committed golden vector is required. The construction is concrete: use five full local draw patterns carrying 5 X / 4 O marks and four color-swapped full draw patterns, giving 41 X / 40 O marks, `forced=ANY`, no local or macro win, and a NOLOSS terminal. Canonicalization preserves well-formedness and terminal draw. A read-only executable probe built this exact 237-byte certificate in memory with manifest row `ply=81`.

Keep the 82-valued MUT-H14 exact-code check as the negative side. Together, `81 accepts` and `82 rejects E_ROW_PLY_RANGE` lock both sides of the amended boundary.

## MR-2 — prove routed reachability or narrow the rationale

Lines 20-22 claim that an 81-mark all-closed draw is reachable and therefore certain initial-root draw proofs were inexpressible. Appendix A rule 4 imposes forced-board routing; the fact that nine isolated local boards can each fill without a win does not itself establish that one legal routed 81-move history fills all nine. The current sentence assumes the game whose existence it is meant to prove.

Choose one exact repair:

1. Supply a legal 81-move witness from the canonical `forced=4` initial state, checked against rules 2-6, and retain the deliverable-proof claim; or
2. Narrow the rationale to the schema's declared state domain: DD §2.6 deliberately does not require full root reachability for every stored state, §3.3 permits arbitrary well-formed subgame roots, and a canonical 81-mark all-drawn state is a valid terminal NOLOSS root. On this fork, remove the stronger assertion about reachable initial-game draw proofs unless separately evidenced.

The second fork is sufficient to justify the wire cap and is the smaller revision.

## MR-3 — repair the §6 locator and state the version disposition

Line 16 says the base DD repeats a literal §6 refusal `manifest row ply > 80`. It does not: §6 step 1 refers generically to §4.3 manifest-row violations; the only literal repeated bound is §9's MUT-H14 row. Revise the alignment sentence either to say §6 inherits the amended §4.3 range by reference, or explicitly define newly added §6 wording rather than describing nonexistent text as a replacement.

Also state that SCHEMA_VERSION remains `0.1` because this is a pre-checker-merge, pre-emitter erratum correcting the schema of record, not an additive post-release compatibility promise. This makes the versioning decision explicit for the solver/checker boundary without changing the orchestrator-authorized mechanics.

## Acceptance criteria for the successor

1. Exact normative range is `0..81`; `E_ROW_PLY_RANGE` means `ply > 81` and retains its first-failure position.
2. A canonical, well-formed ply-81 all-drawn NOLOSS terminal certificate passes; an otherwise well-framed MUT-H14 with row ply 82 rejects `E_ROW_PLY_RANGE`.
3. MUT-H14.bin regenerates deterministically and its catalogue wording says `ply > 81`; all other committed vectors/mutants and their digests remain byte-identical.
4. Appendix A extraction remains byte-identical and RULESET_DIGEST remains `16e85b36332493470fa1006516128579`.
5. The rationale no longer claims routed reachability without a checked witness; the §6 alignment names the actual base text; SCHEMA_VERSION `0.1` is explicitly disposed.
6. Solver code remains untouched; the later emitter consumes the approved cap.

## Boundary contract

Writes: an erratum overlay for the existing certificate manifest-row `ply` range; after later dispatch, checker/corpus/test constants and one positive boundary probe.

Reads: immutable base DD rev3, Appendix A rules 2-6, current checker parser, MUT-H14 generator/catalogue, and the Python checker battery.

Target entity: `DD-theory-c2-20260828` as amended by erratum stem `DD-theory-c2-20260828-erratum-1`.

Downstream consumers: theory checker and the later solver emitter.

Contract: accept representable record plies 0 through 81 inclusive; reject values above 81 before row ordering; retain schema/ruleset identities and all unrelated bytes.

Proof: approving successor DESIGN-REVIEW plus the later fold's positive-81 and negative-82 E2 gates and deterministic artifact census.

No-consumer action: not applicable — both checker and emitter are commissioned; solver implementation remains out of this fold.

OPERATOR_ITEMS: none — all findings preserve the orchestrator-authorized value and scope; no new product-semantic choice or waiver is requested

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static schema review
- migration/backfill/destructive-write/canonical-data-repair: no — no data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifact only
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — shared theory-checker/solver-emitter certificate contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — requested positive and negative gates exercise the actual checker boundary
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — revisions narrow proof claims and retain the authorized one-value scope
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or used

Verification:
- E1: erratum bytes, commit identity, one-file commit scope, base DD §§2.6/3.3/4.1/4.3/6/9, Appendix A, checker range guard, MUT-H14 generator/catalogue, and battery expectations inspected directly.
- E1: incoming DESIGN relay has zero exact-file lint errors; active-root lint remains red on artifact-resolution probes, including this new erratum locator, despite the manual Git-object and SHA-256 match.
- E2 incidental: an in-memory emitter probe constructed a canonical, well-formed 41/40 all-drawn NOLOSS terminal certificate with one manifest row at ply 81 and 237 total bytes. This supports MR-1's cheap positive gate but is not used to claim a routed initial-game history.

The Planner should amend the same erratum forward, float `DESIGN_SHA256`, and reissue the DESIGN request with a new handoff id. No plan, fold-2, implementation, or merge action is authorized by this must-revise review.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/merge/publication/deployment/release change claimed; reviewed main@eb07107ef5929759681bcef9621c62f9d3757d5a and authored only this DESIGN-REVIEW draft for daemon filing; final status below attributes existing scheduler, daemon, and relay bytes
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260830-041021.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260830-035925.md
