import random

import numpy as np

from auction_ttt import rules as R
from auction_ttt.continuous import solve as solve_continuous
from auction_ttt.crosscheck import in_band
from auction_ttt.discrete import full_matrix_value, solve as solve_discrete


def _reservoir_add(reservoir, item, seen, limit, rng):
    seen += 1
    if len(reservoir) < limit:
        reservoir.append(item)
    else:
        replacement = rng.randrange(seen)
        if replacement < limit:
            reservoir[replacement] = item
    return seen


def test_c9a_seeded_stratified_sample_matches_fast_path_at_n12():
    """Catch any reduced-matrix optimization that changes full-game values."""
    scale = 12
    rng = random.Random(0)
    solved_continuous = solve_continuous()
    values, masks = solve_discrete(scale)

    near_band = []
    both_zero = []
    uniform = []
    seen_near = seen_zero = seen_uniform = 0

    for key in sorted(values):
        board, h = key
        if R.is_terminal(board):
            continue
        threshold = solved_continuous[key].T
        for bx, bo in np.argwhere(masks[key]):
            state = (board, h, int(bx), int(bo))
            seen_uniform = _reservoir_add(
                uniform, state, seen_uniform, 120, rng
            )
            if in_band(board, threshold, int(bx), int(bo)):
                seen_near = _reservoir_add(
                    near_band, state, seen_near, 80, rng
                )
            if bx == 0 and bo == 0:
                seen_zero = _reservoir_add(
                    both_zero, state, seen_zero, 40, rng
                )

    assert len(near_band) == 80
    assert len(both_zero) == 40
    assert len(uniform) == 120

    sample = list(dict.fromkeys([*near_band, *both_zero, *uniform]))
    if len(sample) < 200:
        raise AssertionError(f"stratified sample too small: {len(sample)}")

    for board, h, bx, bo in sample:
        maximin, minimax = full_matrix_value(board, h, bx, bo, values)
        assert maximin == minimax == int(values[(board, h)][bx, bo])
