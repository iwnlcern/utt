import json
from collections.abc import Iterator
from pathlib import Path

from .rules import Position


def _integer(value) -> bool:
    return type(value) is int


def load_fixture(obj: dict) -> tuple[Position, str | None, dict[str, int]]:
    if not isinstance(obj, dict) or obj.get("game") != "uttt":
        raise ValueError("fixture must be a theory UTTT object")
    state = obj.get("state")
    if not isinstance(state, dict):
        raise ValueError("fixture state must be an object")
    board = state.get("board")
    if not (
        isinstance(board, list)
        and len(board) == 9
        and all(
            isinstance(cells, str)
            and len(cells) == 9
            and set(cells) <= {".", "X", "O"}
            for cells in board
        )
    ):
        raise ValueError("fixture board must be nine canonical board strings")
    forced = state.get("forced")
    if forced is not None and not (_integer(forced) and 0 <= forced <= 8):
        raise ValueError("fixture forced must be an integer 0..8 or null")
    tie_owner = state.get("h")
    if tie_owner not in ("X", "O", None):
        raise ValueError("fixture h must be X, O, or null")
    source_budgets = state.get("budgets")
    if not (
        isinstance(source_budgets, dict)
        and set(source_budgets) >= {"x", "o"}
        and all(
            _integer(source_budgets[key]) and source_budgets[key] >= 0
            for key in ("x", "o")
        )
    ):
        raise ValueError("fixture budgets must contain nonnegative integer x/o")
    return (
        Position(board=tuple(board), forced=forced),
        tie_owner,
        {"X": source_budgets["x"], "O": source_budgets["o"]},
    )


def iter_fixture_files(root) -> Iterator[dict]:
    root_path = Path(root)
    if not root_path.is_dir():
        return
    for path in sorted(root_path.glob("*.json")):
        if path.name == "schema-v1.json":
            continue
        with path.open(encoding="utf-8") as fh:
            envelope = json.load(fh)
        if not isinstance(envelope, dict) or envelope.get("schema_version") != 1:
            raise ValueError(f"{path} is not a schema-version-1 fixture envelope")
        fixtures = envelope.get("fixtures")
        if not isinstance(fixtures, list):
            raise ValueError(f"{path} fixture envelope has no fixtures array")
        for fixture in fixtures:
            if not isinstance(fixture, dict):
                raise ValueError(f"{path} contains a non-object fixture")
            fixture = dict(fixture)
            fixture.setdefault("game", envelope.get("game"))
            yield fixture
