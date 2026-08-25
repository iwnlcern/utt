# Poor Man's Ultimate Tic-Tac-Toe Protocol v1

This document defines the local engine protocol, referee behavior, and replay log for `poorman-uttt-v1`.

The JSON Schema files in [`schema/`](schema/) are the normative machine-readable definitions of message and log fields.

The field tables below are continuously checked against those schema files.

## Transport and framing

The referee and each engine communicate through UTF-8 JSON Lines over the engine's standard input and standard output.

Every protocol message is exactly one JSON object terminated by LF.

An engine MUST flush standard output after every message.

Standard output is protocol-only, while standard error is free-form diagnostic output.

The referee captures at most 65,536 bytes of standard error per engine and records whether additional bytes were truncated.

The capped bytes are persisted to a per-game, per-seat diagnostic file, including bytes emitted by process generations that were replaced during recovery.

A protocol line may contain at most 32,768 bytes before LF.

The referee detects an overlong partial line before LF arrives and classifies it as `oversize_line`.

Before sending a request, the referee drains all currently available output from that engine.

Any complete or partial stale output at that boundary is `extra_protocol_line`, and the new request is not written.

Both turn requests are fully written and flushed before either reply is awaited.

Readers ignore unknown object keys within protocol major version 1.

Missing required fields and fields of the wrong type fail closed.

Non-standard numeric constants such as `NaN` and strings that cannot be encoded as valid UTF-8 are rejected and are never re-emitted into canonical JSON.

All game-affecting money values are nonnegative JSON integers, and 1,000,000,000 units is the initial budget.

## Message field tables

The referee sends `hello`, `turn`, and `game_end` messages.

An engine sends `hello` and `turn` messages.

### `hello_request.schema.json`

| Field | Required | Type | Meaning |
| --- | --- | --- | --- |
| `type` | yes | string constant `hello` | Selects the hello message family. |
| `protocol` | yes | integer constant `1` | Selects protocol major version 1. |
| `game_id` | yes | string | Identifies this game. |
| `you` | yes | `X` or `O` | Assigns the engine's canonical seat. |
| `rules` | yes | string constant `poorman-uttt-v1` | Selects the rules contract. |
| `time_ms` | yes | nonnegative integer | Gives the usable per-turn computation allowance. |
| `grace_ms` | yes | nonnegative integer | Gives the fixed transport grace added to each turn. |
| `budget` | yes | nonnegative integer | Gives this seat's initial integer budget. |

The engine performs startup work before answering hello.

There is no separate first-turn warmup allowance.

### `hello_reply.schema.json`

| Field | Required | Type | Meaning |
| --- | --- | --- | --- |
| `type` | yes | string constant `hello` | Selects the hello reply family. |
| `protocol` | yes | integer constant `1` | Confirms protocol major version 1. |
| `name` | yes | string | Names the engine. |
| `version` | yes | string | Identifies the engine build. |
| `author` | no | string | Optionally identifies the author. |

A malformed or late startup reply is a game-level `hello_fault`.

If both startup replies fault, the game is void.

### `turn_request.schema.json`

| Field | Required | Type | Meaning |
| --- | --- | --- | --- |
| `type` | yes | string constant `turn` | Selects the turn message family. |
| `protocol` | yes | integer constant `1` | Selects protocol major version 1. |
| `game_id` | yes | string | Identifies this game. |
| `request_id` | yes | string | Uniquely identifies this ply attempt. |
| `ply` | yes | nonnegative integer | Gives the zero-based attempted-auction index. |
| `attempt` | yes | integer from 1 through 3 | Counts double-fault retries for this ply. |
| `you` | yes | `X` or `O` | Repeats the receiving engine's canonical seat. |
| `board` | yes | nine 9-character board strings | Gives the complete authoritative position. |
| `forced` | yes | integer 0 through 8 or null | Gives the forced local board, or free choice when null. |
| `legal` | yes | array of `[local, cell]` pairs | Enumerates every legal move. |
| `budgets` | yes | object with integer `X` and `O` | Gives both remaining canonical-seat budgets. |
| `tie_owner` | yes | `X`, `O`, or null | Gives the seat that wins an equal bid, or null at ply 0. |
| `time_ms` | yes | nonnegative integer | Gives the usable allowance for this reply. |

Each board string and each local board use row-major indices 0 through 8.

The nine local boards are also indexed row-major from 0 through 8.

The initial position forces local board 4.

An engine can be a correct client without implementing legality because `legal` is complete.

### `turn_reply.schema.json`

| Field | Required | Type | Meaning |
| --- | --- | --- | --- |
| `type` | yes | string constant `turn` | Selects the turn reply family. |
| `protocol` | yes | integer constant `1` | Confirms protocol major version 1. |
| `request_id` | yes | string | Echoes the request identifier exactly. |
| `bid` | yes | nonnegative integer | Offers at most the sender's current budget. |
| `move` | yes | `[local, cell]` | Supplies an intended member of the request's `legal` list. |
| `info` | no | object | Carries up to 8,192 UTF-8 bytes of advisory analysis. |

The move is required even when an engine expects its bid to lose.

The referee copies `info` to the log but never uses it to decide the game.

Unknown keys and arbitrary object keys inside `info` are allowed.

### `game_end.schema.json`

| Field | Required | Type | Meaning |
| --- | --- | --- | --- |
| `type` | yes | string constant `game_end` | Selects the terminal message family. |
| `protocol` | yes | integer constant `1` | Selects protocol major version 1. |
| `result` | yes | `X`, `O`, `draw`, or `void` | Gives the final disposition. |
| `reason` | yes | terminal-reason enum | Explains how the game ended. |
| `budgets` | yes | object with integer `X` and `O` | Gives the final budgets. |

Terminal reasons are `macro_win`, `chip_count`, `exact_tie_draw`, `hello_fault`, `recovery_fault`, and `triple_double_fault_void`.

Delivery is best-effort and never rewrites an already decided result.

The referee closes standard input, waits the configured shutdown grace, kills the full process group if necessary, and reaps it.

## Fault taxonomy and auction recovery

The ten turn fault classes are `timeout`, `eof_or_crash`, `invalid_utf8`, `invalid_json`, `schema_violation`, `wrong_request_id`, `extra_protocol_line`, `oversize_line`, `illegal_bid`, and `illegal_move`.

Validation is fail-closed in framing, UTF-8, JSON, required-schema, request-id, bid, move, and analysis-size order.

A boolean is not an integer bid.

A bid below zero or above the sender's budget is `illegal_bid`.

A move not present in the authoritative `legal` list is `illegal_move`.

When exactly one submission faults, the legal opponent wins that auction at payment zero and its intended move is applied.

The resulting move, closures, terminal state, budgets, and tie owner are committed before recovery begins.

If that move ends the game, no recovery is attempted and a hypothetical restart failure cannot replace the earned result.

If the game continues, the referee kills the faulter's process group, starts a fresh process, and repeats hello before that seat's next turn.

When both submissions fault, neither bid is revealed through a resolution and neither move is applied.

The referee recovers X and then O before sending a fresh request for the identical state with `attempt` incremented.

Three consecutive double faults produce a void game with reason `triple_double_fault_void`.

A failed recovery hello makes continuation impossible and ends the game with reason `recovery_fault`.

One failed recovery awards the game to the other seat, while two failed recoveries make it void.

Every non-OK reply stores at most the first 4,096 raw bytes in base64 together with the total byte count and truncation flag.

Any bid, move, or analysis object that was parseable before the detected fault remains present in that turn record, so the log retains both engines' intentions.

## Clock semantics

Each engine has an independent monotonic clock window for each turn.

The window opens after that engine's complete request line has been written and flushed.

The window closes when one complete reply line has been received.

The usable allowance is `time_ms`, and `grace_ms` is added only for transport tolerance.

Exceeding their sum is `timeout`.

The referee writes both sealed requests before collecting either response and completes both collection windows before resolving the auction.

Elapsed milliseconds are logged for every hello and turn record.

## Seats, ties, payment, and the pair coin

All messages and logs use canonical `X` and `O` marks rather than engine-relative names.

At ply 0, `tie_owner` is null and no random choice is made unless the bids actually tie.

The pair seed is SHA-256 over the tournament seed, canonical UTF-8 engine identifiers, a `0x1f` separator, and the pair round.

The pair coin seat is X when the first pair-seed byte is even and O when it is odd.

The same pair coin seat is logged and used in both games of a seat-swapped pair.

Therefore a first-ply tie in both games favors opposite engines without inverting the coin rule.

After every applied move, including a fault-won move and a zero-bid tie, `tie_owner` becomes the opponent of the actual mover.

The auction winner pays its own bid only for a normal or tied valid resolution.

A single-fault winner pays zero.

Money is compared, paid, serialized, and aggregated as integers.

## Log schema v1 and replay

Each game log is append-only canonical JSONL with sorted keys and compact separators.

The event families are `game_start`, `auction`, `recovery`, and `game_end`.

The corresponding normative schemas are `log_game_start.schema.json`, `log_auction.schema.json`, `log_recovery.schema.json`, and `log_game_end.schema.json`.

The `game_start` event fixes engine identities and commands, seeds, seat mapping, hello outcomes, time controls, and initial budgets.

Each `auction` contains every attempt, both per-seat validations and intentions, the outcome, the applied resolution when one exists, post-budgets, and the complete post-board.

A resolved auction has exactly one `resolution`, while a voided or recovery-aborted auction has none.

A recovery records its triggering ply and request identifier, so consumers associate it by identity rather than event position.

Double-fault recoveries occur before their auction event, while a continuing single-fault recovery occurs after its resolved auction event.

The `game_end` event records result, reason, integer budget margin, attempted plies, per-seat delivery status, and per-seat stderr file metadata.

[`transcript-v1.jsonl`](transcript-v1.jsonl) is a real seeded fraction-versus-random game produced by this referee.

The conformance logs under `referee/tests/fixtures/` cover success, every fault class, a fault-won terminal move with no recovery, both recovery-fault dispositions, both-seat recovery failure, and both pair-seed parities.

The shared replay reader reconstructs every frame only from logged events and rejects missing attempts, boards, terminal events, or ply gaps.

## Write a client in an afternoon

Start with a loop that reads one line from standard input and parses it as a JSON object.

When `type` is `hello`, answer with `type`, `protocol`, `name`, and `version`, print one compact line, and flush.

When `type` is `game_end`, exit normally.

When `type` is `turn`, choose an element from the supplied `legal` array.

Read your current integer budget from `budgets[you]`.

To bid a fixed fraction without floating point, represent the fraction in parts per billion and compute `budget * fraction_ppb // 1_000_000_000`.

Answer with the exact `request_id`, the integer bid, and the selected move, then flush.

Seed any randomness from a command-line option so test runs reproduce.

Keep diagnostics on standard error because any stray standard-output byte can fault the next auction.

The complete commented implementation of these steps is [`fraction_bot.py`](../../referee/bots/fraction_bot.py).

Run it against the zero bot with the packaged CLI:

```sh
uv run --project referee python -m poorman_referee play \
  --x-cmd '["python","referee/bots/fraction_bot.py","--seed","1","--fraction-ppb","100000000"]' \
  --o-cmd '["python","referee/bots/zero_bot.py","--seed","2"]' \
  --x-id fraction --o-id zero --seed demo --log demo.jsonl
```

The CLI also runs an even, seat-swapped round robin from a JSON configuration with `python -m poorman_referee tourney --config tourney.json`.

Tournament summaries encode `avg_budget_margin` exactly as integer `numerator` and positive integer `denominator` fields, so aggregation never introduces floating-point money.
