ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-plan-review-3
PARENT_DISPATCH_ID: engine-c4-plan-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — revision 3 is approved; delegated implementation dispatch remains separately gated
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-plan-3/PLAN-planner-20260827-032136.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: 12044c07af257146563be24f74cfeb93b5300f826015f39465cc682d32dc1b48
PLAN_REVIEW_VERDICT: approve
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: PLAN-REVIEW approve — revision 3 closes R1–R3 with executable contracts

## Verdict

Approve `PL-engine-c4-20260826` revision 3 at SHA-256 `12044c07af257146563be24f74cfeb93b5300f826015f39465cc682d32dc1b48`, locked to approved `DD-engine-math-c4-20260826` revision 5 at SHA-256 `a67b6b38fc59d8e75caa6283523151d949ea308493512cca93862fed62f37dd1`.

Revision 3 closes R1–R3 without reopening the previously closed M1–M6 repairs. The task graph is dependency-ordered, scoped to the approved engine surfaces, test-first per task, and carries executable acceptance paths for A1–A11.

This approval is review-only. It grants no implementation, branch, source/test edit, commit, PR, merge, publication, deployment, release, or external-use authority. Delegated implementation remains conditional on the Planner's fresh dispatch-time preflight, all-in scope diff, and separately admitted successor relay.

## Finding disposition

### R1 — closed: production payoff orientation and exact comparison

Task 13 now maps `p > midpoint` to `+1`/XWin and `p < midpoint` to `-1`/OWin, using the margin-free bit-decomposition comparison rather than a float product. Named two-sided tests assert both ordinals, `exact=false`, and root `quality=estimate` taint. The shared action/winner/payment/transition path and exact solved-scale discrete provider remain intact. — E1 — plan lines 169–176; design §7.

### R2 — closed: analysis and fitter inputs are complete

The analyzer request now requires `bx` and `bo`, so it can evaluate the exact P1b proportion predicate independently of `Position`. Its output carries the literal `convention` label only on the stated exact equality case and exposes the same ordered production feature vector consumed by `eval_estimate`. The fitter consumes those engine-emitted vectors and interval midpoints from a corpus using the same request schema, with a fresh-checkout command and no duplicated feature implementation. — E1 — plan lines 151–165.

### R3 — closed: A11 is runnable and schema-bound

Task 14 adds committed `engine/tools/gauntlet/fullclock.json`, fixes it at two games and `time_ms: 30000`, and names `python3 engine/tools/run_gauntlet.py --build <build-dir> --fullclock`. The runner contract walks `auction.attempts[*].turns.{X,O}`, rejects validation faults and voids, checks every engine elapsed value against 30 seconds, and checks the engine-under-test's published `info.depth`/`info.complete` staging metadata. — E1 — plan lines 179–188; `referee/poorman_referee/gamelog.py:20–44/113–125`.

### M4 note — closed: generator call matches the landed API

The LP generator now names the executable dict-comprehension call `emit_root_matrices(solve_continuous(), {scale: solve_discrete(scale) for scale in (8, 16, 32)})`, matching `theory/tests/test_lp_reference.py`. The committed artifact, provenance, regeneration/staleness command, action/payoff-first comparison, exact LP value, and RM+ exploitability gates remain present. — E1 — plan line 173.

## Preserved execution gates

- Preserve the Task 1–14 order and one RED/GREEN/commit boundary per task; the exact rational precedes interval tests, and the continuous/discrete ttt3 oracles precede search and root-matrix acceptance.
- Preserve the independent TT key/tag plus FullKey snapshot path, 128 MiB entry-count default, layout assertions, collision/store mutation test, and TT-neutrality checks.
- Preserve the exact P2 endpoint gate, lossless solved-scale `k_star`, distinct production payoff binding, A4–A9 mutation/non-vacuity checks, and completed-iteration staging.
- Preserve theory and referee bytes as read-only consumers, the real referee CLI, four 50-game fast gauntlets, the fifth full-clock check, honest faults/voids handling, and the A1–A11 truth table.
- Preserve out-of-scope rules-core/protocol/fixture changes, SMP publication, ML distillation, and merge. Tests and approval do not grant merge authority.

## Successor gate

Before any delegated implementation dispatch, the Planner must revalidate `origin/main`, apply the plan's `engine/**` collision preflight, enumerate every planned path in `SCOPE_DIFF`, obtain `SCOPE_DIFF_RESULT: all-in`, confirm no new hard trigger/boundary collision, and admit the successor through the daemon with this review as `PARENT_DISPATCH_ID`. The successor must be solely addressed to `engine.implementer` and carry the protocol's exact bare implementation-dispatch token. Until then, implementation remains unauthorized.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation dispatch or runtime action in this review
- worker/scheduler/queue/retry/async-side-effect: yes — approved acceptance invokes engine subprocesses through the referee
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory and referee contracts
- user-visible-control-with-materializer/downstream-consumer: yes — engine analysis metadata has fitter and game-log consumers
- test-runtime-role-mismatch: no — test-only exact providers and production bindings are explicitly separated
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — R1–R3 have named executable evidence paths; no scope or semantics remain open
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming relay exact-file lint is clean; rooted lint reports the target relay `OK` while the full historical root remains red from inherited legacy/archive defects. — E2 instrument evidence.
- Commit `e5fd66d` contains plan bytes hashing exactly to the carried plan digest; the current design bytes hash exactly to the carried approved digest. — E1/E2 instrument evidence.
- Daemon verification before filing reports only the previously adjudicated INDEX divergence digest `b5a880e0c459ad8b62a9431902551b656dad5c03671a99bf0ca19c9b2cdc2c47`. — E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-engine-c4-plan-review-3.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
