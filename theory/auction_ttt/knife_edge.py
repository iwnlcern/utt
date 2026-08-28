"""Exact p = T comparison between continuous thresholds and discrete oracles."""

from collections import Counter
from collections.abc import Callable
from dataclasses import dataclass
from fractions import Fraction
from hashlib import sha256
from itertools import groupby
import json

import numpy as np

from auction_ttt import rules as R
from auction_ttt.continuous import Node, solve as solve_continuous
from auction_ttt.crosscheck import select_spot_targets
from auction_ttt.discrete import DRAW, OWIN, XWIN, solve as solve_discrete
from auction_ttt.discrete import solve_targeted


_OUTCOME = {OWIN: R.O, DRAW: "draw", XWIN: R.X}
GroupKey = tuple[str, str, str, bool]


@dataclass(frozen=True)
class KnifePoint:
    board: str
    h: str
    bx: int
    bo: int
    T: Fraction
    discrete: str
    convention: str
    agrees: bool
    state_class: str


def _validate_scale(N: int) -> None:
    if not isinstance(N, int) or isinstance(N, bool) or N <= 0:
        raise ValueError("N must be a positive integer")


def _state_class(board: str, node: Node) -> str:
    if node.a > node.b:
        raise RuntimeError(
            f"oracle-value surprise at {(board, node.a, node.b)!r}: a > b"
        )
    relation = "a=b" if node.a == node.b else "a<b"
    return f"{relation}/E{board.count(R.EMPTY)}"


def _equality_mask(T: Fraction, mask: np.ndarray, N: int) -> np.ndarray:
    expected_shape = (N + 1, N + 1)
    if mask.shape != expected_shape:
        raise ValueError(
            f"mask grid has shape {mask.shape}, expected {expected_shape}"
        )
    p, q = T.numerator, T.denominator
    bx = np.arange(N + 1, dtype=np.int64)[:, None]
    bo = np.arange(N + 1, dtype=np.int64)[None, :]
    max_magnitude = N * q + 2 * N * abs(p)
    if max_magnitude <= np.iinfo(np.int64).max:
        signed = bx * q - p * (bx + bo)
    else:
        bx_object = bx.astype(object)
        bo_object = bo.astype(object)
        signed = bx_object * q - p * (bx_object + bo_object)
    return mask & ((bx + bo) > 0) & (signed == 0)


def _fold_key(
    board: str,
    h: str,
    node: Node,
    values: np.ndarray,
    mask: np.ndarray,
    N: int,
    fold: Callable[[KnifePoint], None],
) -> int:
    expected_shape = (N + 1, N + 1)
    values = np.asarray(values)
    mask = np.asarray(mask, dtype=bool)
    if values.shape != expected_shape:
        raise ValueError(
            f"value grid for {(board, h)!r} has shape {values.shape}, "
            f"expected {expected_shape}"
        )
    selected = _equality_mask(node.T, mask, N)
    expected = int(np.count_nonzero(selected))
    state_class = _state_class(board, node)
    emitted = 0
    for bx, bo in np.argwhere(selected):
        bx_i, bo_i = int(bx), int(bo)
        raw_outcome = int(values[bx_i, bo_i])
        if raw_outcome not in _OUTCOME:
            raise RuntimeError(
                f"unexpected discrete outcome {raw_outcome} at "
                f"{(board, h, bx_i, bo_i)!r}"
            )
        discrete = _OUTCOME[raw_outcome]
        point = KnifePoint(
            board=board,
            h=h,
            bx=bx_i,
            bo=bo_i,
            T=node.T,
            discrete=discrete,
            convention=h,
            agrees=discrete == h,
            state_class=state_class,
        )
        fold(point)
        emitted += 1
    if emitted != expected:
        raise RuntimeError(
            f"knife-point conservation failed for {(board, h)!r} at scale {N}: "
            f"expected {expected}, emitted {emitted}"
        )
    return emitted


def _fold_exhaustive(
    solved: dict[tuple[str, str], Node],
    values: dict[tuple[str, str], np.ndarray],
    masks: dict[tuple[str, str], np.ndarray],
    N: int,
    fold: Callable[[KnifePoint], None],
) -> None:
    for key in sorted(values):
        board, h = key
        if R.is_terminal(board):
            continue
        if key not in solved:
            raise KeyError(f"missing continuous state {key!r}")
        if key not in masks:
            raise RuntimeError(f"missing mask for {key!r} at scale {N}")
        _fold_key(board, h, solved[key], values[key], masks[key], N, fold)


def sweep(N: int) -> list[KnifePoint]:
    """Return every masked nonterminal positive-total point at exact p = T."""
    _validate_scale(N)
    solved = solve_continuous()
    values, masks = solve_discrete(N)
    points: list[KnifePoint] = []
    _fold_exhaustive(solved, values, masks, N, points.append)
    return points


def _spot_sweep_with_solved(
    solved: dict[tuple[str, str], Node],
    N: int,
    targets: list[tuple[str, str]],
    fold: Callable[[KnifePoint], None],
) -> None:
    _validate_scale(N)
    if not targets:
        raise RuntimeError("targeted spot set is empty")
    ordered_targets = sorted(targets)
    if len(ordered_targets) != len(set(ordered_targets)):
        raise RuntimeError("targeted spot set contains duplicate keys")

    for board, grouped_targets in groupby(ordered_targets, key=lambda key: key[0]):
        board_targets = list(grouped_targets)
        result = solve_targeted(board, N)
        for key in board_targets:
            _board, h = key
            if key not in solved:
                raise KeyError(f"missing continuous state {key!r}")
            if key not in result.mask:
                raise RuntimeError(f"target {key!r} has a missing mask at scale {N}")
            if key not in result.values:
                raise RuntimeError(f"target {key!r} has missing values at scale {N}")
            mask = np.asarray(result.mask[key], dtype=bool)
            if not mask.any():
                raise RuntimeError(f"target {key!r} has an empty mask at scale {N}")
            _fold_key(
                board,
                h,
                solved[key],
                result.values[key],
                mask,
                N,
                fold,
            )
            del mask
        del result, board_targets


def spot_sweep(
    N: int,
    targets: list[tuple[str, str]],
    fold: Callable[[KnifePoint], None],
) -> None:
    """Fold selected equality points one solved board at a time."""
    _spot_sweep_with_solved(solve_continuous(), N, targets, fold)


class _ScaleFold:
    def __init__(self, scale: int, mode: str, target_count: int | None):
        self.scale = scale
        self.mode = mode
        self.target_count = target_count
        self.equality_points = 0
        self.agreements = 0
        self.disagreements = 0
        self.groups: Counter[GroupKey] = Counter()
        self.digest = sha256()

    def __call__(self, point: KnifePoint) -> None:
        self.equality_points += 1
        if point.agrees:
            self.agreements += 1
        else:
            self.disagreements += 1
            encoded = json.dumps(
                [point.board, point.h, point.bx, point.bo],
                ensure_ascii=True,
                separators=(",", ":"),
            ).encode("ascii")
            self.digest.update(encoded + b"\n")
        self.groups[
            (point.h, point.state_class, point.discrete, point.agrees)
        ] += 1

    def finish(self) -> tuple[dict[str, object], list[dict[str, object]], dict[str, object]]:
        if self.equality_points != self.agreements + self.disagreements:
            raise RuntimeError(f"per-scale conservation failed at scale {self.scale}")
        groups = [
            {
                "scale": self.scale,
                "mode": self.mode,
                "h": h,
                "state_class": state_class,
                "discrete": discrete,
                "agrees": agrees,
                "count": count,
            }
            for (h, state_class, discrete, agrees), count in sorted(
                self.groups.items()
            )
            if count > 0
        ]
        if sum(group["count"] for group in groups) != self.equality_points:
            raise RuntimeError(f"group conservation failed at scale {self.scale}")
        disagreement_count = sum(
            group["count"] for group in groups if not group["agrees"]
        )
        if disagreement_count != self.disagreements:
            raise RuntimeError(
                f"disagreement-group conservation failed at scale {self.scale}"
            )
        record = {
            "scale": self.scale,
            "mode": self.mode,
            "target_count": self.target_count,
            "equality_points": self.equality_points,
            "agreements": self.agreements,
            "disagreements": self.disagreements,
        }
        digest = {
            "scale": self.scale,
            "mode": self.mode,
            "sha256": self.digest.hexdigest(),
        }
        return record, groups, digest


def _ordered_scales(scales: list[int], label: str) -> list[int]:
    for scale in scales:
        _validate_scale(scale)
    ordered = sorted(scales)
    if len(ordered) != len(set(ordered)):
        raise ValueError(f"{label} contains duplicate scales")
    return ordered


def report(scales: list[int], spot_scales: list[int]) -> dict[str, object]:
    """Build the deterministic aggregate knife-edge report without point retention."""
    exhaustive = _ordered_scales(scales, "scales")
    spots = _ordered_scales(spot_scales, "spot_scales")
    if set(exhaustive) & set(spots):
        raise ValueError("exhaustive and targeted scales must be disjoint")

    solved = solve_continuous()
    targets: list[tuple[str, str]] = []
    per_scale: list[dict[str, object]] = []
    groups: list[dict[str, object]] = []
    stream_digests: list[dict[str, object]] = []

    for scale in exhaustive:
        values, masks = solve_discrete(scale)
        accumulator = _ScaleFold(scale, "exhaustive", None)
        _fold_exhaustive(solved, values, masks, scale, accumulator)
        if spots and scale == 32:
            targets = select_spot_targets(solved, masks, 32)
        record, scale_groups, digest = accumulator.finish()
        per_scale.append(record)
        groups.extend(scale_groups)
        stream_digests.append(digest)
        del accumulator, values, masks

    if spots and not targets:
        values_32, masks_32 = solve_discrete(32)
        targets = select_spot_targets(solved, masks_32, 32)
        del values_32, masks_32
    if spots and not targets:
        raise RuntimeError("N=32 produced an empty targeted spot set")
    if spots == [64, 128] and len(targets) != 20332:
        raise RuntimeError(
            f"canonical spot target count is {len(targets)}, expected 20332"
        )

    for scale in spots:
        accumulator = _ScaleFold(scale, "targeted", len(targets))
        _spot_sweep_with_solved(solved, scale, targets, accumulator)
        record, scale_groups, digest = accumulator.finish()
        per_scale.append(record)
        groups.extend(scale_groups)
        stream_digests.append(digest)
        del accumulator

    disagreement_groups = [
        group for group in groups if not group["agrees"] and group["count"] > 0
    ]
    for record in per_scale:
        scale = record["scale"]
        if sum(
            group["count"] for group in disagreement_groups if group["scale"] == scale
        ) != record["disagreements"]:
            raise RuntimeError(
                f"report disagreement conservation failed at scale {scale}"
            )

    return {
        "methodology": {
            "version": 1,
            "convention": "P1b tie-owner-wins",
            "predicate": "bx*q == p_T*M",
            "exhaustive_scales": exhaustive,
            "spot_scales": spots,
            "spot_target_count": len(targets),
            "no_sampling": True,
        },
        "per_scale": per_scale,
        "groups": groups,
        "disagreement_groups": disagreement_groups,
        "stream_digests": stream_digests,
    }
