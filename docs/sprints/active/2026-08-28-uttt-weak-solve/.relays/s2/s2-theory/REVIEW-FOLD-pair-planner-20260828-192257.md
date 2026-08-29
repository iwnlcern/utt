ROLE: Pair Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c2-fold-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold direction to the Implementer; merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/IMPL-pair-implementer-20260828-185514.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c2-20260828
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BRANCH: theory/certschema-c2
PR: https://github.com/iwnlcern/utt/pull/12
SUBJECT: REVIEW-FOLD theory-c2 — panel verdicts on PR #12 @ 45994d3: 2 must-fix, no blockers

## Panel record

PANEL_CHOSEN: custom
DEFAULT_ROLES_CHANGED: yes
WHY_THIS_PANEL: PR is a local deterministic Python test-corpus generator with committed binary artifacts; correctness IS byte-level conformance to the locked DD, and the dominant risk classes are spec drift, false-green tests, and mutant mis-targeting. No authz/network/secret surface (fixed-argv xxhsum subprocess only) and no runtime/perf surface, so security and performance lenses were dropped and replaced with an independent corpus adversary.
ROLES:
- correctness/design-conformance — DD §§2-7/Appendix A vs code, plan drift
- test-coverage/false-green — tautology and escape-route hunting in the test suite
- corpus-adversary — independent from-spec reimplementation of parser/verifier; re-derivation of every committed byte; measured first-failure per mutant

Planner pre-verification (this seat, before paneling): scope diff d305625..45994d3 exactly the dispatched 17-path surface; fixtures diff empty; golden cert (237 B) and verdict (139 B) byte-match DD Appendix B hex; adjacent member copy identical; full suite at head 140 passed — E2.

## Verdicts

- correctness/design-conformance: approve. D4 tables, 37-byte serialization, wire layout, Appendix A rules, P2/P3 constructions, ruleset digest, catalogue census all independently re-derived and conformant (E1/E2).
- test-coverage/false-green: must-fix. Corpus itself verified correct and acceptance criteria 1-5 genuinely met, but the head-commit isolation battery has three PROVEN escape routes (planted-defect probes passed green).
- corpus-adversary: must-fix. Independent from-spec verifier: all four positives parse byte-exhaustively and verify semantically; golden verdict passes subgame mode and fails deliverable mode; ALL 42 mutants' measured first failure equals the intended target (including the head-commit R12/R13/R17 fixes and the requested H10/R14/V05/V07 attacks). One coverage hole in the corpus itself (MF-2).

Consolidated: NO blockers. Two must-fix items to fold before the merge recommendation. Optional items are your discretion per protocol.

## Must-fix (fold before merge)

MF-1 — isolation battery escape routes (test-coverage findings 1-3; all in theory/tests/test_certschema_mutants.py):
- MUT-H09's only check is a 4-zero-byte tail (`:153-155`); a garbage mutant ending in four zero bytes passes. Assert header-prefix identity: h09 equals build_p1()'s bytes up to and including a zeroed manifest_rows u32.
- MUT-H11's check `changed[0] < changed[-1]` (`:157-162`) accepts ANY ≥2-byte diff. Assert the exact changed-index set: one byte inside row-0 byte_offset plus the 16 manifest-digest bytes.
- The battery never diffs header fields of non-target mutants (`:99-150`): a mistargeted raw byte-patch (probe: MUT-H03 patched at reserved instead of claim) ships green. Add a per-mutant expected-changed-field table diffed via parse_cert (the reviewer prototyped this in ~10 lines and it caught all three planted defects); cover V02-V08's target fields in the same table.
Rationale: these tests are the only guard on future corpus regeneration; today's committed bytes are correct (independently verified), so this is regression armor, not a present corpus defect.

MF-2 — missing converse kind/terminality mutant (corpus-adversary finding 1):
- DD §9's row for MUT-R12 reads "non-terminal state marked TERMINAL (and the converse kind/terminality mismatch)" — the converse is named by the DD but no artifact exercises it.
- It is load-bearing: an O-won terminal with O-to-move parity is well-formed and side-consistent as an OPPONENT record; its legal-reply set is empty, so a checker missing the terminal→TERMINAL check verifies it vacuously and an O-won root can "prove" WIN_X while passing all 42 current mutants.
- Fold: add one mutant (suggested id MUT-R21) — base p1 or p2, terminal record's kind byte → OPPONENT (payload already 0xFF), reseal all; catalogue row maps to the SAME DD §9 trace row as MUT-R12 (the row's parenthetical already covers it — no DD edit, so no orchestrator schema routing); extend the census/expected-id lists and regenerate.

## Strongly recommended optionals (cheap, same vacuous-verification family; your discretion)

- P4 positive with an OPPONENT node having ≥2 legal replies, plus a mutant dropping a NON-FIRST reply's child: P3's reply set is a singleton, so a checker verifying only the first legal reply is currently corpus-conformant.
- Pin catalogue row count exactly (currently 48; adjust for additions) and exact-id-pin closure_routing in the fixture census.
- Catalogue reason wording: MUT-R17 → "manifest record_count ≠ fixed_count + rule_count" (currently ambiguous with R18 totals).

## Discretionary optionals (from the panel, no direction)

Rename test_p1_and_verdict_byte_match_dd to reflect its length-only assertion; dedupe _u16/_u32/_u64/_str16 into wire.py; memoize base-vector builds to cut xxhsum subprocess count; vectors/ dir census (not 4-name list) in the determinism test; second-side mutants for multi-refusal rows (H05 id-mismatch, H07 ill-formed root, V01 version/tag/reserved variants, R08 out-of-forced-board move); machine-readable first_failure field in catalogue rows for c3 parametrization.

## Ledger notes (recorded here, no code change requested)

- MUT-R17 and MUT-R10 deviate from the plan Task 6 table's literal mutation shape (R17 appends a real second record with stale row count; R10 drops the whole ply-26 chunk+row). Both deviations are CORRECT — the literal forms would fire a different first failure — and are hereby recorded as the plan-vs-artifact delta; the catalogue, not the plan table, is the c3 checker's expectation source.
- GATE-01's positive branch remains unit-level in c3 per DD §9; nothing in this bundle claims it.

## Fold mechanics

- All fold targets are inside the dispatched SCOPE_DIFF surface (vectors.py, generate.py output, catalogue.json, mutants/, theory/tests/test_certschema_*.py). Write your FOLD_SCOPE artifact before editing per protocol; any OUT row stops the fold.
- After folding: reply with the fold report (FOLD_SCOPE above ACTIONS_GIT_REF, FOLD_SCOPE_RESULT: all-in, new head sha, updated test count). I will run a TARGETED check (mutant census, new-mutant first-failure re-measurement, suite green) — not a full re-panel, since design and blast radius are unchanged.
- Merge remains a separate operator/orchestrator gate; after the fold check I will file the merge-gate recommendation to the orchestrator.

ACTIONS_GIT_REF: none — review synthesis relay; no edits claimed by this seat (panel ran read-only against a throwaway worktree at PR head 45994d31f2b2ca09b08b13352a2a2644eb427663)
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-185204.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-implementer-20260828-185514.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-planner-20260828-175603.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(daemon lock/INDEX plus both pairs' uncommitted relay chains and the solver design doc — none authored by this relay; tracked tree clean)
