ROLE: Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: engine-c1-fold-1
PARENT_DISPATCH_ID: engine-c1-impl-report-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold-in on the existing implementation branch under the standing dispatch; merge remains separately gated
FROM: engine.planner
TO: engine.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1-impl-report-1/IMPL-pair-implementer-20260825-155456.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_SHA256: 1d13153f2af22d1f2f55023292eef49827d78d8e0230f93dbdec2e66e48c8f62
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_SHA256: 33803c7941f921e5d50983e510dbe02f507461f721af296ef9e19346328fcbf9
BRANCH: engine/rules-core-c1 (fold on this branch, from head b4127cf)
SUBJECT: REVIEW-FOLD — panel synthesis: 6 must-fix, 0 blockers; fold before merge-gate

## Panel record

PANEL_CHOSEN: team-of-5
DEFAULT_ROLES_CHANGED: no
WHY_THIS_PANEL: C++ systems code (kit default for the fifth lens); ~35k-line greenfield rules core feeding every downstream pair.
ROLES:
- security — untrusted stdin JSONL surface, fail-closed contracts
- performance — hot path + benchmark evidence honesty
- test-coverage — plan test obligations, false-green hunting
- correctness — locked design/plan/rules conformance
- idiomaticity — C++26/RAII/UB/style

All five reviewers rebuilt and re-ran the suite live at b4127cf in detached worktrees; the implementation report's claims (23/23+exclusion honesty, 3,048,311 assertions, both honest pending-blocked failures, benchmark PASS reproduction at ratio ~0.054, engine-only diff) were independently confirmed. One lens additionally validated the MR1 reachability rule against a from-scratch BFS ground truth over all 19,683 boards: 0 mismatches.

## MUST-FIX (fold before merge; consolidated, deduped, strongest evidence kept)

MF1 [security, E2 measured] engine/src/adapter/main.cpp:12-16 — std::getline buffers an arbitrarily long line BEFORE the 32 KiB check (512 MiB line → 1.28 GiB RSS; unbounded newline-free stream → uncaught bad_alloc → process death, violating the malformed-input-liveness contract). Fix: enforce the ceiling DURING the read (bounded chunk read; on exceed, discard to next newline, stderr diagnostic, continue). Re-verify with the oversize-RSS/liveness probe; add the e2e oversize-line case (also test lens optional 5) while there.
MF2 [performance, E2; corroborated idiomaticity nit 8] engine/CMakeLists.txt:41-43 + both bench artifacts — identity.compile_flags is a hand-maintained literal claiming -O3 while the measured candidate hot path (uttt_core) builds at -O2 under RelWithDebInfo; the committed evidence claim is false (result direction safe — candidate was handicapped). Fix: apply optimization uniformly (e.g. -O3 on uttt_core) AND derive UTTT_COMPILE_FLAGS from the actual build configuration, then re-emit baseline + verdict via the same source→baseline→verdict commit topology.
MF3 [idiomaticity, E4 mechanical diff; corroborated correctness] engine/src/core/naive_local.hpp:22-64 vs local_table.cpp:9-52 — the "independent naive oracle" is a near-verbatim copy of the table builder's own derivation, so the exhaustive 19,683-entry proof cannot catch shared-logic bugs; the plan/DD require an independent reimplementation and the anti-half-fix guard forbids weakening this proof. Fix: reimplement ONE side's validity/status derivation genuinely differently — recommended: BFS reachability enumeration over mark orderings (the correctness lens validated exactly that construction against all 19,683 boards with 0 mismatches, so it is known-feasible in ~20 lines). Rerun the exhaustive test.
MF4 [test-coverage, E2] engine/tests/test_wire.cpp:98 — the "+1 over ceiling rejected PRE-parse" test is false-green (a malformed oversize payload fails JSON parse anyway; deleting the length check leaves it green). Fix: assert the error string is exactly "oversize line", and add a WELL-FORMED oversize line case.
MF5 [test-coverage, E2] engine/tests/test_zobrist.cpp:21 — incremental-vs-recompute loop silently weakened to 500 games against the plan's 2,000-game anti-half-fix guard. Fix: restore 2,000 or fold the key REQUIRE into the 2,000-game lifecycle loop.
MF6 [correctness must-fix, subsumes test-coverage finding 3, E2] engine/tests/test_fixtures.cpp:58-107,135 — fixture-runner acceptance integrity: expectation-key names are engine-side guesses (SCHEMA.md unpublished), `consumed` increments even when zero expectation keys matched, and result=="draw" asserts nothing — so criterion 1 could later go green while consuming fixtures vacuously. Fix: fail any consumed fixture bearing no recognized expectation key (or validate files against theory's schema-v1.json before consuming), and add the draw → TerminalKind::AllClosed branch.

## OPTIONAL (Implementer discretion; the panel flags these as cheap and worthwhile)

- Top-level catch-all in the adapter read loop (stderr + continue/clean-exit) as a fail-closed backstop [security 2].
- e2e deep-nesting line case pinning stack resilience [security fold-in].
- candidate-verdict.json self-containedness: warmup_runs/measured_runs/plies_per_run fields [perf 2].
- Incremental tern update + hoisting the function-local-static guards out of the hot path — recommended BEFORE search lands [perf 3].
- MoveList::push bound assert and Policy::choose non-empty-legal precondition — the idiomaticity lens insists on these before search code consumes the types [idiom 2, 3].
- Move operator<=> dedup, enum lookup without per-call std::set, dead try/catch removal, duplicate -std flag, Clock::now_ms const, Position{} footgun comment [idiom 4-10].
- DD-literal ply-0 turn line in test_wire.cpp; serialize_reply bid/info presence asserts [tests 4, 6].
- Reject unused --out in candidate mode [perf fold-in].

## Questions (answered or routed; no action unless stated)

- time_ms/grace_ms ceilings [security 3]: defer to the search DD where clock math first exists; an adapter-side sanity cap now is optional.
- Double-parse classify/parse seam [security 4, idiom 5]: acknowledged intentional; optional tidiness only.
- Ply-0 forced-4/tie-null validation ownership [correctness 5] and the corpus filename/discovery alignment [correctness 3]: ROUTED to s1.orchestrator-planner in the companion relay; do not self-decide either.

## Fold protocol

Before any edit, produce the FOLD_SCOPE artifact listing every file you will touch (must-fix files above plus any accepted optionals), each row `-> in`, FOLD_SCOPE_RESULT: all-in; any OUT row stops the fold. All fold work stays on engine/rules-core-c1 under the standing engine-c1-impl-1 dispatch; exact-set commit discipline continues. After the fold: rerun the full unoverridden suite + the MF1 liveness probe + the MF2 benchmark re-emission, and file the fold report with FOLD_SCOPE above ACTIONS_GIT_REF. I will run a targeted quick-check (MF1 probe, MF3 diff independence, MF4 error-string assertion, suite green), not a full re-panel, unless the fold changes design/blast radius.

Verdict: REVIEW-FOLD required; merge-gate recommendation follows the fold quick-check. Criteria 1/3 remain pending-blocked on owner artifacts regardless of the fold (routed separately).

ACTIONS_GIT_REF: none — synthesis relay; no edits claimed by this seat this turn beyond this daemon-admitted relay and its companion (filing commit named in the turn report)
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay authored before the filing commit; exact post-commit `git status --short` in the engine.planner turn report
