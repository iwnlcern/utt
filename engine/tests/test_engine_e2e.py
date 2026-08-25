#!/usr/bin/env python3

import json
import os
import select
import subprocess
import sys
from pathlib import Path


HELLO = {
    "type": "hello",
    "protocol": 1,
    "game_id": "e2e-game",
    "you": "X",
    "rules": "poorman-uttt-v1",
    "time_ms": 1000,
    "grace_ms": 250,
    "budget": 1_000_000_000,
}
LEGAL = [[4, cell] for cell in range(9)]
TURN = {
    "type": "turn",
    "protocol": 1,
    "game_id": "e2e-game",
    "request_id": "e2e-r0",
    "ply": 0,
    "attempt": 1,
    "you": "X",
    "board": ["........."] * 9,
    "forced": 4,
    "legal": LEGAL,
    "budgets": {"X": 1_000_000_000, "O": 1_000_000_000},
    "tie_owner": None,
    "time_ms": 1000,
}
GAME_END = {
    "type": "game_end",
    "protocol": 1,
    "result": "void",
    "reason": "triple_double_fault_void",
    "budgets": {"X": 1_000_000_000, "O": 1_000_000_000},
}


def send(process: subprocess.Popen[str], value: object) -> None:
    assert process.stdin is not None
    line = value if isinstance(value, str) else json.dumps(value, separators=(",", ":"))
    process.stdin.write(line + "\n")
    process.stdin.flush()


def read_json_line(process: subprocess.Popen[str], timeout: float = 2.0) -> dict:
    assert process.stdout is not None
    ready, _, _ = select.select([process.stdout], [], [], timeout)
    assert ready, "engine did not emit a protocol line before timeout"
    line = process.stdout.readline()
    assert line, "engine stdout reached EOF while a reply was expected"
    return json.loads(line)


def assert_hello_reply(reply: dict) -> None:
    assert reply["type"] == "hello"
    assert reply["protocol"] == 1
    assert isinstance(reply["name"], str)
    assert isinstance(reply["version"], str)


def assert_turn_reply(reply: dict, request: dict) -> None:
    assert reply["type"] == "turn"
    assert reply["protocol"] == 1
    assert reply["request_id"] == request["request_id"]
    assert isinstance(reply["bid"], int) and not isinstance(reply["bid"], bool)
    assert 0 <= reply["bid"] <= request["budgets"][request["you"]]
    assert reply["move"] in request["legal"]
    assert reply["info"]["quality"] in {"exact", "bound", "estimate"}


def spawn(binary: Path) -> subprocess.Popen[str]:
    return subprocess.Popen(
        [str(binary)],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
    )


def run_basic(binary: Path) -> None:
    process = spawn(binary)
    send(process, HELLO)
    assert_hello_reply(read_json_line(process))
    send(process, TURN)
    reply = read_json_line(process)
    assert_turn_reply(reply, TURN)
    assert reply["bid"] == 0
    assert reply["info"]["quality"] == "estimate"

    send(process, '{"type":')
    assert process.stdout is not None
    ready, _, _ = select.select([process.stdout], [], [], 0.2)
    assert not ready, "malformed input produced stdout"
    assert process.poll() is None, "malformed input killed the engine"

    send(process, GAME_END)
    assert process.wait(timeout=2.0) == 0
    assert process.stdout.read() == "", "engine emitted extra stdout"


def corpus_messages(path: Path) -> list[dict]:
    messages: list[dict] = []
    for raw in path.read_text(encoding="utf-8").splitlines():
        if not raw.strip():
            continue
        value = json.loads(raw)
        if value.get("type") in {"hello", "turn", "game_end"}:
            messages.append(value)
        elif value.get("direction") in {"referee_to_engine", "referee->engine"}:
            message = value.get("message")
            if isinstance(message, dict):
                messages.append(message)
        elif isinstance(value.get("referee_to_engine"), dict):
            messages.append(value["referee_to_engine"])
        elif isinstance(value.get("referee_to_engine"), list):
            messages.extend(value["referee_to_engine"])
    return messages


def run_corpus(binary: Path, repository: Path) -> None:
    path = repository / "docs" / "protocol" / "transcript-v1.jsonl"
    if not path.is_file():
        if os.environ.get("UTTT_ALLOW_MISSING_CORPUS") == "1":
            print("harness conformance corpus not present — criterion 3 pending-blocked", file=sys.stderr)
            return
        raise AssertionError("harness conformance corpus not present — criterion 3 pending-blocked")
    messages = corpus_messages(path)
    if not messages:
        raise AssertionError("harness conformance corpus contains no referee-to-engine messages")

    process = spawn(binary)
    for message in messages:
        send(process, message)
        if message["type"] == "hello":
            assert_hello_reply(read_json_line(process))
        elif message["type"] == "turn":
            assert_turn_reply(read_json_line(process), message)
        elif message["type"] == "game_end":
            assert process.wait(timeout=2.0) == 0
            break
    else:
        assert process.stdin is not None
        process.stdin.close()
        assert process.wait(timeout=2.0) == 0
    assert process.stdout is not None
    assert process.stdout.read() == "", "corpus run produced an extra protocol line"


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("usage: test_engine_e2e.py ENGINE_BINARY REPOSITORY_ROOT")
    binary = Path(sys.argv[1]).resolve()
    repository = Path(sys.argv[2]).resolve()
    run_basic(binary)
    run_corpus(binary, repository)


if __name__ == "__main__":
    main()
