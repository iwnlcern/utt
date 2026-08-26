import itertools
import json
import multiprocessing
from dataclasses import dataclass
from fractions import Fraction
from pathlib import Path

from .gamelog import read_log
from .protocol import canonical_dumps
from .referee import GameConfig, play_game
from .seeds import (
    game_seed,
    pair_coin_seat,
    pair_order,
    pair_seed,
    validate_engine_ids,
    validate_tournament_seed,
)


@dataclass(kw_only=True)
class TournamentConfig:
    engines: list[dict]
    games_per_pair: int
    tournament_seed: str
    time_ms: int
    jobs: int = 1
    out_dir: str


def _fault_counts(events: list[dict]) -> dict[str, int]:
    counts = {"X": 0, "O": 0}
    start = events[0]
    for seat in ("X", "O"):
        if start["hellos"][seat]["validation"] != "ok":
            counts[seat] += 1
    for event in events:
        if event["event"] == "auction":
            for attempt in event["attempts"]:
                for seat in ("X", "O"):
                    if attempt["turns"][seat]["validation"] != "ok":
                        counts[seat] += 1
        elif event["event"] == "recovery":
            if event["hello"]["validation"] != "ok":
                counts[event["seat"]] += 1
    return counts


def _play_task(task: dict) -> dict:
    cfg = GameConfig(
        cmds=task["cmds"],
        engine_ids=task["engine_ids"],
        tournament_id=task["tournament_id"],
        pair_seed=task["pair_seed"],
        game_seed=task["game_seed"],
        pair_coin_seat=task["pair_coin_seat"],
        time_ms=task["time_ms"],
        log_path=task["log_path"],
    )
    result = play_game(cfg)
    return {
        "index": task["index"],
        "engine_ids": task["engine_ids"],
        "result": result.result,
        "reason": result.reason,
        "budgets": result.budgets,
        "faults": _fault_counts(read_log(task["log_path"])),
    }


def _tasks(cfg: TournamentConfig, out_dir: Path) -> list[dict]:
    by_id = {engine["id"]: engine for engine in cfg.engines}
    tasks = []
    index = 0
    for pair_index, (left, right) in enumerate(
        itertools.combinations(cfg.engines, 2), 1
    ):
        engine_a, engine_b = pair_order(left["id"], right["id"])
        for round_ in range(1, cfg.games_per_pair // 2 + 1):
            seed = pair_seed(
                cfg.tournament_seed, engine_a, engine_b, round_
            )
            coin = pair_coin_seat(seed)
            for k, seat_ids in (
                (1, {"X": engine_a, "O": engine_b}),
                (2, {"X": engine_b, "O": engine_a}),
            ):
                index += 1
                tasks.append(
                    {
                        "index": index,
                        "cmds": {
                            seat: list(by_id[engine_id]["cmd"])
                            for seat, engine_id in seat_ids.items()
                        },
                        "engine_ids": seat_ids,
                        "tournament_id": cfg.tournament_seed,
                        "pair_seed": seed,
                        "game_seed": game_seed(seed, k),
                        "pair_coin_seat": coin,
                        "time_ms": cfg.time_ms,
                        "log_path": str(
                            out_dir
                            / f"game-{pair_index:04d}-r{round_:04d}-g{k}.jsonl"
                        ),
                    }
                )
    return tasks


def run_tournament(cfg: TournamentConfig) -> dict:
    if cfg.games_per_pair <= 0 or cfg.games_per_pair % 2:
        raise ValueError("games_per_pair must be positive and even")
    if cfg.jobs <= 0:
        raise ValueError("jobs must be positive")
    if len(cfg.engines) < 2:
        raise ValueError("at least two engines are required")
    ids = [engine.get("id") for engine in cfg.engines]
    if not all(isinstance(engine_id, str) for engine_id in ids):
        raise ValueError("every engine requires a string id")
    validate_engine_ids(ids)
    validate_tournament_seed(cfg.tournament_seed)
    for engine in cfg.engines:
        if not (
            isinstance(engine.get("cmd"), list)
            and engine["cmd"]
            and all(isinstance(part, str) for part in engine["cmd"])
        ):
            raise ValueError("every engine requires a non-empty string command list")

    out_dir = Path(cfg.out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    tasks = _tasks(cfg, out_dir)
    if cfg.jobs == 1:
        results = [_play_task(task) for task in tasks]
    else:
        with multiprocessing.Pool(processes=cfg.jobs) as pool:
            results = pool.map(_play_task, tasks)
    results.sort(key=lambda item: item["index"])

    totals = {
        engine_id: {
            "wins": 0,
            "draws": 0,
            "losses": 0,
            "margin_total": 0,
            "margin_games": 0,
            "faults": 0,
            "voids": 0,
        }
        for engine_id in ids
    }
    for game in results:
        seat_ids = game["engine_ids"]
        for seat in ("X", "O"):
            totals[seat_ids[seat]]["faults"] += game["faults"][seat]
        if game["result"] == "void":
            for engine_id in seat_ids.values():
                totals[engine_id]["voids"] += 1
            continue
        for seat in ("X", "O"):
            engine_id = seat_ids[seat]
            other = "O" if seat == "X" else "X"
            totals[engine_id]["margin_total"] += (
                game["budgets"][seat] - game["budgets"][other]
            )
            totals[engine_id]["margin_games"] += 1
        if game["result"] == "draw":
            for engine_id in seat_ids.values():
                totals[engine_id]["draws"] += 1
        else:
            winner_id = seat_ids[game["result"]]
            loser_id = seat_ids["O" if game["result"] == "X" else "X"]
            totals[winner_id]["wins"] += 1
            totals[loser_id]["losses"] += 1

    engine_summary = {}
    for engine_id in ids:
        stats = totals[engine_id]
        games = stats.pop("margin_games")
        margin_total = stats.pop("margin_total")
        average = Fraction(margin_total, games) if games else Fraction(0, 1)
        stats["avg_budget_margin"] = {
            "numerator": average.numerator,
            "denominator": average.denominator,
        }
        engine_summary[engine_id] = stats
    summary = {
        "tournament_seed": cfg.tournament_seed,
        "games_per_pair": cfg.games_per_pair,
        "games_played": len(results),
        "engines": engine_summary,
    }
    (out_dir / "summary.json").write_text(
        canonical_dumps(summary) + "\n", encoding="utf-8"
    )
    return summary
