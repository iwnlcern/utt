from auction_ttt import rules as R
from auction_ttt.discrete import (
    DRAW,
    OWIN,
    XWIN,
    full_matrix_value,
    reachable_masks,
    solve,
)


def test_mask_start_state():
    masks = reachable_masks(2)
    board = R.apply_move("." * 9, 0, "X")

    assert masks[(board, "O")][2, 2]
    assert masks[(board, "O")][0, 2]


def test_mask_budget_conservation():
    masks = reachable_masks(2)

    for (board, _h), grid in masks.items():
        marks = 9 - board.count(".")
        for bx in range(3):
            for bo in range(3):
                if grid[bx, bo]:
                    assert (2 - bx) + (2 - bo) <= 2 * marks or marks == 0


def test_terminal_grid_chips():
    values, _masks = solve(2)
    grid = values[("XOXXOOOXX", "X")]

    assert grid[2, 1] == XWIN
    assert grid[1, 2] == OWIN
    assert grid[1, 1] == DRAW


def test_one_move_win_all_budgets():
    values, _masks = solve(2)
    grid = values[("XX.......", "X")]

    assert grid[0, 0] == XWIN


def test_determinacy_exhaustive_small():
    for n in (1, 2, 3, 4):
        solve(n)


def test_targeted_prefix_reachability():
    from auction_ttt.discrete import solve_targeted

    result = solve_targeted("X" + "." * 8, 8)

    assert result.mask[("X........", "X")].sum() == 0
    mask = result.mask[("X........", "O")]
    assert mask[:, 8].any()
    assert not mask[:, :8].any()


def test_fast_equals_full_matrix_N4():
    N = 4
    values, masks = solve(N)

    for (board, h), grid in values.items():
        if R.is_terminal(board):
            continue
        mask = masks[(board, h)]
        for bx in range(N + 1):
            for bo in range(N + 1):
                if not mask[bx, bo]:
                    continue
                maximin, minimax = full_matrix_value(board, h, bx, bo, values)
                assert maximin == minimax == grid[bx, bo]
