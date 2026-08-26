ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c2-plan-review-4
PARENT_DISPATCH_ID: harness-c2-plan-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the pair Planner can correct the remaining generator contract and reissue; implementation stays held
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-plan-3/PLAN-planner-20260825-213428.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
PLAN_SHA256: 3674bb02761577d79e664736d6598a5475f73ebb19b1f960c62b532ccdf29f74
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c2
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: main@86722b6a3354d697586770caa4366cf8a5031cfa observed during final review; dispatch-time rebase remains required
TARGET_BRANCH: main
BRANCH: none — PLAN-REVIEW is read-only
SUBJECT: PLAN-REVIEW must-revise — generator clock oracle and check hermeticity remain invalid

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The successor is correctly addressed, exact-file clean, selected-file root-context clean, and its design/plan digests match. MR1's pin lineage, MR3's delivery rule, MR4's exact reader, MR5's loader STOP, and the carrier-status correction are accepted. The remaining MR2 generator contract cannot meet its own byte-identity and hermetic-check claims against the current committed corpus, so implementation dispatch remains held.

## Required revision — MR2 generator contract

### MR2-A — the frozen-zero clock premise is false

Plan line 77 says the committed `elapsed_ms` values are all zero and therefore selects one shared frozen-zero clock. Direct census of all nineteen committed `game_start` records shows the opposite: every X hello is nonzero (nine are 22 ms, eight are 23 ms, and two are 25 ms), while O is zero. Other committed recovery/turn records also include nonzero 56 ms and 354 ms values. The current `referee/**` tree is status-clean, so these are the committed inputs the proposed `--check` must reproduce.

A frozen-zero clock would change all nineteen `game_start` records, contradicting plan lines 74 and 77's byte-identical-rewrite and byte-equal assertions. Choose and pin one valid path:

1. Provide deterministic per-game clock scripts/vectors that reproduce every committed elapsed field byte-for-byte; or
2. Explicitly declare a baseline reset to zero-clock logs, enumerate the resulting nineteen changed logs, and obtain any scope/contract ruling needed before reissue.

Do not describe a baseline-changing regeneration as a byte-identical rewrite.

### MR2-B — the matrix source is circular

Plan line 77 has `gen_fixtures.py` build its supposedly pinned matrix by reading the generated event logs that `--check` is meant to verify. That makes the output corpus the runtime configuration oracle and does not create the checked-in independent single source required by the prior MR2. Pin the nineteen rows as literal generator data or a separately named checked-in manifest: exact output path, commands/engine IDs, seeds, coin/tournament values, time controls, deterministic clock input, and marker policy. `--check` must consume that independent matrix and compare generated outputs to the committed corpus; it must not derive its expected configuration from those outputs at runtime.

### MR2-C — `--check` is not hermetic and its inventory is incomplete

The current corpus contains six embedded marker paths across four fixtures. Plan line 79 explicitly allows `--check` to create and delete those paths inside the committed fixture tree, while also claiming it writes nothing outside the temporary target; those statements conflict. Pre/post deletion is still a committed-tree mutation and can destroy pre-existing state. The same line tells inventory comparison to ignore all dotfiles, so an unexpected marker dotfile cannot fail the claimed complete bidirectional inventory check.

Run marker effects under the temporary tree while preserving the logged argv bytes, for example by using a temporary repo-shaped cwd with real temporary `referee/tests/fixtures/` paths and the required engine scripts available there. Require the committed tree to remain byte/inventory unchanged throughout `--check`, and compare an exact marker allowlist rather than ignoring dotfiles wholesale.

### MR2-D — spell the sidecar allowlist literally

Plan lines 74 and 78 use the abbreviated `03f5d3f9….{X,O}.stderr` form while calling it an exact literal allowlist. Replace it with both full paths:

- `referee/tests/fixtures/03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.X.stderr`
- `referee/tests/fixtures/03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.O.stderr`

## Accepted portions

- `harness-c1-contract-rulings-3`, acknowledged by `s1-corpus-ruling-2`, is now the sole operative pin.
- Bijective subdirectory-preserving 19-to-38 mapping and recursive stream glob.
- One engine-delivery algorithm: positive short writes continue; zero/error faults; capture only after full delivery.
- Capture-sink defects raise as referee defects; idempotent truncate-once stream handles survive recovery generations.
- Existing reader pinned as `poorman_referee.gamelog.read_log`, followed by type-specific schema validation and canonical byte equality.
- Explicit event-log globs, conditional theory-seam STOP, branch/PR/report exit, and separate merge gate.

## Evidence

- Incoming relay SHA256 `94117ad8...`; plan SHA256 `3674bb02...`; design SHA256 `c935c29c...`.
- Orchestrator acknowledgment `s1-corpus/SITREP-orchestrator-planner-20260825-212648.md` is exact-file clean.
- Current corpus is 19 event logs, zero stream files, two committed stderr sidecars, and six distinct embedded marker paths.
- Hello census: 19 of 19 X `game_start.hellos.X.elapsed_ms` values are nonzero; distribution = 22 ms x 9, 23 ms x 8, 25 ms x 2. O values are all zero.
- Baseline `uv run --project referee pytest -q referee/tests/test_protocol_artifacts.py` passed 7 tests; product paths and the existing harness implementation worktree are status-clean.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local fixture/runtime plan review
- migration/backfill/destructive-write/canonical-data-repair: no — no product-data mutation
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets only
- AI-or-automation-acts-downstream: yes — referee drives subprocess engines and exports their input corpus
- worker/scheduler/queue/retry/async-side-effect: yes — subprocess pipe writes, recovery restarts, and marker-file side effects
- cross-repo/service-contract/generated-schema/shared-API-event: yes — filename/glob/raw-byte corpus contract consumed by engine-c2
- user-visible-control-with-materializer/downstream-consumer: no — no deployed UI in scope
- test-runtime-role-mismatch: no — current merged referee surfaces inspected
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — an undeclared nineteen-log baseline reset or committed-tree-mutating check would accept residual scope risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none — review-only; no source, test, design, plan, branch, commit, PR, or integration action; only this daemon draft/admission is authored
FINAL_GIT_STATUS_SHORT: exact pre-admission status follows; product paths and the existing harness implementation worktree are clean

```text
 M .superpowers/sdd/PL-ui-c1-20260825/progress.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-8/
```

Next requested action: correct the MR2 generator contract above and reissue `harness-c2-plan-4` for review. Do not dispatch implementation from this verdict.
