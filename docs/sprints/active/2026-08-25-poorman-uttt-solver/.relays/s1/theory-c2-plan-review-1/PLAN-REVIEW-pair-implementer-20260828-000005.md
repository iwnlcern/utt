ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c2-plan-review-1
PARENT_DISPATCH_ID: theory-c2-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — two bounded execution-contract corrections only; implementation remains held
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c2-plan-1/PLAN-planner-20260827-131151.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
PLAN_LOCK_ID: PL-theory-c2-20260827
PLAN_ARTIFACT: PL-theory-c2-20260827
PLAN_SHA256: ed9125a729e0e14b8e3df7598c6a196119728e4233908ae7b4edcb4802010057
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: /Users/jack/Programming/utt
BASE: main@dce510e
TARGET_BRANCH: main
BRANCH: theory/c2 — not created; implementation remains held
SUBJECT: PLAN-REVIEW must-revise — pin targeted P1c population and auditable JSON contract

## Verdict

Must revise `PL-theory-c2-20260827` at SHA-256 `ed9125a729e0e14b8e3df7598c6a196119728e4233908ae7b4edcb4802010057` before implementation dispatch.

The MR17 generator route, six-fixture tag delta, exact equality predicate, P1b agreement semantics, canonical reachable state classes, theory-only scope, hard-stop posture, and separate merge gate are sound. Two P1c execution-contract gaps remain: the N=64/128 target population is not mechanically bound to the landed selector, and neither the report schema nor its tests prove that the committed JSON contains a deterministic, complete, untruncated disagreement table.

This verdict is review-only. It grants no implementation, source/test edit, branch creation or movement, regeneration, commit, push, PR, merge, publication, deployment, release, or external-use authority. No live implementation token is present.

## Required revisions

### M1 — bind the N=64/128 target population and prove no-sampling coverage

Replace the generic phrase “targeted spot populations” with the executable population contract already landed in `crosscheck.py` and recorded in the Stage-1 evidence:

- compute the canonical target keys once from `solve_continuous()`, the N=32 exhaustive masks, and `select_spot_targets(solved_cont, masks_32, 32)`;
- preserve the selector's sorted `(board, h)` keys and the landed canonical cardinality of 20,332 targets;
- at each of N=64 and N=128, call `solve_targeted(board, N)` once per distinct board, consume only the selected owner keys, and enumerate every mask-true, nonterminal, positive-total point satisfying `bx*q == p_T*(bx+bo)`; and
- hard-fail an empty/missing selected mask or any population/count conservation failure rather than dropping a target or sampling.

Add a focused test seam, using small fakes/mocks rather than the full expensive run, that proves the same selected keys feed both spot scales, distinct boards are solved once per scale, every qualifying mask-true equality coordinate is retained, off-mask/non-equality/M=0 coordinates are excluded, and no selected owner is silently widened to the unselected owner. Keep the canonical run's 20,332-target count as artifact/acceptance evidence. Without this binding, implementations with different target sets can satisfy the current three unit tests and produce incomparable N=64/128 results. — E1 — plan lines 77–82 and 88–112; `crosscheck.py:200–278`; `EVIDENCE.md:31–38,92–97`; DD D3 at lines 97 and 206.

### M2 — define and test the report/JSON completeness contract

Replace `report(...) -> dict` plus “full disagreement table” with a concrete deterministic JSON contract. At minimum pin:

- top-level methodology/version metadata and the requested exhaustive and spot scales;
- one per-scale record with explicit `mode`, target count for targeted scales, total equality population, agreement count, disagreement count, and count-conservation invariants;
- deterministic grouped rows keyed by scale/mode, `h`, `state_class` (including `E(s)`), `discrete`, and `agrees`, with canonical `p/q` strings wherever a rational is emitted; and
- an explicit definition of the “full disagreement table”: either every disagreement coordinate with `board,h,bx,bo,T,state_class,discrete,convention`, in deterministic order, or a complete nonzero aggregate partition whose dimensions and conservation rule are fixed. State explicitly that the CLI applies no sampling, limit, truncation, or representative-row reduction.

Add RED tests for `report` and the combined `knife-edge --max-scale ... --spots ... --out ...` path. The tests must pin JSON serializability and deterministic byte output, the per-scale modes/scales, separate draw versus opponent-win disagreement buckets, equality/group/disagreement count conservation, and complete disagreement inclusion. The current tests exercise only `sweep(N)` and the `agrees` field; they can all pass if the spot path, aggregation, serializer, or CLI output is incomplete. — E1 — plan lines 68–85, 88–112, and 123–147; P1c in `FINDINGS.md:315–332`.

## Finding disposition

- Locked design and lineage: pass. The design and plan locators resolve, both current byte digests match the incoming relay, and the incoming relay parents to the approved design-review carrier.
- MR17 route and fixture delta: pass. The generator owns `backup_cases.json`; it currently emits exactly six fixtures, all with `consumed_by: ["theory"]`, so adding `engine` through `fixtures_gen.py` yields the required six added consumer lines with no hand edit.
- Exact knife predicate and convention semantics: pass. Integer equality and positive-total masking are pinned; `draw` and opponent wins are correctly distinct disagreements against P1b.
- State classes: pass. The locked C11 census proves no canonical reachable `a>b` states; `a<b` and `a=b`, joined with E(s), are complete for this masked canonical sweep. Synthetic `a>b` backup cases remain outside P1c.
- Scope and collision: pass at review time. Main has advanced to `20cb330604fa`, but `theory/auction_ttt`, `theory/tests`, `theory/fixtures`, and `theory/results` are byte-unchanged from dispatched base `dce510e`; no duplicate knife-edge module/artifact or `theory/c2` branch is present.
- Tests and auditable acceptance: must revise per M1–M2.
- Operator judgment: none. The ratified convention and conservative “any nonempty disagreement table” routing rule remain unchanged.

## Successor gate

The Planner should revise the same plan lock and return a fresh PLAN carrier, parented to this review, with the new digest. A later delegated implementation dispatch remains conditional on a fresh approving PLAN-REVIEW, a parent edge to that approval, an exhaustive all-in SCOPE_DIFF, no hard trigger, no boundary-contract deviation, no cross-bundle collision, and daemon admission. Until then implementation remains unauthorized.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or generated-data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets only
- AI-or-automation-acts-downstream: no — this bundle emits local analysis and fixtures, not autonomous external actions
- worker/scheduler/queue/retry/async-side-effect: no — local deterministic batch computation only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — MR17 changes a theory-owned fixture consumer contract used by engine
- user-visible-control-with-materializer/downstream-consumer: no — no user-facing control in this bundle
- test-runtime-role-mismatch: yes — current planned tests omit the targeted-report, serializer, and combined CLI path that produces the acceptance artifact
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — M1–M2 are bounded mechanical plan corrections under the ratified P1c obligation
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming PLAN relay exact-file lint with freshness disabled: clean. Current design SHA-256 is `232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689`; current plan SHA-256 is `ed9125a729e0e14b8e3df7598c6a196119728e4233908ae7b4edcb4802010057`. — E2 instrument evidence.
- Duplicate/already-built gate: no `auction_ttt.knife_edge`, `knife-edge-sweep.json`, `theory/c2` branch, or earlier `theory-c2-plan-review-1` dispatch exists. — E2 Git/filesystem evidence.
- A read-only N=4 oracle probe found 38,764 exact masked positive-total equality points and 8,076 disagreements across exactly the locked `a<b`/`a=b` classes. This corroborates that disagreement completeness and deterministic aggregation are material acceptance properties, not decorative output. — E2 local oracle evidence.
- Relay daemon is kit 2.9.2, fingerprint `ae5567d7f00eb92c3ea8f86ab41cfefd4b770d4ed7e67fb0dc100b1e88260143`, ready with zero pending renders. Its one inherited INDEX divergence is root noise and does not alter this exact-file verdict.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/theory.implementer/PLAN-REVIEW-theory-c2-plan-review-1-20260827-235819.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, regeneration, commit, push, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT: none — clean main worktree at `20cb330604fa` before daemon admission; daemon-rendered review relay and INDEX transport occur only after submission
