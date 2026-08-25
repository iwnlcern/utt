"""Exact masked comparison of continuous thresholds and discrete values."""

from dataclasses import dataclass
from fractions import Fraction

import numpy as np

from auction_ttt import rules as R
from auction_ttt.discrete import OWIN, XWIN, solve_targeted


Mismatch = tuple[str, str, int, int, int, int]
State = tuple[str, str, int, int]


@dataclass
class BandReport:
    scale: int
    out_of_band_mismatches: list[Mismatch]
    r_mis: Fraction
    r_mis_state: State | None
    max_inband_ratio: Fraction
    inband_fraction: Fraction
    excluded_m0_points: int
    offmask_diagnostics: int


def _validate_scale(N: int) -> None:
    if not isinstance(N, int) or isinstance(N, bool) or N < 0:
        raise ValueError("N must be a nonnegative integer")


def in_band(board: str, T: Fraction, bx: int, bo: int) -> bool:
    """Return whether a budget point is within the a priori E(board) band."""
    if not isinstance(T, Fraction):
        raise TypeError("T must be a Fraction")
    if not isinstance(bx, int) or isinstance(bx, bool) or bx < 0:
        raise ValueError("bx must be a nonnegative integer")
    if not isinstance(bo, int) or isinstance(bo, bool) or bo < 0:
        raise ValueError("bo must be a nonnegative integer")

    p, q = T.numerator, T.denominator
    total = bx + bo
    return abs(bx * q - p * total) <= board.count(".") * q


def _signed_distance_grid(T: Fraction, N: int) -> np.ndarray:
    p, q = T.numerator, T.denominator
    bx = np.arange(N + 1, dtype=np.int64)[:, None]
    bo = np.arange(N + 1, dtype=np.int64)[None, :]
    max_magnitude = N * q + 2 * N * abs(p)
    if max_magnitude <= np.iinfo(np.int64).max:
        return bx * q - p * (bx + bo)
    return bx.astype(object) * q - p * (bx.astype(object) + bo.astype(object))


def _validate_grids(
    values: np.ndarray, mask: np.ndarray, N: int, key: tuple[str, str]
) -> None:
    expected_shape = (N + 1, N + 1)
    if values.shape != expected_shape:
        raise ValueError(f"value grid for {key!r} has shape {values.shape}, expected {expected_shape}")
    if mask.shape != expected_shape:
        raise ValueError(f"mask grid for {key!r} has shape {mask.shape}, expected {expected_shape}")


def _first_maximum(
    selected: np.ndarray, distances: np.ndarray
) -> tuple[int, int, int] | None:
    if not selected.any():
        return None
    maximum = int(np.max(distances[selected]))
    bx, bo = np.argwhere(selected & (distances == maximum))[0]
    return maximum, int(bx), int(bo)


def check(solved_cont, V_disc, masks, N: int) -> BandReport:
    """Compare all masked nonterminal positive-total points at scale ``N``."""
    _validate_scale(N)
    total_grid = np.add.outer(np.arange(N + 1), np.arange(N + 1))
    positive_total = total_grid > 0
    out_of_band_mismatches: list[Mismatch] = []
    r_mis = Fraction(0)
    r_mis_state: State | None = None
    max_inband_ratio = Fraction(0)
    eligible_points = 0
    inband_points = 0
    excluded_m0_points = 0
    offmask_diagnostics = 0

    for key in sorted(V_disc):
        board, h = key
        if R.is_terminal(board):
            continue
        if key not in solved_cont:
            raise KeyError(f"missing continuous state {key!r}")
        if key not in masks:
            raise KeyError(f"missing reachability mask {key!r}")

        values = np.asarray(V_disc[key])
        mask = np.asarray(masks[key], dtype=bool)
        _validate_grids(values, mask, N, key)
        T = solved_cont[key].T
        signed = _signed_distance_grid(T, N)
        distance = np.abs(signed)
        width_units = board.count(".") * T.denominator
        band = distance <= width_units
        disagreement = ((signed > 0) & (values != XWIN)) | (
            (signed < 0) & (values != OWIN)
        )
        eligible = mask & positive_total
        masked_disagreement = eligible & disagreement
        masked_inband = eligible & band
        masked_out_of_band_disagreement = masked_disagreement & ~band

        eligible_points += int(np.count_nonzero(eligible))
        inband_points += int(np.count_nonzero(masked_inband))
        excluded_m0_points += int(np.count_nonzero(mask & ~positive_total))
        offmask_diagnostics += int(
            np.count_nonzero(~mask & positive_total & disagreement)
        )

        candidate = _first_maximum(masked_disagreement, distance)
        if candidate is not None:
            numerator, bx, bo = candidate
            ratio = Fraction(numerator, width_units)
            if ratio > r_mis:
                r_mis = ratio
                r_mis_state = (board, h, bx, bo)

        candidate = _first_maximum(masked_inband, distance)
        if candidate is not None:
            numerator, _bx, _bo = candidate
            ratio = Fraction(numerator, width_units)
            if ratio > max_inband_ratio:
                max_inband_ratio = ratio

        for bx, bo in np.argwhere(masked_out_of_band_disagreement):
            bx_i, bo_i = int(bx), int(bo)
            expected = XWIN if signed[bx_i, bo_i] > 0 else OWIN
            out_of_band_mismatches.append(
                (board, h, bx_i, bo_i, expected, int(values[bx_i, bo_i]))
            )

    return BandReport(
        scale=N,
        out_of_band_mismatches=out_of_band_mismatches,
        r_mis=r_mis,
        r_mis_state=r_mis_state,
        max_inband_ratio=max_inband_ratio,
        inband_fraction=Fraction(inband_points, eligible_points)
        if eligible_points
        else Fraction(0),
        excluded_m0_points=excluded_m0_points,
        offmask_diagnostics=offmask_diagnostics,
    )


def population_counts(solved_cont, masks, N: int) -> dict[str, int]:
    """Return masked positive-total in-band/out-of-band population counts."""
    _validate_scale(N)
    total_grid = np.add.outer(np.arange(N + 1), np.arange(N + 1))
    positive_total = total_grid > 0
    inband_points = 0
    out_of_band_points = 0

    for key in sorted(masks):
        board, _h = key
        if R.is_terminal(board):
            continue
        if key not in solved_cont:
            raise KeyError(f"missing continuous state {key!r}")
        mask = np.asarray(masks[key], dtype=bool)
        expected_shape = (N + 1, N + 1)
        if mask.shape != expected_shape:
            raise ValueError(
                f"mask grid for {key!r} has shape {mask.shape}, expected {expected_shape}"
            )
        T = solved_cont[key].T
        distance = np.abs(_signed_distance_grid(T, N))
        band = distance <= board.count(".") * T.denominator
        eligible = mask & positive_total
        inband_points += int(np.count_nonzero(eligible & band))
        out_of_band_points += int(np.count_nonzero(eligible & ~band))

    return {
        "masked_positive_total": inband_points + out_of_band_points,
        "in_band": inband_points,
        "out_of_band": out_of_band_points,
    }


def select_spot_targets(solved_cont, masks, N: int) -> list[tuple[str, str]]:
    """Select every eligible low-empty zugzwang or in-band-owning state."""
    _validate_scale(N)
    total_grid = np.add.outer(np.arange(N + 1), np.arange(N + 1))
    positive_total = total_grid > 0
    targets: list[tuple[str, str]] = []

    for key in sorted(solved_cont):
        board, _h = key
        if board.count(".") > 5 or key not in masks:
            continue
        mask = np.asarray(masks[key], dtype=bool)
        eligible = mask & positive_total
        if not eligible.any():
            continue
        T = solved_cont[key].T
        distance = np.abs(_signed_distance_grid(T, N))
        owns_inband = bool(
            np.any(eligible & (distance <= board.count(".") * T.denominator))
        )
        if solved_cont[key].zugzwang or owns_inband:
            targets.append(key)
    return targets


def targeted_report(
    solved_cont, targets: list[tuple[str, str]], N: int
) -> tuple[BandReport, dict[str, int], list[dict[str, object]]]:
    """Run exact ``solve_targeted`` checks for selected ``(board, h)`` states."""
    _validate_scale(N)
    if not targets:
        raise RuntimeError("targeted spot set is empty")

    V_disc = {}
    masks = {}
    target_details: list[dict[str, object]] = []
    by_board = {}
    for board, _h in targets:
        if board not in by_board:
            by_board[board] = solve_targeted(board, N)

    for board, h in targets:
        key = (board, h)
        result = by_board[board]
        mask = np.asarray(result.mask[key], dtype=bool)
        if not mask.any():
            raise RuntimeError(f"target {key!r} has an empty mask at scale {N}")
        V_disc[key] = result.values[key]
        masks[key] = result.mask[key]
        counts = population_counts({key: solved_cont[key]}, {key: masks[key]}, N)
        target_details.append({"board": board, "h": h, "population": counts})

    report = check(solved_cont, V_disc, masks, N)
    return report, population_counts(solved_cont, masks, N), target_details


def fraction_text(value: Fraction) -> str:
    return f"{value.numerator}/{value.denominator}"


def report_dict(report: BandReport) -> dict[str, object]:
    return {
        "scale": report.scale,
        "out_of_band_mismatches": report.out_of_band_mismatches,
        "r_mis": fraction_text(report.r_mis),
        "r_mis_state": report.r_mis_state,
        "max_inband_ratio": fraction_text(report.max_inband_ratio),
        "inband_fraction": fraction_text(report.inband_fraction),
        "excluded_m0_points": report.excluded_m0_points,
        "offmask_diagnostics": report.offmask_diagnostics,
    }
