ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c2-plan-1
PARENT_DISPATCH_ID: s2-theory-c2-design-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan gate is the Implementer plan review; merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-orchestrator-planner-20260828-113001.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 1fd34cf6dd78dae8c2fe21bd120d50eb8a0eced869711c1bc4dd5d9b31dc317e
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@709dbca
TARGET_BRANCH: main
SUBJECT: PLAN s2-theory-c2 — certschema conformance corpus — PLAN-REVIEW REQUEST

## Team theory — theory-c2: PLAN for review

Current scope for the TO addressee: read-only PLAN-REVIEW.
No implementation until a separate dispatch; delegated conditional dispatch authority rides the orchestrator proceed relay (s2-theory-c2-proceed-2) and binds only after your approve + mechanical SCOPE_DIFF all-in.

Locked design: DD-theory-c2-20260828 rev3 (sha256 pinned above), approved by you at s2-theory-c2-design-review-4.
Plan: docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-theory-c2-20260828.md, committed at 709dbca (sha256 pinned above).

Scope (locked): materialize the DD §9 conformance corpus as the theory-c2 IMPL bundle —
`theory/certschema/` Python package (xxh wrapper, game rules pinned to fixtures v1, DD §4/§7 wire builders, positives P1–P3 + golden verdict, 42 table-driven mutants with the integrity re-seal rule, deterministic generator CLI), committed artifact bytes, catalogue.json, README, and `theory/pyproject.toml` wheel-packages line.
Full file targets, interfaces, code, and TDD steps are in the plan (7 tasks).

Acceptance criteria (from the plan):
1. Committed golden vectors byte-equal DD Appendix B hex (E2 tests).
2. Regeneration determinism: fresh run byte-equals committed artifacts (E2).
3. game.py passes every consumable fixtures-v1 case with fixtures unmodified (E2).
4. catalogue.json covers the DD §9 trace table exactly: 42 mutants (H01-14, R01-20, V01-08) + GATE-01/02 rows (E2).
5. Full theory suite green; no edits outside the plan's file lists.

Out of scope (anti-half-fix): no checker (c3 proves mutant rejection), no alternating fixture profile (c3), no rule kernel (c4), no value-conjecture memo, no DD/fixtures-v1/engine//solver//referee/ edits.
Boundary contract: Writes: theory/certschema corpus (shared test truth). Reads: DD §§1-9 + Appendix B, fixtures v1. Downstream consumers: c3 checker test suite; solver-pair emitter developers. Proof: E2 byte-match + determinism tests. No-consumer action: not applicable (consumers commissioned).
Verification target: E2.
Ceremony: tier stays large; no downgrade requested (no ESCALATION_SCAN downgrade justification needed).
Operator-judgment items: none — all judgment calls were taken in the grill (G1-G8) and rulings R-s2-1..4.

Known review lenses I flag proactively:
- Task 3's fixture adapter is written against my reading of SCHEMA.md; the plan instructs aligning the adapter to the actual envelope, never the fixtures.
- Mutant constructions were adjusted in plan self-review so each fires its OWN check first (MUT-R11 via valid-tag claim flip; MUT-R19 via header-root swap with untouched records); verify the ordering logic against DD §6.
- P3 masks are hand-derived; the plan's Step-1 tests re-verify them mechanically before any cert is built.

Requested response:
PHASE: PLAN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-plan-1, PLAN_LOCK_ID: PL-theory-c2-20260828, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plan committed at 709dbca (main), sha256 1fd34cf6dd78dae8c2fe21bd120d50eb8a0eced869711c1bc4dd5d9b31dc317e; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
(harness lock only; taken immediately before filing — s2 relay files from earlier this cycle were committed by the orchestrator batch)
