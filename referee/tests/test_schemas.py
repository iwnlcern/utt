import copy
import json
from pathlib import Path

import pytest
from jsonschema import Draft202012Validator, ValidationError

from poorman_referee.protocol import game_end_msg, hello_request, turn_request
from poorman_referee.rules import INITIAL


SCHEMA_ROOT = Path(__file__).parents[2] / "docs" / "protocol" / "schema"


def load_schema(name):
    with (SCHEMA_ROOT / f"{name}.schema.json").open(encoding="utf-8") as fh:
        schema = json.load(fh)
    Draft202012Validator.check_schema(schema)
    return schema


def validate(name, value):
    Draft202012Validator(load_schema(name)).validate(value)


def message_examples():
    return {
        "hello_request": hello_request(
            game_id="g1", you="X", time_ms=30_000, grace_ms=250, budget=10**9
        ),
        "hello_reply": {
            "type": "hello",
            "protocol": 1,
            "name": "stub",
            "version": "1.0",
        },
        "turn_request": turn_request(
            game_id="g1",
            request_id="g1-p0-a1",
            ply=0,
            attempt=1,
            you="X",
            pos=INITIAL,
            budgets={"X": 10**9, "O": 10**9},
            tie_owner=None,
            time_ms=30_000,
        ),
        "turn_reply": {
            "type": "turn",
            "protocol": 1,
            "request_id": "g1-p0-a1",
            "bid": 0,
            "move": [4, 4],
        },
        "game_end": game_end_msg(
            result="X", reason="macro_win", budgets={"X": 10, "O": 9}
        ),
    }


def turn_record(validation="ok"):
    record = {"validation": validation, "elapsed_ms": 2}
    if validation == "ok":
        record.update({"bid": 0, "move": [4, 4], "info": {"vendor": True}})
    else:
        record["raw"] = {"b64": "eA==", "truncated": False, "bytes_total": 1}
    return record


def hello_record():
    return {"validation": "ok", "name": "stub", "version": "1", "elapsed_ms": 1}


def log_examples():
    board = ["........."] * 9
    start = {
        "event": "game_start",
        "log_version": 1,
        "protocol": 1,
        "rules": "poorman-uttt-v1",
        "game_id": "g1",
        "tournament_id": None,
        "engines": {
            "X": {"engine_id": "A", "cmd": ["python", "a.py"]},
            "O": {"engine_id": "B", "cmd": ["python", "b.py"]},
        },
        "pair_seed": "00" * 32,
        "game_seed": "11" * 32,
        "pair_coin_seat": "X",
        "hellos": {"X": hello_record(), "O": hello_record()},
        "time_control": {
            "time_ms": 30_000,
            "grace_ms": 250,
            "hello_timeout_ms": 10_000,
            "shutdown_grace_ms": 2_000,
        },
        "budgets": {"X": 10**9, "O": 10**9},
    }
    auction = {
        "event": "auction",
        "ply": 0,
        "outcome": "resolved",
        "attempts": [
            {
                "request_id": "g1-p0-a1",
                "attempt": 1,
                "turns": {"X": turn_record(), "O": turn_record()},
            }
        ],
        "resolution": {
            "winner": "X",
            "reason": "tie_coin",
            "coin": "X",
            "payment": 0,
            "move": [4, 4],
            "closures": [],
            "macro_line": None,
            "forced_next": 4,
        },
        "budgets_after": {"X": 10**9, "O": 10**9},
        "post_board": [*board[:4], "....X....", *board[5:]],
    }
    recovery = {
        "event": "recovery",
        "seat": "X",
        "ply": 0,
        "trigger_request_id": "g1-p0-a1",
        "fault": "timeout",
        "hello": hello_record(),
    }
    end = {
        "event": "game_end",
        "result": "X",
        "reason": "macro_win",
        "budgets": {"X": 5, "O": 2},
        "budget_margin": 3,
        "plies": 21,
        "delivery": {"X": "ok", "O": "failed"},
    }
    return {
        "log_game_start": start,
        "log_auction": auction,
        "log_recovery": recovery,
        "log_game_end": end,
    }


@pytest.mark.parametrize("name", list(message_examples()))
def test_codec_message_examples_validate_and_allow_unknown_keys(name):
    example = message_examples()[name]
    validate(name, example)

    extended = copy.deepcopy(example)
    extended["future_key"] = {"ignored": True}
    validate(name, extended)


@pytest.mark.parametrize(
    ("name", "missing_key", "typed_key", "bad_value"),
    [
        ("hello_request", "game_id", "time_ms", "slow"),
        ("hello_reply", "version", "name", 3),
        ("turn_request", "legal", "ply", False),
        ("turn_reply", "move", "bid", 1.5),
        ("game_end", "result", "budgets", []),
    ],
)
def test_message_schemas_reject_only_required_or_typed_field_mutations(
    name, missing_key, typed_key, bad_value
):
    missing = copy.deepcopy(message_examples()[name])
    del missing[missing_key]
    with pytest.raises(ValidationError):
        validate(name, missing)

    wrong_type = copy.deepcopy(message_examples()[name])
    wrong_type[typed_key] = bad_value
    with pytest.raises(ValidationError):
        validate(name, wrong_type)


def test_turn_reply_schema_accepts_any_object_info():
    reply = message_examples()["turn_reply"]
    reply["info"] = {"unregistered": [1, {"shape": None}]}

    validate("turn_reply", reply)


@pytest.mark.parametrize("name", list(log_examples()))
def test_log_event_examples_validate_and_allow_unknown_keys(name):
    example = log_examples()[name]
    validate(name, example)

    extended = copy.deepcopy(example)
    extended["future_key"] = True
    validate(name, extended)


@pytest.mark.parametrize("outcome", ["voided", "aborted_recovery_fault"])
def test_nonresolved_auction_forbids_resolution(outcome):
    event = log_examples()["log_auction"]
    event["outcome"] = outcome
    event.pop("resolution")
    validate("log_auction", event)

    event["resolution"] = log_examples()["log_auction"]["resolution"]
    with pytest.raises(ValidationError):
        validate("log_auction", event)


def test_resolved_auction_requires_resolution():
    event = log_examples()["log_auction"]
    event.pop("resolution")

    with pytest.raises(ValidationError):
        validate("log_auction", event)


@pytest.mark.parametrize("required", ["seat", "ply", "trigger_request_id", "fault", "hello"])
def test_recovery_schema_requires_association_and_restart_fields(required):
    event = log_examples()["log_recovery"]
    event.pop(required)

    with pytest.raises(ValidationError):
        validate("log_recovery", event)
