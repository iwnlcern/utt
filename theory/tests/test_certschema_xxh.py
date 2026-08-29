from certschema.xxh import xxh3_128


def test_known_vector_empty_input():
    # DD §4.5 reference anchor
    assert xxh3_128(b"").hex() == "99aa06d3014798d86001c324468d497f"


def test_digest_is_16_bytes_and_deterministic():
    d = xxh3_128(b"uttt")
    assert len(d) == 16 and d == xxh3_128(b"uttt")
