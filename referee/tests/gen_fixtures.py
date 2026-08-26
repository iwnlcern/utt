#!/usr/bin/env python3
"""Regenerate the complete referee fixture matrix hermetically."""

from __future__ import annotations

import argparse
from contextlib import contextmanager
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile
import threading


REPO_ROOT = Path(__file__).resolve().parents[2]
REFEREE_ROOT = REPO_ROOT / "referee"
FIXTURE_ROOT = Path(__file__).resolve().parent / "fixtures"
DEFAULT_MANIFEST = FIXTURE_ROOT / "matrix.json"
ENGINE_STREAM_ROOT = Path("engine-stdin")
FAULTED_STREAM_ROOT = Path("engine-stdin-faulted")
ALLOWLISTED_SIDECARS = {
    "03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.X.stderr",
    "03f5d3f90d8b2d5c6e8308ad73f97366a54d4c5993ec071ab6d6ce2c2e2e6e75.O.stderr",
}

sys.path.insert(0, str(REFEREE_ROOT))

import poorman_referee.procs as procs  # noqa: E402
from poorman_referee.referee import GameConfig, play_game  # noqa: E402


class SelectAdvancedClock:
    """Thread-local fake clock advanced only by exhausted select waits."""

    def __init__(self, initial: float):
        self.initial = initial
        self._local = threading.local()

    def __call__(self) -> float:
        return getattr(self._local, "value", self.initial)

    def advance(self, seconds: float) -> None:
        self._local.value = self() + seconds


@contextmanager
def _deterministic_select(clock: SelectAdvancedClock):
    real_select = procs.select.select

    def select_and_advance(readers, writers, errors, timeout=None):
        result = real_select(readers, writers, errors, timeout)
        if timeout is not None and not result[0]:
            clock.advance(timeout)
        return result

    procs.select.select = select_and_advance
    try:
        yield
    finally:
        procs.select.select = real_select


def _safe_relative(raw: str) -> Path:
    path = Path(raw)
    if path.is_absolute() or ".." in path.parts:
        raise ValueError(f"manifest path escapes the repo root: {raw!r}")
    return path


def load_manifest(path: Path) -> dict:
    matrix = json.loads(path.read_text(encoding="utf-8"))
    if matrix.get("version") != 1 or not isinstance(matrix.get("rows"), list):
        raise ValueError("matrix manifest must have version 1 and a rows list")
    rows = matrix["rows"]
    if len(rows) != 19:
        raise ValueError(f"matrix manifest requires 19 rows, found {len(rows)}")
    outputs = []
    markers = []
    for row in rows:
        output = _safe_relative(row["output"])
        if output.suffix != ".jsonl":
            raise ValueError(f"event-log output is not JSONL: {output}")
        outputs.append(output.as_posix())
        if set(row["cmds"]) != {"X", "O"} or set(row["engine_ids"]) != {"X", "O"}:
            raise ValueError(f"row {output} must define X and O")
        if row["game_id"] != row["game_seed"]:
            raise ValueError(f"row {output} game_id must equal game_seed")
        if len(bytes.fromhex(row["pair_seed"])) != 32:
            raise ValueError(f"row {output} pair_seed must be 32 bytes")
        if len(bytes.fromhex(row["game_seed"])) != 32:
            raise ValueError(f"row {output} game_seed must be 32 bytes")
        if row["pair_coin_seat"] not in {"X", "O"}:
            raise ValueError(f"row {output} has invalid pair_coin_seat")
        if set(row["time_control"]) != {
            "time_ms",
            "grace_ms",
            "hello_timeout_ms",
            "shutdown_grace_ms",
        }:
            raise ValueError(f"row {output} must define all time-control fields")
        if row["clock"] != {"kind": "select_advanced", "initial": 0.0}:
            raise ValueError(f"row {output} has unsupported deterministic clock")
        for marker in row["marker_paths"]:
            markers.append(_safe_relative(marker).as_posix())
    if len(set(outputs)) != 19:
        raise ValueError("matrix output paths must be unique")
    if len(markers) != 6 or len(set(markers)) != 6:
        raise ValueError("matrix must enumerate exactly six distinct marker paths")
    return matrix


def copy_engine_scripts(matrix: dict, temp_root: Path) -> set[Path]:
    """Copy every existing repo-relative file referenced by an engine argv."""

    scripts = set()
    for row in matrix["rows"]:
        for cmd in row["cmds"].values():
            for arg in cmd:
                relative = _safe_relative(arg)
                source = REPO_ROOT / relative
                if source.is_file():
                    scripts.add(relative)
    if not scripts:
        raise ValueError("manifest argv references no engine scripts")
    for relative in scripts:
        target = temp_root / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(REPO_ROOT / relative, target)
    return scripts


def _log_relative_paths(matrix: dict) -> set[Path]:
    return {_safe_relative(row["output"]) for row in matrix["rows"]}


def is_engine_conforming_stream(path: Path) -> bool:
    """Return whether a captured input stream has the normal engine shape."""

    message_types = [
        json.loads(line)["type"]
        for line in path.read_text(encoding="utf-8").splitlines()
    ]
    return (
        len(message_types) >= 2
        and message_types[0] == "hello"
        and message_types[-1] == "game_end"
        and all(message_type == "turn" for message_type in message_types[1:-1])
    )


def _stream_identity(log: Path, seat: str) -> Path:
    return log.parent / f"{log.stem}.{seat}.jsonl"


def _route_generated_streams(row: dict, fixture_root: Path) -> set[Path]:
    log = _safe_relative(row["output"])
    routed = set()
    for seat in ("X", "O"):
        identity = _stream_identity(log, seat)
        staged = fixture_root / ENGINE_STREAM_ROOT / identity
        if is_engine_conforming_stream(staged):
            routed.add(ENGINE_STREAM_ROOT / identity)
            continue
        destination = fixture_root / FAULTED_STREAM_ROOT / identity
        destination.parent.mkdir(parents=True, exist_ok=True)
        staged.replace(destination)
        routed.add(FAULTED_STREAM_ROOT / identity)
    return routed


def expected_generated_paths(matrix: dict, streams: set[Path]) -> set[Path]:
    logs = _log_relative_paths(matrix)
    expected_stream_identities = {
        _stream_identity(log, seat)
        for log in logs
        for seat in ("X", "O")
    }
    observed_stream_identities = {
        relative.relative_to(relative.parts[0]) for relative in streams
    }
    engine_streams = {
        relative for relative in streams if relative.parts[0] == ENGINE_STREAM_ROOT.name
    }
    faulted_streams = {
        relative for relative in streams if relative.parts[0] == FAULTED_STREAM_ROOT.name
    }
    sidecars = {Path(name) for name in ALLOWLISTED_SIDECARS}
    expected = logs | streams | sidecars
    if (
        len(logs) != 19
        or len(engine_streams) != 23
        or len(faulted_streams) != 15
        or len(streams) != 38
        or observed_stream_identities != expected_stream_identities
        or len(expected) != 59
    ):
        raise ValueError("matrix does not map bijectively to the required corpus")
    return expected


def _fixture_inventory(root: Path) -> set[Path]:
    if not root.exists():
        return set()
    return {path.relative_to(root) for path in root.rglob("*") if path.is_file()}


def _make_config(row: dict, temp_root: Path, clock: SelectAdvancedClock) -> GameConfig:
    fixture_root = temp_root / "referee" / "tests" / "fixtures"
    time_control = row["time_control"]
    return GameConfig(
        cmds={seat: list(row["cmds"][seat]) for seat in ("X", "O")},
        engine_ids=dict(row["engine_ids"]),
        tournament_id=row["tournament_id"],
        pair_seed=bytes.fromhex(row["pair_seed"]),
        game_seed=bytes.fromhex(row["game_seed"]),
        pair_coin_seat=row["pair_coin_seat"],
        time_ms=time_control["time_ms"],
        grace_ms=time_control["grace_ms"],
        hello_timeout_ms=time_control["hello_timeout_ms"],
        shutdown_grace_ms=time_control["shutdown_grace_ms"],
        log_path=str(fixture_root / _safe_relative(row["output"])),
        stream_dir=str(fixture_root / ENGINE_STREAM_ROOT),
        clock=clock,
    )


def generate_into(matrix: dict, temp_root: Path) -> tuple[Path, set[Path]]:
    copy_engine_scripts(matrix, temp_root)
    fixture_root = temp_root / "referee" / "tests" / "fixtures"
    fixture_root.mkdir(parents=True, exist_ok=True)
    previous_cwd = Path.cwd()
    streams = set()
    try:
        os.chdir(temp_root)
        for row in matrix["rows"]:
            clock = SelectAdvancedClock(row["clock"]["initial"])
            with _deterministic_select(clock):
                play_game(_make_config(row, temp_root, clock))
            streams.update(_route_generated_streams(row, fixture_root))
    finally:
        os.chdir(previous_cwd)

    marker_paths = {
        _safe_relative(marker)
        for row in matrix["rows"]
        for marker in row["marker_paths"]
    }
    for marker in marker_paths:
        (temp_root / marker).unlink(missing_ok=True)
    for sidecar in fixture_root.rglob("*.stderr"):
        relative = sidecar.relative_to(fixture_root)
        if relative.as_posix() not in ALLOWLISTED_SIDECARS:
            sidecar.unlink()

    expected = expected_generated_paths(matrix, streams)
    actual = _fixture_inventory(fixture_root)
    if actual != expected:
        missing = sorted(path.as_posix() for path in expected - actual)
        unexpected = sorted(path.as_posix() for path in actual - expected)
        raise RuntimeError(
            f"generated inventory mismatch; missing={missing}, unexpected={unexpected}"
        )
    return fixture_root, expected


def _validate_committed_inventory(
    expected_generated: set[Path], *, allow_missing: bool = False
) -> None:
    expected = expected_generated | {Path("matrix.json")}
    actual = _fixture_inventory(FIXTURE_ROOT)
    unexpected = actual - expected
    missing = expected - actual
    if unexpected or (missing and not allow_missing):
        raise RuntimeError(
            "committed inventory mismatch; "
            f"missing={sorted(path.as_posix() for path in missing)}, "
            f"unexpected={sorted(path.as_posix() for path in unexpected)}"
        )


def _compare_generated(generated_root: Path, expected: set[Path]) -> None:
    _validate_committed_inventory(expected)
    mismatches = []
    for relative in sorted(expected):
        if (generated_root / relative).read_bytes() != (FIXTURE_ROOT / relative).read_bytes():
            mismatches.append(relative.as_posix())
    if mismatches:
        raise RuntimeError(f"generated bytes differ: {mismatches}")


def _promote_generated(generated_root: Path, expected: set[Path]) -> None:
    _validate_committed_inventory(expected, allow_missing=True)
    for relative in sorted(expected):
        target = FIXTURE_ROOT / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(generated_root / relative, target)
    _validate_committed_inventory(expected)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    parser.add_argument("--manifest", type=Path, default=DEFAULT_MANIFEST)
    args = parser.parse_args(argv)
    matrix = load_manifest(args.manifest.resolve())
    with tempfile.TemporaryDirectory(prefix="poorman-fixtures-") as raw_temp:
        generated_root, expected = generate_into(matrix, Path(raw_temp))
        if args.check:
            _compare_generated(generated_root, expected)
        else:
            _promote_generated(generated_root, expected)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
