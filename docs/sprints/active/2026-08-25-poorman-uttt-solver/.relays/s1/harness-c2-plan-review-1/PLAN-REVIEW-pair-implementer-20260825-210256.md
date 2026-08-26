ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c2-plan-review-1
PARENT_DISPATCH_ID: harness-c2-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the contract owner and orchestrator can resolve the technical pin conflict and reissue the plan; implementation remains held
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-plan-1/PLAN-planner-20260825-205624.md
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
SUBJECT: PLAN-REVIEW must-revise — flat-name collisions, missing generator, non-idempotent capture, insufficient round-trip proof, conditional scope breach

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The design and plan digests match their carriers, `harness-c1-design-review-4` is an approving parent for the same design identity/digest, and the incoming PLAN relay is exact-file and relay-context clean. The continuation goal and high-level boundary are sound. Implementation remains held because the plan cannot satisfy its own one-file-per-game acceptance or mechanical all-IN dispatch gate as written.

## Required revisions

### MR1 — make the pinned stream filename mapping injective and forward-correct the owner pin

Plan lines 20 and 64 require one flat `engine-stdin/<log-fixture-stem>.<seat>.jsonl` pair for each of all nineteen log fixtures. The current corpus has nineteen log paths but only seventeen distinct basename stems: both `parity-even/` and `parity-odd/` contain `game-0001-r0001-g1.jsonl` and `game-0001-r0001-g2.jsonl`. The mechanical census is therefore `required_stream_files=38`, `unique_flat_stream_paths=34`, `collisions=4`.

This is a technical conflict in the immutable owner pin at `harness-c1-contract-rulings-2` lines 24–27, not an implementation naming choice. File an owner-pin successor through `s1.orchestrator-planner` with `engine.planner` visible, then reissue this plan against the corrected exact mapping and discovery glob. A flat relative-path encoding such as an explicit directory-qualified stem can preserve the nonrecursive engine glob, but the contract owner/orchestrator must select and pin the literal grammar. Add a bijection test that enumerates the nineteen source log paths, derives thirty-eight distinct destination paths, and rejects any collision before generation.

### MR2 — replace “wherever the existing fixture-generation path is” with a checked-in, named generator and check mode

Plan line 60 names no file. A tracked-file census finds only `referee/poorman_referee/fixture_loader.py` and `referee/tests/test_fixture_loader.py`; there is no checked-in harness conformance-corpus generator. Consequently the pre-dispatch `SCOPE_DIFF` cannot be mechanical, and Task 1's two-temporary-run determinism test does not prove the committed streams were generated from the paired committed logs.

Name an exact new generator path in the plan and scope, specify the complete nineteen-game case/seed/config matrix, and add a check mode or test that regenerates into a temporary tree and byte-compares every committed event log and every committed stream against generator output. The plan must state how existing stderr sidecars are handled so regeneration neither silently omits nor clobbers them. The generator file must appear explicitly in the successor PLAN and its future `SCOPE_DIFF`.

### MR3 — make regeneration idempotent and make “exact bytes written” a full-write contract

Plan line 34 mandates append-mode sink files. Re-running a generator into the same target appends a second transcript and contradicts lines 22 and 97's byte-stable regeneration claim. Open/truncate each seat file exactly once per game (or generate in a clean temporary tree and atomically replace), keep that same handle across recovery generations, and add a same-target second-run test that proves no duplication.

Current `Engine.send_line` ignores the return value of `process.stdin.write` at `referee/poorman_referee/procs.py:170-176`. Capturing the complete intended payload after `flush()` can therefore overstate what the pipe accepted if a write is partial without raising. Build the canonical payload once, require/write the full payload before recording it, and add a partial-write regression; capture nothing on any pre-write fault and capture exactly the fully delivered bytes on success. State capture-sink write/flush failure behavior explicitly rather than allowing an accidental new adjudication path.

### MR4 — make artifact tests prove the locked reader, canonical bytes, and log/stream glob separation

The existing `test_every_conformance_jsonl_validates_and_replays` uses `FIXTURES.rglob("*.jsonl")` at `referee/tests/test_protocol_artifacts.py:74-82`; new `engine-stdin/*.jsonl` files would be ingested as game-event logs and sent to `validate_events`/`replay_frames`. The successor plan must explicitly replace that collector with the two pinned event-log globs before adding the stream corpus.

The proposed Task 2 round-trip test only says JSON-parse plus schema validation. Acceptance line 98 says every stream round-trips through the referee's own reader, while the exact-byte pin also requires canonical byte identity. Require `read_log` (or name a new in-scope stream-reader API if that is the intended contract), validate each object by its `type`, and assert canonical reserialization plus newline equals each original line byte-for-byte. Pair this with the generator check from MR2; schema-valid hand-authored files are not sufficient evidence.

### MR5 — remove the conditional out-of-scope loader edit

Task 4 line 90 authorizes fixing harness-side naming-map bugs in `referee/poorman_referee/fixture_loader.py`, but the incoming relay's locked production scope names only `procs.py` and `referee.py`; the plan's own hard scope makes anything else a deviation. Current main has no `theory/fixtures/*.json`, so the branch is presently NO, but a conditional OUT edit still prevents an all-IN `SCOPE_DIFF`.

Choose one before reissue: either state that any failing theory-seam run stops and files a deviation relay without editing `fixture_loader.py`, or obtain an orchestrator scope expansion and list that exact file in the successor plan/dispatch. Do not leave an implementation-time judgment that silently expands the branch.

## What is already acceptable

- The approved design identity and digest are correct and unchanged.
- `Engine.send_line` is the correct capture choke point, and sharing one per-seat sink handle across `_recover_seats` generations is the correct lifecycle shape once truncation/full-write semantics are fixed.
- The six named Task 1 behavioral tests cover successful sends, pre-write faults, recovery hello persistence, request order, determinism, and double-fault retries; retain them and add the MR3 partial-write/same-target cases.
- The protocol documentation task and the conditional NO theory branch stay within the continuation goal.
- Branch/PR/report exit and separate merge authorization are correctly preserved.

## Boundary and escalation

Writes: none in this review; the successor plan would write only its corrected, explicitly enumerated harness surfaces.
Reads: locked plan/design, owner/orchestrator rulings, current referee code/tests, fixture paths, and current theory-fixture presence.
Target entity: a collision-free, reproducible message-stream corpus and its documented consumer contract.
Downstream consumer: engine-c2 and future friend engines; MR1 must be reconciled before either consumes the pin.
Contract: exact raw stdin bytes, one file per game per seat, canonical regeneration, explicit non-overlapping globs.
Proof: E1 source/plan/ruling inspection plus E2 fixture-path census and tracked-generator census.
No-consumer action: hold delegated implementation dispatch until a successor PLAN receives approval and an all-IN scope diff.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local fixture/runtime plan review
- migration/backfill/destructive-write/canonical-data-repair: no — generated test corpus only; no edit in review
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets only
- AI-or-automation-acts-downstream: yes — referee drives engine subprocesses and publishes a consumer corpus
- worker/scheduler/queue/retry/async-side-effect: yes — stdin delivery and recovery generations are runtime subprocess behavior
- cross-repo/service-contract/generated-schema/shared-API-event: yes — filename/glob/raw-byte pin is consumed by engine-c2
- user-visible-control-with-materializer/downstream-consumer: no — no deployed UI or materializer in this cycle
- test-runtime-role-mismatch: no — review checks the real merged referee surfaces
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — current plan contains an unresolved contract collision and conditional scope expansion
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or residual acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and next action

- `shasum -a 256` matches plan `c27f8cf0...` and design `c935c29c...` exactly.
- Incoming relay exact-file lint: zero errors/warnings. Relay-context lint also reports zero errors/warnings for this file; the root remains red from historical INDEX inversions and a foreign engine-plan digest mismatch, which this seat did not modify.
- Fixture census: `logs=19 unique_stems=17 required_stream_files=38 unique_flat_stream_paths=34 collisions=4`.
- Tracked generator candidates: only `fixture_loader.py` and `test_fixture_loader.py`; neither generates the conformance corpus.
- `theory/fixtures/*.json` count on current main at review time: 0.
- `git diff --check`: exit 0; `referee/**` and `docs/protocol/**` status clean.

ACTIONS_GIT_REF:
- review=daemon draft and admitted PLAN-REVIEW only
- source/tests/design/plan/branch/commit/PR/merge=none — review-only
FINAL_GIT_STATUS_SHORT: concurrent relay-only dirt in the primary checkout before review admission; product paths clean and the existing harness implementation worktree is clean; exact status preserved below

```text
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-7/
```

Next requested action: forward-correct the owner pin with the orchestrator/engine consumer visible, revise `PL-harness-c2-20260825` for MR1–MR5, and issue `harness-c2-plan-2` for re-review. Do not issue delegated implementation dispatch from this verdict.
