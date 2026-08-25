#!/usr/bin/env python3
"""A small, annotated protocol-v1 client suitable for copying and modifying."""

import argparse
import json
import random
import sys


parser = argparse.ArgumentParser()
parser.add_argument("--seed", type=int, required=True)
parser.add_argument("--fraction-ppb", type=int, default=100_000_000)
args = parser.parse_args()
if not 0 <= args.fraction_ppb <= 1_000_000_000:
    parser.error("--fraction-ppb must be between 0 and 1000000000")
rng = random.Random(args.seed)


def send(message):
    # Protocol output is one compact JSON object per line and must be flushed.
    print(json.dumps(message, separators=(",", ":")), flush=True)


for line in sys.stdin:
    request = json.loads(line)
    if request["type"] == "hello":
        # The hello tells us our canonical X/O seat for the whole game.
        send(
            {
                "type": "hello",
                "protocol": 1,
                "name": "fraction",
                "version": "1",
            }
        )
        continue
    if request["type"] == "game_end":
        break

    # The referee supplies the complete legal move list, so clients need no
    # rules implementation just to participate. A seeded RNG is reproducible.
    legal = request["legal"]
    move = legal[rng.randrange(len(legal))]

    # Money is integer-only. Parts per billion gives an exact fraction without
    # floating point; 1_000_000_000 ppb means the entire remaining budget.
    mine = request["budgets"][request["you"]]
    bid = mine * args.fraction_ppb // 1_000_000_000
    send(
        {
            "type": "turn",
            "protocol": 1,
            "request_id": request["request_id"],
            "bid": bid,
            "move": move,
        }
    )
