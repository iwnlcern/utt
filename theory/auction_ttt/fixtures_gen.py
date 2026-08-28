"""Deterministically generate theory-owned oracle fixture categories."""

from __future__ import annotations

import difflib
import json
from fractions import Fraction
from pathlib import Path

import numpy as np

from auction_ttt import rules as R
from auction_ttt.continuous import Node, backup, solve as solve_continuous
from auction_ttt.discrete import DRAW, OWIN, XWIN, solve as solve_discrete


GENERATED = ("thresholds_ttt3.json", "backup_cases.json")
HAND_AUTHORED = (
    "legality.json",
    "closure_routing.json",
    "auction_traces.json",
    "terminal.json",
)
SCALES = (8, 16, 32)
_RESULT = {OWIN: "O", DRAW: "draw", XWIN: "X"}


def _fraction_text(value: Fraction) -> str:
    return f"{value.numerator}/{value.denominator}"


def _nearest_masked_point(
    board: str,
    h: str,
    scale: int,
    node: Node,
    values,
    masks,
) -> dict[str, int | str]:
    mask = np.asarray(masks[(board, h)], dtype=bool)
    candidates = np.argwhere(mask)
    candidates = candidates[np.sum(candidates, axis=1) > 0]
    if not len(candidates):
        raise RuntimeError(f"no positive-total masked point for {(board, h, scale)!r}")

    p, q = node.T.numerator, node.T.denominator
    bx, bo = min(
        ((int(raw_bx), int(raw_bo)) for raw_bx, raw_bo in candidates),
        key=lambda pair: (
            abs(pair[0] * q - p * (pair[0] + pair[1])),
            pair[0],
            pair[1],
        ),
    )
    return {
        "scale": scale,
        "bx": bx,
        "bo": bo,
        "result": _RESULT[int(values[(board, h)][bx, bo])],
    }


def _threshold_fixture(
    fixture_id: str,
    board: str,
    h: str,
    notes: str,
    solved,
    discrete,
) -> dict[str, object]:
    node = solved[(board, h)]
    if node.zugzwang:
        raise RuntimeError("reachable zugzwang threshold fixtures are forbidden by C11")
    expectations = [
        _nearest_masked_point(board, h, scale, node, *discrete[scale])
        for scale in SCALES
    ]
    return {
        "id": fixture_id,
        "game": "ttt3",
        "consumed_by": ["engine", "ui", "theory"],
        "notes": (
            f"{notes} The engine tag reserves future search-consumer use; "
            "the landed engine rules fixture runner skips game != uttt."
        ),
        "state": {
            "board": board,
            "forced": None,
            "h": h,
            "budgets": {
                "x": expectations[-1]["bx"],
                "o": expectations[-1]["bo"],
            },
        },
        "expected_T": {
            R.X: _fraction_text(solved[(board, R.X)].T),
            R.O: _fraction_text(solved[(board, R.O)].T),
        },
        "expected_critical_bid": _fraction_text(node.r),
        "expected_best_move": {
            "if_X_wins": node.best_x,
            "if_O_wins": node.best_o,
        },
        "expected_child_h": {"if_X_wins": R.O, "if_O_wins": R.X},
        "discrete_expectations": expectations,
    }


def _thresholds_payload() -> dict[str, object]:
    solved = solve_continuous()
    discrete = {scale: solve_discrete(scale) for scale in SCALES}
    fixtures = [
        _threshold_fixture(
            "threshold-a-equals-b-zero-critical",
            ".OXXOOOXX",
            R.X,
            "Census-minimum reachable a=b state; r=0. All scale points are selected only from the exact reachability mask and serve as knife-edge checks.",
            solved,
            discrete,
        ),
        _threshold_fixture(
            "threshold-a-less-than-b-after-x-opening",
            "X........",
            R.O,
            "Reachable non-root a<b state after X opens; continuous backup and masked discrete outcomes are kept in separate fields.",
            solved,
            discrete,
        ),
        _threshold_fixture(
            "threshold-root-conditional-h-x",
            ".........",
            R.X,
            "Conditional ordinary-ttt3 root with X owning a bid tie; paired with the h=O fixture rather than assigning a value to the hidden coin.",
            solved,
            discrete,
        ),
        _threshold_fixture(
            "threshold-root-conditional-h-o",
            ".........",
            R.O,
            "Conditional ordinary-ttt3 root with O owning a bid tie; paired with the h=X fixture rather than assigning a value to the hidden coin.",
            solved,
            discrete,
        ),
    ]
    return {"schema_version": 1, "game": "ttt3", "fixtures": fixtures}


def _backup_fixture(
    fixture_id: str, a: Fraction, b: Fraction, h: str, notes: str
) -> dict[str, object]:
    threshold, critical_bid = backup(a, b, h)
    return {
        "id": fixture_id,
        "game": "backup",
        "consumed_by": ["engine", "theory"],
        "notes": notes,
        "a": _fraction_text(a),
        "b": _fraction_text(b),
        "h": h,
        "expected_T": _fraction_text(threshold),
        "expected_critical_bid": _fraction_text(critical_bid),
    }


def _backup_payload() -> dict[str, object]:
    cases = (
        ("backup-zugzwang-three-quarters-h-x", Fraction(3, 4), Fraction(1, 4), R.X),
        ("backup-zugzwang-three-quarters-h-o", Fraction(3, 4), Fraction(1, 4), R.O),
        ("backup-zugzwang-two-thirds-h-x", Fraction(2, 3), Fraction(1, 3), R.X),
        ("backup-zugzwang-two-thirds-h-o", Fraction(2, 3), Fraction(1, 3), R.O),
        ("backup-equal-children-zero-critical", Fraction(1, 2), Fraction(1, 2), R.X),
        ("backup-ordered-children-positive-critical", Fraction(1, 4), Fraction(3, 4), R.O),
    )
    fixtures = [
        _backup_fixture(
            fixture_id,
            a,
            b,
            h,
            "Abstract backup-operator evidence; theory-only until an addressed MR17 engine-tag acknowledgment lands.",
        )
        for fixture_id, a, b, h in cases
    ]
    return {"schema_version": 1, "game": "backup", "fixtures": fixtures}


def generated_payloads() -> dict[str, dict[str, object]]:
    return {
        "thresholds_ttt3.json": _thresholds_payload(),
        "backup_cases.json": _backup_payload(),
    }


def render(payload: dict[str, object]) -> str:
    return json.dumps(payload, indent=2, sort_keys=True) + "\n"


def write_or_check(root: Path, *, check: bool) -> list[str]:
    mismatches: list[str] = []
    for filename, payload in generated_payloads().items():
        path = root / filename
        expected = render(payload)
        if not check:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(expected, encoding="utf-8")
            continue
        actual = path.read_text(encoding="utf-8") if path.is_file() else ""
        if actual != expected:
            mismatches.append(filename)
            print(
                "".join(
                    difflib.unified_diff(
                        actual.splitlines(keepends=True),
                        expected.splitlines(keepends=True),
                        fromfile=str(path),
                        tofile=f"generated:{filename}",
                    )
                ),
                end="",
            )
    return mismatches
