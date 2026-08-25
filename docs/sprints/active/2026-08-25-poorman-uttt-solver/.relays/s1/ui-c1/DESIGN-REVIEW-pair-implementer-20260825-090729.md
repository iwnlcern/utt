ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-design-review-2
PARENT_DISPATCH_ID: ui-c1-design-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — narrow replay-totality correction is planner-owned; the existing M4 owner-contract gate remains orchestrator-routed
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/ui-c1/DESIGN-planner-20260825-090309.md
DESIGN_DOC_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: d155cad3c217297b3d62f95a9939dd9a60377349d5723e2c4ee5cecefb19195f
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — valid trailing recovery prefix is not representable

## Verdict

Revision 2 closes M1–M5 in substance and preserves the complete `GRILL_LOCK`, but the new typed replay model and the new partial-prefix policy compose into one residual totality hole. The locked harness emits each `recovery` event before the corresponding `auction` event; a crash between those appends yields a schema-valid complete-event prefix ending in one or more recoveries. Revision 2 accepts that prefix, yet its model can represent recovery only by attaching it to an `AuctionStep` that is absent. Return one narrow successor that represents and tests this trailing-recovery state.

This verdict reviews design bytes only. It does not authorize PLAN, implementation, scaffolding, dependency installation, source/test edits, branch creation, commit, PR, merge, publication, or deployment.

## Prior finding disposition

- M1 pre/post timebase and zero-auction setup: closed except for the trailing-recovery residual below. `ReplayModel`, cursor-between-states semantics, pre-auction association, resolved/unresolved outcomes, and the two falsifiable tests are explicit. — E1 — revision 2 §§2.1, 3, 5, and 8.
- M2 both-zero totality: closed. `p`, `p−T`, and percent-of-combined use a typed not-applicable state at zero combined budget while exact integer facts remain visible; the fixture case is named. — E1 — revision 2 §§3, 6, and 8.
- M3 value-quality metadata: closed. The complete approved engine shape (`quality`, `lo`, `hi`, `depth`, `complete`) is retained, bound intervals and provenance are rendered, malformed advisory fields degrade without invalidating the log, and the owner digest is pinned. — E1 — revision 2 §§3, 6, and 12; engine owner record §10 @ `265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47`.
- M4 state/perspective contract: closed for DESIGN. The speculative PV mapping is removed, fields stay unavailable until the producing owners pin perspective and state association, and PLAN is explicitly gated on an orchestrator-routed ruling. Approval of a later digest will not itself satisfy or waive that gate. — E1 — revision 2 §§6, 9, 10, and 12.
- M5 partial/fail-closed policy: closed in its parser distinction, with the model-totality residual below. Complete prefixes, a narrowly defined unterminated final tail, and malformed interior lines now have separate outcomes and named tests. — E1 — revision 2 §§4 and 8.

## Must-revise residual — preserve recovery events when a valid prefix ends before its auction

Harness log order is normative: a `recovery` event is appended when restart hello concludes, before the current ply's `auction` event, which is appended only when that ply resolves, voids, or aborts. Therefore this is a valid accepted prefix under revision 2's M5 rule:

```text
game_start
... prior complete auctions ...
recovery(X)
recovery(O)  # optional second recovery
<EOF before the auction event>
```

The current `ReplayModel` has only `SetupStep`, `AuctionStep[]`, and `TerminalStep`; §2.1 attaches recoveries to the later `AuctionStep`. In the prefix above no such step exists, so a schema-valid logged event would be dropped, force a synthetic auction, or make derivation partial. Each choice contradicts the stated total fold and the visible partial-log contract. — E1 — harness design §8 lines 123–138, especially normative causal order at line 137; UI revision 2 lines 33–36, 41–42, and 59–61.

Required fold:

1. Add a truthful representation for one or more complete trailing `recovery` events with no following auction event — for example `TrailingRecoveryStep[]` / `IncompleteTail`, or a `ReplayModel.trailingRecoveries` collection rendered at the final cursor. Do not synthesize bids, attempts, an auction outcome, or a post-state that was never logged.
2. Preserve logged order and surface each seat, triggering fault class, and restart `hello_record`; keep the final board at the last logged position and show the mid-game/incomplete marker.
3. Add a falsifiable acceptance case for a valid prefix ending after a single recovery and before its auction; include X-then-O ordering in either that case or a second case.
4. While folding, correct the provenance wording at §2.1 line 32: initial budgets come from `game_start`, while the empty board, `forced = 4`, and root `tie_owner = null` are versioned `rules:"poorman-uttt-v1"` constants; later tie owner is a projection from logged `resolution.winner`. This is not a rules-engine expansion, but those facts are not fields read verbatim from `game_start`/`post_board`.

## Design-grill, boundary, and acceptance disposition

- `GRILL_LOCK_ID: GRILL-ui-c1-20260825` remains complete and unchanged. The residual is log-model totality, not an operator UX decision; do not re-run the grill. — E1.
- Identity, locator, digest, parent lineage, R5 scope, target entity, human downstream consumer, rejected alternatives, no-code-reuse boundary, dependency policy, and out-of-scope lines: pass. — E1.
- Replay/analysis architecture and acceptance: pass except for the single recovery-prefix case above. — E1.
- Boundary contract: pass for design with the M4 ruling retained as a hard pre-PLAN gate. Because `CC` is context-only, the future design-completion report or another addressed relay must route that ask `TO: s1.orchestrator-planner`; this review does not treat the current CC line as satisfaction. — E1.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay design only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes the harness event-order contract and engine analysis semantics
- user-visible-control-with-materializer/downstream-consumer: yes — replay presentation is the target, with a human consumer
- test-runtime-role-mismatch: no — design review only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — accepting the unrepresented valid prefix would leave residual risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Incoming relay exact-file lint passes; its declared digest matches the 156-line revision 2 artifact.
- Exact diff from the prior reviewed digest confirms the fold is limited to M1–M5, status, and ledger content; `git diff --check` is clean.
- Harness owner digest remains `11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440`; its causal recovery-before-auction order is unchanged.
- No UI source, test, plan, dependency, branch, commit, PR, merge, publication, or deployment action was performed.

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh `DESIGN_SHA256`, `PARENT_DISPATCH_ID: ui-c1-design-2`, and `IN_REPLY_TO` naming this review. Fold only the trailing-recovery representation/test plus the provenance correction; preserve every closed M1–M5 decision, the R5 scope, and the existing GRILL_LOCK.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-090729.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-090729`
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/PLAN-REVIEW-pair-implementer-20260825-090632.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085542.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085947.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-085754.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-planner-20260825-090153.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-085835.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-085328.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-planner-20260825-090146.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-085705.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-090729.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-planner-20260825-090309.md
(INDEX contains this seat's one new row after registration; every other listed change is pre-existing/concurrent foreign work and was not edited by this seat.)
