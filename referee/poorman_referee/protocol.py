import json
from dataclasses import dataclass

from .auction import Submission
from .rules import Move, Position, legal_moves


FAULTS = (
    "timeout",
    "eof_or_crash",
    "invalid_utf8",
    "invalid_json",
    "schema_violation",
    "wrong_request_id",
    "extra_protocol_line",
    "oversize_line",
    "illegal_bid",
    "illegal_move",
)
MAX_LINE, MAX_INFO, MAX_RAW = 32768, 8192, 4096


def canonical_dumps(obj) -> str:
    return json.dumps(obj, sort_keys=True, separators=(",", ":"), ensure_ascii=False)


@dataclass(frozen=True)
class ParsedReply:
    validation: str
    bid: int | None
    move: Move | None
    info: dict | None
    raw: bytes | None
    raw_total: int | None
    raw_truncated: bool | None


@dataclass(frozen=True)
class ParsedHello:
    validation: str
    name: str | None
    version: str | None
    raw: bytes | None
    raw_total: int | None
    raw_truncated: bool | None


def hello_request(
    *, game_id: str, you: str, time_ms: int, grace_ms: int, budget: int
) -> dict:
    return {
        "type": "hello",
        "protocol": 1,
        "game_id": game_id,
        "you": you,
        "rules": "poorman-uttt-v1",
        "time_ms": time_ms,
        "grace_ms": grace_ms,
        "budget": budget,
    }


def turn_request(
    *,
    game_id: str,
    request_id: str,
    ply: int,
    attempt: int,
    you: str,
    pos: Position,
    budgets: dict[str, int],
    tie_owner: str | None,
    time_ms: int,
) -> dict:
    return {
        "type": "turn",
        "protocol": 1,
        "game_id": game_id,
        "request_id": request_id,
        "ply": ply,
        "attempt": attempt,
        "you": you,
        "board": list(pos.board),
        "forced": pos.forced,
        "legal": [list(move) for move in legal_moves(pos)],
        "budgets": dict(budgets),
        "tie_owner": tie_owner,
        "time_ms": time_ms,
    }


def game_end_msg(*, result: str, reason: str, budgets: dict[str, int]) -> dict:
    return {
        "type": "game_end",
        "protocol": 1,
        "result": result,
        "reason": reason,
        "budgets": dict(budgets),
    }


def _raw_fields(raw: bytes) -> tuple[bytes, int, bool]:
    return raw[:MAX_RAW], len(raw), len(raw) > MAX_RAW


def _reply_fault(
    validation: str,
    raw: bytes,
    *,
    bid: int | None = None,
    move: Move | None = None,
    info: dict | None = None,
) -> ParsedReply:
    captured, total, truncated = _raw_fields(raw)
    return ParsedReply(
        validation=validation,
        bid=bid,
        move=move,
        info=info,
        raw=captured,
        raw_total=total,
        raw_truncated=truncated,
    )


def _hello_fault(validation: str, raw: bytes) -> ParsedHello:
    captured, total, truncated = _raw_fields(raw)
    return ParsedHello(
        validation=validation,
        name=None,
        version=None,
        raw=captured,
        raw_total=total,
        raw_truncated=truncated,
    )


def parse_turn_reply(
    raw: bytes, *, request_id: str, budget: int, legal: list[Move]
) -> ParsedReply:
    if len(raw) > MAX_LINE:
        return _reply_fault("oversize_line", raw)
    try:
        text = raw.decode("utf-8")
    except UnicodeDecodeError:
        return _reply_fault("invalid_utf8", raw)
    try:
        data = json.loads(text)
    except (json.JSONDecodeError, RecursionError):
        return _reply_fault("invalid_json", raw)

    if not isinstance(data, dict):
        return _reply_fault("schema_violation", raw)
    if data.get("type") != "turn" or type(data.get("protocol")) is not int:
        return _reply_fault("schema_violation", raw)
    if data["protocol"] != 1 or not isinstance(data.get("request_id"), str):
        return _reply_fault("schema_violation", raw)
    if "bid" not in data or "move" not in data:
        return _reply_fault("schema_violation", raw)
    move_value = data["move"]
    if not (
        isinstance(move_value, list)
        and len(move_value) == 2
        and all(type(value) is int for value in move_value)
    ):
        return _reply_fault("schema_violation", raw)
    info = data.get("info")
    if info is not None and not isinstance(info, dict):
        return _reply_fault("schema_violation", raw)

    move = (move_value[0], move_value[1])
    bid_value = data["bid"]
    bid = bid_value if type(bid_value) is int else None
    if data["request_id"] != request_id:
        return _reply_fault("wrong_request_id", raw, bid=bid, move=move, info=info)
    if type(bid_value) is not int or not 0 <= bid_value <= budget:
        return _reply_fault("illegal_bid", raw, bid=bid, move=move, info=info)
    if move not in legal:
        return _reply_fault("illegal_move", raw, bid=bid_value, move=move, info=info)
    if info is not None and len(canonical_dumps(info).encode("utf-8")) > MAX_INFO:
        return _reply_fault(
            "schema_violation", raw, bid=bid_value, move=move, info=info
        )
    return ParsedReply(
        validation="ok",
        bid=bid_value,
        move=move,
        info=info,
        raw=None,
        raw_total=None,
        raw_truncated=None,
    )


def parse_hello_reply(raw: bytes) -> ParsedHello:
    if len(raw) > MAX_LINE:
        return _hello_fault("oversize_line", raw)
    try:
        text = raw.decode("utf-8")
    except UnicodeDecodeError:
        return _hello_fault("invalid_utf8", raw)
    try:
        data = json.loads(text)
    except (json.JSONDecodeError, RecursionError):
        return _hello_fault("invalid_json", raw)

    if not isinstance(data, dict):
        return _hello_fault("schema_violation", raw)
    if data.get("type") != "hello" or type(data.get("protocol")) is not int:
        return _hello_fault("schema_violation", raw)
    if data["protocol"] != 1:
        return _hello_fault("schema_violation", raw)
    if not isinstance(data.get("name"), str) or not isinstance(
        data.get("version"), str
    ):
        return _hello_fault("schema_violation", raw)
    if "author" in data and not isinstance(data["author"], str):
        return _hello_fault("schema_violation", raw)
    return ParsedHello(
        validation="ok",
        name=data["name"],
        version=data["version"],
        raw=None,
        raw_total=None,
        raw_truncated=None,
    )


def to_submission(parsed: ParsedReply) -> Submission:
    return Submission(
        fault=None if parsed.validation == "ok" else parsed.validation,
        bid=parsed.bid,
        move=parsed.move,
    )
