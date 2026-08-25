#!/usr/bin/env python3
import argparse
import json
import os
import random
import sys
import threading
import time


def write_line(obj):
    data = json.dumps(obj, sort_keys=True, separators=(",", ":")).encode() + b"\n"
    sys.stdout.buffer.write(data)
    sys.stdout.buffer.flush()


def delayed_write(data, delay=0.08):
    def worker():
        time.sleep(delay)
        sys.stdout.buffer.write(data)
        sys.stdout.buffer.flush()

    threading.Thread(target=worker, daemon=False).start()


parser = argparse.ArgumentParser()
parser.add_argument("--fault", action="append", default=[])
parser.add_argument("--bid", default="0")
parser.add_argument("--seed", type=int, default=1)
parser.add_argument("--spam-stderr", type=int, default=0)
parser.add_argument("--exit-delay-ms", type=int, default=0)
args = parser.parse_args()
faults = {}
for item in args.fault:
    name, _, when = item.partition(":")
    faults.setdefault(int(when or 1), []).append(name)

if args.spam_stderr:
    sys.stderr.buffer.write(b"e" * args.spam_stderr)
    sys.stderr.buffer.flush()

rng = random.Random(args.seed)
turn_number = 0
hello_number = 0
for raw in sys.stdin.buffer:
    request = json.loads(raw)
    if request["type"] == "hello":
        hello_number += 1
        if "hello_timeout" in faults.get(hello_number, []):
            time.sleep(10)
            continue
        if "hello_die" in faults.get(hello_number, []):
            os._exit(2)
        write_line({"type": "hello", "protocol": 1, "name": "stub", "version": "1"})
        continue
    if request["type"] == "game_end":
        time.sleep(args.exit_delay_ms / 1000)
        break

    turn_number += 1
    active = faults.get(turn_number, [])
    if "timeout" in active:
        time.sleep(10)
        continue
    if "die" in active:
        os._exit(3)
    if "oversize_nolf" in active:
        sys.stdout.buffer.write(b"x" * 40000)
        sys.stdout.buffer.flush()
        time.sleep(10)
        continue
    if "invalid_utf8" in active:
        sys.stdout.buffer.write(b"\xff\n")
        sys.stdout.buffer.flush()
        continue
    if "bad_json" in active:
        sys.stdout.buffer.write(b"{\n")
        sys.stdout.buffer.flush()
        continue

    legal = request["legal"]
    move = legal[rng.randrange(len(legal))]
    bid = request["budgets"][request["you"]] if args.bid == "all_in" else int(args.bid)
    reply = {
        "type": "turn",
        "protocol": 1,
        "request_id": request["request_id"],
        "bid": bid,
        "move": move,
    }
    if "wrong_id" in active:
        reply["request_id"] += "-wrong"
    if "schema" in active:
        reply.pop("bid")
    if "illegal_bid" in active:
        reply["bid"] = request["budgets"][request["you"]] + 1
    if "illegal_move" in active:
        reply["move"] = [8, 8] if [8, 8] not in legal else [7, 7]

    write_line(reply)
    if "extra_line" in active or "extra_line_before_sweep" in active:
        write_line(reply)
    if "unsolicited_between_plies" in active:
        delayed_write(b'{"unsolicited":true}\n')
    if "partial_between_plies" in active:
        delayed_write(b"garbage-no-newline")
