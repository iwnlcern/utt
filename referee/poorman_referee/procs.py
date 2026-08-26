import os
import select
import signal
import subprocess
import threading
import time
from collections.abc import Callable
from typing import BinaryIO

from .protocol import (
    MAX_LINE,
    MAX_RAW,
    ParsedHello,
    ParsedReply,
    canonical_dumps,
    parse_hello_reply,
)


def _capture(raw: bytes) -> tuple[bytes, int, bool]:
    return raw[:MAX_RAW], len(raw), len(raw) > MAX_RAW


def _hello_fault(validation: str, raw: bytes | None) -> ParsedHello:
    captured, total, truncated = _capture(raw or b"")
    return ParsedHello(validation, None, None, captured, total, truncated)


class StderrSink:
    def __init__(self, cap: int):
        self.cap = cap
        self.data = bytearray()
        self.total = 0
        self.lock = threading.Lock()


class Engine:
    def __init__(
        self,
        cmd: list[str],
        seat: str,
        *,
        shutdown_grace_ms: int = 2000,
        stderr_cap: int = 65536,
        clock=time.monotonic,
        stderr_sink: StderrSink | None = None,
        stdin_capture: BinaryIO | None = None,
    ):
        self.cmd = list(cmd)
        self.seat = seat
        self.shutdown_grace_ms = shutdown_grace_ms
        self.stderr_cap = stderr_cap
        self.clock = clock
        self.process: subprocess.Popen | None = None
        self._pgid: int | None = None
        self._stdout_buffer = bytearray()
        self._stdout_eof = False
        self._last_fault_raw: bytes | None = None
        self._stderr_sink = stderr_sink or StderrSink(stderr_cap)
        self._stdin_capture = stdin_capture
        if self._stderr_sink.cap != stderr_cap:
            raise ValueError("shared stderr sink cap does not match stderr_cap")
        self._stderr_thread: threading.Thread | None = None

    @property
    def stderr(self) -> bytes:
        with self._stderr_sink.lock:
            return bytes(self._stderr_sink.data)

    @property
    def stderr_total(self) -> int:
        with self._stderr_sink.lock:
            return self._stderr_sink.total

    @property
    def stderr_truncated(self) -> bool:
        return self.stderr_total > self.stderr_cap

    def start(self) -> None:
        if self.process is not None and self.process.poll() is None:
            raise RuntimeError(f"engine {self.seat} is already running")
        self._stdout_buffer.clear()
        self._stdout_eof = False
        self._last_fault_raw = None
        self.process = subprocess.Popen(
            self.cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            start_new_session=True,
            bufsize=0,
        )
        self._pgid = self.process.pid
        if self.process.stdout is None or self.process.stderr is None:
            raise RuntimeError("failed to create engine pipes")
        os.set_blocking(self.process.stdout.fileno(), False)
        self._stderr_thread = threading.Thread(target=self._capture_stderr, daemon=True)
        self._stderr_thread.start()

    def _capture_stderr(self) -> None:
        process = self.process
        if process is None or process.stderr is None:
            return
        while True:
            try:
                chunk = process.stderr.read(8192)
            except (OSError, ValueError):
                return
            if not chunk:
                return
            with self._stderr_sink.lock:
                self._stderr_sink.total += len(chunk)
                remaining = self.stderr_cap - len(self._stderr_sink.data)
                if remaining > 0:
                    self._stderr_sink.data.extend(chunk[:remaining])

    def _drain_stdout(self) -> None:
        process = self.process
        if process is None or process.stdout is None:
            self._stdout_eof = True
            return
        fd = process.stdout.fileno()
        while True:
            remaining = MAX_LINE + 1 - len(self._stdout_buffer)
            if remaining <= 0:
                return
            try:
                chunk = os.read(fd, min(65536, remaining))
            except BlockingIOError:
                return
            except OSError:
                self._stdout_eof = True
                return
            if not chunk:
                self._stdout_eof = True
                return
            self._stdout_buffer.extend(chunk)

    def _discard_as_fault(self) -> bytes:
        raw = bytes(self._stdout_buffer)
        self._stdout_buffer.clear()
        self._last_fault_raw = raw
        return raw

    def take_fault_raw(self) -> bytes | None:
        raw = self._last_fault_raw
        self._last_fault_raw = None
        return raw

    def hello(self, req: dict, timeout_ms: int) -> tuple[ParsedHello, int]:
        started = self.clock()
        send_fault = self.send_line(req)
        if send_fault is not None:
            return _hello_fault(send_fault, self.take_fault_raw()), 0
        raw, framing_fault = self.read_reply(started + timeout_ms / 1000)
        elapsed = max(0, int((self.clock() - started) * 1000))
        if framing_fault is not None:
            return _hello_fault(framing_fault, raw), elapsed
        return parse_hello_reply(raw or b""), elapsed

    def send_line(self, obj: dict) -> str | None:
        process = self.process
        if process is None or process.stdin is None or process.poll() is not None:
            self._last_fault_raw = b""
            return "eof_or_crash"
        self._drain_stdout()
        if self._stdout_buffer:
            self._discard_as_fault()
            return "extra_protocol_line"
        if self._stdout_eof:
            self._last_fault_raw = b""
            return "eof_or_crash"
        payload = (canonical_dumps(obj) + "\n").encode("utf-8")
        remaining = memoryview(payload)
        try:
            fd = process.stdin.fileno()
            while remaining:
                written = os.write(fd, remaining)
                if written <= 0:
                    self._last_fault_raw = b""
                    return "eof_or_crash"
                remaining = remaining[written:]
        except (BrokenPipeError, OSError, ValueError):
            self._last_fault_raw = b""
            return "eof_or_crash"
        if self._stdin_capture is not None:
            try:
                captured = self._stdin_capture.write(payload)
                if captured != len(payload):
                    raise RuntimeError(
                        "stdin capture short write: "
                        f"expected {len(payload)} bytes, wrote {captured!r}"
                    )
                self._stdin_capture.flush()
            except RuntimeError:
                raise
            except Exception as exc:
                raise RuntimeError("stdin capture delivery failed") from exc
        return None

    def read_reply(self, deadline: float) -> tuple[bytes | None, str | None]:
        process = self.process
        if process is None or process.stdout is None:
            return None, "eof_or_crash"
        fd = process.stdout.fileno()
        while True:
            self._drain_stdout()
            newline = self._stdout_buffer.find(b"\n")
            if newline >= 0:
                if newline > MAX_LINE:
                    return self._discard_as_fault(), "oversize_line"
                first = bytes(self._stdout_buffer[:newline])
                del self._stdout_buffer[: newline + 1]
                if b"\n" in self._stdout_buffer:
                    raw = first + b"\n" + self._discard_as_fault()
                    return raw, "extra_protocol_line"
                return first, None
            if len(self._stdout_buffer) > MAX_LINE:
                return self._discard_as_fault(), "oversize_line"
            if self._stdout_eof or process.poll() is not None:
                return self._discard_as_fault(), "eof_or_crash"

            remaining = deadline - self.clock()
            if remaining <= 0:
                raw = self._discard_as_fault()
                return (raw or None), "timeout"
            try:
                ready, _, _ = select.select([fd], [], [], remaining)
            except (OSError, ValueError):
                return self._discard_as_fault(), "eof_or_crash"
            if not ready:
                raw = self._discard_as_fault()
                return (raw or None), "timeout"

    def sweep_extra(self) -> bool:
        self._drain_stdout()
        if b"\n" not in self._stdout_buffer:
            return False
        self._discard_as_fault()
        return True

    def _close_pipes(self) -> None:
        process = self.process
        if process is None:
            return
        for pipe in (process.stdin, process.stdout, process.stderr):
            if pipe is not None:
                try:
                    pipe.close()
                except OSError:
                    pass
        if self._stderr_thread is not None:
            self._stderr_thread.join(timeout=0.2)

    def finish(self, game_end: dict) -> str:
        process = self.process
        if process is None or process.poll() is not None:
            if process is not None:
                try:
                    process.wait(timeout=0)
                except subprocess.TimeoutExpired:
                    pass
                self._kill_group()
            self._close_pipes()
            return "failed"
        delivery = "ok" if self.send_line(game_end) is None else "failed"
        if process.stdin is not None:
            try:
                process.stdin.close()
            except OSError:
                pass
        try:
            process.wait(timeout=self.shutdown_grace_ms / 1000)
        except subprocess.TimeoutExpired:
            self._kill_group()
            process.wait()
        else:
            self._kill_group()
        self._close_pipes()
        return delivery

    def _kill_group(self) -> None:
        pgid = self._pgid
        if pgid is None:
            return
        try:
            os.killpg(pgid, signal.SIGKILL)
        except (ProcessLookupError, PermissionError):
            pass
        self._pgid = None

    def kill(self) -> None:
        process = self.process
        if process is None:
            return
        self._kill_group()
        try:
            process.wait(timeout=1)
        except subprocess.TimeoutExpired:
            process.kill()
            process.wait()
        self._stdout_buffer.clear()
        self._close_pipes()


def collect_both(
    engines: dict[str, Engine],
    reqs: dict[str, dict],
    parse: Callable[[str, bytes | None, str | None], ParsedReply],
    time_ms: int,
    grace_ms: int,
) -> dict[str, tuple[ParsedReply, int]]:
    starts: dict[str, float] = {}
    immediate: dict[str, str] = {}
    for seat in ("X", "O"):
        status = engines[seat].send_line(reqs[seat])
        starts[seat] = engines[seat].clock()
        if status is not None:
            immediate[seat] = status

    results: dict[str, tuple[ParsedReply, int]] = {}
    lock = threading.Lock()

    def read_one(seat: str) -> None:
        engine = engines[seat]
        raw, status = engine.read_reply(
            starts[seat] + (time_ms + grace_ms) / 1000
        )
        elapsed = max(0, int((engine.clock() - starts[seat]) * 1000))
        with lock:
            results[seat] = (parse(seat, raw, status), elapsed)

    threads = []
    for seat in ("X", "O"):
        if seat in immediate:
            raw = engines[seat].take_fault_raw()
            results[seat] = (parse(seat, raw, immediate[seat]), 0)
        else:
            thread = threading.Thread(target=read_one, args=(seat,))
            threads.append(thread)
            thread.start()
    for thread in threads:
        thread.join()

    for seat in ("X", "O"):
        if engines[seat].sweep_extra():
            raw = engines[seat].take_fault_raw()
            _, elapsed = results[seat]
            results[seat] = (parse(seat, raw, "extra_protocol_line"), elapsed)
    return results
