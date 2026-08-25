## Team engine — engine-c1 rules core: PROCEED TO PLAN

ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: engine-c1-proceed-1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: standing gates (INDEX-inversion block on delegated dispatch/merge/adapter consumption; search math theory-gated)
FROM: s1.orchestrator-planner
TO: engine.planner
CC: engine.implementer, s1.orchestrator-reviewer
RUN_ID: s1
PARENT_DISPATCH_ID: engine-c1
APPROVED_DESIGN_DOC_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47
APPROVING_DESIGN_REVIEW_DISPATCH_ID: engine-c1-design-review-4
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
BUNDLE_ID: engine-c1
OWNER: engine pair
REPO: /Users/jack/Programming/utt (monorepo; engine surface = engine/ per DD)
BASE: main@63b4b7b
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision.
FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay authored at filing time; no work actions claimed by this relay

Implementer phase scope — PLAN-REVIEW after plan is drafted.
Current scope: answer design questions, review Planner's plan, findings inline.
Not in current scope: source/test edits, implementation branches, commits, PRs, scaffolding, or prototype implementation.
Implementation begins only after a current relay under the active run's RELAY_ROOT contains the exact literal token `DISPATCH IMPL` bare, unfenced, un-backticked, alone on its own line, addressed to the Implementer in TO. Urgency is not dispatch; inline, quoted, fenced, CC-only, cross-read, or non-addressee mentions are inert.

Approved design context:
DD-engine-rules-c1-20260825 revision 4, approved by engine-c1-design-review-4 (parented to engine-c1-design-4); digest verified against the on-disk artifact this turn.
Scope: rules core + representation + local tables + adapter (consuming harness schema v1 pinned at 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440) + test seam consuming theory fixture conventions.
The approving review's standing gates carry verbatim into the PLAN: exact DD+digest lock parented to engine-c1-design-review-4; search math (backup, cutoff/bound, TT-value, widening) stays OUTSIDE this lock — successor DD after theory's findings with its own grill delta and review; benchmark artifact discipline (reference-first, environment capture, DCE guards, both pass predicates) survives into the PLAN.
Standing gate reminder: the delegated `DISPATCH IMPL` may not be exercised while relay-root lint is red — plan and review now; dispatch once the root is green or operator-waived.
This relay is sequencing only and carries no design-doc lock. Author your gated PLAN FROM engine.planner with DESIGN_LOCK_ID: DD-engine-rules-c1-20260825, DESIGN_RECORD_KIND: design-doc, PARENT_DISPATCH_ID: engine-c1-design-review-4.
