ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: ui-c1-plan-review-4
PARENT_DISPATCH_ID: ui-c1-plan-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the bounded terminal-contract correction is coherent; implementation-resume acknowledgment remains orchestrator-owned
FROM: ui.implementer
TO: ui.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-plan-4/PLAN-planner-20260825-204504.md
DESIGN_LOCK_ID: DD-ui-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-ui-c1-20260825
DESIGN_SHA256: a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95
PLAN_LOCK_ID: PL-ui-c1-20260825
PLAN_ARTIFACT: PL-ui-c1-20260825
PLAN_SHA256: d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a
PLAN_REVIEW_VERDICT: approve
BUNDLE_ID: ui-c1
OWNER: ui
REPO: /Users/jack/Programming/utt
BASE: origin/main@54b443df218f3c023ab411cad86d3e0002ebe4fb
TARGET_BRANCH: main
BRANCH: ui-c1-impl
SUBJECT: PLAN-REVIEW approve — both-zero terminal now conforms to the landed exact-tie contract

## Verdict

Approve `PL-ui-c1-20260825` revision 4 at digest `d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a`.

The bounded fold closes `ui-c1-impl-blocker-1`: `both-zero.jsonl` may reach `{X:0,O:0}` mid-game and continue legally, but equal terminal budgets must produce `{result:"draw", reason:"exact_tie_draw", budget_margin:0}`. The revised Task-2 fixture requirement now states that owner contract exactly while preserving its UI purpose: typed not-applicable percent/share behavior and visible exact `0 / 0` units.

This approval covers only the exact revision-4 plan bytes. It grants no implementation resume, scaffold, dependency install, source/test/design/plan edit, branch action, commit, PR, merge, publication, deployment, or release authority.

## Bounded-delta disposition

- Artifact identity and lineage: pass. The incoming relay and current plan artifact both pin `d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a`; the unchanged design carrier and approved design artifact remain `a2e80f1e437462b51d7eab8f52394964248404acff79e50daeb4307762cf1a95`. The plan remains parented to approving `ui-c1-design-review-7` and explicitly answers the implementation blocker. — E1/E2 instrument evidence.
- Exact rev-3 to rev-4 diff: pass. The retained revision-3 bytes in the implementation worktree hash to `6309f3ef52351e8f967575b876e948ff808d48552defb67ad73b17c5c0782503`. Their diff against revision 4 contains exactly the declared revision note and the single `both-zero.jsonl` fixture-line replacement; no other plan byte changed. — E2 instrument evidence.
- Owner-contract conformance: pass. `docs/protocol/poorman-uttt-protocol-v1.md` line 213 requires a strict remaining-budget leader for `chip_count` and maps equality to `draw` / `exact_tie_draw`; `referee/poorman_referee/referee.py` lines 105–114 implements that same branch; `referee/tests/test_referee_e2e.py` lines 307–323 proves equal terminal budgets end `draw` / `exact_tie_draw`. Zero margin therefore cannot legally pair with `chip_count`. — E1/E2 repository evidence.
- UI acceptance preservation: pass. Task 5 still requires total `share(0,0)` handling and the exact not-applicable string; Task 11 still requires that string plus exact zero units. The fixture correction changes terminal semantics only and does not weaken either acceptance obligation. — E1.
- Prior passing dispositions: pass. Owner-schema gating, rules-owner fixture provenance, recovery composition, locked scope, visual/accessibility criteria, and the remaining 15-task TDD structure are outside the two-line diff and remain byte-identical to approved revision 3. — E1/E2 instrument evidence.

## Authority transition

The standing direct implementation dispatch `ui-c1-impl-1` pins the superseded plan digest `6309f3ef52351e8f967575b876e948ff808d48552defb67ad73b17c5c0782503`; this review does not rewrite or extend it. Consistent with the incoming carrier, `ui.planner` must relay this approval TO `s1.orchestrator-planner` and obtain a fresh direct dispatch or explicit digest acknowledgment at revision-4 digest `d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a` before Tasks 2–15 resume.

Task 1 commit `ui-c1-impl@ab7276f9ee147b477236f696a7f7c76813306432` is not invalidated by this plan-only correction, but no implementation action was taken under this review. The implementation hold remains in force pending the orchestrator-owned authority refresh.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — analysis display takes no external action
- worker/scheduler/queue/retry/async-side-effect: no — UI has no worker or external side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI consumes the landed harness protocol, schemas, and conformance artifacts
- user-visible-control-with-materializer/downstream-consumer: yes — replay derivation materializes human-facing views
- test-runtime-role-mismatch: no — review-only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the bounded plan bug is closed; implementation authority remains an explicit gate
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming PLAN relay, current and retained prior plan bytes, approving design review, prior PLAN-REVIEW, direct IMPL dispatch, implementation blocker, landed protocol, referee terminal logic, owner E2 test, index projection, and daemon process state were inspected directly. — E1/E2 instrument evidence.
- Incoming exact-file lint, current plan/design digest checks, exact revision diff, prior-plan digest check, and `git diff --check` pass. — E2 instrument evidence.
- No source, test, design, plan, scaffold, dependency, branch, commit, PR, merge, publication, deployment, or release action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/ui.implementer/PLAN-REVIEW-pair-implementer-20260825-204916.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT: clean at the final pre-draft inspection; daemon draft transport is ignored and the rendered relay plus INDEX projection are daemon-owned admission outputs
