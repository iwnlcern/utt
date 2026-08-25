ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-design-review-3
PARENT_DISPATCH_ID: ui-c1-design-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — both residuals are narrow internal design-consistency corrections; the existing M4 owner-contract gate remains orchestrator-routed
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-design-3/DESIGN-planner-20260825-092742.md
DESIGN_DOC_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: 6fc04e6e1a2ca0d2a30d9361a7440c6d839134d091a9b0269cbf40945bfd52c2
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: ui-c1
OWNER: ui
SUBJECT: DESIGN-REVIEW must-revise — trailing-recovery model and test inventories remain stale

## Verdict

Revision 3 closes the substantive trailing-recovery representation and provenance findings from `ui-c1-design-review-2`, preserves the complete `GRILL_LOCK`, and matches the harness recovery-event contract. Two downstream inventories now contradict the new §2.1 lock, however: the module decomposition omits `ReplayModel.trailingRecoveries`, and the testing section still says there are two §2.1 cases while naming only those two. Return one narrow successor that makes those inventories carry the third locked case.

This verdict reviews design bytes only. It does not authorize PLAN, implementation, scaffolding, dependency installation, source/test edits, branch creation, commit, PR, merge, publication, or deployment.

## Prior-finding disposition

- Trailing-recovery representation: closed in substance. §2.1 defines `ReplayModel.trailingRecoveries` as complete recovery events in logged order, with seat, triggering fault class, and restart `hello_record`; it renders them at the final cursor with the last logged board and synthesizes no auction state. This matches DD-harness-c1-20260825 §§5.1 and 8. — E1 — UI design lines 35 and 38; harness design lines 93–100 and 125–138.
- Trailing-recovery falsifiable case: closed at the lock declaration. §2.1 case (c) covers both a single-recovery tail and X-then-O order. — E1 — UI design line 38.
- Provenance correction: closed. `position_0` now separates initial budgets read from `game_start` from `poorman-uttt-v1` constants, and later tie owner is explicitly a projection from logged `resolution.winner`. — E1 — UI design line 32.
- M1–M5, R5 scope, all six operator grill decisions, rejected alternatives, no-code-reuse boundary, dependency policy, and M4 pre-PLAN gate: preserved. — E1 — UI design §§1–12 and exact revision diff.

## Must-revise residuals — make downstream inventories agree with §2.1

1. In §3 `src/replay/`, add `trailingRecoveries` to the enumerated `ReplayModel` shape. The sentence currently claims to list the §2.1 model but names only `SetupStep`, `AuctionStep[]`, `TerminalStep`, and `position_0..R`; a PLAN or Implementer following that inventory can omit the newly locked state. — E1 — UI design line 43 versus line 35.
2. In §8, change “the two §2.1 falsifiable cases” to three and explicitly include derivation/render coverage for the valid complete-event prefix ending after one recovery and before its auction, including X-then-O logged-order preservation. The current test inventory names only zero-auction setup-terminal and pre-auction ghost association. — E1 — UI design line 90 versus line 38.

No new UX, schema, operator decision, or architecture is requested. Preserve the exact §2.1 trailing-recovery semantics, the provenance correction, every previously closed finding, and `GRILL_LOCK_ID: GRILL-ui-c1-20260825` unchanged.

## Design-grill, boundary, and acceptance disposition

- `GRILL_REQUIRED: yes` is satisfied by the embedded `GRILL_LOCK`; all six operator decisions and the code-derived/default decisions remain present. Do not re-run the grill. — E1 — UI design lines 106–141.
- Target entity, human downstream consumer, boundary contract, R5 out-of-scope line, rejected alternatives, dependency policy, and open-question ownership: pass. — E1 — UI design §§1, 3, 9–11.
- The M4 PV-perspective/state-key owner ruling remains a hard pre-PLAN gate. Approval of a later digest will not satisfy or waive it; route the addressed design-completion ask to `s1.orchestrator-planner`. — E1 — UI design lines 74–75, 95–104, and 139–140.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay design only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes the harness recovery-event and engine-analysis contracts
- user-visible-control-with-materializer/downstream-consumer: yes — replay presentation is the target, with a human consumer
- test-runtime-role-mismatch: no — design review only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approving contradictory model/test inventories would leave residual omission risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Declared design digest matches the current 161-line artifact: `6fc04e6e1a2ca0d2a30d9361a7440c6d839134d091a9b0269cbf40945bfd52c2`. — E1.
- Incoming relay exact-file lint passes under kit 2.9.2 with freshness disabled for historical re-read; `git diff --check` is clean. — E2 instrument evidence for relay/diff shape; design claims remain E1.
- Harness design digest matches the incoming carrier's pin: `11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440`; its recovery event contains `seat`, triggering `fault`, and restart `hello_record`, and its normative order is recovery-before-auction with X before O. — E1.
- No UI source, test, plan, dependency, branch, commit, PR, merge, publication, or deployment action was performed.

Return a successor DESIGN request with a fresh dispatch id parented to `ui-c1-design-3`, the same `DESIGN_DOC_ID`, a fresh `DESIGN_SHA256`, and `IN_REPLY_TO` naming this review. Fold only the two inventory corrections above.

ACTIONS_GIT_REF:
- review=draft authored under `.engine/drafts/ui.implementer/` for daemon admission
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/SEATS.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-planner-20260825-092831.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/s1-daemon/RECONCILE-orchestrator-reviewer-20260825-092910.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/s1-daemon/SITREP-pair-implementer-20260825-092838.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-3/

(INDEX/SEATS and all listed foreign files are pre-existing or concurrent work; this seat authored only the daemon draft and its admitted review relay.)
