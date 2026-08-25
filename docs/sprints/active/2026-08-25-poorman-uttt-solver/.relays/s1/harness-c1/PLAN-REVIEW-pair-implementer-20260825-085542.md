ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-plan-review-3
PARENT_DISPATCH_ID: harness-c1-plan-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — two residual plan contradictions remain within the Planner's authority
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/PLAN-planner-20260825-085204.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: 2745d3d3b3c381838416f263c749ce15f35725c2e5a296f029a367d6fd21cd97
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW reissue 3 — two residual supervisor-interface contradictions

## Fold disposition

- R2 is closed: the tests now distinguish consecutive same-mover ownership from the alternating 0-0 owner-wins chain. — E1 — plan lines 27 and 368.
- R3 is closed: required additive `ply` and `trigger_request_id` recovery keys plus the raw ordered event timeline are compatible elaborations of the DD's total, replay-sufficient log contract; they do not change a pinned game or protocol semantic. — E1 — plan lines 241, 266-286; DD lines 123-139.
- R4's configured-grace data path is present in Task 9, but the exact Task 8 interface still contradicts it as detailed below. — E1 — plan lines 299-305 and 364.
- `BASE: main@63b4b7b` was current when reissued and HEAD has since advanced to `989533a`; the existing dispatch-time rebase plus fresh no-collision `SCOPE_DIFF` condition correctly owns that ordinary drift. Root INDEX lint is currently red, including the concurrent 085240→085204 inversion, so the declared root-lint-green-or-operator-waiver dispatch condition remains live. — E1 — incoming relay lines 26 and 29; current `git rev-parse`; root lint.

## Residual must-revise findings

### R1 — The stale-output check does not cover bytes waiting in the OS pipe

The revised contract makes `send_line` reject bytes already in the per-engine buffer, but it does not require `send_line` to perform a non-blocking stdout drain before inspecting that buffer. Output arriving after `sweep_extra()` can therefore remain unread in the OS pipe: the internal buffer is empty, `send_line` writes the next request, and `read_reply` can accept the stale line as the new reply (usually classifying it as `wrong_request_id`) rather than the required `extra_protocol_line`. The delayed-write test is also scheduling-dependent: a non-blocking sweep cannot catch a line written 50 ms after both reader threads have already returned. — E1 — plan lines 306-336; DD lines 21-25 and 104-105.

Required: make the pre-request boundary explicit. `send_line` must first non-blockingly drain all currently available stdout bytes from the OS pipe into the framing buffer, then return `extra_protocol_line` without writing if even a partial byte is present. Split the delayed-write proof into deterministic schedules: (a) hold the companion reply open long enough that the delayed complete line exists before the final sweep and faults that auction; (b) emit after the final sweep but before the next request and prove the pre-send drain faults the upcoming submission without writing it. Do not promise that an instantaneous non-blocking sweep observes bytes that have not arrived yet.

### R2 — The exact `Engine.__init__` interface still declares two signatures

Under “Produces (exact),” the executable signature at lines 299-301 omits `shutdown_grace_ms`; a later comment at lines 304-305 supplies a different signature that includes it. Task 9 constructs `Engine(..., shutdown_grace_ms=cfg.shutdown_grace_ms)`. A literal implementation of the first exact signature rejects the planned construction, so R4 is not yet unambiguous. — E1 — plan lines 294-305 and 364.

Required: replace the two declarations with one executable exact signature containing `shutdown_grace_ms: int = 2000`, and retain the configured non-default supervisor and E2E assertions.

## Preserved gates

- Scope, design/GRILL locks, TDD order, acceptance fixtures, ownership fences, recovery correlation, and theory-seam honesty remain approved in substance.
- This review grants no implementation authority. A fresh approving review, addressed bare `DISPATCH IMPL`, dispatch-time base/scope/collision proof, and root lint green or operator waiver remain separate gates.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085542.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-085542`
- source/tests/plan/design=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085542.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-085328.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-085443.md

## Requested successor

Return `harness-c1-plan-4` with the same `PLAN_LOCK_ID`, a fresh digest, both residuals dispositioned, and `IN_REPLY_TO` pointing here. Preserve the approved design identity/digest.
