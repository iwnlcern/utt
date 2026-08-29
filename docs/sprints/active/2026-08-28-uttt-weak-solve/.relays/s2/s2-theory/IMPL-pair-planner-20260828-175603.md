ROLE: Pair Planner
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: s2-theory-c2-impl-1
PARENT_DISPATCH_ID: s2-theory-c2-plan-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — delegated conditional dispatch per s2-theory-c2-proceed-2; merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 4558eeda83cdc6e94b2bf45e93161856b7ab1cda6ac53706926504103d4bd5f7
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@d305625
TARGET_BRANCH: main
BRANCH: theory/certschema-c2
SUBJECT: DISPATCH — theory-c2 implementation (PL-theory-c2-20260828 rev4, approved)

## Team theory — theory-c2: implementation dispatch

Delegation basis: orchestrator PLAN relay s2-theory/PLAN-orchestrator-planner-20260828-113001.md (s2-theory-c2-proceed-2) carries DELEGATED_DISPATCH_AUTHORITY: yes.
Conditions, checked mechanically before this token:

- Implementer plan review = approve: s2-theory-c2-plan-review-4 (s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md), whose PARENT_DISPATCH_ID is my PLAN s2-theory-c2-plan-4 addressed to you in TO.
- This dispatch's PARENT_DISPATCH_ID points to that approving review.
- SCOPE_DIFF below returns all-in against the proceed relay's in-scope boundary (theory-surface schema/vector/mutant-test artifacts; no checker, no solver/, no engine/).
- No new hard trigger: the standing cross-pair shared-contract trigger is the one the delegation anticipated; tier stays large, no downgrade requested, the heavier workflow is in use.
- No boundary-contract deviation; no cross-bundle collision (solver-c2 design touches no theory/ path).

SCOPE_DIFF:
- theory/certschema/__init__.py -> in
- theory/certschema/xxh.py -> in
- theory/certschema/game.py -> in
- theory/certschema/wire.py -> in
- theory/certschema/vectors.py -> in
- theory/certschema/generate.py -> in
- theory/certschema/README.md -> in
- theory/certschema/vectors/ -> in
- theory/certschema/mutants/ -> in
- theory/certschema/catalogue.json -> in
- theory/tests/test_certschema_xxh.py -> in
- theory/tests/test_certschema_game.py -> in
- theory/tests/test_certschema_rules.py -> in
- theory/tests/test_certschema_wire.py -> in
- theory/tests/test_certschema_vectors.py -> in
- theory/tests/test_certschema_mutants.py -> in
- theory/pyproject.toml -> in
SCOPE_DIFF_RESULT: all-in

Every row is inside the theory surface named by the proceed relay; the pyproject.toml row is the wheel-packages line the reviewed plan specifies (Task 1), reviewed twice with the orchestrator in CC.

Execution:

- Plan of record: docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-theory-c2-20260828.md revision 4 at commit d305625, sha256 pinned above; execute task-by-task per its header instruction (superpowers executing-plans or subagent-driven-development).
- Branch theory/certschema-c2 off main@d305625; per-task commits per the plan's Step-5 commands; exit with branch + PR + implementation report relay (PHASE: IMPL, PARENT_DISPATCH_ID: s2-theory-c2-impl-1).
- Scope fence is the SCOPE_DIFF list verbatim; any file outside it stops work and relays a deviation — no self-expansion, no exceptions.
- Verification target E2: full theory suite green from theory/ (`uv run pytest`), byte-equality and determinism tests passing.
- Merge is NOT authorized by this dispatch; the merge gate is a separate operator/orchestrator relay.

DISPATCH IMPL

ACTIONS_GIT_REF: none — dispatch relay only; no edits claimed by this seat this turn
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(daemon lock/INDEX, this cycle's uncommitted relay chain, and solver-pair artifacts — none authored by this dispatch; tracked tree clean at BASE main@d305625)
