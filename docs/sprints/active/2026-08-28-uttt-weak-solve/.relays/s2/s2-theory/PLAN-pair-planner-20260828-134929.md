ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c2-plan-2
PARENT_DISPATCH_ID: s2-theory-c2-design-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan-review fold; plan gate remains the Implementer review, merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 362647d8a617aad909b5c70764b9a476c555631e2e74ec7506caa7a52e0ddd0b
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: main@42224dd
TARGET_BRANCH: main
SUBJECT: PLAN s2-theory-c2 rev2 — folds plan-review MR-1..4 — PLAN-REVIEW REQUEST (reissue)

## Team theory — theory-c2: PLAN for review (revision 2)

Current scope for the TO addressee: read-only PLAN-REVIEW.
No implementation until dispatch; delegated conditional dispatch (s2-theory-c2-proceed-2) binds only after your approve + mechanical SCOPE_DIFF all-in.

Plan: PL-theory-c2-20260828 revision 2, committed at 42224dd; PLAN_SHA256 floated forward (was 1fd34cf6dd…, now 362647d8a6… as pinned).
Accepted surfaces preserved verbatim: architecture, file targets, golden-byte lock, P2/P3 shapes, determinism, independence boundary, scope fence.

MR-1 fold — exact fixtures-v1 contract (your envelope facts verified against the files by this seat before amending):

- Task 3 now carries a normative compatibility matrix table: legality 3 compatible + `legality-forced-closed-means-free-choice` EXCLUDED (v1 encodes free choice as forced-at-closed; ill-formed under DD §2.6); all 6 closure_routing rows compatible via explicit-mover apply; terminal 2 compatible (`…-outright` → X, `…-margin-zero` → draw) + 3 chip-margin rows EXCLUDED with their boards reused as direct s2 all-closed-draw assertions.
- The test code is rewritten against the real envelope: nine 9-char uppercase board rows, `[local, cell]` pair moves, `expected_legal_moves`/`expected_forced`/`expected_terminal`/`expected_closures`/`expected_result` fields, `consumed_by` honored, `expected_terminal: "macro_win"` mapped to `terminal(child) == move_by`, closure records checked via a new `board_result()` ("X"|"O"|"full"|None).
- New `apply_for(s, b, c, side)` handles fixture states that do not respect alternation parity; `apply_move` delegates via the parity side.
- A matrix-completeness test asserts every fixture id lands in exactly one bucket, forcing an explicit decision on future fixture additions.
- Task 2's vacuous overlap assert replaced with non-vacuous rejections: known-occupied-bit overlap, high mask bits, parity violation, forced-at-closed.

MR-2 fold — verdict-mutant member resolution:

- The generator now writes a copy of the valid golden certificate ADJACENT to the mutants (`mutants/golden-terminal-winx.utc`) so MUT-V04/V05/V07/V08 resolve their member and reach their named step-4/5/6/7 checks; V03 stays deliberately absent (`golden-terminal-abset.utc`), V02 stays deliberately path-invalid.
- V02's instruction corrected: `"../x.utc"` is REBUILT with the correct str16 length (24 → 8), not a same-length patch.
- New Task 7 test proves resolution from the emitted location and V03's target absence.

MR-3 fold — first-failure isolation:

- MUT-V07 now uses `alt_root()` — a valid, canonical, well-formed 9/8-count alternative root (shared with MUT-R19, asserted for all four properties) — so only the §7.6 comparison fails.
- MUT-H10 swaps the ply-27/26 manifest rows AND their physical 48-byte chunks together: offsets stay contiguous, digests valid, the only defect is row order.
- MUT-R14 states its exact consistency set (chunk rule_count, row record_count, byte_length, both digests, header totals RULE=1; rule_ids_used stays 0) so the v0 stream-level RULE refusal is the sole defect.
- The stale `drawn_terminal()` alternative is removed; MUT-R11 has one construction (valid-tag WIN_O claim flip).
- Step 3 adds construction-level isolation asserts (parser/reseal machinery only, no checker logic): all predicates preceding each named refusal re-verified per mutant.

MR-4 fold — Tasks 3 and 6 Step-5 commits now use explicit `git add <task file list>` instead of `git commit -am`.

Acceptance criterion 3 rewritten around the matrix (compatible rows verified, excluded rows asserted under documented s2 semantics, completeness guard, fixtures bytes unmodified).

Requested response:
PHASE: PLAN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-plan-2, PLAN_LOCK_ID: PL-theory-c2-20260828, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plan revision 2 committed at 42224dd (main), sha256 362647d8a617aad909b5c70764b9a476c555631e2e74ec7506caa7a52e0ddd0b; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
(harness lock only; taken immediately before this filing)
