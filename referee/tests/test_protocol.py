import json

import pytest

from poorman_referee.auction import Submission
from poorman_referee.protocol import (
    FAULTS,
    MAX_INFO,
    MAX_LINE,
    MAX_RAW,
    canonical_dumps,
    game_end_msg,
    hello_request,
    parse_hello_reply,
    parse_turn_reply,
    to_submission,
    turn_request,
)
from poorman_referee.rules import INITIAL


REQUEST_ID = "g1-p0-a1"
LEGAL = [(4, cell) for cell in range(9)]


def raw_turn(**changes):
    reply = {
        "type": "turn",
        "protocol": 1,
        "request_id": REQUEST_ID,
        "bid": 7,
        "move": [4, 2],
    }
    reply.update(changes)
    return canonical_dumps(reply).encode()


def parse(raw):
    return parse_turn_reply(raw, request_id=REQUEST_ID, budget=10, legal=LEGAL)


def test_fault_taxonomy_is_exact_and_stable():
    assert FAULTS == (
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


@pytest.mark.parametrize(
    ("raw", "fault"),
    [
        (b"\xff\xfe", "invalid_utf8"),
        (b"{", "invalid_json"),
        (b"[]", "schema_violation"),
        (raw_turn(type="hello"), "schema_violation"),
        (raw_turn(request_id="wrong"), "wrong_request_id"),
        (raw_turn(bid=True), "illegal_bid"),
        (raw_turn(bid=11), "illegal_bid"),
        (raw_turn(move=[0, 0]), "illegal_move"),
    ],
)
def test_turn_reply_classifies_first_validation_fault(raw, fault):
    parsed = parse(raw)

    assert parsed.validation == fault
    assert parsed.raw == raw[:MAX_RAW]
    assert parsed.raw_total == len(raw)
    assert parsed.raw_truncated is False


def test_oversize_line_is_classified_before_decode_or_json():
    raw = b"x" * (MAX_LINE + 1)

    parsed = parse(raw)

    assert parsed.validation == "oversize_line"
    assert parsed.raw == raw[:MAX_RAW]
    assert parsed.raw_total == MAX_LINE + 1
    assert parsed.raw_truncated is True


def test_fault_raw_capture_is_bounded_at_4096_bytes():
    raw = b"{" + b"x" * (MAX_RAW + 100)

    parsed = parse(raw)

    assert parsed.validation == "invalid_json"
    assert len(parsed.raw) == MAX_RAW
    assert parsed.raw == raw[:MAX_RAW]
    assert parsed.raw_total == len(raw)
    assert parsed.raw_truncated is True


def test_ok_turn_reply_accepts_missing_info_and_unknown_top_level_keys():
    parsed = parse(raw_turn(future_key={"anything": True}))

    assert parsed.validation == "ok"
    assert parsed.bid == 7
    assert parsed.move == (4, 2)
    assert parsed.info is None
    assert parsed.raw is None
    assert to_submission(parsed) == Submission(fault=None, bid=7, move=(4, 2))


def test_ok_turn_reply_preserves_arbitrary_object_info():
    info = {"quality": "estimate", "pv": [[4, 2]], "vendor": {"x": 1}}

    parsed = parse(raw_turn(info=info))

    assert parsed.validation == "ok"
    assert parsed.info == info


def test_info_must_be_an_object_within_canonical_size_limit():
    assert parse(raw_turn(info=[])).validation == "schema_violation"
    oversized = {"payload": "x" * MAX_INFO}
    assert parse(raw_turn(info=oversized)).validation == "schema_violation"


def test_turn_request_matches_design_field_for_field():
    request = turn_request(
        game_id="g1",
        request_id=REQUEST_ID,
        ply=0,
        attempt=1,
        you="X",
        pos=INITIAL,
        budgets={"X": 1_000_000_000, "O": 1_000_000_000},
        tie_owner=None,
        time_ms=30_000,
    )

    assert request == {
        "type": "turn",
        "protocol": 1,
        "game_id": "g1",
        "request_id": REQUEST_ID,
        "ply": 0,
        "attempt": 1,
        "you": "X",
        "board": ["........."] * 9,
        "forced": 4,
        "legal": [[4, cell] for cell in range(9)],
        "budgets": {"X": 1_000_000_000, "O": 1_000_000_000},
        "tie_owner": None,
        "time_ms": 30_000,
    }


def test_hello_and_game_end_builders_use_canonical_wire_keys():
    assert hello_request(
        game_id="g1", you="O", time_ms=30_000, grace_ms=250, budget=10**9
    ) == {
        "type": "hello",
        "protocol": 1,
        "game_id": "g1",
        "you": "O",
        "rules": "poorman-uttt-v1",
        "time_ms": 30_000,
        "grace_ms": 250,
        "budget": 10**9,
    }
    assert game_end_msg(
        result="X", reason="macro_win", budgets={"X": 5, "O": 2}
    ) == {
        "type": "game_end",
        "protocol": 1,
        "result": "X",
        "reason": "macro_win",
        "budgets": {"X": 5, "O": 2},
    }


def test_hello_reply_accepts_optional_author_and_unknown_keys():
    raw = canonical_dumps(
        {
            "type": "hello",
            "protocol": 1,
            "name": "stub",
            "version": "1.2.3",
            "author": "A",
            "future": 9,
        }
    ).encode()

    parsed = parse_hello_reply(raw)

    assert parsed.validation == "ok"
    assert parsed.name == "stub"
    assert parsed.version == "1.2.3"
    assert parsed.raw is None


@pytest.mark.parametrize(
    ("raw", "fault"),
    [
        (b"\xff", "invalid_utf8"),
        (b"{", "invalid_json"),
        (json.dumps({"type": "hello", "protocol": 1, "name": "x"}).encode(), "schema_violation"),
    ],
)
def test_hello_reply_faults_have_bounded_raw_capture(raw, fault):
    parsed = parse_hello_reply(raw)

    assert parsed.validation == fault
    assert parsed.raw == raw[:MAX_RAW]
    assert parsed.raw_total == len(raw)


def test_canonical_dumps_is_compact_unicode_and_key_sorted():
    assert canonical_dumps({"z": "é", "a": [2, 1]}) == '{"a":[2,1],"z":"é"}'
