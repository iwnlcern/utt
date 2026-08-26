ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-plan-review-2
PARENT_DISPATCH_ID: ui-c1-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the plan needs bounded technical repairs and orchestrator routing, not a new product-semantic decision
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-plan-2/PLAN-planner-20260825-155605.md
DESIGN_LOCK_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
PLAN_SHA256: 9293f04eed68c78ea49b8edc080e0457200d6e7e4dd07d4207605803dadefb5f
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: ui-c1
OWNER: ui
BASE: main@03eff1d
TARGET_BRANCH: main
BRANCH: ui-c1-impl
SUBJECT: PLAN-REVIEW must-revise — schema gate is late; recovery acceptance and legal-fixture proof remain incomplete

## Verdict

Must revise `PL-ui-c1-20260825` revision 2 at digest `9293f04eed68c78ea49b8edc080e0457200d6e7e4dd07d4207605803dadefb5f`.

The successor closes the original MR3 ordinal defect and MR4 closure-carrier defect, and it substantially folds MR2 recovery semantics and MR5 fixture intent. Approval is blocked by four execution defects: the machine-schema boundary check occurs after the plan has already committed speculative wire types; Task 15 proves only that recovery fixtures render without throwing, not that the five required semantics render correctly; the legal sample has no rules-owner proof; and the architecture carrier still names superseded UI design revision 4.

This verdict is review-only. It authorizes no scaffolding, dependency install, source/test/design/plan edit, branch action, commit, PR, implementation dispatch, merge, publication, deployment, or release.

## MR1 — move the owner-schema gate before wire types and fixtures

The plan calls the interface block “exact names” and defines `Attempt.records`, then defers the machine-schema cross-check until Task 15 after Tasks 1–14 have implemented and committed against that type. The only available harness implementation schema at `harness-c1-impl@477f491` requires the field `turns`, not `records`, in `log_auction.schema.json` lines 61–73. The schema branch is still under harness review, so it is evidence of an already-known mismatch, not yet authority to copy foreign provisional bytes.

Required successor fold:

1. Make availability of the approved/landed harness schema and conformance artifacts a precondition before authoring wire types or game-shaped fixtures, at Task 2/3 rather than Task 15.
2. Generate or manually define `src/log/types.ts` from those approved exact field names; do not keep the speculative `records` field. Preserve the existing stop-and-route rule for any mismatch.
3. Keep the late Task 15 cross-check as a drift check, but do not use it as the first point capable of discovering the contract is wrong.

This is the design's boundary rule applied early enough to prevent twelve tasks of knowingly disposable implementation.

## MR2 — Task 15 must prove recovery semantics, not only totality

Tasks 7 and 10 now correctly specify key-based attachment, backward single-fault attachment, both recovery-fault shapes, absent-auction trailing classification, and marker placement. Task 15, however, only opens every fixture through GameView “without throw.” That proves totality, not correct association or presentation. The final acceptance list still names only “trailing-recovery (2 cases)” and omits backward attachment, the at-EOF not-trailing case, and both terminal shapes.

Required successor fold: add end-to-end assertions in Task 15 and the final acceptance gates for all five locked recovery cases:

- pre-auction double-fault recoveries attach to and render with their auction in raw order;
- post-auction single-fault recovery attaches backward and renders under the resolved row;
- post-auction recovery at EOF is attached, not trailing;
- absent-auction recovery tail stays trailing with board unchanged and X-then-O order preserved;
- both double-fault-abort and resolved-then-recovery-fault terminal shapes render as logged.

The unit/component tests remain necessary; the acceptance sweep must also prove the composed parse → derive → GameView path.

## MR3 — legal fixture and bundled-sample validity need owner proof

Task 2 requires every game-shaped fixture to be legal and referee-emittable, but its only fixture-corpus test checks JSONL splitting and manifest completeness. Later parse/derive/render tests deliberately do no rules execution, so they cannot prove forced routing, legal closure order, or a legal macro win. A manifest hand-trace note is E1 intent, not the E2 legality proof needed before copying `success-macro-win.jsonl` into the bundled sample.

Required successor fold: obtain legal base transcripts from the approved harness conformance/generator output, or validate each hand fixture with a harness-owned validator/generator command after those artifacts land. UI must not add a rules core. Record the exact owner artifact/command in fixture provenance, and copy the sample only after that gate passes. Current provisional harness fixtures are not sufficient while their owner review requires regeneration.

## MR4 — correct stale lock and execution carriers

- Architecture line 13 says “Everything per DD-ui-c1-20260825 rev 4,” while the header, relay, and approved bytes lock revision 7. Change that carrier to revision 7 so the plan cannot instruct a worker to prefer the pre-recovery-amendment lock.
- The current Codex seat exposes neither a `dataviz` nor `frontend-design` skill. They cannot remain unconditional implementation prerequisites. Name available installed equivalents at dispatch time or make those consultations optional-if-available, with the DD's chart, visual, and accessibility acceptance criteria remaining mandatory.

## Passing dispositions

- Design lineage and integrity: pass. The PLAN parents to approving `ui-c1-design-review-7`; current design and plan digests match the relay carriers exactly. — E1/E2 instrument evidence.
- Original MR3: pass. Wire `ply` is zero-based, row ordinal `j` is one-based, selection uses `j-1`, sequential plies are validated, and first/middle/unresolved tests prevent underflow. — E1.
- Original MR4: pass. `Position.closed` is cumulative and log-derived; Task 9 includes both persistence and no-recomputation falsifiers. — E1.
- Original MR2 unit/component substance: pass subject to MR2 above. The recovery type carries both keys; Tasks 7 and 10 cover dual emission positions, backward association, trailing classification, and both terminal shapes. — E1.
- Original MR5 intent: pass subject to MR3 above. The impossible five-ply claim is removed and the macro-win minimum is corrected to at least nine applied winning marks. — E1.
- M4, scope, target, and ownership: pass. Canonical X/O identity mapping, the single `PV_PIN`, no-rules-execution boundary, R5 exclusions, human target, dispatch-time rebase, and fresh scope-diff fence remain intact. — E1.

## Authority transition gate

The review scan has hard triggers for the harness shared contract and the human-visible materialized UI. `ui-c1-proceed-1` delegates pair-Planner dispatch only when “no hard trigger” holds. Therefore even a corrected PLAN-REVIEW approval will not authorize delegated self-dispatch under that relay's literal conditions. Route the approved plan to `s1.orchestrator-planner` for a direct implementation dispatch or an explicit authority ruling; do not infer dispatch from plan approval.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local replay UI only
- migration/backfill/destructive-write/canonical-data-repair: no — review-only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — UI has no worker or external side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes harness schemas and conformance artifacts
- user-visible-control-with-materializer/downstream-consumer: yes — replay derivation materializes human-facing views
- test-runtime-role-mismatch: no — review-only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approval would accept unproved recovery/sample semantics and a known schema mismatch
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and requested successor

- Incoming PLAN exact-file lint passes with historical freshness disabled; current artifact digests match its carriers and `git diff --check` is clean. — E1/E2 instrument evidence.
- The approved UI lock, prior PLAN-REVIEW, orchestrator proceed relay, current harness design, provisional harness schema at `477f491`, and current harness fold findings were read directly. — E1.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action was performed by this seat.

Return `ui-c1-plan-3` with only the four bounded folds above, parented again to `ui-c1-design-review-7` and answering this review through `IN_REPLY_TO`. Preserve every passing disposition. After its review approves, route the hard-trigger authority transition to the orchestrator rather than issuing delegated implementation locally.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/ui.implementer/PLAN-REVIEW-pair-implementer-20260825-161908.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT: unavailable — exact pre-draft status was inspected, but concurrent foreign work and ignored daemon-draft transport make an embedded snapshot non-authoritative; final status is reported after admission
