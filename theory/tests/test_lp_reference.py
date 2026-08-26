from fractions import Fraction as F

import auction_ttt.lp_reference as LP
from auction_ttt.continuous import solve as solve_continuous
from auction_ttt.discrete import solve as solve_discrete
from auction_ttt.lp_reference import emit_root_matrices, solve_zero_sum


def test_matching_pennies():
    result = solve_zero_sum([[F(1), F(-1)], [F(-1), F(1)]])

    assert result.value == F(0)
    assert result.x == [F(1, 2), F(1, 2)]
    assert result.y == [F(1, 2), F(1, 2)]
    assert result.path == "direct"
    assert result.certificate == "exact-verified"


def test_saddle_point():
    result = solve_zero_sum([[F(2), F(1)], [F(0), F(-1)]])

    assert result.value == F(1)
    assert result.x == [F(1), F(0)]
    assert result.y == [F(0), F(1)]


def test_forced_bad_support_exercises_ladder():
    matrix = [[F(1), F(-1)], [F(-1), F(1)]]
    bad_hook = lambda _x_float, _y_float: ([0], [0])

    result = solve_zero_sum(matrix, _support_hook=bad_hook)

    assert result.path in ("perturbed", "fallback")
    assert result.value == F(0)
    assert result.certificate in (
        "exact-verified",
        "exact-fallback-verified",
    )


def test_exact_rational_fallback_is_independently_executable(monkeypatch):
    def fail_float_stage(*_args, **_kwargs):
        raise LP._CandidateFailure("forced float-stage failure")

    monkeypatch.setattr(LP, "_float_strategies", fail_float_stage)

    result = solve_zero_sum([[F(1), F(-1)], [F(-1), F(1)]])

    assert result.path == "fallback"
    assert result.certificate == "exact-fallback-verified"
    assert result.value == F(0)
    assert result.x == result.y == [F(1, 2), F(1, 2)]


def test_every_emitted_root_and_synthetic_matrix_is_exactly_certified():
    solved_continuous = solve_continuous()
    values_by_scale = {scale: solve_discrete(scale) for scale in (8, 16, 32)}

    records = emit_root_matrices(solved_continuous, values_by_scale)

    root_keys = {
        (record["scale"], record["h"])
        for record in records
        if record["kind"] == "root"
    }
    assert root_keys == {
        (8, "X"),
        (8, "O"),
        (16, "X"),
        (16, "O"),
        (32, "X"),
        (32, "O"),
    }
    assert any(record["synthetic"] is True for record in records)

    for record in records:
        assert set(value for row in record["matrix"] for value in row) <= {
            -1,
            0,
            1,
        }
        assert record["certificate"] in (
            "exact-verified",
            "exact-fallback-verified",
        )
        certified = solve_zero_sum(record["matrix"])
        assert str(certified.value) == record["value"]

        if record["kind"] == "root":
            assert 24 <= len(record["row_actions"]) <= 48
            assert 24 <= len(record["column_actions"]) <= 48
