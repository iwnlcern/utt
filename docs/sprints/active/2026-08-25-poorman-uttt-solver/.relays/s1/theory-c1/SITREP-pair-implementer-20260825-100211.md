ROLE: Pair Implementer
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: theory-c1
PARENT_DISPATCH_ID: theory-c1-impl-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — technical plan/literature contradiction requires Planner correction before implementation resumes
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-impl-2/IMPL-pair-planner-20260825-095115.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 943d25db26fce8d9891ee5989cbd5d8be5946a63bd7cf498f738a02a8c6d8cbe
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: 577cf979185e3fc753483890246ae8d37af419b6e5a6a2cc7ab42e494263ef17
BRANCH: theory/stage1-c1
BASE: main@d89f006
TARGET_BRANCH: main
SUBJECT: theory-c1 IMPL HOLD — Task 5 draw terminal contradicts the published 133/256 self-check

## SITREP — theory.implementer / theory-c1

Phase: IMPL dispatched; execution stopped at Task 5's mandatory Richman self-check.
Current artifact: `PL-theory-c1-20260825` at the locked digest above, implemented through committed Task 4 on `theory/stage1-c1`; Task 5 test and implementation remain uncommitted for diagnosis.

Claims:
- Task 5 specifies Richman backup `R = (a + b)/2`, X-line terminal 0, O-line terminal 1, full-draw terminal 1/2, and empty-root expectation 133/256. — E1 — `plans/PL-theory-c1-20260825.md:276-300`.
- The specified implementation deterministically returns 1/2 at the empty board, failing only the 133/256 assertion; the full suite is 1 failed, 21 passed. — E2 — `cd theory && uv run pytest -q` this turn.
- A diagnostic run changing only the full-draw terminal gives: draw=0 -> 123/256, draw=1/2 -> 1/2, draw=1 -> 133/256. — E2 — exact-Fraction local computation over the same ascending-empty-count DAG this turn.
- The draw=1/2 result is structurally forced: swapping X/O complements every terminal value and the average backup preserves the complement relation; the empty board is swap-invariant, so its value must equal its complement and therefore equals 1/2. — E1 — finite-DAG induction over the locked recurrence.
- Develin and Payne obtain the published 133/256 result for a reduced game `TTT` in which Bob is declared the winner of an ordinary tie; that is the diagnostic's draw=1 convention, not the locked Task 5 draw=1/2 convention. — E1 — Develin and Payne, *Discrete bidding games*, Section 6 and Theorem 6.1, https://doi.org/10.37236/357.
- The locked DD requires `richman.py` to reproduce published 133/256 as a machinery self-check, while the canonical project rule retains its separate chip-comparison/full-draw semantics. — E1 — `designs/DD-theory-c1-20260825.md:104,146`; canonical spec line 65.

Actions:
- Validated the addressed implementation grant, lock digests, dispatched base, daemon gate, and isolated worktree before implementation.
- Completed Tasks 1-4 under TDD with exact-path commits and passing tests at each checkpoint.
- Added Task 5's failing test and the literal locked recurrence/terminal implementation, reproduced the failure, ran systematic diagnosis, and stopped without changing the locked terminal or expected value.
- Preserved all main-worktree foreign/concurrent bytes; no push, PR, merge, or out-of-scope edit.

ACTIONS_GIT_REF:
- branch=theory/stage1-c1@b589dc45b3c7ece219814e14f6f456470621e61c
- commits=c91dbc9 (Task 1), 25d6add (Task 2), 5cdf98d (Task 3), b589dc4 (Task 4)
- uncommitted_task5=theory/auction_ttt/richman.py, theory/tests/test_richman.py
- implementation_worktree_status:
  ?? theory/auction_ttt/richman.py
  ?? theory/tests/test_richman.py

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-pair-implementer-20260825-095726.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-4/

Done: Tasks 1-4; Task 5 RED and diagnostic isolation; lock and scope checks.
Not done: Task 5 GREEN/commit; Tasks 6-13; complete acceptance suite; findings/fixtures/results; push; PR; math-lock handoff; merge.
Blocked: Task 5 simultaneously requires a draw=1/2 terminal and the published 133/256 root, which are incompatible under its locked recurrence.
Scope drift risk: high if I silently change either the terminal convention or the asserted oracle value; the plan says to stop on disagreement.
Tests / verification: `cd theory && uv run pytest -q` -> 1 failed, 21 passed in 0.26s; failure is `Fraction(1, 2) != Fraction(133, 256)`. Lock SHA-256 values rechecked and match the dispatch.
Next requested action: correct forward with a successor plan. Recommended: scope Bob-wins-ties (`full draw -> 1`) only to the literature Richman machinery self-check so it reproduces 133/256, while leaving the canonical Stage-1 auction terminal semantics unchanged; obtain fresh Implementer PLAN-REVIEW approval and issue a successor implementation dispatch. If Task 5 is intended to use the canonical draw=1/2 payoff instead, revise the DD/acceptance target to 1/2 before re-review.
