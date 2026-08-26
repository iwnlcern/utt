ROLE: Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: harness-c1-fold-1
PARENT_DISPATCH_ID: harness-c1-impl-report-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold loop within the dispatched bundle; merge remains separately gated; PR-remote question rides to the orchestrator below
FROM: harness.planner
TO: harness.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c1-impl-report-1/IMPL-pair-implementer-20260825-154918.md
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c1-20260825
PLAN_ARTIFACT: PL-harness-c1-20260825
PLAN_SHA256: 15c722467ba91d7033ca3059d1cc036f6b3c0a14a974f3abf21cbd5d08e8fe05
BUNDLE_ID: harness-c1
OWNER: harness
BRANCH: harness-c1-impl (at 477f491)
SUBJECT: REVIEW-FOLD — panel verdicts on harness-c1-impl@477f491: 2 blockers, 5 must-fix; fold and report

## Reconciliation of your implementation report

Verified independently before paneling: scope fences hold (zero files outside referee/ + docs/protocol/ in the 64-file diff — E1, git diff census); suite re-run in your worktree — 178 passed, 1 skipped (the sanctioned Task 12 marker) at 477f491 — E2. Your report's claims stand except where the panel found gaps below.

## Panel record

PANEL_CHOSEN: team-of-4
DEFAULT_ROLES_CHANGED: no
WHY_THIS_PANEL: non-trivial ~6.4k-line Python PR; risk shape = subprocess concurrency, hostile-input framing, cross-pair schema contracts
ROLES:
- defensive-security — approve (3 optional)
- performance-concurrency — approve (4 optional + 3 cheap fold-ins)
- test-coverage — BLOCK (1 blocker, 3 must-fix, 4 optional; mutation-verified)
- design-conformance — BLOCK (1 blocker, 2 must-fix, 3 optional, 1 question; executed-repro-verified)

## Blockers (merge-preventing; fold required)

B1 (design-conformance; planner-verified at E1 this turn): `referee/poorman_referee/referee.py:105-116` decides the all-closed terminal by counting WON LOCAL BOARDS per seat — exactly the macro-mark-count tiebreak spec canonical rule 8 forbids. "Chips remaining" is leftover BUDGET (rule 8; theory terminal `p ≷ 1/2`; DD §3.5 `chip_count`/`exact_tie_draw`). Committed fixture evidence: both parity fixtures record equal budgets (1000000000 each) yet score 1–0 `chip_count` — must be ½–½. Required: compare post-payment `budgets["X"]` vs `budgets["O"]` (strict greater wins `chip_count`; equal → `draw`/`exact_tie_draw`); pass budgets into `_terminal_from_position`; add the missing named E2E pair (all-closed unequal budgets → budget leader wins; all-closed equal budgets → draw — this gap is why the bug survived a green suite); regenerate every affected committed artifact; update dependent test expectations.

B2 (test-coverage; mutation-proven): the plan's deterministic schedule (a) "sweep-visible extra line" is a false green — `stub_engine.py:141` implements `extra_line_before_sweep` as a same-write emission, so `read_reply`'s buffered-second-line path classifies it and `collect_both`'s `sweep_extra()` downgrade (procs.py:322-326) has ZERO effective coverage: deleting the sweep loop leaves the whole suite green. Required: make the stub emit the reply first and the extra line in a genuinely separate, coordinated write (e.g. marker-file handshake with the companion seat's held-open reply) so the line provably arrives after `read_reply` returned and before the sweep; then re-run the sweep-deletion mutation and confirm the test fails without the sweep.

## Must-fix (fold before merge)

M1 (coverage): `test_procs.py:199-210` cannot falsify `shutdown_grace_ms` — hardcoding 2000 in `Engine.__init__` leaves the suite green (mutation-proven) because the stub exits at 100 ms. Use an engine that ignores game_end and outlives the grace; assert `finish` returns ≈0.3 s and well under the 2 s default.
M2 (coverage): plan Task 7's destructive check "removing an attempt record raises" does not exist and cannot pass — `replay_frames` (gamelog.py:156-159) accepts an attempts list starting at attempt 2 (probe-proven). Add attempt-number contiguity validation and the destructive test.
M3 (coverage): the plan-named 0-0 zugzwang owner-wins ALTERNATION is never asserted E2E (ties test uses bids of 3; 0-0 games assert only the first auction). Parametrize the ties test over bids {3, 0} or extend a parity game across the first three 0-0 plies.
M4 (conformance): committed transcript-v1.jsonl and all conformance fixtures except terminal-fault-won-no-recovery.jsonl predate 477f491's `game_end.stderr` field and no longer match the current emitter. Regenerate ALL committed artifacts from the current referee — fold into the B1 regeneration pass, which also covers conformance optionals 5-6 below.
M5 (conformance): DD §4's "info lacking/misusing `quality` logs a warning, nothing more" is unimplemented (no code path inspects `quality`). Add the non-fault warning (referee stderr or an additive logged key; do NOT add a new event type — `replay_frames` rejects unknown kinds).

## Planner ruling on the conformance question (F7)

Protocol-major mismatch disposition: the implemented reading — mismatch classifies as `schema_violation` → `hello_fault` forfeit — is ACCEPTED as the v1 meaning of DD §3.2's "aborts before ply 0". Rationale: the locked `game_end.reason` enum has no separate abort value, mixed-major pairings are an operator configuration error, and forfeit is deterministic. Fold as documentation only: one sentence in docs/protocol stating referee and engine must share protocol major 1 and a mismatch forfeits at hello. No DD amendment; orchestrator on CC for visibility.

## Optional (your discretion; the panel flagged these as cheap)

- security: kill-before-reap reorder in `finish()`'s clean-exit/dead paths (two-line consistency fix); document argv-in-log expectation; optional no-clobber guard on per-game logs.
- performance: `os.write` loop for stdin short-write safety; stdin write deadline; hello clock measured after send; close stderr fd before thread join; drop/comment the redundant `results.sort`; concurrent hellos; `.gitignore` note for ad-hoc stderr files.
- coverage: causal-order index assertions (recovery vs auction event, both directions); advancing fake clock instead of constant 0.0; assert the fault-winner's move was the applied move in the ten-fault test + validate_log at lines 282/298; use or drop the dead `hello_timeout` stub option; assert `time_control.shutdown_grace_ms == 300` on a logged game_start.
- conformance: info-type validation-order note or reorder (fault-class label only); align re-hello `budget` semantics with the doc (current-budget is the better reading — amend the doc line); omit-or-document always-present `resolution.coin`.

## Verification after fold

Rerun the full E2 suite; re-run the B2 sweep-deletion and M1/M2 mutations to confirm the tests now fail without the behavior; regenerate artifacts and re-run the E3 tournament smoke (terminal results of all-closed games change W/D/L stats — record the fresh smoke in your fold report). File the fold report per protocol: FOLD_SCOPE listing every file before editing, `FOLD_SCOPE_RESULT: all-in` (findings above name the scope; anything outside referee/** + docs/protocol/** is a deviation relay, not an edit), report via daemon admission with PARENT_DISPATCH_ID: harness-c1-fold-1.

## For the orchestrator (CC; no action from the implementer)

- PR creation remains blocked on the repo having no git remote — operator decision whether to configure one or gate merge on the local branch + report evidence.
- Theory-fixture integration remains honestly pending/blocked on theory's fixtures landing; acceptance language in the implementation report is correct.

ACTIONS_GIT_REF: none — findings relay; panel evidence in reviewer outputs summarized above; B1 planner-verified by direct read of referee.py:105-116 and fixture line 66 this turn; no edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; no edits claimed; branch state of record is harness-c1-impl@477f491
