import json
import re
from pathlib import Path


THEORY = Path(__file__).parents[1]
ALLOWED_STATUSES = {
    "cited",
    "proven-here",
    "computationally-verified",
    "defined",
    "proposed-for-ratification",
}


def _fixture_ids() -> set[str]:
    ids = set()
    for path in (THEORY / "fixtures").glob("*.json"):
        if path.name == "schema-v1.json":
            continue
        envelope = json.loads(path.read_text(encoding="utf-8"))
        ids.update(fixture["id"] for fixture in envelope["fixtures"])
    return ids


def test_findings_claims_statuses_and_fixture_references():
    findings = (THEORY / "FINDINGS.md").read_text(encoding="utf-8")

    for number in range(1, 12):
        assert re.search(rf"^## C{number}\b", findings, re.MULTILINE), f"missing C{number}"
    assert re.search(r"^### C8a\b", findings, re.MULTILINE)
    assert re.search(r"^### C9a\b", findings, re.MULTILINE)
    assert re.search(r"^### C11 lemma\b", findings, re.MULTILINE)
    for number in range(1, 4):
        assert re.search(rf"^## P{number}\b", findings, re.MULTILINE), f"missing P{number}"

    statuses = re.findall(r"^Status: `([^`]+)`$", findings, re.MULTILINE)
    assert len(statuses) >= 16
    assert set(statuses) <= ALLOWED_STATUSES

    cited = set(re.findall(r"`fixture:([a-z0-9-]+)`", findings))
    assert cited
    assert cited <= _fixture_ids(), f"unknown fixture ids: {sorted(cited - _fixture_ids())}"
