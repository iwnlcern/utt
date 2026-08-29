from certschema.game import apply_move, canonicalize, legal_moves, terminal
from certschema.vectors import (
    build_golden_verdict,
    build_p1,
    build_p2,
    build_p3,
    golden_root,
)


def test_p1_and_verdict_byte_match_dd():
    assert len(build_p1()) == 237 and len(build_golden_verdict()) == 139


def test_p2_parent_semantics():
    from certschema.vectors import p2_states

    parent_raw, move = p2_states()
    assert parent_raw.side_to_move() == "X" and terminal(parent_raw) is None
    child = apply_move(parent_raw, *move)
    assert terminal(child) == "X"
    assert canonicalize(child)[0] == golden_root()


def test_p3_chain_semantics():
    from certschema.vectors import p3_states

    s0, o_move, x_move = p3_states()
    assert s0.side_to_move() == "O" and legal_moves(s0) == [o_move]
    s1 = apply_move(s0, *o_move)
    assert s1.forced == 8 and s1.side_to_move() == "X"
    s2 = apply_move(s1, *x_move)
    assert terminal(s2) == "X"


def test_p2_p3_are_valid_certificates_structurally():
    for blob in (build_p2(), build_p3()):
        assert blob[:8] == b"UTTTCERT" and len(blob) > 100
