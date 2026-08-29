ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-plan-review-1
PARENT_DISPATCH_ID: s2-theory-c2-plan-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can correct the fixture and mutant-construction defects without changing the approved design or G1-G8
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-pair-planner-20260828-115015.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 1fd34cf6dd78dae8c2fe21bd120d50eb8a0eced869711c1bc4dd5d9b31dc317e
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@709dbca
TARGET_BRANCH: main
SUBJECT: PLAN-REVIEW theory-c2 — must-revise fixture contract and mutant first-failure isolation

## Scope and identity

Reviewed the exact 923-line `PL-theory-c2-20260828` bytes at SHA-256 `1fd34cf6dd78dae8c2fe21bd120d50eb8a0eced869711c1bc4dd5d9b31dc317e`, committed as the sole artifact in current-main commit `709dbca770512025ec47eab8410403aa3e457f45`. The PLAN correctly parents to the approving design review `s2-theory-c2-design-review-4`, pins the approved design digest `629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c`, and stays within the orchestrator's theory-c2 corpus boundary. No source, test, fixture, design, plan, branch, commit, or PR change was made.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The architecture, target files, golden-byte lock, P2/P3 proof shapes, generator determinism goal, checker-independence boundary, and no-checker/no-fixture-edit scope are accepted and should be preserved. Implementation dispatch remains held because the fixture adapter/acceptance gate is not executable against current fixtures-v1 and several mutant constructions do not reach their named first failure.

## Acceptance-criteria disposition

1. Golden vectors byte-equal Appendix B: PASS at plan level. The embedded constants and builder layout match the approved DD.
2. Regeneration determinism: PASS as an approach; the artifact comparison covers the named corpus outputs.
3. `game.py` passes every consumable fixtures-v1 case: FAIL — MR-1.
4. Exact 42-mutant catalogue plus GATE-01/02: FAIL as conformance truth — MR-2/MR-3.
5. Full suite and scope fence: baseline PASS (`99 passed`), file fence PASS; task commit commands still omit two newly created tests — MR-4.

## Confirmed sound constructions

Independent evaluation of the plan's rules model reproduced the DD golden root and verified:

- P2 parent is well-formed at counts 8/8; the transported winning move is legal and its canonical child equals the golden terminal root.
- P3 starts well-formed at counts 13/12 with exactly one O reply `(5,8)`; the child is forced to open board 8, and X's `(8,8)` reply creates the macro X win.
- Current prerequisites exist (`xxhsum 0.8.3`, `uv 0.11.15`), and the unmodified theory baseline is `99 passed in 96.68s`.

## MR-1 — replace the hypothetical fixtures-v1 adapter with an exact compatible-case contract

Plan lines 323–358 do not match the current fixture schema:

- `state.board` is an array of nine nine-character strings over uppercase `X`/`O`/`.`, not one 81-character lowercase string.
- Legality expects `expected_legal_moves`; closure uses pair-valued `move`, `expected_forced`, and `expected_terminal`; terminal uses `expected_result`. There is no nested `expect` object, and `move // 9` is invalid for the current pair-valued move.
- `SCHEMA.md` also requires consumers to honor each row's `consumed_by` list.

This cannot be repaired by field renaming alone. Running the plan's alternating rule model over the correctly decoded fixtures produced:

- `legality-forced-closed-means-free-choice`: got no moves, expected six free-choice moves. Under DD §2.6 the row's `forced=4` pointing at a closed board is ill-formed, so silently normalizing it would contradict the design.
- `terminal-all-closed-chip-margin-plus-one`, `terminal-all-closed-chip-margin-minus-one`, and `terminal-final-payment-before-chip-comparison`: the s2 rule lock correctly returns draw, while fixtures-v1 returns X/O from auction chip semantics.

Required revision:

1. Enumerate the exact fixtures-v1 case ids and fields whose semantics are shared with the s2 alternating lock; do not defer envelope discovery to implementation.
2. Explicitly exclude or separately characterize auction-only rows and DD-ill-formed forced-closed rows. Preserve fixtures-v1 bytes and keep the additive alternating profile in c3 as designed.
3. Rewrite Task 3 tests and acceptance criterion 3 around that exact compatibility matrix, including `consumed_by`, actual field names, actual move shape, and explicit result mappings.
4. Add direct s2 tests for the excluded rule-lock semantics rather than treating auction outcomes as alternating truth.
5. Replace Task 2's vacuous overlap assertion (`assert not ... or c.x[0] == 0`) with a known occupied X bit and an unconditional rejection assertion; cover parity, high mask bits, and forced-to-closed refusal non-vacuously.

## MR-2 — make verdict mutants resolve their intended member before later-step failures

The generator writes verdict mutants to `theory/certschema/mutants/MUT-V*.bin`, but the golden verdict's member path remains `golden-terminal-winx.utc`. From a mutant manifest that path resolves to `theory/certschema/mutants/golden-terminal-winx.utc`, while the plan creates the certificate only at `theory/certschema/vectors/golden-terminal-winx.utc`.

Consequently MUT-V04 through MUT-V08 would fail at §7 step 2 (absent member) before their intended digest, claim-binding, shared-field, or completeness checks. Required revision: define the actual on-disk member-path strategy for every verdict mutant and add a construction test proving that V04–V08 resolve the valid golden certificate from their emitted location. V02 remains deliberately path-invalid and V03 deliberately absent; those two must stay distinct.

Also correct V02's wire instruction: `golden-terminal-winx.utc` is 24 bytes and `../x.utc` is 8, so the replacement is not same-length and must be rebuilt with the correct `str16` length.

## MR-3 — isolate the named mutant predicates

- V07 flips root-state byte 12 of the golden root. Independent decoding gives counts 10/8, fails DD parity, and is non-canonical, so it rejects at §7 step 1 rather than the intended step-6 member/root mismatch. Use a different valid, canonical, well-formed root (the R19 alternative is a suitable pattern) so parsing succeeds and only the cross-member field comparison fails.
- H10 swaps complete ply-27 and ply-26 manifest rows. In P3 this changes the first declared offset from header end 269 to 317, creating a 48-byte initial gap as well as the claimed row-order violation. Construct an order-only header mutant while preserving contiguous offsets and valid manifest integrity.
- R14 says both “reseal all” and “except header.” State exact fields: keep header rule registry count zero, but make chunk counts, manifest record count/digests, offsets/lengths, and header totals mutually consistent so the stream-level RULE refusal is the first defect.
- Task 6 Step 3 still requests a `drawn_terminal()` helper for R11 although the normative row now uses a valid WIN_O header-claim flip over the X terminal. Remove the stale alternative so the plan has one construction.

Add construction-level assertions for the above rows that prove all predicates preceding the named refusal remain valid. This is not checker implementation: the existing planned parser/reseal machinery can verify framing, counters, integrity, canonical/well-formed states, and path resolvability without deciding the target refusal.

## MR-4 — preserve new tests in task commits

Task 3 creates `theory/tests/test_certschema_rules.py` and Task 6 creates `theory/tests/test_certschema_mutants.py`, but both Step-5 commands use `git commit -am`, which does not include untracked files. Replace each with an explicit `git add` of the task's complete file list followed by `git commit`. Keep the per-task commit boundary and final exact scope check.

## Required successor

Amend `PL-theory-c2-20260828` forward while retaining `PLAN_LOCK_ID`, float `PLAN_SHA256`, and preserve the accepted design/scope surfaces above. Reissue the pair-Planner PLAN with a unique successor dispatch id, parent it again to `s2-theory-c2-design-review-4`, and carry this review in `IN_REPLY_TO`. Do not issue delegated implementation dispatch on this verdict.

OPERATOR_ITEMS: none — all required changes make the existing DD and orchestrator corpus scope executable; they do not change G1-G8 or product semantics.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only plan review
- migration/backfill/destructive-write/canonical-data-repair: no — no data actions
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof test corpus only
- AI-or-automation-acts-downstream: no — generator execution remains future implementation scope
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — corpus is shared truth for the theory checker and solver emitter
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — emitter-side Python and future independent C3 checker roles remain explicit
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — verdict refuses rather than accepts the exposed gaps
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming PLAN relay exact-file lint passes; PLAN/DESIGN digests and current-main commits match the pinned bytes.
- E1: approving design-review lineage and superseding orchestrator proceed relay verified on disk and in INDEX.
- E2: current theory baseline `uv run pytest -q` — 99 passed in 96.68s.
- E2: independent DD-rule model reproduced the golden root and validated P2/P3 state, parity, forced-board, move, canonical-child, and terminal claims.
- E2: current fixtures-v1 envelope decoded directly; exact compatibility run exposed one legality and three terminal semantic failures after field-shape correction.
- E2: byte/path checks proved V02's 24→8 length change, V07's 10/8 invalid parity plus non-canonical state, H10's 48-byte offset gap, and V04–V08's unresolved emitted member path.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR changes claimed; authored only this PLAN-REVIEW relay for daemon filing; reviewed plan bytes are clean at 709dbca770512025ec47eab8410403aa3e457f45
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
