ROLE: Pair Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: theory-c1-fold-1
PARENT_DISPATCH_ID: theory-c1-impl-report-6
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold authority rides the standing implementation lineage; P1–P3 ratification follows the fold on final bytes; merge remains fenced on engine-c3
FROM: theory.planner
TO: theory.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-impl-report-6/IMPL-pair-implementer-20260826-042445.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_SHA256: a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd
BUNDLE_ID: theory-c1
OWNER: theory
REPO: /Users/jack/Programming/utt
BRANCH: theory/stage1-c1 (fold onto a8bea8f; update PR #7)
SUBJECT: REVIEW-FOLD — adversarial panel findings for PR #7: 5 must-fix + 2 proof-wording must-haves; no library or math defects

PANEL_CHOSEN: custom
DEFAULT_ROLES_CHANGED: yes
WHY_THIS_PANEL: local math package, no auth/data/deploy surface — security lens swapped for an independent math-verification lens; four lenses: correctness-vs-docs, tests/false-green, math-verification, fixtures/consumers.
ROLES:
- correctness-vs-docs — plan/DD conformance and acceptance criteria (verdict: must-fix)
- tests-false-green — vacuity/tolerance/mutation reasoning (verdict: must-fix)
- math-verification — independent re-derivation and recomputation (verdict: APPROVE — all 22,186 T values, census, Richman 133/256 and 1/2, Kalai, and crosscheck-N8 independently reproduced; C8a induction numerically verified at >1M budget points, zero violations; C8a proof VALID in substance)
- fixtures-consumers — schema validation, empirical harness-loader run (PASS, zero errors), engine built and run against the bytes (fence characterized)

## Must-fix (blockers for the fold; fold onto the PR branch, FOLD_SCOPE before edits)

B1. Census count unpinned + FINDINGS overclaim. tests/test_zugzwang_synthetic.py:11–15 pins only zero-zugzwang/a≤b; nothing pins 22,186 (or 11,093 boards), yet FINDINGS.md:273 claims "The regression test hard-pins that count". Fix both sides: add `assert len(solved) == 22186` (and optionally the 11,093 board count) to the C11 test, and make the FINDINGS sentence exactly true.
B2. Regeneration check never test-covered. `fixtures --check` is CLI-only; drift between fixtures_gen.py and committed generated files leaves the suite green. Add a test asserting the check exits 0, plus a negative unit proving a tampered generated file is reported.
B3. Out-of-band population unguarded. tests/test_crosscheck.py:83–89 asserts only `out_of_band_mismatches == []` — vacuously green if a mask/band regression empties the out-of-band population. Assert the per-scale out-of-band population is > 0 (or pin today's counts: 160 / 6,060 / 162,856 at N=2/4/8).
B4. Kalai discrepancy undocumented (DD section 2 requires documentation either way). Exact root is T_lo = T_hi = 1/2 (ratio exactly 1:1) vs the E0 ~101.84:100 blog target — a large discrepancy recorded nowhere in EVIDENCE.md or the handoff. Add it to EVIDENCE.md with the exact-solver-authoritative disposition, and carry it in your amended handoff.
B5. legality-forced-closed-means-free-choice (theory/fixtures/legality.json) tags `engine` in consumed_by, but the landed engine treats forced-to-closed as an import invariant (Position::from_parts → ImportError; fatal REQUIRE aborts the whole TEST_CASE, masking the real fence failures). This is a permanent representational conflict engine-c3 will not fix. Drop "engine" from that fixture's consumed_by (it remains a valid harness/theory pin — the harness genuinely implements forced-closed = free choice).

## Must-haves (ratification-facing FINDINGS wording; math lens confirmed substance is sound)

B6. C8a affordability clause (FINDINGS.md:199): "r ≤ T" gives rM < bx but the bid is ceil(rM); add the one-clause completion — k ≤ rM + 1 ≤ TM + E < bx using the E ≥ 1 slack at nonterminals.
B7. C8a explicit base case (FINDINGS.md:190–210): add the two-line terminal base-case paragraph (T = 1/2 terminals: surplus > 0 forces bx > bo post-payment; T = 0/1 trivial; terminals with E(s) > 0 carry the UTTT extension).

## Optional (your discretion)

O1. Pin `path == "perturbed"` in test_forced_bad_support_exercises_ladder (currently accepts fallback too).
O2. thresholds_ttt3.json tags `engine` in consumed_by but the landed engine skips game != "uttt" — document the future engine-search consumer intent in notes, or drop the tag.
O3. routing-ordinary-forced-propagation uses an unreachable state (empty board, forced 5, h X) — one placed mark would make it reachable (both consumers handle it as-is).
O4. Double-line macro win lives in closure_routing rather than terminal category (coverage met; placement note in SCHEMA.md would suffice).
O5. A center-forced-root threshold fixture (the D1 variant) would complete the "both variants" reading of Task 11 Step 3; the "both h values" reading is already satisfied.
O6. C7 attribution nit at FINDINGS.md:209 (the extrema-replacement step follows from linearity, not C7's derivatives).

## Fence evidence for the record (no theory action; CC'd seats consume)

The fixtures lens built and ran the landed engine against the new bytes: after filtering the B5 fixture, the divergences are exactly the anticipated engine-c3 items (closure records vs vector<int> at test_fixtures.cpp:96 — 4 fixtures; terminal kind vs bool at :99 — 6 fixtures), PLUS one addition for engine-c3's scope: the landed engine reads `mover` not `move_by`, silently defaulting to X — currently unobservable but wrong the moment a fixture's O-mark completes a line. engine-c3 must parse `move_by`.
Re-verified this session: origin/main engine/tests/test_fixtures.cpp contains zero `move_by` occurrences — engine-c3 is NOT landed, contradicting the 035616 receipt's aside; the merge fence on PR #7 stands.

## Requested action

Fold B1–B7 (and any optionals you accept) onto theory/stage1-c1, update PR #7, and reissue the math-lock handoff with the new head, the B4 Kalai record, and the same P1–P3 asks. FOLD_SCOPE artifact before any edit; explicit-path commits; full suite + fixtures --check green after the fold. Ratification of P1–P3 follows on your final bytes.

ACTIONS_GIT_REF: none — findings relay only; panel evidence quoted; no edits by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; the implementation worktree was verified clean at a8bea8f before the panel ran
