ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-design-review-5
PARENT_DISPATCH_ID: ui-c1-design-5
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — an existing orchestrator ruling supplies the semantics; only a document-currentness fold is required
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-design-5/DESIGN-planner-20260825-150224.md
DESIGN_DOC_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: f480e979d2797d9552e0fb33c0c60faefc2e22a4c597492f81b89353113112ef
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: ui-c1
OWNER: ui
SUBJECT: DESIGN-REVIEW must-revise — recovery and closure folds pass; cited M4 ruling is not folded into the lock

## Verdict

Must revise `DD-ui-c1-20260825` revision 5 at digest `f480e979d2797d9552e0fb33c0c60faefc2e22a4c597492f81b89353113112ef`. The harness-amendment recovery fold and the cumulative closure carrier close PLAN-REVIEW MR1/MR4 correctly. Approval is blocked by one independent lock-currentness contradiction: revision 5 adds `s1-m4-ruling-1` to its inputs of record but leaves every operative M4 paragraph in the pre-ruling state, saying the perspective and state-key ruling has not landed.

This verdict is review-only. It authorizes no implementation, scaffolding, dependency installation, source/test/plan edits, branch creation, commit, PR, merge, publication, deployment, or release. A gated PLAN must not parent to this must-revise review.

## MR1 — fold the already-issued M4 ruling into the design lock

The orchestrator ruling fixes `pv_if_win` / `pv_if_lose` as canonical X/O and fixes analysis association direction as the harness-owned canonical pre-state hash with `(game_id, ply)` as the human-readable secondary key. It explicitly permits UI PLAN to cite the ruling as M4 gate satisfaction while harness pins the exact hash definition in its protocol/conformance artifact. — E1 — `s1-m4/RECONCILE-orchestrator-planner-20260825-143955.md` lines 15–26.

Revision 5 cites that ruling in its input ledger, but §6 still says both perspective and stable state key are not pinned, renders PV as `unavailable — perspective unpinned`, and says ghost rendering activates only once the M4 perspective ruling lands. §9 still labels both questions a REQUIRED pre-PLAN gate and says "Until the ruling exists"; the GRILL_LOCK impact and review ledger preserve the same pre-ruling wording. Those statements cannot all be true after the cited ruling and after `ui-c1-plan-1` was expressly allowed to cite it. — E1 — design lines 6, 76–77, 104, 142, 157, and 173 versus ruling lines 17–26.

Required successor fold:

1. State that the perspective decision has landed and is canonical X/O.
2. State that the state-key direction has landed: harness canonical pre-state hash plus `(game_id, ply)` as secondary key; harness still owns and must publish the exact hash input bytes, algorithm, and encoding.
3. Distinguish the satisfied orchestrator-ruling gate from the pending harness artifact pin. Preserve the accepted anti-half-fix posture: conditional PV rendering may remain unavailable behind the single pin until that artifact exists, but the reason must no longer be `perspective unpinned` and the design must not say the ruling has not landed.
4. Update §6, §9, the GRILL_LOCK design-impact line, and the review-fold ledger together. Preserve `s1-m4-ruling-1` as authority and do not invent the harness hash definition locally.

No human decision is required: this is a faithful fold of an existing orchestrator ruling, with the remaining owner-artifact obligation already assigned to harness.

## Passing dispositions

- Artifact identity/integrity: pass. The carrier resolves to current design bytes at `f480e979d2797d9552e0fb33c0c60faefc2e22a4c597492f81b89353113112ef`; incoming exact-file lint passes. — E1/E2 instrument evidence.
- Harness owner pin: pass. Current harness design bytes match approved amendment digest `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`. — E1.
- Recovery association: pass. Normative fields include `seat`, `ply`, `trigger_request_id`, `fault`, and `hello_record`; attachment uses `ply`/`trigger_request_id`, never emission position, while raw order remains visible. — E1 — UI design lines 35 and 45; harness lines 135 and 138–140.
- Dual positions and terminal shapes: pass. Double-fault recovery precedes its auction; single-fault recovery follows and attaches backward; both recovery-fault shapes preserve whether position advanced. — E1 — UI lines 35–36; harness lines 138–141.
- Trailing recovery totality: pass. Only a recovery whose auction is absent is trailing; a matching post-auction recovery at EOF attaches backward. Named tests cover both classifications and X-then-O order. — E1 — design lines 37, 40, and 92.
- Cumulative closures: pass. `Position.closed` initializes empty and folds only logged `resolution.closures`; prior closures persist and Board executes no rules. — E1 — design lines 22, 32, 69, and 92.
- Boundary, target, and scope: pass. The design consumes harness schema/fixtures, writes only UI-private state, targets rendered replay/analysis views for humans, and routes insufficiency upward. R5 exclusions and dependency policy remain intact. — E1 — design lines 10–26, 42–51, and 97–106.
- Plan-layer MR2/MR3/MR5 remain correctly deferred to the reissued PLAN. This review does not approve stale plan bytes. — E1.

## Design-grill pressure pass

`GRILL_REQUIRED: yes` was applied. The amended recovery tree was answerable from the approved harness owner record, so no operator question was asked: keys uniquely route pre/post-auction recovery; raw order remains a display fact; missing-auction recovery is the only trailing case; each terminal shape has an unambiguous position result. The closure branch was answerable from the log sufficiency contract: cumulative logged closures are the only carrier consistent with zero re-execution. Those branches pass.

The M4 currentness branch fails for the reason above. It reopens no product choice; the orchestrator answer already exists. Preserve the six original operator decisions and rejected alternatives while folding the ruling's current state.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay design only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — UI has no worker or external side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes harness recovery and analysis contracts
- user-visible-control-with-materializer/downstream-consumer: yes — replay model materializes human-facing views
- test-runtime-role-mismatch: no — review-only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — contradictory M4 lock text would leave implementation ambiguity
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Current design/harness digests were recomputed; incoming DESIGN and approved harness review were read at exact rendered paths. — E1.
- `git diff` confirms the recovery/closure delta; `git diff --check` is clean. — E1/E2 instrument evidence.
- No UI source, test, scaffold, dependency, plan, branch, commit, PR, merge, publication, deployment, or release action was performed.

Return `ui-c1-design-6` (or next unused DESIGN id) with the narrow M4 currentness fold and fresh digest, then request fresh DESIGN-REVIEW. Only after approval should `ui-c1-plan-2` be reissued with MR2/MR3/MR5 folded.

ACTIONS_GIT_REF:
- review=lintable staging bytes prepared for daemon admission
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT: unavailable — relay draft guard cannot embed path-bearing status bytes in a non-rendered staging target; fresh literal status was captured immediately before authoring and will be reported in the handoff
