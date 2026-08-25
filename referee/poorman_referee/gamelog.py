import base64
import json
from dataclasses import dataclass
from pathlib import Path
from typing import TextIO

from .protocol import ParsedHello, ParsedReply, canonical_dumps


def _raw_record(raw: bytes | None, total: int | None, truncated: bool | None) -> dict:
    if raw is None or total is None or truncated is None:
        raise ValueError("fault record is missing raw capture metadata")
    return {
        "b64": base64.b64encode(raw).decode("ascii"),
        "truncated": truncated,
        "bytes_total": total,
    }


def turn_record(parsed: ParsedReply, elapsed_ms: int) -> dict:
    record = {"validation": parsed.validation, "elapsed_ms": elapsed_ms}
    if parsed.validation != "ok":
        if parsed.bid is not None:
            record["bid"] = parsed.bid
        if parsed.move is not None:
            record["move"] = list(parsed.move)
        if parsed.info is not None:
            record["info"] = parsed.info
        record["raw"] = _raw_record(
            parsed.raw, parsed.raw_total, parsed.raw_truncated
        )
        return record
    if parsed.bid is None or parsed.move is None:
        raise ValueError("ok turn is missing bid or move")
    record.update({"bid": parsed.bid, "move": list(parsed.move)})
    if parsed.info is not None:
        record["info"] = parsed.info
    return record


def hello_record(parsed: ParsedHello, elapsed_ms: int) -> dict:
    record = {"validation": parsed.validation, "elapsed_ms": elapsed_ms}
    if parsed.validation != "ok":
        record["raw"] = _raw_record(
            parsed.raw, parsed.raw_total, parsed.raw_truncated
        )
        return record
    if parsed.name is None or parsed.version is None:
        raise ValueError("ok hello is missing name or version")
    record.update({"name": parsed.name, "version": parsed.version})
    return record


class GameLogWriter:
    def __init__(self, fh: TextIO):
        self._fh = fh

    def emit(self, event: dict) -> None:
        if not isinstance(event, dict):
            raise TypeError("log event must be an object")
        self._fh.write(canonical_dumps(event) + "\n")
        self._fh.flush()


def read_log(path: str | Path) -> list[dict]:
    events = []
    with Path(path).open(encoding="utf-8") as fh:
        for line_number, line in enumerate(fh, 1):
            try:
                event = json.loads(line)
            except json.JSONDecodeError as exc:
                raise ValueError(f"invalid JSON on log line {line_number}") from exc
            if not isinstance(event, dict):
                raise ValueError(f"log line {line_number} is not an object")
            events.append(event)
    return events


@dataclass(frozen=True)
class Frame:
    ply: int
    board: tuple[str, ...]
    budgets: dict[str, int]
    forced: int | None
    outcome: str
    attempts: tuple[dict, ...]
    resolution: dict | None
    recoveries: tuple[dict, ...]


@dataclass(frozen=True)
class Replay:
    start: dict
    frames: tuple[Frame, ...]
    end: dict
    events: tuple[dict, ...]


def _required(obj: dict, key: str, context: str):
    if key not in obj:
        raise ValueError(f"{context} missing {key}")
    return obj[key]


def _validate_attempt(attempt: dict, ply: int) -> None:
    if not isinstance(attempt, dict):
        raise ValueError(f"auction ply {ply} attempt is not an object")
    for key in ("request_id", "attempt", "turns"):
        _required(attempt, key, f"auction ply {ply} attempt")
    turns = attempt["turns"]
    if not isinstance(turns, dict):
        raise ValueError(f"auction ply {ply} turns is not an object")
    for seat in ("X", "O"):
        turn = _required(turns, seat, f"auction ply {ply} turns")
        if not isinstance(turn, dict):
            raise ValueError(f"auction ply {ply} turn {seat} is not an object")
        for key in ("validation", "elapsed_ms"):
            _required(turn, key, f"auction ply {ply} turn {seat}")


def replay_frames(events: list[dict]) -> Replay:
    if not events:
        raise ValueError("log is missing game_start")
    starts = [event for event in events if event.get("event") == "game_start"]
    ends = [event for event in events if event.get("event") == "game_end"]
    if len(starts) != 1 or events[0] is not starts[0]:
        raise ValueError("log requires exactly one leading game_start")
    if len(ends) != 1 or events[-1] is not ends[0]:
        raise ValueError("log requires exactly one trailing game_end")

    recoveries_by_ply: dict[int, list[dict]] = {}
    auctions: list[dict] = []
    for event in events[1:-1]:
        kind = _required(event, "event", "log event")
        if kind == "recovery":
            ply = _required(event, "ply", "recovery")
            _required(event, "trigger_request_id", "recovery")
            recoveries_by_ply.setdefault(ply, []).append(event)
        elif kind == "auction":
            auctions.append(event)
        else:
            raise ValueError(f"unexpected log event: {kind!r}")

    frames = []
    seen_plies = set()
    current_forced = 4
    for expected_ply, auction in enumerate(auctions):
        ply = _required(auction, "ply", "auction")
        if ply != expected_ply:
            raise ValueError(f"auction ply gap: expected {expected_ply}, got {ply}")
        if ply in seen_plies:
            raise ValueError(f"duplicate auction ply {ply}")
        seen_plies.add(ply)
        outcome = _required(auction, "outcome", f"auction ply {ply}")
        attempts = _required(auction, "attempts", f"auction ply {ply}")
        if not isinstance(attempts, list) or not attempts:
            raise ValueError(f"auction ply {ply} attempts must be non-empty")
        for attempt in attempts:
            _validate_attempt(attempt, ply)
        board = _required(auction, "post_board", f"auction ply {ply}")
        budgets = _required(auction, "budgets_after", f"auction ply {ply}")
        resolution = auction.get("resolution")
        if outcome == "resolved":
            if not isinstance(resolution, dict):
                raise ValueError(f"auction ply {ply} missing resolution")
            forced = _required(resolution, "forced_next", f"auction ply {ply} resolution")
            current_forced = forced
        else:
            if resolution is not None:
                raise ValueError(f"auction ply {ply} non-resolved outcome has resolution")
            forced = current_forced
        frames.append(
            Frame(
                ply=ply,
                board=tuple(board),
                budgets=dict(budgets),
                forced=forced,
                outcome=outcome,
                attempts=tuple(attempts),
                resolution=resolution,
                recoveries=tuple(recoveries_by_ply.pop(ply, ())),
            )
        )
    if recoveries_by_ply:
        raise ValueError(f"recovery references missing auction ply {min(recoveries_by_ply)}")
    return Replay(
        start=starts[0],
        frames=tuple(frames),
        end=ends[0],
        events=tuple(events),
    )
