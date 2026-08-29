from certschema.vectors import (
    MUTANTS,
    build_golden_verdict,
    build_mutant,
    build_p1,
    build_p2,
    build_p3,
    catalogue,
)

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
