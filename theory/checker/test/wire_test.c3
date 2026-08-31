module checker::wire_test;

import checker::verify;
import std::collections::map;
import std::io::file;

fn char[] load_vector(String name)
{
    switch (name)
    {
        case "P1": return file::load_temp("../certschema/vectors/golden-terminal-winx.utc")!!;
        case "P2": return file::load_temp("../certschema/vectors/p2-prover-winx.utc")!!;
        case "P3": return file::load_temp("../certschema/vectors/p3-opponent-winx.utc")!!;
        case "P4": return file::load_temp("../certschema/vectors/p4-opponent2-winx.utc")!!;
        default: unreachable();
    }
}

fn void test_golden_certificate_verified_result_fields() @test
{
    verify::VerifyResult result = verify::verify_certificate(load_vector("P1"));
    test::@check(result.ok, "code=%s", result.code);
    test::eq("OK", result.code);
    test::eq(-1, result.fail_position.byte_offset);
    test::eq(-1, result.fail_position.chunk);
    test::eq(-1, result.fail_position.record);
    test::eq((ulong)0, result.totals[0]);
    test::eq((ulong)0, result.totals[1]);
    test::eq((ulong)1, result.totals[2]);
    test::eq((ulong)0, result.totals[3]);
    test::eq((sz)0, result.orphans);
    test::@check(!result.root_is_initial, "golden is a subgame root");

    char[16] ruleset_digest = x'16e85b36332493470fa1006516128579';
    char[16] header_digest = x'285856ee77314b80f2440452c97463bd';
    test::eq(ruleset_digest[..], result.ruleset_digest[..]);
    test::eq(header_digest[..], result.header_digest[..]);
}

fn void test_all_positive_certificate_shapes_verify_without_orphans() @test
{
    String[3] names = { "P2", "P3", "P4" };
    foreach (name : names)
    {
        verify::VerifyResult result = verify::verify_certificate(load_vector(name));
        test::@check(result.ok, "%s: %s", name, result.code);
        test::eq((sz)0, result.orphans);
    }
}

fn void test_truncated_chunk_is_layout_failure_with_header_position() @test
{
    char[] golden = load_vector("P1");
    verify::VerifyResult result = verify::verify_certificate(golden[:200]);
    test::@check(!result.ok, "truncation must reject");
    test::eq("E_CHUNK_LAYOUT", result.code);
    test::@check(result.fail_position.byte_offset >= 0, "layout byte position");
    test::eq(-1, result.fail_position.chunk);
    test::eq(-1, result.fail_position.record);
}

fn void test_string_keyed_hashmap_uses_byte_content_equality() @test
{
    HashMap { String, bool } set;
    set.tinit();
    char[37] first;
    char[37] second;
    first[36] = 4;
    second[0] = 1;
    second[36] = 9;
    set.set(((String)first[..]).copy(tmem), true);
    set.set(((String)second[..]).copy(tmem), true);

    char[37] first_probe = first;
    char[37] second_probe = second;
    test::@check(set.has_key((String)first_probe[..]), "first content key");
    test::@check(set.has_key((String)second_probe[..]), "second content key");
    test::eq(2, set.len());
}
