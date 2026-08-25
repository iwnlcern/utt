import json
import sys
from pathlib import Path

import pytest
from jsonschema import Draft202012Validator

from poorman_referee.gamelog import read_log, replay_frames
from poorman_referee.referee import GameConfig, play_game


STUB = Path(__file__).with_name("stub_engine.py")
SCHEMAS = Path(__file__).parents[2] / "docs" / "protocol" / "schema"
FAULT_CASES = {
    "timeout": "timeout",
    "eof_or_crash": "die",
    "invalid_utf8": "invalid_utf8",
    "invalid_json": "bad_json",
    "schema_violation": "schema",
    "wrong_request_id": "wrong_id",
    "extra_protocol_line": "extra_line",
    "oversize_line": "oversize_nolf",
    "illegal_bid": "illegal_bid",
    "illegal_move": "illegal_move",
}


def cmd(*args):
    return [sys.executable, str(STUB), *args]


def config(tmp_path, x_cmd=None, o_cmd=None, *, coin="X", name="game"):
    return GameConfig(
        cmds={"X": x_cmd or cmd("--seed", "1"), "O": o_cmd or cmd("--seed", "2")},
        engine_ids={"X": "engine-X", "O": "engine-O"},
        tournament_id="test-tourney",
        pair_seed=bytes.fromhex("10" * 32),
        game_seed=bytes.fromhex("20" * 32),
        pair_coin_seat=coin,
        time_ms=150,
        grace_ms=25,
        hello_timeout_ms=300,
        shutdown_grace_ms=300,
        log_path=str(tmp_path / f"{name}.jsonl"),
    )


def validate_log(events):
    for event in events:
        schema_name = {
            "game_start": "log_game_start",
            "auction": "log_auction",
            "recovery": "log_recovery",
            "game_end": "log_game_end",
        }[event["event"]]
        with (SCHEMAS / f"{schema_name}.schema.json").open(encoding="utf-8") as fh:
            Draft202012Validator(json.load(fh)).validate(event)


def auctions(events):
    return [event for event in events if event["event"] == "auction"]


def test_clean_stub_game_completes_and_log_replays(tmp_path):
    cfg = config(
        tmp_path,
        cmd("--seed", "1", "--exit-delay-ms", "30"),
        cmd("--seed", "2", "--exit-delay-ms", "30"),
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert result.result in {"X", "O"}
    assert result.reason in {"macro_win", "chip_count"}
    assert result.plies == len(auctions(events))
    assert events[-1]["delivery"] == {"X": "ok", "O": "ok"}
    validate_log(events)
    replay = replay_frames(events)
    assert replay.end["result"] == result.result
    assert len(replay.frames) == result.plies


@pytest.mark.parametrize("fault_class", list(FAULT_CASES))
def test_each_single_fault_loses_auction_at_zero_cost_and_recovers(
    tmp_path, fault_class
):
    cfg = config(
        tmp_path,
        cmd("--fault", f"{FAULT_CASES[fault_class]}:1", "--seed", "1"),
        cmd("--seed", "2"),
        name=fault_class,
    )

    play_game(cfg)
    events = read_log(cfg.log_path)
    first = auctions(events)[0]
    recoveries = [event for event in events if event["event"] == "recovery"]

    assert first["resolution"]["winner"] == "O"
    assert first["resolution"]["reason"] == "fault"
    assert first["resolution"]["payment"] == 0
    assert first["attempts"][0]["turns"]["X"]["validation"] == fault_class
    assert recoveries[0]["seat"] == "X"
    assert recoveries[0]["fault"] == fault_class
    assert len(auctions(events)) > 1
    validate_log(events)


def test_one_double_fault_recovers_both_then_retries_attempt_two(tmp_path):
    x_once = tmp_path / "x-once"
    o_once = tmp_path / "o-once"
    cfg = config(
        tmp_path,
        cmd("--fault", "bad_json:1", "--fault-once-file", str(x_once)),
        cmd("--fault", "bad_json:1", "--fault-once-file", str(o_once)),
        name="double-once",
    )

    play_game(cfg)
    events = read_log(cfg.log_path)
    first = auctions(events)[0]

    assert [attempt["attempt"] for attempt in first["attempts"]] == [1, 2]
    assert all(
        first["attempts"][0]["turns"][seat]["validation"] == "invalid_json"
        for seat in ("X", "O")
    )
    assert first["outcome"] == "resolved"
    validate_log(events)


def test_three_double_faults_void_the_game(tmp_path):
    cfg = config(
        tmp_path,
        cmd("--fault", "bad_json:1"),
        cmd("--fault", "bad_json:1"),
        name="triple-double",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert result.result == "void"
    assert result.reason == "triple_double_fault_void"
    assert auctions(events)[0]["outcome"] == "voided"
    assert len(auctions(events)[0]["attempts"]) == 3
    validate_log(events)


def test_failed_restart_hello_aborts_nonterminal_game(tmp_path):
    marker = tmp_path / "x-started"
    cfg = config(
        tmp_path,
        cmd(
            "--fault",
            "bad_json:1",
            "--die-on-hello-after-restart",
            str(marker),
        ),
        cmd(),
        name="recovery-fault",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert (result.result, result.reason) == ("O", "recovery_fault")
    assert auctions(events)[0]["outcome"] == "resolved"
    assert events[-2]["event"] == "recovery"
    assert events[-2]["hello"]["validation"] == "eof_or_crash"
    validate_log(events)


def test_double_fault_restart_failure_emits_aborted_auction(tmp_path):
    x_marker = tmp_path / "x-restart"
    o_once = tmp_path / "o-fault-once"
    cfg = config(
        tmp_path,
        cmd(
            "--fault",
            "bad_json:1",
            "--die-on-hello-after-restart",
            str(x_marker),
        ),
        cmd("--fault", "bad_json:1", "--fault-once-file", str(o_once)),
        name="double-recovery-abort",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert (result.result, result.reason) == ("O", "recovery_fault")
    assert auctions(events)[0]["outcome"] == "aborted_recovery_fault"
    assert "resolution" not in auctions(events)[0]
    assert [e["seat"] for e in events if e["event"] == "recovery"] == ["X", "O"]
    validate_log(events)


def test_both_restart_hellos_failing_voids_double_fault_game(tmp_path):
    x_marker = tmp_path / "x-restart"
    o_marker = tmp_path / "o-restart"
    cfg = config(
        tmp_path,
        cmd("--fault", "bad_json:1", "--die-on-hello-after-restart", str(x_marker)),
        cmd("--fault", "bad_json:1", "--die-on-hello-after-restart", str(o_marker)),
        name="both-recovery-fail",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert (result.result, result.reason) == ("void", "recovery_fault")
    assert auctions(events)[0]["outcome"] == "aborted_recovery_fault"
    assert events[-1]["result"] == "void"
    validate_log(events)


def test_fault_won_terminal_move_is_not_rewritten_by_restart_failure(tmp_path):
    clean = config(tmp_path, cmd("--seed", "11"), cmd("--seed", "22"), name="probe")
    clean_result = play_game(clean)
    clean_events = read_log(clean.log_path)
    terminal = auctions(clean_events)[-1]
    winner = terminal["resolution"]["winner"]
    faulter = "O" if winner == "X" else "X"
    terminal_turn = terminal["ply"] + 1
    marker = tmp_path / "terminal-started"
    fault_cmd = cmd(
        "--seed",
        "22" if faulter == "O" else "11",
        "--fault",
        f"bad_json:{terminal_turn}",
        "--die-on-hello-after-restart",
        str(marker),
    )
    cfg = config(
        tmp_path,
        fault_cmd if faulter == "X" else cmd("--seed", "11"),
        fault_cmd if faulter == "O" else cmd("--seed", "22"),
        name="terminal-fault",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert result == clean_result
    assert auctions(events)[-1]["resolution"]["reason"] == "fault"
    assert not [e for e in events if e["event"] == "recovery" and e["ply"] == terminal["ply"]]
    assert all(e.get("reason") != "recovery_fault" for e in events)
    validate_log(events)


def test_tie_coin_owner_transitions_and_integer_budgets(tmp_path):
    cfg = config(
        tmp_path,
        cmd("--bid", "3", "--echo-request", "--seed", "3"),
        cmd("--bid", "3", "--echo-request", "--seed", "4"),
        coin="O",
        name="ties",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)
    first_three = auctions(events)[:3]

    assert first_three[0]["resolution"]["reason"] == "tie_coin"
    assert first_three[0]["resolution"]["winner"] == "O"
    assert [a["resolution"]["winner"] for a in first_three] == ["O", "X", "O"]
    assert first_three[0]["budgets_after"] == {"X": 10**9, "O": 10**9 - 3}
    echoed = [a["attempts"][0]["turns"]["X"]["info"]["request_tie_owner"] for a in first_three]
    assert echoed == [None, "X", "O"]
    assert all(type(value) is int for value in result.budgets.values())
    validate_log(events)


def test_consecutive_same_winner_keeps_tie_owner_at_opponent(tmp_path):
    cfg = config(
        tmp_path,
        cmd("--bid", "2", "--echo-request", "--seed", "5"),
        cmd("--bid", "1", "--echo-request", "--seed", "6"),
        name="same-winner",
    )

    play_game(cfg)
    first_three = auctions(read_log(cfg.log_path))[:3]

    assert [a["resolution"]["winner"] for a in first_three] == ["X", "X", "X"]
    echoed = [a["attempts"][0]["turns"]["X"]["info"]["request_tie_owner"] for a in first_three]
    assert echoed == [None, "O", "O"]


def test_fault_applied_move_sets_next_tie_owner_to_movers_opponent(tmp_path):
    cfg = config(
        tmp_path,
        cmd("--fault", "bad_json:1", "--echo-request", "--seed", "5"),
        cmd("--echo-request", "--seed", "6"),
        name="fault-owner",
    )

    play_game(cfg)
    first_two = auctions(read_log(cfg.log_path))[:2]

    assert first_two[0]["resolution"]["winner"] == "O"
    assert first_two[0]["resolution"]["reason"] == "fault"
    assert first_two[1]["attempts"][0]["turns"]["X"]["info"]["request_tie_owner"] == "X"


def test_hello_crash_is_forfeit_and_failed_terminal_delivery(tmp_path):
    cfg = config(
        tmp_path,
        cmd("--fault", "hello_die:1"),
        cmd(),
        name="hello-die",
    )

    result = play_game(cfg)
    events = read_log(cfg.log_path)

    assert (result.result, result.reason, result.plies) == ("O", "hello_fault", 0)
    assert events[-1]["delivery"] == {"X": "failed", "O": "ok"}
    validate_log(events)
