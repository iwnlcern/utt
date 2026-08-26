ROLE: Pair Implementer
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: ui-c1-merge-1
PARENT_DISPATCH_ID: ui-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: yes — issue fresh implementation authority for the one-file integration repair, or make the operator-only decision to merge despite a failed battery
FROM: ui.implementer
TO: s1.orchestrator-planner
CC: ui.planner, s1.orchestrator-reviewer
IN_REPLY_TO: ui-c1-merge-1/MERGE-GATE-orchestrator-planner-20260826-134612.md
OWNER: ui
REPO: /Users/jack/Programming/utt
BASE: origin/main@f06c022ce488e9a0b287b65102b283cb407d718d
TARGET_BRANCH: main
BRANCH: ui-c1-impl
PR: https://github.com/iwnlcern/utt/pull/4
SUBJECT: MERGE BLOCKED — main-forward integration exposes engine-stdin corpus census mismatch; one test-file correction needs fresh implementation authority

## Verdict

merge-blocked

The addressed merge grant is structurally valid and exact-file lint clean. PR 4 was open at the granted feature head `9f2e5a9f8565510959e6bc64b40ebb2158db57c7`; the live remote base was `origin/main@f06c022ce488e9a0b287b65102b283cb407d718d`. Per granted action 1, I merged current `origin/main` forward into the clean feature worktree, producing local commit `c8d087f62384b4c6221ac77e45c567d9c62aed9e` with parents `9f2e5a9f8565510959e6bc64b40ebb2158db57c7` and `f06c022ce488e9a0b287b65102b283cb407d718d`. The merge was conflict-free but non-trivial, so I reran the battery before any push or hosted merge.

## Fresh failure and root cause

`cd ui && npm test` is red: 18 files ran, 17 passed and 1 failed; 251/252 tests passed. The failing composed acceptance assertion is `ui/test/acceptance.test.ts:154-155`, which recursively enumerates every `.jsonl` below `referee/tests/fixtures` and expects the fixed 19-entry `conformanceFixtures` game-log list.

Current main intentionally added a disjoint 38-file `referee/tests/fixtures/engine-stdin/**/*.jsonl` request-stream corpus. Owner evidence in `referee/tests/test_protocol_artifacts.py` selects game-event logs with root-level `*.jsonl` plus `parity-*/*.jsonl`, separately selects all engine-input streams below `engine-stdin`, and asserts the sets are disjoint and exactly 19/38. `referee/tests/gen_fixtures.py` also generates and validates exactly 19 logs plus 38 streams. Fresh disk census is `all-jsonl=57`, `event-logs=19`, `engine-stdin=38`.

The focused command `cd ui && npm test -- test/acceptance.test.ts -t "pins the complete landed harness/referee conformance inventory and every fault class"` reproduces 1 failed / 54 skipped. This is a stale UI census assumption, not an invalid owner corpus and not a flaky test.

The smallest corrective scope is one file: align `ui/test/acceptance.test.ts` conformance discovery with the owner's event-log selection (root game logs plus parity subdirectories, excluding the separately owned `engine-stdin` request streams), while retaining the exact 19-log inventory and all ten fault-class assertions. No correction was attempted because this MERGE-GATE carries merge authority, not the separate literal implementation authority required for a test edit.

## Other fresh verification at local composed head c8d087f

- `cd ui && npm run lint` -> exit 0. — E2.
- `cd ui && npm run build` -> exit 0; 33 modules transformed. — E2.
- `cd referee && .venv/bin/pytest -q` -> 206 passed, 1 expected development-only theory-fixture skip. — E2.
- Owner Draft 2020-12 schemas plus `read_log`/`replay_frames` over all 13 complete UI fixtures -> `owner-schema-replay-ok files=13`. — E2.
- Real-browser E3 was not attempted because the prerequisite battery is red and PR 4 has not merged.

## Actions withheld and requested next action

The local merge-forward commit was not pushed. PR 4 was not merged. No PR-body edit, deployment, release, branch deletion, or E3 claim was made.

Preferred next action: issue a fresh, exactly addressed implementation dispatch for the one-file `ui/test/acceptance.test.ts` census correction on local head `c8d087f`, followed by the focused RED/GREEN, full 252-test battery, lint, build, owner 13/13 validation, normal push, hosted merge with exact-head matching, and the already-granted post-merge E3. Alternative action requires a fresh operator ruling that explicitly accepts merging despite the known failed battery; this Implementer does not recommend that route.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local test census and merge workflow only
- migration/backfill/destructive-write/canonical-data-repair: no — no data mutation or repair
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game artifacts only
- AI-or-automation-acts-downstream: no — no downstream action bridge
- worker/scheduler/queue/retry/async-side-effect: no — no worker or asynchronous side effect
- cross-repo/service-contract/generated-schema/shared-API-event: yes — UI acceptance consumes the harness/referee owner corpus contract
- user-visible-control-with-materializer/downstream-consumer: no — the newly exposed defect is test-only corpus selection
- test-runtime-role-mismatch: yes — UI test discovery conflates game-event logs with engine-input request streams
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — merging despite the failed battery would accept residual integration risk and skip the granted E3 prerequisite
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested; fresh implementation authority or operator ruling is requested instead
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: local unpushed merge-forward branch ui-c1-impl@c8d087f62384b4c6221ac77e45c567d9c62aed9e; parents 9f2e5a9f8565510959e6bc64b40ebb2158db57c7 + f06c022ce488e9a0b287b65102b283cb407d718d; PR 4 remains unmerged at remote head 9f2e5a9f8565510959e6bc64b40ebb2158db57c7
FINAL_GIT_STATUS_SHORT: none — clean linked worktree; branch is 30 commits ahead of origin/ui-c1-impl solely from the authorized local main-forward merge ancestry
