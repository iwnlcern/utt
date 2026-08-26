ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: engine-c1-fold-report-1
PARENT_DISPATCH_ID: engine-c1-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — bounded fold on the existing implementation branch; merge remains separately gated
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1-fold-1/REVIEW-FOLD-planner-20260825-162547.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 1d13153f2af22d1f2f55023292eef49827d78d8e0230f93dbdec2e66e48c8f62
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
PLAN_SHA256: 33803c7941f921e5d50983e510dbe02f507461f721af296ef9e19346328fcbf9
BRANCH: engine/rules-core-c1
SUBJECT: REVIEW-FOLD REPORT — all six must-fixes folded at 040c4f2 with E2 evidence

## Result

Folded all six must-fixes plus the selected bounded optionals into `engine/rules-core-c1` at `040c4f2bb50a41937fdece4fe4fd9c0f74116081`. The fold is exactly the twelve files declared in this daemon-owned draft before the first branch mutation. No design change, scope expansion, merge, push, PR, or remote operation occurred.

Acceptance criteria 1 and 3 remain honestly pending-blocked on the absent theory fixtures and harness conformance corpus. The unoverridden suite therefore remains intentionally red only at those two owner-artifact gates; the owner-independent product suite is green.

## Finding disposition

- MF1 — E2 fixed at `4000ef9`: the adapter now reads at most 32 KiB into memory, identifies oversize input during the read, drains through the next newline, emits the existing diagnostic, and continues. Per-message `std::exception` and catch-all backstops also continue without protocol output. The new e2e sends a 64 MiB newline-free prefix in 1 MiB chunks, asserts RSS stays below 32 MiB before termination, then proves silence and liveness; a 512-level nested JSON line likewise proves silence and liveness. The pre-fix RSS probe was RED with `oversize partial line was buffered in memory`; the folded probe is GREEN.
- MF2 — E2 fixed across source `eb731fe` -> baseline `1aaba19` -> verdict `040c4f2`: CMake now drives all targets, including `uttt_core`, with a final `-O3`; the duplicate explicit standard flag is removed; benchmark identity is derived from the configured RelWithDebInfo flags and the centralized option list. Generated commands confirm both core and benchmark objects end in `-O3`. The committed same-binary evidence is PASS at reference 883.24801025 ns/ply, candidate 50.47830225 ns/ply, ratio 0.0571507681, absolute ceiling true, with identical digest/session identity. The verdict now records plies/warmups/measured runs, and candidate mode rejects unused `--out`.
- MF3 — E2 fixed at `b8f0e3d`: the naive local oracle now constructs reachable states with a genuinely independent breadth-first traversal over alternating mark placements, stopping expansion at wins/full boards instead of duplicating the production table derivation. Focused naive plus exhaustive proof passed 2 cases and 137,800 assertions; the final full product run also passed.
- MF4 — E2 fixed at `4000ef9`: the malformed oversize case asserts exactly `oversize line`, a well-formed oversized JSON object exercises the same pre-parse rejection, and the exact 32 KiB boundary remains accepted. Focused wire coverage passed 6 cases.
- MF5 — E2 fixed at `6b7312f`: randomized incremental-versus-recomputed Zobrist coverage is restored from 500 to 2,000 games. The focused test passed 115,345 assertions.
- MF6 — E2 fixed at `bbd46a8`: fixture consumption now requires a recognized engine expectation; a `move` alone or an orphaned move expectation is not sufficient. A regression test pins recognition behavior, and `expected_result: draw` now requires `TerminalKind::AllClosed`. Focused fixture/import coverage passed 3 cases and 12 assertions.
- Selected invariants — folded at `cceb2cd`: `MoveList::push` asserts its 81-entry bound and `PlaceholderPolicy::choose` asserts a nonempty legal list. `types basics` passed 5 assertions and integrated e2e is green under the owner-artifact override.

## Verification

Fresh post-fold evidence from a clean RelWithDebInfo rebuild:

- Configure plus `cmake --build engine/build --clean-first -j 8` -> 42/42 build steps, exit 0 under Homebrew Clang 22.1.8, C++26, arm64.
- Literal unoverridden `ctest --test-dir engine/build --output-on-failure` -> exactly two expected failures: unit ran 25 cases and 3,134,526 assertions with only `theory schema-v1 UTTT fixtures` failing because `theory/fixtures` is absent; e2e completed basic/RSS/deep-nesting checks and failed only because the harness corpus is absent.
- `engine/build/uttt_tests --test-case-exclude='theory schema-v1 UTTT fixtures'` -> 24/24 cases, 3,134,525/3,134,525 assertions, GREEN.
- `UTTT_ALLOW_MISSING_CORPUS=1 python3 engine/tests/test_engine_e2e.py engine/build/uttt_engine .` -> exit 0 after all built-in protocol, framing, 64 MiB RSS/liveness, and deep-nesting checks; emitted only the truthful criterion-3 pending notice.
- A clean relink changed the Mach-O executable digest and the committed baseline correctly refused reuse. A fresh temporary same-binary reference/candidate pair then passed at reference 885.33235425 ns/ply, candidate 49.56659375 ns/ply, ratio 0.0559864253, identity equality true, with derived flags `-O2 -g -DNDEBUG -std=c++26 -Wall -Wextra -Werror -O3`. Temporary artifacts were removed; committed evidence topology was not mutated.
- Committed baseline/verdict identity equality, PASS predicates, 2,000,000 plies/run, 2 warmups, and 10 measured runs were mechanically checked with `jq`.
- Parent and exact-set checks prove `1aaba19^ == eb731fe`, `040c4f2^ == 1aaba19`, source commit files are exactly CMake plus benchmark source, baseline commit is exactly `baseline.json`, and verdict commit is exactly `candidate-verdict.json`.
- `git diff --check b4127cfb78e07eea179ab2562e9cf4c63f863f1b..HEAD` -> exit 0. Aggregate changed-path equality against the pre-edit list below -> exact match. Worktree status -> clean.

Fold commits, in order:

1. `4000ef9` — bound JSONL reads and harden framing regressions
2. `b8f0e3d` — make naive reachability oracle BFS-independent
3. `6b7312f` — restore 2,000-game Zobrist recompute coverage
4. `bbd46a8` — enforce fixture expectation consumption
5. `cceb2cd` — assert bounded core preconditions
6. `eb731fe` — make benchmark build identity truthful
7. `1aaba19` — refresh truthful benchmark baseline
8. `040c4f2` — record optimized candidate benchmark PASS

FOLD_SCOPE:
- engine/CMakeLists.txt -> in
- engine/bench/bench_playout.cpp -> in
- engine/bench/baseline.json -> in
- engine/bench/candidate-verdict.json -> in
- engine/src/adapter/main.cpp -> in
- engine/src/adapter/policy.hpp -> in
- engine/src/core/naive_local.hpp -> in
- engine/src/core/types.hpp -> in
- engine/tests/test_engine_e2e.py -> in
- engine/tests/test_wire.cpp -> in
- engine/tests/test_zobrist.cpp -> in
- engine/tests/test_fixtures.cpp -> in
FOLD_SCOPE_RESULT: all-in

Scope rationale: MF1 owns adapter framing and its e2e proof; MF2 owns CMake, benchmark source, baseline, and verdict; MF3 owns the naive oracle; MF4 owns wire framing coverage; MF5 owns Zobrist stress coverage; MF6 owns fixture consumption. The accepted bound assertions touch only policy/types. No other path was edited.

Boundary contract: engine-owned build, rules core, adapter, tests, and benchmark evidence only. Theory schema bytes, harness corpus/discovery, ply-0 ownership questions, search, other bundles, merge, push, and release remain untouched.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no credential or permission surface
- migration/backfill/destructive-write/canonical-data-repair: no — benchmark JSON was regenerated by the locked evidence workflow only
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-state and timing evidence only
- AI-or-automation-acts-downstream: yes — the engine emits moves consumed by the referee, within the existing locked protocol
- worker/scheduler/queue/retry/async-side-effect: no — single-process stdin/stdout adapter only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — adapter consumes the existing harness protocol and future theory fixtures; neither contract was changed
- user-visible-control-with-materializer/downstream-consumer: no — no UI or deployed consumer
- test-runtime-role-mismatch: no — real engine subprocess behavior and the optimized binary were exercised
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — criteria 1 and 3 remain pending on absent owner artifacts and are not claimed green
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: engine/rules-core-c1@040c4f2bb50a41937fdece4fe4fd9c0f74116081; eight fold commits; 12 files changed, 234 insertions, 86 deletions; no push, PR, merge, remote, or release action claimed
FINAL_GIT_STATUS_SHORT: none — clean tree

Next requested action: `engine.planner` run the targeted fold quick-check and, if accepted, issue its merge-gate recommendation. Re-run the literal unoverridden suite when theory fixtures and the harness conformance corpus land.
