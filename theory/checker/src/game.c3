module checker::game;

import std::bits;
import std::core::bitorder;

const char ANY = 9;

const char[9][8] D4 = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8 },
    { 2, 5, 8, 1, 4, 7, 0, 3, 6 },
    { 8, 7, 6, 5, 4, 3, 2, 1, 0 },
    { 6, 3, 0, 7, 4, 1, 8, 5, 2 },
    { 2, 1, 0, 5, 4, 3, 8, 7, 6 },
    { 6, 7, 8, 3, 4, 5, 0, 1, 2 },
    { 0, 3, 6, 1, 4, 7, 2, 5, 8 },
    { 8, 5, 2, 7, 4, 1, 6, 3, 0 },
};

const ushort[8] LINES = {
    0x007, 0x038, 0x1c0,
    0x049, 0x092, 0x124,
    0x111, 0x054,
};

faultdef ILLEGAL_MOVE;

enum BoardKind : char
{
    NONE,
    X_WIN,
    O_WIN,
    FULL,
}

enum TerminalKind : char
{
    NONE,
    X_WIN,
    O_WIN,
    DRAW,
}

struct Move
{
    char board;
    char cell;
}

struct MoveList
{
    Move[81] entries;
    sz len;
}

struct State
{
    ushort[9] x;
    ushort[9] o;
    char forced;
}

fn char[37] State.serialize(&self)
{
    char[37] bytes;
    for (sz board = 0; board < 9; board++)
    {
        bitorder::write(self.x[board], bytes[board * 2:2], UShortLE);
        bitorder::write(self.o[board], bytes[18 + board * 2:2], UShortLE);
    }
    bytes[36] = self.forced;
    return bytes;
}

fn State deserialize(char[37] bytes)
{
    State state;
    for (sz board = 0; board < 9; board++)
    {
        state.x[board] = bitorder::read(bytes[board * 2:2], UShortLE);
        state.o[board] = bitorder::read(bytes[18 + board * 2:2], UShortLE);
    }
    state.forced = bytes[36];
    return state;
}

fn bool won(ushort mask) @inline
{
    foreach (line : LINES) if ((mask & line) == line) return true;
    return false;
}

fn BoardKind board_result(State state, sz board)
{
    if (won(state.x[board])) return BoardKind.X_WIN;
    if (won(state.o[board])) return BoardKind.O_WIN;
    if ((state.x[board] | state.o[board]).popcount() == 9) return BoardKind.FULL;
    return BoardKind.NONE;
}

fn bool State.board_closed(&self, sz board)
{
    return board_result(*self, board) != BoardKind.NONE;
}

fn sz State.ply(&self)
{
    sz count;
    for (sz board = 0; board < 9; board++)
    {
        count += (sz)self.x[board].popcount();
        count += (sz)self.o[board].popcount();
    }
    return count;
}

fn char State.side(&self)
{
    sz x_count;
    sz o_count;
    for (sz board = 0; board < 9; board++)
    {
        x_count += (sz)self.x[board].popcount();
        o_count += (sz)self.o[board].popcount();
    }
    return x_count == o_count ? 'X' : 'O';
}

fn bool State.well_formed(&self)
{
    sz x_count;
    sz o_count;
    for (sz board = 0; board < 9; board++)
    {
        if ((self.x[board] & ~0x1ff) != 0) return false;
        if ((self.o[board] & ~0x1ff) != 0) return false;
        if ((self.x[board] & self.o[board]) != 0) return false;
        x_count += (sz)self.x[board].popcount();
        o_count += (sz)self.o[board].popcount();
    }
    if (x_count < o_count || x_count > o_count + 1) return false;
    if (self.forced > ANY) return false;
    if (self.forced != ANY && self.board_closed(self.forced)) return false;
    return true;
}

fn ushort transform_mask(ushort mask, sz transform_index)
{
    ushort result;
    for (sz cell = 0; cell < 9; cell++)
    {
        if ((mask & (1 << cell)) != 0)
        {
            result |= (ushort)(1 << D4[transform_index][cell]);
        }
    }
    return result;
}

fn State transform(State state, sz transform_index)
{
    State result;
    for (sz board = 0; board < 9; board++)
    {
        sz destination = D4[transform_index][board];
        result.x[destination] = transform_mask(state.x[board], transform_index);
        result.o[destination] = transform_mask(state.o[board], transform_index);
    }
    result.forced = state.forced == ANY ? ANY : D4[transform_index][state.forced];
    return result;
}

fn bool bytes_less(char[37] left, char[37] right)
{
    for (sz i = 0; i < 37; i++)
    {
        if (left[i] < right[i]) return true;
        if (left[i] > right[i]) return false;
    }
    return false;
}

fn State canonical(State state)
{
    State best = transform(state, 0);
    char[37] best_bytes = best.serialize();
    for (sz g = 1; g < 8; g++)
    {
        State candidate = transform(state, g);
        char[37] candidate_bytes = candidate.serialize();
        if (bytes_less(candidate_bytes, best_bytes))
        {
            best = candidate;
            best_bytes = candidate_bytes;
        }
    }
    return best;
}

fn bool macro_win(State state, char side)
{
    ushort macro_mask;
    for (sz board = 0; board < 9; board++)
    {
        ushort mask = side == 'X' ? state.x[board] : state.o[board];
        if (won(mask)) macro_mask |= (ushort)(1 << board);
    }
    return won(macro_mask);
}

fn TerminalKind terminal(State state)
{
    if (macro_win(state, 'X')) return TerminalKind.X_WIN;
    if (macro_win(state, 'O')) return TerminalKind.O_WIN;
    for (sz board = 0; board < 9; board++)
    {
        if (!state.board_closed(board)) return TerminalKind.NONE;
    }
    return TerminalKind.DRAW;
}

fn MoveList legal_moves(State state)
{
    MoveList moves;
    if (terminal(state) != TerminalKind.NONE) return moves;
    for (sz board = 0; board < 9; board++)
    {
        if (state.forced != ANY && board != state.forced) continue;
        if (state.board_closed(board)) continue;
        ushort occupied = state.x[board] | state.o[board];
        for (sz cell = 0; cell < 9; cell++)
        {
            if ((occupied & (1 << cell)) != 0) continue;
            moves.entries[moves.len++] = { (char)board, (char)cell };
        }
    }
    return moves;
}

fn bool is_legal(State state, sz board, sz cell)
{
    if (board >= 9 || cell >= 9) return false;
    if (terminal(state) != TerminalKind.NONE) return false;
    if (state.forced != ANY && state.forced != board) return false;
    if (state.board_closed(board)) return false;
    return ((state.x[board] | state.o[board]) & (1 << cell)) == 0;
}

fn State? apply(State state, sz board, sz cell, char side)
{
    if (side != 'X' && side != 'O') return ILLEGAL_MOVE~;
    if (!is_legal(state, board, cell)) return ILLEGAL_MOVE~;
    if (side == 'X') { state.x[board] |= (ushort)(1 << cell); }
    else { state.o[board] |= (ushort)(1 << cell); }
    state.forced = (char)cell;
    if (state.board_closed(cell)) state.forced = ANY;
    return state;
}

fn State? apply_turn(State state, sz board, sz cell)
{
    return apply(state, board, cell, state.side());
}
