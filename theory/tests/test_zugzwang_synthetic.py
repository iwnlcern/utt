from fractions import Fraction as F

import numpy as np
import pytest

from auction_ttt import rules as R
from auction_ttt.continuous import solve
from auction_ttt.discrete import OWIN, XWIN


def test_c11_no_reachable_zugzwang():
    solved = solve()

    assert len(solved) == 22186
    assert len({board for board, _h in solved}) == 11093
    assert sum(1 for node in solved.values() if node.zugzwang) == 0
    assert all(node.a <= node.b for node in solved.values())


def _threshold_payoff(bx, bo, threshold):
    """Synthetic terminal payoff; deliberately not canonical chip comparison."""
    return (
        XWIN
        if bx * threshold.denominator
        > threshold.numerator * (bx + bo)
        else OWIN
    )


def _one_step_matrix(a, b, h, bx, bo):
    bid_x = np.arange(bx + 1, dtype=np.int64)[:, None]
    bid_o = np.arange(bo + 1, dtype=np.int64)[None, :]
    x_children = np.array(
        [_threshold_payoff(bx - i, bo, a) for i in range(bx + 1)],
        dtype=np.int8,
    )[:, None]
    o_children = np.array(
        [_threshold_payoff(bx, bo - j, b) for j in range(bo + 1)],
        dtype=np.int8,
    )[None, :]
    x_moves = (bid_x > bid_o) | ((bid_x == bid_o) & (h == R.X))
    return np.where(x_moves, x_children, o_children)


@pytest.mark.parametrize("a,b", [(F(3, 4), F(1, 4)), (F(2, 3), F(1, 3)), (F(1), F(0))])
@pytest.mark.parametrize("h", [R.X, R.O])
@pytest.mark.parametrize("scale", [8, 16, 32, 64])
def test_synthetic_zugzwang_one_step_games(a, b, h, scale):
    """Validate C3 algebra without claiming a canonical reachable witness."""
    threshold = a if h == R.X else b

    for bx in range(scale + 1):
        for bo in range(scale + 1):
            matrix = _one_step_matrix(a, b, h, bx, bo)
            row_minima = matrix.min(axis=1)
            column_maxima = matrix.max(axis=0)
            maximin = int(row_minima.max())
            minimax = int(column_maxima.min())

            assert maximin == minimax

            total = bx + bo
            signed_distance = (
                bx * threshold.denominator - threshold.numerator * total
            )
            if total and abs(signed_distance) > threshold.denominator:
                expected = XWIN if signed_distance > 0 else OWIN
                assert maximin == expected

            inside_zugzwang_interval = (
                total
                and bx * a.denominator < a.numerator * total
                and bx * b.denominator > b.numerator * total
            )
            if inside_zugzwang_interval:
                assert int(matrix[0, 0]) == maximin
                assert int(row_minima[0]) == maximin
                assert int(column_maxima[0]) == maximin
