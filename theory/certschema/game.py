"""Alternating rule-lock game (DD §2, Appendix A): state identity, D4 canonical form."""

from dataclasses import dataclass

D4 = {
    "e": (0, 1, 2, 3, 4, 5, 6, 7, 8),
    "r90": (2, 5, 8, 1, 4, 7, 0, 3, 6),
    "r180": (8, 7, 6, 5, 4, 3, 2, 1, 0),
    "r270": (6, 3, 0, 7, 4, 1, 8, 5, 2),
    "mh": (2, 1, 0, 5, 4, 3, 8, 7, 6),
    "mv": (6, 7, 8, 3, 4, 5, 0, 1, 2),
    "md": (0, 3, 6, 1, 4, 7, 2, 5, 8),
    "ma": (8, 5, 2, 7, 4, 1, 6, 3, 0),
}
LINES = (
    0b000000111,
    0b000111000,
    0b111000000,
    0b001001001,
    0b010010010,
    0b100100100,
    0b100010001,
    0b001010100,
)
ANY = 9


def _permute_mask(mask: int, g: tuple) -> int:
    out = 0
    for c in range(9):
        if mask >> c & 1:
            out |= 1 << g[c]
    return out


@dataclass(frozen=True)
class State:
    x: tuple
    o: tuple
    forced: int

    def serialize(self) -> bytes:
        blob = b"".join(m.to_bytes(2, "little") for m in self.x)
        blob += b"".join(m.to_bytes(2, "little") for m in self.o)
        return blob + bytes([self.forced])

    def counts(self) -> tuple:
        return (
            sum(m.bit_count() for m in self.x),
            sum(m.bit_count() for m in self.o),
        )

    def side_to_move(self) -> str:
        cx, co = self.counts()
        return "X" if cx == co else "O"

    def board_closed(self, b: int) -> bool:
        return _won(self.x[b]) or _won(self.o[b]) or (
            (self.x[b] | self.o[b]).bit_count() == 9
        )

    def well_formed(self) -> bool:
        for b in range(9):
            if self.x[b] >> 9 or self.o[b] >> 9 or (self.x[b] & self.o[b]):
                return False
        cx, co = self.counts()
        if not (0 <= cx - co <= 1):
            return False
        if not (0 <= self.forced <= 9):
            return False
        if self.forced != ANY and self.board_closed(self.forced):
            return False
        return True


def _won(mask: int) -> bool:
    return any(mask & line == line for line in LINES)


def transform(s: State, g: tuple) -> State:
    nx, no = [0] * 9, [0] * 9
    for b in range(9):
        nx[g[b]] = _permute_mask(s.x[b], g)
        no[g[b]] = _permute_mask(s.o[b], g)
    nf = ANY if s.forced == ANY else g[s.forced]
    return State(x=tuple(nx), o=tuple(no), forced=nf)


def canonicalize(s: State) -> tuple:
    best, best_g = None, None
    for g in D4.values():
        transformed = transform(s, g)
        serialized = transformed.serialize()
        if best is None or serialized < best.serialize():
            best, best_g = transformed, g
    return best, best_g
