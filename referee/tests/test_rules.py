import pytest

from poorman_referee.rules import INITIAL, Position, apply_move, legal_moves, local_status


WIN_LINES = (
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6),
)
DRAWN = "XOXXOOOXX"


def test_initial_moves_are_center_board():
    assert sorted(legal_moves(INITIAL)) == [(4, cell) for cell in range(9)]


def test_won_local_board_is_closed_even_with_empty_cells():
    board = ["........."] * 9
    board[2] = "XXX......"
    pos = Position(board=tuple(board), forced=2)

    assert all(local != 2 for local, _ in legal_moves(pos))
    assert len(legal_moves(pos)) == 72


def test_send_to_closed_board_frees_choice():
    board = ["........."] * 9
    board[0] = "XXX......"
    pos = Position(board=tuple(board), forced=None)

    result = apply_move(pos, (4, 0), "O")

    assert result.position.forced is None
    assert (1, 0) in legal_moves(result.position)
    assert all(local != 0 for local, _ in legal_moves(result.position))


def test_move_routes_by_cell_even_when_it_closes_its_local_board():
    board = ["........."] * 9
    board[4] = "XX......."
    pos = Position(board=tuple(board), forced=4)

    result = apply_move(pos, (4, 2), "X")

    assert result.position.forced == 2
    assert [(closure.local, closure.result) for closure in result.closures] == [(4, "X")]


@pytest.mark.parametrize("macro_line", WIN_LINES)
def test_move_detects_each_macro_win_line(macro_line):
    board = ["........."] * 9
    target = macro_line[-1]
    for local in macro_line[:-1]:
        board[local] = "XXX......"
    board[target] = "XX......."
    pos = Position(board=tuple(board), forced=target)

    result = apply_move(pos, (target, 2), "X")

    assert result.macro_line == macro_line
    assert result.terminal == "macro_win"


def test_ninth_local_closure_without_macro_is_all_closed():
    board = [DRAWN] * 9
    board[8] = "XOXXOOOX."
    pos = Position(board=tuple(board), forced=8)

    result = apply_move(pos, (8, 8), "X")

    assert result.closures[0].result == "full"
    assert result.macro_line is None
    assert result.terminal == "all_closed"
    assert result.position.forced is None


@pytest.mark.parametrize(
    ("cells", "expected"),
    [
        ("XXX......", "X"),
        ("O..O..O..", "O"),
        (DRAWN, "full"),
        ("XO.......", None),
    ],
)
def test_local_status_distinguishes_wins_full_and_open(cells, expected):
    assert local_status(cells) == expected


def test_occupied_cell_is_not_legal_and_cannot_be_applied():
    board = ["........."] * 9
    board[4] = "....X...."
    pos = Position(board=tuple(board), forced=4)

    assert (4, 4) not in legal_moves(pos)
    with pytest.raises(ValueError, match="illegal move"):
        apply_move(pos, (4, 4), "O")


def test_apply_move_rejects_noncanonical_mark():
    with pytest.raises(ValueError, match="mark"):
        apply_move(INITIAL, (4, 4), "x")
