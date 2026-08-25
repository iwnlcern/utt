ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-design-review-2a
PARENT_DISPATCH_ID: harness-c1-design-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — remaining findings are technical contract defects within the Planner's authority
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/DESIGN-planner-20260825-075031.md
SUPERSEDES: s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-080002.md — fresh timestamp after concurrent INDEX append; findings unchanged
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: cfab76fc8330ba5ecc7d751fcedf849252387b2f87deb0d686ffb604a3000066
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW revision 2 successor — M3 and M6 remain incomplete

## Target and dispositions

- Reviewed the 212-line design at the declared, matching SHA-256 above. — E1 — exact-file read and `sha256sum`.
- M1 passes: theory's current owner record locks `forced` integer-or-null; the residual `h`/budget-key loader seam is explicit and E2-gated. M7 passes: the engine adapter delta is explicitly orchestrator-routed pre-PLAN. — E1 — harness lines 152-157; theory lines 109-135; engine lines 72-81.
- M2, M4, M5, `fault_forfeit` removal, and best-effort `game_end` delivery pass. — E1 — harness lines 74-82, 93-100, 144-150.

## M3 must revise — recovery log is not total

Retries live in one eventual `auction.attempts` array while intervening restarts are separate append-only `recovery` events; causal emission order is undefined. If both seats double-fault on attempt 1 and a restart hello fails, the game ends `recovery_fault`, but `auction.resolution` permits an object only for a resolved ply and `null` only for the third-double-fault void. That reachable terminal has no schema-valid auction disposition. The generic recovery hello record also names parsed `bid`/`move`/`info`, not parsed hello `name`/`version`. — E1 — harness lines 93-100, 119-128.

Required: define causal append order and every recovery-fault disposition. Use per-attempt events, or give the aggregate auction an explicit unresolved terminal reason and emission order. Define one startup/recovery hello record with parsed identity on success and bounded raw evidence on failure. Add recovery-fault log fixtures.

## M6 must revise — pair fairness claim is false

The two `game_seed_k` hashes have independent first-byte parity. Flipping game 2's independently derived seat result cannot guarantee the opposite engine. Even with one reused parity, flipping X↔O while also swapping seats favors the same engine twice. `engine_A`/`engine_B` order, delimiter-safe `pair_id` component encoding, and `tournament_seed` byte representation are also unpinned. — E1 — harness lines 109-117.

Required: specify fairness in engine-identity terms. Recommended: stable A/B order, one pair-level coin bit, and the same X/O seat result in both seat-swapped games, with no seat inversion; this necessarily favors opposite engines. Otherwise prove an exact engine-identity `coin_invert` mapping. Pin collision-free component encoding and seed bytes; add both-parity pair tests.

## Verdict and grill

The mandatory grill preserves R1-R5, the analysis-carrier choice, bridge scope, design identity, and `GRILL_LOCK_ID`. Boundary contracts otherwise pass. No human ruling is required; M3/M6 are technical completeness corrections.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-080223.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-080223`
- source/tests=none — review-only; no source, test, design-doc, branch, commit, or PR edits

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-075718.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-080002.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-080223.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-planner-20260825-075031.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/reconcile-c1/RECONCILE-orchestrator-reviewer-20260825-074009.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-REVIEW-pair-implementer-20260825-074956.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-planner-20260825-074556.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-planner-20260825-075941.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-planner-20260825-075940.md

## Requested successor

Return the same `DESIGN_DOC_ID` with a fresh digest, M3/M6 dispositioned, and `IN_REPLY_TO` pointing here. Preserve R1-R5 and `GRILL_LOCK_ID`; float the digest, not the design identity.
