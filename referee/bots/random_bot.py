#!/usr/bin/env python3
import argparse
import json
import random
import sys


parser = argparse.ArgumentParser()
parser.add_argument("--seed", type=int, required=True)
args = parser.parse_args()
rng = random.Random(args.seed)

for line in sys.stdin:
    request = json.loads(line)
    if request["type"] == "hello":
        reply = {"type": "hello", "protocol": 1, "name": "random", "version": "1"}
    elif request["type"] == "turn":
        budget = request["budgets"][request["you"]]
        reply = {
            "type": "turn",
            "protocol": 1,
            "request_id": request["request_id"],
            "bid": rng.randrange(budget + 1),
            "move": request["legal"][rng.randrange(len(request["legal"]))],
        }
    else:
        break
    print(json.dumps(reply, separators=(",", ":")), flush=True)
