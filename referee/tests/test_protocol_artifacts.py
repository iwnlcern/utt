import json
from pathlib import Path

from jsonschema import Draft202012Validator

from poorman_referee.gamelog import read_log, replay_frames


ROOT = Path(__file__).parents[2]
SCHEMA_ROOT = ROOT / "docs" / "protocol" / "schema"
SPEC = ROOT / "docs" / "protocol" / "poorman-uttt-protocol-v1.md"
TRANSCRIPT = ROOT / "docs" / "protocol" / "transcript-v1.jsonl"
FIXTURES = Path(__file__).parent / "fixtures"
MESSAGE_SCHEMAS = (
    "hello_request",
    "hello_reply",
    "turn_request",
    "turn_reply",
    "game_end",
)


def schema(name):
    with (SCHEMA_ROOT / f"{name}.schema.json").open(encoding="utf-8") as fh:
        return json.load(fh)


def validate_events(events):
    for event in events:
        name = {
            "game_start": "log_game_start",
            "auction": "log_auction",
            "recovery": "log_recovery",
            "game_end": "log_game_end",
        }[event["event"]]
        Draft202012Validator(schema(name)).validate(event)


def table_fields(markdown, schema_name):
    heading = f"### `{schema_name}.schema.json`"
    section = markdown.split(heading, 1)[1].split("\n### ", 1)[0]
    fields = {}
    for line in section.splitlines():
        if not line.startswith("| `"):
            continue
        cells = [cell.strip() for cell in line.strip("|").split("|")]
        fields[cells[0].strip("`")] = cells[1]
    return fields


def test_spec_field_tables_match_all_message_schema_properties_and_required_sets():
    markdown = SPEC.read_text(encoding="utf-8")

    for name in MESSAGE_SCHEMAS:
        artifact = schema(name)
        fields = table_fields(markdown, name)
        assert set(fields) == set(artifact["properties"])
        assert {field for field, required in fields.items() if required == "yes"} == set(
            artifact["required"]
        )


def test_real_transcript_validates_and_replays_losslessly():
    events = read_log(TRANSCRIPT)

    validate_events(events)
    replay = replay_frames(events)
    assert replay.events == tuple(events)
    assert replay.frames
    assert replay.end["reason"] in {"macro_win", "chip_count", "exact_tie_draw"}


def test_every_conformance_jsonl_validates_and_replays():
    paths = sorted(FIXTURES.rglob("*.jsonl"))
    assert len(paths) >= 19
    for path in paths:
        events = read_log(path)
        validate_events(events)
        replay = replay_frames(events)
        assert replay.events == tuple(events), path


def test_fault_matrix_fixtures_cover_all_named_faults():
    expected = {
        "timeout",
        "eof_or_crash",
        "invalid_utf8",
        "invalid_json",
        "schema_violation",
        "wrong_request_id",
        "extra_protocol_line",
        "oversize_line",
        "illegal_bid",
        "illegal_move",
    }
    observed = set()
    for path in FIXTURES.glob("fault-*.jsonl"):
        events = read_log(path)
        first = next(event for event in events if event["event"] == "auction")
        validation = first["attempts"][0]["turns"]["X"]["validation"]
        observed.add(validation)
        assert first["resolution"]["reason"] == "fault"
        assert first["resolution"]["payment"] == 0
        assert any(event["event"] == "recovery" for event in events)
    assert observed == expected


def test_recovery_fault_fixtures_cover_both_dispositions_and_both_failed_void():
    single = read_log(FIXTURES / "recovery-single-fail.jsonl")
    double = read_log(FIXTURES / "recovery-double-one-fail.jsonl")
    both = read_log(FIXTURES / "recovery-both-fail.jsonl")

    assert next(e for e in single if e["event"] == "auction")["outcome"] == "resolved"
    assert next(e for e in double if e["event"] == "auction")["outcome"] == "aborted_recovery_fault"
    assert single[-1]["reason"] == double[-1]["reason"] == "recovery_fault"
    assert both[-1]["reason"] == "recovery_fault"
    assert both[-1]["result"] == "void"


def test_fault_won_terminal_fixture_has_no_recovery_or_result_rewrite():
    path = FIXTURES / "terminal-fault-won-no-recovery.jsonl"
    events = read_log(path)
    terminal = next(
        event for event in reversed(events) if event["event"] == "auction"
    )

    assert terminal["resolution"]["reason"] == "fault"
    assert terminal["resolution"]["macro_line"] is not None
    assert not [
        event
        for event in events
        if event["event"] == "recovery" and event["ply"] == terminal["ply"]
    ]
    assert events[-1]["reason"] == "macro_win"
    for record in events[-1]["stderr"].values():
        assert (path.parent / record["path"]).is_file()


def test_both_pair_seed_parity_fixtures_swap_engine_favoritism():
    for directory, parity in (("parity-even", 0), ("parity-odd", 1)):
        paths = sorted((FIXTURES / directory).glob("*.jsonl"))
        assert len(paths) == 2
        events = [read_log(path) for path in paths]
        starts = [game[0] for game in events]
        assert int(starts[0]["pair_seed"][:2], 16) % 2 == parity
        assert starts[0]["pair_seed"] == starts[1]["pair_seed"]
        assert starts[0]["pair_coin_seat"] == starts[1]["pair_coin_seat"]
        coin = starts[0]["pair_coin_seat"]
        favored = []
        for start, game in zip(starts, events):
            first = next(event for event in game if event["event"] == "auction")
            assert first["resolution"]["reason"] == "tie_coin"
            assert first["resolution"]["winner"] == coin
            favored.append(start["engines"][coin]["engine_id"])
        assert set(favored) == {"A", "B"}
