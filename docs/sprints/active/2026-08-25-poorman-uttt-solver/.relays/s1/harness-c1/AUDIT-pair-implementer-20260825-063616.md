ROLE: Pair Implementer
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: harness-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — orchestrator reconciliation is the next transition; operator-semantic items are flagged below before any design lock
FROM: harness.implementer
TO: s1.orchestrator-planner
CC: s1.orchestrator-reviewer
SUBJECT: AUDIT return — harness protocol, referee, and tournament recon

## Four-bucket verdict

PRIMARY_BUCKET: still-open
still-open: The harness implementation is absent and the approved protocol sketch does not yet settle enough wire, lifecycle, failure, or replay semantics for independent implementations to interoperate. — E1 — source: `rg --files -g '!docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/**'` returned only the design, roadmap, and reconciliation documents; design lines 83-98 are the entire harness sketch.
already-closed: No local referee, protocol package, runner, baseline bot, or test exists. External projects close individual pattern questions only; none closes this sealed-bid Poorman UTTT harness. — E1 — source: repository file census above and prior-art sources below.
product-overlapped: Cute Chess, CodinGame, and Socialgorithm overlap process supervision, timeouts, paired scheduling, referee ownership, and replay/tournament structure, but their chess, Java-platform, or socket-server contracts do not implement this game's simultaneous sealed bids, budget destruction, first-turn tie owner, or canonical rules. — E1 — source: prior-art sources below plus design lines 19-35.
recommended-next: Proceed to DESIGN with a small subprocess JSONL v1: one persistent process per player per game, full-state turn requests sent concurrently, strict response validation before either bid is revealed, an explicit tie-owner field, and a canonical append-only JSONL event log. Imitate prior-art behavior, not code or dependencies. — E0 — source: audit recommendation synthesized from the E1/E2 evidence in this return.

## Protocol questions DESIGN must settle

### 1. JSONL framing and value grammar

- The sketch shows JSONL but does not define encoding, line termination, buffering, maximum reply size, stdout noise, unknown fields, or exact board/index grammar. Its illustrative `board` value is notation rather than valid JSON and leaves arrays-versus-strings ambiguous. — E1 — source: design lines 85-91.
- Recommendation: UTF-8; exactly one JSON object per LF-terminated line; engine must flush; stdout is protocol-only and stderr is capped diagnostics; blank/non-JSON/oversize lines are violations; 0-based row-major indices; board is exactly nine 9-character strings over `.`/`X`/`O`; `forced` is `null` or integer 0..8; JSON booleans are not integers; unknown request keys are ignored within protocol major 1. — E0 — source: third-party-afternoon interoperability recommendation.
- Recommendation: every turn message carries `type`, `protocol`, `game_id`, `request_id`, `ply`, `you`, canonical board, `forced`, canonical budgets, explicit `tie_owner`, and `time_ms`; every reply echoes `type`, `protocol`, and `request_id` and always carries both `bid` and intended `move`. — E0 — source: requirements coverage analysis against design lines 19-35 and 83-98.

### 2. Process lifecycle and sealed collection

- “Stateless per request” does not decide whether the subprocess is launched per turn, per game, or per tournament; the same design separately says the referee spawns two engine processes. — E1 — source: design lines 85 and 96.
- Recommendation: launch one process per seat at game start, keep it for that game, and terminate its process group after the terminal event; each turn remains logically stateless because the complete authoritative state is resent. Restart between games by default so a crash or leaked state cannot contaminate later games. — E0 — source: audit recommendation; Cute Chess exposes explicit engine restart/recovery policies as useful precedent.
- The referee must write equivalent pre-auction state to both engines before awaiting either reply, collect both concurrently, validate both independently, and reveal neither output until collection closes. — E1 — source: sealed simultaneous bidding and intended-move rules at design lines 22-28; concurrency behavior is required to preserve those approved semantics.

### 3. Handshake and versioning

- A request-side `protocol: 1` alone does not define how an incompatible engine fails before a rated game or how engine identity/version reaches regression logs. — E1 — source: design lines 88-94 and tournament regression requirement at line 98.
- Recommendation: one startup `hello`/`ready` exchange with protocol major 1, engine name/version, and supported features; reject unsupported major versions before turn 0. Turn messages still repeat `protocol` and `request_id`. Within a major, readers ignore unknown keys but missing/invalid required keys fail closed. — E0 — source: afternoon-implementability and replay provenance recommendation.

### 4. Timeout, EOF, malformed output, and process cleanup

- The design requires referee-enforced time limits but does not define the measured interval, startup allowance, grace, partial lines, EOF, crash, extra output, or process-tree cleanup. — E1 — source: design lines 13, 96, and 124-125.
- Recommendation: measure with a monotonic clock from the completed request write/flush to receipt of one complete response line; `time_ms` is the engine's usable allowance and any fixed transport grace is separately configured and logged; timeout, EOF, crash, invalid UTF-8/JSON/schema, wrong request id, extra protocol line, oversize output, out-of-range bid, or illegal intended move are named termination reasons. Kill the whole process group on terminal failure. — E0 — source: deterministic runner recommendation; CodinGame exposes a distinct timeout exception on output collection and Cute Chess exposes fixed move limits plus an explicit time margin.

### 5. Forfeit and validation rules

- The current sketch supplies no tournament result for one or both engines failing, and “intended move if it wins” could be misread as allowing a missing or illegal move when that engine would lose the auction. — E1 — source: design lines 86-98.
- Recommendation: validate both complete sealed submissions before comparing bids; every reply must contain a currently legal intended move even if its bid later loses. One invalid submission forfeits the game 0-1 without payment or move application; tournament execution continues and logs the precise reason. — E0 — source: fail-closed sealed-auction recommendation.
- Double-failure result and whether an invalid losing intent forfeits are operator-judgment items below; DESIGN must not silently choose them. — E1 — source: absence of those semantics in design lines 19-35 and 83-98.

### 6. Seat/color assignment and perspective

- The wire sketch uses `you`/`opp` budgets but does not identify whether the engine is X or O, while canonical board marks, mover history, paired seat swaps, and statistics require stable identities. — E1 — source: design lines 19-35, 89, and 98.
- Recommendation: game header binds player id/command/version to X or O; each engine request includes `you: "X"|"O"`; wire inputs may expose perspective budgets only if the canonical X/O values are unambiguous, while logs always use canonical X/O. Pair every matchup with X/O swapped and the initial tie advantage swapped. — E0 — source: fairness recommendation plus design line 98.

### 7. First-turn coin flip and deterministic tournaments

- `last_mover: null` cannot carry the tie-owner bit required by the approved search state on turn 0; the design both invokes a first-turn coin flip and says paired tournaments alternate the initial coin flip. — E1 — source: design lines 24-26, 42, 52-53, 89, and 98.
- Recommendation: derive a game seed from the tournament seed and stable game id, choose the initial tie owner once at game setup, record the seed and chosen owner, include explicit `tie_owner` in every request, and pair the return game with both seat and initial tie owner swapped. Replays consume the logged result and never reroll. — E0 — source: reproducibility and search-contract recommendation.
- Whether the preselected first-turn tie owner is disclosed before bids is placed changes strategic information and therefore requires the operator ruling below. — E1 — source: threshold state explicitly depends on tie owner at design lines 42 and 52-53, while the canonical rule at lines 24-26 does not state disclosure timing.

### 8. Replayable game-log schema

- “Replayable JSON game log” does not define a schema, provenance, raw submissions, elapsed times, termination reasons, or whether post-state is stored or recomputed. — E1 — source: design line 96.
- Recommendation: versioned append-only JSONL with `game_start`, one `auction` event per ply, and `game_end`. Header records rules/protocol/log versions, game/tournament ids, seed, players and versions, seat mapping, initial tie owner, budgets, and time control. Auction records request id, canonical pre-state hash, both raw parsed submissions, elapsed milliseconds, validation outcomes, tie resolution, payer/payment, applied move, and canonical post-state. Footer records W/D/L, budget margin, termination reason, and event-chain hash. — E0 — source: replay/UI/regression-consumer recommendation.
- The log is the canonical consumer contract for UI replay and tournament aggregation; raw stderr belongs in separately capped diagnostics so the replay remains stable and language-neutral. — E0 — source: boundary analysis against design lines 96, 98, and 102-104.

## Third-party “one afternoon, any language” bar

- The current sketch is readable but not independently implementable because a friend must guess board shape/indexing, process lifetime, message correlation, initial tie owner, validation order, and failure outcomes. — E1 — source: comparison of design lines 83-98 with the unanswered items above.
- Keep v1 to three message shapes (`hello`, `turn`, `ready/bid` as applicable), integers/strings/arrays/null only, no network transport, no schema generator requirement, no async engine messages, and no optional strategic extensions. Supply one normative transcript, a JSON Schema or equivalent field table, a tiny conformance driver, and fixtures for success plus each named failure. — E0 — source: audit recommendation.
- Reject importing a generic tournament platform or chess protocol wholesale; it adds domain rules, dependencies, and license surface without solving simultaneous sealed collection. Narrow v1 to local subprocesses and a single-game runner before tournament concurrency. — E0 — source: prior-art fit analysis below.

## Duplicate / prior-art gate

- Cute Chess is worth imitating for explicit engine commands, per-move time limits and margins, restart/recover policy, concurrency, deterministic seeds, paired side swaps, machine-readable result fields, and finished-game logs. Its implementation is C++/Qt and GPLv3+, and its UCI/XBoard protocols are sequential chess protocols, so do not reuse it as the harness or wire contract. — E1 — source: https://github.com/cutechess/cutechess and https://github.com/cutechess/cutechess/blob/master/docs/cutechess-cli.6
- CodinGame is worth imitating for a referee that owns rules and turn order, a distinct output-collection timeout, and a game manager that emits replay frames. Its Java platform/toolkit is much heavier than the afternoon subprocess contract. — E1 — source: https://codingame.github.io/codingame-game-engine/com/codingame/gameengine/core/package-summary.html and https://github.com/CodinGame/codingame-game-engine
- Socialgorithm has a UTTT engine/server, a tournament server/data model/UI, and a game-server abstraction. That demonstrates useful component separation, but its socket-server platform and AGPL-licensed game-server library are not a drop-in local JSONL referee and its ordinary alternating UTTT rules do not cover bidding. — E1 — source: https://github.com/socialgorithm/ultimate-ttt, https://github.com/socialgorithm/tournament, and https://github.com/socialgorithm/game-server-js
- `ultimate-ttt/ultimate-ttt` is a React/Redux browser game for ordinary alternating UTTT with tests; it may inform UI interaction only. It supplies no engine subprocess referee, sealed-bid protocol, or tournament runner, matching the approved “no code reuse” constraint. — E1 — source: https://github.com/ultimate-ttt/ultimate-ttt and design line 105.

## Python toolchain

- `python3 --version` => `Python 3.12.5`; `command -v python3` => `/Library/Frameworks/Python.framework/Versions/3.12/bin/python3`. — E2 — source: command output in this audit turn.
- `python3 -m pip --version` => `pip 24.3.1 ... (python 3.12)`; `python3 -m pytest --version` => `pytest 8.4.2`. — E2 — source: command output in this audit turn.
- No project environment, dependency manifest, or Python package exists yet, so exact supported Python minor and dependency pins remain DESIGN/PLAN obligations rather than toolchain facts. — E1 — source: repository file census.

## Boundary contract

Writes: versioned interop request/response schema; referee-owned canonical auction decisions; versioned replay JSONL; tournament result aggregates.
Reads: approved canonical rules; theory-owned rules fixtures; engine command/configuration; two sealed engine replies per auction.
Target entity: authoritative game state and append-only game record after each resolved auction.
Downstream consumer: engine and friend-engine wire clients; UI replay/analysis; tournament regression reporting.
Contract: protocol/log versions, canonical coordinates and marks, seat identity, budgets, tie owner, deadlines, response validity, auction outcome, terminal reason, and deterministic provenance.
Proof: E2 conformance matrix must run the same normative fixtures against referee validation, baseline subprocess bots, and at least one language-minimal sample client; UI consumes logged fixtures at its boundary.
No-consumer action: reject speculative fields that no named engine, referee, UI, or tournament consumer reads; route shared-contract changes through `s1.orchestrator-planner`.

## Operator-judgment items

1. Is the first-turn tie owner selected and disclosed to both engines before the first sealed bid, or selected only if a 0-0/equal-bid tie actually occurs? Recommendation: preselect and disclose, because the approved threshold state explicitly requires `h`; hiding it creates a different imperfect-information root game. — E1/E0 — source: design lines 24-26, 42, 52-53, 89, and 98 plus audit recommendation.
2. If both engines independently fail validation or timeout on the same auction, is the game a draw, a double loss excluded from strength statistics, or a tournament error requiring replay? Recommendation: record double-forfeit and exclude it from strength W/D/L while surfacing it in reliability stats. — E1/E0 — source: failure policy absent from design lines 83-98 plus audit recommendation.
3. Does a syntactically valid bid paired with an illegal intended move forfeit even when that bid would have lost? Recommendation: yes; validate sealed submissions before comparison so result validity cannot depend on inspecting the opponent's secret bid. — E1/E0 — source: simultaneous sealed submission at design lines 22-23 and underspecified “if it wins” wording at line 86.

## Escalation and narrowing

- No source/test/scaffold edits, branch, commit, prototype, or plan were made; this relay and its INDEX row are the only authored artifacts. — E1 — source: fresh final git status below and relay path.
- Scope drift risk: importing chess rules/protocols, building network services, or choosing the three operator-semantic items inside pair DESIGN would exceed the audited lane. — E1 — source: dispatch not-in-scope and boundary-routing requirements.
- Reject/narrow gate: if DESIGN cannot keep a third-party client to a short documented stdin/stdout loop plus deterministic fixtures, narrow before lock rather than delegating complexity to the friend-engine author. — E0 — source: requirement bar at design line 94.
- Exact-file relay lint passed. Root INDEX lint remains blocked by inherited append-order inversions at lines 17 and 19; my appended row is later than the live tail and was not the cause. — E2 — source: `relay-lint.py --no-freshness <this relay>` and `relay-lint.py --index <INDEX>` output.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-pair-implementer-20260825-063616.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-063616`
- source/tests=none — read-only audit; no source or test edits

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? .claude/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-pair-implementer-20260825-063848.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-planner-20260825-063250.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-pair-implementer-20260825-063616.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/AUDIT-pair-implementer-20260825-063801.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-pair-implementer-20260825-063705.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-planner-20260825-063204.md

## Next requested action

Reconcile this independent return with the harness Planner return, route the three operator-semantic items, then dispatch DESIGN with the resolved rule inputs and the protocol/log consumer boundaries preserved. — E0 — source: protocol workflow recommendation.
