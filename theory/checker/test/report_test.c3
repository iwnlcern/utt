module checker::report_test;

import checker::report;
import checker::verify;
import std::encoding::json;

fn void test_canned_verify_result_marshals_complete_contract() @test
{
    verify::VerifyResult result = {
        .ok = true,
        .code = "OK",
        .fail_position = { -1, -1, -1 },
        .claim = 0,
        .schema_major = 0,
        .schema_minor = 1,
        .ruleset_id = "uttt-s2-r1",
        .symmetry_id = "d4-lexmin-v1",
        .root_is_initial = false,
        .totals = { 1, 2, 3, 4 },
        .orphans = 5,
    };
    result.ruleset_digest = x'16e85b36332493470fa1006516128579';

    report::Report rendered = report::from_certificate(result, 7, 3);
    String encoded = json::marshal(tmem, rendered);

    test::eq("pass", rendered.verdict);
    test::eq("0.1", rendered.schema_version);
    test::eq("16e85b36332493470fa1006516128579", rendered.ruleset_digest_hex);
    test::eq((ulong)1, rendered.totals.fixed_prover);
    test::eq((ulong)2, rendered.totals.fixed_opponent);
    test::eq((ulong)3, rendered.totals.terminal);
    test::eq((ulong)4, rendered.totals.rule);
    test::eq((sz)5, rendered.orphans);
    test::eq((long)7, rendered.wall_ms);
    test::eq((long)3, rendered.cpu_ms);
    test::@check(encoded.contains("\"checker_version\":\"0.1.0\""), "%s", encoded);
    test::@check(encoded.contains("\"c3c_version\":\"C3 Compiler Version:       0.8.3\""), "%s", encoded);
}
