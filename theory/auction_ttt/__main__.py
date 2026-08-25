"""Command-line entry points for the continuous auction Tic-Tac-Toe solver."""

import argparse
import json
from collections.abc import Sequence
from fractions import Fraction
from pathlib import Path

from auction_ttt.continuous import kalai_report, root_pair, solve as solve_continuous
from auction_ttt.crosscheck import (
    check,
    population_counts,
    report_dict,
    select_spot_targets,
    targeted_report,
)
from auction_ttt.discrete import solve as solve_discrete


def _print_report(report: dict[str, object]) -> None:
    for name, value in report.items():
        print(f"{name}={value}")


def _positive_int(text: str) -> int:
    value = int(text)
    if value <= 0:
        raise argparse.ArgumentTypeError("must be a positive integer")
    return value


def _exhaustive_scale(text: str) -> int:
    value = _positive_int(text)
    if value > 32:
        raise argparse.ArgumentTypeError("exhaustive scale must be at most 32")
    return value


def _default_results_dir() -> Path:
    return Path(__file__).resolve().parents[1] / "results"


def _write_sweep_reports(payloads: list[dict[str, object]], output_dir: Path) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    for payload in payloads:
        scale = payload["scale"]
        path = output_dir / f"crosscheck-N{scale}.json"
        path.write_text(json.dumps(payload, indent=2, sort_keys=True) + "\n")


def _fraction_from_text(text: str) -> Fraction:
    return Fraction(text)


def _add_rise_anomalies(payloads: list[dict[str, object]]) -> None:
    by_scale = {int(payload["scale"]): payload for payload in payloads}
    for lower, upper in ((8, 16), (16, 32), (64, 128)):
        if lower not in by_scale or upper not in by_scale:
            continue
        earlier, later = by_scale[lower], by_scale[upper]
        findings: list[str] = []
        if _fraction_from_text(str(later["r_mis"])) > _fraction_from_text(
            str(earlier["r_mis"])
        ):
            findings.append(f"r_mis rose from N={lower} to N={upper}")
        if _fraction_from_text(str(later["inband_fraction"])) > _fraction_from_text(
            str(earlier["inband_fraction"])
        ):
            findings.append(f"in-band occupancy rose from N={lower} to N={upper}")
        later["anomaly_findings"] = findings


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser()
    commands = parser.add_subparsers(dest="command", required=True)
    commands.add_parser("solve", help="solve and print conditional root thresholds")
    commands.add_parser("kalai", help="report root budget ratios against the E0 reference")
    crosscheck_parser = commands.add_parser(
        "crosscheck", help="cross-check one exact discrete scale"
    )
    crosscheck_parser.add_argument("--scale", type=_positive_int, required=True)
    sweep_parser = commands.add_parser(
        "sweep", help="write exhaustive or targeted cross-check JSON reports"
    )
    sweep_mode = sweep_parser.add_mutually_exclusive_group(required=True)
    sweep_mode.add_argument("--max-scale", type=_exhaustive_scale)
    sweep_mode.add_argument("--spots", nargs="+", type=_positive_int)
    sweep_parser.add_argument(
        "--output-dir", type=Path, default=_default_results_dir()
    )
    args = parser.parse_args(argv)
    if args.command == "sweep" and args.spots is not None:
        if sorted(args.spots) != [64, 128]:
            parser.error("--spots requires exactly 64 and 128")
        args.spots = [64, 128]

    solved = solve_continuous()
    if args.command == "solve":
        T_lo, T_hi = root_pair(solved)
        _print_report({"T_lo": T_lo, "T_hi": T_hi})
    elif args.command == "kalai":
        _print_report(kalai_report(solved))
        print("E0 reference target: approximately 101.84:100 (no agreement claim)")
    elif args.command == "crosscheck":
        values, masks = solve_discrete(args.scale)
        report = check(solved, values, masks, args.scale)
        payload = report_dict(report)
        payload["mode"] = "exhaustive"
        payload["population"] = population_counts(solved, masks, args.scale)
        print(json.dumps(payload, indent=2, sort_keys=True))
        return 1 if report.out_of_band_mismatches else 0
    elif args.max_scale is not None:
        payloads = []
        reports = []
        for scale in range(1, args.max_scale + 1):
            values, masks = solve_discrete(scale)
            report = check(solved, values, masks, scale)
            payload = report_dict(report)
            payload["mode"] = "exhaustive"
            payload["population"] = population_counts(solved, masks, scale)
            payloads.append(payload)
            reports.append(report)
        _add_rise_anomalies(payloads)
        _write_sweep_reports(payloads, args.output_dir)
        return 1 if any(report.out_of_band_mismatches for report in reports) else 0
    else:
        _values_32, masks_32 = solve_discrete(32)
        targets = select_spot_targets(solved, masks_32, 32)
        if not targets:
            parser.error("N=32 produced an empty targeted spot set")
        payloads = []
        reports = []
        for scale in args.spots:
            report, population, target_details = targeted_report(
                solved, targets, scale
            )
            payload = report_dict(report)
            payload["mode"] = "targeted"
            payload["population"] = population
            payload["targets"] = target_details
            payloads.append(payload)
            reports.append(report)
        _add_rise_anomalies(payloads)
        _write_sweep_reports(payloads, args.output_dir)
        return 1 if any(report.out_of_band_mismatches for report in reports) else 0
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
