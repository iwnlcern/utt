from dataclasses import dataclass
from fractions import Fraction

from auction_ttt import rules as R


@dataclass(frozen=True)
class Node:
    T: Fraction
    r: Fraction
    a: Fraction
    b: Fraction
    best_x: int
    best_o: int
    zugzwang: bool


def backup(a: Fraction, b: Fraction, h: str) -> tuple[Fraction, Fraction]:
    if h not in (R.X, R.O):
        raise ValueError(f"invalid tie owner: {h!r}")
    if a <= b:
        denominator = 1 - a + b
        return b / denominator, (b - a) / denominator
    return (a if h == R.X else b), Fraction(0)


def _terminal_threshold(board: str) -> Fraction:
    line_winner = R.winner(board)
    if line_winner == R.X:
        return Fraction(0)
    if line_winner == R.O:
        return Fraction(1)
    if R.is_full(board):
        return Fraction(1, 2)
    raise ValueError("board is not terminal")


def threshold(board: str, h: str, solved: dict[tuple[str, str], Node]) -> Fraction:
    if R.is_terminal(board):
        return _terminal_threshold(board)
    try:
        return solved[(board, h)].T
    except KeyError as exc:
        raise AssertionError("child not materialized") from exc


def solve() -> dict[tuple[str, str], Node]:
    solved: dict[tuple[str, str], Node] = {}
    boards = sorted(R.nonterminal_boards(), key=lambda board: board.count("."))

    for board in boards:
        moves = R.legal_moves(board)
        x_children = [
            (threshold(R.apply_move(board, cell, R.X), R.O, solved), cell)
            for cell in moves
        ]
        o_children = [
            (threshold(R.apply_move(board, cell, R.O), R.X, solved), cell)
            for cell in moves
        ]
        a, best_x = min(x_children, key=lambda item: item[0])
        b, best_o = max(o_children, key=lambda item: item[0])

        for h in (R.X, R.O):
            value, critical_bid = backup(a, b, h)
            solved[(board, h)] = Node(
                T=value,
                r=critical_bid,
                a=a,
                b=b,
                best_x=best_x,
                best_o=best_o,
                zugzwang=a > b,
            )

    return solved
