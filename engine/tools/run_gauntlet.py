#!/usr/bin/env python3
"""Run the c4 acceptance gauntlets through the real referee entrypoint."""

from __future__ import annotations

import argparse
import copy
from fractions import Fraction
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile


ENGINE_ID = "c4-engine"
FAST_MATCHUPS = ("random", "zero", "fraction", "allin_tactical")
TOKEN_BUILD = "${BUILD}"
TOKEN_PYTHON = "${PYTHON}"
TOKEN_REFEREE = "${REFEREE}"


class GauntletFailure(RuntimeError):
    """A failed configuration, tournament, score, or log acceptance gate."""


def _is_int(value: object) -> bool:
    return isinstance(value, int) and not isinstance(value, bool)


def _require(condition: bool, message: str) -> None:
    if not condition:
        raise GauntletFailure(message)


def _load_object(path: Path) -> dict:
    try:
        value = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise GauntletFailure(f"cannot read {path}: {error}") from error
    _require(isinstance(value, dict), f"{path}: top level must be an object")
    return value


def validate_config(config: dict, config_path: Path, *, fullclock: bool) -> None:
    """Validate the committed config rather than relying on referee defaults."""
    expected_baseline = "fraction" if fullclock else config_path.stem
    expected_games = 2 if fullclock else 50
    expected_time = 30_000 if fullclock else 2_000

    _require(
        set(config) == {
            "engines",
            "games_per_pair",
            "tournament_seed",
            "time_ms",
            "jobs",
            "out_dir",
        },
        f"{config_path}: unexpected or missing config field",
    )
    engines = config["engines"]
    _require(
        isinstance(engines, list) and len(engines) == 2,
        f"{config_path}: requires exactly two engines",
    )
    by_id = {}
    for engine in engines:
        _require(
            isinstance(engine, dict) and set(engine) == {"id", "cmd"},
            f"{config_path}: malformed engine entry",
        )
        engine_id = engine["id"]
        command = engine["cmd"]
        _require(
            isinstance(engine_id, str)
            and isinstance(command, list)
            and command
            and all(isinstance(part, str) for part in command),
            f"{config_path}: each engine needs a string id and command array",
        )
        _require(engine_id not in by_id, f"{config_path}: duplicate engine id")
        by_id[engine_id] = command

    _require(
        set(by_id) == {ENGINE_ID, expected_baseline},
        f"{config_path}: expected {ENGINE_ID} vs {expected_baseline}",
    )
    _require(
        by_id[ENGINE_ID] == [f"{TOKEN_BUILD}/uttt_engine"],
        f"{config_path}: engine command must use the build template",
    )
    bot_command = by_id[expected_baseline]
    expected_bot = f"{TOKEN_REFEREE}/bots/{expected_baseline}_bot.py"
    _require(
        len(bot_command) >= 4
        and bot_command[0] == TOKEN_PYTHON
        and bot_command[1] == expected_bot
        and bot_command[2] == "--seed"
        and bot_command[3].isdigit(),
        f"{config_path}: baseline command or seed is malformed",
    )
    _require(
        config["games_per_pair"] == expected_games,
        f"{config_path}: games_per_pair must be {expected_games}",
    )
    _require(
        config["time_ms"] == expected_time,
        f"{config_path}: time_ms must be {expected_time}",
    )
    _require(
        _is_int(config["jobs"]) and config["jobs"] > 0,
        f"{config_path}: jobs must be a positive integer",
    )
    _require(
        isinstance(config["tournament_seed"], str)
        and config["tournament_seed"],
        f"{config_path}: tournament_seed must be fixed and non-empty",
    )
    out_dir = Path(config["out_dir"])
    _require(
        out_dir.is_absolute()
        and out_dir.parent == Path("/tmp")
        and out_dir.name.startswith("utt-c4-gauntlet-"),
        f"{config_path}: out_dir must be a dedicated direct child of /tmp",
    )


def _replace_tokens(value: object, replacements: dict[str, str]) -> object:
    if isinstance(value, str):
        for token, replacement in replacements.items():
            value = value.replace(token, replacement)
        return value
    if isinstance(value, list):
        return [_replace_tokens(item, replacements) for item in value]
    if isinstance(value, dict):
        return {
            key: _replace_tokens(item, replacements)
            for key, item in value.items()
        }
    return value


def materialize_config(
    config: dict, *, build_dir: Path, referee_dir: Path
) -> dict:
    replacements = {
        TOKEN_BUILD: str(build_dir),
        TOKEN_PYTHON: sys.executable,
        TOKEN_REFEREE: str(referee_dir),
    }
    resolved = _replace_tokens(copy.deepcopy(config), replacements)
    unresolved = json.dumps(resolved, sort_keys=True)
    for token in replacements:
        _require(token not in unresolved, f"unresolved config token {token}")
    return resolved


def evaluate_summary(
    summary: dict, *, baseline: str, expected_games: int, require_score: bool
) -> Fraction:
    _require(
        summary.get("games_played") == expected_games,
        f"{baseline}: expected {expected_games} games, got "
        f"{summary.get('games_played')!r}",
    )
    engines = summary.get("engines")
    _require(
        isinstance(engines, dict)
        and set(engines) == {ENGINE_ID, baseline},
        f"{baseline}: summary engine set is malformed",
    )
    for engine_id, stats in engines.items():
        _require(isinstance(stats, dict), f"{baseline}: malformed {engine_id} stats")
        for field in ("wins", "draws", "losses", "faults", "voids"):
            _require(
                _is_int(stats.get(field)) and stats[field] >= 0,
                f"{baseline}: malformed {engine_id}.{field}",
            )
        _require(
            stats["faults"] == 0 and stats["voids"] == 0,
            f"{baseline}: {engine_id} recorded faults={stats['faults']} "
            f"voids={stats['voids']}",
        )
        _require(
            stats["wins"] + stats["draws"] + stats["losses"]
            == expected_games,
            f"{baseline}: {engine_id} result counts do not cover every game",
        )

    stats = engines[ENGINE_ID]
    points = Fraction(2 * stats["wins"] + stats["draws"], 2)
    if require_score:
        _require(
            points * 10 >= expected_games * 9,
            f"{baseline}: {points} points is below the 90% bar "
            f"({Fraction(expected_games * 9, 10)})",
        )
    return points


def _read_log(path: Path) -> list[dict]:
    events = []
    try:
        with path.open(encoding="utf-8") as stream:
            for line_number, line in enumerate(stream, 1):
                value = json.loads(line)
                _require(
                    isinstance(value, dict),
                    f"{path}:{line_number}: log record is not an object",
                )
                events.append(value)
    except (OSError, json.JSONDecodeError) as error:
        raise GauntletFailure(f"cannot parse {path}: {error}") from error
    return events


def check_fullclock_logs(
    out_dir: Path, *, expected_games: int = 2, time_ms: int = 30_000
) -> int:
    logs = sorted(out_dir.glob("game-*.jsonl"))
    _require(
        len(logs) == expected_games,
        f"fullclock: expected {expected_games} logs, found {len(logs)}",
    )
    checked = 0
    for log_path in logs:
        events = _read_log(log_path)
        _require(events, f"{log_path}: empty game log")
        start = events[0]
        end = events[-1]
        _require(
            start.get("event") == "game_start"
            and end.get("event") == "game_end",
            f"{log_path}: missing boundary records",
        )
        _require(end.get("result") != "void", f"{log_path}: void game")
        start_engines = start.get("engines")
        _require(isinstance(start_engines, dict), f"{log_path}: missing engines")
        engine_seats = {
            seat
            for seat in ("X", "O")
            if isinstance(start_engines.get(seat), dict)
            and start_engines[seat].get("engine_id") == ENGINE_ID
        }
        _require(
            len(engine_seats) == 1,
            f"{log_path}: expected exactly one {ENGINE_ID} seat",
        )
        engine_seat = engine_seats.pop()
        for event in events:
            if event.get("event") != "auction":
                continue
            attempts = event.get("attempts")
            _require(
                isinstance(attempts, list) and attempts,
                f"{log_path}: auction has no attempts",
            )
            for attempt in attempts:
                turns = attempt.get("turns") if isinstance(attempt, dict) else None
                _require(isinstance(turns, dict), f"{log_path}: malformed attempt")
                for seat in ("X", "O"):
                    turn = turns.get(seat)
                    _require(isinstance(turn, dict), f"{log_path}: missing {seat} turn")
                    _require(
                        turn.get("validation") == "ok",
                        f"{log_path}: {seat} validation fault "
                        f"{turn.get('validation')!r}",
                    )
                turn = turns[engine_seat]
                elapsed = turn.get("elapsed_ms")
                _require(
                    _is_int(elapsed) and 0 <= elapsed <= time_ms,
                    f"{log_path}: engine elapsed_ms {elapsed!r} exceeds {time_ms}",
                )
                info = turn.get("info")
                _require(isinstance(info, dict), f"{log_path}: engine info missing")
                depth = info.get("depth")
                complete = info.get("complete")
                _require(
                    _is_int(depth) and depth >= 1 and complete is True,
                    f"{log_path}: engine published non-completed depth metadata "
                    f"depth={depth!r} complete={complete!r}",
                )
                checked += 1
    _require(checked > 0, "fullclock: no engine turn attempts were checked")
    return checked


def _clean_output_dir(out_dir: Path) -> None:
    resolved = out_dir.resolve()
    temp_root = Path("/tmp").resolve()
    _require(
        resolved.parent == temp_root
        and resolved.name.startswith("utt-c4-gauntlet-"),
        f"refusing to clean unsafe output directory {resolved}",
    )
    if resolved.exists():
        shutil.rmtree(resolved)


def run_matchup(
    config_path: Path, *, build_dir: Path, referee_dir: Path, fullclock: bool
) -> tuple[Fraction, int]:
    config = _load_object(config_path)
    validate_config(config, config_path, fullclock=fullclock)
    resolved = materialize_config(
        config, build_dir=build_dir, referee_dir=referee_dir
    )
    out_dir = Path(resolved["out_dir"])
    _clean_output_dir(out_dir)

    with tempfile.TemporaryDirectory(prefix="utt-c4-gauntlet-config-") as temp:
        runtime_path = Path(temp) / config_path.name
        runtime_path.write_text(
            json.dumps(resolved, indent=2) + "\n", encoding="utf-8"
        )
        command = [
            sys.executable,
            "-m",
            "poorman_referee",
            "tourney",
            "--config",
            str(runtime_path),
        ]
        completed = subprocess.run(
            command,
            cwd=referee_dir,
            text=True,
            capture_output=True,
            check=False,
        )
    _require(
        completed.returncode == 0,
        f"{config_path.stem}: referee exited {completed.returncode}\n"
        f"stdout:\n{completed.stdout}\nstderr:\n{completed.stderr}",
    )

    summary_path = out_dir / "summary.json"
    summary = _load_object(summary_path)
    baseline = "fraction" if fullclock else config_path.stem
    points = evaluate_summary(
        summary,
        baseline=baseline,
        expected_games=config["games_per_pair"],
        require_score=not fullclock,
    )
    checked = (
        check_fullclock_logs(
            out_dir,
            expected_games=config["games_per_pair"],
            time_ms=config["time_ms"],
        )
        if fullclock
        else 0
    )
    print(
        f"PASS {config_path.stem}: points={points}/{config['games_per_pair']} "
        f"faults=0 voids=0"
        + (f" engine_turn_attempts={checked}" if fullclock else ""),
        flush=True,
    )
    return points, checked


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--build",
        required=True,
        type=Path,
        help="engine build directory containing uttt_engine",
    )
    parser.add_argument(
        "--fullclock",
        action="store_true",
        help="run the two-game 30 second A11 gauntlet",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    build_dir = args.build.resolve()
    binary = build_dir / "uttt_engine"
    _require(binary.is_file(), f"engine binary not found: {binary}")
    repo_root = Path(__file__).resolve().parents[2]
    referee_dir = repo_root / "referee"
    _require(
        (referee_dir / "poorman_referee" / "__main__.py").is_file(),
        f"real referee entrypoint not found under {referee_dir}",
    )
    config_dir = Path(__file__).resolve().parent / "gauntlet"
    names = ("fullclock",) if args.fullclock else FAST_MATCHUPS
    for name in names:
        run_matchup(
            config_dir / f"{name}.json",
            build_dir=build_dir,
            referee_dir=referee_dir,
            fullclock=args.fullclock,
        )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except GauntletFailure as error:
        print(f"FAIL: {error}", file=sys.stderr)
        raise SystemExit(1)
