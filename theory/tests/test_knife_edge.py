import gc
import json
import weakref
from fractions import Fraction as F
from types import SimpleNamespace

import numpy as np
import pytest

from auction_ttt.__main__ import main
from auction_ttt.discrete import DRAW, OWIN, XWIN
from auction_ttt.knife_edge import report, spot_sweep, sweep


def test_equality_enumeration_is_exact():
    # A wrong equality predicate would admit a non-diagonal point for T = 1/2.
    half = [point for point in sweep(4) if point.T == F(1, 2)]

    assert half
    assert all(point.bx == point.bo for point in half)


def test_sweep_deterministic():
    # Unsorted state or coordinate traversal would change this sequence.
    first = sweep(3)
    second = sweep(3)

    assert [
        (point.board, point.h, point.bx, point.bo) for point in first
    ] == [
        (point.board, point.h, point.bx, point.bo) for point in second
    ]


def test_agreement_field_definition():
    # P1b predicts that the tie owner wins exactly at representable equality.
    for point in sweep(4):
        assert point.convention == point.h
        assert point.agrees == (point.discrete == point.h)


def test_spot_population_binding(monkeypatch):
    # Dropping a selected key, widening to an unselected owner, retaining M=0,
    # or solving a shared board twice changes these literal population counts.
    board_a = "O........"
    board_b = "X........"
    targets = [(board_b, "O"), (board_a, "X"), (board_b, "X")]
    solved = {
        key: SimpleNamespace(T=F(1, 2), a=F(0), b=F(1), zugzwang=False)
        for key in targets
    }
    selector_masks = object()
    selector_calls = []
    solve_calls = []

    monkeypatch.setattr("auction_ttt.knife_edge.solve_continuous", lambda: solved)

    def fake_solve_discrete(scale):
        assert scale == 32
        return {}, selector_masks

    def fake_selector(got_solved, got_masks, scale):
        selector_calls.append((got_solved, got_masks, scale))
        return targets

    def fake_solve_targeted(board, scale):
        solve_calls.append((scale, board))
        values = {}
        masks = {}
        for h in ("X", "O"):
            value = np.full((scale + 1, scale + 1), OWIN, dtype=np.int8)
            mask = np.zeros((scale + 1, scale + 1), dtype=bool)
            mask[0, 0] = True  # excluded: M == 0
            mask[1, 0] = True  # excluded: not p == T
            mask[1, 1] = True  # retained: exact equality
            value[1, 1] = XWIN if h == "X" else DRAW
            values[(board, h)] = value
            masks[(board, h)] = mask
        return SimpleNamespace(values=values, mask=masks, diagnostics=[])

    monkeypatch.setattr("auction_ttt.knife_edge.solve_discrete", fake_solve_discrete)
    monkeypatch.setattr("auction_ttt.knife_edge.select_spot_targets", fake_selector)
    monkeypatch.setattr("auction_ttt.knife_edge.solve_targeted", fake_solve_targeted)

    payload = report(scales=[], spot_scales=[5, 6])

    assert selector_calls == [(solved, selector_masks, 32)]
    assert solve_calls == [
        (5, board_a),
        (5, board_b),
        (6, board_a),
        (6, board_b),
    ]
    assert payload["methodology"]["spot_target_count"] == 3
    assert payload["per_scale"] == [
        {
            "scale": 5,
            "mode": "targeted",
            "target_count": 3,
            "equality_points": 3,
            "agreements": 2,
            "disagreements": 1,
        },
        {
            "scale": 6,
            "mode": "targeted",
            "target_count": 3,
            "equality_points": 3,
            "agreements": 2,
            "disagreements": 1,
        },
    ]


@pytest.mark.parametrize("missing", [False, True])
def test_spot_hard_fails_on_missing_mask(monkeypatch, missing):
    # Missing and empty selected masks must fail instead of silently sampling.
    board = "X........"
    key = (board, "O")
    solved = {key: SimpleNamespace(T=F(1, 2), a=F(0), b=F(1))}
    mask = np.zeros((3, 3), dtype=bool)
    masks = {} if missing else {key: mask}
    values = {key: np.full((3, 3), DRAW, dtype=np.int8)}

    monkeypatch.setattr("auction_ttt.knife_edge.solve_continuous", lambda: solved)
    monkeypatch.setattr(
        "auction_ttt.knife_edge.solve_targeted",
        lambda _board, _scale: SimpleNamespace(
            values=values, mask=masks, diagnostics=[]
        ),
    )

    with pytest.raises(RuntimeError, match="missing mask|empty mask"):
        spot_sweep(2, [key], lambda _point: None)


def test_report_schema_and_conservation():
    # Lost or duplicated points break one of the independent conservation sums.
    payload = report(scales=[2, 3], spot_scales=[])

    json.dumps(payload)
    assert set(payload) == {
        "methodology",
        "per_scale",
        "groups",
        "disagreement_groups",
        "stream_digests",
    }
    for record in payload["per_scale"]:
        scale = record["scale"]
        assert record["equality_points"] == (
            record["agreements"] + record["disagreements"]
        )
        assert sum(
            group["count"]
            for group in payload["groups"]
            if group["scale"] == scale
        ) == record["equality_points"]
        assert sum(
            group["count"]
            for group in payload["disagreement_groups"]
            if group["scale"] == scale
        ) == record["disagreements"]
    assert payload["disagreement_groups"] == [
        group
        for group in payload["groups"]
        if not group["agrees"] and group["count"] > 0
    ]
    assert payload["methodology"]["no_sampling"] is True


def test_spot_streaming_releases_boards(monkeypatch):
    # Retaining a result or grid across iterations violates the bounded-memory path.
    targets = [("X........", "O"), ("O........", "X")]
    solved = {
        key: SimpleNamespace(T=F(1, 2), a=F(0), b=F(1)) for key in targets
    }
    previous_refs = []
    calls = []

    class TrackedResult:
        pass

    def tracked_solve_targeted(board, scale):
        gc.collect()
        assert not any(ref() is not None for ref in previous_refs), (
            "previous targeted board must be released before the next solve"
        )
        calls.append(board)
        result = TrackedResult()
        result.values = {}
        result.mask = {}
        arrays = []
        for h in ("X", "O"):
            values = np.full((scale + 1, scale + 1), DRAW, dtype=np.int8)
            mask = np.zeros((scale + 1, scale + 1), dtype=bool)
            mask[scale, scale] = True
            result.values[(board, h)] = values
            result.mask[(board, h)] = mask
            arrays.extend((values, mask))
        previous_refs[:] = [weakref.ref(result), *(weakref.ref(a) for a in arrays)]
        return result

    monkeypatch.setattr("auction_ttt.knife_edge.solve_continuous", lambda: solved)
    monkeypatch.setattr(
        "auction_ttt.knife_edge.solve_targeted", tracked_solve_targeted
    )
    folded = []

    spot_sweep(
        2,
        targets,
        lambda point: folded.append((point.board, point.h, point.bx, point.bo)),
    )

    assert calls == ["O........", "X........"]
    assert folded == [("O........", "X", 2, 2), ("X........", "O", 2, 2)]


def test_report_deterministic_bytes():
    # Nondeterministic traversal or group ordering changes serialized bytes.
    first = json.dumps(report(scales=[2], spot_scales=[]), sort_keys=True)
    second = json.dumps(report(scales=[2], spot_scales=[]), sort_keys=True)

    assert first == second


def test_disagreement_buckets_distinct(monkeypatch):
    # Merging draw and opponent wins would erase the P1c interpretation boundary.
    board = "X........"
    key = (board, "X")
    solved = {key: SimpleNamespace(T=F(1, 2), a=F(0), b=F(1))}
    values = np.full((3, 3), XWIN, dtype=np.int8)
    masks = np.zeros((3, 3), dtype=bool)
    masks[1, 1] = True
    masks[2, 2] = True
    values[1, 1] = DRAW
    values[2, 2] = OWIN

    monkeypatch.setattr("auction_ttt.knife_edge.solve_continuous", lambda: solved)
    monkeypatch.setattr(
        "auction_ttt.knife_edge.solve_discrete",
        lambda _scale: ({key: values}, {key: masks}),
    )

    payload = report(scales=[2], spot_scales=[])

    assert {
        group["discrete"] for group in payload["disagreement_groups"]
    } == {"draw", "O"}


def test_cli_knife_edge_writes_contract(tmp_path):
    # A missing schema field or unstable serializer breaks the committed artifact contract.
    output = tmp_path / "knife-edge.json"
    argv = ["knife-edge", "--max-scale", "2", "--out", str(output)]

    assert main(argv) == 0
    first = output.read_bytes()
    payload = json.loads(first)
    assert set(payload) == {
        "methodology",
        "per_scale",
        "groups",
        "disagreement_groups",
        "stream_digests",
    }
    assert [record["scale"] for record in payload["per_scale"]] == [1, 2]

    assert main(argv) == 0
    assert output.read_bytes() == first
