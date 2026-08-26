import json
import sys
from pathlib import Path

import pytest

from poorman_referee.cli import main
from poorman_referee.gamelog import read_log
from poorman_referee.tournament import TournamentConfig, run_tournament


STUB = Path(__file__).with_name("stub_engine.py")
BOT_ROOT = Path(__file__).parents[1] / "bots"


def stub(*args):
    return [sys.executable, str(STUB), *args]


def bot(name, seed):
    return [sys.executable, str(BOT_ROOT / f"{name}_bot.py"), "--seed", str(seed)]


def tournament_config(tmp_path, *, jobs=1, engines=None):
    return TournamentConfig(
        engines=engines
        or [
            {"id": "A", "cmd": stub("--bid", "0", "--seed", "1")},
            {"id": "B", "cmd": stub("--bid", "0", "--seed", "2")},
        ],
        games_per_pair=2,
        tournament_seed="tourney-seed",
        time_ms=500,
        jobs=jobs,
        out_dir=str(tmp_path),
    )


def test_two_game_pair_swaps_seats_and_keeps_pair_coin(tmp_path):
    summary = run_tournament(tournament_config(tmp_path))
    logs = sorted(tmp_path.glob("game-*.jsonl"))

    assert len(logs) == 2
    starts = [read_log(path)[0] for path in logs]
    assert starts[0]["engines"]["X"]["engine_id"] == "A"
    assert starts[0]["engines"]["O"]["engine_id"] == "B"
    assert starts[1]["engines"]["X"]["engine_id"] == "B"
    assert starts[1]["engines"]["O"]["engine_id"] == "A"
    assert starts[0]["pair_seed"] == starts[1]["pair_seed"]
    assert starts[0]["pair_coin_seat"] == starts[1]["pair_coin_seat"]
    assert summary["games_played"] == 2


def test_void_games_are_reliability_only_not_win_draw_loss(tmp_path):
    engines = [
        {"id": "A", "cmd": stub("--fault", "bad_json:1")},
        {"id": "B", "cmd": stub("--fault", "bad_json:1")},
    ]

    summary = run_tournament(tournament_config(tmp_path, engines=engines))

    for engine_id in ("A", "B"):
        stats = summary["engines"][engine_id]
        assert (stats["wins"], stats["draws"], stats["losses"]) == (0, 0, 0)
        assert stats["voids"] == 2
        assert stats["faults"] >= 6


def test_parallel_and_sequential_aggregation_are_identical(tmp_path):
    engines = [
        {"id": "zero", "cmd": bot("zero", 1)},
        {"id": "random", "cmd": bot("random", 2)},
        {"id": "fraction", "cmd": [*bot("fraction", 3), "--fraction-ppb", "100000000"]},
    ]

    sequential = run_tournament(
        tournament_config(tmp_path / "one", jobs=1, engines=engines)
    )
    parallel = run_tournament(
        tournament_config(tmp_path / "two", jobs=2, engines=engines)
    )

    assert parallel == sequential


def test_summary_json_is_written_and_sane(tmp_path):
    summary = run_tournament(tournament_config(tmp_path))

    written = json.loads((tmp_path / "summary.json").read_text(encoding="utf-8"))
    assert written == summary
    assert set(summary) == {"tournament_seed", "games_per_pair", "games_played", "engines"}
    assert set(summary["engines"]) == {"A", "B"}
    for stats in summary["engines"].values():
        assert set(stats) == {
            "wins",
            "draws",
            "losses",
            "avg_budget_margin",
            "faults",
            "voids",
        }
        assert set(stats["avg_budget_margin"]) == {"numerator", "denominator"}
        assert type(stats["avg_budget_margin"]["numerator"]) is int
        assert type(stats["avg_budget_margin"]["denominator"]) is int
        assert stats["avg_budget_margin"]["denominator"] > 0

    def assert_no_float(value):
        assert not isinstance(value, float)
        if isinstance(value, dict):
            for child in value.values():
                assert_no_float(child)
        elif isinstance(value, list):
            for child in value:
                assert_no_float(child)

    assert_no_float(summary)


def test_cli_play_and_tourney_commands(tmp_path):
    play_log = tmp_path / "cli-game.jsonl"
    assert (
        main(
            [
                "play",
                "--x-cmd",
                json.dumps(bot("zero", 1)),
                "--o-cmd",
                json.dumps(bot("zero", 2)),
                "--x-id",
                "A",
                "--o-id",
                "B",
                "--seed",
                "cli-seed",
                "--log",
                str(play_log),
                "--time-ms",
                "500",
            ]
        )
        == 0
    )
    events = read_log(play_log)
    assert events[-1]["event"] == "game_end"
    from poorman_referee.seeds import pair_order, pair_seed

    ordered = pair_order("A", "B")
    assert events[0]["pair_seed"] == pair_seed("cli-seed", *ordered, 1).hex()

    cfg_path = tmp_path / "tourney.json"
    cfg_path.write_text(
        json.dumps(
            {
                "engines": [
                    {"id": "A", "cmd": bot("zero", 1)},
                    {"id": "B", "cmd": bot("zero", 2)},
                ],
                "games_per_pair": 2,
                "tournament_seed": "cli-tourney",
                "time_ms": 500,
                "jobs": 1,
                "out_dir": str(tmp_path / "cli-tourney"),
            }
        ),
        encoding="utf-8",
    )
    assert main(["tourney", "--config", str(cfg_path)]) == 0
    assert (tmp_path / "cli-tourney" / "summary.json").is_file()


@pytest.mark.parametrize("games", [0, 1, 3])
def test_games_per_pair_must_be_positive_and_even(tmp_path, games):
    cfg = tournament_config(tmp_path)
    cfg.games_per_pair = games

    with pytest.raises(ValueError, match="positive and even"):
        run_tournament(cfg)


def test_tournament_rejects_separator_bearing_seed(tmp_path):
    cfg = tournament_config(tmp_path)
    cfg.tournament_seed = "ambiguous\x1fseed"

    with pytest.raises(ValueError, match="0x1f"):
        run_tournament(cfg)
