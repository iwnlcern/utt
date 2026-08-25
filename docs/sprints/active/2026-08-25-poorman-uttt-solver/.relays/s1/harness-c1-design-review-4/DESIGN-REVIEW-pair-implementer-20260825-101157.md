ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-design-review-4
PARENT_DISPATCH_ID: harness-c1-design-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — technical amendment is approved; downstream UI alignment is orchestrator-routed and requires no new product ruling
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-design-4/DESIGN-planner-20260825-100451.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
DESIGN_REVIEW_VERDICT: approve
BUNDLE_ID: harness-c1
OWNER: harness
SUBJECT: DESIGN-REVIEW approve — amendment 1 reconciles recovery ordering and terminal shapes

## Verdict

Approve amendment 1 to `DD-harness-c1-20260825` at digest `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`.

The amendment closes the IMPL hold raised in `harness-c1/SITREP-pair-implementer-20260825-095726.md`. It correctly makes the DD agree with R2 and the reviewed plan: a single fault first resolves the auction, applies the legal opponent's move, records any earned terminal, and emits the resolved auction; recovery runs only if another request can occur. Double-fault recovery remains mid-ply before retry. The two recovery-fault terminal shapes and the explicit recovery-association keys make the log model total and replayable in either emission position.

This verdict approves the amended design bytes only. It does not authorize implementation under the stale `harness-c1-impl-1` dispatch, and it grants no merge, publication, deployment, or release authority.

## Amendment disposition

- Original HOLD: closed. DD §5.1 now prevents a restart failure from rewriting a fault-won terminal result and retains uniform recovery before a faulter's next request. — E1 — amended DD lines 93–101; R2 lines 85–91.
- Causal ordering: closed. DD §8 now distinguishes double-fault recoveries before their auction from single-fault recoveries after their resolved auction, preserving append order and the plan's exact Task 7/9 semantics. — E1 — amended DD lines 124–144; plan lines 269–286 and 366–368.
- Recovery association: closed. `ply` and `trigger_request_id` are normative recovery-event fields, so log readers associate both placements without guessing or discarding raw event order. — E1 — amended DD line 135; plan lines 241 and 269–286.
- Terminal totality: closed. A double-fault restart failure yields an `aborted_recovery_fault` auction, while a single-fault restart failure follows an already resolved auction; both end with `recovery_fault`, and the both-seat failure remains a void. — E1 — amended DD lines 138–141.
- Acceptance coverage: closed. §10 now requires fixtures for the double-fault abort, single-fault post-resolution restart failure, and both-seat failure, in addition to the per-fault matrix and parity fixtures. — E1 — amended DD lines 159–165.
- Amendment scope: pass. The exact diff is limited to 9 insertions and 3 replacements in §5.1, §8, §10, and the §14 provenance ledger; `git diff --check` is clean. No R1–R5 ruling, analysis carrier, bridge deferral, seed rule, clock, protocol message, process boundary, or out-of-scope surface changed. — E1/E2.

## Design-grill, target, boundary, and acceptance disposition

- `DESIGN_DOC_ID`, `GRILL_LOCK_ID`, all operator decisions/defaults, rejected alternatives, risks, and open-question state are preserved. Amendment 1 introduces no operator-owned semantic choice. — E1 — unchanged DD §§1–7 and 12–14.
- Target entity remains protocol/log/referee behavior. Producers are the harness referee and schemas; consumers remain engine clients, friend engines, UI replay/analysis, and tournament regression. — E1 — DD §§8–11.
- Reads/writes and out-of-scope boundaries remain exact: write `referee/**` and `docs/protocol/**`, read theory fixtures through the loader seam, and do not build engine changes, theory fixtures, browser/network transport, SPRT, ratings, or a generic platform. — E1 — DD §§9–11.
- The locked plan remains byte-identical at digest `79ea02702b4dc9d2755981e55c2b9ba099f0c47df7578ac1face570f85eae600` and now agrees with the amended DD. It still requires fresh gated PLAN and PLAN-REVIEW lineage before a successor implementation dispatch. — E1.

## Downstream boundary obligation

The current UI design remains stale against this amendment: `DD-ui-c1-20260825` lines 34–35 attach only recoveries preceding an auction and state that the harness appends every recovery before its auction. Amendment 1 makes single-fault recovery follow the resolved auction and supplies `ply`/`trigger_request_id` for backward association. The harness design is authoritative for its log schema, so this is not a reason to reject the harness amendment; it is a required orchestrator-routed UI successor before UI PLAN. No product ruling is needed.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local referee/log design
- migration/backfill/destructive-write/canonical-data-repair: no — pre-implementation design amendment
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated integer chip budgets only
- AI-or-automation-acts-downstream: no — local game engines take no external action
- worker/scheduler/queue/retry/async-side-effect: yes — amendment governs subprocess recovery and retry event order
- cross-repo/service-contract/generated-schema/shared-API-event: yes — log schema and event order are consumed by UI and engine-facing tooling
- user-visible-control-with-materializer/downstream-consumer: yes — UI replay is a named downstream consumer
- test-runtime-role-mismatch: no — review-only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — no new semantic branch or accepted residual in harness; UI alignment is explicitly routed
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and next action

- Incoming design digest matches the amended 233-line artifact; the unchanged plan digest matches its carrier. — E1.
- Incoming relay exact-file lint passes under kit 2.9.2 with freshness disabled for historical re-read; daemon status is ready with zero pending renders and only the adjudicated INDEX divergence. `git diff --check` is clean. — E2 instrument evidence for carrier/diff shape; design claims remain E1.
- No source, test, design, plan, dependency, implementation-worktree, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

Next: `harness.planner` may issue `harness-c1-plan-5`, locking the unchanged plan bytes to amended design digest `c935c29c…`, request a fresh Implementer PLAN-REVIEW, and only after approval issue a successor implementation dispatch. Separately, `s1.orchestrator-planner` should route the named UI design alignment before UI PLAN.

ACTIONS_GIT_REF:
- review=draft authored under `.engine/drafts/harness.implementer/` for daemon admission
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT: none — clean implementation worktree at `/Users/jack/Programming/utt/.worktrees/harness-c1-impl`

(The primary checkout contains pre-existing and concurrent foreign design, plan, INDEX, and relay changes. This seat authored only the daemon draft and its admitted DESIGN-REVIEW relay.)
