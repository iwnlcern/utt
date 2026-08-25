ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-design-review-1
PARENT_DISPATCH_ID: ui-c1-design-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — revision is planner-owned; cross-pair carrier clarification remains orchestrator-routed
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/ui-c1/DESIGN-planner-20260825-084851.md
DESIGN_DOC_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: bd801a3d425adf3afacf235521156a16d9415961e3200efa01f2b24d747f8dea
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — replay timebase and total analysis states need closure

## Verdict

`DD-ui-c1-20260825` has a sound log-driven direction and a complete operator grill, but it is not yet implementable as a total replay/analysis model at the reviewed digest. The selected post-auction `Frame` semantics conflict with the pre-auction state to which engine `info` applies; the frame-only model has no object on which to render a setup `hello_fault`; and two required analysis edge contracts are missing. Revise the same `DESIGN_DOC_ID` and return a successor DESIGN-REVIEW request with a fresh digest.

This verdict reviews design bytes only. It does not authorize PLAN, implementation, scaffolding, dependency installation, source/test edits, branch creation, commit, PR, merge, publication, or deployment.

## Must-revise findings

### M1 — Lock one explicit replay timebase and cover games with zero auctions

The harness log records each engine reply, including `info`, for the state sent in the turn request, then records `resolution`, `post_board`, `budgets_after`, and `forced_next` after the auction resolves. The UI design instead gives one `Frame` per ply a board/budgets/forced state, says the board contains moves through the selected ply, and overlays conditional ghosts for that ply's "current pending auction." Those are different positions: a conditional move from the reply belongs on the pre-auction board, not the post-resolution board on which one intent may already have been applied. — E1 — harness design §§3.3–3.4 and §8; UI design §§3, 5, and 6.

The same `Frame[]` model cannot attach terminal information to "the last frame" when startup ends with `hello_fault` before ply 0; the locked harness model permits that trace and its conformance set covers hello faults. — E1 — harness design §§3.2, 8, and 10; UI design lines 31, 54, and 74–79.

Required fold:

1. Define explicit setup/pre-auction/post-resolution semantics (or an equivalent typed `ReplayStep` model) and state exactly which board, budgets, forced board, and tie owner each view consumes.
2. Associate each seat's `info`, bid, intent, and conditional ghosts with the pre-auction state for its `request_id`; keep the logged post-state distinct and never infer it by executing rules.
3. Define a setup/terminal-only representation for zero-auction `hello_fault` games, plus resolved, voided, and aborted auction traces; specify how causally preceding recovery events attach.
4. Add falsifiable tests for zero-auction terminal logs and for a ply where the applied move differs from both conditional ghost destinations, proving the overlay uses the pre-auction position.

### M2 — Make the share and margin total when both budgets are zero

The theory contract explicitly requires all-in-to-both-zero and continuing both-zero chains. At such a state `B_X + B_O = 0`, so `p = B_X/(B_X+B_O)`, `p-T`, and a critical-bid percentage of combined budget are undefined. The current formatter and metrics contract has no zero-denominator result and would otherwise produce a misleading value or `NaN`. — E1 — theory design §§2–3, especially the required both-zero fixture category; UI design lines 33, 60, 64, 76, and 79.

Required fold: define an explicit unavailable/not-applicable state for `p`, signed margin, and any combined-budget percentage when the total is zero; continue to show exact integer budgets and any verbatim analysis fields that remain meaningful. Add a formatter/metrics test using the required both-zero fixture shape. No new operator choice is needed.

### M3 — Consume the approved value-quality metadata rather than reducing it to a badge

The approved engine owner record locks `quality`, `lo`, `hi`, `depth`, and `complete` as UI-consumed analysis metadata. The UI's `AnalysisEntry` retains only `quality`, and the surface promises only an exact/bound/estimate badge. A `bound` badge without its interval and provenance cannot satisfy the audit's honest bounded-value requirement. — E1 — `DD-engine-rules-c1-20260825` §§10 and 12 at sha256 `265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47`; approving review `engine-c1-design-review-4`; UI implementer audit analysis-carrier finding; UI design lines 32, 60–65, and 79.

Required fold: extend the typed extraction, malformed/degraded states, surface, and fixture acceptance to preserve and honestly render the approved metadata shape, including `lo`/`hi` for a bound and `depth`/`complete` when present. Keep the public harness rule that arbitrary or malformed advisory `info` never invalidates an otherwise valid game log.

### M4 — Close the state/perspective convention before PLAN and remove assumed contract language from the lock

The design correctly names the remaining carrier clarification, and the approved engine record already establishes `T` as X-centric. However, the locked harness carrier still accepts any object and does not pin whether `pv_if_win`/`pv_if_lose` are emitter-relative or canonical X/O, nor does it state the stable pre-auction state key beyond attachment to a turn record. The UI's ghost mapping and "matches logged info verbatim" acceptance therefore depend on an assumption that can reverse the O-seat lines. An isolating mapping function limits code churn but does not make either interpretation correct. — E1 — harness design §4; engine design §§9–10; UI design lines 15, 31–32, 61–62, 79, and 88.

Required fold: retain this as an orchestrator-routed owner-contract gate, but make it a required pre-PLAN input rather than a non-blocking assumption. Pin the analysis state association and PV perspective in the producing owner contract; cite that ruling from the successor. Until pinned, define such fields as unavailable rather than mapping them speculatively. This is contract clarification, not a new product/UX question.

### M5 — Reconcile partial-log viewing with fail-closed parsing

The design says missing/type-invalid required data fails closed, while also saying truncated logs derive frames through the last complete event. A crash can yield either a valid complete-event prefix with no `game_end` or a syntactically incomplete final JSONL line; those cases currently have no distinct policy. — E1 — UI design lines 30, 48, and 50.

Required fold: state whether partial viewing accepts only a schema-valid complete-event prefix (recommended) and treats a malformed final line as `LogError`, or explicitly define a safe ignored-tail rule. Add one acceptance case for a complete prefix missing `game_end` and one for a malformed final line.

## Design-grill and checklist disposition

- `GRILL_LOCK_ID: GRILL-ui-c1-20260825` is present and traceable to the addressed `GRILL_REQUIRED: yes` dispatch. The six operator decisions, rejected alternatives, and `Still operator-owned: none` disposition are coherent; preserve them without re-running the interview. — E1.
- Identity, digest, parent lineage, target entity, scope narrowing under R5, no-code-reuse boundary, dependency policy, ownership, and human downstream consumer: pass. — E1.
- No-rules-engine architecture, structured load errors, integer-exact monetary handling, coordinate convention, keyboard/a11y direction, and harness-fixture test seam: pass subject to M1/M2/M5 totality corrections. — E1.
- Boundary and analysis carrier: fail pending M3/M4. The successor must distinguish the harness public carrier schema from the harness-owned and engine-owned producer semantics it consumes. — E1.
- Acceptance criteria: fail pending explicit zero-auction, pre/post-position, both-zero, quality-metadata, and partial-tail cases. — E1.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay design only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis is displayed to a human and takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes harness log schemas and engine analysis semantics
- user-visible-control-with-materializer/downstream-consumer: yes — replay/analysis presentation is the target, with a human consumer
- test-runtime-role-mismatch: no — design review only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — M1/M4 expose unresolved state and perspective semantics
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Incoming relay passes exact-file relay lint; its declared UI digest and the approved harness digest both match the current artifacts.
- Reviewed current owner records: harness `11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440`, UI `bd801a3d425adf3afacf235521156a16d9415961e3200efa01f2b24d747f8dea`, and engine rules `265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47`.
- `git diff --check` is clean. No UI source, test, design, plan, dependency, branch, commit, PR, merge, publication, or deployment action was performed.
- Relay-root INDEX lint remains red on inherited/concurrent append-order inversions; this seat preserves foreign rows and updates only its own row/filename to the real filing time. Exact-file lint is the proof for this review artifact; delegated dispatch and automated root consumption remain governed by the existing reconciliation hold.

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh `DESIGN_SHA256`, `PARENT_DISPATCH_ID: ui-c1-design-1`, and `IN_REPLY_TO` naming this review. Close M1–M5, preserve the R5 scope and GRILL_LOCK decisions, and cite the routed owner-contract clarification for M4 or keep PLAN gated until it exists.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-085705.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-085705`
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085542.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-085647.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-085328.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-085705.md
(INDEX contains this seat's one row; every other listed change is concurrent foreign work and was not edited by this seat.)
