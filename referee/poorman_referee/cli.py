import argparse
import json
import shlex
from pathlib import Path

from .protocol import canonical_dumps
from .referee import GameConfig, play_game
from .seeds import game_seed, pair_coin_seat, pair_seed
from .tournament import TournamentConfig, run_tournament


def _command(value: str) -> list[str]:
    try:
        decoded = json.loads(value)
    except json.JSONDecodeError:
        decoded = None
    if isinstance(decoded, list) and decoded and all(
        isinstance(part, str) for part in decoded
    ):
        return decoded
    command = shlex.split(value)
    if not command:
        raise argparse.ArgumentTypeError("command must not be empty")
    return command


def _parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog="poorman_referee")
    subparsers = parser.add_subparsers(dest="action", required=True)
    play = subparsers.add_parser("play")
    play.add_argument("--x-cmd", required=True, type=_command)
    play.add_argument("--o-cmd", required=True, type=_command)
    play.add_argument("--x-id", required=True)
    play.add_argument("--o-id", required=True)
    play.add_argument("--seed", required=True)
    play.add_argument("--log", required=True)
    play.add_argument("--time-ms", type=int, default=30000)
    tourney = subparsers.add_parser("tourney")
    tourney.add_argument("--config", required=True)
    return parser


def main(argv=None) -> int:
    args = _parser().parse_args(argv)
    if args.action == "play":
        seed = pair_seed(args.seed, args.x_id, args.o_id, 1)
        cfg = GameConfig(
            cmds={"X": args.x_cmd, "O": args.o_cmd},
            engine_ids={"X": args.x_id, "O": args.o_id},
            tournament_id=None,
            pair_seed=seed,
            game_seed=game_seed(seed, 1),
            pair_coin_seat=pair_coin_seat(seed),
            time_ms=args.time_ms,
            log_path=args.log,
        )
        result = play_game(cfg)
        print(canonical_dumps(result.__dict__))
        return 0

    with Path(args.config).open(encoding="utf-8") as fh:
        raw = json.load(fh)
    summary = run_tournament(TournamentConfig(**raw))
    print(canonical_dumps(summary))
    return 0
