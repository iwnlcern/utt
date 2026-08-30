from __future__ import annotations

import json
import pathlib
import shutil
import subprocess
from dataclasses import dataclass

import pytest

from certschema import vectors as emitter_vectors


THEORY = pathlib.Path(__file__).resolve().parents[1]
CHECKER = THEORY / "checker"
PKG = THEORY / "certschema"
VEC = PKG / "vectors"
MUT = PKG / "mutants"

EXPECTED_CODE = {
    "MUT-H01": "E_MAGIC",
    "MUT-H02": "E_SCHEMA_VERSION",
    "MUT-H03": "E_CLAIM_TAG",
    "MUT-H04": "E_HEADER_RESERVED",
    "MUT-H05": "E_RULESET",
    "MUT-H06": "E_SYMMETRY",
    "MUT-H07": "E_ROOT_STATE",
    "MUT-H08": "E_RULE_IDS_V0",
    "MUT-H09": "E_MANIFEST_EMPTY",
    "MUT-H10": "E_ROW_ORDER",
    "MUT-H11": "E_CHUNK_LAYOUT",
    "MUT-H12": "E_MANIFEST_DIGEST",
    "MUT-H13": "E_ROW_FLAGS",
    "MUT-H14": "E_ROW_PLY_RANGE",
    "MUT-R01": "E_DUP_STATE",
    "MUT-R02": "E_NONCANONICAL",
    "MUT-R03": "E_ILLFORMED_STATE",
    "MUT-R04": "E_FORCED_CLOSED",
    "MUT-R05": "E_PLY_MISMATCH",
    "MUT-R06": "E_RECORD_RESERVED",
    "MUT-R07": "E_PAYLOAD",
    "MUT-R08": "E_ILLEGAL_MOVE",
    "MUT-R09": "E_CHILD_MISSING",
    "MUT-R10": "E_REPLY_MISSING",
    "MUT-R11": "E_TERMINAL_CLAIM",
    "MUT-R12": "E_KIND_TERMINALITY",
    "MUT-R13": "E_KIND_SIDE",
    "MUT-R14": "E_RULE_RECORD_V0",
    "MUT-R15": "E_CHUNK_DIGEST",
    "MUT-R16": "E_CHUNK_LAYOUT",
    "MUT-R17": "E_CHUNK_COUNTS",
    "MUT-R18": "E_TOTALS",
    "MUT-R19": "E_ROOT_ABSENT",
    "MUT-R20": "E_KIND_TAG",
    "MUT-R21": "E_KIND_TERMINALITY",
    "MUT-R22": "E_REPLY_MISSING",
    "MUT-V01": "EV_PARSE",
    "MUT-V02": "EV_MEMBER_PATH",
    "MUT-V03": "EV_MEMBER_ABSENT",
    "MUT-V04": "EV_MEMBER_DIGEST",
    "MUT-V05": "EV_CLAIM_BINDING",
    "MUT-V06": "EV_MEMBER_TAG",
    "MUT-V07": "EV_FIELD_MISMATCH",
    "MUT-V08": "EV_COMPLETENESS",
}


@dataclass(frozen=True)
class RunResult:
    exit: int
    json: dict[str, object]
    stdout: str
    stderr: str


class Checker:
    def __init__(self, binary: pathlib.Path):
        self.binary = binary

    def run(self, *args: str | pathlib.Path, expect_json: bool = True) -> RunResult:
        completed = subprocess.run(
            [str(self.binary), *(str(arg) for arg in args)],
            check=False,
            capture_output=True,
            text=True,
        )
        payload = json.loads(completed.stdout) if expect_json else {}
        return RunResult(
            exit=completed.returncode,
            json=payload,
            stdout=completed.stdout,
            stderr=completed.stderr,
        )

    def cert(self, path: pathlib.Path) -> RunResult:
        return self.run("cert", path)

    def verdict(self, path: pathlib.Path, *, mode: str | None = None) -> RunResult:
        args: list[str | pathlib.Path] = ["verdict", path]
        if mode is not None:
            args.extend(("--mode", mode))
        return self.run(*args)


@pytest.fixture(scope="session")
def checker() -> Checker:
    compiler = shutil.which("c3c")
    assert compiler is not None, "c3c is required for the checker conformance battery"
    build = subprocess.run(
        [compiler, "build"],
        cwd=CHECKER,
        check=False,
        capture_output=True,
        text=True,
    )
    assert build.returncode == 0, (
        "c3c build failed for the checker conformance battery\n"
        f"stdout:\n{build.stdout}\nstderr:\n{build.stderr}"
    )
    binary = CHECKER / "build" / "uttt-checker"
    assert binary.is_file(), f"c3c build did not produce {binary}"
    return Checker(binary)


def test_all_positive_certificates_pass(checker: Checker):
    for name in (
        "golden-terminal-winx.utc",
        "p2-prover-winx.utc",
        "p3-opponent-winx.utc",
        "p4-opponent2-winx.utc",
    ):
        result = checker.cert(VEC / name)
        assert result.exit == 0 and result.json["verdict"] == "pass", (
            name,
            result.json,
        )


@pytest.mark.parametrize("mutant_id, expected", EXPECTED_CODE.items())
def test_every_mutant_rejects_with_its_intended_code(
    checker: Checker, mutant_id: str, expected: str
):
    path = MUT / f"{mutant_id}.bin"
    result = (
        checker.verdict(path, mode="subgame")
        if mutant_id.startswith("MUT-V")
        else checker.cert(path)
    )
    assert result.exit == 1 and result.json["code"] == expected, (
        mutant_id,
        result.json,
    )


def test_gate02_modes(checker: Checker):
    ok = checker.verdict(VEC / "golden-winx.utv", mode="subgame")
    assert ok.exit == 0 and ok.json["root_is_initial"] is False
    bad = checker.verdict(VEC / "golden-winx.utv", mode="deliverable")
    assert bad.exit == 1 and bad.json["code"] == "EV_ROOT_NOT_INITIAL"
    default = checker.verdict(VEC / "golden-winx.utv")
    assert default.exit == 1 and default.json["code"] == "EV_ROOT_NOT_INITIAL"


def test_battery_covers_catalogue_exactly(checker: Checker):
    del checker
    catalogue = json.loads((PKG / "catalogue.json").read_text())
    mutant_ids = {
        row["id"] for row in catalogue["vectors"] if row["kind"] == "mutant"
    }
    assert mutant_ids == set(EXPECTED_CODE)


def test_report_contract_fields(checker: Checker):
    ok = checker.cert(VEC / "golden-terminal-winx.utc")
    for key in (
        "verdict",
        "code",
        "fail_position",
        "claim",
        "root_hex",
        "root_is_initial",
        "schema_version",
        "ruleset_id",
        "ruleset_digest_hex",
        "symmetry_id",
        "totals",
        "orphans",
        "wall_ms",
        "cpu_ms",
        "checker_version",
        "c3c_version",
    ):
        assert key in ok.json, key
    assert ok.json["ruleset_digest_hex"] == "16e85b36332493470fa1006516128579"
    assert ok.json["orphans"] == 0
    assert ok.json["wall_ms"] >= 0 and ok.json["cpu_ms"] >= 0
    assert ok.stdout.count("\n") == 1

    bad = checker.cert(MUT / "MUT-R15.bin")
    fail_position = bad.json["fail_position"]
    assert bad.exit == 1 and fail_position["byte_offset"] >= 0
    assert fail_position["chunk"] >= 0


def test_orphan_island_reported_not_failed(checker: Checker, tmp_path: pathlib.Path):
    path = tmp_path / "orphan.utc"
    path.write_bytes(emitter_vectors.append_orphan_terminal(emitter_vectors.build_p1()))
    result = checker.cert(path)
    assert result.exit == 0 and result.json["orphans"] == 1


def test_long_chunk_digest_is_checked_with_emitter_bytes(
    checker: Checker, tmp_path: pathlib.Path
):
    blob = emitter_vectors.build_long_digest_probe()
    parsed = emitter_vectors.parse_cert(blob)
    assert max(row.byte_length for row in parsed.rows) > 240

    path = tmp_path / "long-digest.utc"
    path.write_bytes(blob)
    result = checker.cert(path)
    assert result.exit == 1 and result.json["code"] == "E_DUP_STATE"


def test_repeated_verification_is_semantically_deterministic(checker: Checker):
    first = checker.cert(VEC / "p4-opponent2-winx.utc").json
    second = checker.cert(VEC / "p4-opponent2-winx.utc").json
    for payload in (first, second):
        payload.pop("wall_ms")
        payload.pop("cpu_ms")
    assert first == second


def test_usage_and_top_level_io_errors_exit_two(checker: Checker, tmp_path: pathlib.Path):
    usage = checker.run(expect_json=False)
    assert usage.exit == 2 and usage.stdout == ""

    missing = tmp_path / "missing.utc"
    cert = checker.run("cert", missing, expect_json=False)
    verdict = checker.run("verdict", missing, expect_json=False)
    assert cert.exit == 2 and cert.stdout == ""
    assert verdict.exit == 2 and verdict.stdout == ""


def test_missing_verdict_member_is_semantic_reject(checker: Checker):
    # Members are resolved and fully verified one at a time; keep this
    # member-major order rather than hoisting resolution into a separate pass.
    result = checker.verdict(MUT / "MUT-V03.bin", mode="subgame")
    assert result.exit == 1 and result.json["code"] == "EV_MEMBER_ABSENT"


def test_verdict_path_normalization_fault_is_semantic_reject(checker: Checker):
    pathological = "/.." + str(VEC / "golden-winx.utv")
    result = checker.run("verdict", pathological, "--mode", "subgame")
    assert result.exit == 1 and result.json["code"] == "EV_PARSE"
    assert result.stdout.count("\n") == 1
