ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-design-review-1
PARENT_DISPATCH_ID: harness-c1-design-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the Planner can revise the technical contract; cross-domain convention changes route through s1.orchestrator-planner
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/DESIGN-planner-20260825-072649.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: ee308ab68ffa085df8cc1a4393b25815e52ccfee99ff7b513cc404d9b4625bc4
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW — harness protocol/log design must revise contract gaps

## Review target and integrity

- Reviewed `docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md` (182 lines). — E1 — source: exact-file read.
- Declared digest matches the bytes under review: `ee308ab68ffa085df8cc1a4393b25815e52ccfee99ff7b513cc404d9b4625bc4`. — E1 — source: `sha256sum`.
- R1–R4 match the operator-ratified spec at `a3f250c`; R5 and the analysis-carrier choice are recorded in the embedded `GRILL_LOCK`. No product-semantic ruling is being reopened. — E1 — source: design §§1/12, spec at `a3f250c` lines 19-67, and reconciliation lines 12-19.

## Must-revise findings

### M1 — Shared fixture seam contradicts the theory-owned contract

Harness §11 says theory fixtures will use `forced` integer-or-null, and §3.3 uses wire `null` for free choice. The theory design's owner-of-record schema instead locks `forced` to `0..8 | "any"`. The dispatch says harness consumes theory conventions verbatim; the current documents cannot both be true. — E1 — source: harness lines 58, 139, 174, 182; theory design lines 68-85.

Required revision: obtain the orchestrator-routed alignment and record one of: (a) the same representation in both contracts, or (b) an explicit fixture-to-wire translation boundary with named E2 seam tests. Do not state that theory uses int-or-null unless its owner record says so.

### M2 — R2 has no post-fault process-recovery contract

R2 makes a turn fault lose only the auction, so the engine must be able to participate on the next ply. Yet timeout can leave stale computation/output, EOF/crash leaves no process, double-fault retry reuses the same state, and §9 says only that one process lives per seat per game and restarts between games. The design never says when to drain, kill, restart, re-hello, or how a restart hello fault affects the continuing game. — E1 — source: harness lines 45-46, 84-90, 115-126; fixed R2 at lines 13-16.

Required revision: define recovery per fault class. Recommended default: after any turn fault, terminate that seat's process group and start a clean process before its next request; for double-fault retries recover both seats before the fresh request; log recovery and hello outcome. Reconcile this with “one process per seat per game” and `hello_fault` semantics.

### M3 — Log schema is not total over its claimed fault outcomes

The log defines one `auction` per resolved ply, but triple double-fault ends the game with no resolved ply, so its three attempts have no event that can carry them. `game_start` also has no hello attempt/fault record. Per-attempt `bid`/`move`/`info` cannot represent invalid UTF-8, malformed JSON, EOF, an oversize line, or extra raw protocol lines, despite the claim that every fault is replayable and logged. — E1 — source: harness lines 45, 84-90, 106-113.

Required revision: make the event model total. Define an auction-attempt event/record even when no move resolves (or another explicit event type), include hello evidence, and define bounded raw-line capture for unparsable bytes (for example base64 plus truncation metadata) separately from nullable parsed fields. Every named fault fixture must map to a schema-valid log.

### M4 — Byte-identical determinism conflicts with logged wall-clock and engine analysis

Acceptance requires same seed to produce byte-identical logs, while every reply logs real `elapsed_ms` and optional engine `info`; time-limited engines may legally return different search results on repeated runs. Seed equality alone cannot make those bytes identical. — E1 — source: harness lines 77-82, 92-104, 108-113, 130-134; engine design lines 75-76 independently rejects bit-identical re-request replies.

Required revision: scope the byte-identity test to deterministic scripted stubs plus an injected fake clock and canonical JSON serialization, or define a normalized deterministic projection that excludes elapsed/analysis fields. Keep production elapsed timing in logs.

### M5 — `info.quality` is simultaneously required and non-faulting

Section 4 says `quality` is REQUIRED whenever `info` exists, then says missing `quality` only warns and is not a fault. A JSON Schema cannot make the same reply both schema-valid advisory data and missing-required-key data under the fail-closed rule. — E1 — source: harness lines 25 and 77-82.

Required revision: choose one contract. Recommended: `quality` is required for harness-owned analysis producers/conformance, but the public v1 reply schema accepts any size-capped JSON object and logs a missing/unknown quality warning without faulting the bid.

### M6 — Pair seed and per-game seed are contradictory

Section 7 says per-game seed hashes `(tournament_seed, game_id)`, then says paired games share a base seed and game two inverts the coin mapping. Different game ids yield different hashes unless a separate pair seed and derivation are specified; “SHA-256(tournament_seed, game_id)” also lacks an unambiguous byte encoding. — E1 — source: harness lines 99-104.

Required revision: define `pair_id`, canonical seed-input encoding, `pair_seed`, per-game derivation, and the exact lazy-coin inversion rule. Log enough identifiers to reproduce the mapping without guessing.

### M7 — Downstream engine adapter is already divergent and is absent from the completion gate

Harness drops `last_mover`, requires hello/turn envelopes, canonical X/O budgets, and optional info. The current engine design still validates `last_mover` and serializes only `{bid, move}`. Harness owns the schema, so engine must change, but the harness lock currently names only the theory convention as an orchestrator-routed pre-PLAN ack. — E1 — source: harness lines 30-68 and 173-175; engine design lines 69-78.

Required revision: add an explicit consumer-alignment obligation to the boundary contract/design-completion report and route the concrete adapter delta through `s1.orchestrator-planner`. The harness design need not adopt the engine draft; it must make the incompatibility visible before either plan treats the seam as locked.

## Additional consistency correction

- `game_end.reason` includes `fault_forfeit`, but R2 turn faults lose an auction rather than forfeit a game, and hello faults already use `hello_fault`; no reachable producer for `fault_forfeit` is defined. Remove it or define its exact reachable transition without contradicting R2. — E1 — source: harness lines 45, 70-75, 84-90.
- Define `game_end` delivery as best-effort after terminal resolution so inability to notify a crashed/faulted process cannot rewrite an already-decided result. — E0 — source: lifecycle stress test against harness lines 70-75.

## Design-review checklist

- DESIGN_DOC_ID and digest: pass. — E1 — source: header and `sha256sum`.
- Target entity and decomposition: pass; authoritative game state/log plus separated rules, protocol, process, auction, log, referee, and tournament modules are coherent. — E1 — source: §§8-9.
- Operator decisions/defaults and rejected alternatives: pass, subject to the technical revisions above. — E1 — source: §§1/12.
- Boundary contract: must-revise (M1, M7). — E1 — source: §11 and owner/consumer designs.
- Fault/replay completeness: must-revise (M2, M3, consistency correction). — E1 — source: §§5, 8, 9.
- Acceptance criteria draft: must-revise (M4); the remaining spec/schema/transcript/fault-matrix/afternoon-bar items are appropriately testable. — E1 — source: §10.
- Open questions: the document says none, but M1–M7 are unresolved technical dependencies. None requires a new operator product ruling. — E1 — source: stress test above.

## Grill review

The embedded `GRILL_LOCK` has the required sources, resolved decisions, rejected alternatives, operator-owned section, and design-lock impacts. The operator answers are preserved. The grill nevertheless missed the fault-recovery branch, total log representation for unresolved/faulted auctions, deterministic-test clock boundary, and already-visible owner/consumer schema deltas; M1–M7 must be folded into a successor design digest before approval.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-073619.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-073619`
- source/tests=none — review-only; no source, test, design-doc, branch, commit, or PR edits

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-073619.md

## Requested successor

Return a successor design-review request with the same `DESIGN_DOC_ID`, a fresh digest, M1–M7 and both consistency corrections dispositioned, and `IN_REPLY_TO` pointing to this review. Preserve R1–R5 and `GRILL_LOCK_ID`; float the design digest forward rather than replacing the design identity.
