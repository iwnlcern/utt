import pytest

from auction_ttt import rules as R


# winner/terminal (spec rule 7 analog)
def test_winner_row():
    assert R.winner("XXX" + "." * 6) == "X"


def test_winner_none_empty():
    assert R.winner("." * 9) is None


def test_full_draw_board():
    b = "XOXXOOOXX"  # hand-checked: no line, full
    assert R.winner(b) is None and R.is_full(b) and R.is_terminal(b)


# terminal chips (spec rule 8 analog): leader wins, equality draws
def test_chip_leader_wins():
    assert R.terminal_result("XOXXOOOXX", 5, 4) == "X"


def test_chip_equal_draws():
    assert R.terminal_result("XOXXOOOXX", 4, 4) == "draw"


def test_board_win_beats_chips():
    assert R.terminal_result("XXX......", 0, 9) == "X"


# auction resolution (spec rules 3-4): higher pays own bid and moves; tie -> h
def test_higher_bid_wins():
    assert R.resolve_auction(5, 3, "O") == "X"


def test_tie_goes_to_owner():
    assert R.resolve_auction(4, 4, "O") == "O"


def test_zero_zero_tie():
    assert R.resolve_auction(0, 0, "X") == "X"


# h transition (amended spec paragraph, main@2aef375): h' = opponent(mover);
# consecutive paid wins by the same player KEEP h (DD C4).
def test_h_persistence_consecutive_wins():
    h = "O"  # X moved last
    w = R.resolve_auction(3, 1, h)  # X outbids again
    assert w == "X" and R.opponent(w) == "O"  # h stays "O"


# both-zero alternation (spec rule 9): 0-0 ties alternate the mover
def test_both_zero_chain_alternates():
    h = "X"
    seq = []
    for _ in range(4):
        w = R.resolve_auction(0, 0, h)
        seq.append(w)
        h = R.opponent(w)
    assert seq == ["X", "O", "X", "O"]


# one stack zero: opponent may win with 1 unit or contest the 0 tie
def test_one_stack_zero():
    assert R.resolve_auction(0, 1, "X") == "O"
    assert R.resolve_auction(0, 0, "X") == "X"


# apply_move guards
def test_apply_move_occupied_raises():
    with pytest.raises(ValueError):
        R.apply_move("X" + "." * 8, 0, "O")


def test_apply_move_on_terminal_raises():
    with pytest.raises(ValueError):
        R.apply_move("XXX" + "." * 6, 3, "O")


def test_nonterminal_count():
    bs = R.nonterminal_boards()
    assert len(bs) == len(set(bs)) and "." * 9 in bs
