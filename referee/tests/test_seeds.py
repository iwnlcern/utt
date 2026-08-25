import hashlib

import pytest

from poorman_referee.seeds import (
    SEP,
    game_seed,
    pair_coin_seat,
    pair_order,
    pair_seed,
    validate_engine_ids,
)


def test_pair_seed_matches_known_byte_vector():
    assert pair_seed("seed", "A", "B", 1).hex() == (
        "305f0e604c0e0dd5813773ab917a73033e921e14469ccde41c5bf24f774d9cb5"
    )


def test_pair_seed_composition_uses_literal_separator_and_ascii_round():
    expected = hashlib.sha256(
        "tournoi".encode()
        + b"\x1f"
        + "z".encode()
        + b"\x1f"
        + "é".encode()
        + b"\x1f"
        + b"12"
    ).digest()

    assert SEP == b"\x1f"
    assert pair_seed("tournoi", "z", "é", 12) == expected


def test_pair_order_uses_utf8_byte_order():
    assert pair_order("é", "z") == ("z", "é")
    assert pair_order("alpha", "alpha-2") == ("alpha", "alpha-2")


@pytest.mark.parametrize(
    "ids",
    [
        ["same", "same"],
        ["ok", "bad\x1fid"],
    ],
)
def test_engine_ids_reject_duplicates_and_separator(ids):
    with pytest.raises(ValueError):
        validate_engine_ids(ids)


def test_game_seed_appends_one_raw_game_index_byte():
    parent = bytes(range(32))

    assert game_seed(parent, 2) == hashlib.sha256(parent + b"\x02").digest()


@pytest.mark.parametrize("k", [0, 3, 256])
def test_game_seed_rejects_nonpaired_game_indices(k):
    with pytest.raises(ValueError, match="1 or 2"):
        game_seed(bytes(32), k)


def test_pair_coin_has_reachable_even_and_odd_parities():
    examples = {}
    for index in range(1000):
        digest = pair_seed(f"seed-{index}", "A", "B", 1)
        examples.setdefault(digest[0] % 2, digest)
        if len(examples) == 2:
            break

    assert pair_coin_seat(examples[0]) == "X"
    assert pair_coin_seat(examples[1]) == "O"


def test_seed_derivation_is_deterministic():
    first = pair_seed("repeat", "A", "B", 9)

    assert pair_seed("repeat", "A", "B", 9) == first
    assert game_seed(first, 1) == game_seed(first, 1)
