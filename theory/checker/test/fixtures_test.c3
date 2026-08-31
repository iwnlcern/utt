module checker::fixtures_test;

import checker::game;
import std::collections::object;
import std::encoding::json;
import std::io::file;

const String[3] COMPATIBLE_LEGALITY = {
    "legality-forced-center-opening",
    "legality-free-two-open-boards",
    "legality-free-single-open-board",
};
const String[1] EXCLUDED_LEGALITY = { "legality-forced-closed-means-free-choice" };
const String[2] COMPATIBLE_TERMINAL = {
    "terminal-x-macro-win-outright",
    "terminal-all-closed-chip-margin-zero",
};
const String[3] EXCLUDED_TERMINAL = {
    "terminal-all-closed-chip-margin-plus-one",
    "terminal-all-closed-chip-margin-minus-one",
    "terminal-final-payment-before-chip-comparison",
};

fn Object* fixture_rows(String name)
{
    String path;
    switch (name)
    {
        case "legality.json": path = "../fixtures/legality.json";
        case "closure_routing.json": path = "../fixtures/closure_routing.json";
        case "terminal.json": path = "../fixtures/terminal.json";
        default: unreachable();
    }
    char[] bytes = file::load_temp(path)!!;
    Object* root = json::tparse((String)bytes)!!;
    return root.get("fixtures")!!;
}

fn bool consumes_theory(Object* fixture)
{
    Object* consumers = fixture.get("consumed_by")!!;
    test::@check(consumers.get_len() > 0, "missing consumed_by");
    for (sz i = 0; i < consumers.get_len(); i++)
    {
        if (consumers.get_string_at(i)!! == "theory") return true;
    }
    return false;
}

fn Object* fixture_by_id(String name, String id)
{
    Object* rows = fixture_rows(name);
    for (sz i = 0; i < rows.get_len(); i++)
    {
        Object* fixture = rows.get_at(i);
        if (consumes_theory(fixture) && fixture.get_string("id")!! == id) return fixture;
    }
    unreachable();
}

fn bool contains(String id, String[] ids)
{
    foreach (candidate : ids) if (candidate == id) return true;
    return false;
}

fn game::State state_from(Object* fixture)
{
    Object* state = fixture.get("state")!!;
    Object* board = state.get("board")!!;
    game::State result;
    for (sz b = 0; b < 9; b++)
    {
        String row = board.get_string_at(b)!!;
        test::eq(9, row.len);
        for (sz c = 0; c < 9; c++)
        {
            if (row[c] == 'X') result.x[b] |= (ushort)(1 << c);
            if (row[c] == 'O') result.o[b] |= (ushort)(1 << c);
        }
    }
    Object* forced = state.get("forced")!!;
    result.forced = forced.is_null() ? game::ANY : (char)state.get_int("forced")!!;
    return result;
}

fn void test_matrix_covers_every_theory_fixture_exactly_once() @test
{
    Object* legality = fixture_rows("legality.json");
    sz legality_count;
    for (sz i = 0; i < legality.get_len(); i++)
    {
        Object* fixture = legality.get_at(i);
        if (!consumes_theory(fixture)) continue;
        legality_count++;
        String id = fixture.get_string("id")!!;
        int classified = (int)contains(id, COMPATIBLE_LEGALITY[..]) + (int)contains(id, EXCLUDED_LEGALITY[..]);
        test::eq(1, classified);
    }
    test::eq(4, legality_count);

    Object* terminal = fixture_rows("terminal.json");
    sz terminal_count;
    for (sz i = 0; i < terminal.get_len(); i++)
    {
        Object* fixture = terminal.get_at(i);
        if (!consumes_theory(fixture)) continue;
        terminal_count++;
        String id = fixture.get_string("id")!!;
        int classified = (int)contains(id, COMPATIBLE_TERMINAL[..]) + (int)contains(id, EXCLUDED_TERMINAL[..]);
        test::eq(1, classified);
    }
    test::eq(5, terminal_count);

    Object* closure = fixture_rows("closure_routing.json");
    sz closure_count;
    for (sz i = 0; i < closure.get_len(); i++) if (consumes_theory(closure.get_at(i))) closure_count++;
    test::eq(6, closure_count);
}

fn void test_excluded_legality_is_forced_at_closed() @test
{
    Object* fixture = fixture_by_id("legality.json", EXCLUDED_LEGALITY[0]);
    game::State state = state_from(fixture);
    test::@check(state.board_closed(state.forced), "documented exclusion");
    test::@check(!state.well_formed(), "DD 2.6 rejection");
}

fn void test_compatible_legality_rows() @test
{
    foreach (id : COMPATIBLE_LEGALITY)
    {
        Object* fixture = fixture_by_id("legality.json", id);
        game::MoveList moves = game::legal_moves(state_from(fixture));
        Object* expected = fixture.get("expected_legal_moves")!!;
        test::eq(expected.get_len(), moves.len);
        for (sz i = 0; i < moves.len; i++)
        {
            Object* pair = expected.get_at(i);
            test::eq(pair.get_int_at(0)!!, (int)moves.entries[i].board);
            test::eq(pair.get_int_at(1)!!, (int)moves.entries[i].cell);
        }
    }
}

fn game::BoardKind expected_board_kind(String name)
{
    switch (name)
    {
        case "X": return game::BoardKind.X_WIN;
        case "O": return game::BoardKind.O_WIN;
        case "full": return game::BoardKind.FULL;
        default: unreachable();
    }
}

fn void test_all_six_closure_routing_rows() @test
{
    Object* rows = fixture_rows("closure_routing.json");
    for (sz i = 0; i < rows.get_len(); i++)
    {
        Object* fixture = rows.get_at(i);
        if (!consumes_theory(fixture)) continue;
        Object* move = fixture.get("move")!!;
        String side = fixture.get_string("move_by")!!;
        game::State child = game::apply(
            state_from(fixture),
            (sz)move.get_int_at(0)!!,
            (sz)move.get_int_at(1)!!,
            side[0],
        )!!;

        Object* expected_forced = fixture.get("expected_forced")!!;
        char forced = expected_forced.is_null()
            ? game::ANY
            : (char)fixture.get_int("expected_forced")!!;
        test::eq(forced, child.forced);

        Object* expected_terminal = fixture.get("expected_terminal")!!;
        game::TerminalKind result = game::terminal(child);
        if (expected_terminal.is_null()) { test::eq(game::TerminalKind.NONE, result); }
        else
        {
            test::eq("macro_win", fixture.get_string("expected_terminal")!!);
            test::eq(side[0] == 'X' ? game::TerminalKind.X_WIN : game::TerminalKind.O_WIN, result);
        }

        Object* closures = fixture.get("expected_closures")!!;
        for (sz j = 0; j < closures.get_len(); j++)
        {
            Object* closure = closures.get_at(j);
            sz local = (sz)closure.get_int("local")!!;
            String expected = closure.get_string("result")!!;
            test::eq(expected_board_kind(expected), game::board_result(child, local));
        }
        if (child.forced != game::ANY) test::@check(!child.board_closed(child.forced), "reachable forced board is open");
    }
}

fn void test_compatible_terminal_rows() @test
{
    foreach (id : COMPATIBLE_TERMINAL)
    {
        game::TerminalKind expected = id == "terminal-x-macro-win-outright"
            ? game::TerminalKind.X_WIN
            : game::TerminalKind.DRAW;
        test::eq(expected, game::terminal(state_from(fixture_by_id("terminal.json", id))));
    }
}

fn void test_auction_only_terminal_rows_are_s2_draws() @test
{
    foreach (id : EXCLUDED_TERMINAL)
    {
        test::eq(game::TerminalKind.DRAW, game::terminal(state_from(fixture_by_id("terminal.json", id))));
    }
}
