from dataclasses import dataclass

from .rules import Move


@dataclass(frozen=True)
class Submission:
    fault: str | None
    bid: int | None
    move: Move | None


@dataclass(frozen=True)
class Resolution:
    winner: str
    reason: str
    coin: str | None
    payment: int


def resolve(
    sub_x: Submission,
    sub_o: Submission,
    tie_owner: str | None,
    pair_coin_seat: str,
) -> Resolution | None:
    if sub_x.fault is not None and sub_o.fault is not None:
        return None
    if sub_x.fault is not None:
        return Resolution(winner="O", reason="fault", coin=None, payment=0)
    if sub_o.fault is not None:
        return Resolution(winner="X", reason="fault", coin=None, payment=0)

    if sub_x.bid is None or sub_o.bid is None:
        raise ValueError("valid submissions require integer bids")
    if sub_x.bid > sub_o.bid:
        return Resolution(
            winner="X", reason="higher_bid", coin=None, payment=sub_x.bid
        )
    if sub_o.bid > sub_x.bid:
        return Resolution(
            winner="O", reason="higher_bid", coin=None, payment=sub_o.bid
        )

    if tie_owner is None:
        return Resolution(
            winner=pair_coin_seat,
            reason="tie_coin",
            coin=pair_coin_seat,
            payment=sub_x.bid,
        )
    return Resolution(
        winner=tie_owner,
        reason="tie_last_mover",
        coin=None,
        payment=sub_x.bid,
    )
