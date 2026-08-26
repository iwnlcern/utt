import pytest

from poorman_referee.auction import Resolution, Submission, resolve


def valid(bid, move=(4, 4)):
    return Submission(fault=None, bid=bid, move=move)


def fault(name="timeout"):
    return Submission(fault=name, bid=None, move=None)


def test_higher_bid_wins_and_pays_own_bid():
    assert resolve(valid(9), valid(4), tie_owner="O", pair_coin_seat="O") == Resolution(
        winner="X", reason="higher_bid", coin=None, payment=9
    )


def test_equal_bid_is_won_by_tie_owner_and_costs_tied_amount():
    assert resolve(valid(17), valid(17), tie_owner="O", pair_coin_seat="X") == Resolution(
        winner="O", reason="tie_last_mover", coin=None, payment=17
    )


def test_first_move_tie_is_won_by_pair_coin_seat():
    assert resolve(valid(3), valid(3), tie_owner=None, pair_coin_seat="X") == Resolution(
        winner="X", reason="tie_coin", coin="X", payment=3
    )


def test_zero_zero_tie_uses_the_same_tie_owner_rule():
    result = resolve(valid(0), valid(0), tie_owner="O", pair_coin_seat="X")

    assert result == Resolution(
        winner="O", reason="tie_last_mover", coin=None, payment=0
    )


@pytest.mark.parametrize(("faulting", "winner"), [("X", "O"), ("O", "X")])
def test_single_fault_gives_opponent_a_free_auction_win(faulting, winner):
    submissions = {"X": valid(100), "O": valid(2)}
    submissions[faulting] = fault()

    assert resolve(
        submissions["X"], submissions["O"], tie_owner="X", pair_coin_seat="O"
    ) == Resolution(winner=winner, reason="fault", coin=None, payment=0)


def test_double_fault_returns_none_for_retry():
    assert resolve(fault("invalid_json"), fault("timeout"), None, "X") is None


def test_integer_exact_all_in_beats_one_unit_less():
    result = resolve(valid(10**9), valid(10**9 - 1), tie_owner="O", pair_coin_seat="O")

    assert result == Resolution(
        winner="X", reason="higher_bid", coin=None, payment=10**9
    )
