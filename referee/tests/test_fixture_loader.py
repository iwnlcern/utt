from pathlib import Path

import pytest

from poorman_referee.fixture_loader import iter_fixture_files, load_fixture
from poorman_referee.rules import apply_move, legal_moves


def inline_fixture():
    return {
        "id": "inline-legality",
        "game": "uttt",
        "state": {
            "board": ["........."] * 9,
            "forced": 4,
            "h": "O",
            "budgets": {"x": 17, "o": 23},
        },
        "consumed_by": ["harness"],
        "expected_legal_moves": [[4, cell] for cell in range(9)],
    }


def test_load_fixture_maps_theory_names_to_harness_names_verbatim():
    fixture = inline_fixture()

    position, tie_owner, budgets = load_fixture(fixture)

    assert position.board == tuple(fixture["state"]["board"])
    assert position.forced == 4
    assert tie_owner == "O"
    assert budgets == {"X": 17, "O": 23}


@pytest.mark.parametrize("forced", [True, "4", [4], 9, -1, 1.5])
def test_load_fixture_rejects_unknown_forced_shapes(forced):
    fixture = inline_fixture()
    fixture["state"]["forced"] = forced

    with pytest.raises(ValueError, match="forced"):
        load_fixture(fixture)


def test_iter_fixture_files_reads_envelopes_in_stable_order(tmp_path):
    first = tmp_path / "b.json"
    second = tmp_path / "a.json"
    first.write_text(
        '{"schema_version":1,"game":"uttt","fixtures":[{"id":"b"}]}',
        encoding="utf-8",
    )
    second.write_text(
        '{"schema_version":1,"game":"uttt","fixtures":[{"id":"a"}]}',
        encoding="utf-8",
    )

    assert [fixture["id"] for fixture in iter_fixture_files(tmp_path)] == ["a", "b"]


def test_theory_legality_and_closure_fixtures_run_through_rules():
    root = Path(__file__).parents[2] / "theory" / "fixtures"
    fixture_files = [
        path
        for path in root.glob("*.json")
        if path.name != "schema-v1.json"
    ]
    if not fixture_files:
        pytest.skip("development-only: theory/fixtures have not landed")

    consumed = 0
    for fixture in iter_fixture_files(root):
        if fixture.get("game") != "uttt":
            continue
        position, _, _ = load_fixture(fixture)
        if "expected_legal_moves" in fixture:
            assert legal_moves(position) == [
                tuple(move) for move in fixture["expected_legal_moves"]
            ]
            consumed += 1
        if "move" in fixture:
            expected = {
                "closures": fixture["expected_closures"],
                "forced": fixture["expected_forced"],
                "terminal": fixture["expected_terminal"],
            }
            result = apply_move(position, tuple(fixture["move"]), fixture["move_by"])
            assert [
                {"local": closure.local, "result": closure.result}
                for closure in result.closures
            ] == expected["closures"]
            assert result.position.forced == expected["forced"]
            assert result.terminal == expected["terminal"]
            consumed += 1
    assert consumed > 0
