import hashlib


SEP = b"\x1f"


def validate_engine_ids(ids: list[str]) -> None:
    if len(ids) != len(set(ids)):
        raise ValueError("engine ids must be unique")
    if any("\x1f" in engine_id for engine_id in ids):
        raise ValueError("engine ids may not contain byte 0x1f")


def validate_tournament_seed(tournament_seed: str) -> None:
    if not isinstance(tournament_seed, str):
        raise ValueError("tournament seed must be a string")
    if "\x1f" in tournament_seed:
        raise ValueError("tournament seed may not contain byte 0x1f")


def pair_order(id1: str, id2: str) -> tuple[str, str]:
    first, second = sorted((id1, id2), key=lambda engine_id: engine_id.encode("utf-8"))
    return first, second


def pair_seed(
    tournament_seed: str, engine_a: str, engine_b: str, round_: int
) -> bytes:
    validate_tournament_seed(tournament_seed)
    validate_engine_ids([engine_a, engine_b])
    payload = SEP.join(
        (
            tournament_seed.encode("utf-8"),
            engine_a.encode("utf-8"),
            engine_b.encode("utf-8"),
            str(round_).encode("ascii"),
        )
    )
    return hashlib.sha256(payload).digest()


def game_seed(parent_seed: bytes, k: int) -> bytes:
    if k not in (1, 2):
        raise ValueError("game index must be 1 or 2")
    return hashlib.sha256(parent_seed + bytes((k,))).digest()


def pair_coin_seat(parent_seed: bytes) -> str:
    return "X" if parent_seed[0] % 2 == 0 else "O"
