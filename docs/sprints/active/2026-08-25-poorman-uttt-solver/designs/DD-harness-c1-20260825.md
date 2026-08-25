# DD-harness-c1-20260825 — Harness design: interop protocol v1, game-log schema v1, referee/runner architecture

DESIGN_DOC_ID: DD-harness-c1-20260825
GRILL_REQUIRED: yes (mirrored from the addressed DESIGN dispatch; GRILL_LOCK embedded in §12)
Status: awaiting DESIGN-REVIEW by harness.implementer
Inputs of record: DESIGN dispatch `s1/harness-c1/DESIGN-orchestrator-planner-20260825-065713.md`; RECONCILE.md §c1-audits; both harness-c1 audit returns; operator rulings R1–R4 (spec @ a3f250c) and R5 (grill, 2026-08-25).

## 1. Scope and fixed inputs

This design settles the harness domain for s1: the versioned engine interop protocol, the replayable game-log schema, and the referee / baseline-bot / tournament-runner architecture.

Fixed inputs, not design questions:
- R1: first-move tie owner hidden; `tie_owner` is `null` at ply 0 only; referee coin consulted only on an actual first-move bid tie.
- R2: faulted submission → opponent wins the auction at no cost and the opponent's intended move is applied; double-fault → re-request, 3 attempts total; three consecutive double-faults → game voided, excluded from strength stats.
- R3: bids are integers in fixed-point units, `0 ≤ bid ≤ own budget` inclusive; zero and all-in legal.
- R4: integers are canonical for all referee facts and everything on the wire; doubles live only inside engine analysis values.
- R5 (operator, grill 2026-08-25): the browser bridge is fully deferred from the v1 lock; v1 is engine-vs-engine; s1 ui narrows to replay + analysis-from-logs; play-vs-engine arrives later as an additive protocol supplement.

## 2. Protocol v1 — transport and framing

- JSONL over stdin/stdout: UTF-8, exactly one JSON object per LF-terminated line, no embedded newlines, no pretty-printing.
- Engines MUST flush stdout after every protocol line (normative MUST; the classic third-party failure mode).
- stdout is protocol-only; any non-protocol stdout line is a fault. stderr is free-form diagnostics: never parsed, captured to a per-engine file, capped at 64 KiB per game (excess truncated, noted in the log).
- Maximum protocol line length: 32 KiB in both directions; oversize is a fault.
- Within protocol major 1, readers MUST ignore unknown object keys (forward compatibility); a missing or type-invalid required key fails closed as a fault.
- All monetary values are integers in fixed-point units (10^9 = starting budget); all fit IEEE-754 doubles exactly (< 2^53), so plain JSON numbers are safe in every language.

## 3. Protocol v1 — message shapes

Exactly three referee→engine types (`hello`, `turn`, `game_end`) and two engine→referee types (`hello`, `turn`).

### 3.1 Canonical-marks rule

Everything on the wire uses canonical marks `"X"` and `"O"` — board cells, budget keys, `tie_owner` values.
The engine learns its own seat once from `you` and derives "mine/theirs" itself.
This supersedes the spec sketch's `you`/`opp` budget keys (the sketch is illustrative; the schema is harness-owned): one canonical frame kills the perspective-flip bug class and makes wire and log identical.

### 3.2 hello (once per game, before ply 0)

Referee → engine:
`{"type":"hello","protocol":1,"game_id":"<id>","you":"X","rules":"poorman-uttt-v1","time_ms":30000,"grace_ms":250,"budget":1000000000}`
Engine → referee (within `hello_timeout_ms`, default 10000):
`{"type":"hello","protocol":1,"name":"<engine>","version":"<semver-ish>","author":"<optional>"}`
- Unsupported protocol major from either side aborts before ply 0.
- A hello fault (timeout, malformed, wrong type) forfeits the game 0–1 for the faulting engine with reason `hello_fault`; both engines faulting hello voids the game (derived default extending R2's philosophy; see GRILL_LOCK).
- Engine startup cost (TT allocation, JIT) is absorbed here; there is no per-move warmup allowance (see GRILL_LOCK).

### 3.3 turn request (referee → both engines, written to both before awaiting either)

```json
{"type":"turn","protocol":1,"game_id":"g1","request_id":"g1-p0-a1","ply":0,"attempt":1,
 "you":"X","board":[".........",".........",".........",".........",".........",".........",".........",".........","........."],
 "forced":4,"legal":[[4,0],[4,1],[4,2],[4,3],[4,4],[4,5],[4,6],[4,7],[4,8]],
 "budgets":{"X":1000000000,"O":1000000000},"tie_owner":null,"time_ms":30000}
```
(Illustrative values; the normative transcript in `docs/protocol/` will carry a real game.)
- `board`: nine 9-character strings over `.`/`X`/`O`; local boards 0–8 row-major across the macro grid; cells 0–8 row-major within a local board.
- `forced`: integer 0–8 or `null` (free choice); ply 0 is always `"forced":4` (rule 5).
- `legal`: the complete enumeration of legal `[local, cell]` moves for this turn (CodinGame lesson; the single biggest afternoon-bar enabler).
- `tie_owner`: `"X"`, `"O"`, or `null`; `null` is valid only at ply 0 (R1). The sketch's `last_mover` field is dropped: it is derivable from `tie_owner` and carrying both invites disagreement.
- `attempt`: 1–3; increments only on R2 double-fault re-requests (same state, fresh `request_id`).
- `time_ms`: the engine's usable allowance for this turn (constant per game in v1).

### 3.4 turn reply (engine → referee)

`{"type":"turn","protocol":1,"request_id":"g1-p0-a1","bid":63410000,"move":[4,2],"info":{...}}`
- `request_id` must echo the request; `bid` per R3; `move` is ALWAYS required and must be a member of `legal`, even when the engine expects to lose the auction.
- `info` is optional (§4).

### 3.5 game_end (referee → both engines)

`{"type":"game_end","protocol":1,"result":"X","reason":"macro_win","budgets":{"X":312000000,"O":0}}`
- `result`: `"X" | "O" | "draw" | "void"`.
- `reason` enum: `macro_win`, `chip_count`, `exact_tie_draw`, `fault_forfeit`, `hello_fault`, `triple_double_fault_void`.
- After sending, the referee closes the engine's stdin, waits `shutdown_grace_ms` (default 2000), then kills the engine's whole process group.

## 4. Analysis carrier (operator-selected at grill)

- `info` is an optional object in the turn reply; the referee validates only its size (≤ 8 KiB) and JSON-ness, then copies it verbatim into the game log. Friend engines omit it at zero cost.
- Recommended keys our engine will emit: `t` (threshold, double), `critical_bid` (int units), `pv_if_win` / `pv_if_lose` (arrays of `[local, cell]`), `quality`: `"exact" | "bound" | "estimate"`.
- `quality` is REQUIRED whenever `info` is present (the engine-c1 value-quality obligation); replies with `info` lacking `quality` log a warning but are not faults (analysis is advisory, never game-affecting).
- Replay and live analysis read the same logged shape; no separate analysis request type exists in v1 (rejected at grill).

## 5. Faults and R2 mechanics

Fault classes (each a named enum used on the wire, in logs, and as a conformance fixture): `timeout`, `eof_or_crash`, `invalid_utf8`, `invalid_json`, `schema_violation`, `wrong_request_id`, `extra_protocol_line`, `oversize_line`, `illegal_bid`, `illegal_move`.
- Both sealed submissions are validated independently and completely BEFORE any bid comparison; result validity never depends on inspecting the opponent's secret bid.
- Single fault: the legal opponent wins the auction at cost 0, their intended move is applied, nothing is paid, the faulter's raw reply and fault class are logged. The legal player's bid is not revealed on the wire mid-game (it appears in the post-game log like all bids).
- Double fault: the identical auction state is re-requested with a fresh `request_id` and `attempt+1`, up to 3 attempts total; the third consecutive double-fault voids the game (`result:"void"`, excluded from W/D/L, counted in reliability stats).
- Sealed-bid integrity invariant: retries occur only on double-faults, so a legal player's sealed bid never leaks (spec rule 3).

## 6. Clock semantics

- Monotonic clock, per engine, per turn: the window opens when the referee has completely written and flushed that engine's request line and closes when a complete reply line has been received.
- Allowance is `time_ms`; a fixed transport grace `grace_ms` (default 250, configured and logged in the game header) is added on top; exceeding `time_ms + grace_ms` is a `timeout` fault.
- No move-1 warmup: startup is amortized in the hello phase (GRILL_LOCK).
- Elapsed milliseconds per reply are recorded in the log for every submission.

## 7. Determinism and pairing

- The referee owns all randomness. Per-game seed = SHA-256(tournament_seed, game_id), recorded in the log header.
- The first-move coin is consulted lazily (R1) from the game seed; its outcome, when consulted, is logged in that auction's resolution.
- Paired games: each matchup plays games in pairs with seats swapped; the pair shares a base seed and the second game inverts the coin mapping, so a first-move tie (if it occurs in both) favors opposite engines.
- Replays consume logged outcomes and never re-roll.

## 8. Game-log schema v1 (JSONL, append-only, one file per game)

Event types: `game_start`, `auction` (one per resolved ply), `game_end`.
- `game_start`: `log_version:1`, `protocol:1`, `rules:"poorman-uttt-v1"`, `game_id`, optional `tournament_id`, `seed`, `players` (per seat: name/version from hello, command line), `time_control` (`time_ms`, `grace_ms`, `hello_timeout_ms`), initial `budgets`.
- `auction`: `ply`; `attempts` array (per attempt: `request_id`, per-seat submission record: raw `bid`, `move`, `info`, `elapsed_ms`, `validation` = `ok` or fault class); `resolution` (`winner`, `reason`: `higher_bid | tie_last_mover | tie_coin | fault`, `coin` when consulted, `payment`, applied `move`, `closures` array of `{local, result: "X"|"O"|"full"}`, `macro_line` when the game ends by macro win, `forced_next`); `budgets_after`; `post_board` (nine strings).
- `game_end`: `result`, `reason`, `budget_margin`, `plies`.
- Sufficiency contract: the ui replays every frame from the log alone with zero re-execution — both sealed bids, both intents, resolution reason including tie/coin, payments, post-budgets, timing, terminal reason including all R2 outcomes, `post_board` snapshots, integer units, and a version field (ui needs 1–8, all satisfied).
- Rejected: an event-chain integrity hash (H-I suggestion) — v1 logs are local files consumed by trusted local tools; deferred.

## 9. Referee and runner architecture (Python, stdlib-only)

Monorepo surface `referee/` (package) + `docs/protocol/` (spec docs):
- `rules.py` — pure rules core: board state, legality, closure, terminal detection, forced-board routing. Property-tested against theory's fixtures; no I/O.
- `protocol.py` — codec: schema validation (fail-closed), framing, fault classification. Table-driven from the same field tables the spec doc publishes.
- `procs.py` — engine supervisor: spawn per game in own process group, monotonic per-turn clocks, concurrent sealed collection, stderr capture/capping, process-group kill.
- `auction.py` — sealed-auction resolution implementing R1–R3 exactly (integer-only comparisons).
- `gamelog.py` — JSONL writer/reader for schema v1 (reader is shared with tests and any local tooling).
- `referee.py` — single-game loop: hello, turn cycle (write both → collect both → validate both → resolve → log), termination.
- `tournament.py` — runner: paired seat-swapped games, W/D/L + average budget-margin + reliability stats, per-game log files plus one JSON summary; sequential by default with optional `--jobs N` process parallelism; no SPRT, no ratings.
- `bots/` — baseline bots (spec list): `random` (uniform legal move, uniform bid), `zero` (always bid 0), `fraction` (fixed fraction of budget), `allin_tactical` (all-in on tactical wins). The `fraction` bot is written as the ~100-line stdlib reference client and doubles as executable documentation.
- One process per seat per game, restarted between games (crash/state isolation); each turn remains logically stateless because the full authoritative state is resent.

## 10. Deliverables this design binds PLAN to

1. `docs/protocol/poorman-uttt-protocol-v1.md` — normative spec: framing rules, field tables for all five message shapes, fault taxonomy, clock semantics, one complete annotated game transcript.
2. `docs/protocol/schema/` — machine-readable JSON Schema files for every message and log event type.
3. `referee/` package as in §9, with pytest suites: codec round-trip; rules-vs-theory-fixtures; fault-matrix E2E driving stub engines that commit each named fault; determinism (same seed → byte-identical logs); log-sufficiency (reconstruct frames from log alone).
4. Conformance fixtures: one success transcript plus one fixture per fault class, runnable against any client.
5. Interpreter/env pinning (uv-managed, Python ≥3.12) per the reconciliation's toolchain obligation.

## 11. Boundary contracts

- Provides: protocol spec v1 + JSON Schemas + log schema v1 (consumers: engine pair, friend engine, ui replay/analysis, tournament regression). Changes route through s1.orchestrator-planner.
- Consumes: theory-owned rules fixtures. Shared convention to align with theory (via orchestrator): coordinates `[local 0–8, cell 0–8]` row-major, `forced` as integer-or-null — flagged in the design-completion report.
- Not provided in v1 (R5): browser bridge; any network transport; SPRT; generic tournament platform.

## 12. GRILL_LOCK

```text
GRILL_LOCK_ID: GRILL-harness-c1-20260825
GRILL_REQUIRED: yes
GRILL_SOURCE:
- plan/design/audit relay read: DESIGN dispatch harness-c1 (065713); RECONCILE.md §c1-audits; AUDIT-planner-20260825-063841; AUDIT-pair-implementer-20260825-063616; ui-c1 AUDIT-planner-20260825-063204 needs 1-8
- code/docs inspected: design spec @ a3f250c (rules 3-4, numeric policy); repo census (greenfield)
- questions answered from codebase/dispatch: framing, board grammar, lifecycle, seat identity, clock measurement, legal-action enumeration, game-end notification, stderr policy, seeded determinism, paired swaps (all pre-settled by dispatch/rulings/audit convergence)
- questions asked operator: 2 (analysis carrier; bridge scope)

Resolved decisions:
- Analysis carrier — optional `info` object in the turn reply, referee logs verbatim with required `quality` tag — one message shape, zero cost to friend engines, replay and live analysis share it — source operator (grill Q1)
- Bridge scope — fully deferred from v1 lock; v1 is engine-vs-engine; s1 ui narrows to replay+analysis-from-logs; bridge is a later additive supplement — source operator (grill Q2; recorded as R5)
- Handshake — single hello/hello exchange before ply 0 — afternoon-bar tiebreak per dispatch; engine name/version is required by the roadmap's regression tracking and must ride some message — source dispatch tiebreak + roadmap line 98 (default)
- Warmup — no per-move warmup; startup absorbed by hello_timeout (10 s) — keeps every turn's clock identical and fair — source default (derived; CodinGame's first-turn bonus rejected as a second clock rule)
- Wire frame — canonical X/O marks everywhere plus a `you` field; drop `last_mover` in favor of explicit `tie_owner` — kills perspective-flip and off-by-one bug classes — source default (supersedes illustrative sketch; schema is harness-owned per spec)
- Log form — JSONL events (game_start/auction/game_end), one file per game — append-only, streamable, same shapes a future bridge would emit — source default (H-I recommendation adopted)
- hello fault handling — forfeit 0-1 with reason hello_fault; both-fault = void — extends R2's philosophy to the setup phase — source default (derived)

Rejected alternatives:
- Separate analysis request type — second message shape + new clock semantics every engine author sees — rejected at grill Q1
- Referee log enrichment (referee-computed analysis) — duplicates engine search post-hoc — rejected at grill Q1
- WebSocket or HTTP+SSE bridge in v1 — mooted by R5 bridge deferral — rejected at grill Q2
- Pure stateless (no hello) — loses engine identity/version needed for regression logs and pre-game version failure — rejected by dispatch tiebreak
- Event-chain hash in log — integrity machinery without a v1 threat model — rejected as YAGNI
- SPRT / ratings / network transport / generic platform — dispatch v1 scope gate — rejected

Still operator-owned:
- none — R1-R5 cover every product-semantic item surfaced by both audits and the ui routing

Design-lock impact:
- DESIGN_LOCK_ID must reference this GRILL_LOCK_ID; R5 must be reported to s1.orchestrator-planner (ui gate narrows); the coordinate/forced convention needs an orchestrator-routed ack from theory
```

## 13. Risks

- The `legal` enumeration makes requests the largest lines on the wire (~70 moves × 8 bytes ≈ 0.6 KiB — well under the 32 KiB cap); acceptable.
- R2 re-requests give engines a second look at the same state; engines with nondeterministic search may answer differently — this is by ruling (R2) and logged per attempt.
- Exact bid ties are strategically reachable: `auction.py` compares integers only (R4) and the tie fixtures from theory must include tie-at-nonzero and tie-at-zero (zugzwang alternation) cases.
- If theory's fixture conventions diverge from §11's coordinate convention, engine and harness tests fork — mitigated by routing the convention through the orchestrator before PLAN.
