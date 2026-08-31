module checker::report;

import checker::verify;
import std::encoding::hex;

const String CHECKER_VERSION = "0.1.0";
const String C3C_VERSION = "C3 Compiler Version:       0.8.3";

struct TotalsReport
{
    ulong fixed_prover;
    ulong fixed_opponent;
    ulong terminal;
    ulong rule;
}

struct Report
{
    String verdict;
    String code;
    verify::FailPosition fail_position;
    char claim;
    String root_hex;
    bool root_is_initial;
    String schema_version;
    String ruleset_id;
    String ruleset_digest_hex;
    String symmetry_id;
    TotalsReport totals;
    sz orphans;
    long wall_ms;
    long cpu_ms;
    String checker_version;
    String c3c_version;
}

fn Report from_fields(
    bool ok,
    String code,
    verify::FailPosition fail_position,
    char claim,
    ushort schema_major,
    ushort schema_minor,
    String ruleset_id,
    char[16] ruleset_digest,
    String symmetry_id,
    char[37] root,
    bool root_is_initial,
    ulong[4] totals,
    sz orphans,
    long wall_ms,
    long cpu_ms)
{
    return {
        .verdict = ok ? "pass" : "reject",
        .code = code,
        .fail_position = fail_position,
        .claim = claim,
        .root_hex = hex::tencode(root[..]),
        .root_is_initial = root_is_initial,
        .schema_version = schema_major == 0 && schema_minor == 1 ? "0.1" : "unknown",
        .ruleset_id = ruleset_id,
        .ruleset_digest_hex = hex::tencode(ruleset_digest[..]),
        .symmetry_id = symmetry_id,
        .totals = { totals[0], totals[1], totals[2], totals[3] },
        .orphans = orphans,
        .wall_ms = wall_ms,
        .cpu_ms = cpu_ms,
        .checker_version = CHECKER_VERSION,
        .c3c_version = C3C_VERSION,
    };
}

fn Report from_certificate(verify::VerifyResult result, long wall_ms, long cpu_ms)
{
    return from_fields(
        result.ok,
        result.code,
        result.fail_position,
        result.claim,
        result.schema_major,
        result.schema_minor,
        result.ruleset_id,
        result.ruleset_digest,
        result.symmetry_id,
        result.root,
        result.root_is_initial,
        result.totals,
        result.orphans,
        wall_ms,
        cpu_ms);
}

fn Report from_verdict(verify::VerdictResult result, long wall_ms, long cpu_ms)
{
    verify::VerifyResult certificate = result.certificate;
    char claim = certificate.ruleset_id.len > 0 ? certificate.claim : result.game_value;
    ulong[4] totals = certificate.totals;
    sz orphans = certificate.orphans;
    return from_fields(
        result.ok,
        result.code,
        result.fail_position,
        claim,
        result.schema_major,
        result.schema_minor,
        result.ruleset_id,
        result.ruleset_digest,
        result.symmetry_id,
        result.root,
        result.root_is_initial,
        totals,
        orphans,
        wall_ms,
        cpu_ms);
}
