#!/usr/bin/env python3
import argparse
import json
import random
import sys


LINES = (
    (0, 1, 2),
    (3, 4, 5),
    (6, 7, 8),
    (0, 3, 6),
    (1, 4, 7),
    (2, 5, 8),
    (0, 4, 8),
    (2, 4, 6),
)


def wins(cells, mark):
    return any(all(cells[index] == mark for index in line) for line in LINES)


def tactical(request, move):
    local, cell = move
    cells = list(request["board"][local])
    cells[cell] = request["you"]
    return wins(cells, request["you"])


parser = argparse.ArgumentParser()
parser.add_argument("--seed", type=int, required=True)
args = parser.parse_args()
rng = random.Random(args.seed)

for line in sys.stdin:
    request = json.loads(line)
    if request["type"] == "hello":
        reply = {
            "type": "hello",
            "protocol": 1,
            "name": "allin-tactical",
            "version": "1",
        }
    elif request["type"] == "turn":
        candidates = [move for move in request["legal"] if tactical(request, move)]
        move = (
            candidates[rng.randrange(len(candidates))]
            if candidates
            else request["legal"][rng.randrange(len(request["legal"]))]
        )
        reply = {
            "type": "turn",
            "protocol": 1,
            "request_id": request["request_id"],
            "bid": request["budgets"][request["you"]] if candidates else 0,
            "move": move,
        }
    else:
        break
    print(json.dumps(reply, separators=(",", ":")), flush=True)
