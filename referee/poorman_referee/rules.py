from dataclasses import dataclass


Move = tuple[int, int]
_WIN_LINES = (
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6),
)


@dataclass(frozen=True)
class Position:
    board: tuple[str, ...]
    forced: int | None


@dataclass(frozen=True)
class Closure:
    local: int
    result: str


@dataclass(frozen=True)
class ApplyResult:
    position: Position
    closures: tuple[Closure, ...]
    macro_line: tuple[int, int, int] | None
    terminal: str | None


INITIAL = Position(board=("." * 9,) * 9, forced=4)


def local_status(cells: str) -> str | None:
    for a, b, c in _WIN_LINES:
        if cells[a] in "XO" and cells[a] == cells[b] == cells[c]:
            return cells[a]
    if "." not in cells:
        return "full"
    return None


def legal_moves(pos: Position) -> list[Move]:
    if pos.forced is not None and local_status(pos.board[pos.forced]) is None:
        locals_ = (pos.forced,)
    else:
        locals_ = tuple(
            local for local, cells in enumerate(pos.board) if local_status(cells) is None
        )
    return [
        (local, cell)
        for local in locals_
        for cell, mark in enumerate(pos.board[local])
        if mark == "."
    ]


def apply_move(pos: Position, move: Move, mark: str) -> ApplyResult:
    if mark not in ("X", "O"):
        raise ValueError(f"invalid mark: {mark!r}")
    if move not in legal_moves(pos):
        raise ValueError(f"illegal move: {move!r}")

    local, cell = move
    board = list(pos.board)
    cells = list(board[local])
    cells[cell] = mark
    board[local] = "".join(cells)
    board_tuple = tuple(board)

    closed = local_status(board_tuple[local])
    closures = (Closure(local=local, result=closed),) if closed is not None else ()

    statuses = tuple(local_status(local_board) for local_board in board_tuple)
    macro_line = next(
        (
            line
            for line in _WIN_LINES
            if all(statuses[index] == mark for index in line)
        ),
        None,
    )
    terminal = "macro_win" if macro_line is not None else None
    if terminal is None and all(status is not None for status in statuses):
        terminal = "all_closed"

    forced = cell if local_status(board_tuple[cell]) is None else None
    return ApplyResult(
        position=Position(board=board_tuple, forced=forced),
        closures=closures,
        macro_line=macro_line,
        terminal=terminal,
    )
