"""Deterministic corpus generator. Usage: python -m certschema.generate [--out DIR]."""

import argparse
import json
import pathlib

from .vectors import (
    MUTANTS,
    build_golden_verdict,
    build_mutant,
    build_p1,
    build_p2,
    build_p3,
    build_p4,
    catalogue,
)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", default=str(pathlib.Path(__file__).parent))
    out = pathlib.Path(parser.parse_args().out)
    vectors = out / "vectors"
    mutants = out / "mutants"
    vectors.mkdir(parents=True, exist_ok=True)
    mutants.mkdir(parents=True, exist_ok=True)
    (vectors / "golden-terminal-winx.utc").write_bytes(build_p1())
    (vectors / "golden-winx.utv").write_bytes(build_golden_verdict())
    (vectors / "p2-prover-winx.utc").write_bytes(build_p2())
    (vectors / "p3-opponent-winx.utc").write_bytes(build_p3())
    (vectors / "p4-opponent2-winx.utc").write_bytes(build_p4())
    for mutant_id in MUTANTS:
        (mutants / f"{mutant_id}.bin").write_bytes(build_mutant(mutant_id))
    # Verdict members resolve relative to the manifest location. Keep the valid
    # golden certificate adjacent to verdict mutants so V04/V05/V07/V08 reach
    # their named check; V03 deliberately names the absent "abset" path.
    (mutants / "golden-terminal-winx.utc").write_bytes(build_p1())
    (out / "catalogue.json").write_text(
        json.dumps(catalogue(), indent=2, sort_keys=True) + "\n"
    )


if __name__ == "__main__":
    main()
