import os
import signal
import subprocess
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


def turn_req(seat="X", request_id="g1-p0-a1", ply=0):
    return {
        "type": "turn",
        "protocol": 1,
        "game_id": "g1",
        "request_id": request_id,
        "ply": ply,
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


def test_large_continuous_line_capture_is_bounded_at_framing_limit():
    engine = started_engine("X", "--fault", "flood_nolf:1")
    try:
        engine.hello(hello_req(), 500)
        engine.send_line(turn_req())
        raw, status = engine.read_reply(time.monotonic() + 1)
        assert status == "oversize_line"
        assert 32768 < len(raw) <= 32768 + 65536
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


def test_final_sweep_deterministically_catches_extra_while_companion_is_open(
    tmp_path,
):
    release = tmp_path / "release-extra"
    extra_written = tmp_path / "extra-written"
    engines = {
        "X": started_engine(
            "X",
            "--fault",
            "extra_line_before_sweep:1",
            "--sweep-release-file",
            str(release),
            "--sweep-extra-written-file",
            str(extra_written),
        ),
        "O": started_engine("O", "--wait-for-file", str(extra_written)),
    }
    reqs = {seat: turn_req(seat) for seat in engines}
    try:
        for seat, engine in engines.items():
            engine.hello(hello_req(seat), 500)

        original_read = engines["X"].read_reply

        def read_then_release(deadline):
            reply = original_read(deadline)
            release.write_text("read-complete\n", encoding="utf-8")
            return reply

        engines["X"].read_reply = read_then_release
        replies = collect_both(engines, reqs, parser_for(reqs), 100, 25)
        assert replies["X"][0].validation == "extra_protocol_line"
        assert replies["O"][0].validation == "ok"
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
        assert engine.send_line(turn_req(request_id="g1-p1-a1", ply=1)) == "extra_protocol_line"
    finally:
        engine.kill()


def test_fault_buffer_is_clean_after_classification():
    engine = started_engine("X", "--fault", "extra_line:1")
    try:
        engine.hello(hello_req(), 500)
        engine.send_line(turn_req())
        assert engine.read_reply(time.monotonic() + 1)[1] == "extra_protocol_line"
        assert engine.send_line(turn_req(request_id="g1-p1-a1", ply=1)) is None
        assert engine.read_reply(time.monotonic() + 1)[1] is None
    finally:
        engine.kill()


def test_finish_live_engine_honors_nondefault_grace_and_reaps_group():
    engine = started_engine(
        "X", "--exit-delay-ms", "5000", shutdown_grace_ms=300
    )
    engine.hello(hello_req(), 500)
    pgid = engine.process.pid
    started = time.monotonic()
    assert engine.finish({"type": "game_end", "protocol": 1}) == "ok"
    elapsed = time.monotonic() - started
    assert 0.25 <= elapsed < 0.8
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


def test_kill_signals_original_group_after_leader_has_exited(tmp_path):
    child_file = tmp_path / "child.pid"
    engine = started_engine(
        "X", "--fork-child-exit", "--child-pid-file", str(child_file)
    )
    deadline = time.monotonic() + 1
    while engine.process.poll() is None and time.monotonic() < deadline:
        time.sleep(0.01)
    assert engine.process.poll() is not None
    child_pid = int(child_file.read_text(encoding="utf-8"))

    engine.kill()

    deadline = time.monotonic() + 1
    while time.monotonic() < deadline:
        state = subprocess.run(
            ["ps", "-o", "state=", "-p", str(child_pid)],
            capture_output=True,
            text=True,
            check=False,
        ).stdout.strip()
        if state in ("", "Z"):
            break
        time.sleep(0.01)
    assert state in ("", "Z")
    assert engine._pgid is None


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
