module checker::wire;

import checker::game;
import checker::xxh3;
import std::collections::list;
import std::core::bitorder;
import std::io;

const String RULESET_ID = "uttt-s2-r1";
const String SYMMETRY_ID = "d4-lexmin-v1";
const char[16] RULESET_DIGEST = x'16e85b36332493470fa1006516128579';

struct ManifestRow
{
    char ply;
    char flags;
    ushort ordinal;
    uint record_count;
    ulong byte_offset;
    ulong byte_length;
    char[16] chunk_digest;
    sz row_offset;
}

struct Header
{
    bool ok;
    String code;
    long fail_offset;
    ushort schema_major;
    ushort schema_minor;
    char claim;
    String ruleset_id;
    char[16] ruleset_digest;
    String symmetry_id;
    char[37] root;
    ulong[4] totals;
    List { ManifestRow } rows;
    sz manifest_start;
    sz header_end;
    char[16] header_digest;
}

struct Reader
{
    char[] blob;
    sz position;
}

fn Header fail(Header header, String code, sz offset)
{
    header.ok = false;
    header.code = code;
    header.fail_offset = (long)offset;
    return header;
}

fn bool available(Reader* reader, sz count)
{
    return count <= reader.blob.len - reader.position;
}

fn ushort? read_u16(Reader* reader)
{
    if (!available(reader, 2)) return io::EOF~;
    ushort value = bitorder::read(reader.blob[reader.position:2], UShortLE);
    reader.position += 2;
    return value;
}

fn uint? read_u32(Reader* reader)
{
    if (!available(reader, 4)) return io::EOF~;
    uint value = bitorder::read(reader.blob[reader.position:4], UIntLE);
    reader.position += 4;
    return value;
}

fn ulong? read_u64(Reader* reader)
{
    if (!available(reader, 8)) return io::EOF~;
    ulong value = bitorder::read(reader.blob[reader.position:8], ULongLE);
    reader.position += 8;
    return value;
}

fn bool read_ascii16(Reader* reader, String* output)
{
    if (!available(reader, 2)) return false;
    ushort length = read_u16(reader)!!;
    if (length < 1 || length > 64 || !available(reader, length)) return false;
    char[] bytes = reader.blob[reader.position:length];
    foreach (byte : bytes) if (byte < 0x21 || byte > 0x7e) return false;
    reader.position += length;
    *output = (String)bytes;
    return true;
}

fn bool same_state(game::State left, game::State right)
{
    char[37] left_bytes = left.serialize();
    char[37] right_bytes = right.serialize();
    return left_bytes[..] == right_bytes[..];
}

fn Header parse_header(char[] blob)
{
    Header header = { .ok = false, .code = "E_MAGIC", .fail_offset = 0 };
    Reader reader = { blob, 0 };

    if (!available(&reader, 8)) return fail(header, "E_MAGIC", 0);
    if ((String)blob[:8] != "UTTTCERT") return fail(header, "E_MAGIC", 0);
    reader.position = 8;

    sz field_offset = reader.position;
    if (!available(&reader, 4)) return fail(header, "E_SCHEMA_VERSION", field_offset);
    header.schema_major = read_u16(&reader)!!;
    header.schema_minor = read_u16(&reader)!!;
    if (header.schema_major != 0 || header.schema_minor != 1)
    {
        return fail(header, "E_SCHEMA_VERSION", field_offset);
    }

    field_offset = reader.position;
    if (!available(&reader, 1)) return fail(header, "E_CLAIM_TAG", field_offset);
    header.claim = blob[reader.position++];
    if (header.claim > 3) return fail(header, "E_CLAIM_TAG", field_offset);

    field_offset = reader.position;
    if (!available(&reader, 3)) return fail(header, "E_HEADER_RESERVED", field_offset);
    for (sz i = 0; i < 3; i++)
    {
        if (blob[reader.position + i] != 0) return fail(header, "E_HEADER_RESERVED", reader.position + i);
    }
    reader.position += 3;

    field_offset = reader.position;
    if (!read_ascii16(&reader, &header.ruleset_id)) return fail(header, "E_RULESET", field_offset);
    if (header.ruleset_id != RULESET_ID) return fail(header, "E_RULESET", field_offset);
    if (!available(&reader, 16)) return fail(header, "E_RULESET", reader.position);
    header.ruleset_digest[..] = blob[reader.position:16];
    if (header.ruleset_digest[..] != RULESET_DIGEST[..]) return fail(header, "E_RULESET", reader.position);
    reader.position += 16;

    field_offset = reader.position;
    if (!read_ascii16(&reader, &header.symmetry_id)) return fail(header, "E_SYMMETRY", field_offset);
    if (header.symmetry_id != SYMMETRY_ID) return fail(header, "E_SYMMETRY", field_offset);

    field_offset = reader.position;
    if (!available(&reader, 37)) return fail(header, "E_ROOT_STATE", field_offset);
    header.root[..] = blob[reader.position:37];
    reader.position += 37;
    game::State root = game::deserialize(header.root);
    if (!root.well_formed() || !same_state(root, game::canonical(root)))
    {
        return fail(header, "E_ROOT_STATE", field_offset);
    }

    field_offset = reader.position;
    if (!available(&reader, 2)) return fail(header, "E_RULE_IDS_V0", field_offset);
    ushort rule_count = read_u16(&reader)!!;
    if (rule_count != 0) return fail(header, "E_RULE_IDS_V0", field_offset);

    field_offset = reader.position;
    if (!available(&reader, 32)) return fail(header, "E_CHUNK_LAYOUT", field_offset);
    for (sz i = 0; i < 4; i++)
    {
        header.totals[i] = read_u64(&reader)!!;
    }

    field_offset = reader.position;
    if (!available(&reader, 4)) return fail(header, "E_MANIFEST_EMPTY", field_offset);
    uint row_count = read_u32(&reader)!!;
    if (row_count == 0) return fail(header, "E_MANIFEST_EMPTY", field_offset);
    if ((ulong)row_count * 40 + 16 > (ulong)(blob.len - reader.position))
    {
        return fail(header, "E_CHUNK_LAYOUT", field_offset);
    }

    header.rows.tinit(row_count);
    header.manifest_start = reader.position;
    for (uint i = 0; i < row_count; i++)
    {
        ManifestRow row = { .row_offset = reader.position };
        row.ply = blob[reader.position++];
        row.flags = blob[reader.position++];
        row.ordinal = read_u16(&reader)!!;
        row.record_count = read_u32(&reader)!!;
        row.byte_offset = read_u64(&reader)!!;
        row.byte_length = read_u64(&reader)!!;
        row.chunk_digest[..] = blob[reader.position:16];
        reader.position += 16;
        header.rows.push(row);
    }

    for (sz i = 0; i < header.rows.size; i++)
    {
        if (header.rows[i].flags != 0) return fail(header, "E_ROW_FLAGS", header.rows[i].row_offset + 1);
    }
    for (sz i = 0; i < header.rows.size; i++)
    {
        if (header.rows[i].ply > 81) return fail(header, "E_ROW_PLY_RANGE", header.rows[i].row_offset);
    }
    for (sz i = 0; i < header.rows.size; i++)
    {
        ManifestRow row = header.rows[i];
        if (i == 0)
        {
            if (row.ordinal != 0) return fail(header, "E_ROW_ORDER", row.row_offset + 2);
            continue;
        }
        ManifestRow previous = header.rows[i - 1];
        if (row.ply > previous.ply) return fail(header, "E_ROW_ORDER", row.row_offset);
        if (row.ply == previous.ply && row.ordinal != previous.ordinal + 1)
        {
            return fail(header, "E_ROW_ORDER", row.row_offset + 2);
        }
        if (row.ply < previous.ply && row.ordinal != 0)
        {
            return fail(header, "E_ROW_ORDER", row.row_offset + 2);
        }
    }

    sz digest_offset = reader.position;
    reader.position += 16;
    header.header_end = reader.position;
    ulong expected_offset = (ulong)header.header_end;
    for (sz i = 0; i < header.rows.size; i++)
    {
        ManifestRow row = header.rows[i];
        if (row.byte_offset != expected_offset) return fail(header, "E_CHUNK_LAYOUT", row.row_offset + 8);
        if (row.byte_length > ulong::max - row.byte_offset) return fail(header, "E_CHUNK_LAYOUT", row.row_offset + 16);
        expected_offset = row.byte_offset + row.byte_length;
    }
    if (expected_offset != blob.len) return fail(header, "E_CHUNK_LAYOUT", digest_offset);

    char[16] manifest_digest = xxh3::hash128(blob[header.manifest_start:digest_offset - header.manifest_start]);
    if (manifest_digest[..] != blob[digest_offset:16]) return fail(header, "E_MANIFEST_DIGEST", digest_offset);

    header.header_digest = xxh3::hash128(blob[:header.header_end]);
    header.ok = true;
    header.code = "OK";
    header.fail_offset = -1;
    return header;
}
