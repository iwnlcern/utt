import json
import re
from pathlib import Path

from auction_ttt.__main__ import main
from auction_ttt.fixtures_gen import write_or_check


FIXTURES = Path(__file__).parents[1] / "fixtures"
HAND_AUTHORED = [
    "legality.json",
    "closure_routing.json",
    "auction_traces.json",
    "terminal.json",
]
GENERATED = ["thresholds_ttt3.json", "backup_cases.json"]


def _resolve(schema: dict, root: dict) -> dict:
    if "$ref" not in schema:
        return schema
    node = root
    for part in schema["$ref"].removeprefix("#/").split("/"):
        node = node[part]
    return node


def _valid(instance, schema: dict, root: dict) -> bool:
    schema = _resolve(schema, root)
    if "allOf" in schema and not all(_valid(instance, item, root) for item in schema["allOf"]):
        return False
    if "oneOf" in schema and sum(_valid(instance, item, root) for item in schema["oneOf"]) != 1:
        return False
    if "const" in schema and instance != schema["const"]:
        return False
    if "enum" in schema and instance not in schema["enum"]:
        return False

    kinds = schema.get("type")
    if kinds is not None:
        kinds = [kinds] if isinstance(kinds, str) else kinds
        matches = {
            "object": lambda value: isinstance(value, dict),
            "array": lambda value: isinstance(value, list),
            "string": lambda value: isinstance(value, str),
            "integer": lambda value: type(value) is int,
            "null": lambda value: value is None,
        }
        if not any(matches[kind](instance) for kind in kinds):
            return False

    if isinstance(instance, dict):
        if any(key not in instance for key in schema.get("required", [])):
            return False
        for key, subschema in schema.get("properties", {}).items():
            if key in instance and not _valid(instance[key], subschema, root):
                return False
    if isinstance(instance, list):
        if len(instance) < schema.get("minItems", 0):
            return False
        if "maxItems" in schema and len(instance) > schema["maxItems"]:
            return False
        if schema.get("uniqueItems") and len({json.dumps(item, sort_keys=True) for item in instance}) != len(instance):
            return False
        if "items" in schema and not all(_valid(item, schema["items"], root) for item in instance):
            return False
    if type(instance) is int:
        if instance < schema.get("minimum", instance):
            return False
        if instance > schema.get("maximum", instance):
            return False
    if isinstance(instance, str) and "pattern" in schema:
        if re.fullmatch(schema["pattern"], instance) is None:
            return False
    return True


def _load_and_validate(filename: str, schema: dict) -> list[str]:
    path = FIXTURES / filename
    assert path.is_file(), f"missing fixture file: {filename}"
    envelope = json.loads(path.read_text(encoding="utf-8"))
    assert _valid(envelope, schema, schema), f"{filename} does not conform to schema-v1.json"
    ids = []
    for fixture in envelope["fixtures"]:
        assert fixture["game"] == envelope["game"]
        assert fixture["consumed_by"]
        ids.append(fixture["id"])
    assert len(ids) == len(set(ids)), f"duplicate fixture id in {filename}"
    return ids


def test_hand_authored_fixtures_valid():
    schema = json.loads((FIXTURES / "schema-v1.json").read_text(encoding="utf-8"))
    assert schema["$schema"] == "https://json-schema.org/draft/2020-12/schema"
    schema_doc = (FIXTURES / "SCHEMA.md").read_text(encoding="utf-8")
    assert "SITREP-orchestrator-planner-20260825-080619.md" in schema_doc

    ids = [fixture_id for filename in HAND_AUTHORED for fixture_id in _load_and_validate(filename, schema)]
    assert len(ids) == len(set(ids)), "fixture ids must be unique across hand-authored files"


def test_generated_fixtures_valid():
    schema = json.loads((FIXTURES / "schema-v1.json").read_text(encoding="utf-8"))
    generated_ids = [
        fixture_id
        for filename in GENERATED
        for fixture_id in _load_and_validate(filename, schema)
    ]
    assert len(generated_ids) == len(set(generated_ids))

    all_ids = [
        fixture_id
        for filename in HAND_AUTHORED + GENERATED
        for fixture_id in _load_and_validate(filename, schema)
    ]
    assert len(all_ids) == len(set(all_ids)), "fixture ids must be globally unique"


def test_generated_fixtures_are_reproducible(capsys):
    assert main(["fixtures", "--check"]) == 0
    assert capsys.readouterr().out == ""


def test_generated_fixture_check_reports_tampered_file(
    tmp_path, monkeypatch, capsys
):
    expected = {"thresholds_ttt3.json": {"schema_version": 1}}
    monkeypatch.setattr(
        "auction_ttt.fixtures_gen.generated_payloads", lambda: expected
    )
    tampered = tmp_path / "thresholds_ttt3.json"
    tampered.write_text('{"schema_version": 2}\n', encoding="utf-8")

    assert write_or_check(tmp_path, check=True) == ["thresholds_ttt3.json"]
    output = capsys.readouterr().out
    assert f"--- {tampered}" in output
    assert "+++ generated:thresholds_ttt3.json" in output


def test_forced_closed_free_choice_fixture_excludes_engine_consumer():
    envelope = json.loads((FIXTURES / "legality.json").read_text(encoding="utf-8"))
    fixture = next(
        item
        for item in envelope["fixtures"]
        if item["id"] == "legality-forced-closed-means-free-choice"
    )

    assert fixture["consumed_by"] == ["harness", "theory"]


def test_backup_cases_are_engine_contract():
    # MR17 owner decision: theory-c1/SITREP-pair-planner-20260827-021436.md
    envelope = json.loads(
        (FIXTURES / "backup_cases.json").read_text(encoding="utf-8")
    )
    for fixture in envelope["fixtures"]:
        assert fixture["consumed_by"] == ["engine", "theory"]
