from dataclasses import replace
from fractions import Fraction as F

from auction_ttt import rules as R
from auction_ttt.__main__ import main
from auction_ttt.continuous import kalai_report, root_pair, solve


EMPTY = "." * 9


def test_root_pair_returns_sorted_fraction_thresholds():
    thresholds = root_pair(solve())

    assert thresholds == (F(1, 2), F(1, 2))
    assert all(isinstance(threshold, F) for threshold in thresholds)
    assert thresholds[0] <= thresholds[1]


def test_center_forced_recomputes_only_the_root_backup():
    solved = solve()
    stale_root = {
        **solved,
        (EMPTY, R.X): replace(solved[(EMPTY, R.X)], T=F(0)),
        (EMPTY, R.O): replace(solved[(EMPTY, R.O)], T=F(1)),
    }
    noncenter_x_child = (R.apply_move(EMPTY, 0, R.X), R.O)
    changed_children = {
        **stale_root,
        noncenter_x_child: replace(stale_root[noncenter_x_child], T=F(0)),
    }

    assert root_pair(stale_root, center_forced=True) == (F(1, 2), F(1, 2))
    assert root_pair(changed_children, center_forced=True) == (F(1, 2), F(1, 2))
    assert root_pair(changed_children) == (F(19367, 52451), F(19367, 52451))


def test_kalai_report_exposes_ratios_without_a_target_agreement_claim():
    report = kalai_report(solve())

    assert report == {
        "T_lo": F(1, 2),
        "T_hi": F(1, 2),
        "ratio_lo": F(1),
        "ratio_hi": F(1),
    }


def test_cli_skeleton_offers_solve_and_kalai_commands(capsys):
    assert main(["solve"]) == 0
    solve_output = capsys.readouterr().out
    assert "T_lo" in solve_output and "T_hi" in solve_output

    assert main(["kalai"]) == 0
    kalai_output = capsys.readouterr().out
    assert "ratio_lo" in kalai_output and "ratio_hi" in kalai_output
    assert "101.84:100" in kalai_output
