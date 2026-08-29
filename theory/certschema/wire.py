"""DD §4/§7 byte builders. All integers LE; digests are canonical XXH3-128."""

from dataclasses import dataclass

from .game import State
from .xxh import xxh3_128

MAGIC_CERT = b"UTTTCERT"
MAGIC_VERDICT = b"UTTTVRDT"
SCHEMA = (0, 1)
RULESET_ID = "uttt-s2-r1"
RULESET_DIGEST = bytes.fromhex("16e85b36332493470fa1006516128579")
SYMMETRY_ID = "d4-lexmin-v1"
CLAIM = {"WIN_X": 0, "WIN_O": 1, "NOLOSS_X": 2, "NOLOSS_O": 3}
KIND = {"PROVER": 0, "OPPONENT": 1, "TERMINAL": 2, "RULE": 3}


def _u16(v):
    return v.to_bytes(2, "little")


def _u32(v):
    return v.to_bytes(4, "little")


def _u64(v):
    return v.to_bytes(8, "little")


def _str16(s: str) -> bytes:
    encoded = s.encode()
    return _u16(len(encoded)) + encoded


@dataclass(frozen=True)
class Record:
    state: State
    kind: int
    payload: int

    def encode(self) -> bytes:
        return self.state.serialize() + bytes([self.kind, self.payload, 0x00])


def build_certificate(claim: int, root: State, plies: dict) -> bytes:
    header = MAGIC_CERT + _u16(SCHEMA[0]) + _u16(SCHEMA[1])
    header += bytes([claim, 0, 0, 0])
    header += _str16(RULESET_ID) + RULESET_DIGEST + _str16(SYMMETRY_ID)
    header += root.serialize()
    header += _u16(0)
    totals = [0, 0, 0, 0]
    chunks = []
    for ply in sorted(plies, reverse=True):
        records = plies[ply]
        for record in records:
            totals[record.kind] += 1
        chunks.append(
            (
                ply,
                _u32(len(records))
                + _u32(0)
                + b"".join(record.encode() for record in records),
            )
        )
    header += b"".join(_u64(total) for total in totals) + _u32(len(chunks))
    header_len = len(header) + 40 * len(chunks) + 16
    rows = b""
    offset = header_len
    for ply, chunk in chunks:
        rows += bytes([ply, 0]) + _u16(0)
        rows += _u32((len(chunk) - 8) // 40)
        rows += _u64(offset) + _u64(len(chunk)) + xxh3_128(chunk)
        offset += len(chunk)
    return header + rows + xxh3_128(rows) + b"".join(chunk for _, chunk in chunks)


def header_region(cert: bytes) -> bytes:
    i = len(MAGIC_CERT) + 4 + 4
    ruleset_id_len = int.from_bytes(cert[i : i + 2], "little")
    i += 2 + ruleset_id_len + 16
    symmetry_id_len = int.from_bytes(cert[i : i + 2], "little")
    i += 2 + symmetry_id_len
    i += 37
    rule_count = int.from_bytes(cert[i : i + 2], "little")
    i += 2
    for _ in range(rule_count):
        rule_id_len = int.from_bytes(cert[i : i + 2], "little")
        i += 2 + rule_id_len + 2
    i += 32
    row_count = int.from_bytes(cert[i : i + 4], "little")
    i += 4
    first_offset = int.from_bytes(cert[i + 8 : i + 16], "little")
    assert first_offset == i + 40 * row_count + 16
    return cert[:first_offset]


def header_digest(cert: bytes) -> bytes:
    return xxh3_128(header_region(cert))


def build_verdict(game_value: int, root: State, members: list) -> bytes:
    verdict = MAGIC_VERDICT + _u16(SCHEMA[0]) + _u16(SCHEMA[1])
    verdict += bytes([game_value, 0, 0, 0])
    verdict += _str16(RULESET_ID) + RULESET_DIGEST + _str16(SYMMETRY_ID)
    verdict += root.serialize()
    verdict += bytes([len(members)])
    for claim, path, digest in members:
        verdict += bytes([claim]) + _str16(path) + digest
    return verdict
