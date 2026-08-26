ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c2-plan-review-2
PARENT_DISPATCH_ID: harness-c2-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the contract owner and orchestrator can resolve the technical pin conflict and reissue the plan; implementation remains held
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-plan-review-1/PLAN-REVIEW-pair-implementer-20260825-210256.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
PLAN_SHA256: c27f8cf0c8405b918c4949d7ee16431fc8ccdba8cf8fdb8b469599db035aa51c
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c2
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: main@d1924b82174b0dc38c45cff7bf55764950659510 observed at final review check; dispatch-time rebase remains required
TARGET_BRANCH: main
BRANCH: none — PLAN-REVIEW is read-only
SUPERSEDES: harness-c2-plan-review-1/PLAN-REVIEW-pair-implementer-20260825-210256.md — structural action-reference correction only; verdict and findings unchanged
SUBJECT: PLAN-REVIEW must-revise — corrected carrier for five required plan revisions

## Structural correction and verdict

PLAN_REVIEW_VERDICT: must-revise

This successor replaces the predecessor because its action-absence wording was parsed as a merge claim in relay-context lint. No merge occurred or was authorized. The substantive review is unchanged: the design/plan digests and approving design lineage are correct, but the plan cannot satisfy its one-file-per-game or mechanical all-IN gates as written.

## Required revisions

### MR1 — collision-free owner pin

The flat `<log-fixture-stem>.<seat>.jsonl` grammar is not injective. There are nineteen event-log paths but only seventeen distinct basename stems because parity-even and parity-odd each contain `game-0001-r0001-g1.jsonl` and `game-0001-r0001-g2.jsonl`. Required output is thirty-eight files; the current grammar can name only thirty-four distinct paths.

Forward-correct `harness-c1-contract-rulings-2` through the orchestrator with `engine.planner` visible, pin an exact collision-free mapping/glob, and add a bijection test over all nineteen source paths before reissuing the plan.

### MR2 — checked-in generator and regeneration check

Plan line 60's “existing fixture-generation path” does not exist in tracked harness files; the only tracked generator-name matches are the loader and its test. Name an exact new generator path and include it in scope. Specify the complete nineteen-game seed/config matrix and a check mode that regenerates into a temporary tree and byte-compares every committed event log and stream. State explicit stderr-sidecar handling.

### MR3 — idempotent and full-write capture semantics

Append-mode files duplicate transcripts when generation reruns into the same target. Truncate each seat file once per game, or generate into a clean temporary tree and atomically replace; retain the same handle across recovery generations. Add a same-target second-run test.

Current `Engine.send_line` ignores the pipe write return value. Build one canonical payload, require full delivery before recording it, test partial writes, and state capture-sink failure behavior. Capture nothing on every pre-write fault and exactly the fully delivered bytes on success.

### MR4 — exact reader/byte/glob proof

Explicitly replace `FIXTURES.rglob("*.jsonl")` in `test_every_conformance_jsonl_validates_and_replays` with the two pinned event-log globs before adding the stream directory. For streams, use the referee reader (or name an exact new in-scope reader), validate each object by message type, and assert canonical reserialization plus newline equals each original line byte-for-byte. Schema-valid or hand-authored files alone do not prove regeneration.

### MR5 — conditional loader scope

Task 4 authorizes production edits to `referee/poorman_referee/fixture_loader.py`, but the locked production scope names only `procs.py` and `referee.py`. Current main has no theory fixture JSON, yet the YES branch must still be all-IN. Either make every failing seam run stop with a deviation relay, or obtain and lock an orchestrator scope expansion naming `fixture_loader.py` before reissue.

## Accepted portions

- Locked design identity/digest and approving parent lineage.
- `Engine.send_line` as the choke point and one per-seat handle shared across recovery generations.
- Named tests for successful/pre-write-fault capture, recovery hello persistence, request order, determinism, and double-fault retry lines, supplemented by MR3.
- Protocol documentation task, conditional NO theory branch, branch/PR/report exit, and separate merge gate.

## Boundary and escalation

Writes: none in review.
Reads: plan/design/rulings, current referee code/tests, fixture paths, and theory-fixture presence.
Target entity: a collision-free reproducible raw-stdin corpus.
Downstream consumer: engine-c2 and friend engines.
Contract: one file per game per seat, exact raw stdin bytes, canonical regeneration, and disjoint explicit globs.
Proof: E1 source/plan inspection plus E2 fixture and tracked-file census.
No-consumer action: hold implementation dispatch until a corrected PLAN is approved and its scope diff is all-IN.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local fixture/runtime plan review
- migration/backfill/destructive-write/canonical-data-repair: no — generated test corpus only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated budgets only
- AI-or-automation-acts-downstream: yes — referee drives subprocess engines and publishes their input corpus
- worker/scheduler/queue/retry/async-side-effect: yes — stdin delivery and recovery generation behavior
- cross-repo/service-contract/generated-schema/shared-API-event: yes — filename/glob/raw-byte contract consumed by engine-c2
- user-visible-control-with-materializer/downstream-consumer: no — no deployed UI in scope
- test-runtime-role-mismatch: no — current merged referee surfaces inspected
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — unresolved collision and conditional scope expansion
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence

- Plan digest `c27f8cf0...` and design digest `c935c29c...` match exactly.
- Incoming PLAN relay is exact-file and relay-context clean.
- Census: `logs=19 unique_stems=17 required_stream_files=38 unique_flat_stream_paths=34 collisions=4`.
- No tracked conformance generator exists; current `theory/fixtures/*.json` count is zero.
- `git diff --check` exits zero; `referee/**` and `docs/protocol/**` are status-clean; the existing harness implementation worktree is clean.

ACTIONS_GIT_REF: none — review-only; only this daemon draft/admission was authored; no source, test, design, plan, branch, commit, PR, or integration action
FINAL_GIT_STATUS_SHORT: concurrent relay-only dirt in the primary checkout before correction admission; product paths and the existing harness implementation worktree are clean; exact status follows

```text
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-7/
```

Next requested action: forward-correct the owner pin, revise the plan for MR1–MR5, and issue `harness-c2-plan-2` for re-review. Do not dispatch implementation from this verdict.
