## Team theory — theory-c1: PROCEED TO PLAN

ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: theory-c1-proceed-1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: standing gates (P1/P2 operator ratification at math lock; INDEX-inversion block on delegated dispatch/merge/adapter consumption)
FROM: s1.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s1.orchestrator-reviewer
RUN_ID: s1
PARENT_DISPATCH_ID: theory-c1
APPROVED_DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 943d25db26fce8d9891ee5989cbd5d8be5946a63bd7cf498f738a02a8c6d8cbe
APPROVING_DESIGN_REVIEW_DISPATCH_ID: theory-c1-design-review-3
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
BUNDLE_ID: theory-c1
OWNER: theory pair
REPO: /Users/jack/Programming/utt (monorepo; theory surface = theory/ plus fixture artifacts per DD)
BASE: main@b26a90a
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision.
FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay authored at filing time; no work actions claimed by this relay

Implementer phase scope — PLAN-REVIEW after plan is drafted.
Current scope: answer design questions, review Planner's plan, findings inline.
Not in current scope: source/test edits, implementation branches, commits, PRs, scaffolding, or prototype implementation.
Implementation begins only after a current relay under the active run's RELAY_ROOT contains the exact literal token `DISPATCH IMPL` bare, unfenced, un-backticked, alone on its own line, and addressed to the Implementer in TO, or a direct message to that single Implementer contains the same bare own-line token. Urgency is not dispatch; inline, quoted, fenced, CC-only, cross-read, or non-addressee mentions are inert.

Approved design context:
DD-theory-c1-20260825 revision 3, approved by theory-c1-design-review-3 (parented to theory-c1-design-3); digest verified against the on-disk artifact this turn.
Scope per the DD: findings doc; Stage-1 two-oracle package (exact-rational continuous DP + exact integer-budget brute force, unrestricted canonical 3x3 + center-forced variant, exhaustive N <= 32 with spot 64/128); rules-fixture schema v1 (forced integer-or-null, locked by the 080619 acknowledgment); LP reference with exact certificate.
Carried obligations accepted as PLAN obligations: C8a attempt-and-record; C9 non-determinacy = hard escalation and math-lock blocker; N = 24 fallback is orchestrator-gated; P1/P2 remain operator-ratified at the math lock.
Standing gate reminder: the run-level INDEX-inversion block means the delegated DISPATCH IMPL may not be exercised while relay-root lint is red — sequence your plan and review now; dispatch only once the root is green or operator-waived.
This relay is sequencing only: it does not carry the gated design-doc lock. Author your gated PLAN FROM theory.planner with DESIGN_LOCK_ID: DD-theory-c1-20260825, DESIGN_RECORD_KIND: design-doc, and PARENT_DISPATCH_ID: theory-c1-design-review-3.
