# Harness c2 (engine-stdin export, corpus doc pins, conditional theory-seam flip) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

PLAN_LOCK_ID: PL-harness-c2-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825 @ sha256 c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
GRILL_LOCK_ID: GRILL-harness-c1-20260825 (unchanged; this cycle executes the adopted corpus ruling s1-corpus-ruling-1 under the owner pin harness-c1-contract-rulings-2 — no new design phase)

**Goal:** Export per-game per-seat engine-stdin message-stream conformance fixtures, pin all three corpus locations in the protocol doc, and (conditionally) flip the theory-seam skip to required if theory/fixtures lands within this cycle.

**Architecture:** Capture happens at the single choke point where stdin bytes exist — `Engine.send_line` — into a per-seat append sink that persists across recovery restarts (same pattern as the per-seat stderr sink). `play_game` owns sink lifecycle via a new optional `stream_dir`. The committed stream corpus is produced by the same seeded fake-clock runs as the paired log fixtures.

**Tech Stack:** unchanged — Python ≥3.12 stdlib runtime; pytest/jsonschema dev-only.

**Spec:** DD-harness-c1-20260825 (normative) + the pin relay harness-c1-contract-rulings-2 (location/shape of record) + dispatch harness-c2 (scope/acceptance).

## Global Constraints

- In-scope surfaces ONLY: `referee/` fixture-generation and test files, generated `referee/tests/fixtures/engine-stdin/` artifacts, `docs/protocol/poorman-uttt-protocol-v1.md`. Anything else is a deviation relay.
- Stream file mapping (pin as forward-corrected by harness-c1-contract-rulings-3; BIJECTIVE over all nineteen event-log paths): for each event log at path `L` relative to `referee/tests/fixtures/` (subdirectories included, e.g. `parity-even/game-0001-r0001-g1.jsonl`), the two stream files are `referee/tests/fixtures/engine-stdin/<L minus .jsonl>.<seat>.jsonl` for seat X and O — e.g. `engine-stdin/parity-even/game-0001-r0001-g1.X.jsonl`, `engine-stdin/success.O.jsonl`. Stream glob: `referee/tests/fixtures/engine-stdin/**/*.jsonl` (recursive; the directory holds only stream files). 19 logs → exactly 38 distinct stream paths.
- Stream content: EXACTLY the bytes the referee wrote to that seat's stdin, in order: hello_request line, every turn_request line including re-request attempts, game_end line; no wrapper, no envelope, no extra fields.
- Capture records only FULLY DELIVERED bytes: `send_line` builds one canonical payload, requires complete delivery before recording; a pre-write fault captures nothing; a partial or failed delivery is classified `eof_or_crash` and captures nothing; a capture-sink write failure is a referee-side error (raised), never an engine fault.
- Idempotent generation: each seat's stream file is opened truncating (`"wb"`) once per game and the SAME handle is retained across §5.1 recovery generations; rerunning generation into the same target produces identical bytes, never appended duplicates.
- Canonical serialization and the fake-clock generation path keep streams byte-stable across regenerations.
- Branch `harness-c2-impl` from dispatch-time main (BASE at plan time: main@e92caff); commits after each green cycle; merge separately gated.

---

### Task 1: stdin capture in Engine + play_game plumbing

**Files:**
- Modify: `referee/poorman_referee/procs.py` (Engine), `referee/poorman_referee/referee.py` (GameConfig, play_game, recovery path)
- Test: `referee/tests/test_procs.py`, `referee/tests/test_referee_e2e.py`

**Interfaces:**
- Produces: `Engine.__init__(..., stdin_capture: typing.BinaryIO | None = None)` — when set, `send_line` records the exact payload bytes to the sink, ONLY after complete delivery to the engine's stdin. Delivery is made short-write-safe: build the canonical payload once, deliver via an `os.write` loop on the raw descriptor (or assert the buffered write count equals the payload length); partial or failed delivery → `"eof_or_crash"`, nothing captured; a sink `OSError` raises (referee-side defect, not an engine fault). `GameConfig.stream_dir: str | None = None` — when set, `play_game` computes each seat's stream path by the pinned bijection from `cfg.log_path`'s fixture-relative location, opens it TRUNCATING (`"wb"`) exactly once per game per seat, passes the same handle to every Engine generation for that seat (initial spawn and every §5.1 recovery restart), and closes both in the terminal `finally`.

- [ ] **Step 1: Failing tests** —
```python
def test_stdin_capture_receives_exactly_written_bytes(tmp_path):
    # ok hello + one turn: capture == canonical hello line + canonical turn line
def test_stdin_capture_skips_unwritten_faulted_send(tmp_path):
    # stale-bytes-at-send fault: send_line returns "extra_protocol_line", capture unchanged
def test_stream_files_span_recovery_generations(tmp_path):
    # E2E single-fault game with stream_dir: faulter's stream contains its re-hello line
    # from the restarted generation appended after the pre-fault turn requests
def test_stream_file_is_replayable_request_sequence(tmp_path):
    # E2E clean game: each line json-parses; line 1 type=="hello"; middle lines type=="turn"
    # with request_ids equal, in order, to the paired log's attempts; last line type=="game_end"
def test_stream_regeneration_is_byte_identical(tmp_path):
    # two seeded fake-clock runs -> byte-equal stream files (both seats)
def test_double_fault_rerequest_lines_present(tmp_path):
    # double-fault game: attempt-2 turn_request line present in both seats' streams
def test_second_run_same_target_is_idempotent(tmp_path):
    # run the same seeded game twice with the same stream_dir: files byte-equal,
    # sizes unchanged after run 2 (truncate semantics, no append duplication)
def test_partial_stdin_write_is_eof_fault_and_uncaptured(tmp_path):
    # monkeypatched raw write returning a short count: send_line -> "eof_or_crash",
    # capture sink unchanged
```
- [ ] **Step 2: run, FAIL.**
- [ ] **Step 3: Implement** — sink write in `send_line` immediately after `process.stdin.flush()` succeeds; `play_game` opens sinks when `stream_dir` set, threads them through initial `Engine(...)` construction and the recovery-restart constructor, closes in `finally`; hello during recovery flows through the same `send_line` path so it is captured automatically.
- [ ] **Step 4: run, PASS. Step 5: Commit** `"harness: engine-stdin stream capture"`.

### Task 2: generate + commit the stream corpus, extend artifact tests

**Files:**
- Create: `referee/tests/gen_fixtures.py` (the checked-in generator — no tracked generator exists today; this file is IN SCOPE), `referee/tests/fixtures/engine-stdin/**/*.jsonl` (generated, checked in)
- Modify: `referee/tests/test_protocol_artifacts.py`

**Interfaces:**
- Produces: `gen_fixtures.py` encodes the COMPLETE nineteen-game seed/config matrix (one entry per checked-in event log: stub commands, fault scripts, seeds, clocks) as the single source of regeneration; run as `uv run --project referee python referee/tests/gen_fixtures.py` to (re)write logs + streams + the stderr sidecars for the one fixture that has them, and with `--check` to regenerate into a temporary tree and byte-compare EVERY committed event log, stream file, and `.stderr` sidecar, exiting nonzero on any difference. One stream pair (`.X.jsonl`, `.O.jsonl`) exists per event log under the pinned bijection (38 files).

- [ ] **Step 1: Failing tests** —
```python
def test_stream_mapping_is_bijective_over_all_logs():
    # apply the pinned mapping to all 19 committed event-log paths: exactly 38 distinct
    # stream paths, all existing on disk, and no stream file exists outside the mapping
def test_every_stream_file_round_trips_through_referee_reader():
    # each line parses via the referee's own protocol module by message type, validates
    # against docs/protocol/schema/{hello_request,turn_request,game_end}.schema.json,
    # AND canonical_dumps(parsed) + "\n" byte-equals the original line
def test_log_corpus_globs_and_stream_glob_are_disjoint_and_exact():
    # the two pinned event-log globs (fixtures/*.jsonl, fixtures/parity-*/*.jsonl) match
    # all 19 logs and zero stream files; engine-stdin/**/*.jsonl matches all 38 streams
def test_generator_check_mode_passes_on_committed_artifacts():
    # subprocess: gen_fixtures.py --check exits 0 against the committed tree
```
- [ ] **Step 2: PRE-STEP (MR4)** — replace `FIXTURES.rglob("*.jsonl")` in `test_every_conformance_jsonl_validates_and_replays` with the two pinned event-log globs BEFORE any stream file lands, so log-corpus validation never sweeps stream files. Run suite: still green.
- [ ] **Step 3: FAIL (new tests). Step 4: Write gen_fixtures.py + generate** the full corpus (fake clock, matrix-encoded seeds); check in generated artifacts unmodified. **Step 5: PASS (full suite + `--check`). Step 6: Commit** `"harness: engine-stdin stream corpus + checked-in generator"`.

### Task 3: protocol-doc corpus section

**Files:**
- Modify: `docs/protocol/poorman-uttt-protocol-v1.md`; Test: `referee/tests/test_protocol_artifacts.py`

- [ ] **Step 1: Failing test** — a doc test asserting the corpus section names all three pinned locations verbatim: `docs/protocol/transcript-v1.jsonl`; log corpus `referee/tests/fixtures/*.jsonl` + `referee/tests/fixtures/parity-*/*.jsonl`; stream corpus `referee/tests/fixtures/engine-stdin/**/*.jsonl` with the bijective `<log-relative-path minus .jsonl>.<seat>.jsonl` mapping sentence and the raw-stdin shape sentence.
- [ ] **Step 2: FAIL. Step 3: Write the section** (replaces the previously owed single sentence; full sentences on their own lines). **Step 4: PASS. Step 5: Commit** `"harness: pin corpus locations in protocol doc"`.

### Task 4 (conditional): theory-seam flip

- [ ] **Step 1: Check** — does `theory/fixtures/` contain landed fixture files on current main at execution time?
- [ ] If YES: remove the development-only skip marker from `referee/tests/test_fixture_loader.py` (a test-file edit, in scope) and run the integration test. GREEN → record the run and commit `"harness: theory-seam integration required and green"`. ANY failure → STOP with a deviation relay carrying the failure evidence; make NO other edit — `referee/poorman_referee/fixture_loader.py` and every other production file are OUT of this cycle's locked scope, whatever the failure's cause.
- [ ] If NO: make no edit; the report states the obligation stays standing and this cycle closes without it.

---

## Acceptance (from dispatch harness-c2, verbatim targets)

1. Stream corpus regenerates byte-identically with the paired logs (Task 1 test + Task 2 regeneration).
2. Every stream file round-trips through the referee's own reader (Task 2 schema round-trip test).
3. Doc section pins all three locations (Task 3 test).
4. Full suite green; the only permitted skip is the theory-seam marker and only if Task 4 hit the NO branch.
5. Engine's criterion-3 consumer satisfiable at `referee/tests/fixtures/engine-stdin/**/*.jsonl` (their alignment is engine-c2's job, not this cycle's).
6. Exit: branch + PR + report; merge under a separate addressed grant.

## Boundary contract

Writes: referee capture plumbing + stream corpus + doc pins. Reads: existing log fixtures/seeds; theory fixtures only via the Task 12 (c1) loader seam in Task 4's YES branch. Consumers: engine-c2 corpus round-trip, friend engines, future tooling. Out of scope: everything else — engine/, theory/, ui/, bridge/network, schema changes, new message types.

## Operator-judgment items

None — executes an adopted orchestrator ruling under the locked DD and the owner pin.
