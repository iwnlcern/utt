# DD-harness-c1-20260825 — Harness design: interop protocol v1, game-log schema v1, referee/runner architecture

DESIGN_DOC_ID: DD-harness-c1-20260825
GRILL_REQUIRED: yes (mirrored from the addressed DESIGN dispatch; GRILL_LOCK embedded in §12)
Status: revision 2 — M1–M7 and both consistency corrections from DESIGN-REVIEW `harness-c1-design-review-1` folded; awaiting re-review by harness.implementer
Inputs of record: DESIGN dispatch `s1/harness-c1/DESIGN-orchestrator-planner-20260825-065713.md`; RECONCILE.md §c1-audits; both harness-c1 audit returns; operator rulings R1–R4 (spec @ a3f250c) and R5 (grill, 2026-08-25); DESIGN-REVIEW `s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-073619.md` (fold ledger in §14).

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
- `reason` enum: `macro_win`, `chip_count`, `exact_tie_draw`, `hello_fault`, `recovery_fault`, `triple_double_fault_void`. (`fault_forfeit` removed: under R2 a turn fault loses only the auction, never the game; the reachable fault-terminal transitions are exactly `hello_fault` at setup and `recovery_fault` per §5.1.)
- Delivery is best-effort after the result is already terminally resolved: inability to deliver `game_end` to a crashed or faulted process never rewrites the decided result; delivery outcome is logged.
- After sending (or failing to send), the referee closes the engine's stdin, waits `shutdown_grace_ms` (default 2000), then kills the engine's whole process group.

## 4. Analysis carrier (operator-selected at grill)

- `info` is an optional object in the turn reply; the referee validates only its size (≤ 8 KiB) and JSON-ness, then copies it verbatim into the game log. Friend engines omit it at zero cost.
- Recommended keys our engine will emit: `t` (threshold, double), `critical_bid` (int units), `pv_if_win` / `pv_if_lose` (arrays of `[local, cell]`), `quality`: `"exact" | "bound" | "estimate"`.
- The public v1 reply schema accepts ANY size-capped JSON object as `info`; it is advisory and never game-affecting, so no `info` content can be a fault. `quality` is a conformance requirement on harness-owned analysis producers (our engine, reference tooling) — the engine-c1 value-quality obligation — and a reply whose `info` lacks or misuses `quality` logs a warning, nothing more. (Resolves the required-but-non-faulting contradiction: "required" binds our producers' conformance suite, not the public schema.)
- Replay and live analysis read the same logged shape; no separate analysis request type exists in v1 (rejected at grill).

## 5. Faults and R2 mechanics

Fault classes (each a named enum used on the wire, in logs, and as a conformance fixture): `timeout`, `eof_or_crash`, `invalid_utf8`, `invalid_json`, `schema_violation`, `wrong_request_id`, `extra_protocol_line`, `oversize_line`, `illegal_bid`, `illegal_move`.
- Both sealed submissions are validated independently and completely BEFORE any bid comparison; result validity never depends on inspecting the opponent's secret bid.
- Single fault: the legal opponent wins the auction at cost 0, their intended move is applied, nothing is paid, the faulter's raw reply and fault class are logged. The legal player's bid is not revealed on the wire mid-game (it appears in the post-game log like all bids).
- Double fault: the identical auction state is re-requested with a fresh `request_id` and `attempt+1`, up to 3 attempts total; the third consecutive double-fault voids the game (`result:"void"`, excluded from W/D/L, counted in reliability stats).
- Sealed-bid integrity invariant: retries occur only on double-faults, so a legal player's sealed bid never leaks (spec rule 3).

### 5.1 Post-fault process recovery

R2 lets a faulted engine keep playing, so the referee must guarantee a usable process for its next request:
- After ANY turn fault by a seat (uniform rule, all ten fault classes), the referee terminates that seat's process group, starts a clean process, and re-runs the hello exchange BEFORE that seat's next turn request. Uniform restart eliminates stale-computation/stale-output hazards (timeout leftovers, half-written lines) at the cost of the faulter's warm state — their fault, acceptable.
- Ordering on a SINGLE fault (amendment 1, 2026-08-25): the auction is resolved first — pay 0, apply the legal opponent's move, update tie owner, detect terminal, emit the `auction` event — and the faulter is recovered ONLY IF the game continues. A fault-won terminal move ends the game with no recovery attempt, so a restart failure can never rewrite an earned result (R2: a fault loses the auction, never retroactively the game).
- On a double-fault, both seats are recovered before the fresh `attempt+1` request is written.
- Every recovery is logged as a `recovery` event (§8) with the triggering fault class and the restart hello outcome.
- If the restart hello itself faults, the game cannot continue: the seat's opponent wins with `game_end` reason `recovery_fault`. If both restart hellos fault, the game is voided. (This is a process-liveness terminal, distinct from R2's per-auction fault semantics, which are unchanged.)
- §9's lifecycle rule is therefore: one LIVE process per seat at any time, started at game begin, restarted on fault recovery, always restarted between games.

## 6. Clock semantics

- Monotonic clock, per engine, per turn: the window opens when the referee has completely written and flushed that engine's request line and closes when a complete reply line has been received.
- Allowance is `time_ms`; a fixed transport grace `grace_ms` (default 250, configured and logged in the game header) is added on top; exceeding `time_ms + grace_ms` is a `timeout` fault.
- No move-1 warmup: startup is amortized in the hello phase (GRILL_LOCK).
- Elapsed milliseconds per reply are recorded in the log for every submission.

## 7. Determinism and pairing

- The referee owns all randomness. Identity and encoding pins:
  - Every engine in a tournament has a unique configured `engine_id` (UTF-8 string); the referee rejects duplicate ids and any config string containing byte `0x1F`.
  - Stable pair order: `engine_A` is the lexicographically smaller `engine_id` by UTF-8 byte order; `engine_B` the other.
  - `tournament_seed` is an arbitrary UTF-8 config string, hashed as its UTF-8 bytes.
- Seed derivation (`||` is byte concatenation, `0x1F` a literal separator byte, components delimiter-free by the config rule above):
  - `pair_seed = SHA-256(tournament_seed || 0x1F || engine_id_A || 0x1F || engine_id_B || 0x1F || ascii(round))`.
  - `game_seed_k = SHA-256(pair_seed || byte(k))` for game `k ∈ {1, 2}` of the pair (used for any future in-game randomness; the coin does NOT depend on it).
- Pair-level coin, expressed in seat terms with NO inversion anywhere: `pair_coin_seat = "X"` if `pair_seed[0]` is even, else `"O"`. In BOTH games of the pair, an actual first-move bid tie (lazy per R1) is won by `pair_coin_seat`.
- Fairness by construction: seats swap between the two games while `pair_coin_seat` stays fixed, so a first-move tie occurring in both games necessarily favors opposite ENGINES. Single (unpaired) games use round 1 and the same derivation.
- `game_start` logs `engine_id` per seat, `pair_seed`, `game_seed` (hex), and `pair_coin_seat`, so the mapping reproduces without guessing; replays consume logged outcomes and never re-roll.
- The determinism suite includes both-parity pair fixtures (one pair with `pair_seed[0]` even, one odd) asserting the opposite-engine property.

## 8. Game-log schema v1 (JSONL, append-only, one file per game)

Event types: `game_start`, `auction` (one per ply ATTEMPTED, resolved or not), `recovery`, `game_end`. The event model is total: every named fault class, hello outcome, recovery, and void maps to a schema-valid log, and the conformance suite proves it fixture-by-fixture.

Record shapes (two, distinct):
- `turn_record` (per seat, inside `auction` attempts): `validation` (`ok` or fault class); parsed `bid`, `move`, `info` present only when parseable; `elapsed_ms`; on any non-`ok` validation, `raw`: `{b64: <first 4 KiB of the raw bytes, base64>, truncated: bool, bytes_total: int}` — so invalid UTF-8, malformed JSON, EOF (empty capture), oversize, and extra protocol lines are all representable separately from nullable parsed fields.
- `hello_record` (per seat, used at startup in `game_start` and at restart in `recovery`): `validation` (`ok` or fault class); parsed `name`, `version` present only on `ok`; `elapsed_ms`; same bounded `raw` capture on any non-`ok` validation.

Events:
- `game_start`: `log_version:1`, `protocol:1`, `rules:"poorman-uttt-v1"`, `game_id`, optional `tournament_id`, per-seat `engine_id` and command line, `pair_seed`, `game_seed`, `pair_coin_seat`, per-seat startup `hello_record`, `time_control` (`time_ms`, `grace_ms`, `hello_timeout_ms`, `shutdown_grace_ms`), initial `budgets`.
- `auction`: `ply`; `outcome`: `"resolved" | "voided" | "aborted_recovery_fault"`; `attempts` array so far (per attempt: `request_id`, `attempt`, per-seat `turn_record`s); `resolution` — object present iff `outcome:"resolved"` (`winner`, `reason`: `higher_bid | tie_last_mover | tie_coin | fault`, `coin` when consulted, `payment`, applied `move`, `closures` array of `{local, result: "X"|"O"|"full"}`, `macro_line` when the game ends by macro win, `forced_next`), else absent; `budgets_after` and `post_board` (nine strings) equal the pre-state when not resolved.
- `recovery`: `seat`, `ply` (the triggering ply), `trigger_request_id`, triggering `fault` class, restart `hello_record` (amendment 1: the two association keys are normative so replay consumers attach recoveries to their ply regardless of emission position).
- `game_end`: `result`, `reason`, `budget_margin`, `plies`, per-seat `delivery`: `ok | failed`.

Causal emission order (normative; rewritten by amendment 1, 2026-08-25, to match §5.1's single-fault ordering): events are appended in wall-clock completion order; a `recovery` event is appended at the moment its restart hello concludes. Consequences:
- DOUBLE-fault recoveries are mid-ply (nothing resolved yet): their `recovery` events precede that ply's `auction` event, which is appended only when the ply concludes (resolved, voided, or aborted). When both seats recover, seat X's `recovery` is appended before seat O's.
- SINGLE-fault recoveries run after the ply resolved (§5.1): their `recovery` event FOLLOWS that ply's `auction` event and precedes the next ply's events; association is by the `recovery` event's `ply`/`trigger_request_id` keys, not by position.
- The `recovery_fault` terminal has two shapes: (a) double-fault path — restart hello fails mid-ply: attempts so far inside an `auction` event with `outcome:"aborted_recovery_fault"`, preceded by the failing `recovery` event(s), then `game_end` (`reason:"recovery_fault"`; result `"void"` when both restart hellos fail); (b) single-fault path — the ply's `auction` event is RESOLVED (the legal opponent's move stood), followed by the failing `recovery` event, then `game_end` (`reason:"recovery_fault"`).
- Sufficiency contract: the ui replays every frame from the log alone with zero re-execution — both sealed bids, both intents, resolution reason including tie/coin, payments, post-budgets, timing, terminal reason including all R2 outcomes plus `recovery_fault`, `post_board` snapshots, integer units, and a version field (ui needs 1–8, all satisfied).
- Canonical serialization: the log writer emits canonical JSON (UTF-8, sorted keys, compact separators) so equal event content is equal bytes.
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
- Process lifecycle: one LIVE process per seat at any time — started at game begin, restarted with re-hello on fault recovery (§5.1), always restarted between games (crash/state isolation); each turn remains logically stateless because the full authoritative state is resent.

## 10. Deliverables this design binds PLAN to

1. `docs/protocol/poorman-uttt-protocol-v1.md` — normative spec: framing rules, field tables for all five message shapes, fault taxonomy, clock semantics, one complete annotated game transcript.
2. `docs/protocol/schema/` — machine-readable JSON Schema files for every message and log event type.
3. `referee/` package as in §9, with pytest suites: codec round-trip; rules-vs-theory-fixtures (including the fixture-to-wire loader seam test, §11); fault-matrix E2E driving stub engines that commit each named fault, each producing a schema-valid log; determinism — same seed → byte-identical logs, scoped to deterministic scripted stub engines with an injected fake clock and the canonical log serialization (production logs keep real `elapsed_ms` and engine `info`, which seed equality cannot and need not make identical); log-sufficiency (reconstruct frames from log alone).
4. Conformance fixtures: one success transcript plus one fixture per fault class, plus recovery-fault terminal fixtures covering BOTH §8 shapes (double-fault mid-ply abort → `aborted_recovery_fault`, single-fault post-resolution restart failure → resolved auction + `recovery_fault`, and both-seat restart failure → void) and the both-parity pair-fairness fixtures from §7, runnable against any client.
5. Interpreter/env pinning (uv-managed, Python ≥3.12) per the reconciliation's toolchain obligation.

## 11. Boundary contracts

- Provides: protocol spec v1 + JSON Schemas + log schema v1 (consumers: engine pair, friend engine, ui replay/analysis, tournament regression). Changes route through s1.orchestrator-planner.
- Consumes: theory-owned rules fixtures. The `forced` encoding is ALIGNED by both owner records: theory's design (DD-theory-c1-20260825, fixture schema) locks `forced` as integer 0–8 or `null`, explicitly matching this wire convention, with the boundary acknowledgment routed through s1.orchestrator-planner on theory's side. Residual naming differences (fixture `h`/`budgets:{x,o}` vs wire `tie_owner`/`budgets:{X,O}`) live in one harness-owned fixture loader with a named E2 seam test (§10, deliverable 3).
- Consumer-alignment obligation (pre-PLAN, orchestrator-routed): the engine design's adapter boundary (DD-engine-rules-c1-20260825 §6) currently validates `last_mover` and serializes bare `{bid, move}` — divergent from this schema's hello/turn envelopes, canonical X/O budget keys, dropped `last_mover`/explicit `tie_owner`, and optional `info`. Harness owns the schema; the concrete adapter delta must be routed through s1.orchestrator-planner before either pair's PLAN treats the seam as locked. This obligation is named in the design-completion report.
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
- DESIGN_LOCK_ID must reference this GRILL_LOCK_ID; R5 must be reported to s1.orchestrator-planner (ui gate narrows); the coordinate/forced convention needs an orchestrator-routed ack from theory (theory's record now carries it — see §11); the engine adapter delta (§11) is a second orchestrator-routed pre-PLAN obligation
```

## 13. Risks

- The `legal` enumeration makes requests the largest lines on the wire (~70 moves × 8 bytes ≈ 0.6 KiB — well under the 32 KiB cap); acceptable.
- R2 re-requests give engines a second look at the same state; engines with nondeterministic search may answer differently — this is by ruling (R2) and logged per attempt.
- Exact bid ties are strategically reachable: `auction.py` compares integers only (R4) and the tie fixtures from theory must include tie-at-nonzero and tie-at-zero (zugzwang alternation) cases.
- If theory's fixture conventions diverge from §11's coordinate convention, engine and harness tests fork — mitigated by routing the convention through the orchestrator before PLAN (theory's record already matches; the loader seam test guards the residual naming map).
- Uniform post-fault restart (§5.1) costs the faulting engine its warm transposition table mid-game; a strength-affecting but fault-triggered penalty, accepted for supervisor simplicity and stale-state elimination.

## 14. Review-fold ledger (DESIGN-REVIEW harness-c1-design-review-1, 2026-08-25)

- M1 (fixture seam) — folded with a correction: theory's owner record (DD-theory-c1-20260825 fixture schema) already locks `forced` int-or-null aligned to this wire convention, so representation alignment path (a) holds by both records; the residual naming map got an explicit loader boundary + E2 seam test (§11, §10).
- M2 (post-fault recovery) — folded as recommended: §5.1 uniform restart + re-hello, both-seat recovery on double-fault, `recovery` log event, `recovery_fault` terminal; §9 lifecycle reworded.
- M3 (log totality) — folded: `auction` per ply attempted with nullable `resolution`, hello records in `game_start`, bounded base64 raw capture on every non-`ok` validation, `recovery` event type; fault-fixture → schema-valid-log named in §10.
- M4 (determinism scope) — folded as recommended: byte-identity scoped to scripted stubs + injected fake clock + canonical serialization; production keeps real timing (§10).
- M5 (`info.quality`) — folded as recommended: public schema accepts any capped object; `quality` binds harness-owned producers' conformance; warn-not-fault (§4).
- M6 (seeds) — folded: full derivation (`pair_id`, `pair_seed`, `game_seed_k`, byte encodings, coin mapping + `coin_invert`) and logged identifiers (§7, §8).
- M7 (engine adapter divergence) — folded: named consumer-alignment obligation in §11 and in the GRILL_LOCK design-lock impact; delta routes through s1.orchestrator-planner pre-PLAN.
- Correction A (`fault_forfeit`) — folded: removed; reachable fault terminals are `hello_fault` and `recovery_fault` (§3.5).
- Correction B (`game_end` delivery) — folded: best-effort delivery, logged, never rewrites a decided result (§3.5, §8).

Second pass (DESIGN-REVIEW harness-c1-design-review-2a, 2026-08-25) — M1/M2/M4/M5/M7 and both corrections passed; residuals folded:
- M3 residual (recovery log totality) — folded: two distinct record shapes (`turn_record`, `hello_record` with parsed identity on success); `auction.outcome` enum gains `aborted_recovery_fault`; normative causal emission order (wall-clock completion, recovery before its ply's auction event, X before O); recovery-fault dispositions fully enumerated; recovery-fault log fixtures added to §10.
- M6 residual (pair fairness) — folded as recommended: stable lexicographic A/B engine order, delimiter-free config strings enforced, one pair-level coin bit (`pair_coin_seat` from `pair_seed[0]`), same seat result in both seat-swapped games, NO inversion — opposite-engine favoritism holds by construction; encodings pinned; both-parity pair fixtures added.

Amendment 1 (2026-08-25, pre-implementation; raised by harness.implementer IMPL-hold SITREP 095726): the §8 causal-emission paragraph written at the M3 fold predated the plan review's MR3 correction (single fault: resolve/pay/apply/terminal BEFORE recovery, recovery only if the game continues — required by R2's fault-loses-only-the-auction) and over-generalized "mid-ply, before the auction event" to all recoveries and "aborted_recovery_fault" to all recovery-fault terminals. Amended: §5.1 gains the explicit single-fault ordering bullet; §8's emission order now distinguishes double-fault (mid-ply, before) from single-fault (after the resolved auction) recoveries, the `recovery` event carries normative `ply`/`trigger_request_id` association keys, and the `recovery_fault` terminal has its two shapes enumerated; §10 fixture line updated to cover both shapes. Same DESIGN_DOC_ID; digest floats forward with the re-review request.
