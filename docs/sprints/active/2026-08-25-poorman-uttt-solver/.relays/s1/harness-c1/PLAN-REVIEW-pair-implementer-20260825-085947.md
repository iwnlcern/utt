ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c1-plan-review-4
PARENT_DISPATCH_ID: harness-c1-plan-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan is technically approved; delegated dispatch remains conditioned on root lint green or operator waiver
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/harness-c1/PLAN-planner-20260825-085754.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: 11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: 79ea02702b4dc9d2755981e55c2b9ba099f0c47df7578ac1face570f85eae600
PLAN_REVIEW_VERDICT: approve
SUBJECT: PLAN-REVIEW reissue 4 — approved; dispatch root-lint gate preserved

## Verdict

Approve the locked plan at digest `79ea02702b4dc9d2755981e55c2b9ba099f0c47df7578ac1face570f85eae600`.

## Finding disposition

- Prior R1 is closed. `send_line` now normatively drains all currently available OS-pipe stdout before inspecting the framing buffer, faults on any complete or partial byte without writing, and only then writes a clean request. The two delayed-output tests use deterministic schedules: sweep-visible output faults the current auction, while post-sweep output is caught by the next pre-send drain. — E1 — plan lines 299-336; DD lines 21-25 and 104-105.
- Prior R2 is closed. Task 8 contains one executable exact `Engine.__init__` signature with `shutdown_grace_ms: int = 2000`; Task 9 passes `cfg.shutdown_grace_ms`, and both supervisor and E2E assertions retain a non-default value. — E1 — plan lines 299-304, 320-336, and 349-364.
- The earlier tie-owner and replay-association corrections remain coherent. Required recovery correlation keys are additive log-schema elaboration serving the DD's total replay contract; the raw ordered timeline remains available. — E1 — plan lines 241, 266-286, and 368; DD lines 123-139.

## Locked execution surface

- Design identity and digest match the approving `harness-c1-design-review-3` lineage. Plan identity is unchanged and its floated digest matches the exact bytes on disk.
- Scope remains `docs/protocol/**` and `referee/**`, with theory fixtures read only through the loader seam; `engine/**`, `theory/**`, bridge/network/platform, merge, deployment, and release remain out of scope.
- TDD order, the ten-class fault matrix, deterministic framing/recovery fixtures, lossless replay proof, lifecycle shutdown proof, integer-only money, acceptance criteria, and the theory-seam pending/blocked honesty rule are implementation gates.
- `BASE: main@989533a` matches review-time HEAD. Any later drift is owned by the required dispatch-time rebase, fresh no-collision `SCOPE_DIFF`, and all-in result.

## Separate dispatch hold

Approval is not implementation authority. Root lint is currently red on ten inherited/concurrent append-order inversions, including 085240→085204 and the post-approval 085947→085908 append race. The delegated dispatch condition therefore remains unsatisfied until root lint is green or an operator explicitly waives it. Only after every declared condition is met may the Planner issue a fresh relay parented to this approval with the exact addressed bare implementation token.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085947.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-085947`
- source/tests/plan/design=none — review-only

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085542.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085947.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-085754.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-085835.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-085328.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-085705.md

## Next requested action

Hold implementation while root lint is red. Once every delegated-dispatch condition is evidenced, issue the separately addressed implementation dispatch parented to `harness-c1-plan-review-4`; otherwise relay the unmet condition upward.
