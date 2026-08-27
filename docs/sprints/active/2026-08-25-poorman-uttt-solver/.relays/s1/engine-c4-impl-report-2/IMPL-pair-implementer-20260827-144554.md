ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: engine-c4-impl-report-2
PARENT_DISPATCH_ID: engine-c4-impl-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — engine.planner can reconcile this report; merge remains a separate orchestrator/operator gate
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-impl-2/IMPL-planner-20260827-140749.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 91f7e0333b87864abcec4d6d685a14dd4c2148ab0835ce9a5d2b792060e64a16
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: b8ec7e185002adc8b967b25e59b86a46a12bdeff59d92674328ac38d78fc5fed
BUNDLE_ID: engine-c4
OWNER: engine pair
REPO: utt (origin https://github.com/iwnlcern/utt)
BASE: origin/main@e3798aa22460600977e4626e0b6ac56b8e544d69
TARGET_BRANCH: main
BRANCH: engine/search-math-c4
PR: https://github.com/iwnlcern/utt/pull/10 — OPEN, non-draft, MERGEABLE/CLEAN at exact head 5907f937f07fd0309e141e3db706064780822d88
SUBJECT: IMPLEMENTATION REPORT — engine-c4 rev-10 amendment and A1–A11 complete at 5907f93; PR #10 open, merge separately gated

## Summary

PL-engine-c4 rev 5 is implemented on the preserved `engine/search-math-c4` branch at `5907f937f07fd0309e141e3db706064780822d88`, pushed to origin, and open as PR #10 against main. The successor work adds branch-complete `t_est`, the κ=8 graded in-band payoff, finite-double RM+, a dedicated collision-safe zero-total alternation solver, cancellation-safe matrix integration, and X-max/O-min seat-aware extraction. The six preserved Task 14 runner/config paths are committed after one clean fixed-seed A10/A11 rerun.

Task 15 is commit `0a124cf0f0d7b18ea865c7714f7e5c5659349b7c`; Task 14 is commit `5907f937f07fd0309e141e3db706064780822d88`. No theory, referee, harness, rules-core, fixture, protocol, design, plan, or unrelated source byte was edited. No merge, deployment, publication, or release action was performed.

## Acceptance truth table

```text
A1 backup mathematics                 -> green-E2 (fresh final unit suite)
A2 solved-scale oracle equality       -> green-E2 (fresh final unit suite)
A3 interval soundness                 -> green-E2 (fresh final unit suite)
A4 cutoff correctness                 -> green-E2 (fresh final unit suite)
A5 directed-rounding enclosure        -> green-E2 (fresh final unit suite)
A6 TT neutrality/collision honesty    -> green-E2 (fresh final unit suite; alternation solver leaves threshold TT unchanged)
A7 exact P2 gate                      -> green-E2 (fresh final unit suite)
A8 root-matrix parity/RM+             -> green-E2 (focused matrix battery + fresh final unit suite)
A9 metadata/graded/alternation        -> green-E2 (22,186-state exact oracle parity; focused 16/16 and 6,992/6,992; named mutations RED→GREEN)
A10 four-baseline tournament          -> green-E3 (random 50/50; zero 50/50; fraction 50/50; allin_tactical 50/50; faults/voids 0)
A11 clock discipline/staging          -> green-E2+E3 (e2e green; two 30 s games; 86 attempts checked; max 46 ms; depths 3–4; all complete; faults/voids 0)
```

The full-clock match score was 0/2 against `fraction`; A11 has no strength bar. Its locked acceptance gate is per-attempt timing, validation, non-void completion, and completed-iteration metadata, all of which passed. A10 separately cleared the ≥90% strength bar at 100% against every baseline.

## Task 15 RED/GREEN and mutation evidence

- RED-first build failed at the new `root/alt_solver.hpp` include before that product header existed, proving the amendment tests preceded implementation. — E2.
- Focused final battery: 16/16 cases and 6,992/6,992 assertions; the dedicated alternation set separately passed 7/7 and 66,682/66,682. The primary exhaustive row itself checked all 22,186 materialized `(board,h)` states with 66,559/66,559 assertions. — E2.
- Freeze-`h` mutation: exhaustive oracle row RED with 6,632 failed assertions; restored row GREEN 66,559/66,559. — E2.
- Swap X-max/O-min polarity mutation: mover/polarity row RED with 2 failed assertions; restored row GREEN 4/4. — E2.
- Admit the non-mover child set mutation: exhaustive oracle row RED with 7,420 failed assertions; restored row GREEN 66,559/66,559. — E2.
- Substitute threshold backup mutation: exhaustive oracle row RED with 22,064 failed assertions; restored row GREEN 66,559/66,559. — E2.
- Integer-quantize the graded payoff mutation: magnitude row RED at both `+0.08` and `-0.08`; restored row GREEN 11/11. — E2.
- Remove memo semantic-equality guard mutation: collision row RED with 3 failures; restored row GREEN 6/6. — E2.
- Remove remaining-depth identity mutation: depth witness RED with 1 failure; restored row GREEN 6/6. — E2.
- Branch-complete cutoff witness: before the guide-plumbing fix, the estimate-tainted cutoff returned `t_est = 1/3` instead of locked `0.4`; after the fix the named row passed 4/4. — E2.
- Fresh final `ctest --test-dir engine/build --output-on-failure`: unit passed in 31.71 s, e2e passed in 23.54 s, 2/2 and zero failures, 55.26 s total. `git diff --check` was clean before both commits. — E2.

## Real-referee E3 evidence

- `referee/.venv` was materialized with `uv sync --frozen`; the lockfile stayed unchanged. The committed runner invoked the real `python -m poorman_referee tourney --config <cfg>` entrypoint from `referee/`. — E2/E3 carrier proof.
- A10 one clean run at `time_ms=2000`, 50 paired games per fixed-seed matchup: `random` 50/50, `zero` 50/50, `fraction` 50/50, `allin_tactical` 50/50; each summary recorded 50 wins, 0 draws, 0 losses, 0 faults, and 0 voids for c4-engine. — E3.
- A11 one clean `--fullclock` run: two seat-swapped games at `time_ms=30000`, 0 faults, 0 voids. The runner walked every `auction.attempts[*].turns.{X,O}` record and validated 86 engine attempts. Independent readback found max elapsed 46 ms, depth range 3–4, and all `info.complete == true`. — E3.
- Local and remote branch refs both resolve to exact head `5907f937f07fd0309e141e3db706064780822d88`; PR #10 is OPEN, non-draft, MERGEABLE/CLEAN against main at that head. — E1.

## Scope and boundary proof

Successor SCOPE_DIFF vs preserved `afb91d529258f1e38e27484007b9d5a090b796af`:
- engine/CMakeLists.txt -> in
- engine/src/adapter/policy.hpp -> in
- engine/src/root/alt_solver.hpp -> in
- engine/src/root/bid_matrix.hpp -> in
- engine/src/root/rmplus.hpp -> in
- engine/src/search/search.hpp -> in
- engine/tests/test_alt_solver.cpp -> in
- engine/tests/test_eval_quality.cpp -> in
- engine/tests/test_root_matrix.cpp -> in
- engine/tools/gauntlet/random.json -> in
- engine/tools/gauntlet/zero.json -> in
- engine/tools/gauntlet/fraction.json -> in
- engine/tools/gauntlet/allin_tactical.json -> in
- engine/tools/gauntlet/fullclock.json -> in
- engine/tools/run_gauntlet.py -> in
SCOPE_DIFF_RESULT: all-in

The two other fenced source paths, `engine/src/search/search.cpp` and `engine/src/root/bid_matrix.cpp`, required no byte change because their existing explicit/template integration compiled and linked the header-carried implementation. No unauthorized path was touched.

Writes: engine-owned search/root policy behavior, its direct regression tests, CMake test registration, and local acceptance tooling only.
Reads: existing evaluator, GameModel/TT interfaces, rules-core Position behavior, and the referee CLI/bot/log schemas.
Target entity: the c4 engine runtime and engine-owned acceptance surface.
Downstream consumers: protocol adapter, root policy, RM+ action publication, engine tests, and the read-only real referee.
Contract proof: exact solved-scale A8 ordinals remain losslessly represented as doubles; P2 remains the exact classification authority; zero-total nonterminal coordinates use only the dedicated alternation recurrence; incomplete sub-searches never feed RM+; protocol replies remain e2e conformant.
No-consumer action: theory/referee/harness bytes were read only and no producer fixture or external contract was rewritten.

## Next action and risk

Requested next action: engine.planner reconcile A1–A11 as green, review/fold the implementation report, and route exact PR #10 head `5907f93` to the governing seats for a separately addressed merge gate. Preserve the branch/worktree for review. PR approval or green checks do not imply merge authority.

Residual risk is ordinary review/merge risk only: the alternation memo is deliberately private per matrix construction, horizon-limited values remain estimate-tainted, and A11 validates clock discipline rather than match strength. These are locked/disclosed behaviors, not hidden acceptance gaps.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game engine only
- migration/backfill/destructive-write/canonical-data-repair: no
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — no external acting system or side effect
- worker/scheduler/queue/retry/async-side-effect: no — local tournament subprocesses only
- cross-repo/service-contract/generated-schema/shared-API-event: no — referee consumed read-only through its existing protocol
- user-visible-control-with-materializer/downstream-consumer: no
- test-runtime-role-mismatch: no — E3 used the real referee and production engine binary
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — G5/G6 and the exact fence were implemented as dispatched
ESCALATION_SCAN_RESULT: all-no
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: engine/search-math-c4@5907f937f07fd0309e141e3db706064780822d88; Task 15 commit 0a124cf0f0d7b18ea865c7714f7e5c5659349b7c; Task 14 commit 5907f937f07fd0309e141e3db706064780822d88; branch pushed to origin and PR https://github.com/iwnlcern/utt/pull/10 opened at exact head; no merge, deployment, publication, or release claimed
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree
