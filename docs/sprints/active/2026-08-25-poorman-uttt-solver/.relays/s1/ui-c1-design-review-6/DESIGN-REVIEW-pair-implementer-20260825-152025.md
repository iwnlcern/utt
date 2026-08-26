ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-design-review-6
PARENT_DISPATCH_ID: ui-c1-design-6
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — current owner digest is already approved; only a mechanical consumer-pin refresh is required
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-design-6/DESIGN-planner-20260825-151628.md
DESIGN_DOC_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: c42bc13a1b02ed5832162575cd6f7f43a20b9900ab493c1b73af4a2c1807a05f
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: ui-c1
OWNER: ui
SUBJECT: DESIGN-REVIEW must-revise — M4 fold passes; engine analysis owner digest is superseded

## Verdict

Must revise `DD-ui-c1-20260825` revision 6 at digest `c42bc13a1b02ed5832162575cd6f7f43a20b9900ab493c1b73af4a2c1807a05f`. The four requested M4 currentness folds are complete and internally consistent. Approval is blocked by one newly current boundary-integrity defect: the UI design still names superseded engine-design digest `265773e3…` as the lock for its value-quality metadata, although engine amendment 1 was approved at digest `1d13153f…` before this UI successor was filed.

This verdict is review-only. It authorizes no implementation, scaffolding, dependency installation, source/test/plan edits, branch creation, commit, PR, merge, publication, deployment, or release. A gated PLAN must not parent to this must-revise review.

## MR1 — refresh the engine analysis owner pin

UI §3 says its `AnalysisEntry` full value-quality metadata shape is locked by `DD-engine-rules-c1-20260825` §10 at SHA-256 `265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47`. — E1 — UI design line 46.

The current legitimate owner lineage has floated that same design identity to amendment-1 digest `1d13153f2af22d1f2f55023292eef49827d78d8e0230f93dbdec2e66e48c8f62`. `engine-c1-design-review-5` approved that exact digest at 15:08:53, explicitly proving that the value-quality metadata shape and all engine-facing message shapes were preserved byte-for-byte outside the narrow harness-owner-digest amendment. The UI rev-6 carrier was filed later, at 15:16:28. — E1 — `engine-c1-design-review-5/DESIGN-REVIEW-pair-implementer-20260825-150853.md` lines 23–38; current owner artifact digest recomputation.

Required successor fold:

1. Replace the superseded engine digest in §3 with `1d13153f2af22d1f2f55023292eef49827d78d8e0230f93dbdec2e66e48c8f62` and cite `engine-c1-design-review-5` as the approving lineage.
2. Record the mechanical owner-pin refresh in the revision ledger or inputs-of-record provenance so the digest float is acknowledged rather than silent.
3. Preserve the metadata shape, M4 ruling fold, `PV_PIN`, recovery/closure model, GRILL_LOCK decisions, R5 scope, and every acceptance criterion unchanged. The engine amendment created no new UI semantic or implementation obligation.

No operator decision is required. The owner amendment and its approval already establish the exact successor bytes and contract preservation.

## M4 finding disposition — pass

- Perspective: pass. §6 now says the ruling landed and fixes move-valued analysis fields as canonical X/O with identity mapping. — E1 — design line 76; ruling lines 17–18.
- State-key direction: pass. §6 and §9 carry harness canonical pre-state hash plus `(game_id, ply)` secondary and leave exact bytes/algorithm/encoding to harness. — E1 — design lines 76 and 104; ruling lines 19 and 23–26.
- Gate versus artifact pin: pass. The orchestrator-ruling gate is satisfied for PLAN while PV activation remains behind one `PV_PIN` with reason `awaiting harness artifact pin`; no stale `perspective unpinned` operational text remains. — E1 — design lines 76–77 and 104.
- Lock-wide consistency: pass. Status, §6, §9, GRILL_LOCK impact, and fifth-pass ledger agree; older pre-ruling ledger text is explicitly marked superseded history. — E1 — design lines 5, 76–77, 104, 142, and 175–177.

## Other passing dispositions

- Artifact identity/integrity: pass. Incoming locator and digest match the current UI design bytes at `c42bc13a1b02ed5832162575cd6f7f43a20b9900ab493c1b73af4a2c1807a05f`; incoming exact-file lint passes. — E1/E2 instrument evidence.
- Recovery/closure model: preserved. Key-based recovery association, dual event positions, both terminal shapes, trailing classification, cumulative logged closures, and five falsifiable cases remain unchanged from the prior passing disposition. — E1.
- Boundary, target, acceptance, and scope: pass apart from MR1's exact owner pin. The UI reads owner contracts and fixtures, writes only UI-private state, targets human replay/analysis views, and routes schema deltas upward. R5 exclusions and dependency policy remain intact. — E1.
- Plan-layer MR2/MR3/MR5 remain correctly deferred to the fresh PLAN after design approval. This review does not approve stale plan bytes. — E1.

## Design-grill pressure pass

`GRILL_REQUIRED: yes` was applied. Every M4 branch is resolved by the ruling and design bytes, so no operator question was asked. The only failed branch is owner-currentness: a consumer lock cannot continue naming a superseded digest after the same design identity has a later approved amendment, even when the consumed subsection is unchanged. Recommended answer: refresh the exact digest and provenance mechanically; reject reopening semantics or duplicating the engine contract locally.

The six original operator decisions, rejected alternatives, recovery decisions, closure carrier, and anti-half-fix posture remain sound and unopened.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay design only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — UI has no worker or external side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes the engine analysis owner contract and harness schema
- user-visible-control-with-materializer/downstream-consumer: yes — replay model materializes human-facing views
- test-runtime-role-mismatch: no — review-only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — exact owner-pin refresh; no semantic choice or accepted residual
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Current UI and engine design digests were recomputed; incoming UI DESIGN and current engine approving DESIGN-REVIEW were read at their exact rendered paths. — E1.
- `git diff --check` is clean. No UI source, test, scaffold, dependency, plan, branch, commit, PR, merge, publication, deployment, or release action was performed. — E2 instrument evidence.

Return `ui-c1-design-7` (or the next unused DESIGN id) with only the exact engine-owner pin/provenance refresh and a fresh UI digest, then request fresh DESIGN-REVIEW. After approval, reissue `ui-c1-plan-2` with the already-required plan-layer folds.

ACTIONS_GIT_REF:
- review=lintable staging bytes prepared for daemon admission
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT: none — clean tree immediately before ignored daemon-draft staging and admission
