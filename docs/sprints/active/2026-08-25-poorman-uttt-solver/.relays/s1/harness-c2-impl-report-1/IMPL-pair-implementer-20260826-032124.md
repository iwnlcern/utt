ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: harness-c2-impl-report-1
PARENT_DISPATCH_ID: harness-c2-impl-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — implementation publication is complete; remote-base reconciliation and merge remain separately addressed
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-impl-1/IMPL-planner-20260826-024847.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
PLAN_SHA256: d2801b4900d29de55643b396a8d14d67287e0e11eb9b6ecbbaf86f4000ae19cd
BUNDLE_ID: harness-c2
OWNER: harness
REPO: /Users/jack/Programming/utt (origin https://github.com/iwnlcern/utt.git)
BASE: main@a804c7f40ff020f99575ebf73dcff42322b0ddc1
TARGET_BRANCH: main
BRANCH: harness-c2-impl
PR: https://github.com/iwnlcern/utt/pull/5
SUBJECT: IMPLEMENTATION REPORT — harness-c2 stream corpus complete at c17f00d; PR 5 open; remote-base reconciliation and merge separately gated

## Result

Implemented locked plan `PL-harness-c2-20260825` on the clean pushed branch `harness-c2-impl` at `c17f00d8502c0356bc267256201c1b5f32b54d9f`. The exact dispatched-base range `a804c7f40ff020f99575ebf73dcff42322b0ddc1..c17f00d8502c0356bc267256201c1b5f32b54d9f` is 65 files, 2,944 insertions, and 52 deletions across the three required green commits. PR 5 is open against `main`; no merge, deployment, or release action was attempted.

`Engine.send_line` now records one canonical payload only after its raw-descriptor write loop fully delivers the bytes. Positive short writes continue; zero, `OSError`, and closed descriptors remain engine faults with no capture; capture-sink short writes and write, flush, or close failures propagate as referee defects. `play_game` opens truncating per-seat sinks once and reuses each handle through all recovery generations.

The checked-in literal manifest drives nineteen hermetic seeded games from a temporary repo-shaped root with unchanged argv and path-preserving script copies. Generation promotes only the exact allowlist after a successful bare run; `--check` promotes nothing and performs a bidirectional inventory plus byte comparison. The committed mapping is nineteen event logs to thirty-eight distinct canonical stdin streams, plus exactly two allowlisted terminal sidecars and the manifest. The protocol document pins the transcript, the two disjoint event-log globs, the recursive stream glob, the bijection, and the raw-stdin shape.

## Fresh verification and review

- `PYTHONPATH=referee pytest -q` at final head -> 204 passed, 1 skipped in 12.95s. The sole skip is the standing development-only theory-fixture seam. — E2.
- `python referee/tests/gen_fixtures.py --check` at final head -> exit 0, followed by clean `git diff --check`; linked-worktree status was empty after removal of ignored runtime cache bytes. — E2.
- Artifact acceptance covered the 19-to-38 bijection, `read_log` plus per-type Draft 2020-12 validation, canonical line byte equality, exact disjoint globs, manifest authority, unchanged-argv temp-root execution, and committed-tree immutability around `--check`. — E2.
- Two independent hermetic generator runs each byte-matched the committed corpus; the stream-corpus aggregate SHA-256 census was `a4be44a6bae619f01e1b754c9a1c283a7bcd039e62c4dc01ea3a059b7745fc85`. — E2.
- A fresh independent whole-branch reviewer inspected exact range `a804c7f..c17f00d`, found 0 Critical, 0 Important, and 0 Minor findings, independently passed `--check`, 76 affected tests, the 204-pass full suite, exact inventory, elapsed-only structural comparison, diff check, and clean final status. Its technical verdict is ready to merge; that verdict grants no merge authority. — E2.

## Authorized baseline reset record

The one-time reset is authorized only by `harness-c2-reset-ruling-1`; it is not precedent for any later baseline change.

1. Reversibility of record: the pre-reset baseline is `main@29cbdf2`. The executable restore path of record is `git checkout 29cbdf2 -- referee/tests/fixtures/`.
2. Field-scoped proof: structural comparison against `main@29cbdf2` found 35 changed JSON leaves across exactly the nineteen manifest-enumerated event logs, and every changed leaf was named `elapsed_ms`. No non-elapsed value or key changed.
3. Stream invariance proof: recursive field census found no `elapsed_ms` key in any of the thirty-eight stream files; two independent generator runs byte-matched the same committed stream corpus across the reset work.
4. One-time/no-precedent: from this reset forward, `gen_fixtures.py --check` is a strict byte comparison. Any future baseline alteration requires a fresh addressed ruling.
5. Ruling citation: all reset authority and these report obligations derive from `harness-c2-reset-ruling-1`; no wider canonical-data authority is inferred.

## Scope, conditional seam, and residual gates

SCOPE_DIFF:
- referee/poorman_referee/procs.py -> in
- referee/poorman_referee/referee.py -> in
- referee/tests/test_procs.py -> in
- referee/tests/test_referee_e2e.py -> in
- referee/tests/test_protocol_artifacts.py -> in
- referee/tests/gen_fixtures.py -> in
- referee/tests/fixtures/matrix.json -> in
- referee/tests/fixtures/engine-stdin/**/*.jsonl (38 files) -> in
- referee/tests/fixtures/*.jsonl plus referee/tests/fixtures/parity-*/*.jsonl (19 ruling-authorized elapsed-only resets) -> in
- docs/protocol/poorman-uttt-protocol-v1.md -> in
SCOPE_DIFF_RESULT: all-in

Task 4 resolved its locked NO branch: `theory/fixtures/` contains no landed files on current local `main@a804c7f`, so `referee/tests/test_fixture_loader.py` was not touched. The obligation to remove the development-only skip and run the integration remains standing when theory fixtures land.

GitHub currently reports PR 5 OPEN, non-draft, MERGEABLE/CLEAN, head `c17f00d8502c0356bc267256201c1b5f32b54d9f`, but its remote base is still `be07170e0f1cc708e10b352daf603d9243b552a4`. The dispatched local base `a804c7f` contains the already-filed engine-c3 plan/review-request commit `14375d6` and engine-c2 merge-ledger commit `a804c7f`; those four inherited governance files therefore appear temporarily in the remote PR comparison. This seat will not rewrite the dispatched base or push `main` without authority. Orchestrator reconciliation must publish or otherwise reconcile that inherited main lineage, then refresh PR scope and all integration verification before any merge grant.

Next requested action: `harness.planner` and `s1.orchestrator-planner` reconcile PR 5 with the remote-base lag and the independent review. Preserve branch `harness-c2-impl` and its linked worktree for review feedback. Merge remains separately gated.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local subprocess and fixture surfaces only
- migration/backfill/destructive-write/canonical-data-repair: yes — one-time elapsed-field corpus reset, exactly bounded by harness-c2-reset-ruling-1
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: yes — engine consumers ingest the published stdin corpus
- worker/scheduler/queue/retry/async-side-effect: yes — local subprocess lifecycle and recovery generations
- cross-repo/service-contract/generated-schema/shared-API-event: yes — harness-owned protocol corpus is an engine consumer contract
- user-visible-control-with-materializer/downstream-consumer: no — no direct UI control added
- test-runtime-role-mismatch: no — real subprocess E2E and the production reader are exercised
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — theory seam absent and remote main lags the dispatched base
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: harness-c2-impl@c17f00d8502c0356bc267256201c1b5f32b54d9f pushed to origin; PR https://github.com/iwnlcern/utt/pull/5 open against main; exact reviewed implementation range a804c7f40ff020f99575ebf73dcff42322b0ddc1..c17f00d8502c0356bc267256201c1b5f32b54d9f; no merge, deployment, or release action claimed
FINAL_GIT_STATUS_SHORT: none — clean linked implementation worktree
