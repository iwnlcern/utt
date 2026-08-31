module checker::game_test;

import checker::game;

const char[9][8] EXPECTED_D4 = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
    { 2, 5, 8, 1, 4, 7, 0, 3, 6 },
    { 8, 7, 6, 5, 4, 3, 2, 1, 0 },
    { 6, 3, 0, 7, 4, 1, 8, 5, 2 },
    { 2, 1, 0, 5, 4, 3, 8, 7, 6 },
    { 6, 7, 8, 3, 4, 5, 0, 1, 2 },
    { 0, 3, 6, 1, 4, 7, 2, 5, 8 },
    { 8, 5, 2, 7, 4, 1, 6, 3, 0 },
};

fn game::State golden_root()
{
    char[37] bytes = x'000000000000000000000000c001c001c00100001000100001018200280000000000000009';
    return game::deserialize(bytes);
}

fn void test_d4_tables_are_the_locked_destination_tables() @test
{
    for (sz g = 0; g < 8; g++)
    {
        for (sz i = 0; i < 9; i++) test::eq(EXPECTED_D4[g][i], game::D4[g][i]);
    }
}

fn void test_golden_root_is_canonical_and_canonicalization_is_idempotent() @test
{
    game::State root = golden_root();
    char[37] serialized = root.serialize();
    char[37] expected = x'000000000000000000000000c001c001c00100001000100001018200280000000000000009';
    test::eq(expected[..], serialized[..]);
    test::eq(root.serialize()[..], game::canonical(root).serialize()[..]);
    for (sz g = 0; g < 8; g++)
    {
        game::State transformed = game::transform(root, g);
        game::State canonical = game::canonical(transformed);
        test::eq(root.serialize()[..], canonical.serialize()[..]);
        test::eq(canonical.serialize()[..], game::canonical(canonical).serialize()[..]);
    }
}

fn void test_well_formedness_rejects_each_nonvacuous_rule() @test
{
    game::State overlap = golden_root();
    overlap.o[6] |= 1 << 6;
    test::@check(!overlap.well_formed(), "overlap");

    game::State high_bit = golden_root();
    high_bit.x[0] = 1 << 9;
    test::@check(!high_bit.well_formed(), "high mask bit");

    game::State parity;
    parity.forced = game::ANY;
    parity.o[0] = 1;
    test::@check(!parity.well_formed(), "parity");

    game::State forced_closed = golden_root();
    forced_closed.forced = 6;
    test::@check(forced_closed.board_closed(6), "precondition");
    test::@check(!forced_closed.well_formed(), "forced closed");
}
