ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-plan-review-1
PARENT_DISPATCH_ID: ui-c1-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the upstream design amendment and plan corrections are owner-routed; no new product-semantic decision is required
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-plan-1/PLAN-planner-20260825-144824.md
DESIGN_LOCK_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: dcea3f9e83a73f5d1515c8604f25f7a6298fc0d54a66600dd0cacf517b4ecb6a
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
PLAN_SHA256: fddba9fe5907c8f0f893e750105ddb8d57cb76a15ea2f3f459e4000354a8b532
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: ui-c1
OWNER: ui
BASE: main@f8e26ae
TARGET_BRANCH: main
BRANCH: ui-c1-impl
SUBJECT: PLAN-REVIEW must-revise — approved harness amendment invalidates recovery model; three execution defects remain

## Verdict

Must revise `PL-ui-c1-20260825` at digest `fddba9fe5907c8f0f893e750105ddb8d57cb76a15ea2f3f459e4000354a8b532`. The relay's design-review parent, artifact digests, M4 ruling, broad scope, and dispatch-time rebase fence are structurally sound. Approval is blocked because the locked UI design predates an approved harness contract amendment that explicitly requires a UI design successor before PLAN, and the plan also contains three independent implementation defects: zero-based `ply` is decremented, cumulative closure state has no log-derived carrier, and the sample macro-win fixture is impossible in five plies.

This verdict is review-only. It authorizes no implementation, scaffolding, dependency installation, source/test edits, branch creation, commit, PR, merge, publication, deployment, or release. The delegated-dispatch conditions in `ui-c1-proceed-1` are not met.

## MR1 — required UI design successor before any reissued PLAN

Harness amendment 1 is approved at `DD-harness-c1-20260825` digest `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`. It makes `recovery.ply` and `recovery.trigger_request_id` normative, places double-fault recovery before its auction, places single-fault recovery after its already-resolved auction, and defines both `recovery_fault` terminal shapes. — E1 — harness design lines 128–143 and amendment ledger lines 233–234.

The approving harness review explicitly records that UI rev 4 is stale and requires an orchestrator-routed UI successor before UI PLAN. The current UI lock still says recoveries attach only when they causally precede an auction and that the harness appends every recovery before its auction. — E1 — `harness-c1-design-review-4/DESIGN-REVIEW-pair-implementer-20260825-101157.md`, Downstream boundary obligation; UI design lines 34–35.

Required sequence:

1. Route a successor DESIGN for the same `DD-ui-c1-20260825` identity. Fold the amended recovery fields, both emission positions, association by `ply`/`trigger_request_id`, the two recovery-fault shapes, and the rule that only recovery events with no matching auction remain `trailingRecoveries`. Preserve the GRILL lock and all unrelated approved semantics.
2. Obtain an approving DESIGN-REVIEW for that fresh design digest.
3. Reissue the PLAN with a fresh PLAN digest parented to that new approving design review. A plan-only wording fold cannot repair stale design lineage.

## MR2 — recovery derivation and fixtures follow event position instead of owner keys

The plan says `recovery-mid-game` attaches to its following `AuctionStep` and covers only the pre-auction abort shape. That misattributes a normal single-fault recovery—which follows its resolved auction—to the next ply. — E1 — plan lines 103–110 and 147–152 versus harness design lines 135 and 138–141.

Required plan fold after MR1:

- Make `RecoveryEvent` require `ply` and `trigger_request_id`.
- Associate recovery events to `AuctionStep.ply` by those keys regardless of emission position; preserve raw event order.
- Add distinct fixtures/tests for double-fault recovery before the auction, single-fault recovery after the resolved auction but attached backward to that same step, double-fault `aborted_recovery_fault`, and single-fault resolved-auction-then-`recovery_fault`.
- Keep only a recovery whose referenced auction is absent in the valid prefix under `trailingRecoveries`; test that a post-auction recovery at EOF is not misclassified as trailing.
- Carry these cases through Task 15 end-to-end acceptance and the timeline markers.

## MR3 — timeline click subtracts one from a zero-based logged `ply`

The harness begins at `ply: 0`. Task 10 specifies `onSelect(ply-1)`, sending the first auction row to `-1`; its test switches terminology to `j-1` without resolving whether `j` is a one-based row ordinal or the wire value. — E1 — harness design lines 38, 51, and 58; plan lines 176–178.

Required: keep the design's one-based `AuctionStep_j` ordinal distinct from the zero-based wire `ply`. Select cursor `j-1` by row index, equivalently raw `ply` only after validating sequential zero-based plies. Add first-row, middle-row, and unresolved-terminal row tests that prove the callback never underflows and selects the pending pre-auction view.

## MR4 — closure overlays lack a log-derived cumulative state

The approved design forbids UI-side rules execution and requires closures to be read from the log. The plan's `Position` has board/budgets/forced/tieOwner only, while `Board` promises winner/full overlays without naming a cumulative closure map or a GameView derivation from logged `resolution.closures`. Recomputing wins/fullness from board strings would violate the lock; passing only the current auction's closures loses earlier closed boards. — E1 — UI design lines 20–25 and 66–67; plan lines 65, 70–75, 138–145, and 163–169.

Required: add an explicit log-derived cumulative closure carrier per replay position (or an equivalent typed replay annotation), fold only logged `resolution.closures`, feed it to `Board`, and test that closures from earlier auctions remain overlaid at later cursors without board-rule recomputation.

## MR5 — the five-ply macro-win fixture cannot be a valid game log

Task 2 requests a `5-ply macro win` from the fixed empty 3×3-of-3×3 start. A local win requires three marks and a macro win requires three won local boards, so even one player winning every auction needs at least nine applied marks. A five-ply sample cannot serve as truthful schema-v1 success or acceptance evidence. — E1 — rules spec lines 19 and 35–38; plan lines 103–110.

Required: replace it with a legal referee-emittable macro-win transcript (at least nine applied marks), or adopt a generated harness conformance fixture when available. Keep the `tie_coin` and bound-analysis coverage in a legal fixture, and ensure the bundled sample is copied only from a valid transcript.

## Passing dispositions and held gates

- Plan/design identity and byte integrity: pass for the submitted artifacts. Current hashes match the relay carriers: design `dcea3f9e…`, plan `fddba9fe…`. The later harness amendment is a semantic boundary supersession, not an unexplained byte mismatch. — E1.
- M4: pass for PLAN sequencing. `s1-m4-ruling-1` explicitly permits UI PLAN to cite the ruling while PV rendering remains unavailable behind `PV_PIN` until the harness-owned pin lands. The anti-half-fix guard is correct and must be preserved. — E1 — ruling lines 15–26; plan lines 8 and 154–161.
- Scope and ownership: pass. `ui/**` plus sprint-tree reporting is bounded; protocol/schema and theory edits remain forbidden; schema mismatches stop and route upward. — E1 — plan lines 18–27 and 217–235.
- Target entity and downstream consumer: pass. Rendered replay/analysis views have a human consumer; no other pair consumes UI writes. — E1 — plan lines 229–235.
- Test/build target and E3 honesty: pass provisionally, subject to MR1–MR5. Vitest/build are E2; real referee log and adopted harness fixtures remain explicitly E3 done-or-blocked. — E1 — plan lines 233–243.
- Repository duplicate check: `ui/` is absent at review time; the scaffold remains genuinely still-open. — E1.
- BASE is stale relative to current `main`, but the proceed relay and plan require dispatch-time rebase plus fresh SCOPE_DIFF; preserve that fence. No branch action was taken. — E1.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay UI only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — UI has no worker or external side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — approved harness log-contract amendment changes UI recovery consumption
- user-visible-control-with-materializer/downstream-consumer: yes — replay model materializes the human-facing board/timeline
- test-runtime-role-mismatch: no — no runtime action in review
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approving stale recovery semantics or invalid fixtures would accept residual correctness risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Incoming PLAN relay exact-file lint passes under kit 2.9.2 with freshness disabled for historical re-read. — E2 instrument evidence; plan findings remain E1.
- `git diff --check` is clean for the plan artifact; current plan and design digests match their carriers. — E1/E2.
- Approved harness amendment and its approving review were read at their exact daemon-rendered paths; the amendment review exact-file lint passes. — E1/E2.
- No UI source, test, scaffold, dependency, branch, commit, PR, merge, publication, deployment, or release action was performed.

Return the DESIGN successor first, not a PLAN-only reissue. After its approving DESIGN-REVIEW, return `ui-c1-plan-2` (or the next unused PLAN handoff id) with a fresh digest, parented to that new review, folding MR2–MR5 and preserving all passing dispositions above.

ACTIONS_GIT_REF:
- review=draft authored under `.engine/drafts/ui.implementer/` for daemon admission
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-ui-c1-20260825.md

(INDEX and all listed relay paths are pre-existing or concurrent daemon/foreign work; the plan file is the Planner's reviewed artifact. This seat authored only the daemon draft and its admitted review relay.)
