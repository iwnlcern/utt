# Harness c2 (engine-stdin export, corpus doc pins, conditional theory-seam flip) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

PLAN_LOCK_ID: PL-harness-c2-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825 @ sha256 c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
GRILL_LOCK_ID: GRILL-harness-c1-20260825 (unchanged; this cycle executes the adopted corpus ruling s1-corpus-ruling-1 under the owner pin of record harness-c1-contract-rulings-3, orchestrator-acked by s1-corpus-ruling-2 — rulings-2 is superseded history; no new design phase)

**Goal:** Export per-game per-seat engine-stdin message-stream conformance fixtures, pin all three corpus locations in the protocol doc, and (conditionally) flip the theory-seam skip to required if theory/fixtures lands within this cycle.

**Architecture:** Capture happens at the single choke point where stdin bytes exist — `Engine.send_line` — into a per-seat append sink that persists across recovery restarts (same pattern as the per-seat stderr sink). `play_game` owns sink lifecycle via a new optional `stream_dir`. The committed stream corpus is produced by the same seeded fake-clock runs as the paired log fixtures.

**Tech Stack:** unchanged — Python ≥3.12 stdlib runtime; pytest/jsonschema dev-only.

**Spec:** DD-harness-c1-20260825 (normative) + the pin relay harness-c1-contract-rulings-3 (location/shape of record; rulings-2 superseded) + dispatch harness-c2 (scope/acceptance).

## Global Constraints

- In-scope surfaces ONLY: `referee/` fixture-generation and test files, generated `referee/tests/fixtures/engine-stdin/` artifacts, `docs/protocol/poorman-uttt-protocol-v1.md`. Anything else is a deviation relay.
- Stream file mapping (pin as forward-corrected by harness-c1-contract-rulings-3; BIJECTIVE over all nineteen event-log paths): for each event log at path `L` relative to `referee/tests/fixtures/` (subdirectories included, e.g. `parity-even/game-0001-r0001-g1.jsonl`), the two stream files are `referee/tests/fixtures/engine-stdin/<L minus .jsonl>.<seat>.jsonl` for seat X and O — e.g. `engine-stdin/parity-even/game-0001-r0001-g1.X.jsonl`, `engine-stdin/success.O.jsonl`. Stream glob: `referee/tests/fixtures/engine-stdin/**/*.jsonl` (recursive; the directory holds only stream files). 19 logs → exactly 38 distinct stream paths.
- Stream content: EXACTLY the bytes the referee wrote to that seat's stdin, in order: hello_request line, every turn_request line including re-request attempts, game_end line; no wrapper, no envelope, no extra fields.
- Capture records only FULLY DELIVERED bytes, ONE literal delivery algorithm: `send_line` builds one canonical payload and delivers it with an `os.write` loop on the raw descriptor that CONTINUES after positive short writes (normal POSIX) and FAILS on a zero return or `OSError` → `"eof_or_crash"`, nothing captured; capture happens exactly once, only after the loop delivers the full payload. Sink contract (`stdin_capture` is generic `BinaryIO`): sink delivery uses the same full-write discipline; a sink short write, `write`/`flush` failure, or close failure RAISES (`RuntimeError` chained from the cause) — referee-side defects propagate and are never reclassified as engine faults.
- Idempotent generation: each seat's stream file is opened truncating (`"wb"`) once per game and the SAME handle is retained across §5.1 recovery generations; rerunning generation into the same target produces identical bytes, never appended duplicates.
- Canonical serialization and the fake-clock generation path keep streams byte-stable across regenerations.
- Branch `harness-c2-impl` from dispatch-time main (BASE at plan time: main@e92caff); commits after each green cycle; merge separately gated.

---

### Task 1: stdin capture in Engine + play_game plumbing

**Files:**
- Modify: `referee/poorman_referee/procs.py` (Engine), `referee/poorman_referee/referee.py` (GameConfig, play_game, recovery path)
- Test: `referee/tests/test_procs.py`, `referee/tests/test_referee_e2e.py`

**Interfaces:**
- Produces: `Engine.__init__(..., stdin_capture: typing.BinaryIO | None = None)` — when set, `send_line` records the exact payload bytes to the sink, ONLY after complete delivery to the engine's stdin. Delivery follows the ONE algorithm in Global Constraints: `os.write` loop, continue on positive short writes, fail (`"eof_or_crash"`, uncaptured) on zero return or `OSError`; capture exactly once after full delivery; sink defects raise per the sink contract. `GameConfig.stream_dir: str | None = None` — when set, `play_game` computes each seat's stream path by the pinned bijection from `cfg.log_path`'s fixture-relative location, opens it TRUNCATING (`"wb"`) exactly once per game per seat, passes the same handle to every Engine generation for that seat (initial spawn and every §5.1 recovery restart), and closes both in the terminal `finally`.

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
def test_delivery_loop_decisive_sequences(tmp_path):
    # monkeypatched os.write: (a) positive-short then completion -> success, captured
    # exactly once; (b) zero return -> "eof_or_crash", uncaptured; (c) OSError ->
    # "eof_or_crash", uncaptured
def test_capture_sink_defects_raise_not_engine_fault(tmp_path):
    # sink whose write returns short / raises / whose flush raises: send_line raises
    # RuntimeError (referee defect); no fault classification, no capture record
```
- [ ] **Step 2: run, FAIL.**
- [ ] **Step 3: Implement** — replace the buffered `process.stdin.write(...)` in `send_line` with the raw-descriptor delivery loop; record to the sink only after the loop completes; `play_game` opens sinks when `stream_dir` set, threads them through initial `Engine(...)` construction and the recovery-restart constructor, closes in `finally`; hello during recovery flows through the same `send_line` path so it is captured automatically.
- [ ] **Step 4: run, PASS. Step 5: Commit** `"harness: engine-stdin stream capture"`.

### Task 2: generate + commit the stream corpus, extend artifact tests

**Files:**
- Create: `referee/tests/gen_fixtures.py` (the checked-in generator — no tracked generator exists today; IN SCOPE), `referee/tests/fixtures/matrix.json` (the literal 19-row manifest), `referee/tests/fixtures/engine-stdin/**/*.jsonl` (generated, checked in — 38 files)
- Modify: `referee/tests/test_protocol_artifacts.py`
- Touched by the declared baseline reset (elapsed-field-only regeneration, carried through SCOPE_DIFF explicitly): the 19 committed event logs (`referee/tests/fixtures/*.jsonl`, `referee/tests/fixtures/parity-*/*.jsonl`) and the two allowlisted sidecars named above. Transient marker files exist only inside the temporary generation tree (both modes), never in the committed tree. All inside the dispatch's `referee/` fixture surface; if the reviewer or dispatcher reads any of these as outside it, that is a stop-for-ruling, not a silent IN.

**Interfaces:**
- Manifest (the independent single source, MR2-B): a NEW checked-in literal file `referee/tests/fixtures/matrix.json` carrying all 19 rows verbatim — per row: output log path, per-seat argv (`cmds`) and `engine_ids`, `pair_seed`/`game_seed` (hex), `pair_coin_seat`, `tournament_id`, `game_id`, all four `time_control` values, the deterministic clock spec, and that row's marker-file paths (six embedded paths across four rows). The manifest is AUTHORED ONCE during this task by extracting those fields from the committed logs (one-time bootstrap, then frozen); at runtime `gen_fixtures.py` reads ONLY the manifest and `--check` compares generated outputs against the committed corpus — it never derives its expected configuration from the outputs it verifies.
- AUTHORIZED baseline reset (MR2-A; NOT a byte-identical rewrite; ruling of record: harness-c2-reset-ruling-1, which satisfies the delegated-dispatch "no hard trigger" condition FOR THIS ENUMERATED RESET ONLY): the committed hello `elapsed_ms` values are live wall-clock artifacts (X = 22/23/25 ms across the 19 logs; other records carry 56/354 ms), which no honest deterministic clock reproduces. This task performs the ONE-TIME reset: all 19 event logs are regenerated under the manifest's deterministic scripted clock, changing exactly the elapsed-time fields. Binding ruling conditions, carried into the implementation report verbatim:
  1. Reversibility of record: cite the pre-reset baseline `main@29cbdf2` and the executable restore path `git checkout <pre-reset sha> -- referee/tests/fixtures/`.
  2. Field-scoped proof: a diff check proving the reset changed ONLY elapsed-time fields across exactly the 19 enumerated files — any other key changing fails the cycle.
  3. Stream invariance proof (proven, not asserted): (a) a field census showing no stream line carries any elapsed key, and (b) byte-identical stream outputs across two independent generator runs spanning the reset commit.
  4. One-time: from this reset onward `--check` is a true byte-compare; any future baseline change is a fresh ruling, never a precedent.
- Modes — BOTH are hermetic; no mode uses committed fixtures as transient engine state: every generation run (bare and `--check`) builds a temporary repo-shaped root with PATH-PRESERVING copies of exactly the scripts the manifest argv reference at their verbatim relative locations — `tmp/referee/tests/stub_engine.py`, `tmp/referee/bots/fraction_bot.py`, `tmp/referee/bots/random_bot.py` (the census-complete set; the copy list is derived from the manifest argv, so a future row addition extends it automatically) — and spawns every engine with cwd = that tmp root, so unchanged argv (including the six marker paths under `referee/tests/fixtures/`) resolve INSIDE the tmp tree. All logs/streams/sidecars are written there. Bare mode then PROMOTES only the exact expected allowlist into the committed tree after a successful run; `--check` promotes nothing and byte-compares. The committed tree stays byte- and inventory-unchanged during every run (bare mode changes it only via the post-success allowlist promotion), asserted by a before/after tree-hash test around `--check`. A dedicated test runs one manifest row's UNCHANGED argv from the tmp root to prove the layout.
- Inventory comparison (bidirectional, complete, no dotfile blanket-ignore): expected = the 19 event logs + the 38 stream files + exactly the two allowlisted sidecars, literally:
  `referee/tests/fixtures/03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.X.stderr`
  `referee/tests/fixtures/03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.O.stderr`
  plus the manifest itself. The six manifest-enumerated marker paths must be ABSENT at comparison time (cleaned by the run); any other unexpected or missing path — dotfile or not — fails. Non-allowlisted sidecars produced by generation are pruned pre-compare and never checked in.

- [ ] **Step 1: Failing tests** —
```python
def test_stream_mapping_is_bijective_over_all_logs():
    # apply the pinned mapping to all 19 committed event-log paths: exactly 38 distinct
    # stream paths, all existing on disk, and no stream file exists outside the mapping
def test_every_stream_file_round_trips_through_referee_reader():
    # the reader IS poorman_referee.gamelog.read_log (the existing in-scope JSONL
    # reader — no new production reader): read_log(stream_path) yields the objects;
    # dispatch on obj["type"] ("hello"/"turn"/"game_end"), validate each against
    # docs/protocol/schema/{hello_request,turn_request,game_end}.schema.json,
    # AND canonical_dumps(obj) + "\n" byte-equals the corresponding original line
def test_log_corpus_globs_and_stream_glob_are_disjoint_and_exact():
    # the two pinned event-log globs (fixtures/*.jsonl, fixtures/parity-*/*.jsonl) match
    # all 19 logs and zero stream files; engine-stdin/**/*.jsonl matches all 38 streams
def test_generator_check_mode_passes_on_committed_artifacts():
    # subprocess: gen_fixtures.py --check exits 0 against the committed tree
def test_check_mode_leaves_committed_tree_untouched():
    # hash every file under referee/tests/fixtures/ before and after --check:
    # identical trees, no transient paths left behind (hermeticity, MR2-C)
def test_matrix_manifest_is_the_runtime_source():
    # mutate one manifest row's seed in a tmp copy and point the generator at it:
    # --check against the committed corpus now FAILS (proves the manifest, not the
    # committed logs, drives generation)
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
