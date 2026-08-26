from fractions import Fraction

from auction_ttt import rules as R


def _terminal_value(board: str, draw: str) -> Fraction:
    line_winner = R.winner(board)
    if line_winner == R.X:
        return Fraction(0)
    if line_winner == R.O:
        return Fraction(1)
    if R.is_full(board):
        return Fraction(1) if draw == R.O else Fraction(1, 2)
    raise ValueError("board is not terminal")


def solve_richman(draw: str) -> dict[str, Fraction]:
    if draw not in (R.O, "half"):
        raise ValueError(f"invalid draw convention: {draw!r}")

    solved: dict[str, Fraction] = {}

    def value(board: str) -> Fraction:
        if R.is_terminal(board):
            return _terminal_value(board, draw)
        try:
            return solved[board]
        except KeyError as exc:
            raise AssertionError("child not materialized") from exc

    for board in sorted(R.nonterminal_boards(), key=lambda b: b.count(".")):
        moves = R.legal_moves(board)
        a = min(value(R.apply_move(board, cell, R.X)) for cell in moves)
        b = max(value(R.apply_move(board, cell, R.O)) for cell in moves)
        solved[board] = (a + b) / 2

    return solved
