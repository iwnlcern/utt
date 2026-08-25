from dataclasses import dataclass
from itertools import product

import numpy as np

from auction_ttt import rules as R

OWIN, DRAW, XWIN = 0, 1, 2


class NonDeterminacy(Exception):
    """Pure maximin != pure minimax at a REACHABLE state. HARD STOP (DD C9)."""


class ValueTable(dict):
    """Value grids plus compact diagnostics for off-mask inequalities."""

    def __init__(self):
        super().__init__()
        self.diagnostics: list[tuple[str, str, np.ndarray]] = []


@dataclass(frozen=True)
class TargetedResult:
    values: dict[tuple[str, str], np.ndarray]
    mask: dict[tuple[str, str], np.ndarray]
    diagnostics: list[tuple[str, str, np.ndarray]]


def _validate_scale(N: int) -> None:
    if not isinstance(N, int) or isinstance(N, bool) or N < 0:
        raise ValueError("N must be a nonnegative integer")


def _all_boards() -> tuple[str, ...]:
    return tuple("".join(cells) for cells in product("XO.", repeat=9))


def _new_masks(boards: tuple[str, ...], N: int):
    shape = (N + 1, N + 1)
    return {
        (board, h): np.zeros(shape, dtype=bool)
        for board in boards
        for h in (R.X, R.O)
    }


def _propagate_masks(
    boards: tuple[str, ...],
    N: int,
    masks: dict[tuple[str, str], np.ndarray],
) -> None:
    board_set = set(boards)
    ordered = sorted(boards, key=lambda board: 9 - board.count("."))

    for board in ordered:
        if R.is_terminal(board):
            continue
        moves = R.legal_moves(board)

        if board == "." * 9:
            if not (masks[(board, R.X)][N, N] or masks[(board, R.O)][N, N]):
                continue
            for cell in moves:
                x_child = R.apply_move(board, cell, R.X)
                o_child = R.apply_move(board, cell, R.O)
                if x_child in board_set:
                    masks[(x_child, R.O)][:, N] = True
                if o_child in board_set:
                    masks[(o_child, R.X)][N, :] = True
            continue

        for h in (R.X, R.O):
            source = masks[(board, h)]
            if not source.any():
                continue
            for cell in moves:
                x_child = R.apply_move(board, cell, R.X)
                if x_child in board_set:
                    target = masks[(x_child, R.O)]
                    if h == R.X:
                        target |= source
                    for payment in range(1, N + 1):
                        target[: N + 1 - payment, :] |= source[payment:, :]

                o_child = R.apply_move(board, cell, R.O)
                if o_child in board_set:
                    target = masks[(o_child, R.X)]
                    if h == R.O:
                        target |= source
                    for payment in range(1, N + 1):
                        target[:, : N + 1 - payment] |= source[:, payment:]


def _reachability_for_boards(
    boards: tuple[str, ...], N: int
) -> dict[tuple[str, str], np.ndarray]:
    masks = _new_masks(boards, N)
    root = "." * 9
    if root in boards:
        masks[(root, R.X)][N, N] = True
        masks[(root, R.O)][N, N] = True
    _propagate_masks(boards, N, masks)
    return masks


def reachable_masks(N: int) -> dict[tuple[str, str], np.ndarray]:
    _validate_scale(N)
    return _reachability_for_boards(_all_boards(), N)


def _terminal_grid(board: str, N: int) -> np.ndarray:
    winner = R.winner(board)
    if winner == R.X:
        return np.full((N + 1, N + 1), XWIN, dtype=np.int8)
    if winner == R.O:
        return np.full((N + 1, N + 1), OWIN, dtype=np.int8)
    if not R.is_full(board):
        raise ValueError("board is not terminal")

    bx = np.arange(N + 1)[:, None]
    bo = np.arange(N + 1)[None, :]
    return np.where(bx > bo, XWIN, np.where(bx < bo, OWIN, DRAW)).astype(
        np.int8
    )


def _fast_value(WX: np.ndarray, WO: np.ndarray, h: str) -> tuple[np.ndarray, np.ndarray]:
    size = WX.shape[0]
    N = size - 1
    prefmin_wo = np.minimum.accumulate(WO, axis=1)
    prefmax_wx = np.maximum.accumulate(WX, axis=0)

    maximin = np.full_like(WX, -1)
    for bid_x in range(size):
        rows = slice(bid_x, size)
        row_min = np.full((size - bid_x, size), XWIN + 1, dtype=np.int8)
        if bid_x > 0:
            row_min = np.minimum(row_min, WX[: size - bid_x, :])
        if bid_x < N:
            row_min[:, bid_x + 1 :] = np.minimum(
                row_min[:, bid_x + 1 :],
                prefmin_wo[bid_x:, : N - bid_x],
            )
        if h == R.X:
            tie = WX[: size - bid_x, :]
            row_min[:, bid_x:] = np.minimum(
                row_min[:, bid_x:], tie[:, bid_x:]
            )
        else:
            tie = WO[bid_x:, : size - bid_x]
            row_min[:, bid_x:] = np.minimum(row_min[:, bid_x:], tie)
        maximin[rows, :] = np.maximum(maximin[rows, :], row_min)

    minimax = np.full_like(WX, XWIN + 1)
    for bid_o in range(size):
        columns = slice(bid_o, size)
        column_max = np.full((size, size - bid_o), -1, dtype=np.int8)
        if bid_o > 0:
            column_max = np.maximum(column_max, WO[:, : size - bid_o])
        if bid_o < N:
            column_max[bid_o + 1 :, :] = np.maximum(
                column_max[bid_o + 1 :, :],
                prefmax_wx[: N - bid_o, bid_o:],
            )
        if h == R.X:
            tie = WX[: size - bid_o, bid_o:]
        else:
            tie = WO[bid_o:, : size - bid_o]
        column_max[bid_o:, :] = np.maximum(column_max[bid_o:, :], tie)
        minimax[:, columns] = np.minimum(minimax[:, columns], column_max)

    return maximin, minimax


def _solve_boards(
    boards: tuple[str, ...],
    N: int,
    masks: dict[tuple[str, str], np.ndarray],
) -> ValueTable:
    values = ValueTable()
    ordered = sorted(boards, key=lambda board: board.count("."))

    for board in ordered:
        if R.is_terminal(board):
            grid = _terminal_grid(board, N)
            values[(board, R.X)] = grid.copy()
            values[(board, R.O)] = grid.copy()
            continue

        moves = R.legal_moves(board)
        WX = np.maximum.reduce(
            [values[(R.apply_move(board, cell, R.X), R.O)] for cell in moves]
        )
        WO = np.minimum.reduce(
            [values[(R.apply_move(board, cell, R.O), R.X)] for cell in moves]
        )

        for h in (R.X, R.O):
            maximin, minimax = _fast_value(WX, WO, h)
            mismatch = maximin != minimax
            reachable_mismatch = mismatch & masks[(board, h)]
            if reachable_mismatch.any():
                bx, bo = np.argwhere(reachable_mismatch)[0]
                raise NonDeterminacy(
                    f"reachable state {(board, h, int(bx), int(bo))}: "
                    f"maximin={int(maximin[bx, bo])}, "
                    f"minimax={int(minimax[bx, bo])}"
                )
            off_mask = np.argwhere(mismatch & ~masks[(board, h)])
            if off_mask.size:
                values.diagnostics.append((board, h, off_mask))
            values[(board, h)] = maximin

    return values


def solve(
    N: int,
) -> tuple[
    dict[tuple[str, str], np.ndarray], dict[tuple[str, str], np.ndarray]
]:
    _validate_scale(N)
    boards = _all_boards()
    masks = _reachability_for_boards(boards, N)
    return _solve_boards(boards, N, masks), masks


def full_matrix_value(
    board: str,
    h: str,
    bx: int,
    bo: int,
    V: dict[tuple[str, str], np.ndarray],
) -> tuple[int, int]:
    if h not in (R.X, R.O):
        raise ValueError(f"invalid tie owner: {h!r}")
    if R.is_terminal(board):
        value = int(V[(board, h)][bx, bo])
        return value, value

    moves = R.legal_moves(board)
    rows: list[int] = []
    columns = {(bid_o, cell_o): [] for bid_o in range(bo + 1) for cell_o in moves}

    for bid_x in range(bx + 1):
        for cell_x in moves:
            outcomes = []
            for bid_o in range(bo + 1):
                for cell_o in moves:
                    winner = R.resolve_auction(bid_x, bid_o, h)
                    if winner == R.X:
                        child = R.apply_move(board, cell_x, R.X)
                        value = int(V[(child, R.O)][bx - bid_x, bo])
                    else:
                        child = R.apply_move(board, cell_o, R.O)
                        value = int(V[(child, R.X)][bx, bo - bid_o])
                    outcomes.append(value)
                    columns[(bid_o, cell_o)].append(value)
            rows.append(min(outcomes))

    return max(rows), min(max(outcomes) for outcomes in columns.values())


def _prefix_boards(target: str) -> tuple[str, ...]:
    choices = [(".", mark) if mark != "." else (".",) for mark in target]
    return tuple("".join(cells) for cells in product(*choices))


def _descendant_boards(target: str) -> tuple[str, ...]:
    choices = [("X", "O", ".") if mark == "." else (mark,) for mark in target]
    return tuple("".join(cells) for cells in product(*choices))


def _validate_board(board: str) -> None:
    if len(board) != 9 or any(mark not in "XO." for mark in board):
        raise ValueError("board must contain exactly nine X, O, or . cells")


def solve_targeted(board: str, N: int) -> TargetedResult:
    _validate_board(board)
    _validate_scale(N)

    prefix = _prefix_boards(board)
    prefix_masks = _reachability_for_boards(prefix, N)
    target_masks = {
        (board, h): prefix_masks[(board, h)].copy() for h in (R.X, R.O)
    }
    if not any(mask.any() for mask in target_masks.values()):
        raise ValueError(f"target board is unreachable at scale {N}")

    descendants = _descendant_boards(board)
    descendant_masks = _new_masks(descendants, N)
    for key, mask in target_masks.items():
        descendant_masks[key] = mask.copy()
    _propagate_masks(descendants, N, descendant_masks)
    descendant_values = _solve_boards(descendants, N, descendant_masks)
    target_values = {
        (board, h): descendant_values[(board, h)] for h in (R.X, R.O)
    }
    return TargetedResult(
        values=target_values,
        mask=target_masks,
        diagnostics=descendant_values.diagnostics,
    )
