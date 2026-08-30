"""Positive vectors and mutants for the DD §9 conformance corpus."""

from collections import namedtuple
from dataclasses import dataclass

from .game import ANY, State, apply_move, canonicalize, legal_moves, terminal
from .wire import (
    CLAIM,
    KIND,
    Record,
    build_certificate,
    build_verdict,
    header_digest,
)
from .xxh import xxh3_128


def _mk(xd: dict, od: dict, forced: int) -> State:
    x = [0] * 9
    o = [0] * 9
    for b, cells in xd.items():
        for c in cells:
            x[b] |= 1 << c
    for b, cells in od.items():
        for c in cells:
            o[b] |= 1 << c
    return State(x=tuple(x), o=tuple(o), forced=forced)


def golden_root() -> State:
    raw = _mk(
        {0: (0, 1, 2), 1: (0, 1, 2), 2: (0, 1, 2)},
        {3: (3, 5), 4: (1, 7), 5: (0, 8), 6: (4,), 7: (4,)},
        ANY,
    )
    return canonicalize(raw)[0]


def _prover_record(raw_parent: State, move: tuple) -> Record:
    canonical_parent, transform = canonicalize(raw_parent)
    board, cell = move
    stored = transform[board] * 9 + transform[cell]
    return Record(state=canonical_parent, kind=KIND["PROVER"], payload=stored)


def build_p1() -> bytes:
    root = golden_root()
    return build_certificate(
        CLAIM["WIN_X"], root, {17: [Record(root, KIND["TERMINAL"], 0xFF)]}
    )


def build_golden_verdict() -> bytes:
    root = golden_root()
    return build_verdict(
        0,
        root,
        [(CLAIM["WIN_X"], "golden-terminal-winx.utc", header_digest(build_p1()))],
    )


def p2_states():
    golden = golden_root()
    x = list(golden.x)
    x[8] &= ~(1 << 8)
    parent = State(x=tuple(x), o=golden.o, forced=8)
    return parent, (8, 8)


def build_p2() -> bytes:
    parent_raw, move = p2_states()
    child = canonicalize(apply_move(parent_raw, *move))[0]
    prover = _prover_record(parent_raw, move)
    assert terminal(child) == "X" and child == golden_root()
    return build_certificate(
        CLAIM["WIN_X"],
        prover.state,
        {16: [prover], 17: [Record(child, KIND["TERMINAL"], 0xFF)]},
    )


def p3_states():
    state = _mk(
        {
            4: (4,),
            5: (0, 1, 5, 6),
            6: (6, 7, 8),
            7: (6, 7, 8),
            8: (6, 7),
        },
        {
            0: (0, 1),
            1: (3, 4),
            2: (5, 6),
            3: (7, 8),
            5: (2, 3, 4, 7),
        },
        5,
    )
    return state, (5, 8), (8, 8)


def build_p3() -> bytes:
    s0, o_move, x_move = p3_states()
    assert legal_moves(s0) == [o_move] and terminal(s0) is None
    s1 = apply_move(s0, *o_move)
    s2 = apply_move(s1, *x_move)
    assert terminal(s2) == "X"
    opponent = Record(canonicalize(s0)[0], KIND["OPPONENT"], 0xFF)
    prover = _prover_record(s1, x_move)
    terminal_record = Record(canonicalize(s2)[0], KIND["TERMINAL"], 0xFF)
    return build_certificate(
        CLAIM["WIN_X"],
        opponent.state,
        {25: [opponent], 26: [prover], 27: [terminal_record]},
    )


def p4_states():
    state = _mk(
        {
            4: (4,),
            5: (0, 1, 5, 6),
            6: (6, 7, 8),
            7: (6, 7, 8),
            8: (6, 7),
        },
        {
            0: (0, 1),
            1: (3, 4),
            2: (5, 6),
            3: (7, 8),
            4: (0,),
            5: (2, 3, 4),
        },
        5,
    )
    return state, [(5, 7), (5, 8)], (8, 8)


def build_p4() -> bytes:
    s0, replies, x_move = p4_states()
    assert s0.counts() == (13, 12)
    assert legal_moves(s0) == replies and terminal(s0) is None

    opponent = Record(canonicalize(s0)[0], KIND["OPPONENT"], 0xFF)
    provers = []
    terminals = []
    canonical_states = [opponent.state]
    for reply in replies:
        parent = apply_move(s0, *reply)
        assert parent.well_formed() and parent.side_to_move() == "X"
        child = apply_move(parent, *x_move)
        assert terminal(child) == "X"
        prover = _prover_record(parent, x_move)
        terminal_record = Record(canonicalize(child)[0], KIND["TERMINAL"], 0xFF)
        provers.append(prover)
        terminals.append(terminal_record)
        canonical_states.extend((prover.state, terminal_record.state))
    assert len({state.serialize() for state in canonical_states}) == 5
    return build_certificate(
        CLAIM["WIN_X"],
        opponent.state,
        {25: [opponent], 26: provers, 27: terminals},
    )


def _u16(value: int) -> bytes:
    return value.to_bytes(2, "little")


def _u32(value: int) -> bytes:
    return value.to_bytes(4, "little")


def _u64(value: int) -> bytes:
    return value.to_bytes(8, "little")


def _str16(value: str) -> bytes:
    encoded = value.encode()
    return _u16(len(encoded)) + encoded


def _read_str16(blob: bytes, offset: int) -> tuple[str, int]:
    length = int.from_bytes(blob[offset : offset + 2], "little")
    offset += 2
    return blob[offset : offset + length].decode(), offset + length


def _state_from_bytes(blob: bytes) -> State:
    assert len(blob) == 37
    x = tuple(int.from_bytes(blob[i : i + 2], "little") for i in range(0, 18, 2))
    o = tuple(int.from_bytes(blob[i : i + 2], "little") for i in range(18, 36, 2))
    return State(x=x, o=o, forced=blob[36])


@dataclass
class _Chunk:
    fixed_count: int
    rule_count: int
    fixed_records: list[bytearray]
    rule_records: list[bytes]

    def encode(self) -> bytes:
        return (
            _u32(self.fixed_count)
            + _u32(self.rule_count)
            + b"".join(bytes(record) for record in self.fixed_records)
            + b"".join(self.rule_records)
        )


@dataclass
class _Row:
    ply: int
    flags: int
    ordinal: int
    record_count: int
    byte_offset: int
    byte_length: int
    chunk_digest: bytes
    chunk: _Chunk


@dataclass
class _Cert:
    magic: bytes
    schema_major: int
    schema_minor: int
    claim: int
    reserved: bytes
    ruleset_id: str
    ruleset_digest: bytes
    symmetry_id: str
    root_state: bytes
    rule_ids_used: list[tuple[str, int]]
    totals: list[int]
    rows: list[_Row]
    manifest_digest: bytes


def parse_cert(blob: bytes) -> _Cert:
    """Parse a well-framed certificate into a mutable emitter-side model."""
    offset = 0
    magic = blob[offset : offset + 8]
    offset += 8
    schema_major = int.from_bytes(blob[offset : offset + 2], "little")
    schema_minor = int.from_bytes(blob[offset + 2 : offset + 4], "little")
    offset += 4
    claim = blob[offset]
    reserved = blob[offset + 1 : offset + 4]
    offset += 4
    ruleset_id, offset = _read_str16(blob, offset)
    ruleset_digest = blob[offset : offset + 16]
    offset += 16
    symmetry_id, offset = _read_str16(blob, offset)
    root_state = blob[offset : offset + 37]
    offset += 37
    rule_count = int.from_bytes(blob[offset : offset + 2], "little")
    offset += 2
    rule_ids_used = []
    for _ in range(rule_count):
        rule_id, offset = _read_str16(blob, offset)
        version = int.from_bytes(blob[offset : offset + 2], "little")
        offset += 2
        rule_ids_used.append((rule_id, version))
    totals = [
        int.from_bytes(blob[offset + i * 8 : offset + (i + 1) * 8], "little")
        for i in range(4)
    ]
    offset += 32
    row_count = int.from_bytes(blob[offset : offset + 4], "little")
    offset += 4
    row_fields = []
    for _ in range(row_count):
        row_fields.append(
            (
                blob[offset],
                blob[offset + 1],
                int.from_bytes(blob[offset + 2 : offset + 4], "little"),
                int.from_bytes(blob[offset + 4 : offset + 8], "little"),
                int.from_bytes(blob[offset + 8 : offset + 16], "little"),
                int.from_bytes(blob[offset + 16 : offset + 24], "little"),
                blob[offset + 24 : offset + 40],
            )
        )
        offset += 40
    manifest_digest = blob[offset : offset + 16]
    rows = []
    for ply, flags, ordinal, record_count, byte_offset, byte_length, digest in row_fields:
        chunk_blob = blob[byte_offset : byte_offset + byte_length]
        fixed_count = int.from_bytes(chunk_blob[0:4], "little")
        chunk_rule_count = int.from_bytes(chunk_blob[4:8], "little")
        cursor = 8
        fixed_records = []
        for _ in range(fixed_count):
            fixed_records.append(bytearray(chunk_blob[cursor : cursor + 40]))
            cursor += 40
        rule_records = []
        for _ in range(chunk_rule_count):
            start = cursor
            cursor += 38
            rule_id_len = int.from_bytes(chunk_blob[cursor : cursor + 2], "little")
            cursor += 2 + rule_id_len + 2
            witness_len = int.from_bytes(chunk_blob[cursor : cursor + 4], "little")
            cursor += 4 + witness_len
            rule_records.append(chunk_blob[start:cursor])
        assert cursor == len(chunk_blob)
        rows.append(
            _Row(
                ply,
                flags,
                ordinal,
                record_count,
                byte_offset,
                byte_length,
                digest,
                _Chunk(fixed_count, chunk_rule_count, fixed_records, rule_records),
            )
        )
    return _Cert(
        magic,
        schema_major,
        schema_minor,
        claim,
        reserved,
        ruleset_id,
        ruleset_digest,
        symmetry_id,
        root_state,
        rule_ids_used,
        totals,
        rows,
        manifest_digest,
    )


def _count_kinds(cert: _Cert) -> list[int]:
    totals = [0, 0, 0, 0]
    for row in cert.rows:
        for record in row.chunk.fixed_records:
            kind = record[37]
            if 0 <= kind <= 3:
                totals[kind] += 1
        totals[KIND["RULE"]] += len(row.chunk.rule_records)
    return totals


def _header_prefix(cert: _Cert, totals: list[int], row_count: int) -> bytes:
    prefix = cert.magic + _u16(cert.schema_major) + _u16(cert.schema_minor)
    prefix += bytes([cert.claim]) + cert.reserved
    prefix += _str16(cert.ruleset_id) + cert.ruleset_digest + _str16(cert.symmetry_id)
    prefix += cert.root_state + _u16(len(cert.rule_ids_used))
    for rule_id, version in cert.rule_ids_used:
        prefix += _str16(rule_id) + _u16(version)
    return prefix + b"".join(_u64(total) for total in totals) + _u32(row_count)


def rebuild(cert: _Cert, *, skip: set[str] | None = None) -> bytes:
    """Rebuild and re-seal a parsed certificate, preserving named target defects."""
    skip = set() if skip is None else skip
    totals = cert.totals if "totals" in skip else _count_kinds(cert)
    prefix = _header_prefix(cert, totals, len(cert.rows))
    next_offset = len(prefix) + 40 * len(cert.rows) + 16
    rows_blob = bytearray()
    chunks = []
    for row in cert.rows:
        chunk = row.chunk.encode()
        chunks.append(chunk)
        byte_offset = row.byte_offset if "row_offset" in skip else next_offset
        digest = row.chunk_digest if "chunk_digest" in skip else xxh3_128(chunk)
        rows_blob += bytes([row.ply, row.flags]) + _u16(row.ordinal)
        rows_blob += _u32(row.record_count) + _u64(byte_offset) + _u64(len(chunk)) + digest
        next_offset += len(chunk)
    manifest_digest = (
        cert.manifest_digest
        if "manifest_digest" in skip
        else xxh3_128(bytes(rows_blob))
    )
    return prefix + bytes(rows_blob) + manifest_digest + b"".join(chunks)


def reseal_cert(blob: bytes, *, skip: set[str]) -> bytes:
    return rebuild(parse_cert(blob), skip=skip)


def alt_root() -> State:
    raw = _mk(
        {0: (0, 1, 2), 1: (0, 1, 2), 2: (0, 1, 2)},
        {3: (3, 5), 4: (1, 7), 5: (0, 8), 6: (5,), 7: (4,)},
        ANY,
    )
    alternate = canonicalize(raw)[0]
    assert alternate.counts() == (9, 8)
    assert alternate.well_formed()
    assert canonicalize(alternate)[0] == alternate
    assert alternate != golden_root()
    return alternate


def append_orphan_terminal(blob: bytes) -> bytes:
    """Append a distinct, valid terminal that is not referenced by any edge."""

    def edit(cert):
        row = next(row for row in cert.rows if row.ply == 17)
        orphan = Record(alt_root(), KIND["TERMINAL"], 0xFF).encode()
        assert all(bytes(record) != orphan for record in row.chunk.fixed_records)
        row.chunk.fixed_records.append(bytearray(orphan))
        row.chunk.fixed_count += 1
        row.record_count += 1

    return _model_mutation(blob, edit)


def _patch(blob: bytes, offset: int, value: int) -> bytes:
    edited = bytearray(blob)
    edited[offset] = value
    return bytes(edited)


def _first_fixed(cert: _Cert, *, kind: int | None = None) -> tuple[_Row, bytearray]:
    for row in cert.rows:
        for record in row.chunk.fixed_records:
            if kind is None or record[37] == kind:
                return row, record
    raise AssertionError("record not found")


def _model_mutation(blob: bytes, edit, *, skip: set[str] | None = None) -> bytes:
    cert = parse_cert(blob)
    edit(cert)
    return rebuild(cert, skip=set() if skip is None else skip)


def _h07(blob: bytes) -> bytes:
    def edit(cert):
        root = _state_from_bytes(cert.root_state)
        transformed = canonicalize(root)[0]
        from .game import D4, transform

        noncanonical = transform(transformed, D4["r90"])
        assert noncanonical != transformed and canonicalize(noncanonical)[0] == transformed
        cert.root_state = noncanonical.serialize()

    return _model_mutation(blob, edit)


def _h08(blob: bytes) -> bytes:
    return _model_mutation(
        blob, lambda cert: cert.rule_ids_used.append(("claimpair", 1))
    )


def _h09(blob: bytes) -> bytes:
    cert = parse_cert(blob)
    return _header_prefix(cert, _count_kinds(cert), 0)


def _h10(blob: bytes) -> bytes:
    def edit(cert):
        assert [row.ply for row in cert.rows] == [27, 26, 25]
        cert.rows[0], cert.rows[1] = cert.rows[1], cert.rows[0]

    mutant = _model_mutation(blob, edit)
    parsed = parse_cert(mutant)
    assert [row.ply for row in parsed.rows] == [26, 27, 25]
    for row in parsed.rows:
        assert xxh3_128(row.chunk.encode()) == row.chunk_digest
    return mutant


def _h11(blob: bytes) -> bytes:
    def edit(cert):
        cert.rows[0].byte_offset += 1

    return _model_mutation(blob, edit, skip={"row_offset"})


def _h12(blob: bytes) -> bytes:
    def edit(cert):
        cert.manifest_digest = bytes([cert.manifest_digest[0] ^ 0xFF]) + cert.manifest_digest[1:]

    return _model_mutation(blob, edit, skip={"manifest_digest"})


def _h13(blob: bytes) -> bytes:
    return _model_mutation(blob, lambda cert: setattr(cert.rows[0], "flags", 1))


def _h14(blob: bytes) -> bytes:
    return _model_mutation(blob, lambda cert: setattr(cert.rows[0], "ply", 81))


def _r01(blob: bytes) -> bytes:
    def edit(cert):
        row, record = _first_fixed(cert)
        row.chunk.fixed_records.append(bytearray(record))
        row.chunk.fixed_count += 1
        row.record_count += 1

    return _model_mutation(blob, edit)


def _r02(blob: bytes) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert)
        from .game import D4, transform

        state = _state_from_bytes(record[:37])
        transformed = transform(state, D4["r90"])
        assert transformed != state
        record[:37] = transformed.serialize()

    return _model_mutation(blob, edit)


def _r03(blob: bytes) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert)
        state = _state_from_bytes(record[:37])
        board = next(i for i, mask in enumerate(state.x) if mask)
        bit = state.x[board] & -state.x[board]
        o = list(state.o)
        o[board] |= bit
        malformed = State(state.x, tuple(o), state.forced)
        assert not malformed.well_formed()
        record[:37] = malformed.serialize()

    return _model_mutation(blob, edit)


def _r04(blob: bytes) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert)
        record[36] = 6
        assert not _state_from_bytes(record[:37]).well_formed()

    return _model_mutation(blob, edit)


def _r05(blob: bytes) -> bytes:
    return _model_mutation(blob, lambda cert: setattr(cert.rows[0], "ply", 16))


def _r06(blob: bytes) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert)
        record[39] = 1

    return _model_mutation(blob, edit)


def _r07(blob: bytes) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert, kind=KIND["TERMINAL"])
        record[38] = 0

    return _model_mutation(blob, edit)


def _r08(blob: bytes) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert, kind=KIND["PROVER"])
        state = _state_from_bytes(record[:37])
        board = record[38] // 9
        occupied = state.x[board] | state.o[board]
        cell = next(cell for cell in range(9) if occupied >> cell & 1)
        record[38] = board * 9 + cell

    return _model_mutation(blob, edit)


def _drop_ply(blob: bytes, ply: int) -> bytes:
    def edit(cert):
        cert.rows = [row for row in cert.rows if row.ply != ply]
        assert len(cert.rows) > 0

    return _model_mutation(blob, edit)


def _r11(blob: bytes) -> bytes:
    return _model_mutation(blob, lambda cert: setattr(cert, "claim", CLAIM["WIN_O"]))


def _change_kind(blob: bytes, old: int, new: int) -> bytes:
    def edit(cert):
        _row, record = _first_fixed(cert, kind=old)
        record[37] = new
        if new != KIND["PROVER"]:
            record[38] = 0xFF

    return _model_mutation(blob, edit)


def _rule_record(state: State) -> bytes:
    witness = b"\x00"
    return (
        state.serialize()
        + bytes([KIND["RULE"]])
        + _str16("claimpair")
        + _u16(1)
        + _u32(len(witness))
        + witness
    )


def _r14(blob: bytes) -> bytes:
    def edit(cert):
        row = cert.rows[0]
        row.chunk.rule_records.append(_rule_record(alt_root()))
        row.chunk.rule_count += 1
        row.record_count += 1

    return _model_mutation(blob, edit)


def _r15(blob: bytes) -> bytes:
    def edit(cert):
        row = cert.rows[0]
        row.chunk_digest = bytes([row.chunk_digest[0] ^ 0xFF]) + row.chunk_digest[1:]

    return _model_mutation(blob, edit, skip={"chunk_digest"})


def _r17(blob: bytes) -> bytes:
    def edit(cert):
        row = cert.rows[0]
        # Keep the chunk fully parseable: the sole target is the manifest's
        # record_count (1) disagreeing with fixed_count + rule_count (2).
        row.chunk.fixed_records.append(
            bytearray(Record(alt_root(), KIND["TERMINAL"], 0xFF).encode())
        )
        row.chunk.fixed_count = 2

    return _model_mutation(blob, edit)


def _r18(blob: bytes) -> bytes:
    def edit(cert):
        cert.totals[KIND["TERMINAL"]] = 2

    return _model_mutation(blob, edit, skip={"totals"})


def _r19(blob: bytes) -> bytes:
    return _model_mutation(
        blob, lambda cert: setattr(cert, "root_state", alt_root().serialize())
    )


def _r22(blob: bytes) -> bytes:
    s0, _replies, _x_move = p4_states()
    missing_parent = canonicalize(apply_move(s0, 5, 8))[0].serialize()

    def edit(cert):
        row = next(row for row in cert.rows if row.ply == 26)
        matches = [
            record for record in row.chunk.fixed_records if record[:37] == missing_parent
        ]
        assert len(matches) == 1
        row.chunk.fixed_records.remove(matches[0])
        row.chunk.fixed_count -= 1
        row.record_count -= 1

    return _model_mutation(blob, edit)


def _parse_verdict(blob: bytes) -> tuple[int, State, list[tuple[int, str, bytes]]]:
    offset = 8 + 4
    game_value = blob[offset]
    offset += 4
    _ruleset_id, offset = _read_str16(blob, offset)
    offset += 16
    _symmetry_id, offset = _read_str16(blob, offset)
    root = _state_from_bytes(blob[offset : offset + 37])
    offset += 37
    count = blob[offset]
    offset += 1
    members = []
    for _ in range(count):
        claim = blob[offset]
        path, offset = _read_str16(blob, offset + 1)
        digest = blob[offset : offset + 16]
        offset += 16
        members.append((claim, path, digest))
    assert offset == len(blob)
    return game_value, root, members


def _verdict_edit(blob: bytes, *, game_value=None, root=None, member_edit=None) -> bytes:
    old_value, old_root, members = _parse_verdict(blob)
    if member_edit is not None:
        members = member_edit(members)
    return build_verdict(
        old_value if game_value is None else game_value,
        old_root if root is None else root,
        members,
    )


def _v02(blob: bytes) -> bytes:
    return _verdict_edit(
        blob, member_edit=lambda rows: [(rows[0][0], "../x.utc", rows[0][2])]
    )


def _v03(blob: bytes) -> bytes:
    return _verdict_edit(
        blob,
        member_edit=lambda rows: [
            (rows[0][0], "golden-terminal-abset.utc", rows[0][2])
        ],
    )


def _v04(blob: bytes) -> bytes:
    def edit(rows):
        claim, path, digest = rows[0]
        return [(claim, path, bytes([digest[0] ^ 0xFF]) + digest[1:])]

    return _verdict_edit(blob, member_edit=edit)


def _v05(blob: bytes) -> bytes:
    return _verdict_edit(
        blob,
        member_edit=lambda rows: [(CLAIM["NOLOSS_X"], rows[0][1], rows[0][2])],
    )


def _v06(blob: bytes) -> bytes:
    return _verdict_edit(
        blob, member_edit=lambda rows: [(9, rows[0][1], rows[0][2])]
    )


MutantSpec = namedtuple("MutantSpec", "base dd_ref description mutate")


MUTANTS = {
    "MUT-H01": MutantSpec("p1", "§6.1", "bad certificate magic", lambda b: _patch(b, 0, 0x56)),
    "MUT-H02": MutantSpec("p1", "§6.1", "unsupported schema minor", lambda b: b[:10] + _u16(9) + b[12:]),
    "MUT-H03": MutantSpec("p1", "§6.1", "unknown claim tag", lambda b: _patch(b, 12, 7)),
    "MUT-H04": MutantSpec("p1", "§6.1", "nonzero header reserved byte", lambda b: _patch(b, 13, 1)),
    "MUT-H05": MutantSpec("p1", "§6.1", "ruleset digest mismatch", lambda b: _patch(b, 28, b[28] ^ 0xFF)),
    "MUT-H06": MutantSpec("p1", "§6.1", "unknown symmetry id", lambda b: b[:46] + b"d4-lexmin-v9" + b[58:]),
    "MUT-H07": MutantSpec("p1", "§6.1", "non-canonical root state", _h07),
    "MUT-H08": MutantSpec("p1", "§6.1", "nonempty v0 rule registry", _h08),
    "MUT-H09": MutantSpec("p1", "§6.1", "zero manifest rows", _h09),
    "MUT-H10": MutantSpec("p3", "§4.3/§6.1", "descending manifest order violation", _h10),
    "MUT-H11": MutantSpec("p3", "§4.3/§6.1", "first chunk offset gap", _h11),
    "MUT-H12": MutantSpec("p1", "§6.1", "manifest digest mismatch", _h12),
    "MUT-H13": MutantSpec("p1", "§4.3/§6.1", "nonzero manifest flags", _h13),
    "MUT-H14": MutantSpec("p1", "§4.3/§6.1", "manifest ply above 80", _h14),
    "MUT-R01": MutantSpec("p1", "§6.4", "duplicate canonical state", _r01),
    "MUT-R02": MutantSpec("p1", "§6.4", "non-canonical record state", _r02),
    "MUT-R03": MutantSpec("p1", "§2.6/§6.4", "overlapping X and O masks", _r03),
    "MUT-R04": MutantSpec("p1", "§2.6/§6.4", "forced board is closed", _r04),
    "MUT-R05": MutantSpec("p1", "§6.4", "record mark count differs from chunk ply", _r05),
    "MUT-R06": MutantSpec("p1", "§4.2/§6.4", "nonzero record reserved byte", _r06),
    "MUT-R07": MutantSpec("p1", "§4.2/§6.4", "non-PROVER payload is not 0xFF", _r07),
    "MUT-R08": MutantSpec("p2", "§3.2/§6.4", "illegal occupied PROVER move", _r08),
    "MUT-R09": MutantSpec("p2", "§3.2/§6.4", "PROVER child absent", lambda b: _drop_ply(b, 17)),
    "MUT-R10": MutantSpec("p3", "§3.2/§6.4", "OPPONENT reply missing", lambda b: _drop_ply(b, 26)),
    "MUT-R11": MutantSpec("p1", "§3.2/§6.4", "terminal does not satisfy WIN_O", _r11),
    "MUT-R12": MutantSpec("p2", "§3.2/§6.4", "non-terminal state marked TERMINAL", lambda b: _change_kind(b, KIND["PROVER"], KIND["TERMINAL"])),
    "MUT-R13": MutantSpec("p2", "§3.2/§6.4", "kind inconsistent with side to move", lambda b: _change_kind(b, KIND["PROVER"], KIND["OPPONENT"])),
    "MUT-R14": MutantSpec("p1", "§5/§6.4", "RULE record in a v0 stream", _r14),
    "MUT-R15": MutantSpec("p1", "§4.3/§6.4", "chunk digest mismatch", _r15),
    "MUT-R16": MutantSpec("p1", "§4.3/§6.4", "trailing bytes", lambda b: b + bytes.fromhex("deadbeef")),
    "MUT-R17": MutantSpec("p1", "§4.3/§6.4", "manifest record_count != fixed_count + rule_count", _r17),
    "MUT-R18": MutantSpec("p1", "§4.3/§6.4", "streamed totals mismatch", _r18),
    "MUT-R19": MutantSpec("p1", "§6.4", "declared root absent", _r19),
    "MUT-R20": MutantSpec("p1", "§4.2/§6.4", "RULE kind in fixed section", lambda b: _change_kind(b, KIND["TERMINAL"], KIND["RULE"])),
    "MUT-R21": MutantSpec("p1", "§3.2/§6.4", "terminal state marked OPPONENT", lambda b: _change_kind(b, KIND["TERMINAL"], KIND["OPPONENT"])),
    "MUT-R22": MutantSpec("p4", "§3.2/§6.4", "OPPONENT reply child missing (non-first reply)", _r22),
    "MUT-V01": MutantSpec("verdict", "§7", "bad verdict magic", lambda b: _patch(b, 0, 0x56)),
    "MUT-V02": MutantSpec("verdict", "§7/§4.5", "parent path segment", _v02),
    "MUT-V03": MutantSpec("verdict", "§7.2", "member target absent", _v03),
    "MUT-V04": MutantSpec("verdict", "§7.4", "member header digest mismatch", _v04),
    "MUT-V05": MutantSpec("verdict", "§7.5", "member claim mismatch", _v05),
    "MUT-V06": MutantSpec("verdict", "§7.1", "unknown member claim tag", _v06),
    "MUT-V07": MutantSpec("verdict", "§7.6", "member and manifest roots differ", lambda b: _verdict_edit(b, root=alt_root())),
    "MUT-V08": MutantSpec("verdict", "§7.7", "draw completeness multiset violation", lambda b: _verdict_edit(b, game_value=1)),
}


_BASE_BUILDERS = {
    "p1": build_p1,
    "p2": build_p2,
    "p3": build_p3,
    "p4": build_p4,
    "verdict": build_golden_verdict,
}


def build_mutant(mut_id: str) -> bytes:
    spec = MUTANTS[mut_id]
    base = _BASE_BUILDERS[spec.base]()
    if spec.base != "verdict":
        assert rebuild(parse_cert(base)) == base
    mutant = spec.mutate(base)
    assert mutant != base
    if mut_id in {"MUT-V04", "MUT-V05", "MUT-V07", "MUT-V08"}:
        assert _parse_verdict(mutant)[2][0][1] == "golden-terminal-winx.utc"
    return mutant


def catalogue() -> dict:
    rows = [
        {"id": "P1", "file": "vectors/golden-terminal-winx.utc", "kind": "positive", "dd_ref": "Appendix B"},
        {"id": "P2", "file": "vectors/p2-prover-winx.utc", "kind": "positive", "dd_ref": "§3.2 PROVER"},
        {"id": "P3", "file": "vectors/p3-opponent-winx.utc", "kind": "positive", "dd_ref": "§3.2 OPPONENT"},
        {"id": "P4", "file": "vectors/p4-opponent2-winx.utc", "kind": "positive", "dd_ref": "§3.2 OPPONENT"},
        {"id": "V1", "file": "vectors/golden-winx.utv", "kind": "positive-verdict-subgame", "dd_ref": "Appendix B"},
        {"id": "GATE-01", "file": None, "kind": "gate", "dd_ref": "§7/§9", "note": "deliverable-mode root predicate; unit-level in c3"},
        {"id": "GATE-02", "file": "vectors/golden-winx.utv", "kind": "gate", "dd_ref": "§7/§9", "note": "must pass subgame mode, fail deliverable mode"},
    ]
    for mutant_id, spec in sorted(MUTANTS.items()):
        rows.append(
            {
                "id": mutant_id,
                "file": f"mutants/{mutant_id}.bin",
                "kind": "mutant",
                "base": spec.base,
                "dd_ref": spec.dd_ref,
                "expected": "reject",
                "reason": spec.description,
            }
        )
    return {
        "schema": "certschema-catalogue-v0",
        "ruleset_id": "uttt-s2-r1",
        "vectors": rows,
    }
