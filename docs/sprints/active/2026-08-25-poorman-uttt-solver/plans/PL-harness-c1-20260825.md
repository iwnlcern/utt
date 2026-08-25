# Harness v1 (protocol, referee, bots, tournament) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

PLAN_LOCK_ID: PL-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825 @ sha256 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440
GRILL_LOCK_ID: GRILL-harness-c1-20260825

**Goal:** Implement the approved harness v1: JSONL interop protocol spec + schemas, stdlib-only Python referee with R1–R3/R5 semantics and total fault/recovery logging, four standalone baseline bots, and a paired tournament runner.

**Architecture:** Pure rules/auction/seed cores with no I/O; a codec that fail-closes into ten named fault classes; a process supervisor doing sealed concurrent collection; a game loop emitting a total, canonical JSONL log; a tournament layer pairing seat-swapped games with one pair-level coin bit. Everything per DD-harness-c1-20260825 (the DD is the normative contract; where this plan and the DD disagree, the DD wins and the disagreement is a plan bug to relay).

**Tech Stack:** Python ≥ 3.12, runtime stdlib-only; uv-managed project; dev-only deps: pytest, jsonschema (schema cross-checks in tests only).

**Spec:** docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md (read it in full before Task 1; §§2–9 define every field and enum this plan implements).

## Global Constraints

- Runtime imports: stdlib only (`json`, `subprocess`, `selectors`/`threading`, `hashlib`, `base64`, `time`, `os`, `signal`, `argparse`, `dataclasses`). Dev deps allowed in `[dependency-groups] dev` only.
- All money values are `int` fixed-point units; no float ever touches budgets, bids, payments, or comparisons (R3/R4).
- Wire and log use canonical marks `"X"`/`"O"` everywhere; canonical JSON = `json.dumps(obj, sort_keys=True, separators=(",", ":"), ensure_ascii=False)`.
- Fault classes, exactly these strings everywhere: `timeout`, `eof_or_crash`, `invalid_utf8`, `invalid_json`, `schema_violation`, `wrong_request_id`, `extra_protocol_line`, `oversize_line`, `illegal_bid`, `illegal_move`.
- Limits: protocol line ≤ 32768 bytes; `info` ≤ 8192 bytes (canonical-serialized); raw capture ≤ 4096 bytes before base64; stderr capture ≤ 65536 bytes/game.
- Defaults: `time_ms=30000`, `grace_ms=250`, `hello_timeout_ms=10000`, `shutdown_grace_ms=2000`.
- Coordinates: `[local, cell]`, both 0–8 row-major; `forced` is int 0–8 or `null`; board is nine 9-char strings over `.XO`.
- Every commit lands on branch `harness-c1-impl`, created from the main HEAD current AT DISPATCH TIME (main moves fast in this sprint — the PLAN relay's `BASE` names the value checked at reissue; a dispatch-time rebase onto current main plus a fresh no-collision `SCOPE_DIFF` is an explicit precondition of the delegated `DISPATCH IMPL`, never a silent float). Implementer commits after each green test cycle.
- Tie-owner transition (R1/spec rule 4, pinned): after EVERY applied mark — auction win, tie win, or fault-applied opponent move — `tie_owner' = opponent(actual mover)`. There is no other transition; ply 0 starts at `None`.
- Out of scope (hard): browser bridge or any network transport (R5), SPRT/ratings, generic tournament platform, engine adapter changes (engine-owned), theory fixture authoring (theory-owned), any edit outside `referee/`, `docs/protocol/`, and this sprint tree.

## File Structure

```
referee/
  pyproject.toml                 # uv project "poorman-referee", requires-python >=3.12
  poorman_referee/
    __init__.py
    rules.py                     # pure UTTT rules core (no I/O)
    auction.py                   # sealed-auction resolution R1-R3 (pure)
    seeds.py                     # engine ids, pair/game seeds, pair coin (pure)
    protocol.py                  # codec: build requests, parse/validate replies, fault classification
    gamelog.py                   # canonical JSONL event writer/reader + replay-sufficiency reader
    procs.py                     # engine subprocess supervisor + sealed concurrent collection
    referee.py                   # single-game loop (hello, turns, faults/recovery, game_end, logging)
    tournament.py                # paired runner, stats, --jobs
    cli.py                       # `python -m poorman_referee` entrypoints: play, tourney
    __main__.py
  bots/                          # standalone stdlib scripts, NO package imports (each is a sample client)
    random_bot.py
    zero_bot.py
    fraction_bot.py              # ~100 lines, the normative reference client
    allin_tactical_bot.py
  tests/
    test_rules.py  test_auction.py  test_seeds.py  test_protocol.py
    test_gamelog.py  test_procs.py  test_referee_e2e.py  test_tournament.py
    test_schemas.py  test_fixture_loader.py
    stub_engine.py               # scriptable misbehaving client for fault-matrix E2E
    fixtures/                    # conformance fixtures (success + per-fault + recovery + parity)
  poorman_referee/fixture_loader.py  # theory-fixture → wire naming map (h→tie_owner, {x,o}→{X,O})
docs/protocol/
  poorman-uttt-protocol-v1.md    # normative spec (field tables, fault taxonomy, clock, transcript)
  schema/*.schema.json           # hello_request, hello_reply, turn_request, turn_reply, game_end,
                                 # log_game_start, log_auction, log_recovery, log_game_end
  transcript-v1.jsonl            # annotated real game transcript (generated, then committed)
```

---

### Task 1: Project scaffold

**Files:**
- Create: `referee/pyproject.toml`, `referee/poorman_referee/__init__.py`, `referee/tests/test_smoke.py`

**Interfaces:**
- Produces: importable package `poorman_referee`; `uv run --project referee pytest referee/tests` as the test command for every later task.

- [ ] **Step 1: Write pyproject**

```toml
[project]
name = "poorman-referee"
version = "0.1.0"
requires-python = ">=3.12"
dependencies = []

[dependency-groups]
dev = ["pytest>=8", "jsonschema>=4"]

[tool.pytest.ini_options]
testpaths = ["tests"]
```

- [ ] **Step 2: Smoke test** — `def test_import(): import poorman_referee` in `referee/tests/test_smoke.py`; run `uv run --project referee pytest referee/tests -v`; expect PASS.
- [ ] **Step 3: Commit** — `git add referee && git commit -m "harness: scaffold poorman-referee project"`.

### Task 2: rules.py — pure rules core

**Files:**
- Create: `referee/poorman_referee/rules.py`, Test: `referee/tests/test_rules.py`

**Interfaces:**
- Produces (exact):

```python
Move = tuple[int, int]                      # (local, cell), each 0-8
@dataclass(frozen=True)
class Position:
    board: tuple[str, ...]                  # nine 9-char strings over ".XO"
    forced: int | None                      # None = free choice
@dataclass(frozen=True)
class Closure:
    local: int
    result: str                             # "X" | "O" | "full"
@dataclass(frozen=True)
class ApplyResult:
    position: Position                      # with forced updated for the NEXT turn
    closures: tuple[Closure, ...]           # closures caused by this move (0 or 1 in v1 rules)
    macro_line: tuple[int, int, int] | None # set iff this move completes macro 3-in-a-row
    terminal: str | None                    # None | "macro_win" | "all_closed"
INITIAL = Position(board=("." * 9,) * 9, forced=4)
def local_status(cells: str) -> str | None  # None=open, "X"/"O"=won, "full"=drawn-full
def legal_moves(pos: Position) -> list[Move]
def apply_move(pos: Position, move: Move, mark: str) -> ApplyResult
```

- [ ] **Step 1: Failing tests** — hand-authored golden cases (never derived from the implementation): initial position has exactly the nine center-board moves; a won local board is closed even with empty cells; send-to-closed board yields free choice (`forced=None` and legal spans all open boards); a move that closes its own target board still routes by its cell index; macro win detected on all 8 macro lines (parametrize); `all_closed` when the ninth board closes without a macro line; `local_status` on won/full/open samples; move into an occupied cell absent from `legal_moves`.

```python
def test_initial_moves_are_center_board():
    assert sorted(legal_moves(INITIAL)) == [(4, c) for c in range(9)]

def test_send_to_closed_board_frees_choice():
    b = ["........."] * 9
    b[0] = "XXX......"                       # local 0 won by X
    pos = Position(board=tuple(b), forced=None)
    r = apply_move(pos, (4, 0), "O")         # cell 0 routes to closed local 0
    assert r.position.forced is None
```

- [ ] **Step 2: Run, verify FAIL.**  
- [ ] **Step 3: Implement** — precompute the 8 win lines; `local_status` scans lines then fullness; `legal_moves` = forced board's empty cells, else all empty cells of open boards; `apply_move` places the mark, computes closure of the target local board, macro line over per-board statuses, `all_closed` when no board open, `forced_next = move.cell if local_status(board[cell]) is None else None`.
- [ ] **Step 4: Run, verify PASS.**  
- [ ] **Step 5: Commit** — `"harness: rules core"`.

### Task 3: auction.py — sealed resolution R1–R3

**Files:**
- Create: `referee/poorman_referee/auction.py`, Test: `referee/tests/test_auction.py`

**Interfaces:**
- Consumes: `Move` from rules.
- Produces (exact):

```python
@dataclass(frozen=True)
class Submission:
    fault: str | None                       # None = valid; else a fault-class string
    bid: int | None
    move: Move | None
@dataclass(frozen=True)
class Resolution:
    winner: str                             # "X" | "O"
    reason: str                             # "higher_bid" | "tie_last_mover" | "tie_coin" | "fault"
    coin: str | None                        # seat, set iff reason == "tie_coin"
    payment: int                            # units paid by winner (0 on fault wins)
def resolve(sub_x: Submission, sub_o: Submission,
            tie_owner: str | None, pair_coin_seat: str) -> Resolution | None
    # None = double fault (caller re-requests). Ply-0 tie (tie_owner None) consults pair_coin_seat.
```

- [ ] **Step 1: Failing tests** — higher bid wins and pays own bid; equal bids won by `tie_owner` paying the tied amount (loser pays nothing); ply-0 equal bids (`tie_owner=None`) won by `pair_coin_seat` with `reason="tie_coin"`, `coin` set; 0–0 tie follows the same tie rules (zugzwang alternation); single fault → opponent wins, `reason="fault"`, `payment=0`; double fault → `None`; integer-exact: bids `10**9` vs `10**9 - 1`.
- [ ] **Step 2: FAIL. Step 3: Implement (pure integer comparisons only). Step 4: PASS. Step 5: Commit** `"harness: auction resolution R1-R3"`.

### Task 4: seeds.py — identity, seeds, pair coin

**Files:**
- Create: `referee/poorman_referee/seeds.py`, Test: `referee/tests/test_seeds.py`

**Interfaces:**
- Produces (exact):

```python
SEP = b"\x1f"
def validate_engine_ids(ids: list[str]) -> None       # raises ValueError on dupes or "\x1f" in any id
def pair_order(id1: str, id2: str) -> tuple[str, str] # lexicographic by UTF-8 bytes
def pair_seed(tournament_seed: str, engine_a: str, engine_b: str, round_: int) -> bytes
    # sha256(utf8(seed) + SEP + utf8(a) + SEP + utf8(b) + SEP + ascii(round)).digest()
def game_seed(pair_seed: bytes, k: int) -> bytes       # sha256(pair_seed + bytes([k])).digest(), k in {1,2}
def pair_coin_seat(pair_seed: bytes) -> str            # "X" if pair_seed[0] % 2 == 0 else "O"
```

- [ ] **Step 1: Failing tests** — known-vector test (compute one digest by hand in the test with `hashlib` composed independently, not by calling the function); `pair_order` on tricky UTF-8; duplicate/0x1F rejection; both-parity: craft two `(tournament_seed, ids, round)` triples found by brute force in the test setup so `pair_seed[0]` is even for one and odd for the other, assert `pair_coin_seat` differs; determinism (same inputs, same bytes).
- [ ] **Steps 2–5: FAIL → implement → PASS → commit** `"harness: seed and pair-coin derivation"`.

### Task 5: protocol.py — codec and fault classification

**Files:**
- Create: `referee/poorman_referee/protocol.py`, Test: `referee/tests/test_protocol.py`

**Interfaces:**
- Consumes: `Move`, `Position`, `legal_moves` from rules.
- Produces (exact):

```python
FAULTS = ("timeout","eof_or_crash","invalid_utf8","invalid_json","schema_violation",
          "wrong_request_id","extra_protocol_line","oversize_line","illegal_bid","illegal_move")
MAX_LINE, MAX_INFO, MAX_RAW = 32768, 8192, 4096
def canonical_dumps(obj) -> str
@dataclass(frozen=True)
class ParsedReply:
    validation: str                          # "ok" or a fault class
    bid: int | None; move: Move | None; info: dict | None
    raw: bytes | None                        # first MAX_RAW bytes iff validation != "ok"
    raw_total: int | None; raw_truncated: bool | None
@dataclass(frozen=True)
class ParsedHello:
    validation: str; name: str | None; version: str | None
    raw: bytes | None; raw_total: int | None; raw_truncated: bool | None
def hello_request(*, game_id: str, you: str, time_ms: int, grace_ms: int, budget: int) -> dict
def turn_request(*, game_id: str, request_id: str, ply: int, attempt: int, you: str,
                 pos: Position, budgets: dict[str, int], tie_owner: str | None, time_ms: int) -> dict
def game_end_msg(*, result: str, reason: str, budgets: dict[str, int]) -> dict
def parse_turn_reply(raw: bytes, *, request_id: str, budget: int, legal: list[Move]) -> ParsedReply
def parse_hello_reply(raw: bytes) -> ParsedHello
def to_submission(p: ParsedReply) -> "auction.Submission"
```

Validation order in `parse_turn_reply` (fail-closed, first hit wins): oversize → UTF-8 decode → JSON parse → required keys/types + `type=="turn"` + `protocol==1` (`schema_violation`; unknown keys ignored) → `request_id` echo → bid is int in `[0, budget]` (`illegal_bid`; booleans are not ints) → move in `legal` (`illegal_move`) → info: object, canonical size ≤ MAX_INFO else `schema_violation`; missing/odd `quality` inside info never faults.

- [ ] **Step 1: Failing tests** — one test per fault class with a crafted raw line (e.g. `b"\xff\xfe"` → `invalid_utf8`; `b"{"` → `invalid_json`; `bid=True` → `illegal_bid`; bid over budget → `illegal_bid`; move not in legal → `illegal_move`; 33 KB line → `oversize_line`; wrong echo → `wrong_request_id`); ok-path with and without `info`; unknown top-level key tolerated; raw capture fields populated exactly on faults (`raw_total`, truncation at 4096); `turn_request` golden dict matches DD §3.3 field-for-field; canonical_dumps stable ordering.
- [ ] **Steps 2–5: FAIL → implement → PASS → commit** `"harness: protocol codec + fault taxonomy"`.

### Task 6: JSON Schemas + schema tests

**Files:**
- Create: `docs/protocol/schema/{hello_request,hello_reply,turn_request,turn_reply,game_end,log_game_start,log_auction,log_recovery,log_game_end}.schema.json`, Test: `referee/tests/test_schemas.py`

**Interfaces:**
- Consumes: codec builders from Task 5 (their outputs must validate).
- Produces: draft 2020-12 schemas mirroring DD §3/§8 field tables; tests are the drift guard between code and schema.

- [ ] **Step 1: Failing tests** — for each message type: the codec-built example validates; an example with an ADDED UNKNOWN key also validates (wire schemas keep `additionalProperties: true` — forward compatibility per DD §2, tested on every message family); failure mutations are ONLY missing-required-key and type-invalid-required-field; `turn_reply` schema accepts any object `info`; `log_auction` requires `outcome` enum `["resolved","voided","aborted_recovery_fault"]` and enforces BOTH halves of the iff (use `if/then/else`): `outcome=="resolved"` without `resolution` fails, and any non-resolved outcome WITH `resolution` fails; `log_recovery` requires `seat`, `ply`, `trigger_request_id`, `fault`, and the restart `hello_record`.
- [ ] **Steps 2–5: write schemas → PASS → commit** `"harness: protocol + log JSON Schemas"`.

### Task 7: gamelog.py — total event log + sufficiency reader

**Files:**
- Create: `referee/poorman_referee/gamelog.py`, Test: `referee/tests/test_gamelog.py`

**Interfaces:**
- Consumes: `ParsedReply`, `ParsedHello`, `canonical_dumps`, `Resolution`, `Closure`.
- Produces (exact):

```python
def turn_record(p: ParsedReply, elapsed_ms: int) -> dict     # b64 raw block iff fault (DD §8)
def hello_record(p: ParsedHello, elapsed_ms: int) -> dict
class GameLogWriter:
    def __init__(self, fh): ...
    def emit(self, event: dict) -> None                      # canonical line + "\n", flush
def read_log(path) -> list[dict]
@dataclass(frozen=True)
class Frame:                                     # one UI replay frame — LOSSLESS projection of a ply
    ply: int
    board: tuple[str, ...]                       # post_board
    budgets: dict[str, int]                      # budgets_after
    forced: int | None                           # forced_next
    outcome: str                                 # "resolved" | "voided" | "aborted_recovery_fault"
    attempts: tuple[dict, ...]                   # every attempt: per-seat {validation, bid, move, info, elapsed_ms}
    resolution: dict | None                      # winner/reason/coin/payment/move/closures/macro_line
    recoveries: tuple[dict, ...]                 # ALL recovery events whose ply field equals this frame's
                                                 # ply, regardless of emission position: double-fault
                                                 # recoveries are emitted before their auction event,
                                                 # single-fault recoveries after it — association is by the
                                                 # recovery event's explicit `ply`/`trigger_request_id`
                                                 # fields (additive log-schema keys completing DD §8's
                                                 # totality goal; the DD's field list does not prohibit
                                                 # them and the wire/log contract ignores unknown keys)
@dataclass(frozen=True)
class Replay:
    start: dict                                  # game_start event (seeds, hellos, ids, time control)
    frames: tuple[Frame, ...]
    end: dict                                    # game_end event (result, reason, margins, delivery)
    events: tuple[dict, ...]                     # the raw ordered event timeline, lossless
def replay_frames(events: list[dict]) -> Replay  # from log alone; raises ValueError on any gap or missing field
```

- [ ] **Step 1: Failing tests** — event round-trip through writer/reader is byte-stable; `turn_record` on an `invalid_json` ParsedReply carries `{"b64":..., "truncated":false, "bytes_total":...}` and no parsed fields; `hello_record` carries name/version on ok and raw on fault; `replay_frames` log-only tests over five hand-built event lists, each asserting the CONSUMER-VISIBLE fields (not just schema validity): (a) clean ply — both bids/moves/info/elapsed present in `attempts`, tie payment reflected in budgets; (b) double-fault retry — two attempts retained with per-seat validations; (c) single-fault recovery — the recovery event is emitted AFTER its auction event yet `recoveries` on that SAME ply's frame carries the restart hello record (association by `ply`, not position); (c2) double-fault recovery — recovery events emitted BEFORE the auction event attach to the same frame; `Replay` additionally retains the raw ordered event timeline (`Replay.events: tuple[dict, ...]`) so no consumer is forced through the frame projection; (d) recovery abort — `outcome="aborted_recovery_fault"`, `resolution is None`; (e) terminal — `end` exposes result/reason/delivery. Plus destructive checks: removing `post_board`, an attempt record, or `game_end` each raises `ValueError` — the sufficiency guarantee is a test, not a comment.
- [ ] **Steps 2–5: FAIL → implement → PASS → commit** `"harness: game log writer/reader + replay sufficiency"`.

### Task 8: procs.py — supervisor and sealed collection

**Files:**
- Create: `referee/poorman_referee/procs.py`, Tests: `referee/tests/test_procs.py`, `referee/tests/stub_engine.py`

**Interfaces:**
- Consumes: codec from Task 5.
- Produces (exact):

```python
class Engine:
    def __init__(self, cmd: list[str], seat: str, *, shutdown_grace_ms: int = 2000,
                 stderr_cap: int = 65536, clock=time.monotonic): ...
    def start(self) -> None                    # Popen, start_new_session=True (own process group)
    def hello(self, req: dict, timeout_ms: int) -> tuple[ParsedHello, int]
    def send_line(self, obj: dict) -> str | None
        # PRE-SEND BOUNDARY (normative, in order): (1) non-blockingly drain ALL currently available
        # stdout bytes from the OS pipe into the framing buffer; (2) if the buffer then contains
        # even a single byte (complete OR partial line), return "extra_protocol_line" WITHOUT
        # writing — protocol bytes are never silently discarded; the stale output faults the
        # upcoming submission through the normal R2 path and recovery clears it; (3) otherwise
        # write the canonical line + flush and return None.
    def read_reply(self, deadline: float) -> tuple[bytes | None, str | None]
        # Bounded INCREMENTAL framing over a per-engine buffer:
        #   (raw, None)                    complete LF line, nothing else buffered
        #   (partial, "oversize_line")     buffer exceeded MAX_LINE before any LF (classified EARLY, pre-LF)
        #   (raw, "extra_protocol_line")   a second complete line was already buffered behind the reply
        #   (partial_or_None, "timeout" | "eof_or_crash") otherwise
        # On any fault status the buffer is drained and discarded; stale bytes NEVER reach a later request.
    def sweep_extra(self) -> bool              # final non-blocking drain sweep; True iff a late complete
                                               # protocol line arrived after read_reply returned
    def finish(self, game_end: dict) -> str    # best-effort terminal: send game_end (returns "ok"|"failed"),
                                               # close stdin, wait self.shutdown_grace_ms for exit, then killpg
    def kill(self) -> None                     # immediate killpg SIGKILL, reap, close stderr capture
def collect_both(engines: dict[str, Engine], reqs: dict[str, dict],
                 parse: Callable[[str, bytes | None, str | None], ParsedReply],
                 time_ms: int, grace_ms: int) -> dict[str, tuple[ParsedReply, int]]
    # COLLECTION BOUNDARY (normative): writes BOTH requests before reading either (a send_line
    # stale-bytes status faults that seat's submission immediately); two reader threads with
    # per-seat monotonic windows; after BOTH windows close and BEFORE returning (i.e. before the
    # auction can be resolved), calls sweep_extra() on each engine — a late-arriving complete line
    # downgrades that seat's submission to "extra_protocol_line". Late output is therefore observed
    # and faulted up to the resolution point, never silently dropped.
```

`stub_engine.py` (test-only, standalone stdlib): speaks the protocol correctly by default; misbehaves on command via argv, e.g. `--fault timeout:2` (sleep past deadline on ply 2), `bad_json:1`, `illegal_move:3`, `oversize:1`, `extra_line:2`, `die:2` (exit mid-turn), `hello_timeout`, `bid <int>|all_in|echo_legal0`; deterministic via `--seed`.

- [ ] **Step 1: Failing tests** — spawn stub, clean hello returns name/version and elapsed; `collect_both` returns both ok replies and measured windows; timeout stub → (`None`,"timeout") after `time_ms+grace_ms` (use tiny 200 ms budgets in tests); dead stub → `eof_or_crash`; oversize stub (`--fault oversize_nolf:1`, streams > 32 KiB with no LF) → `"oversize_line"` classified BEFORE any newline arrives; extra-line stub (`--fault extra_line:1`, same-write) → `"extra_protocol_line"` with the second line quarantined; delayed extra output is proven with two DETERMINISTIC schedules, not a race: (a) sweep-visible — the stub emits its extra line while its companion seat's reply is held open (stub `--fault extra_line_before_sweep:1` writes the extra line, then the OTHER stub is released), so the line exists in the pipe before `collect_both`'s final `sweep_extra()`, which downgrades the seat to `"extra_protocol_line"` on THAT auction; (b) post-sweep — the stub emits an unsolicited line only after the ply fully resolves (`--fault unsolicited_between_plies:1`), and the NEXT `send_line`'s pre-send drain pulls it from the OS pipe and returns `"extra_protocol_line"` without writing, faulting the upcoming submission; a partial-byte variant (stub writes `b"garbage-no-newline"` between plies) faults the same way — no test may depend on a line arriving during an instantaneous sweep; after any fault, the next request sees a clean buffer; `finish()` returns `"ok"` for a live engine and `"failed"` for a dead one, honors a CONFIGURED non-default `shutdown_grace_ms` (construct `Engine(..., shutdown_grace_ms=300)` and assert the wait), then the process group is gone; `kill()` leaves no child (poll returncode, check process group gone); stderr capped (stub `--spam-stderr`).
- [ ] **Steps 2–5: FAIL → implement → PASS → commit** `"harness: engine supervisor + sealed collection"`.

### Task 9: referee.py — single-game loop (the DD §5/§5.1 machine)

**Files:**
- Create: `referee/poorman_referee/referee.py`, Test: `referee/tests/test_referee_e2e.py`

**Interfaces:**
- Consumes: everything above.
- Produces (exact):

```python
@dataclass
class GameConfig:
    cmds: dict[str, list[str]]; engine_ids: dict[str, str]     # keyed "X"/"O"
    tournament_id: str | None; pair_seed: bytes; game_seed: bytes; pair_coin_seat: str
    time_ms: int = 30000; grace_ms: int = 250
    hello_timeout_ms: int = 10000; shutdown_grace_ms: int = 2000
    log_path: str; clock = time.monotonic
@dataclass(frozen=True)
class GameResult:
    result: str                               # "X" | "O" | "draw" | "void"
    reason: str                               # DD §3.5 enum
    budgets: dict[str, int]; plies: int
def play_game(cfg: GameConfig) -> GameResult
```

Engines are constructed with `Engine(cmd, seat, shutdown_grace_ms=cfg.shutdown_grace_ms, clock=cfg.clock)` — the configured grace flows from `GameConfig` into the supervisor, and the E2E graceful-shutdown assertion uses a non-default value.

Loop (all per DD, cite sections in code comments only where the code cannot show it): start engines → hellos (fault ⇒ `hello_fault` forfeit / both ⇒ void) → emit `game_start` → per ply: build per-seat requests, `collect_both`, resolve. ORDERING RULE (single fault): resolve → pay (0) → apply the legal opponent's move → update `tie_owner' = opponent(actual mover)` → detect terminal → emit `auction`; ONLY IF the game continues, recover the faulter (§5.1: kill, restart, re-hello, emit `recovery` before the next ply's events) — a fault-won terminal move ends the game and no recovery runs, so a restart failure can never rewrite an earned result. Double fault: nothing resolved, so recover BOTH seats first, then re-request up to attempt 3, third ⇒ `voided`; failed restart hello ⇒ `aborted_recovery_fault` + `recovery_fault` terminal (both ⇒ void). Normal resolution: apply move, pay, update budgets and `tie_owner' = opponent(actual mover)` (also after tie wins and 0-0 zugzwang ties), emit `auction` in causal order. Terminal: macro win / all closed (chip comparison: strict greater wins, equal ⇒ draw ½–½) → `Engine.finish(game_end)` best-effort to both, log per-seat delivery, process groups reaped by `finish`.

- [ ] **Step 1: Failing E2E tests (each also asserts its log validates against Task 6 schemas):** clean stub-vs-stub game completes with `macro_win` or `chip_count`; each of the ten fault classes on one seat → that ply resolved by `reason="fault"`, `payment=0`, opponent's move applied, `recovery` event present, game continues; fault-win TERMINAL case — the legal opponent's fault-won move completes a macro line while the faulter's restart is scripted to die (`--fault die_on_hello_after_restart`) → result is the macro win, NO `recovery` event exists, and `recovery_fault` appears nowhere; double fault → attempt 2 re-request visible in `auction.attempts`; triple double-fault → `outcome="voided"`, result `void`; kill-on-restart stub mid-game (non-terminal) → `aborted_recovery_fault` + `recovery_fault`; ply-0 bid tie with `--bid` equal stubs → `tie_coin` by `pair_coin_seat`; tie-owner transition multi-ply: consecutive paid wins by the SAME engine keep `tie_owner` constant at that winner's opponent (X moves twice → owner is O after both), a fault-applied move sets `tie_owner = opponent(mover)`, and a 0-0 zugzwang owner-wins chain ALTERNATES it (the owner is forced to move, so ownership flips each ply) — assert the `tie_owner` field in successive logged turn requests; budgets integer-exact after tie payment; `game_end` per-seat `delivery` is `"ok"` for live engines and `"failed"` for a dead one.
- [ ] **Step 2: FAIL. Step 3: Implement. Step 4: PASS (use 200–500 ms clocks). Step 5: Commit** `"harness: referee game loop with fault/recovery semantics"`.

### Task 10: baseline bots + determinism suite

**Files:**
- Create: `referee/bots/{random_bot,zero_bot,fraction_bot,allin_tactical_bot}.py`, Modify: `referee/tests/test_referee_e2e.py` (add determinism tests)

**Interfaces:**
- Produces: four standalone stdlib scripts (no `poorman_referee` imports — each is a working third-party-style client). Common CLI: `--seed <int>`; fraction: `--fraction-ppb <int>` (parts-per-billion of own budget, integer math); allin_tactical: bids all-in iff it has an immediate local-win or macro-win move, else 0. `fraction_bot.py` is the reference client: ≤ ~100 lines, heavily commented, reads turn requests, picks `legal[rng % len]`, bids `budget * ppb // 10**9`.
- [ ] **Step 1: Failing tests** — every bot completes a full game vs `zero_bot` under the real referee (1 s clocks); determinism: same `GameConfig` with fake `clock` (monotonic counter) + seeded bots run twice → byte-identical log files; both-parity pair fixtures from Task 4 drive two pairs asserting opposite-engine tie favoritism end-to-end (equal-bid stubs, ply-0 tie in both games of each pair).
- [ ] **Steps 2–5: FAIL → implement bots → PASS → commit** `"harness: baseline bots + determinism suite"`.

### Task 11: tournament.py + CLI

**Files:**
- Create: `referee/poorman_referee/tournament.py`, `referee/poorman_referee/cli.py`, `referee/poorman_referee/__main__.py`, Test: `referee/tests/test_tournament.py`

**Interfaces:**
- Produces (exact):

```python
@dataclass
class TournamentConfig:
    engines: list[dict]        # {"id": str, "cmd": list[str]}
    games_per_pair: int        # even; each pair alternates seats, shares pair_seed
    tournament_seed: str; time_ms: int; jobs: int = 1; out_dir: str
def run_tournament(cfg: TournamentConfig) -> dict   # summary (also written as JSON)
    # summary per engine: {"wins","draws","losses","avg_budget_margin","faults","voids"}
    # voids excluded from W/D/L, counted under reliability. Round-robin over engine pairs.
```

CLI: `python -m poorman_referee play --x-cmd ... --o-cmd ... --x-id A --o-id B --seed S --log PATH` and `python -m poorman_referee tourney --config tourney.json`.
- [ ] **Step 1: Failing tests** — 2-engine 2-game pair: seats swap between games, same `pair_coin_seat` logged in both `game_start`s; void games excluded from W/D/L; `--jobs 2` result equals `--jobs 1` result for the same seed (order-independent aggregation); summary JSON written and schema-sane.
- [ ] **Steps 2–5: FAIL → implement (`multiprocessing.Pool` for jobs) → PASS → commit** `"harness: tournament runner + CLI"`.

### Task 12: fixture loader seam (theory boundary)

**Files:**
- Create: `referee/poorman_referee/fixture_loader.py`, Test: `referee/tests/test_fixture_loader.py`

**Interfaces:**
- Consumes: theory fixture envelope per DD-theory-c1-20260825 (`state` with `board`, `forced` int-or-null, `h`, `budgets: {x, o}`; categories legality/closure/auction-trace/terminal).
- Produces: `def load_fixture(obj: dict) -> tuple[Position, str | None, dict[str, int]]` mapping `h`→`tie_owner`, `{x,o}`→`{"X","O"}`; `def iter_fixture_files(root) -> Iterator[dict]`.

- [ ] **Step 1: Failing tests** — the naming map on an inline sample matching theory's locked envelope verbatim; loader rejects unknown `forced` shapes. Then a consumption test: `for fx in iter_fixture_files("theory/fixtures")` run legality/closure fixtures through `rules.py`. The skip-when-absent marker is DEVELOPMENT-ONLY: the final acceptance report may not claim the loader-seam criterion at E2 while this test skips — if theory's fixtures have landed, the test must run green against them; if they have not, the implementation report states the boundary criterion as `pending/blocked on theory fixtures`, explicitly, instead of counting a green-with-skip suite as E2-complete (do not author theory fixtures here; theory owns them).
- [ ] **Steps 2–5: FAIL → implement → PASS → commit** `"harness: theory fixture loader seam"`.

### Task 13: normative spec doc, transcript, conformance fixtures

**Files:**
- Create: `docs/protocol/poorman-uttt-protocol-v1.md`, `docs/protocol/transcript-v1.jsonl`, `referee/tests/fixtures/*`

**Interfaces:**
- Consumes: everything; the doc's field tables must byte-match the Task 6 schemas (state the schema files as normative source and generate tables from them or cross-check in a test).

- [ ] **Step 1: Generate transcript** — run one seeded `fraction_bot` vs `random_bot` game with the real referee; commit the log as `transcript-v1.jsonl`; add a test that replays it through `replay_frames` and validates every event against the log schemas.
- [ ] **Step 2: Write the spec doc** — sections: transport & framing (flush MUST), message field tables (all five shapes), fault taxonomy + R2 mechanics + recovery, clock semantics, seat/tie/coin semantics, log schema v1, "write a client in an afternoon" walkthrough that reproduces `fraction_bot.py` step by step. Full sentences on their own lines.
- [ ] **Step 3: Conformance fixtures** — copy the fault-matrix E2E artifacts into `referee/tests/fixtures/` (one success transcript, one log per fault class, both recovery-fault dispositions, both pair-seed parities) with a small driver test asserting each fixture validates against the schemas and replays.
- [ ] **Step 4: Full suite green** — `uv run --project referee pytest referee/tests -v`; expect all PASS.
- [ ] **Step 5: E3 smoke** — `python -m poorman_referee tourney` with 4 baseline bots, 2 games/pair, `time_ms=30000` on the laptop; verify wall-clock per move ≤ 30 s enforced and summary sane; record the command + output in the PR body (E3 evidence).
- [ ] **Step 6: Commit** — `"harness: protocol v1 spec doc + transcript + conformance fixtures"`.

---

## Acceptance Criteria (from DD §10 and the approving review, verbatim obligations)

1. Exact `DESIGN_LOCK_ID` + digest locked (header above); GRILL-harness-c1-20260825 referenced.
2. Every §10 fixture present: per-fault-class (including the framing-level `oversize_line`-before-LF and `extra_protocol_line` cases), both recovery-fault dispositions (including the fault-win-terminal no-recovery case), both pair-seed parities, determinism byte-identity (stubs + fake clock + canonical serialization), lossless log-sufficiency replay (attempts/recoveries/terminal asserted field-by-field), loader seam at E2 — with the Task 12 honesty rule: a skipped theory-fixture integration is reported `pending/blocked`, never E2-complete.
3. Afternoon bar: spec doc + schemas + annotated transcript + reference bot + conformance fixtures.
4. Engine adapter delta remains orchestrator-routed (no engine/ edits from this plan); theory fixture schema consumed verbatim through the loader seam only.
5. E2 = full pytest suite green; E3 = tournament smoke with 30 s enforcement on a laptop.

## Boundary contract (unchanged from DD §11)

Writes: `docs/protocol/` spec + schemas, `referee/` package, game logs. Reads: theory fixtures (via Task 12 seam only). Consumers: engine pair, friend engine, ui (logs), regression stats. Changes route through s1.orchestrator-planner. Not built (R5): bridge, network, SPRT, platform.

## Anti-half-fix guards

- No fault class may be "handled" without its named E2E test and conformance fixture.
- `replay_frames` raising on gaps is the sufficiency contract; weakening it to warnings is a plan violation.
- Integer-only money: any `float` in budgets/bids/payments/comparisons fails review regardless of tests.

## Operator-judgment items

None — R1–R5 cover all product semantics; everything here is technical execution of the approved DD.
