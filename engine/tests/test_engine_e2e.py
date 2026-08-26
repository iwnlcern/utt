#!/usr/bin/env python3

import json
import os
import select
import subprocess
import sys
import tempfile
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

# Corpus locations PINNED by the contract owner (harness-c1-contract-rulings-3,
# which supersedes rulings-2's flat glob;
# under adopted option A / s1-corpus-ruling-1). Files under engine-stdin/ are
# raw replayable stdin bytes: hello_request, every turn_request (including
# re-request attempts), then game_end — no envelope. If harness ever re-pins,
# update ONLY these constants.
TRANSCRIPT_PATH = ("docs", "protocol", "transcript-v1.jsonl")
CORPUS_GLOB = "referee/tests/fixtures/engine-stdin/**/*.jsonl"  # pinned RECURSIVE (rulings-3: bijective subdir-preserving mapping; the engine-stdin subtree holds only stream files)


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


def rss_kib(process: subprocess.Popen[str]) -> int:
    output = subprocess.check_output(
        ["ps", "-o", "rss=", "-p", str(process.pid)], text=True
    )
    return int(output.strip())


def assert_silent_and_alive(process: subprocess.Popen[str], message: str) -> None:
    assert process.stdout is not None
    ready, _, _ = select.select([process.stdout], [], [], 0.2)
    assert not ready, f"{message} produced stdout"
    assert process.poll() is None, f"{message} killed the engine"


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
    assert_silent_and_alive(process, "malformed input")

    # The reader must stop buffering at 32 KiB while it drains an overlong line.
    # A getline-based implementation retains this entire 64 MiB partial line.
    assert process.stdin is not None
    chunk = "x" * (1024 * 1024)
    for _ in range(64):
        process.stdin.write(chunk)
        process.stdin.flush()
    assert rss_kib(process) < 32 * 1024, "oversize partial line was buffered in memory"
    process.stdin.write("\n")
    process.stdin.flush()
    assert_silent_and_alive(process, "oversize line")

    send(process, "[" * 512 + "0" + "]" * 512)
    assert_silent_and_alive(process, "deeply nested JSON")

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


def strict_corpus_messages(path: Path) -> tuple[list[bytes], list[dict]]:
    def producer_defect(detail: str) -> None:
        raise AssertionError(f"harness message-stream producer defect: {path}: {detail}")

    content = path.read_bytes()
    parts = content.split(b"\n")
    raw_lines = [part + b"\n" for part in parts[:-1]]
    if parts[-1]:
        raw_lines.append(parts[-1])
    if not raw_lines:
        producer_defect("stream is empty")

    messages: list[dict] = []
    for line_number, raw_bytes in enumerate(raw_lines, start=1):
        try:
            raw = raw_bytes.decode("utf-8")
        except UnicodeDecodeError as error:
            producer_defect(f"line {line_number} is not UTF-8: {error.reason}")
        if not raw.strip():
            producer_defect(f"line {line_number} is blank")
        try:
            value = json.loads(raw)
        except json.JSONDecodeError as error:
            producer_defect(f"line {line_number} is malformed JSON: {error.msg}")
        if not isinstance(value, dict) or value.get("type") not in {
            "hello",
            "turn",
            "game_end",
        }:
            producer_defect(
                f"line {line_number} is not a bare hello/turn/game_end message"
            )
        messages.append(value)

    types = [message["type"] for message in messages]
    if types[0] != "hello":
        producer_defect("stream must begin with hello")
    if types.count("hello") != 1:
        producer_defect("stream must contain exactly one hello")
    if types[-1] != "game_end":
        if "game_end" in types:
            producer_defect("stream contains content after game_end")
        producer_defect("stream ended without game_end")
    if any(message_type != "turn" for message_type in types[1:-1]):
        producer_defect("only turn messages are allowed between hello and game_end")
    return raw_lines, messages


def discover_corpus(repository: Path) -> list[Path]:
    transcript = repository.joinpath(*TRANSCRIPT_PATH)
    paths = [transcript] if transcript.is_file() else []
    paths.extend(repository.glob(CORPUS_GLOB))
    return sorted(paths)


def replay_messages(
    binary: Path,
    path: Path,
    messages: list[dict],
    raw_lines: list[bytes] | None = None,
) -> None:
    if raw_lines is not None:
        assert len(raw_lines) == len(messages)
    process = spawn(binary)
    try:
        for index, message in enumerate(messages):
            if raw_lines is None:
                send(process, message)
            else:
                assert process.stdin is not None
                written = process.stdin.buffer.write(raw_lines[index])
                assert written == len(raw_lines[index]), "corpus stdin write was partial"
                process.stdin.buffer.flush()
            if message["type"] == "hello":
                assert_hello_reply(read_json_line(process))
            elif message["type"] == "turn":
                assert_turn_reply(read_json_line(process), message)
            elif message["type"] == "game_end":
                assert process.stdin is not None
                process.stdin.close()
                assert process.wait(timeout=2.0) == 0
                break
        else:
            assert process.stdin is not None
            process.stdin.close()
            assert process.wait(timeout=2.0) == 0
        assert process.stdout is not None
        assert process.stdout.read() == "", f"corpus run produced an extra protocol line: {path}"
    except BaseException:
        if process.poll() is None:
            process.kill()
            process.wait()
        raise


def run_corpus(binary: Path, repository: Path) -> None:
    transcript = repository.joinpath(*TRANSCRIPT_PATH)
    paths = discover_corpus(repository)
    stream_paths = [path for path in paths if path != transcript]

    if transcript in paths:
        messages = corpus_messages(transcript)
        if messages:
            replay_messages(binary, transcript, messages)
        else:
            print(
                f"skipping legacy transcript with no referee-to-engine messages: {transcript}",
                file=sys.stderr,
            )

    if not stream_paths:
        message = "harness message-stream corpus not present — criterion 3 pending-blocked"
        if os.environ.get("UTTT_ALLOW_MISSING_CORPUS") == "1":
            print(message, file=sys.stderr)
            return
        raise AssertionError(message)

    for path in stream_paths:
        raw_lines, messages = strict_corpus_messages(path)
        replay_messages(binary, path, messages, raw_lines)


def run_discovery_selftest(binary: Path) -> None:
    with tempfile.TemporaryDirectory() as temporary:
        repository = Path(temporary)
        transcript = repository / "docs" / "protocol" / "transcript-v1.jsonl"
        transcript.parent.mkdir(parents=True)
        transcript.write_text('{"type":"game_event"}\n', encoding="utf-8")

        corpus = repository / "referee" / "tests" / "fixtures" / "engine-stdin"
        corpus.mkdir(parents=True)
        raw_stream = "".join(
            json.dumps(message, separators=(",", ":")) + "\n"
            for message in (HELLO, TURN, GAME_END)
        )
        synthetic = corpus / "synthetic.X.jsonl"
        synthetic.write_text(raw_stream, encoding="utf-8")
        nested = corpus / "parity-even"
        nested.mkdir()
        nested_stream = nested / "deep.X.jsonl"
        nested_stream.write_text(raw_stream, encoding="utf-8")
        (corpus / "notes.stderr").write_text(raw_stream, encoding="utf-8")
        decoy_log = corpus.parent / "decoy-log.jsonl"
        decoy_log.write_text(raw_stream, encoding="utf-8")

        # Mutation guard: a misspelled pin misses synthetic.X.jsonl; flattening
        # the pin to single-level *.jsonl misses parity-even/deep.X.jsonl.
        assert discover_corpus(repository) == sorted([transcript, synthetic, nested_stream])
        assert corpus_messages(transcript) == []
        run_corpus(binary, repository)

        wrapped = corpus / "wrapped.X.jsonl"
        wrapped.write_text(
            "".join(
                json.dumps(
                    {"direction": "referee_to_engine", "message": message},
                    separators=(",", ":"),
                )
                + "\n"
                for message in (HELLO, TURN, GAME_END)
            ),
            encoding="utf-8",
        )
        try:
            strict_corpus_messages(wrapped)
        except AssertionError as error:
            assert "harness message-stream producer defect" in str(error)
        else:
            raise AssertionError("direction-wrapped engine-stdin stream was accepted")

        oversize = corpus / "oversize.X.jsonl"
        compact_hello = json.dumps(HELLO, separators=(",", ":"))
        padded_hello = compact_hello + " " * (32_769 - len(compact_hello))
        oversize.write_text(
            padded_hello
            + "\n"
            + json.dumps(TURN, separators=(",", ":"))
            + "\n"
            + json.dumps(GAME_END, separators=(",", ":"))
            + "\n",
            encoding="utf-8",
        )
        raw_lines, messages = strict_corpus_messages(oversize)
        try:
            replay_messages(binary, oversize, messages, raw_lines)
        except AssertionError as error:
            assert str(error) == "engine did not emit a protocol line before timeout"
        else:
            raise AssertionError("corpus replay normalized rather than preserving raw stdin bytes")

        no_final_newline = corpus / "no-final-newline.X.jsonl"
        no_final_newline.write_text(raw_stream.rstrip("\n"), encoding="utf-8")
        raw_lines, messages = strict_corpus_messages(no_final_newline)
        replay_messages(binary, no_final_newline, messages, raw_lines)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit("usage: test_engine_e2e.py ENGINE_BINARY REPOSITORY_ROOT")
    binary = Path(sys.argv[1]).resolve()
    repository = Path(sys.argv[2]).resolve()
    run_basic(binary)
    run_discovery_selftest(binary)
    run_corpus(binary, repository)


if __name__ == "__main__":
    main()
