import time
from dataclasses import dataclass, field
from pathlib import Path

from .auction import resolve
from .gamelog import GameLogWriter, hello_record, turn_record
from .procs import Engine, collect_both
from .protocol import (
    MAX_RAW,
    ParsedReply,
    game_end_msg,
    hello_request,
    parse_turn_reply,
    to_submission,
    turn_request,
)
from .rules import INITIAL, Position, apply_move, local_status


STARTING_BUDGET = 10**9


@dataclass(kw_only=True)
class GameConfig:
    cmds: dict[str, list[str]]
    engine_ids: dict[str, str]
    tournament_id: str | None
    pair_seed: bytes
    game_seed: bytes
    pair_coin_seat: str
    time_ms: int = 30000
    grace_ms: int = 250
    hello_timeout_ms: int = 10000
    shutdown_grace_ms: int = 2000
    log_path: str
    clock: object = field(default=time.monotonic, repr=False)


@dataclass(frozen=True)
class GameResult:
    result: str
    reason: str
    budgets: dict[str, int]
    plies: int


def _fault_reply(validation: str, raw: bytes | None) -> ParsedReply:
    data = raw or b""
    return ParsedReply(
        validation=validation,
        bid=None,
        move=None,
        info=None,
        raw=data[:MAX_RAW],
        raw_total=len(data),
        raw_truncated=len(data) > MAX_RAW,
    )


def _other(seat: str) -> str:
    return "O" if seat == "X" else "X"


def _new_engine(cfg: GameConfig, seat: str) -> Engine:
    return Engine(
        cfg.cmds[seat],
        seat,
        shutdown_grace_ms=cfg.shutdown_grace_ms,
        clock=cfg.clock,
    )


def _game_start_event(
    cfg: GameConfig, game_id: str, hellos: dict[str, dict], budgets: dict[str, int]
) -> dict:
    return {
        "event": "game_start",
        "log_version": 1,
        "protocol": 1,
        "rules": "poorman-uttt-v1",
        "game_id": game_id,
        "tournament_id": cfg.tournament_id,
        "engines": {
            seat: {"engine_id": cfg.engine_ids[seat], "cmd": cfg.cmds[seat]}
            for seat in ("X", "O")
        },
        "pair_seed": cfg.pair_seed.hex(),
        "game_seed": cfg.game_seed.hex(),
        "pair_coin_seat": cfg.pair_coin_seat,
        "hellos": hellos,
        "time_control": {
            "time_ms": cfg.time_ms,
            "grace_ms": cfg.grace_ms,
            "hello_timeout_ms": cfg.hello_timeout_ms,
            "shutdown_grace_ms": cfg.shutdown_grace_ms,
        },
        "budgets": dict(budgets),
    }


def _terminal_from_position(pos: Position, mover: str, terminal: str) -> tuple[str, str]:
    if terminal == "macro_win":
        return mover, "macro_win"
    chips = {
        seat: sum(local_status(cells) == seat for cells in pos.board)
        for seat in ("X", "O")
    }
    if chips["X"] > chips["O"]:
        return "X", "chip_count"
    if chips["O"] > chips["X"]:
        return "O", "chip_count"
    return "draw", "exact_tie_draw"


def play_game(cfg: GameConfig) -> GameResult:
    if set(cfg.cmds) != {"X", "O"} or set(cfg.engine_ids) != {"X", "O"}:
        raise ValueError("cmds and engine_ids must be keyed by X and O")
    if cfg.pair_coin_seat not in ("X", "O"):
        raise ValueError("pair_coin_seat must be X or O")
    game_id = cfg.game_seed.hex()
    budgets = {"X": STARTING_BUDGET, "O": STARTING_BUDGET}
    engines = {seat: _new_engine(cfg, seat) for seat in ("X", "O")}
    path = Path(cfg.log_path)
    path.parent.mkdir(parents=True, exist_ok=True)

    with path.open("w", encoding="utf-8") as fh:
        log = GameLogWriter(fh)
        try:
            for engine in engines.values():
                engine.start()
            parsed_hellos = {}
            hello_records = {}
            for seat in ("X", "O"):
                parsed, elapsed = engines[seat].hello(
                    hello_request(
                        game_id=game_id,
                        you=seat,
                        time_ms=cfg.time_ms,
                        grace_ms=cfg.grace_ms,
                        budget=budgets[seat],
                    ),
                    cfg.hello_timeout_ms,
                )
                parsed_hellos[seat] = parsed
                hello_records[seat] = hello_record(parsed, elapsed)
            log.emit(_game_start_event(cfg, game_id, hello_records, budgets))

            hello_faults = [
                seat for seat, parsed in parsed_hellos.items() if parsed.validation != "ok"
            ]
            if hello_faults:
                result = "void" if len(hello_faults) == 2 else _other(hello_faults[0])
                return _finish_game(
                    log, engines, result, "hello_fault", budgets, plies=0
                )

            pos = INITIAL
            tie_owner = None
            ply = 0
            while True:
                attempts = []
                for attempt in range(1, 4):
                    request_id = f"{game_id}-p{ply}-a{attempt}"
                    reqs = {
                        seat: turn_request(
                            game_id=game_id,
                            request_id=request_id,
                            ply=ply,
                            attempt=attempt,
                            you=seat,
                            pos=pos,
                            budgets=budgets,
                            tie_owner=tie_owner,
                            time_ms=cfg.time_ms,
                        )
                        for seat in ("X", "O")
                    }

                    def parse(seat, raw, framing_fault):
                        if framing_fault is not None:
                            return _fault_reply(framing_fault, raw)
                        return parse_turn_reply(
                            raw or b"",
                            request_id=request_id,
                            budget=budgets[seat],
                            legal=[tuple(move) for move in reqs[seat]["legal"]],
                        )

                    collected = collect_both(
                        engines, reqs, parse, cfg.time_ms, cfg.grace_ms
                    )
                    parsed = {seat: collected[seat][0] for seat in ("X", "O")}
                    attempts.append(
                        {
                            "request_id": request_id,
                            "attempt": attempt,
                            "turns": {
                                seat: turn_record(*collected[seat])
                                for seat in ("X", "O")
                            },
                        }
                    )
                    resolution = resolve(
                        to_submission(parsed["X"]),
                        to_submission(parsed["O"]),
                        tie_owner,
                        cfg.pair_coin_seat,
                    )
                    if resolution is not None:
                        winner = resolution.winner
                        move = parsed[winner].move
                        if move is None:
                            raise RuntimeError("resolved winner has no legal move")
                        budgets[winner] -= resolution.payment
                        applied = apply_move(pos, move, winner)
                        pos = applied.position
                        tie_owner = _other(winner)
                        log.emit(
                            _auction_event(
                                ply,
                                "resolved",
                                attempts,
                                budgets,
                                pos,
                                resolution={
                                    "winner": winner,
                                    "reason": resolution.reason,
                                    "coin": resolution.coin,
                                    "payment": resolution.payment,
                                    "move": list(move),
                                    "closures": [
                                        {"local": closure.local, "result": closure.result}
                                        for closure in applied.closures
                                    ],
                                    "macro_line": (
                                        list(applied.macro_line)
                                        if applied.macro_line is not None
                                        else None
                                    ),
                                    "forced_next": pos.forced,
                                },
                            )
                        )
                        if applied.terminal is not None:
                            result, reason = _terminal_from_position(
                                pos, winner, applied.terminal
                            )
                            return _finish_game(
                                log, engines, result, reason, budgets, plies=ply + 1
                            )

                        faulters = [
                            seat
                            for seat in ("X", "O")
                            if parsed[seat].validation != "ok"
                        ]
                        if faulters:
                            failed = _recover_seats(
                                cfg,
                                engines,
                                faulters,
                                log,
                                game_id,
                                budgets,
                                ply,
                                request_id,
                                parsed,
                            )
                            if failed:
                                result = (
                                    "void" if len(failed) == 2 else _other(failed[0])
                                )
                                return _finish_game(
                                    log,
                                    engines,
                                    result,
                                    "recovery_fault",
                                    budgets,
                                    plies=ply + 1,
                                )
                        ply += 1
                        break

                    if attempt == 3:
                        log.emit(
                            _auction_event(
                                ply, "voided", attempts, budgets, pos, resolution=None
                            )
                        )
                        return _finish_game(
                            log,
                            engines,
                            "void",
                            "triple_double_fault_void",
                            budgets,
                            plies=ply + 1,
                        )

                    failed = _recover_seats(
                        cfg,
                        engines,
                        ["X", "O"],
                        log,
                        game_id,
                        budgets,
                        ply,
                        request_id,
                        parsed,
                    )
                    if failed:
                        log.emit(
                            _auction_event(
                                ply,
                                "aborted_recovery_fault",
                                attempts,
                                budgets,
                                pos,
                                resolution=None,
                            )
                        )
                        result = "void" if len(failed) == 2 else _other(failed[0])
                        return _finish_game(
                            log,
                            engines,
                            result,
                            "recovery_fault",
                            budgets,
                            plies=ply + 1,
                        )
        finally:
            for engine in engines.values():
                engine.kill()


def _auction_event(
    ply: int,
    outcome: str,
    attempts: list[dict],
    budgets: dict[str, int],
    pos: Position,
    *,
    resolution: dict | None,
) -> dict:
    event = {
        "event": "auction",
        "ply": ply,
        "outcome": outcome,
        "attempts": attempts,
        "budgets_after": dict(budgets),
        "post_board": list(pos.board),
    }
    if resolution is not None:
        event["resolution"] = resolution
    return event


def _recover_seats(
    cfg: GameConfig,
    engines: dict[str, Engine],
    seats: list[str],
    log: GameLogWriter,
    game_id: str,
    budgets: dict[str, int],
    ply: int,
    request_id: str,
    parsed: dict[str, ParsedReply],
) -> list[str]:
    failed = []
    for seat in ("X", "O"):
        if seat not in seats:
            continue
        engines[seat].kill()
        engines[seat] = _new_engine(cfg, seat)
        engines[seat].start()
        hello, elapsed = engines[seat].hello(
            hello_request(
                game_id=game_id,
                you=seat,
                time_ms=cfg.time_ms,
                grace_ms=cfg.grace_ms,
                budget=budgets[seat],
            ),
            cfg.hello_timeout_ms,
        )
        log.emit(
            {
                "event": "recovery",
                "seat": seat,
                "ply": ply,
                "trigger_request_id": request_id,
                "fault": parsed[seat].validation,
                "hello": hello_record(hello, elapsed),
            }
        )
        if hello.validation != "ok":
            failed.append(seat)
    return failed


def _finish_game(
    log: GameLogWriter,
    engines: dict[str, Engine],
    result: str,
    reason: str,
    budgets: dict[str, int],
    *,
    plies: int,
) -> GameResult:
    message = game_end_msg(result=result, reason=reason, budgets=budgets)
    delivery = {seat: engines[seat].finish(message) for seat in ("X", "O")}
    log.emit(
        {
            "event": "game_end",
            "result": result,
            "reason": reason,
            "budgets": dict(budgets),
            "budget_margin": budgets["X"] - budgets["O"],
            "plies": plies,
            "delivery": delivery,
        }
    )
    return GameResult(result=result, reason=reason, budgets=dict(budgets), plies=plies)
