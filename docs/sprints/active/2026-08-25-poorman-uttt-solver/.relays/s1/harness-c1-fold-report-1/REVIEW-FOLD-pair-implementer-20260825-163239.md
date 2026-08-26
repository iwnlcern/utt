ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: harness-c1-fold-report-1
PARENT_DISPATCH_ID: harness-c1-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — harness.planner can reconcile the fold; PR remote provisioning and merge remain separate operator/orchestrator gates
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-fold-1/REVIEW-FOLD-planner-20260825-160652.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: 15c722467ba91d7033ca3059d1cc036f6b3c0a14a974f3abf21cbd5d08e8fe05
BUNDLE_ID: harness-c1
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: harness-c1-impl@477f491c8f3c713f0981925adba39801f2591d02
TARGET_BRANCH: main
BRANCH: harness-c1-impl
PR: unavailable — this repository still has no configured git remote; no push was attempted
SUBJECT: REVIEW-FOLD REPORT — B1-B2 and M1-M5 folded at 8b6325c with E2/E3 evidence

## Result

Folded both blockers, all five must-fixes, and the F7 documentation ruling into `harness-c1-impl` at `8b6325c8146b2dad30f38a128b7c7c8c5d975bcb`. Optional findings were deferred. No merge, push, remote configuration, or edit outside the accepted harness scope was performed.

The initial pre-edit scope is `harness-c1-fold-1/REVIEW-FOLD-pair-implementer-20260825-161951.md`. Before either required `procs.py` mutation, the corrected all-IN successor `harness-c1-fold-1/REVIEW-FOLD-pair-implementer-20260825-162240.md` added that relay-named file; no out-of-scope edit occurred.

## Finding disposition

- B1 — E2 fixed: all-closed terminal adjudication now compares post-payment remaining budgets; strict leader wins `chip_count`, equality yields `draw`/`exact_tie_draw`. Two named real-referee E2E tests failed against the old local-board counter and pass at fold head. All affected generated logs were refreshed.
- B2 — E2 fixed: the stub now performs a separate, marker-coordinated late write only after the first reply has returned while the companion remains held open. Deleting `collect_both`'s final sweep produced the expected RED (`ok` instead of `extra_protocol_line`); restoring the sweep returned GREEN.
- M1 — E2 fixed: the shutdown test uses a 5-second engine exit delay with configured 300 ms grace and asserts 0.25-0.8 s completion. Hardcoding the 2000 ms default produced the expected RED at 2.005 s; restored code passed at approximately 0.35 s.
- M2 — E2 fixed: replay now requires attempt numbers to be contiguous from 1. Removing attempt 1 produced the expected RED when validation was absent; restored validation rejects `expected 1, got 2`.
- M3 — E2 fixed: the tie-owner E2E test now covers bids 3 and 0 across three plies. Mutating the owner update to retain the mover produced the expected zero-bid RED (`O,O,O` instead of `O,X,O`); restored logic is GREEN.
- M4 — E2 fixed: all nineteen conformance JSONL logs plus the normative transcript were regenerated from the current referee. Every terminal now carries X/O stderr metadata; schema validation, lossless replay, attempt contiguity, and terminal budget semantics pass across all twenty logs.
- M5 — E2 fixed: present `info` with missing or unsupported `quality` remains validation `ok`, is preserved, and adds `info_quality_missing_or_invalid` to the turn record's additive `warnings` array. Unit and real-referee E2E paths cover the behavior.
- F7 — E1 folded: the protocol now states that referee and engine must share major 1 and a mismatched hello reply forfeits as `hello_fault` before ply 0.

## Verification

Fresh post-commit E2:

- `uv run --project referee pytest referee/tests -q` -> `184 passed, 1 skipped in 5.58s`.
- The sole skip remains `development-only: theory/fixtures have not landed`; theory-backed acceptance remains pending/blocked and is not counted as complete.
- `uv lock --project referee --check` -> `Resolved 13 packages in 3ms`.
- `uv run --project referee python -m compileall -q referee/poorman_referee referee/bots` -> exit 0.
- Generated-artifact semantic probe -> `validated 20 regenerated logs`.
- `git diff --check 70377b7593628911cb86946de661ffc910ea78f0..HEAD` -> exit 0.

Fresh E3 tournament smoke:

- Command: `/usr/bin/time -p uv run --project referee python -m poorman_referee tourney --config /tmp/poorman-e3-fold-config.json` with four baseline bots, two games per pair, `time_ms=30000`, seed `e3-fold-20260825`.
- Result: `games_played=12`; every engine reported `faults=0` and `voids=0`; timing `real 0.54`, `user 0.60`, `sys 0.22` seconds.
- Canonical summary: `{"engines":{"allin":{"avg_budget_margin":{"denominator":3,"numerator":-20275564},"draws":0,"faults":0,"losses":4,"voids":0,"wins":2},"fraction":{"avg_budget_margin":{"denominator":1,"numerator":-649922812},"draws":0,"faults":0,"losses":0,"voids":0,"wins":6},"random":{"avg_budget_margin":{"denominator":3,"numerator":-1009680436},"draws":0,"faults":0,"losses":2,"voids":0,"wins":4},"zero":{"avg_budget_margin":{"denominator":3,"numerator":2979724436},"draws":0,"faults":0,"losses":6,"voids":0,"wins":0}},"games_per_pair":2,"games_played":12,"tournament_seed":"e3-fold-20260825"}`

FOLD_SCOPE:
- referee/poorman_referee/referee.py -> in
- referee/poorman_referee/gamelog.py -> in
- referee/poorman_referee/procs.py -> in
- referee/tests/stub_engine.py -> in
- referee/tests/test_referee_e2e.py -> in
- referee/tests/test_procs.py -> in
- referee/tests/test_gamelog.py -> in
- referee/tests/test_protocol_artifacts.py -> in
- docs/protocol/poorman-uttt-protocol-v1.md -> in
- docs/protocol/transcript-v1.jsonl -> in
- referee/tests/fixtures/03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.O.stderr -> in
- referee/tests/fixtures/03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.X.stderr -> in
- referee/tests/fixtures/fault-eof_or_crash.jsonl -> in
- referee/tests/fixtures/fault-extra_protocol_line.jsonl -> in
- referee/tests/fixtures/fault-illegal_bid.jsonl -> in
- referee/tests/fixtures/fault-illegal_move.jsonl -> in
- referee/tests/fixtures/fault-invalid_json.jsonl -> in
- referee/tests/fixtures/fault-invalid_utf8.jsonl -> in
- referee/tests/fixtures/fault-oversize_line.jsonl -> in
- referee/tests/fixtures/fault-schema_violation.jsonl -> in
- referee/tests/fixtures/fault-timeout.jsonl -> in
- referee/tests/fixtures/fault-wrong_request_id.jsonl -> in
- referee/tests/fixtures/parity-even/game-0001-r0001-g1.jsonl -> in
- referee/tests/fixtures/parity-even/game-0001-r0001-g2.jsonl -> in
- referee/tests/fixtures/parity-odd/game-0001-r0001-g1.jsonl -> in
- referee/tests/fixtures/parity-odd/game-0001-r0001-g2.jsonl -> in
- referee/tests/fixtures/recovery-both-fail.jsonl -> in
- referee/tests/fixtures/recovery-double-one-fail.jsonl -> in
- referee/tests/fixtures/recovery-single-fail.jsonl -> in
- referee/tests/fixtures/success.jsonl -> in
- referee/tests/fixtures/terminal-fault-won-no-recovery.jsonl -> in
FOLD_SCOPE_RESULT: all-in

Boundary contract: harness-owned protocol/runtime/tests/artifacts only. Theory fixtures remain foreign and absent; engine, UI, network, ratings, merge, push, and remote configuration remain untouched.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no credential or permission surface
- migration/backfill/destructive-write/canonical-data-repair: no — generated local conformance artifacts only
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game budgets only
- AI-or-automation-acts-downstream: yes — referee supervises bot subprocesses and adjudicates outcomes
- worker/scheduler/queue/retry/async-side-effect: yes — subprocess timing, sweep, and shutdown behavior are directly tested
- cross-repo/service-contract/generated-schema/shared-API-event: yes — protocol artifacts are cross-pair contracts within the existing lock
- user-visible-control-with-materializer/downstream-consumer: no — no UI or deployed consumer
- test-runtime-role-mismatch: no — real subprocess behavior and generated logs are exercised
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — theory fixtures and PR publication remain explicitly unavailable
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: harness-c1-impl@8b6325c8146b2dad30f38a128b7c7c8c5d975bcb; commit 8b6325c; 28 files changed, 215 insertions, 83 deletions; no push, PR, merge, or remote change claimed
FINAL_GIT_STATUS_SHORT: none — clean tree

Next requested action: `harness.planner` verify and accept the fold. PR creation still requires an operator-provided repository remote; merge requires a separate addressed grant. Re-run the sanctioned theory-fixture test when `theory/fixtures/` lands.
