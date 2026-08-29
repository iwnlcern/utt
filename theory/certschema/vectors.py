"""Positive vectors and mutants for the DD §9 conformance corpus."""

from .game import ANY, State, apply_move, canonicalize, legal_moves, terminal
from .wire import (
    CLAIM,
    KIND,
    Record,
    build_certificate,
    build_verdict,
    header_digest,
)


def _mk(xd: dict, od: dict, forced: int) -> State:
    x = [0] * 9
    o = [0] * 9
    for b, cells in xd.items():
        for c in cells:
            x[b] |= 1 << c
    for b, cells in od.items():
        for c in cells:
            o[b] |= 1 << c
    return State(x=tuple(x), o=tuple(o), forced=forced)


def golden_root() -> State:
    raw = _mk(
        {0: (0, 1, 2), 1: (0, 1, 2), 2: (0, 1, 2)},
        {3: (3, 5), 4: (1, 7), 5: (0, 8), 6: (4,), 7: (4,)},
        ANY,
    )
    return canonicalize(raw)[0]


def _prover_record(raw_parent: State, move: tuple) -> Record:
    canonical_parent, transform = canonicalize(raw_parent)
    board, cell = move
    stored = transform[board] * 9 + transform[cell]
    return Record(state=canonical_parent, kind=KIND["PROVER"], payload=stored)


def build_p1() -> bytes:
    root = golden_root()
    return build_certificate(
        CLAIM["WIN_X"], root, {17: [Record(root, KIND["TERMINAL"], 0xFF)]}
    )


def build_golden_verdict() -> bytes:
    root = golden_root()
    return build_verdict(
        0,
        root,
        [(CLAIM["WIN_X"], "golden-terminal-winx.utc", header_digest(build_p1()))],
    )


def p2_states():
    golden = golden_root()
    x = list(golden.x)
    x[8] &= ~(1 << 8)
    parent = State(x=tuple(x), o=golden.o, forced=8)
    return parent, (8, 8)


def build_p2() -> bytes:
    parent_raw, move = p2_states()
    child = canonicalize(apply_move(parent_raw, *move))[0]
    prover = _prover_record(parent_raw, move)
    assert terminal(child) == "X" and child == golden_root()
    return build_certificate(
        CLAIM["WIN_X"],
        prover.state,
        {16: [prover], 17: [Record(child, KIND["TERMINAL"], 0xFF)]},
    )


def p3_states():
    state = _mk(
        {
            4: (4,),
            5: (0, 1, 5, 6),
            6: (6, 7, 8),
            7: (6, 7, 8),
            8: (6, 7),
        },
        {
            0: (0, 1),
            1: (3, 4),
            2: (5, 6),
            3: (7, 8),
            5: (2, 3, 4, 7),
        },
        5,
    )
    return state, (5, 8), (8, 8)


def build_p3() -> bytes:
    s0, o_move, x_move = p3_states()
    assert legal_moves(s0) == [o_move] and terminal(s0) is None
    s1 = apply_move(s0, *o_move)
    s2 = apply_move(s1, *x_move)
    assert terminal(s2) == "X"
    opponent = Record(canonicalize(s0)[0], KIND["OPPONENT"], 0xFF)
    prover = _prover_record(s1, x_move)
    terminal_record = Record(canonicalize(s2)[0], KIND["TERMINAL"], 0xFF)
    return build_certificate(
        CLAIM["WIN_X"],
        opponent.state,
        {25: [opponent], 26: [prover], 27: [terminal_record]},
    )
