import json
import pathlib

import pytest

from certschema.game import (
    ANY,
    State,
    apply_for,
    apply_move,
    board_result,
    legal_moves,
    terminal,
)

FIX = pathlib.Path(__file__).resolve().parents[1] / "fixtures"

COMPATIBLE_LEGALITY = (
    "legality-forced-center-opening",
    "legality-free-two-open-boards",
    "legality-free-single-open-board",
)
EXCLUDED_LEGALITY = ("legality-forced-closed-means-free-choice",)
COMPATIBLE_TERMINAL = {
    "terminal-x-macro-win-outright": "X",
    "terminal-all-closed-chip-margin-zero": "draw",
}
EXCLUDED_TERMINAL = (
    "terminal-all-closed-chip-margin-plus-one",
    "terminal-all-closed-chip-margin-minus-one",
    "terminal-final-payment-before-chip-comparison",
)


def _state(st: dict) -> State:
    x = [0] * 9
    o = [0] * 9
    for b, row in enumerate(st["board"]):
        for c, ch in enumerate(row):
            if ch == "X":
                x[b] |= 1 << c
            elif ch == "O":
                o[b] |= 1 << c
    return State(
        x=tuple(x),
        o=tuple(o),
        forced=st["forced"] if st["forced"] is not None else ANY,
    )


def _rows(name: str) -> list:
    rows = json.loads((FIX / name).read_text())["fixtures"]
    assert all(r.get("consumed_by") for r in rows)
    return [r for r in rows if "theory" in r["consumed_by"]]


def _by_id(name: str, fid: str) -> dict:
    return next(r for r in _rows(name) if r["id"] == fid)


def test_matrix_covers_every_consumable_fixture_exactly_once():
    assert sorted(r["id"] for r in _rows("legality.json")) == sorted(
        COMPATIBLE_LEGALITY + EXCLUDED_LEGALITY
    )
    assert sorted(r["id"] for r in _rows("terminal.json")) == sorted(
        list(COMPATIBLE_TERMINAL) + list(EXCLUDED_TERMINAL)
    )
    assert len(_rows("closure_routing.json")) == 6


@pytest.mark.parametrize("fid", EXCLUDED_LEGALITY)
def test_excluded_legality_rows_are_ill_formed_under_s2(fid):
    s = _state(_by_id("legality.json", fid)["state"])
    assert s.board_closed(s.forced)
    assert not s.well_formed()


@pytest.mark.parametrize("fid", COMPATIBLE_LEGALITY)
def test_legality_fixtures(fid):
    fx = _by_id("legality.json", fid)
    got = sorted(legal_moves(_state(fx["state"])))
    assert got == sorted(tuple(m) for m in fx["expected_legal_moves"])


@pytest.mark.parametrize("fx", _rows("closure_routing.json"), ids=lambda f: f["id"])
def test_closure_routing_fixtures(fx):
    s = _state(fx["state"])
    child = apply_for(s, fx["move"][0], fx["move"][1], fx["move_by"])
    expected_forced = fx["expected_forced"]
    assert child.forced == (expected_forced if expected_forced is not None else ANY)
    expected_terminal = fx["expected_terminal"]
    assert (
        (terminal(child) is None)
        if expected_terminal is None
        else (expected_terminal == "macro_win" and terminal(child) == fx["move_by"])
    )
    for closure in fx["expected_closures"]:
        assert board_result(child, closure["local"]) == closure["result"]


@pytest.mark.parametrize("fid,expected", sorted(COMPATIBLE_TERMINAL.items()))
def test_terminal_fixtures(fid, expected):
    assert terminal(_state(_by_id("terminal.json", fid)["state"])) == expected


@pytest.mark.parametrize("fid", EXCLUDED_TERMINAL)
def test_excluded_terminal_rows_are_draws_under_s2_rule_lock(fid):
    assert terminal(_state(_by_id("terminal.json", fid)["state"])) == "draw"


def test_send_to_closed_yields_any_directly():
    for fx in _rows("closure_routing.json"):
        child = apply_for(
            _state(fx["state"]), fx["move"][0], fx["move"][1], fx["move_by"]
        )
        if child.forced != ANY:
            assert not child.board_closed(child.forced)
