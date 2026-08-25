import argparse
import json
from fractions import Fraction as F
from types import SimpleNamespace

import numpy as np
import pytest

from auction_ttt.__main__ import _add_rise_anomalies, _exhaustive_scale, main
from auction_ttt.continuous import solve as solve_continuous
from auction_ttt.crosscheck import (
    check,
    in_band,
    select_spot_targets,
    targeted_report,
)
from auction_ttt.discrete import (
    DRAW,
    OWIN,
    XWIN,
    solve as solve_discrete,
    solve_targeted,
)


def test_in_band_uses_exact_integer_boundary_arithmetic():
    board = "XOXOOXXX."
    q = 10**30 + 3
    threshold = F(1, q)

    assert in_band(board, threshold, 2, q - 2)
    assert not in_band(board, threshold, 2, q - 3)


def test_check_reports_only_masked_nonterminal_positive_total_mismatch():
    N = 4
    board = "XOXOOXXX."
    key = (board, "X")
    values = np.empty((N + 1, N + 1), dtype=np.int8)
    for bx in range(N + 1):
        for bo in range(N + 1):
            values[bx, bo] = XWIN if bx > bo else OWIN if bx < bo else DRAW

    values[4, 0] = OWIN  # masked, out of band, and wrong for X
    values[0, 4] = XWIN  # off-mask disagreement is diagnostic only
    mask = np.zeros_like(values, dtype=bool)
    mask[4, 0] = True
    mask[3, 1] = True  # exactly on the one-empty-cell band boundary
    mask[0, 0] = True  # excluded because M == 0

    terminal = "XXXOO...."
    terminal_values = np.full_like(values, OWIN)
    terminal_mask = np.ones_like(mask)
    report = check(
        {
            key: SimpleNamespace(T=F(1, 2)),
        },
        {
            key: values,
            (terminal, "X"): terminal_values,
        },
        {
            key: mask,
            (terminal, "X"): terminal_mask,
        },
        N,
    )

    assert [mismatch[:4] for mismatch in report.out_of_band_mismatches] == [
        (board, "X", 4, 0)
    ]
    assert report.r_mis == F(2)
    assert report.r_mis_state == (board, "X", 4, 0)
    assert report.max_inband_ratio == F(1)
    assert report.inband_fraction == F(1, 2)
    assert report.excluded_m0_points == 1
    assert report.offmask_diagnostics == 1


def test_small_exact_masked_crosschecks_have_no_out_of_band_mismatches():
    solved_continuous = solve_continuous()

    for N in (2, 4, 8):
        values, masks = solve_discrete(N)
        report = check(solved_continuous, values, masks, N)
        assert report.out_of_band_mismatches == []


def test_crosscheck_cli_emits_json(capsys):
    assert main(["crosscheck", "--scale", "2"]) == 0

    payload = json.loads(capsys.readouterr().out)
    assert payload["scale"] == 2
    assert payload["out_of_band_mismatches"] == []
    assert payload["r_mis"].count("/") == 1


def test_sweep_cli_writes_one_report_per_exhaustive_scale(tmp_path):
    assert (
        main(
            [
                "sweep",
                "--max-scale",
                "2",
                "--output-dir",
                str(tmp_path),
            ]
        )
        == 0
    )

    paths = sorted(tmp_path.glob("crosscheck-N*.json"))
    assert [json.loads(path.read_text())["scale"] for path in paths] == [1, 2]


def test_exhaustive_sweep_cannot_silently_exceed_fixed_scale_cap():
    assert _exhaustive_scale("32") == 32
    with pytest.raises(argparse.ArgumentTypeError, match="at most 32"):
        _exhaustive_scale("33")


def test_cross_scale_rises_are_reported_as_diagnostic_findings():
    payloads = [
        {"scale": 64, "r_mis": "1/2", "inband_fraction": "1/4"},
        {"scale": 128, "r_mis": "2/3", "inband_fraction": "1/3"},
    ]

    _add_rise_anomalies(payloads)

    assert payloads[1]["anomaly_findings"] == [
        "r_mis rose from N=64 to N=128",
        "in-band occupancy rose from N=64 to N=128",
    ]


def test_spot_targets_are_exact_union_in_deterministic_order():
    N = 4
    shape = (N + 1, N + 1)
    zugzwang_key = ("XXOXXOOO.", "O")
    inband_key = ("XXOXXOO..", "X")
    neither_key = ("XXOXXO.O.", "X")
    too_many_empties_key = ("XXO......", "O")
    solved = {
        zugzwang_key: SimpleNamespace(T=F(1, 2), zugzwang=True),
        inband_key: SimpleNamespace(T=F(1, 2), zugzwang=False),
        neither_key: SimpleNamespace(T=F(0), zugzwang=False),
        too_many_empties_key: SimpleNamespace(T=F(1, 2), zugzwang=True),
    }
    masks = {key: np.zeros(shape, dtype=bool) for key in solved}
    masks[zugzwang_key][4, 0] = True
    masks[inband_key][2, 2] = True
    masks[neither_key][4, 0] = True
    masks[too_many_empties_key][2, 2] = True

    assert select_spot_targets(solved, masks, N) == [
        inband_key,
        zugzwang_key,
    ]


def test_targeted_report_uses_every_mask_true_point_from_exact_solver():
    N = 2
    board = "X........"
    key = (board, "O")
    solved = solve_continuous()
    exact = solve_targeted(board, N)
    mask = exact.mask[key]

    report, population, details = targeted_report(solved, [key], N)

    expected_positive_total = int(mask.sum()) - int(mask[0, 0])
    assert report.scale == N
    assert report.out_of_band_mismatches == []
    assert population["masked_positive_total"] == expected_positive_total
    assert population["in_band"] + population["out_of_band"] == expected_positive_total
    assert details == [{"board": board, "h": "O", "population": population}]


def test_targeted_report_hard_fails_empty_spot_set_or_selected_mask():
    solved = solve_continuous()

    with pytest.raises(RuntimeError, match="spot set is empty"):
        targeted_report(solved, [], 2)
    with pytest.raises(RuntimeError, match="empty mask"):
        targeted_report(solved, [("X........", "X")], 2)
