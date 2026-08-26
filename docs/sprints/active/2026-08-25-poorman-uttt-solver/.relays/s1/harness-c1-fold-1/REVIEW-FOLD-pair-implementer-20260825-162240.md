ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: harness-c1-fold-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — correction adds the production file explicitly named by the required B2 and M1 mutation checks; merge remains separately gated
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-fold-1/REVIEW-FOLD-pair-implementer-20260825-161951.md
BUNDLE_ID: harness-c1
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: harness-c1-impl@477f491c8f3c713f0981925adba39801f2591d02
TARGET_BRANCH: main
BRANCH: harness-c1-impl
SUBJECT: CORRECTED PRE-EDIT FOLD SCOPE — add procs.py for required B2/M1 mutation checks

The initial scope omitted `referee/poorman_referee/procs.py`, although B2 names its final-sweep loop and M1 requires hardcoding its grace value as negative-control mutations. That file has not been edited. This corrected scope adds it before either mutation; all edits already made are in rows present in the initial scope.

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

Out of scope remains unchanged: `theory/**`, `engine/**`, UI, network transport, ratings, merge, push, and remote configuration.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no credential or permission surface
- migration/backfill/destructive-write/canonical-data-repair: no — generated local test artifacts only
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game state only
- AI-or-automation-acts-downstream: yes — referee supervises bot subprocesses and adjudicates outcomes
- worker/scheduler/queue/retry/async-side-effect: yes — subprocess timing, sweep, and shutdown behavior are directly tested
- cross-repo/service-contract/generated-schema/shared-API-event: yes — protocol artifacts are cross-pair contracts, within the existing lock
- user-visible-control-with-materializer/downstream-consumer: no — no UI or deployed consumer
- test-runtime-role-mismatch: no — real subprocess behavior and generated logs are exercised
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — correction is one relay-named file and E3 remains required
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: uncommitted in-scope RED/GREEN edits in referee.py, stub_engine.py, test_procs.py, and test_referee_e2e.py; `referee/poorman_referee/procs.py` remains untouched at this scope correction
FINAL_GIT_STATUS_SHORT:
 M referee/poorman_referee/referee.py
 M referee/tests/stub_engine.py
 M referee/tests/test_procs.py
 M referee/tests/test_referee_e2e.py
