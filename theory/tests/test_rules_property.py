import random

from auction_ttt import rules as R


def _line_owners(board: str) -> set[str]:
    return {
        board[a]
        for a, b, c in R.LINES
        if board[a] != R.EMPTY and board[a] == board[b] == board[c]
    }


def test_seeded_playout_properties():
    rng = random.Random(0)
    for _ in range(500):
        board = R.EMPTY * 9
        h = rng.choice((R.X, R.O))
        bx = bo = 10
        destroyed = 0
        winning_bids = []

        while not R.is_terminal(board):
            assert R.legal_moves(board) == tuple(
                i for i, mark in enumerate(board) if mark == R.EMPTY
            )

            bid_x = rng.randint(0, bx)
            bid_o = rng.randint(0, bo)
            mover = R.resolve_auction(bid_x, bid_o, h)
            paid = bid_x if mover == R.X else bid_o
            if mover == R.X:
                bx -= paid
            else:
                bo -= paid
            destroyed += paid
            winning_bids.append(paid)

            cell = rng.choice(R.legal_moves(board))
            before = board
            board = R.apply_move(board, cell, mover)
            assert board[cell] == mover
            assert all(
                board[i] == before[i] for i in range(len(board)) if i != cell
            )
            assert len(_line_owners(board)) <= 1
            h = R.opponent(mover)

        assert destroyed == sum(winning_bids) == 20 - bx - bo
