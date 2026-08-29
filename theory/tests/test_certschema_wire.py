from certschema.game import ANY, State, canonicalize
from certschema.wire import (
    CLAIM,
    KIND,
    Record,
    build_certificate,
    build_verdict,
    header_digest,
)

GOLDEN_CERT_HEX = (
    "555454544345525400000100000000000a00757474742d73322d723116e85b36"
    "332493470fa10065161285790c0064342d6c65786d696e2d7631000000000000"
    "000000000000c001c001c0010000100010000101820028000000000000000900"
    "0000000000000000000000000000000000010000000000000000000000000000"
    "00010000001100000001000000bd0000000000000030000000000000002ce613"
    "c5c2df8cfc062df18e6a040fdab88736e164effd77a28051627861f7da010000"
    "0000000000000000000000000000000000c001c001c001000010001000010182"
    "0028000000000000000902ff00"
)
GOLDEN_VERDICT_HEX = (
    "555454545652445400000100000000000a00757474742d73322d723116e85b36"
    "332493470fa10065161285790c0064342d6c65786d696e2d7631000000000000"
    "000000000000c001c001c0010000100010000101820028000000000000000901"
    "001800676f6c64656e2d7465726d696e616c2d77696e782e757463285856ee77"
    "314b80f2440452c97463bd"
)


def _golden_root() -> State:
    x = [0] * 9
    o = [0] * 9
    x[0] = x[1] = x[2] = 0b111
    o[3] = (1 << 3) | (1 << 5)
    o[4] = (1 << 1) | (1 << 7)
    o[5] = (1 << 0) | (1 << 8)
    o[6] = 1 << 4
    o[7] = 1 << 4
    return canonicalize(State(x=tuple(x), o=tuple(o), forced=ANY))[0]


def test_golden_certificate_bytes_match_dd_appendix_b():
    root = _golden_root()
    rec = Record(state=root, kind=KIND["TERMINAL"], payload=0xFF)
    cert = build_certificate(CLAIM["WIN_X"], root, {17: [rec]})
    assert cert.hex() == GOLDEN_CERT_HEX
    assert len(cert) == 237


def test_golden_verdict_bytes_match_dd_appendix_b():
    root = _golden_root()
    rec = Record(state=root, kind=KIND["TERMINAL"], payload=0xFF)
    cert = build_certificate(CLAIM["WIN_X"], root, {17: [rec]})
    verdict = build_verdict(
        0,
        root,
        [(CLAIM["WIN_X"], "golden-terminal-winx.utc", header_digest(cert))],
    )
    assert verdict.hex() == GOLDEN_VERDICT_HEX
    assert len(verdict) == 139
