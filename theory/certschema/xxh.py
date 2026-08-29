"""XXH3-128 (seed 0) via the reference xxhsum CLI; canonical big-endian bytes (DD §4.5)."""

import shutil
import subprocess


def xxh3_128(data: bytes) -> bytes:
    if shutil.which("xxhsum") is None:
        raise RuntimeError("xxhsum CLI not found (install xxHash >= 0.8)")
    out = subprocess.run(
        ["xxhsum", "-H2", "-"], input=data, capture_output=True, check=True
    )
    hexdigest = out.stdout.split()[0].decode()
    if len(hexdigest) != 32:
        raise RuntimeError(f"unexpected xxhsum output: {out.stdout!r}")
    return bytes.fromhex(hexdigest)
