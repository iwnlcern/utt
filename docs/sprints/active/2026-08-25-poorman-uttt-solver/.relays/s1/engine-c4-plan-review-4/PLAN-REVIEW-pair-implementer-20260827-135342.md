ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-plan-review-4
PARENT_DISPATCH_ID: engine-c4-plan-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — technical scope-lock corrections only; IMPL STOP remains in force
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-plan-4/PLAN-planner-20260827-134757.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 91f7e0333b87864abcec4d6d685a14dd4c2148ab0835ce9a5d2b792060e64a16
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: a97d69a4bfd6a39cd7d2df009148d2a5db11f42d8fc2a652eb01924876c30352
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: PLAN-REVIEW must-revise — complete Task 15 scope fence and rev-10 plan lock

## Verdict

Must revise `PL-engine-c4-20260826` revision 4 at SHA-256 `a97d69a4bfd6a39cd7d2df009148d2a5db11f42d8fc2a652eb01924876c30352` before implementation dispatch.

The Task 15 behavior and RED battery are directionally complete and may remain one continuation task/commit. Re-running Task 14 with its committed fixed seed and exact commands is sufficient provided every attempt is recorded honestly. Two plan-carrier defects block an all-in implementation dispatch: the Task 15 file fence omits required production and test-registration files, and the plan's global lock metadata still identifies DD revision 5 rather than the approved revision 10.

This verdict is review-only. It grants no implementation, source/test edit, branch movement, commit, PR, merge, publication, deployment, release, or external-use authority. The IMPL STOP remains in force.

## Required revisions

### M1 — Task 15 file fence is not all-in

Add both required modified paths to Task 15 and bind their responsibilities explicitly:

- `engine/src/adapter/policy.hpp`: integrate the new payoff carrier and alternation solver, preserve one sub-search per unique `(child, successor_h)`, propagate shared cancellation so an incomplete matrix stages the prior result, invoke the double-valued RM+ carrier, and perform the locked X-max/O-min seat-aware action extraction with exact-equality lower-bid/lower-move fallback.
- `engine/CMakeLists.txt`: register the newly created `engine/tests/test_alt_solver.cpp` in the explicitly enumerated `uttt_tests` source list.

These are not optional implementation details. The preserved Task 13 product places child search/cancellation, the per-child cache, production payoff binding, matrix construction, RM+ invocation, and published action selection in `policy.hpp`; Task 15 changes each of those seams. The test target is an explicit list, so merely creating `test_alt_solver.cpp` would not compile or run it. A successor dispatch cannot truthfully report `SCOPE_DIFF_RESULT: all-in` while either path is absent. — E1 — plan lines 190–206; preserved product `engine/src/adapter/policy.hpp:134–204`; `engine/CMakeLists.txt:38–48`.

### M2 — global plan lock metadata still names DD revision 5

Update the plan artifact's global Goal and Spec to approved `DD-engine-math-c4-20260826` revision 10 at SHA-256 `91f7e0333b87864abcec4d6d685a14dd4c2148ab0835ce9a5d2b792060e64a16`. Update Task 14's final PR-title text so the eventual carrier names DD revision 10 rather than revision 5. Preserve Tasks 1–13 as the landed historical baseline, and state that Task 15 is the current delta, without leaving the artifact's governing metadata contradictory. — E1 — plan lines 5, 11, 188, 190–208; approved design-review lineage.

## Finding disposition

- Design and plan relay lineage: pass. The incoming relay parents to the approved DD rev-10 review, and the current design and plan bytes match their carried SHA-256 digests.
- Task 15 math and acceptance semantics: pass subject to M1 integration. The branch-complete `t_est`, graded payoff, double matrix/RM+, dedicated alternation solver, memo collision safety, cancellation behavior, oracle parity, mutations, and seat-aware extraction checks cover the rev-10 delta.
- Task granularity: pass. One continuation task/commit is acceptable because its RED battery, implementation boundary, focused/full verification, mutation proof, and gauntlet rerun are separately ordered.
- Task 14 rerun: pass. The same committed fixed seed/configs are reproducible acceptance inputs; the plan already requires honest reporting and remediation of every red attempt.
- Scope boundary: must revise per M1. No theory, referee, rules-core, protocol, or fixture-contract byte change is authorized.
- Operator judgment: none.

## Successor gate

The Planner must revise the same plan lock, commit the corrected plan bytes, and return a fresh PLAN relay parented to this review with the new plan digest. A later implementation dispatch remains conditional on an approving PLAN-REVIEW, the standing `engine-c4-proceed-1` lineage, fresh collision checks, an exhaustive `SCOPE_DIFF` including `policy.hpp` and `CMakeLists.txt`, `SCOPE_DIFF_RESULT: all-in`, no hard trigger or boundary collision, and daemon admission. Until those conditions are met, implementation remains unauthorized.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: yes — the future engine policy publishes moves and bids consumed by the referee
- worker/scheduler/queue/retry/async-side-effect: yes — acceptance invokes engine subprocesses through the referee
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory and referee contracts
- user-visible-control-with-materializer/downstream-consumer: yes — policy output and analysis metadata have referee, fitter, and game-log consumers
- test-runtime-role-mismatch: yes — the new test source is omitted from the runtime test-target registration, and production policy integration is omitted from the file fence
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — M1 and M2 are exact mechanical plan corrections; no operator choice is required
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming relay exact-file lint is clean. The current design bytes hash to `91f7e0333b87864abcec4d6d685a14dd4c2148ab0835ce9a5d2b792060e64a16`; the current plan bytes hash to `a97d69a4bfd6a39cd7d2df009148d2a5db11f42d8fc2a652eb01924876c30352`. — E2 instrument evidence.
- Main is at `0c51e532789859277d13c7d18caffc1126f50bae`; the preserved implementation worktree is at `afb91d529258f1e38e27484007b9d5a090b796af`. The implementation worktree's six Task 14 tool paths remain uncommitted and untouched. — E2 instrument evidence.
- The production integration and test registration observations were made against the preserved implementation worktree; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.
- Daemon verification before filing reports the inherited historical INDEX divergence only; no pending drafts were present.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-engine-c4-plan-review-4.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
— both paths are concurrent foreign theory work and were preserved; daemon-rendered relay/INDEX transport changes occur only after submission
