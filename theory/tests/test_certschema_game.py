from certschema.game import ANY, D4, State, canonicalize, transform

GOLDEN_ROOT_HEX = "000000000000000000000000c001c001c00100001000100001018200280000000000000009"


def _precanon_golden() -> State:
    # DD Appendix B construction: X wins locals 0,1,2 via top rows; 8 scattered O marks.
    x = [0] * 9
    o = [0] * 9
    x[0] = x[1] = x[2] = 0b111
    o[3] = (1 << 3) | (1 << 5)
    o[4] = (1 << 1) | (1 << 7)
    o[5] = (1 << 0) | (1 << 8)
    o[6] = 1 << 4
    o[7] = 1 << 4
    return State(x=tuple(x), o=tuple(o), forced=ANY)


def test_d4_tables_are_the_dd_bijections_closed_under_composition():
    perms = set(D4.values())
    assert len(perms) == 8
    for g in perms:
        assert sorted(g) == list(range(9))
    for g in perms:
        for h in perms:
            assert tuple(g[h[i]] for i in range(9)) in perms


def test_serialize_is_37_bytes_le():
    s = _precanon_golden()
    b = s.serialize()
    assert len(b) == 37 and b[36] == 9
    assert b[0:2] == (0b111).to_bytes(2, "little")


def test_canonical_golden_root_matches_dd():
    c, _g = canonicalize(_precanon_golden())
    assert c.serialize().hex() == GOLDEN_ROOT_HEX


def test_canonicalize_idempotent_and_transform_invariants():
    c, _ = canonicalize(_precanon_golden())
    assert canonicalize(c)[0] == c
    for g in D4.values():
        t = transform(c, g)
        assert canonicalize(t)[0] == c
        assert t.side_to_move() == c.side_to_move()


def test_well_formed_rejections_non_vacuous():
    c, _ = canonicalize(_precanon_golden())
    assert c.well_formed()
    b = next(i for i in range(9) if c.x[i])
    bit = c.x[b] & -c.x[b]
    overlap = State(x=c.x, o=c.o[:b] + (c.o[b] | bit,) + c.o[b + 1 :], forced=c.forced)
    assert not overlap.well_formed()
    highbit = State(x=(c.x[0] | 1 << 12,) + c.x[1:], o=c.o, forced=c.forced)
    assert not highbit.well_formed()
    parity = State(x=c.x, o=c.o[:3] + (0,) * 6, forced=c.forced)
    assert not parity.well_formed()
    forcedclosed = State(x=c.x, o=c.o, forced=6)
    assert not forcedclosed.well_formed()
