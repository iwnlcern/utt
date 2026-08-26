import hashlib
import importlib.util
import json
from pathlib import Path
import subprocess
import sys

from jsonschema import Draft202012Validator

from poorman_referee.gamelog import read_log, replay_frames
from poorman_referee.protocol import canonical_dumps


ROOT = Path(__file__).parents[2]
SCHEMA_ROOT = ROOT / "docs" / "protocol" / "schema"
SPEC = ROOT / "docs" / "protocol" / "poorman-uttt-protocol-v1.md"
TRANSCRIPT = ROOT / "docs" / "protocol" / "transcript-v1.jsonl"
FIXTURES = Path(__file__).parent / "fixtures"
STREAM_FIXTURES = FIXTURES / "engine-stdin"
MATRIX = FIXTURES / "matrix.json"
GENERATOR = Path(__file__).parent / "gen_fixtures.py"
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
    assert set(replay.end["stderr"]) == {"X", "O"}


def test_every_conformance_jsonl_validates_and_replays():
    paths = sorted(FIXTURES.glob("*.jsonl"))
    paths.extend(sorted(FIXTURES.glob("parity-*/*.jsonl")))
    assert len(paths) >= 19
    for path in paths:
        events = read_log(path)
        validate_events(events)
        replay = replay_frames(events)
        assert replay.events == tuple(events), path
        assert set(replay.end["stderr"]) == {"X", "O"}, path


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
        all_closed = [game[-1] for game in events if game[-1]["reason"] != "macro_win"]
        assert all_closed
        assert all(
            (end["result"], end["reason"]) == ("draw", "exact_tie_draw")
            for end in all_closed
        )


def event_log_paths():
    paths = sorted(FIXTURES.glob("*.jsonl"))
    paths.extend(sorted(FIXTURES.glob("parity-*/*.jsonl")))
    return paths


def expected_stream_path(log_path, seat):
    relative = log_path.relative_to(FIXTURES).with_suffix("")
    return STREAM_FIXTURES / relative.parent / f"{relative.name}.{seat}.jsonl"


def fixture_tree_hashes():
    return {
        path.relative_to(FIXTURES).as_posix(): hashlib.sha256(
            path.read_bytes()
        ).hexdigest()
        for path in sorted(FIXTURES.rglob("*"))
        if path.is_file()
    }


def run_generator(*args, check=True):
    return subprocess.run(
        [sys.executable, str(GENERATOR), *map(str, args)],
        cwd=ROOT,
        check=check,
        capture_output=True,
        text=True,
    )


def load_generator_module():
    spec = importlib.util.spec_from_file_location("fixture_generator", GENERATOR)
    assert spec is not None and spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_stream_mapping_is_bijective_over_all_logs():
    logs = event_log_paths()
    expected = {
        expected_stream_path(log_path, seat)
        for log_path in logs
        for seat in ("X", "O")
    }

    assert len(logs) == 19
    assert len(expected) == 38
    assert all(path.is_file() for path in expected)
    assert set(STREAM_FIXTURES.rglob("*.jsonl")) == expected


def test_every_stream_file_round_trips_through_referee_reader():
    schema_for_type = {
        "hello": Draft202012Validator(schema("hello_request")),
        "turn": Draft202012Validator(schema("turn_request")),
        "game_end": Draft202012Validator(schema("game_end")),
    }
    stream_paths = sorted(STREAM_FIXTURES.rglob("*.jsonl"))
    assert len(stream_paths) == 38
    for path in stream_paths:
        objects = read_log(path)
        lines = path.read_bytes().splitlines(keepends=True)
        assert len(objects) == len(lines)
        for obj, line in zip(objects, lines):
            schema_for_type[obj["type"]].validate(obj)
            assert (canonical_dumps(obj) + "\n").encode() == line
            assert "elapsed_ms" not in json.dumps(obj, sort_keys=True)


def test_log_corpus_globs_and_stream_glob_are_disjoint_and_exact():
    logs = event_log_paths()
    streams = sorted(STREAM_FIXTURES.rglob("*.jsonl"))

    assert len(logs) == 19
    assert len(streams) == 38
    assert set(logs).isdisjoint(streams)
    assert all(STREAM_FIXTURES not in path.parents for path in logs)


def test_generator_check_mode_passes_on_committed_artifacts():
    run_generator("--check")


def test_check_mode_leaves_committed_tree_untouched():
    before = fixture_tree_hashes()
    run_generator("--check")
    assert fixture_tree_hashes() == before


def test_matrix_manifest_is_the_runtime_source(tmp_path):
    matrix = json.loads(MATRIX.read_text(encoding="utf-8"))
    matrix["rows"][0]["game_seed"] = "00" * 32
    matrix["rows"][0]["game_id"] = "00" * 32
    changed = tmp_path / "matrix.json"
    changed.write_text(json.dumps(matrix), encoding="utf-8")

    result = run_generator("--check", "--manifest", changed, check=False)

    assert result.returncode != 0


def test_generator_runs_manifest_argv_unchanged_from_temp_root(tmp_path):
    module = load_generator_module()
    matrix = json.loads(MATRIX.read_text(encoding="utf-8"))
    module.copy_engine_scripts(matrix, tmp_path)
    cmd = matrix["rows"][0]["cmds"]["O"]
    hello = {
        "type": "hello",
        "protocol": 1,
        "game_id": "0" * 64,
        "you": "O",
        "time_ms": 100,
        "grace_ms": 250,
        "budget": 10**9,
    }
    game_end = {
        "type": "game_end",
        "protocol": 1,
        "game_id": "0" * 64,
        "result": "draw",
        "reason": "exact_tie_draw",
    }

    result = subprocess.run(
        cmd,
        cwd=tmp_path,
        input=(canonical_dumps(hello) + "\n" + canonical_dumps(game_end) + "\n").encode(),
        check=True,
        capture_output=True,
    )

    assert json.loads(result.stdout) == {
        "name": "stub",
        "protocol": 1,
        "type": "hello",
        "version": "1",
    }


def test_protocol_pins_all_three_corpus_locations_and_stream_shape():
    markdown = SPEC.read_text(encoding="utf-8")

    assert "`docs/protocol/transcript-v1.jsonl`" in markdown
    assert "`referee/tests/fixtures/*.jsonl`" in markdown
    assert "`referee/tests/fixtures/parity-*/*.jsonl`" in markdown
    assert "`referee/tests/fixtures/engine-stdin/**/*.jsonl`" in markdown
    assert "`<log-relative-path minus .jsonl>.<seat>.jsonl`" in markdown
    assert (
        "raw canonical JSONL bytes written to that seat's standard input, "
        "with no wrapper, envelope, or extra fields"
    ) in markdown
