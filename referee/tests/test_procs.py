import os
import signal
import sys
import time
from pathlib import Path

import pytest

from poorman_referee.procs import Engine, collect_both
from poorman_referee.protocol import ParsedReply, hello_request, parse_turn_reply


STUB = Path(__file__).with_name("stub_engine.py")


def command(*args):
    return [sys.executable, str(STUB), *args]


def hello_req(seat="X"):
    return hello_request(
        game_id="g1", you=seat, time_ms=200, grace_ms=25, budget=100
    )


def turn_req(seat="X", request_id="g1-p0-a1"):
    return {
        "type": "turn",
        "protocol": 1,
        "game_id": "g1",
        "request_id": request_id,
        "ply": 0,
        "attempt": 1,
        "you": seat,
        "board": ["........."] * 9,
        "forced": 4,
        "legal": [[4, 0], [4, 1], [4, 2]],
        "budgets": {"X": 100, "O": 100},
        "tie_owner": None,
        "time_ms": 200,
    }


def parser_for(reqs):
    def parse(seat, raw, status):
        if status is not None:
            captured = b"" if raw is None else raw
            return ParsedReply(status, None, None, None, captured, len(captured), False)
        return parse_turn_reply(
            raw or b"",
            request_id=reqs[seat]["request_id"],
            budget=reqs[seat]["budgets"][seat],
            legal=[tuple(move) for move in reqs[seat]["legal"]],
        )

    return parse


def started_engine(seat="X", *args, **kwargs):
    engine = Engine(command(*args), seat, **kwargs)
    engine.start()
    return engine


def assert_group_gone(pgid):
    with pytest.raises(ProcessLookupError):
        os.killpg(pgid, 0)


def test_clean_hello_returns_identity_and_elapsed():
    engine = started_engine()
    try:
        parsed, elapsed = engine.hello(hello_req(), 500)
        assert parsed.validation == "ok"
        assert (parsed.name, parsed.version) == ("stub", "1")
        assert elapsed >= 0
    finally:
        engine.kill()


def test_collect_both_writes_before_reading_and_returns_ok():
    engines = {"X": started_engine("X"), "O": started_engine("O")}
    reqs = {seat: turn_req(seat) for seat in engines}
    try:
        for seat, engine in engines.items():
            assert engine.hello(hello_req(seat), 500)[0].validation == "ok"
        replies = collect_both(engines, reqs, parser_for(reqs), 200, 25)
        assert set(replies) == {"X", "O"}
        assert all(reply.validation == "ok" for reply, _ in replies.values())
        assert all(elapsed >= 0 for _, elapsed in replies.values())
    finally:
        for engine in engines.values():
            engine.kill()


@pytest.mark.parametrize(
    ("fault", "expected"),
    [("timeout:1", "timeout"), ("die:1", "eof_or_crash")],
)
def test_collection_classifies_timeout_and_crash(fault, expected):
    engines = {
        "X": started_engine("X", "--fault", fault),
        "O": started_engine("O"),
    }
    reqs = {seat: turn_req(seat) for seat in engines}
    try:
        for seat, engine in engines.items():
            engine.hello(hello_req(seat), 500)
        reply, elapsed = collect_both(engines, reqs, parser_for(reqs), 100, 25)["X"]
        assert reply.validation == expected
        assert elapsed >= 0
    finally:
        for engine in engines.values():
            engine.kill()


def test_oversize_without_lf_is_classified_before_timeout():
    engine = started_engine("X", "--fault", "oversize_nolf:1")
    try:
        engine.hello(hello_req(), 500)
        assert engine.send_line(turn_req()) is None
        started = time.monotonic()
        raw, status = engine.read_reply(started + 1)
        assert status == "oversize_line"
        assert len(raw) > 32768
        assert time.monotonic() - started < 0.8
    finally:
        engine.kill()


def test_same_write_extra_line_is_quarantined():
    engine = started_engine("X", "--fault", "extra_line:1")
    try:
        engine.hello(hello_req(), 500)
        assert engine.send_line(turn_req()) is None
        _, status = engine.read_reply(time.monotonic() + 1)
        assert status == "extra_protocol_line"
        assert engine.sweep_extra() is False
    finally:
        engine.kill()


def test_final_sweep_deterministically_catches_extra_while_companion_is_open():
    engines = {
        "X": started_engine("X", "--fault", "extra_line_before_sweep:1"),
        "O": started_engine("O", "--fault", "timeout:1"),
    }
    reqs = {seat: turn_req(seat) for seat in engines}
    try:
        for seat, engine in engines.items():
            engine.hello(hello_req(seat), 500)
        replies = collect_both(engines, reqs, parser_for(reqs), 100, 25)
        assert replies["X"][0].validation == "extra_protocol_line"
        assert replies["O"][0].validation == "timeout"
    finally:
        for engine in engines.values():
            engine.kill()


@pytest.mark.parametrize("fault", ["unsolicited_between_plies:1", "partial_between_plies:1"])
def test_pre_send_boundary_faults_delayed_output_without_writing(fault):
    engine = started_engine("X", "--fault", fault)
    try:
        engine.hello(hello_req(), 500)
        assert engine.send_line(turn_req()) is None
        assert engine.read_reply(time.monotonic() + 1)[1] is None
        assert engine.sweep_extra() is False
        time.sleep(0.15)
        assert engine.send_line(turn_req(request_id="g1-p1-a1")) == "extra_protocol_line"
    finally:
        engine.kill()


def test_fault_buffer_is_clean_after_classification():
    engine = started_engine("X", "--fault", "extra_line:1")
    try:
        engine.hello(hello_req(), 500)
        engine.send_line(turn_req())
        assert engine.read_reply(time.monotonic() + 1)[1] == "extra_protocol_line"
        assert engine.send_line(turn_req(request_id="g1-p1-a1")) is None
        assert engine.read_reply(time.monotonic() + 1)[1] is None
    finally:
        engine.kill()


def test_finish_live_engine_honors_nondefault_grace_and_reaps_group():
    engine = started_engine(
        "X", "--exit-delay-ms", "100", shutdown_grace_ms=300
    )
    engine.hello(hello_req(), 500)
    pgid = engine.process.pid
    started = time.monotonic()
    assert engine.finish({"type": "game_end", "protocol": 1}) == "ok"
    elapsed = time.monotonic() - started
    assert 0.08 <= elapsed < 0.3
    assert engine.process.poll() is not None
    assert_group_gone(pgid)


def test_finish_dead_engine_fails_and_kill_reaps_process_group():
    engine = started_engine("X", "--fault", "die:1")
    engine.hello(hello_req(), 500)
    pgid = engine.process.pid
    engine.send_line(turn_req())
    assert engine.read_reply(time.monotonic() + 1)[1] == "eof_or_crash"
    assert engine.finish({"type": "game_end", "protocol": 1}) == "failed"
    assert engine.process.poll() is not None
    assert_group_gone(pgid)


def test_kill_leaves_no_child_or_process_group():
    engine = started_engine()
    pgid = engine.process.pid
    engine.kill()
    assert engine.process.poll() is not None
    assert_group_gone(pgid)


def test_stderr_capture_is_capped():
    engine = started_engine("X", "--spam-stderr", "100000", stderr_cap=1024)
    try:
        engine.hello(hello_req(), 500)
        deadline = time.monotonic() + 1
        while engine.stderr_total < 100000 and time.monotonic() < deadline:
            time.sleep(0.01)
        assert len(engine.stderr) == 1024
        assert engine.stderr_total == 100000
        assert engine.stderr_truncated is True
    finally:
        engine.kill()
