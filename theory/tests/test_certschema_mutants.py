from certschema.game import State, canonicalize, terminal
from certschema.vectors import (
    MUTANTS,
    _parse_verdict,
    build_golden_verdict,
    build_mutant,
    build_p1,
    build_p2,
    build_p3,
    build_p4,
    catalogue,
    parse_cert,
    rebuild,
)
from certschema.xxh import xxh3_128

EXPECTED_IDS = (
    [f"MUT-H{i:02d}" for i in range(1, 15)]
    + [f"MUT-R{i:02d}" for i in range(1, 23)]
    + [f"MUT-V{i:02d}" for i in range(1, 9)]
)


INTEGRITY_FIELDS = {"manifest.chunk_digest", "manifest.digest"}
FIXED_RECORD_FIELDS = {"chunk.fixed_records"} | INTEGRITY_FIELDS
REMOVED_ROW_FIELDS = {
    "cert.totals",
    "chunk.fixed_count",
    "chunk.fixed_records",
    "chunk.rule_count",
    "chunk.rule_records",
    "manifest.byte_length",
    "manifest.byte_offset",
    "manifest.chunk_digest",
    "manifest.digest",
    "manifest.flags",
    "manifest.ordinal",
    "manifest.ply",
    "manifest.record_count",
    "manifest.row_count",
}
EXPECTED_CHANGED_FIELDS = {
    "MUT-H01": {"cert.magic"},
    "MUT-H02": {"cert.schema_minor"},
    "MUT-H03": {"cert.claim"},
    "MUT-H04": {"cert.reserved"},
    "MUT-H05": {"cert.ruleset_digest"},
    "MUT-H06": {"cert.symmetry_id"},
    "MUT-H07": {"cert.root_state"},
    "MUT-H08": {"cert.rule_ids_used", "manifest.byte_offset", "manifest.digest"},
    "MUT-H09": REMOVED_ROW_FIELDS - {"cert.totals"},
    "MUT-H10": {
        "chunk.fixed_records",
        "manifest.chunk_digest",
        "manifest.digest",
        "manifest.ply",
    },
    "MUT-H11": {"manifest.byte_offset", "manifest.digest"},
    "MUT-H12": {"manifest.digest"},
    "MUT-H13": {"manifest.flags", "manifest.digest"},
    "MUT-H14": {"manifest.ply", "manifest.digest"},
    "MUT-R01": {
        "cert.totals",
        "chunk.fixed_count",
        "chunk.fixed_records",
        "manifest.byte_length",
        "manifest.chunk_digest",
        "manifest.digest",
        "manifest.record_count",
    },
    "MUT-R02": FIXED_RECORD_FIELDS,
    "MUT-R03": FIXED_RECORD_FIELDS,
    "MUT-R04": FIXED_RECORD_FIELDS,
    "MUT-R05": {"manifest.ply", "manifest.digest"},
    "MUT-R06": FIXED_RECORD_FIELDS,
    "MUT-R07": FIXED_RECORD_FIELDS,
    "MUT-R08": FIXED_RECORD_FIELDS,
    "MUT-R09": REMOVED_ROW_FIELDS,
    "MUT-R10": REMOVED_ROW_FIELDS,
    "MUT-R11": {"cert.claim"},
    "MUT-R12": {"cert.totals"} | FIXED_RECORD_FIELDS,
    "MUT-R13": {"cert.totals"} | FIXED_RECORD_FIELDS,
    "MUT-R14": {
        "cert.totals",
        "chunk.rule_count",
        "chunk.rule_records",
        "manifest.byte_length",
        "manifest.chunk_digest",
        "manifest.digest",
        "manifest.record_count",
    },
    "MUT-R15": INTEGRITY_FIELDS,
    "MUT-R16": {"certificate.trailing"},
    "MUT-R17": {
        "cert.totals",
        "chunk.fixed_count",
        "chunk.fixed_records",
        "manifest.byte_length",
        "manifest.chunk_digest",
        "manifest.digest",
    },
    "MUT-R18": {"cert.totals"},
    "MUT-R19": {"cert.root_state"},
    "MUT-R20": {"cert.totals"} | FIXED_RECORD_FIELDS,
    "MUT-R21": {"cert.totals"} | FIXED_RECORD_FIELDS,
    "MUT-R22": {
        "cert.totals",
        "chunk.fixed_count",
        "chunk.fixed_records",
        "manifest.byte_length",
        "manifest.byte_offset",
        "manifest.chunk_digest",
        "manifest.digest",
        "manifest.record_count",
    },
    "MUT-V01": {"verdict.magic"},
    "MUT-V02": {"verdict.member_path"},
    "MUT-V03": {"verdict.member_path"},
    "MUT-V04": {"verdict.member_digest"},
    "MUT-V05": {"verdict.member_claim"},
    "MUT-V06": {"verdict.member_claim"},
    "MUT-V07": {"verdict.root_state"},
    "MUT-V08": {"verdict.game_value"},
}


def test_catalogue_covers_dd_trace_table_exactly():
    assert sorted(MUTANTS) == sorted(EXPECTED_IDS)
    ids = [row["id"] for row in catalogue()["vectors"]]
    for mutant_id in EXPECTED_IDS:
        assert mutant_id in ids
    assert "GATE-01" in ids and "GATE-02" in ids


def test_catalogue_row_count_and_r17_reason_are_pinned():
    rows = catalogue()["vectors"]
    assert len(rows) == 51
    r17 = next(row for row in rows if row["id"] == "MUT-R17")
    assert r17["reason"] == "manifest record_count != fixed_count + rule_count"


BASES = {
    "p1": build_p1,
    "p2": build_p2,
    "p3": build_p3,
    "p4": build_p4,
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
    expected_kind_and_ply = {
        "MUT-R12": (2, 16),
        "MUT-R13": (1, 16),
        "MUT-R21": (1, 17),
    }
    for mutant_id, (kind, ply) in expected_kind_and_ply.items():
        cert = parse_cert(build_mutant(mutant_id))
        changed = [
            record
            for row in cert.rows
            for record in row.chunk.fixed_records
            if record[37] == kind and row.ply == ply
        ]
        assert len(changed) == 1
        assert changed[0][38] == 0xFF
        if mutant_id == "MUT-R21":
            state = _decode_state(changed[0][:37])
            assert state.counts() == (9, 8)  # O to move, so OPPONENT is plausible.
            assert terminal(state) == "X"


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


def _certificate_fields(blob: bytes) -> dict[str, object]:
    cert = parse_cert(blob)
    rows = cert.rows
    declared_end = (
        rows[-1].byte_offset + rows[-1].byte_length if rows else len(blob)
    )
    return {
        "cert.magic": cert.magic,
        "cert.schema_major": cert.schema_major,
        "cert.schema_minor": cert.schema_minor,
        "cert.claim": cert.claim,
        "cert.reserved": cert.reserved,
        "cert.ruleset_id": cert.ruleset_id,
        "cert.ruleset_digest": cert.ruleset_digest,
        "cert.symmetry_id": cert.symmetry_id,
        "cert.root_state": cert.root_state,
        "cert.rule_ids_used": tuple(cert.rule_ids_used),
        "cert.totals": tuple(cert.totals),
        "manifest.row_count": len(rows),
        "manifest.ply": tuple(row.ply for row in rows),
        "manifest.flags": tuple(row.flags for row in rows),
        "manifest.ordinal": tuple(row.ordinal for row in rows),
        "manifest.record_count": tuple(row.record_count for row in rows),
        "manifest.byte_offset": tuple(row.byte_offset for row in rows),
        "manifest.byte_length": tuple(row.byte_length for row in rows),
        "manifest.chunk_digest": tuple(row.chunk_digest for row in rows),
        "manifest.digest": cert.manifest_digest,
        "chunk.fixed_count": tuple(row.chunk.fixed_count for row in rows),
        "chunk.rule_count": tuple(row.chunk.rule_count for row in rows),
        "chunk.fixed_records": tuple(
            tuple(bytes(record) for record in row.chunk.fixed_records) for row in rows
        ),
        "chunk.rule_records": tuple(
            tuple(row.chunk.rule_records) for row in rows
        ),
        "certificate.trailing": blob[declared_end:],
    }


def _verdict_fields(blob: bytes) -> dict[str, object]:
    game_value, root, members = _parse_verdict(blob)
    return {
        "verdict.magic": blob[:8],
        "verdict.schema": blob[8:12],
        "verdict.reserved": blob[13:16],
        "verdict.game_value": game_value,
        "verdict.root_state": root.serialize(),
        "verdict.member_claim": tuple(member[0] for member in members),
        "verdict.member_path": tuple(member[1] for member in members),
        "verdict.member_digest": tuple(member[2] for member in members),
    }


def test_every_mutant_changes_exactly_its_expected_parsed_fields():
    assert set(EXPECTED_CHANGED_FIELDS) == set(EXPECTED_IDS)
    for mutant_id, spec in MUTANTS.items():
        if mutant_id == "MUT-H11":
            continue  # Its deliberately bad offset makes its chunk unparsable.
        fields = _verdict_fields if spec.base == "verdict" else _certificate_fields
        base_fields = fields(BASES[spec.base]())
        mutant_fields = fields(build_mutant(mutant_id))
        changed = {
            name for name, value in base_fields.items() if mutant_fields[name] != value
        }
        assert changed == EXPECTED_CHANGED_FIELDS[mutant_id], mutant_id


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
    base = build_p1()
    cert = parse_cert(base)
    manifest_start = cert.rows[0].byte_offset - 40 * len(cert.rows) - 16
    h09 = build_mutant("MUT-H09")
    assert h09 == base[: manifest_start - 4] + b"\x00\x00\x00\x00"

    base = build_p3()
    cert = parse_cert(base)
    row_start = cert.rows[0].byte_offset - 40 * len(cert.rows) - 16
    h11 = build_mutant("MUT-H11")
    changed = {i for i, (left, right) in enumerate(zip(base, h11)) if left != right}
    expected = {row_start + 8} | set(
        range(row_start + 40 * len(cert.rows), row_start + 40 * len(cert.rows) + 16)
    )
    assert changed == expected
    assert h11[-144:] == base[-144:]
