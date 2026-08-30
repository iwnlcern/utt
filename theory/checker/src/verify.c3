module checker::verify;

import checker::game;
import checker::wire;
import checker::xxh3;
import std::collections::list;
import std::collections::map;
import std::core::bitorder;
import std::bits;

struct FailPosition
{
    long byte_offset;
    long chunk;
    long record;
}

struct VerifyResult
{
    bool ok;
    String code;
    FailPosition fail_position;
    char claim;
    ushort schema_major;
    ushort schema_minor;
    String ruleset_id;
    char[16] ruleset_digest;
    String symmetry_id;
    char[37] root;
    bool root_is_initial;
    char[16] header_digest;
    ulong[4] totals;
    sz orphans;
}

struct Node
{
    game::State state;
    char kind;
    char payload;
}

alias StateSet = HashMap { String, bool };
alias NodeIndex = HashMap { String, sz };
alias NodeList = List { Node };

fn bool is_initial_root(char[37] root)
{
    for (sz i = 0; i < 36; i++) if (root[i] != 0) return false;
    return root[36] == 4;
}

fn VerifyResult reject(VerifyResult result, String code, long byte_offset, long chunk = -1, long record = -1)
{
    result.ok = false;
    result.code = code;
    result.fail_position = { byte_offset, chunk, record };
    return result;
}

fn VerifyResult result_from_header(wire::Header header)
{
    VerifyResult result = {
        .ok = false,
        .code = header.code,
        .fail_position = { header.fail_offset, -1, -1 },
        .claim = header.claim,
        .schema_major = header.schema_major,
        .schema_minor = header.schema_minor,
        .ruleset_id = header.ruleset_id,
        .symmetry_id = header.symmetry_id,
    };
    result.ruleset_digest = header.ruleset_digest;
    result.root = header.root;
    result.root_is_initial = is_initial_root(header.root);
    result.header_digest = header.header_digest;
    result.totals = header.totals;
    return result;
}

fn bool basic_state_valid(game::State state)
{
    sz x_count;
    sz o_count;
    for (sz board = 0; board < 9; board++)
    {
        if ((state.x[board] & ~0x1ff) != 0) return false;
        if ((state.o[board] & ~0x1ff) != 0) return false;
        if ((state.x[board] & state.o[board]) != 0) return false;
        x_count += (sz)state.x[board].popcount();
        o_count += (sz)state.o[board].popcount();
    }
    return x_count >= o_count && x_count <= o_count + 1;
}

fn bool same_state(game::State left, game::State right)
{
    char[37] left_bytes = left.serialize();
    char[37] right_bytes = right.serialize();
    return left_bytes[..] == right_bytes[..];
}

fn bool contains_state(StateSet* states, game::State state)
{
    char[37] bytes = state.serialize();
    return states.has_key((String)bytes[..]);
}

fn bool terminal_satisfies(char claim, game::TerminalKind terminal)
{
    switch (claim)
    {
        case 0: return terminal == game::TerminalKind.X_WIN;
        case 1: return terminal == game::TerminalKind.O_WIN;
        case 2: return terminal == game::TerminalKind.X_WIN || terminal == game::TerminalKind.DRAW;
        case 3: return terminal == game::TerminalKind.O_WIN || terminal == game::TerminalKind.DRAW;
        default: return false;
    }
}

fn char prover(char claim)
{
    return claim == 0 || claim == 2 ? 'X' : 'O';
}

fn bool declared_move_is_legal(game::State state, char payload)
{
    if (payload > 80) return false;
    game::MoveList moves = game::legal_moves(state);
    char board = payload / 9;
    char cell = payload % 9;
    for (sz i = 0; i < moves.len; i++)
    {
        if (moves.entries[i].board == board && moves.entries[i].cell == cell) return true;
    }
    return false;
}

fn bool rule_section_fits(char[] chunk, uint fixed_count, uint rule_count)
{
    ulong fixed_end = 8 + (ulong)fixed_count * 40;
    if (fixed_end > chunk.len) return false;
    sz cursor = (sz)fixed_end;
    for (uint i = 0; i < rule_count; i++)
    {
        if (cursor + 40 > chunk.len) return false;
        cursor += 38;
        ushort id_length = bitorder::read(chunk[cursor:2], UShortLE);
        cursor += 2;
        if (id_length < 1 || id_length > 64 || cursor + id_length + 6 > chunk.len) return false;
        cursor += (sz)id_length + 2;
        uint witness_length = bitorder::read(chunk[cursor:4], UIntLE);
        cursor += 4;
        if ((ulong)witness_length > (ulong)(chunk.len - cursor)) return false;
        cursor += witness_length;
    }
    return cursor == chunk.len;
}

fn VerifyResult verify_certificate(char[] blob)
{
    wire::Header header = wire::parse_header(blob);
    VerifyResult result = result_from_header(header);
    if (!header.ok) return result;

    StateSet verified;
    verified.tinit();
    NodeIndex indices;
    indices.tinit();
    NodeList nodes;
    nodes.tinit();
    ulong[4] streamed_totals;

    for (sz chunk_index = 0; chunk_index < header.rows.size; chunk_index++)
    {
        wire::ManifestRow row = header.rows[chunk_index];
        sz chunk_offset = (sz)row.byte_offset;
        sz chunk_length = (sz)row.byte_length;
        char[] chunk = blob[chunk_offset:chunk_length];

        char[16] digest = xxh3::hash128(chunk);
        if (digest[..] != row.chunk_digest[..])
        {
            return reject(result, "E_CHUNK_DIGEST", (long)chunk_offset, (long)chunk_index);
        }

        if (chunk.len < 8)
        {
            return reject(result, "E_CHUNK_COUNTS", (long)chunk_offset, (long)chunk_index);
        }
        uint fixed_count = bitorder::read(chunk[:4], UIntLE);
        uint rule_count = bitorder::read(chunk[4:4], UIntLE);
        if ((ulong)fixed_count + rule_count != row.record_count)
        {
            return reject(result, "E_CHUNK_COUNTS", (long)chunk_offset, (long)chunk_index);
        }
        if (!rule_section_fits(chunk, fixed_count, rule_count))
        {
            return reject(result, "E_CHUNK_COUNTS", (long)chunk_offset, (long)chunk_index);
        }
        if (rule_count > 0)
        {
            return reject(result, "E_RULE_RECORD_V0", (long)chunk_offset + 4, (long)chunk_index);
        }

        for (uint record_index = 0; record_index < fixed_count; record_index++)
        {
            sz record_offset = chunk_offset + 8 + (sz)record_index * 40;
            char[] record = blob[record_offset:40];
            if (record[39] != 0)
            {
                return reject(result, "E_RECORD_RESERVED", (long)record_offset + 39, (long)chunk_index, record_index);
            }
            char kind = record[37];
            char payload = record[38];
            if (kind != 0 && payload != 0xff)
            {
                return reject(result, "E_PAYLOAD", (long)record_offset + 38, (long)chunk_index, record_index);
            }
            if (kind > 2)
            {
                return reject(result, "E_KIND_TAG", (long)record_offset + 37, (long)chunk_index, record_index);
            }

            char[37] state_bytes;
            state_bytes[..] = record[:37];
            game::State state = game::deserialize(state_bytes);
            if (!basic_state_valid(state) || state.forced > game::ANY)
            {
                return reject(result, "E_ILLFORMED_STATE", (long)record_offset, (long)chunk_index, record_index);
            }
            if (state.forced != game::ANY && state.board_closed(state.forced))
            {
                return reject(result, "E_FORCED_CLOSED", (long)record_offset + 36, (long)chunk_index, record_index);
            }
            if (!same_state(state, game::canonical(state)))
            {
                return reject(result, "E_NONCANONICAL", (long)record_offset, (long)chunk_index, record_index);
            }

            String lookup_key = (String)state_bytes[..];
            if (verified.has_key(lookup_key))
            {
                return reject(result, "E_DUP_STATE", (long)record_offset, (long)chunk_index, record_index);
            }
            if (state.ply() != row.ply)
            {
                return reject(result, "E_PLY_MISMATCH", (long)record_offset, (long)chunk_index, record_index);
            }

            game::TerminalKind terminal = game::terminal(state);
            if ((terminal == game::TerminalKind.NONE && kind == 2)
                || (terminal != game::TerminalKind.NONE && kind != 2))
            {
                return reject(result, "E_KIND_TERMINALITY", (long)record_offset + 37, (long)chunk_index, record_index);
            }
            if ((kind == 0 && state.side() != prover(header.claim))
                || (kind == 1 && state.side() == prover(header.claim)))
            {
                return reject(result, "E_KIND_SIDE", (long)record_offset + 37, (long)chunk_index, record_index);
            }

            if (kind == 2)
            {
                if (!terminal_satisfies(header.claim, terminal))
                {
                    return reject(result, "E_TERMINAL_CLAIM", (long)record_offset, (long)chunk_index, record_index);
                }
            }
            else if (kind == 0)
            {
                if (!declared_move_is_legal(state, payload))
                {
                    return reject(result, "E_ILLEGAL_MOVE", (long)record_offset + 38, (long)chunk_index, record_index);
                }
                game::State child = game::canonical(game::apply_turn(state, payload / 9, payload % 9)!!);
                if (!contains_state(&verified, child))
                {
                    return reject(result, "E_CHILD_MISSING", (long)record_offset + 38, (long)chunk_index, record_index);
                }
            }
            else
            {
                game::MoveList replies = game::legal_moves(state);
                for (sz reply = 0; reply < replies.len; reply++)
                {
                    game::Move move = replies.entries[reply];
                    game::State child = game::canonical(game::apply_turn(state, move.board, move.cell)!!);
                    if (!contains_state(&verified, child))
                    {
                        return reject(result, "E_REPLY_MISSING", (long)record_offset, (long)chunk_index, record_index);
                    }
                }
            }

            String owned_key = lookup_key.copy(tmem);
            verified.set(owned_key, true);
            indices.set(owned_key, nodes.size);
            nodes.push({ state, kind, payload });
            streamed_totals[kind]++;
        }
        streamed_totals[3] += rule_count;
    }

    if (streamed_totals[..] != header.totals[..])
    {
        return reject(result, "E_TOTALS", (long)header.header_end);
    }
    char[37] root_bytes = header.root;
    if (!verified.has_key((String)root_bytes[..]))
    {
        return reject(result, "E_ROOT_ABSENT", (long)header.header_end);
    }

    bool[] reachable = alloc::new_array(tmem, bool, nodes.size);
    reachable[..] = false;
    List { sz } stack;
    stack.tinit();
    sz root_index = indices.get((String)root_bytes[..])!!;
    stack.push(root_index);
    reachable[root_index] = true;
    sz reachable_count;
    while (stack.size > 0)
    {
        sz index = stack.pop()!!;
        reachable_count++;
        Node node = nodes[index];
        if (node.kind == 2) continue;
        game::MoveList moves;
        if (node.kind == 0)
        {
            moves.entries[0] = { node.payload / 9, node.payload % 9 };
            moves.len = 1;
        }
        else { moves = game::legal_moves(node.state); }
        for (sz i = 0; i < moves.len; i++)
        {
            game::Move move = moves.entries[i];
            game::State child = game::canonical(game::apply_turn(node.state, move.board, move.cell)!!);
            char[37] child_bytes = child.serialize();
            sz child_index = indices.get((String)child_bytes[..])!!;
            if (!reachable[child_index])
            {
                reachable[child_index] = true;
                stack.push(child_index);
            }
        }
    }

    result.ok = true;
    result.code = "OK";
    result.fail_position = { -1, -1, -1 };
    result.orphans = nodes.size - reachable_count;
    return result;
}
