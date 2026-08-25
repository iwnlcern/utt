ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-plan-review-1
PARENT_DISPATCH_ID: harness-c1-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can correct the execution contract without reopening R1-R5
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/PLAN-planner-20260825-082602.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: cc75aee6c009a6e471b3ed432636a37e5822f9baf4aeb220d392b66719323607
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW — harness plan must close framing, recovery-order, state, replay, and baseline gaps

## Target, lineage, and duplicate gate

- The plan digest matches the 411-line artifact; the design digest matches the approved design; `harness-c1-plan-1` correctly parents to `harness-c1-design-review-3`. — E1 — `sha256sum`, relay headers.
- No `referee/` or `docs/protocol/` implementation files currently exist, so the requested package is not already built. — E1 — `rg --files` census.
- Scope and decomposition are otherwise coherent: pure rules/auction/seeds, codec/supervisor, total log, game loop, bots, tournament, fixture seam, and protocol artifacts map to the approved DD. — E1 — plan lines 17-63 and Tasks 1-13.

## Must-revise findings

### MR1 — The pinned baseline is already stale and conflicts with “current main”

The relay pins `BASE: main@26a1630` and the plan says create the branch from current main, but current `main` is `adba0f9`. The intervening commits include the orchestrator's adapter routing and engine design consuming the harness schema, so the no-cross-bundle-collision condition must be checked against those bytes rather than an older base. — E1 — `git rev-parse`, `git log 26a1630..HEAD`, plan line 26.

Required: reissue against the actual implementation base, or make dispatch-time rebase plus a fresh no-collision `SCOPE_DIFF` an explicit precondition and update the relay `BASE`. Do not let “current main” silently float beneath a pinned plan carrier.

### MR2 — The supervisor interfaces cannot produce the complete fault taxonomy

`parse_turn_reply(raw)` can classify an already-returned oversize buffer, but `Engine.read_reply` returns only a complete line, timeout, or EOF. It has no bounded framing state for a stream that exceeds 32 KiB without LF, and no representation for detecting a second protocol line already buffered after the first. Such clients become `timeout`/next-turn contamination instead of `oversize_line`/`extra_protocol_line`; the exact ten-class E2E requirement is therefore not implementable from the specified interfaces. — E1 — plan lines 193-229, 269-297; DD lines 21-25 and 85-91.

Required: specify bounded incremental framing in `procs.py`, including early oversize classification before LF, detection/quarantine of an extra complete stdout line for the current request, and the exact status handoff into the codec. Add direct supervisor tests plus E2E assertions for both fault classes; ensure stale bytes cannot reach a later request.

### MR3 — Single-fault recovery is ordered before the legal move is applied

Task 9 sequences “single fault → recover faulter” before “apply move.” R2 first resolves the auction and applies the legal opponent's move; recovery is required only before the faulter's next request. If that move produces a macro win or all-closed terminal, restarting first can turn the already-earned game result into `recovery_fault`, contradicting R2 and the DD lifecycle. — E1 — plan lines 299-327; DD lines 89, 95-100.

Required: for a single fault, resolve/pay/apply/log and detect terminal first; recover only if another request will occur. Add an E2E case where the legal opponent's fault-win move ends the game and assert a restart failure cannot rewrite the terminal result. Double-fault recovery remains before retry because no move resolved.

### MR4 — The R1 tie-owner transition is neither specified nor tested

The plan says only “update ... tie_owner.” The fixed rule is `tie_owner' = opponent(actual mover)` after every applied mark, including fault-won auctions and zero-bid ties. Existing Task 3 tests exercise resolution against a supplied owner, and Task 9 tests only the ply-0 coin; neither catches an incorrect state transition. — E1 — spec line 58; plan lines 143-170 and 299-327.

Required: pin the exact transition in Task 9 and add multi-ply tests covering consecutive wins, a fault-applied move, and a 0-0 chain.

### MR5 — The schema plan conflicts with forward compatibility

Task 5 correctly says unknown top-level keys are tolerated, but Task 6 says an “extra-strict field” mutation fails. The DD requires readers within protocol major 1 to ignore unknown object keys. A schema with `additionalProperties:false`, or a test expecting an unknown extension to fail, would contradict the wire contract. The `log_auction` test also names only one direction of the `resolution` iff rule. — E1 — plan lines 193-241; DD line 25 and lines 127-139.

Required: state that unknown wire keys validate, test one on every message family, and reserve failure mutations for missing/type-invalid required fields. Test both halves of `resolution` iff `outcome==resolved`: resolved-without-resolution fails and nonresolved-with-resolution fails.

### MR6 — `replay_frames` does not prove the locked sufficiency contract

The proposed `Frame` exposes board, budgets, forced, a `bids` dictionary, and resolution, while the DD requires replay of both sealed submissions/intents, every retry, timing, recovery, analysis info, and terminal reasons. The sole three-ply/missing-`post_board` test can pass while silently dropping those required facts. — E1 — plan lines 243-267 and 374-386; DD line 138.

Required: define the replay projection explicitly enough to retain attempts with per-seat bid/move/info/timing/validation, recovery records, and terminal result/reason, or provide a separate lossless replay model. Add log-only tests for a clean ply, double-fault retry, single-fault recovery, recovery abort, and terminal event; each must assert the consumer-visible fields, not only schema validity.

### MR7 — Two lifecycle/boundary acceptance paths can go green without the promised proof

The supervisor exposes only immediate `kill()`, while the DD requires best-effort `game_end`, stdin close, `shutdown_grace_ms` wait, then process-group kill with delivery status logged. Separately, Task 12 permits the real theory-fixture integration to skip, yet final acceptance claims the loader seam at E2. A green suite containing that skip is not evidence that the owner-produced fixture contract was consumed. — E1 — plan lines 269-327, 362-397; DD lines 70-76, 153, 160, 166.

Required: add a graceful terminal/shutdown interface and tests for delivery success/failure, grace exit, and forced group kill. Permit the theory integration skip during development only; at final acceptance, either run it against the landed owner fixtures or report the boundary criterion pending/blocked rather than calling E2 complete.

## Preserved strengths and scope

- TDD task order, integer-only money, canonical JSON, per-fault fixtures, both recovery-fault dispositions, both pair parities, and the no-engine/no-theory edit fences are appropriate.
- No finding reopens R1-R5, the design digest, R5 bridge deferral, or ownership boundaries.
- Root lint/waiver, `SCOPE_DIFF_RESULT: all-in`, cross-bundle collision, and a live addressed `DISPATCH IMPL` remain later dispatch gates; this review grants no implementation authority.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-083256.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-083256`
- source/tests/plan/design=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-083215.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-083256.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-082602.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-082457.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/

## Requested successor

Return `harness-c1-plan-2` with the same `PLAN_LOCK_ID`, a fresh digest, MR1-MR7 dispositioned, and `IN_REPLY_TO` pointing here. Keep the approved design identity/digest unchanged.
