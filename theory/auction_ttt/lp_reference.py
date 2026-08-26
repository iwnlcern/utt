"""Exact-certified reference solver for finite two-player zero-sum games."""

from dataclasses import dataclass
from fractions import Fraction
from typing import Callable

import numpy as np
from scipy.optimize import linprog

from auction_ttt import rules as R


class LPEscalation(Exception):
    """No branch of the required LP ladder produced an exact certificate."""


@dataclass(frozen=True)
class LPResult:
    value: Fraction
    x: list[Fraction]
    y: list[Fraction]
    certificate: str
    path: str


class _CandidateFailure(Exception):
    pass


def _as_fraction_matrix(A):
    if not A or not A[0]:
        raise ValueError("payoff matrix must be nonempty")
    width = len(A[0])
    if any(len(row) != width for row in A):
        raise ValueError("payoff matrix must be rectangular")
    return [[value if isinstance(value, Fraction) else Fraction(value) for value in row] for row in A]


def _solve_square(matrix, rhs):
    size = len(matrix)
    if size == 0 or any(len(row) != size for row in matrix) or len(rhs) != size:
        raise _CandidateFailure("linear system is not square")
    augmented = [list(row) + [rhs[index]] for index, row in enumerate(matrix)]

    for column in range(size):
        pivot = next(
            (row for row in range(column, size) if augmented[row][column]),
            None,
        )
        if pivot is None:
            raise _CandidateFailure("singular exact equalizer system")
        augmented[column], augmented[pivot] = augmented[pivot], augmented[column]
        divisor = augmented[column][column]
        augmented[column] = [entry / divisor for entry in augmented[column]]
        for row in range(size):
            if row == column:
                continue
            multiplier = augmented[row][column]
            if multiplier:
                augmented[row] = [
                    entry - multiplier * pivot_entry
                    for entry, pivot_entry in zip(
                        augmented[row], augmented[column], strict=True
                    )
                ]
    return [augmented[row][-1] for row in range(size)]


def _dot(left, right):
    return sum((a * b for a, b in zip(left, right, strict=True)), Fraction(0))


def _float_strategies(A, perturb=False):
    rows, columns = len(A), len(A[0])
    epsilon = Fraction(1, 10**12)
    working = [
        [
            value + epsilon * (i * columns + j) if perturb else value
            for j, value in enumerate(row)
        ]
        for i, row in enumerate(A)
    ]
    floats = np.asarray([[float(value) for value in row] for row in working])

    row_objective = np.r_[np.zeros(rows), -1.0]
    row_ub = np.c_[-floats.T, np.ones(columns)]
    row_result = linprog(
        row_objective,
        A_ub=row_ub,
        b_ub=np.zeros(columns),
        A_eq=np.r_[np.ones(rows), 0.0][None, :],
        b_eq=np.array([1.0]),
        bounds=[(0.0, None)] * rows + [(None, None)],
        method="highs",
    )

    column_objective = np.r_[np.zeros(columns), 1.0]
    column_ub = np.c_[floats, -np.ones(rows)]
    column_result = linprog(
        column_objective,
        A_ub=column_ub,
        b_ub=np.zeros(rows),
        A_eq=np.r_[np.ones(columns), 0.0][None, :],
        b_eq=np.array([1.0]),
        bounds=[(0.0, None)] * columns + [(None, None)],
        method="highs",
    )
    if not row_result.success or not column_result.success:
        raise _CandidateFailure(
            f"HiGHS failed: row={row_result.message}; column={column_result.message}"
        )
    return row_result.x[:rows], column_result.x[:columns]


def _normal_support(x_float, y_float):
    return (
        [index for index, value in enumerate(x_float) if value > 1e-9],
        [index for index, value in enumerate(y_float) if value > 1e-9],
    )


def _validate_support(support, size, name):
    values = list(support)
    if not values or len(values) != len(set(values)):
        raise _CandidateFailure(f"{name} support is empty or duplicated")
    if any(not isinstance(index, int) or isinstance(index, bool) for index in values):
        raise _CandidateFailure(f"{name} support contains a non-integer index")
    if any(index < 0 or index >= size for index in values):
        raise _CandidateFailure(f"{name} support index is out of range")
    return sorted(values)


def _candidate_from_support(A, row_support, column_support):
    rows, columns = len(A), len(A[0])
    row_support = _validate_support(row_support, rows, "row")
    column_support = _validate_support(column_support, columns, "column")
    if len(row_support) != len(column_support):
        raise _CandidateFailure("support cardinalities differ")

    x_system = [
        [*[A[i][j] for i in row_support], Fraction(-1)]
        for j in column_support
    ]
    x_system.append([*[Fraction(1) for _ in row_support], Fraction(0)])
    x_solution = _solve_square(
        x_system, [Fraction(0)] * len(column_support) + [Fraction(1)]
    )

    y_system = [
        [*[A[i][j] for j in column_support], Fraction(-1)]
        for i in row_support
    ]
    y_system.append([*[Fraction(1) for _ in column_support], Fraction(0)])
    y_solution = _solve_square(
        y_system, [Fraction(0)] * len(row_support) + [Fraction(1)]
    )

    x = [Fraction(0) for _ in range(rows)]
    y = [Fraction(0) for _ in range(columns)]
    for index, probability in zip(row_support, x_solution[:-1], strict=True):
        x[index] = probability
    for index, probability in zip(column_support, y_solution[:-1], strict=True):
        y[index] = probability
    if x_solution[-1] != y_solution[-1]:
        raise _CandidateFailure("row and column equalizers disagree")
    return x_solution[-1], x, y, row_support, column_support


def _verify_candidate(A, value, x, y, row_support, column_support):
    if _dot(x, [Fraction(1)] * len(x)) != 1:
        raise _CandidateFailure("row strategy is not normalized")
    if _dot(y, [Fraction(1)] * len(y)) != 1:
        raise _CandidateFailure("column strategy is not normalized")
    if any(probability < 0 for probability in [*x, *y]):
        raise _CandidateFailure("strategy has a negative entry")
    if any(x[index] <= 0 for index in row_support):
        raise _CandidateFailure("row support contains a zero entry")
    if any(y[index] <= 0 for index in column_support):
        raise _CandidateFailure("column support contains a zero entry")

    column_payoffs = [
        sum((x[i] * A[i][j] for i in range(len(A))), Fraction(0))
        for j in range(len(A[0]))
    ]
    row_payoffs = [
        sum((A[i][j] * y[j] for j in range(len(A[0]))), Fraction(0))
        for i in range(len(A))
    ]
    if any(column_payoffs[j] != value for j in column_support):
        raise _CandidateFailure("on-support column equality failed")
    if any(row_payoffs[i] != value for i in row_support):
        raise _CandidateFailure("on-support row equality failed")
    if any(payoff < value for payoff in column_payoffs):
        raise _CandidateFailure("off-support row-player inequality failed")
    if any(payoff > value for payoff in row_payoffs):
        raise _CandidateFailure("off-support column-player inequality failed")
    if min(column_payoffs) != value or max(row_payoffs) != value:
        raise _CandidateFailure("exact exploitability is nonzero")


def _basis_solution(A, b, basis):
    matrix = [[A[row][column] for column in basis] for row in range(len(A))]
    return _solve_square(matrix, b)


def _revised_simplex_max(A, b, c, basis, enterable):
    basis = list(basis)
    enterable = tuple(enterable)
    for _iteration in range(10000):
        x_basic = _basis_solution(A, b, basis)
        if any(value < 0 for value in x_basic):
            raise _CandidateFailure("simplex basis became infeasible")
        basis_matrix = [
            [A[row][column] for column in basis] for row in range(len(A))
        ]
        multipliers = _solve_square(
            [list(row) for row in zip(*basis_matrix, strict=True)],
            [c[column] for column in basis],
        )
        nonbasic = [column for column in enterable if column not in basis]
        reduced = {
            column: c[column]
            - _dot(multipliers, [A[row][column] for row in range(len(A))])
            for column in nonbasic
        }
        improving = [column for column in nonbasic if reduced[column] > 0]
        if not improving:
            solution = [Fraction(0) for _ in range(len(c))]
            for column, value in zip(basis, x_basic, strict=True):
                solution[column] = value
            return solution, basis

        entering = min(improving)
        direction = _solve_square(
            basis_matrix, [A[row][entering] for row in range(len(A))]
        )
        ratios = [
            (x_basic[row] / direction[row], basis[row], row)
            for row in range(len(basis))
            if direction[row] > 0
        ]
        if not ratios:
            raise _CandidateFailure("exact simplex is unbounded")
        minimum_ratio = min(item[0] for item in ratios)
        leaving_row = min(
            (item for item in ratios if item[0] == minimum_ratio),
            key=lambda item: item[1],
        )[2]
        basis[leaving_row] = entering
    raise _CandidateFailure("exact simplex iteration limit exceeded")


def _min_cover(B):
    variables = len(B)
    constraints = len(B[0])
    surplus_start = variables
    artificial_start = variables + constraints
    width = variables + 2 * constraints
    equations = []
    for column in range(constraints):
        row = [B[index][column] for index in range(variables)]
        row.extend(
            Fraction(-1 if index == column else 0)
            for index in range(constraints)
        )
        row.extend(
            Fraction(1 if index == column else 0)
            for index in range(constraints)
        )
        equations.append(row)
    rhs = [Fraction(1) for _ in range(constraints)]
    phase_one = [Fraction(0) for _ in range(width)]
    for index in range(artificial_start, width):
        phase_one[index] = Fraction(-1)
    initial_basis = list(range(artificial_start, width))
    phase_one_solution, basis = _revised_simplex_max(
        equations, rhs, phase_one, initial_basis, range(width)
    )
    if _dot(phase_one, phase_one_solution) != 0:
        raise _CandidateFailure("exact phase one found no feasible cover")

    for row, basic in enumerate(list(basis)):
        if basic < artificial_start:
            continue
        basis_matrix = [
            [equations[r][column] for column in basis]
            for r in range(constraints)
        ]
        replacement = None
        for candidate in range(artificial_start):
            if candidate in basis:
                continue
            direction = _solve_square(
                basis_matrix,
                [equations[r][candidate] for r in range(constraints)],
            )
            if direction[row] != 0:
                replacement = candidate
                break
        if replacement is None:
            raise _CandidateFailure("could not remove a phase-one artificial")
        basis[row] = replacement

    phase_two = [Fraction(0) for _ in range(width)]
    for index in range(variables):
        phase_two[index] = Fraction(-1)
    solution, _basis = _revised_simplex_max(
        equations,
        rhs,
        phase_two,
        basis,
        range(artificial_start),
    )
    return solution[:variables]


def _fallback_candidate(A):
    minimum = min(min(row) for row in A)
    shift = Fraction(1) - minimum
    positive = [[value + shift for value in row] for row in A]
    row_cover = _min_cover(positive)
    row_total = sum(row_cover, Fraction(0))
    if row_total <= 0:
        raise _CandidateFailure("row cover has nonpositive mass")
    x = [value / row_total for value in row_cover]
    shifted_value = 1 / row_total

    upper = max(max(row) for row in positive) + 1
    complement = [
        [upper - positive[i][j] for i in range(len(positive))]
        for j in range(len(positive[0]))
    ]
    column_cover = _min_cover(complement)
    column_total = sum(column_cover, Fraction(0))
    if column_total <= 0:
        raise _CandidateFailure("column cover has nonpositive mass")
    y = [value / column_total for value in column_cover]
    if upper - 1 / column_total != shifted_value:
        raise _CandidateFailure("exact fallback primal and dual values disagree")

    value = shifted_value - shift
    row_support = [index for index, probability in enumerate(x) if probability]
    column_support = [index for index, probability in enumerate(y) if probability]
    _verify_candidate(A, value, x, y, row_support, column_support)
    return LPResult(
        value=value,
        x=x,
        y=y,
        certificate="exact-fallback-verified",
        path="fallback",
    )


def solve_zero_sum(A, _support_hook: Callable | None = None) -> LPResult:
    """Solve a zero-sum matrix game and certify the answer using Fractions."""
    matrix = _as_fraction_matrix(A)
    failures = []
    for path, perturb in (("direct", False), ("perturbed", True)):
        try:
            x_float, y_float = _float_strategies(matrix, perturb=perturb)
            extractor = _support_hook if path == "direct" and _support_hook else _normal_support
            row_support, column_support = extractor(x_float, y_float)
            value, x, y, row_support, column_support = _candidate_from_support(
                matrix, row_support, column_support
            )
            _verify_candidate(
                matrix, value, x, y, row_support, column_support
            )
            return LPResult(
                value=value,
                x=x,
                y=y,
                certificate="exact-verified",
                path=path,
            )
        except (_CandidateFailure, ValueError, TypeError) as exc:
            failures.append(f"{path}: {exc}")

    try:
        return _fallback_candidate(matrix)
    except (_CandidateFailure, ValueError, TypeError, ZeroDivisionError) as exc:
        failures.append(f"fallback: {exc}")
        raise LPEscalation("; ".join(failures)) from exc


def _round_fraction(value):
    return (2 * value.numerator + value.denominator) // (2 * value.denominator)


def _candidate_bids(stack, total, critical_ratio):
    rounded = _round_fraction(critical_ratio * total)
    candidates = {
        0,
        stack,
        stack // 4,
        stack // 2,
        (3 * stack) // 4,
        *(rounded + offset for offset in range(-2, 3)),
    }
    return sorted(bid for bid in candidates if 0 <= bid <= stack)


def _candidate_moves(board, preferred):
    legal = R.legal_moves(board)
    if board == "." * 9:
        return (0, 1, 4, 8)
    ordered = [preferred, *legal]
    return tuple(dict.fromkeys(ordered))[:4]


def _value_table(payload):
    if isinstance(payload, tuple) and len(payload) == 2:
        return payload
    return payload, None


def _action_matrix(board, h, bx, bo, values, node):
    row_actions = [
        {"bid": bid, "move": move}
        for bid in _candidate_bids(bx, bx + bo, node.r)
        for move in _candidate_moves(board, node.best_x)
    ]
    column_actions = [
        {"bid": bid, "move": move}
        for bid in _candidate_bids(bo, bx + bo, node.r)
        for move in _candidate_moves(board, node.best_o)
    ]
    matrix = []
    for row_action in row_actions:
        row = []
        for column_action in column_actions:
            bid_x, bid_o = row_action["bid"], column_action["bid"]
            if bid_x > bid_o or (bid_x == bid_o and h == R.X):
                child = R.apply_move(board, row_action["move"], R.X)
                outcome = int(values[(child, R.O)][bx - bid_x, bo])
            else:
                child = R.apply_move(board, column_action["move"], R.O)
                outcome = int(values[(child, R.X)][bx, bo - bid_o])
            row.append(outcome - 1)
        matrix.append(row)
    return row_actions, column_actions, matrix


def _certified_record(**record):
    result = solve_zero_sum(record["matrix"])
    return {
        **record,
        "value": str(result.value),
        "x": [str(value) for value in result.x],
        "y": [str(value) for value in result.y],
        "certificate": result.certificate,
        "path": result.path,
    }


def _first_named_inband(solved_cont, masks):
    for key in sorted(solved_cont):
        board, _h = key
        if R.is_terminal(board) or key not in masks:
            continue
        threshold = solved_cont[key].T
        p, q = threshold.numerator, threshold.denominator
        for bx, bo in np.argwhere(masks[key]):
            bx, bo = int(bx), int(bo)
            total = bx + bo
            if total and abs(bx * q - p * total) <= board.count(".") * q:
                return key, bx, bo
    raise ValueError("no named masked in-band state is available")


def _synthetic_matrix(a, b, h, bx, bo):
    matrix = []
    for bid_x in range(bx + 1):
        row = []
        for bid_o in range(bo + 1):
            if bid_x > bid_o or (bid_x == bid_o and h == R.X):
                child_x, child_o, threshold = bx - bid_x, bo, a
            else:
                child_x, child_o, threshold = bx, bo - bid_o, b
            x_wins = (
                child_x * threshold.denominator
                > threshold.numerator * (child_x + child_o)
            )
            row.append(1 if x_wins else -1)
        matrix.append(row)
    return matrix


def emit_root_matrices(solved_cont, V_by_scale):
    """Build and exactly certify the C10 root, near-band, and synthetic cases."""
    required_scales = (8, 16, 32)
    missing = [scale for scale in required_scales if scale not in V_by_scale]
    if missing:
        raise ValueError(f"missing required scales: {missing}")

    records = []
    root = "." * 9
    for scale in required_scales:
        values, masks = _value_table(V_by_scale[scale])
        for h in (R.X, R.O):
            row_actions, column_actions, matrix = _action_matrix(
                root, h, scale, scale, values, solved_cont[(root, h)]
            )
            records.append(
                _certified_record(
                    id=f"root-{h}-N{scale}",
                    kind="root",
                    synthetic=False,
                    scale=scale,
                    board=root,
                    h=h,
                    budgets={"x": scale, "o": scale},
                    row_actions=row_actions,
                    column_actions=column_actions,
                    matrix=matrix,
                )
            )

        if masks is not None:
            (board, h), bx, bo = _first_named_inband(solved_cont, masks)
            row_actions, column_actions, matrix = _action_matrix(
                board, h, bx, bo, values, solved_cont[(board, h)]
            )
            records.append(
                _certified_record(
                    id=f"near-band-{board}-{h}-N{scale}-{bx}-{bo}",
                    kind="near-band",
                    synthetic=False,
                    scale=scale,
                    board=board,
                    h=h,
                    budgets={"x": bx, "o": bo},
                    row_actions=row_actions,
                    column_actions=column_actions,
                    matrix=matrix,
                )
            )

    synthetic_scale = 16
    synthetic_budgets = {"x": synthetic_scale // 2, "o": synthetic_scale // 2}
    a, b, h = Fraction(3, 4), Fraction(1, 4), R.X
    synthetic_matrix = _synthetic_matrix(
        a, b, h, synthetic_budgets["x"], synthetic_budgets["o"]
    )
    records.append(
        _certified_record(
            id="synthetic-zugzwang-3-4-1-4-X-N16",
            kind="synthetic-zugzwang",
            synthetic=True,
            scale=synthetic_scale,
            board=None,
            h=h,
            budgets=synthetic_budgets,
            a=str(a),
            b=str(b),
            row_actions=[{"bid": bid} for bid in range(synthetic_budgets["x"] + 1)],
            column_actions=[
                {"bid": bid} for bid in range(synthetic_budgets["o"] + 1)
            ],
            matrix=synthetic_matrix,
        )
    )
    return records
