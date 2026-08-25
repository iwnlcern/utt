"""Command-line entry points for the continuous auction Tic-Tac-Toe solver."""

import argparse
from collections.abc import Sequence

from auction_ttt.continuous import kalai_report, root_pair, solve


def _print_report(report: dict[str, object]) -> None:
    for name, value in report.items():
        print(f"{name}={value}")


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser()
    commands = parser.add_subparsers(dest="command", required=True)
    commands.add_parser("solve", help="solve and print conditional root thresholds")
    commands.add_parser("kalai", help="report root budget ratios against the E0 reference")
    args = parser.parse_args(argv)

    solved = solve()
    if args.command == "solve":
        T_lo, T_hi = root_pair(solved)
        _print_report({"T_lo": T_lo, "T_hi": T_hi})
    else:
        _print_report(kalai_report(solved))
        print("E0 reference target: approximately 101.84:100 (no agreement claim)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
