from fractions import Fraction as F

from auction_ttt.richman import solve_richman


def test_richman_ttt_root_published():
    # Develin-Payne / Hwa value for their reduced game (draws are a Bob/O win)
    assert solve_richman(draw="O")["." * 9] == F(133, 256)


def test_richman_symmetric_draw_half():
    # forced by complement symmetry: swapping X/O complements all terminals,
    # the average backup preserves complements, the empty board is swap-invariant
    assert solve_richman(draw="half")["." * 9] == F(1, 2)
