ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-design-review-1
PARENT_DISPATCH_ID: engine-c1-design
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — engine.planner can revise the technical contract; shared protocol alignment routes through s1.orchestrator-planner
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/engine-c1/DESIGN-planner-20260825-073729.md
DESIGN_DOC_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 9cbfca857556045c559922bc7e86688f1effeb2e6d2825c67934ae8f8c0becb9
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — engine rules lock needs terminal, table-validity, protocol-seam, test, and benchmark corrections

## Verdict

The split between a rules/representation lock and a later theory-gated search-math lock is mechanically clear, the 80-byte value-copy representation is a reasonable implementation direction, and the collision/value-quality shapes are adequate with the clarifications below. The document is not yet safe to lock: its concrete adapter contradicts the harness-owned protocol, terminal positions can still generate moves, the complete 3^9 LocalTable domain lacks invalid-state semantics and exhaustive proof, the test seam omits load-bearing invariants, and the performance gate depends on an ephemeral benchmark whose copy probe is optimizable away.

## Review target and integrity

- Reviewed `docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-rules-c1-20260825.md` (171 lines) as committed in `9836bf5`. — E1 — source: exact-file read and `git show`.
- The working-tree and committed design bytes both hash to the declared `9cbfca857556045c559922bc7e86688f1effeb2e6d2825c67934ae8f8c0becb9`; the incoming DESIGN relay is also byte-identical to HEAD and passes exact-file relay lint. — E1 — source: `sha256sum`, `git diff`, `git show`, relay-lint.
- Scope reviewed: §§2–10, boundary contract, acceptance criteria, rejected alternatives, open/operator-owned questions, and the embedded `GRILL_LOCK`. Search backup/cutoff/TT-value/widening math remains outside this review by the orchestrator's split amendment. No source, test, design-doc, branch, commit, or PR mutation is authorized or performed. — E1 — source: design lines 6-17 and 118-147.

## Must-revise findings

### M1 — The consumer adapter locks a stale, owner-incompatible wire contract

Engine §6 validates `last_mover` and serializes bare `{bid, move}` even while declaring the harness schema owner-authoritative. The harness owner record defines a hello exchange, typed turn envelopes, canonical X/O budgets plus `you`, explicit `tie_owner` with no `last_mover`, `request_id` echo, and an optional `info` carrier; its boundary section already names this engine delta as an orchestrator-routed pre-PLAN obligation. The engine's root-side `BudgetContext {own_units, opp_units}` also lacks the persistent X/O perspective needed to interpret the harness's canonical budgets and return an own-perspective integer bid. — E1 — source: engine lines 72-81, 98-103, 149-156; harness design lines 28-68 and 152-157.

Required revision: do not lock the stale sketch. Either narrow §6 to an abstract adapter boundary and keep concrete wire bytes pending until the harness design is approved, or consume the approved owner schema verbatim in a successor. Name a root request context carrying seat/perspective (`you` or equivalent) alongside canonical X/O budgets, and route the concrete owner/consumer acknowledgment through `s1.orchestrator-planner`. Preserve the search core's JSON/I/O isolation.

### M2 — Terminal legality is not total

Movegen checks only forced/open-board routing. A macro win ends the game immediately even when open cells remain, so the stated algorithm can enumerate moves from a terminal `macro_win` Position. Apply-move also has no explicit precondition/rejection for an already terminal parent. — E1 — source: engine lines 57-64 and spec rule ordering.

Required revision: define `legal_moves(terminal) = empty` for both `macro_win` and `all_closed`, and require apply to reject every post-terminal move without mutating the parent. Add named fixtures/properties for both terminal kinds and for invalid occupied/closed/out-of-range/wrong-forced-board moves.

### M3 — LocalTable semantics are incomplete over its declared 3^9 domain

The table enumerates all 19,683 ternary assignments, including impossible dual-winner boards, but its 2-bit `status` has only open/X-won/O-won/full-draw and no invalid state. Strict adapter validation therefore has no specified table-level result for an input with both winning lines. `fork_*` is also described only as “two-or-more simultaneous threats”; whether the candidate move itself already wins/closes the board and how distinct winning cells are counted is left open. Finally, §14 claims a naive property-test guard for fork masks, but §7's actual property list never requires one. — E1 — source: engine lines 44-55, 83-90, 167-171.

Required revision: specify invalid/dual-winner detection (an invalid bit, independent line flags, or an explicit validation path), define fork masks mechanically, and require exhaustive comparison of all 19,683 entries against an independent naive evaluator for status/validity, empties, immediate-win masks, and fork masks. Include adapter rejection fixtures for inconsistent masks/caches/closure/forced/tie state.

### M4 — The test seam omits the invariants that make the lock enforceable

Current properties cover movegen, closure/routing, ternary codes, and caches, but not the R1 tie-state lifecycle, first-move center, terminal rejection, chip-count resolution at the all-closed boundary, Zobrist completeness, parent immutability, or precise perft actor semantics. In a bidding game either X or O may make consecutive marks, so a spatial move-count perft is ambiguous unless the mover schedule/branching is part of the definition; mover identity changes marks, tie state, closures, and later terminal counts. — E1 — source: engine lines 34-36, 57-70, 83-90, 98-103 and 159-165.

Required revision: add named E2 tests for initial `forced=4` and `NULL_FIRST_MOVE`; `tie'=opponent(actual mover)` including consecutive moves by one player and NULL disappearing after the first mark; parent value immutability; recomputed Zobrist/key differences for every key input; and budget-layer all-closed outcomes for positive, negative, and zero X-relative margins. Define perft as an explicit `(position, mover)` traversal or a stated both-mover expansion, with pinned values and terminal cutoffs. Correct §9's redundant `(Position, tie) -> T` notation because `tie` is already inside Position, and state whose perspective T uses.

### M5 — The microbenchmark gate is not durable or methodologically locked

The acceptance threshold is tied to 24 ns/ply, but the only source is `/tmp/uttt_bench/bench.cpp`, outside the repository. Inspection of that file shows the pure-copy probe assigns `forced` with both conditional arms equal to `4`, permitting the measured copy work to collapse; the free-choice probe repeatedly uses one empty position and consumes only a count. Those probes do not support “pure copy ≈0” or a stable regression baseline, even though the playout result is useful directional evidence. — E1 — source: engine lines 38-42 and 159-165; exact read of `/tmp/uttt_bench/bench.cpp`.

Required revision: commit or reproduce the benchmark as a named PLAN deliverable before making it a gate; prevent dead-code/constant-fold elimination, define warmup/iterations/compiler/CPU/governor/statistic, and establish a fresh baseline from the committed harness. The representation choice may remain, but the eventual acceptance criterion must compare against durable evidence rather than an untracked `/tmp` number.

## Retained approvals and clarifications

- The rules/search-math split, value-copy Position direction, fixed move-list storage, budget exclusion from Position, and search-core JSON isolation are coherent and may be retained.
- `64-bit Zobrist + 32-bit secondary tag` in play and full semantic-key verification in acceptance mode is adequate. Clarify that the full comparison is fieldwise over identity-bearing Position fields, not padding-sensitive `memcmp` and not derived/cached bytes unless cache consistency is first verified.
- The value-quality shape (`exact | bound | estimate`, interval, depth, complete) is adequate; retain the rule that incomplete move-set exploration cannot report exact and that only a completed iteration is published.
- The embedded grill preserves the recorded operator choices and rejected alternatives. This review does not reopen them; M1–M5 are technical completeness and boundary-alignment corrections.

## Design-review checklist

- Design identity, digest, target entity, and split lock surface: pass. — E1.
- Operator decisions, defaults, and rejected alternatives: pass; no new operator product ruling is required. — E1.
- Boundary contract: must-revise (M1). — E1.
- Rules and representation totality: must-revise (M2, M3). — E1.
- Acceptance/test proof: must-revise (M3–M5). — E1.
- Open questions: document says none, but M1–M5 are unresolved technical obligations. They are Planner/orchestrator-routable, not operator-judgment items. — E1.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — rules-core design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — fixed game budgets are simulation inputs, not real monetary state
- AI-or-automation-acts-downstream: no — this relay grants no implementation or runtime action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the harness-owned JSONL protocol and theory-owned fixtures
- user-visible-control-with-materializer/downstream-consumer: no — no user-facing control changed by this review
- test-runtime-role-mismatch: no — no test runtime is selected or executed by this review
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — protocol-owner alignment and benchmark evidence are unresolved lock risks
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade is requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Successor acceptance

Return a successor DESIGN review request with the same `DESIGN_DOC_ID`, a fresh `DESIGN_SHA256`, and `IN_REPLY_TO` naming this review. Close M1–M5; preserve the split lock and existing operator decisions; cite an addressed orchestrator boundary alignment or keep the concrete adapter pending. No implementation, PLAN lock, prototype, dependency vendoring, commit, PR, merge, publication, or deployment is authorized by this verdict.

Operator-judgment items: none. If revision discovers a product-semantic choice rather than a technical completion issue, stop and route it explicitly.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-075718.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-075718`
- source/tests=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-075718.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-planner-20260825-075031.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/reconcile-c1/RECONCILE-orchestrator-reviewer-20260825-074009.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-REVIEW-pair-implementer-20260825-074956.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-planner-20260825-074556.md
