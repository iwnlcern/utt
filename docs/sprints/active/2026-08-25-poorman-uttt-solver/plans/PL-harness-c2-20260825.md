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
- Stream file shape (pin, verbatim): `referee/tests/fixtures/engine-stdin/<log-fixture-stem>.<seat>.jsonl`; EXACTLY the bytes the referee wrote to that seat's stdin, in order: hello_request line, every turn_request line including re-request attempts, game_end line; no wrapper, no envelope, no extra fields.
- Capture only bytes actually written: a `send_line` that returns a fault status before writing captures nothing; best-effort `game_end` captures only on successful write.
- Canonical serialization and the fake-clock generation path keep streams byte-stable across regenerations.
- Branch `harness-c2-impl` from dispatch-time main (BASE at plan time: main@e92caff); commits after each green cycle; merge separately gated.

---

### Task 1: stdin capture in Engine + play_game plumbing

**Files:**
- Modify: `referee/poorman_referee/procs.py` (Engine), `referee/poorman_referee/referee.py` (GameConfig, play_game, recovery path)
- Test: `referee/tests/test_procs.py`, `referee/tests/test_referee_e2e.py`

**Interfaces:**
- Produces: `Engine.__init__(..., stdin_capture: typing.BinaryIO | None = None)` — when set, `send_line` appends the exact written bytes (line + newline) to the sink and flushes, ONLY on the successful-write path. `GameConfig.stream_dir: str | None = None` — when set, `play_game` opens one append-mode binary file per seat at `<stream_dir>/<log-stem>.<seat>.jsonl` (log-stem = `Path(cfg.log_path).stem`), passes the handle to every Engine generation for that seat (initial spawn and every §5.1 recovery restart), and closes both in the terminal `finally`.

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
```
- [ ] **Step 2: run, FAIL.**
- [ ] **Step 3: Implement** — sink write in `send_line` immediately after `process.stdin.flush()` succeeds; `play_game` opens sinks when `stream_dir` set, threads them through initial `Engine(...)` construction and the recovery-restart constructor, closes in `finally`; hello during recovery flows through the same `send_line` path so it is captured automatically.
- [ ] **Step 4: run, PASS. Step 5: Commit** `"harness: engine-stdin stream capture"`.

### Task 2: generate + commit the stream corpus, extend artifact tests

**Files:**
- Modify: the existing fixture-generation path (wherever the c1 committed fixtures are produced from — extend it to pass `stream_dir`), `referee/tests/test_protocol_artifacts.py`
- Create: `referee/tests/fixtures/engine-stdin/*.jsonl` (generated, committed)

**Interfaces:**
- Produces: one committed stream pair (`.X.jsonl`, `.O.jsonl`) for every committed log fixture (success, each fault-*, each recovery-*, terminal-fault-won-no-recovery, all four parity games) — same stems, same seeded runs.

- [ ] **Step 1: Failing tests** —
```python
def test_every_log_fixture_has_paired_stream_files():
    # for each committed log fixture stem: engine-stdin/<stem>.X.jsonl and .O.jsonl exist
def test_every_stream_file_round_trips_through_referee_reader():
    # each line parses as canonical JSON; hello/turn/game_end fields validate against
    # docs/protocol/schema/{hello_request,turn_request,game_end}.schema.json
def test_stream_corpus_glob_is_exactly_the_pin():
    # glob "engine-stdin/*.jsonl" finds all stream files and NOTHING matches the
    # log-corpus globs (fixtures/*.jsonl + parity-*/*.jsonl exclude engine-stdin/)
```
- [ ] **Step 2: FAIL. Step 3: Regenerate all fixtures WITH streams** via the extended generation path (fake clock, existing seeds); commit generated artifacts unmodified. **Step 4: PASS (full suite). Step 5: Commit** `"harness: engine-stdin stream corpus"`.

### Task 3: protocol-doc corpus section

**Files:**
- Modify: `docs/protocol/poorman-uttt-protocol-v1.md`; Test: `referee/tests/test_protocol_artifacts.py`

- [ ] **Step 1: Failing test** — a doc test asserting the corpus section names all three pinned locations verbatim: `docs/protocol/transcript-v1.jsonl`; log corpus `referee/tests/fixtures/*.jsonl` + `referee/tests/fixtures/parity-*/*.jsonl`; stream corpus `referee/tests/fixtures/engine-stdin/*.jsonl` with the one-file-per-game-per-seat raw-stdin shape sentence.
- [ ] **Step 2: FAIL. Step 3: Write the section** (replaces the previously owed single sentence; full sentences on their own lines). **Step 4: PASS. Step 5: Commit** `"harness: pin corpus locations in protocol doc"`.

### Task 4 (conditional): theory-seam flip

- [ ] **Step 1: Check** — does `theory/fixtures/` exist on current main at execution time?
- [ ] If YES: remove the development-only skip marker from `referee/tests/test_fixture_loader.py`, run the integration test against theory's landed fixtures, fix loader-side mismatches ONLY if they are harness-side naming-map bugs (a theory-schema mismatch is a deviation relay, not an edit), and record the run in the implementation report. Commit `"harness: theory-seam integration required and green"`.
- [ ] If NO: make no edit; the report states the obligation stays standing and this cycle closes without it.

---

## Acceptance (from dispatch harness-c2, verbatim targets)

1. Stream corpus regenerates byte-identically with the paired logs (Task 1 test + Task 2 regeneration).
2. Every stream file round-trips through the referee's own reader (Task 2 schema round-trip test).
3. Doc section pins all three locations (Task 3 test).
4. Full suite green; the only permitted skip is the theory-seam marker and only if Task 4 hit the NO branch.
5. Engine's criterion-3 consumer satisfiable at `referee/tests/fixtures/engine-stdin/*.jsonl` (their alignment is engine-c2's job, not this cycle's).
6. Exit: branch + PR + report; merge under a separate addressed grant.

## Boundary contract

Writes: referee capture plumbing + stream corpus + doc pins. Reads: existing log fixtures/seeds; theory fixtures only via the Task 12 (c1) loader seam in Task 4's YES branch. Consumers: engine-c2 corpus round-trip, friend engines, future tooling. Out of scope: everything else — engine/, theory/, ui/, bridge/network, schema changes, new message types.

## Operator-judgment items

None — executes an adopted orchestrator ruling under the locked DD and the owner pin.
