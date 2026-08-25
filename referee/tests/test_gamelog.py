import json

import pytest

from poorman_referee.gamelog import (
    GameLogWriter,
    hello_record,
    read_log,
    replay_frames,
    turn_record,
)
from poorman_referee.protocol import ParsedHello, ParsedReply, canonical_dumps


def ok_turn(bid=3, move=(4, 4), info=None, elapsed_ms=7):
    return {
        "validation": "ok",
        "elapsed_ms": elapsed_ms,
        "bid": bid,
        "move": list(move),
        "info": {} if info is None else info,
    }


def fault_turn(validation="timeout", elapsed_ms=9):
    return {
        "validation": validation,
        "elapsed_ms": elapsed_ms,
        "raw": {"b64": "eA==", "truncated": False, "bytes_total": 1},
    }


def start_event():
    return {"event": "game_start", "game_id": "g1", "pair_coin_seat": "X"}


def end_event(result="X", reason="macro_win"):
    return {
        "event": "game_end",
        "result": result,
        "reason": reason,
        "delivery": {"X": "ok", "O": "ok"},
    }


def resolved_auction(*, attempts=None, ply=0):
    return {
        "event": "auction",
        "ply": ply,
        "outcome": "resolved",
        "attempts": attempts
        or [
            {
                "request_id": f"g1-p{ply}-a1",
                "attempt": 1,
                "turns": {
                    "X": ok_turn(info={"quality": 1}),
                    "O": ok_turn(bid=3, move=(4, 5), elapsed_ms=8),
                },
            }
        ],
        "resolution": {
            "winner": "X",
            "reason": "tie_coin",
            "coin": "X",
            "payment": 3,
            "move": [4, 4],
            "closures": [],
            "macro_line": None,
            "forced_next": 4,
        },
        "budgets_after": {"X": 97, "O": 100},
        "post_board": ["........."] * 4
        + ["....X...."]
        + ["........."] * 4,
    }


def recovery_event(*, seat="X", ply=0, request_id="g1-p0-a1"):
    return {
        "event": "recovery",
        "seat": seat,
        "ply": ply,
        "trigger_request_id": request_id,
        "fault": "timeout",
        "hello": {
            "validation": "ok",
            "name": "stub",
            "version": "1",
            "elapsed_ms": 2,
        },
    }


def test_writer_reader_round_trip_is_canonical_and_flushed(tmp_path):
    path = tmp_path / "game.jsonl"
    event = {"z": 1, "event": "probe", "a": "snowman ☃"}
    with path.open("w", encoding="utf-8") as fh:
        writer = GameLogWriter(fh)
        writer.emit(event)
        assert path.read_text(encoding="utf-8") == canonical_dumps(event) + "\n"

    assert read_log(path) == [event]


def test_turn_record_fault_has_raw_and_no_parsed_fields():
    parsed = ParsedReply(
        validation="invalid_json",
        bid=None,
        move=None,
        info=None,
        raw=b"{",
        raw_total=1,
        raw_truncated=False,
    )

    assert turn_record(parsed, 12) == {
        "validation": "invalid_json",
        "elapsed_ms": 12,
        "raw": {"b64": "ew==", "truncated": False, "bytes_total": 1},
    }


def test_turn_record_fault_preserves_every_parseable_intention_field():
    parsed = ParsedReply(
        validation="wrong_request_id",
        bid=7,
        move=(4, 2),
        info={"quality": "estimate"},
        raw=b"{}",
        raw_total=2,
        raw_truncated=False,
    )

    assert turn_record(parsed, 6) == {
        "validation": "wrong_request_id",
        "elapsed_ms": 6,
        "bid": 7,
        "move": [4, 2],
        "info": {"quality": "estimate"},
        "raw": {"b64": "e30=", "truncated": False, "bytes_total": 2},
    }


def test_turn_record_ok_and_hello_records():
    turn = ParsedReply("ok", 4, (3, 2), {"quality": "estimate"}, None, None, None)
    hello = ParsedHello("ok", "engine", "2", None, None, None)
    bad_hello = ParsedHello("invalid_utf8", None, None, b"\xff", 1, False)

    assert turn_record(turn, 5) == {
        "validation": "ok",
        "elapsed_ms": 5,
        "bid": 4,
        "move": [3, 2],
        "info": {"quality": "estimate"},
    }
    assert hello_record(hello, 3) == {
        "validation": "ok",
        "elapsed_ms": 3,
        "name": "engine",
        "version": "2",
    }
    assert hello_record(bad_hello, 4) == {
        "validation": "invalid_utf8",
        "elapsed_ms": 4,
        "raw": {"b64": "/w==", "truncated": False, "bytes_total": 1},
    }


@pytest.mark.parametrize("info", [{"pv": []}, {"quality": "unsupported"}])
def test_turn_record_warns_without_faulting_on_missing_or_invalid_info_quality(info):
    parsed = ParsedReply("ok", 4, (3, 2), info, None, None, None)

    record = turn_record(parsed, 5)

    assert record["validation"] == "ok"
    assert record["info"] == info
    assert record["warnings"] == ["info_quality_missing_or_invalid"]


def test_replay_clean_ply_retains_consumer_visible_fields_and_raw_timeline():
    events = [start_event(), resolved_auction(), end_event()]

    replay = replay_frames(events)

    assert replay.start is events[0]
    assert replay.end is events[-1]
    assert replay.events == tuple(events)
    frame = replay.frames[0]
    assert frame.ply == 0
    assert frame.board == tuple(events[1]["post_board"])
    assert frame.budgets == {"X": 97, "O": 100}
    assert frame.forced == 4
    assert frame.outcome == "resolved"
    assert frame.attempts[0]["turns"]["X"] == ok_turn(info={"quality": 1})
    assert frame.attempts[0]["turns"]["O"]["elapsed_ms"] == 8
    assert frame.resolution["payment"] == 3
    assert frame.recoveries == ()


def test_replay_keeps_all_double_fault_attempts():
    attempts = [
        {
            "request_id": "g1-p0-a1",
            "attempt": 1,
            "turns": {"X": fault_turn(), "O": fault_turn("invalid_json")},
        },
        {
            "request_id": "g1-p0-a2",
            "attempt": 2,
            "turns": {"X": ok_turn(), "O": ok_turn(move=(4, 5))},
        },
    ]
    events = [start_event(), resolved_auction(attempts=attempts), end_event()]

    frame = replay_frames(events).frames[0]

    assert len(frame.attempts) == 2
    assert frame.attempts[0]["turns"]["X"]["validation"] == "timeout"
    assert frame.attempts[0]["turns"]["O"]["validation"] == "invalid_json"
    assert frame.attempts[1]["attempt"] == 2


def test_replay_rejects_attempt_sequence_after_first_attempt_is_removed():
    attempts = [
        {
            "request_id": "g1-p0-a1",
            "attempt": 1,
            "turns": {"X": fault_turn(), "O": fault_turn("invalid_json")},
        },
        {
            "request_id": "g1-p0-a2",
            "attempt": 2,
            "turns": {"X": ok_turn(), "O": ok_turn(move=(4, 5))},
        },
    ]
    auction = resolved_auction(attempts=attempts)
    del auction["attempts"][0]

    with pytest.raises(ValueError, match="attempt sequence: expected 1, got 2"):
        replay_frames([start_event(), auction, end_event()])


def test_replay_associates_single_fault_recovery_emitted_after_auction():
    auction = resolved_auction()
    auction["attempts"][0]["turns"]["X"] = fault_turn()
    events = [start_event(), auction, recovery_event(), end_event()]

    frame = replay_frames(events).frames[0]

    assert frame.recoveries == (events[2],)
    assert frame.recoveries[0]["trigger_request_id"] == "g1-p0-a1"
    assert replay_frames(events).events[1]["event"] == "auction"


def test_replay_associates_double_fault_recoveries_emitted_before_auction():
    auction = resolved_auction()
    events = [
        start_event(),
        recovery_event(seat="X"),
        recovery_event(seat="O"),
        auction,
        end_event(),
    ]

    frame = replay_frames(events).frames[0]

    assert tuple(r["seat"] for r in frame.recoveries) == ("X", "O")


def test_replay_recovery_abort_has_no_resolution():
    auction = resolved_auction()
    auction["outcome"] = "aborted_recovery_fault"
    auction.pop("resolution")
    auction["attempts"][0]["turns"]["X"] = fault_turn()
    events = [start_event(), recovery_event(), auction, end_event("O", "recovery_fault")]

    frame = replay_frames(events).frames[0]

    assert frame.outcome == "aborted_recovery_fault"
    assert frame.resolution is None
    assert frame.forced == 4
    assert replay_frames(events).end["reason"] == "recovery_fault"


def test_nonresolved_later_ply_retains_previous_forced_board():
    first = resolved_auction()
    first["resolution"]["forced_next"] = 6
    second = resolved_auction(ply=1)
    second["outcome"] = "voided"
    second.pop("resolution")
    second["post_board"] = first["post_board"]
    second["budgets_after"] = first["budgets_after"]

    replay = replay_frames([start_event(), first, second, end_event("void", "triple_double_fault_void")])

    assert replay.frames[0].forced == 6
    assert replay.frames[1].forced == 6


def test_replay_terminal_exposes_result_reason_and_delivery():
    events = [start_event(), resolved_auction(), end_event("draw", "exact_tie_draw")]

    replay = replay_frames(events)

    assert replay.end["result"] == "draw"
    assert replay.end["reason"] == "exact_tie_draw"
    assert replay.end["delivery"] == {"X": "ok", "O": "ok"}


@pytest.mark.parametrize("missing", ["post_board", "attempts"])
def test_replay_rejects_missing_auction_sufficiency_field(missing):
    auction = resolved_auction()
    del auction[missing]

    with pytest.raises(ValueError, match=missing):
        replay_frames([start_event(), auction, end_event()])


def test_replay_rejects_missing_game_end():
    with pytest.raises(ValueError, match="game_end"):
        replay_frames([start_event(), resolved_auction()])


def test_read_log_rejects_non_object_line(tmp_path):
    path = tmp_path / "bad.jsonl"
    path.write_text(json.dumps([1, 2, 3]) + "\n", encoding="utf-8")

    with pytest.raises(ValueError, match="object"):
        read_log(path)
