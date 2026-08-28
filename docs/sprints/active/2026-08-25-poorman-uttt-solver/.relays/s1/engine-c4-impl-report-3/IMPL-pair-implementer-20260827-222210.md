ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: engine-c4-impl-report-3
PARENT_DISPATCH_ID: engine-c4-impl-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — engine.planner can reconcile this report; merge remains a separate orchestrator/operator gate
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-impl-3/IMPL-planner-20260827-213811.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: f229caf42f1bfb8b64dbe9f8a3fd8e1e4b467a74b8a5194c1926d1492e239acf
BUNDLE_ID: engine-c4
OWNER: engine pair
REPO: utt (origin https://github.com/iwnlcern/utt)
BASE: origin/main@e3798aa22460600977e4626e0b6ac56b8e544d69
TARGET_BRANCH: main
BRANCH: engine/search-math-c4
PR: https://github.com/iwnlcern/utt/pull/10 — OPEN, non-draft, MERGEABLE at exact head 5b147b80ff39587c65dada6851a250dd6c93ba34; statusCheckRollup empty
SUBJECT: IMPLEMENTATION REPORT — engine-c4 DD rev 13 Task 16 and Task 14 rerun complete at 5b147b8; PR #10 refreshed, merge separately gated

## Summary

PL-engine-c4 rev 7 Task 16 is implemented in one commit, `5b147b80ff39587c65dada6851a250dd6c93ba34`, on the preserved `engine/search-math-c4` branch, pushed to origin, and open as refreshed PR #10 against main. The successor work implements staged publication, maximum-probability-first extraction, fair ply-0 coin expectations, fail-closed certified forced actions, exact endpoint ceilings, crossed-r enclosure, outward play windows, sound nonnested refinement, a bounded cardinality-preserving analyze ABI, fitter hard-fail handling, and policy-seam zero-total/TT diagnostics. The fixed-seed Task 14 A10/A11 gauntlet was rerun after the final production bytes.

No theory, referee, harness, rules-core, protocol, fixture, design, plan, or unrelated source byte was edited. No merge, deployment, publication, or release action was performed.

## Acceptance truth table

```text
A1 backup mathematics                 -> green-E2 (fresh final unit suite)
A2 solved-scale oracle equality       -> green-E2 (fresh final unit suite)
A3 interval soundness                 -> green-E2 (fresh final unit suite)
A4 cutoff correctness                 -> green-E2 (full-window, bounded-window, production-cut, play-window verdict, and refinement batteries)
A5 directed-rounding enclosure        -> green-E2 (fresh final suite plus 200-case crossed-r containment battery)
A6 TT neutrality/collision honesty    -> green-E2 (policy-owned before/after TT snapshot plus final unit suite)
A7 exact P2 gate                      -> green-E2 (exact binary64 ceiling and conversion-cliff battery)
A8 root-matrix parity/RM+             -> green-E2 (focused matrix battery plus final unit suite)
A9 metadata/graded/coin/alternation   -> green-E2 (22,186-state alternation parity, ordered analyze/fitter ABI, and named mutation runs)
A10 four-baseline tournament          -> green-E3 (random 50/50; zero 50/50; fraction 50/50; allin_tactical 50/50; faults/voids 0)
A11 clock discipline/staging          -> green-E2+E3 (e2e green; two 30 s games; 83 engine attempts checked; faults/voids 0)
```

The full-clock match score was 0/2 against `fraction`; A11 has no strength bar. Its locked acceptance gate is per-attempt timing, validation, non-void completion, and completed-staging metadata, all of which passed. A10 separately cleared the strength bar at 100% against every baseline.

## Task 16 RED/GREEN and mutation evidence

- RED-first compilation failed because the tests referenced absent `ceil_exact`, `make_play_window`, and `critical_r_enclosure` symbols, proving the Task 16 battery preceded implementation. — E2.
- Focused final amendment battery passed 12/12 cases and 458/458 assertions; the later fair-coin/extraction subset passed 3/3 and 112/112. The fitter self-test passed. — E2.
- Midpoint certificate mutation: the midpoint witness and straddle rows turned RED; restored rows GREEN. — E2.
- Endpoint-ceiling equality removal: certificate row turned RED; restored row GREEN. — E2.
- Dominance removal: overlap row turned RED; restored row GREEN. — E2.
- Positive certified branch disable: positive stable/dominant rows turned RED; restored rows GREEN. — E2.
- Affordability clamp mutation: affordability row turned RED; restored row GREEN. — E2.
- Exact-ceiling floor mutation: exact binary64 ceiling row turned RED, including `1e-9*M` returning 1 rather than 2; restored row GREEN. — E2.
- Directed-r rounding flip: the fixed-seed 200-case enclosure battery turned RED at case 125; restored row GREEN. — E2.
- Probability-filter removal: extraction row turned RED when X selected a near-zero-probability payoff action; restored row GREEN. — E2.
- Fair-coin replacement by the X branch: all 16 expected-value rows turned RED; restored rows GREEN. — E2.
- Integer-quantization mutation: exact binary64 half-boundary row turned RED; restored row GREEN. — E2.
- Play-window outward-ulp removal: byte-exact rows turned RED; restored rows GREEN. — E2.
- Old nonnested-refinement rejection: sound-intersection row turned RED; restored row GREEN. — E2.
- Analyze depth-limit widening: ordered `valid, depth-13, valid` stream row turned RED because depth 13 succeeded; restored row GREEN with one error object and continued processing. — E2.
- Fitter hard-fail removal: self-test turned RED with `analyze rejection self-test failed`; restored self-test GREEN. — E2.
- Staged-move replacement by first legal: tactical zero-RM witness turned RED; restored row GREEN. — E2.
- Threshold-search insertion into zero-total alternation: policy-owned TT snapshot turned RED through changed hits/stores; restored row GREEN. — E2.
- Coin cancellation and range witnesses additionally turned RED before the stop-between-branches and per-branch finite/range validation fixes; restored rows GREEN. — E2.
- Fresh final `cmake --build build --parallel` succeeded. Fresh final `ctest --test-dir build --output-on-failure`: unit passed in 33.22 s, e2e passed in 22.95 s, 2/2 and zero failures, 56.18 s total. — E2.

## Real-referee E3 evidence

- One post-implementation fixed-seed A10 run at 50 paired games per matchup: `random` 50/50, `zero` 50/50, `fraction` 50/50, and `allin_tactical` 50/50; each had zero faults and zero voids. — E3.
- One post-implementation A11 `--fullclock` run: two seat-swapped 30 s games, zero faults and zero voids. The validator checked every engine attempt and accepted all 83 elapsed-time and completed-staging records. — E3.
- Local worktree head and remote PR head both resolve to exact `5b147b80ff39587c65dada6851a250dd6c93ba34`; PR #10 is OPEN, non-draft, and MERGEABLE against main. — E1.

## Scope and boundary proof

Exact Task 16 SCOPE_DIFF vs dispatched base `5907f937f07fd0309e141e3db706064780822d88`:
- engine/src/adapter/main.cpp -> in
- engine/src/adapter/policy.hpp -> in
- engine/src/root/bid_matrix.hpp -> in
- engine/src/root/p2_gate.hpp -> in
- engine/src/search/search.hpp -> in
- engine/tests/test_alt_solver.cpp -> in
- engine/tests/test_cutoffs.cpp -> in
- engine/tests/test_eval_quality.cpp -> in
- engine/tests/test_p2_gate.cpp -> in
- engine/tests/test_root_matrix.cpp -> in
- engine/tools/fit_eval.py -> in
SCOPE_DIFF_RESULT: all-in

`engine/CMakeLists.txt` was permitted only if a new test file was added; no new test file was required, so it has no Task 16 byte change. The commit contains exactly the eleven listed paths and no unauthorized path.

Writes: engine-owned policy, search, root math, protocol analysis behavior, fitter checks, and direct regression tests only.
Reads: locked design/plan semantics, existing evaluator/GameModel/TT interfaces, rules-core Position behavior, and the referee CLI/bot/log schemas.
Target entity: the c4 engine runtime and engine-owned acceptance surface.
Downstream consumers: protocol adapter, root policy, RM+ action publication, engine tests, fitter, and the read-only real referee.
Contract proof: incomplete searches never publish matrix payoffs; a forced action publishes only through the complete six-clause certificate; exact ceilings are derived from the binary64 endpoint; tie-at-root uses the fair expectation of independently valid X/O continuations; extraction filters maximum averaged probability before seat-aware payoff comparison; analyze preserves one response per readable input line and continues after bounded rejection.
No-consumer action: theory/referee/harness/rules/protocol/fixture bytes were read only and no producer fixture or external contract was rewritten.

## Next action and risk

Requested next action: engine.planner reconcile the refreshed A1–A11 ledger, review/fold this Task 16 report, and route exact PR #10 head `5b147b8` to the governing seats for a separately addressed merge gate. Preserve the branch/worktree for review. PR mergeability or green local evidence does not imply merge authority.

Residual risk is ordinary review/merge risk only: the bounded anchor sweep can legitimately leave matrix work for the remaining clock, fail-closed certification deliberately falls back to the matrix, and A11 validates clock discipline rather than match strength. These are locked/disclosed behaviors, not hidden acceptance gaps.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game engine only
- migration/backfill/destructive-write/canonical-data-repair: no
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — no external acting system or side effect
- worker/scheduler/queue/retry/async-side-effect: no — local tournament subprocesses only
- cross-repo/service-contract/generated-schema/shared-API-event: no — referee consumed read-only through its existing protocol
- user-visible-control-with-materializer/downstream-consumer: no
- test-runtime-role-mismatch: no — E3 used the real referee and production engine binary
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — DD rev 13 and the exact Task 16 fence were implemented as dispatched
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: engine/search-math-c4@5b147b80ff39587c65dada6851a250dd6c93ba34; Task 16 commit 5b147b80ff39587c65dada6851a250dd6c93ba34; branch pushed to origin and PR https://github.com/iwnlcern/utt/pull/10 refreshed at exact head; no merge, deployment, publication, or release claimed
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree
