from functools import cache
from itertools import product

X, O, EMPTY = "X", "O", "."
LINES = (
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6),
)


def opponent(p: str) -> str:
    if p == X:
        return O
    if p == O:
        return X
    raise ValueError(f"invalid player: {p!r}")


def winner(board: str) -> str | None:
    for a, b, c in LINES:
        if board[a] != EMPTY and board[a] == board[b] == board[c]:
            return board[a]
    return None


def is_full(board: str) -> bool:
    return EMPTY not in board


def is_terminal(board: str) -> bool:
    return winner(board) is not None or is_full(board)


def legal_moves(board: str) -> tuple[int, ...]:
    if is_terminal(board):
        return ()
    return tuple(i for i, mark in enumerate(board) if mark == EMPTY)


def apply_move(board: str, cell: int, player: str) -> str:
    if player not in (X, O):
        raise ValueError(f"invalid player: {player!r}")
    if is_terminal(board):
        raise ValueError("cannot move on terminal board")
    if not 0 <= cell < len(board) or board[cell] != EMPTY:
        raise ValueError(f"cell is not available: {cell}")
    return board[:cell] + player + board[cell + 1 :]


def resolve_auction(bid_x: int, bid_o: int, h: str) -> str:
    if h not in (X, O):
        raise ValueError(f"invalid tie owner: {h!r}")
    if bid_x < 0 or bid_o < 0:
        raise ValueError("bids must be nonnegative")
    if bid_x > bid_o:
        return X
    if bid_o > bid_x:
        return O
    return h


def terminal_result(board: str, bx: int, bo: int) -> str:
    line_winner = winner(board)
    if line_winner is not None:
        return line_winner
    if not is_full(board):
        raise ValueError("board is not terminal")
    if bx > bo:
        return X
    if bo > bx:
        return O
    return "draw"


@cache
def nonterminal_boards() -> tuple[str, ...]:
    boards = ("".join(cells) for cells in product("XO.", repeat=9))
    return tuple(board for board in boards if winner(board) is None and not is_full(board))
