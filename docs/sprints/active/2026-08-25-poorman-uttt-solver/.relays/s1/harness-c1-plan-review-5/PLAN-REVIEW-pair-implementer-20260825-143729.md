ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-plan-review-5
PARENT_DISPATCH_ID: harness-c1-plan-5
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — one exact lock-coherence defect remains within Planner authority
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-plan-5/PLAN-planner-20260825-143208.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: 79ea02702b4dc9d2755981e55c2b9ba099f0c47df7578ac1face570f85eae600
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c1
OWNER: harness
SUBJECT: PLAN-REVIEW reissue 5 — must revise stale embedded design digest

## Verdict

Must revise `PL-harness-c1-20260825` at digest `79ea02702b4dc9d2755981e55c2b9ba099f0c47df7578ac1face570f85eae600` before implementation dispatch.

The relay-level lineage and digests are shaped correctly, and the plan's Task 7/9 behavior agrees with amended design digest `c935c29c…`. The plan artifact itself, however, still embeds the superseded design digest `11ac4efc…` in its lock header while acceptance criterion 1 requires that exact header digest to be the locked design. A relay header cannot silently replace contradictory artifact bytes. The claimed byte identity is therefore the defect, not evidence that the re-lock is complete.

This review is read-only and grants no implementation, merge, publication, deployment, or release authority. The stale `harness-c1-impl-1` dispatch remains dead.

## Must-revise finding

### R1 — The plan artifact still locks the superseded design digest

`PL-harness-c1-20260825.md` line 6 names `DESIGN_LOCK_ID: DD-harness-c1-20260825 @ sha256 11ac4efc…`. The same artifact's acceptance criterion 1 says the exact design lock and digest are locked by that header. The incoming PLAN relay instead names the approved amendment at `c935c29c…` and says the plan was re-locked without changing its bytes. Both lock claims cannot be true simultaneously. — E1 — plan lines 5–15 and 433–439; incoming relay lines 16–22 and 32–39.

Required: update the plan artifact's embedded design digest to `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`, recompute `PLAN_SHA256`, and issue successor PLAN `harness-c1-plan-6` carrying that fresh plan digest and the same approved design lineage. Preserve the plan's already-approved Task 1–13 semantics, scope, tests, boundary contract, hard fences, theory-seam honesty rule, and operator-items-none disposition.

## Preserved approvals and gates

- Design lineage: pass. `harness-c1-plan-5` parents to approving `harness-c1-design-review-4`, which parents to the Planner's amendment relay, all with matching `DESIGN_DOC_ID` and digest `c935c29c…`. — E1.
- Plan behavior: preserved. The artifact digest exactly matches the previously approved `harness-c1-plan-review-4` bytes; recovery ordering, association keys, recovery-fault shapes, sealed collection, TDD sequence, fault matrix, E2 suite, E3 smoke, and theory-loader honesty remain approved in substance. — E1.
- Scope and boundary: preserved. Writes remain `referee/**` and `docs/protocol/**`; theory fixtures are read only through the loader seam; engine/theory edits, bridge/network, ratings/SPRT/platform, merge, deployment, and release remain out of scope. — E1.
- Base drift: non-blocking. The PLAN reissue recorded `main@4bc2535`; current main is `fd947ef`. The required dispatch-time rebase, collision census, and all-in `SCOPE_DIFF` correctly own ordinary base movement. — E1.
- Instrument shape: the incoming relay exact-file lint passes with freshness disabled; daemon status is ready with zero pending renders and only the adjudicated INDEX divergence. The truth-agnostic linter does not inspect the plan document's embedded digest claim. — E2.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local plan review
- migration/backfill/destructive-write/canonical-data-repair: no — pre-implementation artifact correction
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated integer chip budgets only
- AI-or-automation-acts-downstream: no — no external action
- worker/scheduler/queue/retry/async-side-effect: yes — planned referee supervises subprocess recovery
- cross-repo/service-contract/generated-schema/shared-API-event: yes — protocol/log schemas have engine and UI consumers
- user-visible-control-with-materializer/downstream-consumer: yes — log replay has a named UI/human consumer
- test-runtime-role-mismatch: no — review-only
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — exact metadata correction only; no semantic branch reopened
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and next action

- Measured artifact hashes: plan `79ea02702b4dc9d2755981e55c2b9ba099f0c47df7578ac1face570f85eae600`; amended design `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`. — E1.
- `git diff` confirms no plan-byte delta; that observation proves the stale embedded digest persisted. No tests were run because this phase is review-only and no executable bytes changed. — E1.
- No source, test, design, plan, dependency, implementation-worktree, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

Next: return `harness-c1-plan-6` with the one-line embedded digest correction, fresh `PLAN_SHA256`, parent `harness-c1-design-review-4`, and `IN_REPLY_TO` pointing to this review. A fresh approving PLAN-REVIEW remains required before any successor implementation dispatch.

ACTIONS_GIT_REF:
- review=draft authored under `.engine/drafts/harness.implementer/` for daemon admission
- source/tests/design/plan=none — review-only

FINAL_GIT_STATUS_SHORT: none — clean implementation worktree at `/Users/jack/Programming/utt/.worktrees/harness-c1-impl`

(The primary checkout contains pre-existing and concurrent foreign design, plan, INDEX, and relay changes. This seat authored only the daemon draft and its admitted PLAN-REVIEW relay.)
