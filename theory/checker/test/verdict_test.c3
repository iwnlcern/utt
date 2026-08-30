module checker::verdict_test;

import checker::verify;

const String GOLDEN_VERDICT = "../certschema/vectors/golden-winx.utv";

fn void test_golden_verdict_passes_subgame_mode() @test
{
    verify::VerdictResult result = verify::verify_verdict(GOLDEN_VERDICT, verify::Mode.SUBGAME);
    test::@check(result.ok, "code=%s", result.code);
    test::eq("OK", result.code);
    test::@check(!result.root_is_initial, "golden is a non-initial subgame");
}

fn void test_golden_verdict_fails_deliverable_mode_gate02() @test
{
    verify::VerdictResult result = verify::verify_verdict(GOLDEN_VERDICT, verify::Mode.DELIVERABLE);
    test::@check(!result.ok, "deliverable gate must reject");
    test::eq("EV_ROOT_NOT_INITIAL", result.code);
    test::@check(!result.root_is_initial, "reported predicate");
}

fn void test_gate01_initial_root_predicate_is_exact() @test
{
    char[37] initial;
    initial[36] = 4;
    test::@check(verify::is_initial_root(initial), "canonical initial root");

    char[37] any_forced = initial;
    any_forced[36] = 9;
    test::@check(!verify::is_initial_root(any_forced), "initial masks but ANY forced");

    char[37] golden = x'000000000000000000000000c001c001c00100001000100001018200280000000000000009';
    test::@check(!verify::is_initial_root(golden), "golden subgame root");
}
