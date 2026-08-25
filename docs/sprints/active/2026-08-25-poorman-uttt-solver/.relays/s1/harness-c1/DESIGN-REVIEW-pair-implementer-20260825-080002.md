ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-design-review-2
PARENT_DISPATCH_ID: harness-c1-design-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — remaining findings are technical contract defects within the Planner's authority
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/DESIGN-planner-20260825-075031.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: cfab76fc8330ba5ecc7d751fcedf849252387b2f87deb0d686ffb604a3000066
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW revision 2 — seed fairness and recovery-log totality remain unresolved

## Review target and integrity

- Reviewed `docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md` revision 2 (212 lines). — E1 — source: exact-file read.
- The declared digest matches the reviewed bytes: `cfab76fc8330ba5ecc7d751fcedf849252387b2f87deb0d686ffb604a3000066`. — E1 — source: `sha256sum`.
- The current theory owner record now does lock `forced` to integer-or-null; the residual `h`/budget-key translation is named and E2-gated. The engine adapter divergence is also explicitly routed pre-PLAN. M1 and M7 are folded. — E1 — source: harness lines 152-157; theory design lines 109-135; engine design lines 72-81.
- Uniform restart/re-hello, scoped deterministic testing, public advisory `info`, reachable terminal reasons, and best-effort `game_end` delivery fold M2, M4, M5, and both consistency corrections. — E1 — source: harness lines 74-82, 93-100, 144-150.

## Must-revise findings

### M3 remains incomplete — recovery failure has no total, append-ordered log representation

The revision stores every retry in one `auction.attempts` array, but emits each intervening restart as a separate `recovery` event. In an append-only stream the design does not say whether that recovery precedes the later `auction` event containing its triggering attempt, or whether recovery evidence is buffered and emitted out of causal order. More decisively, if both seats double-fault on attempt 1 and a restart hello then fails, the game ends with `recovery_fault`; the current `auction.resolution` permits an object only for a resolved ply and `null` only for a three-double-fault void, so this reachable terminal has no schema-valid auction disposition. The restart `hello` submission record also lacks an explicit parsed `name`/`version` shape even though the generic submission record names only parsed `bid`/`move`/`info`. — E1 — source: harness lines 93-100 and 119-128.

Required revision: define the causal append order and a schema-valid disposition for every recovery-fault path. A per-attempt event is the cleanest model; alternatively give the aggregate auction an explicit unresolved terminal reason and define when it is emitted relative to recovery events. Define one hello-record shape, including parsed identity on success and bounded raw evidence on failure, for both startup and recovery.

### M6 remains incomplete — the claimed opposite-engine coin pairing is false

Game 1 and game 2 hash distinct `game_seed_k` values, so their first-byte parities are independent. Flipping game 2's independently derived seat result does not guarantee the opposite engine. Even if both games reused one parity, flipping X↔O while also swapping seats would favor the SAME engine twice, not opposite engines. Thus §7's algorithm does not establish §7's stated fairness property. In addition, `engine_A`/`engine_B` ordering and delimiter-safe component encoding for `pair_id` are not fixed, so the claimed canonical hash input can still be constructed differently by conforming implementations. — E1 — source: harness lines 109-117.

Required revision: define the fairness mapping in engine-identity terms. Recommended minimal rule: establish a stable A/B identity order, derive one pair-level coin bit, and reuse the same X/O seat outcome in both seat-swapped games (no seat inversion), which necessarily favors opposite engines. If retaining `coin_invert`, specify an engine-identity mapping that provably selects the other engine in game 2. Pin delimiter-safe component encoding and the `tournament_seed` byte representation, then log the inputs/result and add a two-parity pairing test.

## Design-review checklist

- DESIGN_DOC_ID, digest, target entity, decomposition, operator decisions, rejected alternatives, and scope: pass. — E1.
- Boundary contracts: pass for theory and correctly held pre-PLAN for engine alignment. — E1.
- Fault/replay completeness: must-revise (remaining M3). — E1.
- Determinism and pair fairness: must-revise (remaining M6). — E1.
- Acceptance criteria: otherwise testable; add explicit pairing parity coverage and recovery-fault log fixtures. — E1.
- Human decision: none required; these corrections preserve R1-R5 and `GRILL_LOCK_ID`.

## Grill review

The mandatory grill re-check preserves the operator-selected analysis carrier and bridge scope. It rejects the revised seed rule because its seat inversion and independent per-game parity do not imply the claimed engine-level fairness, and rejects the recovery log as total because a pre-retry recovery failure has no defined auction disposition. No product-semantic branch is reopened.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-080002.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-080002`
- source/tests=none — review-only; no source, test, design-doc, branch, commit, or PR edits

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-075718.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-080002.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-planner-20260825-075031.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/reconcile-c1/RECONCILE-orchestrator-reviewer-20260825-074009.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-REVIEW-pair-implementer-20260825-074956.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-planner-20260825-074556.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-planner-20260825-075941.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-planner-20260825-075940.md

## Requested successor

Return a successor design-review request with the same `DESIGN_DOC_ID`, a fresh digest, M3/M6 above dispositioned, and `IN_REPLY_TO` pointing to this review. Preserve R1-R5 and `GRILL_LOCK_ID`; float the digest forward rather than replacing the design identity.
