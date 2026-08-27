## Team engine — engine-c4 search math: PROCEED TO PLAN

ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: engine-c4-proceed-1
RUN_ID: s1
SUBJECT: PROCEED TO PLAN — engine-c4 (DD-engine-math-c4 approved at rev 5)
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — downstream: standing gates (merge separately gated; A1-A11 acceptance incl. the G4 tournament bar proves at IMPL/E3)
FROM: s1.orchestrator-planner
TO: engine.planner
CC: engine.implementer, s1.orchestrator-reviewer
PARENT_DISPATCH_ID: engine-c4
APPROVED_DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1
APPROVING_DESIGN_REVIEW_DISPATCH_ID: engine-c4-design-review-5
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
BUNDLE_ID: engine-c4
OWNER: engine pair
REPO: /Users/jack/Programming/utt (origin https://github.com/iwnlcern/utt; engine surface = engine/ per DD)
BASE: main@d7acb6a
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision; dispatch filed via daemon admission with no verify events beyond the adjudicated set.
FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay authored at filing time; no work actions claimed by this relay

Implementer phase scope — PLAN-REVIEW after plan is drafted.
Current scope: answer design questions, review Planner's plan, findings inline.
Not in current scope: source/test edits, implementation branches, commits, PRs, scaffolding, or prototype implementation.
Implementation begins only after a daemon-admitted relay under this run's RELAY_ROOT carries the literal dispatch token bare, unfenced, un-backticked, alone on its own line, addressed to the Implementer in TO. Urgency is not dispatch; inline, quoted, fenced, CC-only, or non-addressee mentions are inert.

Approved design context:
DD-engine-math-c4-20260826 rev 5 @ a67b6b38…, approved by engine-c4-design-review-5 (parented to engine-c4-design-5) after five review rounds; digest verified against the on-disk artifact this turn; grill GRILL-engine-c4-20260826 locked with operator decisions G1-G4 consumed (directed-rounding intervals; single-threaded s1 with SMP-precondition TT; hand weights + bootstrap fit non-blocking; >=90% points vs each baseline over 50 paired seat-swapped games).
Acceptance rows A1-A11 carry verbatim into the PLAN, including the theory-oracle parity rows and the G4 tournament bar at E3 via the referee.
MR17 (backup_cases consumed_by tag) is routed to theory by a companion relay; per the DD, engine consumes those records as labeled non-contract reference vectors regardless — no plan dependency.
This relay is sequencing only and carries no design-doc lock. Author your gated PLAN FROM engine.planner with DESIGN_LOCK_ID: DD-engine-math-c4-20260826, DESIGN_RECORD_KIND: design-doc, PARENT_DISPATCH_ID: engine-c4-design-review-5.
