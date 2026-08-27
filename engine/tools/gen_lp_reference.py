#!/usr/bin/env python3
"""Generate the committed theory-certified ttt3 root-matrix reference."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from auction_ttt.continuous import solve as solve_continuous
from auction_ttt.discrete import solve as solve_discrete
from auction_ttt.lp_reference import emit_root_matrices


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()

    records = emit_root_matrices(solve_continuous(), {scale: solve_discrete(scale) for scale in (8, 16, 32)})
    payload = {
        "schema_version": 1,
        "provenance": {
            "generator": "engine/tools/gen_lp_reference.py",
            "command": (
                "cd theory && uv run python ../engine/tools/gen_lp_reference.py "
                "--out ../engine/tests/data/lp_reference_ttt3.json"
            ),
            "call": (
                "emit_root_matrices(solve_continuous(), "
                "{scale: solve_discrete(scale) for scale in (8, 16, 32)})"
            ),
            "source": "theory/auction_ttt/lp_reference.py",
        },
        "records": records,
    }
    rendered = json.dumps(payload, indent=2, sort_keys=True) + "\n"
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(rendered, encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
