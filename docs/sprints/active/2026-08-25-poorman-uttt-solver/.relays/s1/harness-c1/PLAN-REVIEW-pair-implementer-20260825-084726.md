ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-plan-review-2
PARENT_DISPATCH_ID: harness-c1-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — four residual technical contradictions remain within the Planner's authority
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/PLAN-planner-20260825-084300.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: 394dafb377b63f14652b7da448b12f1894d92359ff53dd6652c6a6ab5a2225fe
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW reissue 2 — four residual execution contradictions

## Target and fold disposition

- The plan and design digests match the requesting relay. `BASE: main@19a15ed` equals current HEAD, and dispatch-time rebase plus fresh no-collision `SCOPE_DIFF` is now explicit. MR1 is closed. — E1 — `sha256sum`, `git rev-parse`, plan line 26.
- MR2's early pre-LF oversize path, buffered second-line path, direct supervisor tests, and E2E fault fixtures are present; one late-extra/stale path remains below. — E1 — plan lines 281-316.
- MR3's single-fault apply/terminal-before-recovery order is correct. MR5 schema compatibility, MR6 lossless replay fields/tests, and MR7 boundary honesty are substantially folded, subject to the residuals below. — E1 — plan lines 232-279, 318-346, 381-416.

## Residual must-revise findings

### R1 — Late extra protocol output is silently discarded instead of faulted

`read_reply` classifies `extra_protocol_line` only when a second complete line is already buffered behind the accepted reply. If the extra line arrives just after `read_reply` returns, `send_line` later “discards+counts” it and returns no status, so the engine escapes the required fault and stale output is erased without an auction consequence. This leaves the exact ten-class contract timing-dependent and does not fully close MR2. — E1 — plan lines 296-310, 313-316; DD lines 21-25 and 85-91.

Required: pin a collection boundary that observes/quarantines late stdout before the auction is resolved, or make stale pre-request bytes surface as an explicit `extra_protocol_line` fault through a return/status path that triggers R2 recovery. Never silently discard protocol bytes. Add a test where the second line arrives in a separate delayed write, not the same buffered write.

### R2 — The tie-owner test contradicts the pinned transition

Global Constraint 27 correctly states `tie_owner' = opponent(actual mover)`. Task 9 then says “consecutive paid wins by the same engine flip `tie_owner` each ply.” They do not: if X moves twice, the owner is O after both moves. Only alternating actual movers (including a 0-0 owner-wins chain) flip it. A literal implementation of the named assertion will reject correct behavior or teach the wrong rule. — E1 — plan lines 27 and 343-346; fixed spec line 58.

Required: change the consecutive-same-winner expectation to “remains the opponent of that winner,” retain the fault-applied case, and use the 0-0 chain to assert alternation.

### R3 — Replay recovery association conflicts with the corrected event order

`Frame.recoveries` is defined as recovery events preceding that ply's `auction`. Task 9 correctly emits a resolved single-fault auction first and recovers afterward only if the game continues. The single-fault recovery test nevertheless requires that recovery in the frame. Under the exact interface it is either dropped or incorrectly attached to the next ply. Double-fault recoveries precede their auction; single-fault recoveries follow it. — E1 — plan lines 260-278 and 343-345; DD lines 95-100 and 137-138.

Required: define lossless association for both directions, preferably by giving recovery events an explicit triggering `ply`/`request_id` in the normative schema if permitted by the DD's forward-compatible event shape, or by retaining the original ordered event timeline alongside frames and deterministically associating post-auction single-fault recovery with the prior frame. Test both single- and double-fault placement.

### R4 — `Engine.finish` cannot receive the configured shutdown grace

`GameConfig` owns `shutdown_grace_ms`, but `Engine.__init__` does not, and `finish(game_end)` has no grace parameter while promising to wait that configured duration. The exact interfaces leave no data path from the logged game setting to the supervisor behavior. — E1 — plan lines 291-316 and 327-345; DD lines 70-76.

Required: pass `shutdown_grace_ms` into `Engine` construction or `finish`, and assert the configured non-default value in the graceful-exit/forced-kill tests rather than only relying on a module default.

## Preserved gates

- Scope, DD/GRILL locks, TDD order, acceptance fixtures, ownership fences, and the theory-seam pending/blocked rule remain appropriate.
- No finding reopens R1-R5 or requests a design change. Root lint/waiver, dispatch-time rebase and all-in scope proof, no-collision check, and a live addressed `DISPATCH IMPL` remain separate gates; this relay grants no implementation authority.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-084726.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-084726`
- source/tests/plan/design=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-084547.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-083256.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-084726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-082602.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-084300.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-083444.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-082457.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-084431.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/

## Requested successor

Return `harness-c1-plan-3` with the same `PLAN_LOCK_ID`, a fresh digest, R1-R4 dispositioned, and `IN_REPLY_TO` pointing here. Preserve the approved design identity/digest.
