from fractions import Fraction as F

from auction_ttt.continuous import backup, solve


def test_backup_zero_children():
    assert backup(F(0), F(0), "X") == (F(0), F(0))


def test_backup_identities_grid():
    vals = sorted({F(p, q) for q in range(1, 13) for p in range(q + 1)})
    n = 0
    for i, a in enumerate(vals):
        for b in vals[i:]:
            T, r = backup(a, b, "X")
            assert T == b / (1 - a + b)
            assert r == (b - a) / (1 - a + b)
            assert a <= T <= b
            if r != 1:
                assert (T - r) / (1 - r) == a
                assert T / (1 - r) == b
            n += 1
    assert n >= 968


def test_backup_zugzwang_named_cases():
    assert backup(F(3, 4), F(1, 4), "X") == (F(3, 4), F(0))
    assert backup(F(3, 4), F(1, 4), "O") == (F(1, 4), F(0))
    assert backup(F(2, 3), F(1, 3), "X") == (F(2, 3), F(0))
    assert backup(F(2, 3), F(1, 3), "O") == (F(1, 3), F(0))


def test_solve_traversal_order_guard():
    solved = solve()
    node = solved[("XX.......", "X")]
    assert node.a == F(0) and node.T < F(1, 2)
