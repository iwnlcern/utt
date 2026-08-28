ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c2-plan-review-2
PARENT_DISPATCH_ID: theory-c2-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — one bounded artifact-shape and streaming correction; implementation remains held
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c2-plan-2/PLAN-planner-20260828-001033.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
PLAN_LOCK_ID: PL-theory-c2-20260827
PLAN_ARTIFACT: PL-theory-c2-20260827
PLAN_SHA256: db3deb73e937fac749af1d14bccff6d50a1f3f3b811f918519fc5c1c089c4520
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: /Users/jack/Programming/utt
BASE: main@dce510e
TARGET_BRANCH: main
BRANCH: theory/c2 — not created; implementation remains held
SUBJECT: PLAN-REVIEW must-revise — point-level JSON exceeds the repository transport boundary

## Verdict

Must revise `PL-theory-c2-20260827` revision 2 at SHA-256 `db3deb73e937fac749af1d14bccff6d50a1f3f3b811f918519fc5c1c089c4520` before implementation dispatch.

Revision 2 closes prior M1 and the semantic/schema portion of M2: the 20,332-key spot population is now mechanically bound, selected owners and masks are explicit, no-sampling and conservation rules are testable, and the JSON fields and deterministic ordering are pinned. The selected M2 realization — every disagreement coordinate in one committed JSON array, fed by `spot_sweep(...) -> list[KnifePoint]` — is not executable through the locked branch/PR path at the measured population. One narrow representation correction remains.

This verdict is review-only. It grants no implementation, source/test edit, branch creation or movement, regeneration, commit, push, PR, merge, publication, deployment, release, Git-LFS adoption, or external-use authority. No live implementation token is present.

## Required revision

### M3 — use a complete aggregate partition and preserve board-streaming memory discipline

The revision-2 point-coordinate array cannot be the committed artifact:

- A fresh exact exhaustive census over N=1..32 found 5,869,108 equality points and 1,194,488 disagreements.
- A compact JSON encoding of one shortest legal coordinate row with the required keys is 138 bytes. Therefore the exhaustive disagreement rows alone have a 157.2 MiB lower bound before array separators, top-level methodology, per-scale rows, groups, whitespace, or either targeted scale.
- GitHub enforces a 100 MiB single-object limit for ordinary Git. This repository has no Git-LFS tracking, and adding LFS/configuration or external artifact storage is outside the current theory-only plan.
- The N=64 target mask already contains 722,676 equality points across the locked 20,332 target keys. Independently of final disagreement rate, `spot_sweep(...) -> list[KnifePoint]` requires retaining that entire population and defeats the landed `targeted_report` discipline that releases each board before solving the next.

Use the alternative already allowed by plan-review-1 M2: define the “full disagreement table” as the complete deterministic nonzero aggregate partition, not a coordinate dump. The existing `groups` dimensions — `scale`, `mode`, `h`, `state_class` including E(s), `discrete`, and `agrees` — are sufficient. Emit all group rows, and define `disagreement_groups` as every `agrees == false` row with no truncation; preserve per-scale `equality_points == agreements + disagreements`, group-sum conservation, and disagreement-group-sum conservation. An optional deterministic digest of the streamed coordinate tuples may be added for reproducibility, but sample/witness rows must not substitute for the complete aggregate table.

Replace the canonical spot path's list materialization with a generator, callback, or accumulator fold that solves one distinct board, updates per-scale aggregates/digest, and releases its value/mask arrays before the next board. The public small-scale `sweep(N)` helper may remain list-returning for focused tests, but `report` must not concatenate all exhaustive/spot `KnifePoint` objects. Add a weak-reference or equivalent lifecycle test modeled on the existing `test_targeted_report_releases_each_board_before_solving_the_next`, plus artifact tests proving every nonzero disagreement partition row is emitted and its counts conserve exactly. Update Task 3 and the completion report to reproduce the full aggregate disagreement table, not every coordinate.

Do not solve this by silently compressing the same monolith, splitting undeclared files, adding Git LFS, or moving the artifact outside Git; each would alter the locked artifact/scope/consumer contract and would require its own explicit plan change. — E2 — plan lines 80–107, 134–175, 186–202; exact census below; existing board-release seam in `theory/tests/test_crosscheck.py`.

## Finding disposition

- Prior M1: closed. The selector, canonical cardinality, selected-owner-only rule, distinct-board solve count, mask hard-fails, and seam-test obligations are explicit.
- Prior M2 semantics/schema: closed. Methodology, per-scale fields, deterministic groups, rational encoding, separate draw/opponent buckets, and conservation tests are explicit.
- Prior M2 realization: must revise as M3. Point-level materialization conflicts with the measured artifact and memory boundaries; the complete aggregate-partition alternative preserves the P1c question without sampling.
- MR17, exact predicate, P1a/P1b interpretation, state classes, scope, hard stops, and separate merge gate: pass unchanged.
- Scope/collision: pass at review time. Main remains at `20cb330604fa`; theory source/test/fixture/result bytes are unchanged from dispatched base `dce510e`; no `theory/c2` branch or built knife-edge artifact exists.
- Operator judgment: none. This is a representation/transport correction; the ratified convention and “any nonempty disagreement table” routing rule remain unchanged.

## Successor gate

The Planner should revise the same plan lock and return a fresh PLAN carrier parented to this review with the new digest. A later delegated implementation dispatch remains conditional on a fresh approving PLAN-REVIEW, a parent edge to that approval, an exhaustive all-in SCOPE_DIFF, no hard trigger, no boundary-contract deviation, no cross-bundle collision, and daemon admission. Until then implementation remains unauthorized.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or generated-data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets only
- AI-or-automation-acts-downstream: no — this bundle emits local analysis and fixtures, not autonomous external actions
- worker/scheduler/queue/retry/async-side-effect: no — local deterministic batch computation only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — MR17 changes a theory-owned fixture consumer contract used by engine
- user-visible-control-with-materializer/downstream-consumer: no — no user-facing control in this bundle
- test-runtime-role-mismatch: yes — the planned list/monolith path discards the existing board-release discipline required by the canonical targeted runtime
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — M3 selects the previously permitted aggregate representation inside the existing artifact and scope
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming revision-2 PLAN relay exact-file lint with freshness disabled: clean. Current design SHA-256 is `232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689`; current plan SHA-256 is `db3deb73e937fac749af1d14bccff6d50a1f3f3b811f918519fc5c1c089c4520`. — E2 instrument evidence.
- Exact read-only oracle census, N=1..32: 5,869,108 masked positive-total equality points and 1,194,488 disagreements. Representative checkpoints: N16 = 179,838/37,206; N24 = 273,052/56,454; N32 = 364,462/75,014. — E2 local oracle evidence.
- Exact read-only target-mask census: the landed selector returns 20,332 keys across 10,274 distinct boards; N64 contains 722,676 equality points. The longer N128 mask-only diagnostic was intentionally interrupted after the exhaustive lower bound had already proved the transport defect; no result is claimed for N128. — E2 local mask evidence.
- Minimum-size proof: the shortest compact required coordinate object is 138 bytes; `138 * 1,194,488 = 164,839,344` bytes, or 157.2 MiB as measured by the local serializer, before framing and other artifact content. GitHub's ordinary-Git single-object limit is 100 MiB; no `.gitattributes` LFS rule is present. — E2 local serialization/repository evidence plus current GitHub documentation.
- Relay daemon is kit 2.9.2, fingerprint `ae5567d7f00eb92c3ea8f86ab41cfefd4b770d4ed7e67fb0dc100b1e88260143`, ready with zero pending renders. Its one inherited INDEX divergence is root noise and does not alter this exact-file verdict.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/theory.implementer/PLAN-REVIEW-theory-c2-plan-review-2-20260828-002440.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, regeneration, commit, push, PR, merge, publication, deployment, release, or LFS action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-boot/SITREP-orchestrator-reviewer-20260828-002232.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-seat/
— all listed bytes are concurrent/foreign Planner or s2 governance work and were preserved; daemon-rendered review/INDEX transport occurs only after submission
