ROLE: Pair Implementer
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: engine-c4-merge-2
PARENT_DISPATCH_ID: engine-c4-merge-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E3
HUMAN_GATE_REQUIRED: no — the addressed orchestrator merge grant was executed; receipt reconciliation is next
FROM: engine.implementer
TO: s1.orchestrator-planner
CC: engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c4-merge-2/MERGE-GATE-orchestrator-planner-20260827-223305.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: f229caf42f1bfb8b64dbe9f8a3fd8e1e4b467a74b8a5194c1926d1492e239acf
BUNDLE_ID: engine-c4
OWNER: engine pair
REPO: https://github.com/iwnlcern/utt
BASE: main@d80e63052f5eb3aba7c9b746f682bb120335307b at final merge
BRANCH: engine/search-math-c4@5b147b80ff39587c65dada6851a250dd6c93ba34
TARGET_BRANCH: main
PR: https://github.com/iwnlcern/utt/pull/10
SUBJECT: MERGE CLAIM — PR 10 merged at 13dc562 after exact-tree preflight and green actual-merge CTest

## Verdict

merged-not-deployed

The addressed `engine-c4-merge-2` sequence was executed exactly. GitHub PR #10 is MERGED by merge commit `13dc5629d187e4e90980fbbf2f921276083fd813`; its parents are current pre-merge main `d80e63052f5eb3aba7c9b746f682bb120335307b` and the exact granted PR head `5b147b80ff39587c65dada6851a250dd6c93ba34`. Fresh `origin/main` and the primary local `main` checkout both resolve exactly to the merge commit.

No merge-forward was necessary. No force-push, rebase rewrite, squash, branch deletion, deployment, publication, or release action occurred. The `engine/search-math-c4` worktree and branch remain preserved and clean.

## Grant and execution evidence

1. The exact grant is daemon-admitted, exact-file structurally clean with historical freshness disabled, `PHASE: MERGE-GATE`, authored by `s1.orchestrator-planner`, addressed solely to `engine.implementer`, and carries a bare own-line `DISPATCH MERGE`. Its parent is the engine.planner reconciliation that accepted A1–A11 and independently reproduced the A10 four-baseline 100% sweep. — E1/E3 lineage.
2. Fresh fetch found PR #10 OPEN, non-draft, MERGEABLE/CLEAN, with no configured status checks and exact head `5b147b80ff39587c65dada6851a250dd6c93ba34`. `origin/main` had advanced from the PR base to `d80e63052f5eb3aba7c9b746f682bb120335307b` through governance/spec publication. The changed-on-both-path set from the common base was empty. — E1.
3. The current-main merge preview was conflict-free with tree `38224f45ac38df44e179404ff382cf2d3addd423`. Its delta from current main was exactly the PR's 44 engine files; every governance, relay, design, plan, theory, referee, harness, UI, rules-core, protocol, and fixture path outside that set remained byte-identical to current main. `git diff --check` was clean. — E1/E2.
4. A disposable synthetic two-parent verification commit `3740a2aaabcafcba4a82a7f96c4cb9cebb285cbc` carried that exact prospective tree with parents `d80e63052f5eb3aba7c9b746f682bb120335307b 5b147b80ff39587c65dada6851a250dd6c93ba34`. Fresh Clang 22/C++26 configuration and all 72 Ninja steps succeeded; full CTest passed unit in 31.53 s and e2e in 22.77 s, 2/2 with zero failures in 54.31 s. — E2.
5. The final race check re-fetched the same remote refs, re-confirmed OPEN + MERGEABLE/CLEAN + exact head, and recomputed the identical prospective tree `38224f45ac38df44e179404ff382cf2d3addd423`. GitHub merge-commit mode was invoked with exact-head protection `--match-head-commit 5b147b80ff39587c65dada6851a250dd6c93ba34` and accepted without bypass. The merge message cites DD rev 13 / plan rev 7, the four-lens panel fold, three approving delta design reviews, and both independent A10 sweeps. — E1/E2.
6. GitHub reports PR #10 MERGED at `2026-08-28T05:52:52Z`, merge SHA `13dc5629d187e4e90980fbbf2f921276083fd813`, and unchanged PR head `5b147b80ff39587c65dada6851a250dd6c93ba34`. The merge commit tree is exactly the tested `38224f45ac38df44e179404ff382cf2d3addd423`, its ordered parents are the verified base and head above, and its first-parent delta is exactly the same 44 engine files. — E1/E2.

## Landed scope

The exact first-parent file set is:

- `engine/CMakeLists.txt`
- `engine/src/adapter/main.cpp`, `engine/src/adapter/policy.hpp`
- `engine/src/core/clock.hpp`
- `engine/src/eval/eval.hpp`, `engine/src/eval/weights.hpp`
- `engine/src/root/alt_solver.hpp`, `engine/src/root/bid_matrix.cpp`, `engine/src/root/bid_matrix.hpp`, `engine/src/root/p2_gate.hpp`, `engine/src/root/rmplus.hpp`
- `engine/src/search/backup.hpp`, `engine/src/search/game_model.hpp`, `engine/src/search/search.cpp`, `engine/src/search/search.hpp`, `engine/src/search/tt.cpp`, `engine/src/search/tt.hpp`, `engine/src/search/tvalue.hpp`
- `engine/tests/data/lp_reference_ttt3.json`
- `engine/tests/support/test_rational.hpp`, `engine/tests/support/ttt3_continuous.hpp`, `engine/tests/support/ttt3_discrete.hpp`, `engine/tests/support/ttt3_model.hpp`
- `engine/tests/test_alt_solver.cpp`, `engine/tests/test_backup.cpp`, `engine/tests/test_cutoffs.cpp`, `engine/tests/test_eval_quality.cpp`, `engine/tests/test_game_model.cpp`, `engine/tests/test_p2_gate.cpp`, `engine/tests/test_rational.cpp`, `engine/tests/test_root_matrix.cpp`, `engine/tests/test_search_core.cpp`, `engine/tests/test_tt.cpp`, `engine/tests/test_ttt3_oracles.cpp`, `engine/tests/test_tvalue.cpp`
- `engine/tools/fit_corpus.jsonl`, `engine/tools/fit_eval.py`, `engine/tools/gen_lp_reference.py`, `engine/tools/run_gauntlet.py`
- `engine/tools/gauntlet/allin_tactical.json`, `engine/tools/gauntlet/fraction.json`, `engine/tools/gauntlet/fullclock.json`, `engine/tools/gauntlet/random.json`, `engine/tools/gauntlet/zero.json`

This is exactly the approved c4 product/test/tool surface and exactly the live GitHub PR file list. No out-of-range path landed from the stale PR-side view of main.

## Post-merge verification on actual main

The clean primary checkout was fast-forwarded with `git merge --ff-only origin/main` to actual merge commit `13dc5629d187e4e90980fbbf2f921276083fd813`. Fresh configuration with `/opt/homebrew/opt/llvm/bin/clang++` identified Clang 22.1.8 and C++26; all 72 Ninja build steps succeeded.

- Actual-merge CTest: unit passed in 30.30 s, e2e passed in 22.28 s, 2/2 and zero failures in 52.59 s. — E2.
- `git diff --check d80e630..13dc562` exited 0. — E2.
- `main...origin/main` is `0 0`; local `main`, fresh `origin/main`, and the GitHub merge SHA all equal `13dc5629d187e4e90980fbbf2f921276083fd813`. — E1/E2.
- Primary main and feature worktrees were clean after tests. The disposable preview worktree was removed after confirming it contained only ignored generated build artifacts. — E1.

The two independently reproduced A10 E3 sweeps belong to the approved exact head and were reconciled by the grant lineage. This merge turn did not rerun that tournament after landing; tree identity proves the landed engine bytes are the same tested exact-head bytes. No deployed target was authorized, so the honest verdict remains `merged-not-deployed`.

## Boundary and remaining gates

The complete DD-engine-math-c4 rev-13 / PL-engine-c4 rev-7 engine-c4 bundle is now present on main. Theory-c2 continues independently, and its later landing may upgrade backup-case records through the already-landed `consumed_by` filter; this claim neither grants nor performs theory action. The discretionary performance/idiom polish list remains uncommissioned.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game engine only
- migration/backfill/destructive-write/canonical-data-repair: no
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated chip budgets only
- AI-or-automation-acts-downstream: no — deterministic game engine; referee consumes replies
- worker/scheduler/queue/retry/async-side-effect: no — local tournament subprocesses only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory/referee contracts are consumed read-only; analyze JSONL is engine-owned
- user-visible-control-with-materializer/downstream-consumer: yes — bids, moves, and analysis metadata feed referee, UI, and fitter consumers
- test-runtime-role-mismatch: no — the exact prospective tree and actual merge commit were independently configured, built, and tested
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — exact granted scope landed; deployment and discretionary polish remain explicitly outside the claim
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade, bypass, or residual-risk acceptance was used; the addressed merge grant and fresh exact-tree verification governed the action
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: PR #10 https://github.com/iwnlcern/utt/pull/10; head 5b147b80ff39587c65dada6851a250dd6c93ba34; merge=13dc5629d187e4e90980fbbf2f921276083fd813; parents d80e63052f5eb3aba7c9b746f682bb120335307b 5b147b80ff39587c65dada6851a250dd6c93ba34; tree 38224f45ac38df44e179404ff382cf2d3addd423; origin/main@13dc5629d187e4e90980fbbf2f921276083fd813; prospective-tree CTest 2/2 green in 54.31 s; actual-merge CTest 2/2 green in 52.59 s
FINAL_GIT_STATUS_SHORT: primary main none — clean at actual merge 13dc5629d187e4e90980fbbf2f921276083fd813 with main...origin/main = 0 0; feature worktree none — clean at pushed head 5b147b80ff39587c65dada6851a250dd6c93ba34; relay-root changes after filing are limited to daemon-owned INDEX/relay projections for this merge claim

Next requested action: `s1.orchestrator-planner` and `engine.planner` record the merge receipt and engine-c4 closure. No deployment, release, branch deletion, theory action, discretionary polish, or further engine implementation is authorized by this claim.
