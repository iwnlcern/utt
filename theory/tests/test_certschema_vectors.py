import pathlib
import subprocess
import sys

from certschema.game import ANY, apply_move, canonicalize, legal_moves, terminal
from certschema.vectors import (
    build_golden_verdict,
    build_p1,
    build_p2,
    build_p3,
    build_p4,
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


def test_p4_semantics():
    from certschema.vectors import p4_states

    s0, replies, x_move = p4_states()
    assert s0.counts() == (13, 12)
    assert s0.side_to_move() == "O"
    assert legal_moves(s0) == replies == [(5, 7), (5, 8)]

    states = [canonicalize(s0)[0]]
    for reply in replies:
        parent = apply_move(s0, *reply)
        assert parent.well_formed() and parent.side_to_move() == "X"
        assert parent.forced == (ANY if reply == (5, 7) else 8)
        child = apply_move(parent, *x_move)
        assert terminal(child) == "X"
        states.extend((canonicalize(parent)[0], canonicalize(child)[0]))
    assert len({state.serialize() for state in states}) == 5


def test_p2_p3_p4_are_valid_certificates_structurally():
    for blob in (build_p2(), build_p3(), build_p4()):
        assert blob[:8] == b"UTTTCERT" and len(blob) > 100


PKG = pathlib.Path(__file__).resolve().parents[1] / "certschema"


def test_committed_artifacts_match_generator_output(tmp_path):
    subprocess.run(
        [sys.executable, "-m", "certschema.generate", "--out", str(tmp_path)],
        check=True,
        cwd=PKG.parent,
    )
    for rel in [
        "vectors/golden-terminal-winx.utc",
        "vectors/golden-winx.utv",
        "vectors/p2-prover-winx.utc",
        "vectors/p3-opponent-winx.utc",
        "vectors/p4-opponent2-winx.utc",
        "catalogue.json",
    ]:
        assert (tmp_path / rel).read_bytes() == (PKG / rel).read_bytes(), rel
    committed = sorted(path.name for path in (PKG / "mutants").iterdir())
    fresh = sorted(path.name for path in (tmp_path / "mutants").iterdir())
    assert committed == fresh
    for name in fresh:
        assert (tmp_path / "mutants" / name).read_bytes() == (
            PKG / "mutants" / name
        ).read_bytes()


def test_committed_golden_bytes_equal_dd_hex():
    assert (PKG / "vectors/golden-terminal-winx.utc").read_bytes() == build_p1()
    assert (PKG / "vectors/golden-winx.utv").read_bytes() == build_golden_verdict()


def test_verdict_mutants_resolve_their_member_from_emitted_location():
    adjacent = PKG / "mutants" / "golden-terminal-winx.utc"
    assert adjacent.read_bytes() == build_p1()
    for mutant_id in ("MUT-V04", "MUT-V05", "MUT-V07", "MUT-V08"):
        blob = (PKG / "mutants" / f"{mutant_id}.bin").read_bytes()
        assert b"golden-terminal-winx.utc" in blob, mutant_id
    v03 = (PKG / "mutants" / "MUT-V03.bin").read_bytes()
    assert b"golden-terminal-abset.utc" in v03
    assert not (PKG / "mutants" / "golden-terminal-abset.utc").exists()
