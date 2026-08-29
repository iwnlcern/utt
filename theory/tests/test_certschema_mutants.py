from certschema.game import State, canonicalize
from certschema.vectors import (
    MUTANTS,
    build_golden_verdict,
    build_mutant,
    build_p1,
    build_p2,
    build_p3,
    catalogue,
    parse_cert,
    rebuild,
)
from certschema.xxh import xxh3_128

EXPECTED_IDS = (
    [f"MUT-H{i:02d}" for i in range(1, 15)]
    + [f"MUT-R{i:02d}" for i in range(1, 21)]
    + [f"MUT-V{i:02d}" for i in range(1, 9)]
)


def test_catalogue_covers_dd_trace_table_exactly():
    assert sorted(MUTANTS) == sorted(EXPECTED_IDS)
    ids = [row["id"] for row in catalogue()["vectors"]]
    for mutant_id in EXPECTED_IDS:
        assert mutant_id in ids
    assert "GATE-01" in ids and "GATE-02" in ids


BASES = {
    "p1": build_p1,
    "p2": build_p2,
    "p3": build_p3,
    "verdict": build_golden_verdict,
}


def test_every_mutant_differs_from_its_base_and_is_deterministic():
    for mutant_id, spec in MUTANTS.items():
        base = BASES[spec.base]()
        first, second = build_mutant(mutant_id), build_mutant(mutant_id)
        assert first == second, mutant_id
        assert first != base, mutant_id


def test_single_field_mutants_change_minimal_bytes():
    for mutant_id in ("MUT-H01", "MUT-V01"):
        base = BASES[MUTANTS[mutant_id].base]()
        mutant = build_mutant(mutant_id)
        assert len(mutant) == len(base)
        assert sum(left != right for left, right in zip(mutant, base)) == 1


def test_kind_mutants_normalize_nonprover_payload_before_target_check():
    expected_kind = {"MUT-R12": 2, "MUT-R13": 1}
    for mutant_id, kind in expected_kind.items():
        cert = parse_cert(build_mutant(mutant_id))
        changed = [
            record
            for row in cert.rows
            for record in row.chunk.fixed_records
            if record[37] == kind and row.ply == 16
        ]
        assert len(changed) == 1
        assert changed[0][38] == 0xFF


def test_r17_count_mismatch_has_a_parseable_complete_chunk_body():
    cert = parse_cert(build_mutant("MUT-R17"))
    row = cert.rows[0]
    assert row.record_count == 1
    assert row.chunk.fixed_count == 2 and row.chunk.rule_count == 0
    assert len(row.chunk.fixed_records) == 2


def _decode_state(blob: bytes) -> State:
    return State(
        x=tuple(int.from_bytes(blob[i : i + 2], "little") for i in range(0, 18, 2)),
        o=tuple(int.from_bytes(blob[i : i + 2], "little") for i in range(18, 36, 2)),
        forced=blob[36],
    )


def _row_bytes(row) -> bytes:
    return (
        bytes([row.ply, row.flags])
        + row.ordinal.to_bytes(2, "little")
        + row.record_count.to_bytes(4, "little")
        + row.byte_offset.to_bytes(8, "little")
        + row.byte_length.to_bytes(8, "little")
        + row.chunk_digest
    )


def _certificate_mutant_ids():
    return [mid for mid, spec in MUTANTS.items() if spec.base != "verdict"]


def test_certificate_mutants_preserve_non_target_integrity_and_counters():
    exceptional_framing = {"MUT-H09", "MUT-H11"}
    for mutant_id in _certificate_mutant_ids():
        blob = build_mutant(mutant_id)
        if mutant_id in exceptional_framing:
            continue
        cert = parse_cert(blob)
        rows_blob = b"".join(_row_bytes(row) for row in cert.rows)
        if mutant_id != "MUT-H12":
            assert cert.manifest_digest == xxh3_128(rows_blob), mutant_id
        for row in cert.rows:
            assert row.byte_length == len(row.chunk.encode()), mutant_id
            if mutant_id != "MUT-R15":
                assert row.chunk_digest == xxh3_128(row.chunk.encode()), mutant_id
            if mutant_id != "MUT-R17":
                assert row.record_count == row.chunk.fixed_count + row.chunk.rule_count, mutant_id
        actual_totals = [0, 0, 0, 0]
        for row in cert.rows:
            for record in row.chunk.fixed_records:
                actual_totals[record[37]] += 1
            actual_totals[3] += len(row.chunk.rule_records)
        if mutant_id != "MUT-R18":
            assert cert.totals == actual_totals, mutant_id
        if mutant_id != "MUT-R16":
            assert cert.rows[-1].byte_offset + cert.rows[-1].byte_length == len(blob), mutant_id
        else:
            assert blob.endswith(bytes.fromhex("deadbeef"))
            assert cert.rows[-1].byte_offset + cert.rows[-1].byte_length == len(blob) - 4
        if mutant_id != "MUT-H11":
            for left, right in zip(cert.rows, cert.rows[1:]):
                assert left.byte_offset + left.byte_length == right.byte_offset, mutant_id


def test_certificate_mutants_preserve_non_target_state_and_record_fields():
    for mutant_id in _certificate_mutant_ids():
        if mutant_id in {"MUT-H09", "MUT-H11"}:
            continue
        cert = parse_cert(build_mutant(mutant_id))
        root = _decode_state(cert.root_state)
        if mutant_id != "MUT-H07":
            assert root.well_formed() and canonicalize(root)[0] == root, mutant_id
        for row in cert.rows:
            records = list(row.chunk.fixed_records) + list(row.chunk.rule_records)
            for record in records:
                state = _decode_state(record[:37])
                if mutant_id not in {"MUT-R02", "MUT-R03", "MUT-R04"}:
                    assert state.well_formed() and canonicalize(state)[0] == state, mutant_id
            for record in row.chunk.fixed_records:
                if mutant_id != "MUT-R06":
                    assert record[39] == 0, mutant_id
                if record[37] != 0 and mutant_id != "MUT-R07":
                    assert record[38] == 0xFF, mutant_id


def test_header_offset_and_truncation_targets_are_isolated():
    h09 = build_mutant("MUT-H09")
    assert h09[-4:] == b"\x00\x00\x00\x00"

    base = build_p3()
    h11 = build_mutant("MUT-H11")
    changed = [i for i, (left, right) in enumerate(zip(base, h11)) if left != right]
    # One offset byte changes; the row digest binding then changes as expected.
    assert changed[0] < changed[-1]
    assert h11[-144:] == base[-144:]
