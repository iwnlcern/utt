ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: theory-c2-impl-report-1
PARENT_DISPATCH_ID: theory-c2-impl-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — the nonempty P1c disagreement table requires operator judgment via s1.orchestrator-planner; merge remains separately gated
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c2-impl-1/IMPL-planner-20260828-004256.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c2-20260827
PLAN_ARTIFACT: PL-theory-c2-20260827
PLAN_SHA256: 713d726191d0a5214047ccb76a93626f6ee431cd78b7958bff9de86cb8ff11fa
BUNDLE_ID: theory-c2
OWNER: theory
REPO: /Users/jack/Programming/utt
BASE: main@dce510ef5a8be18638fd6dc4e5718e67941a7a56
TARGET_BRANCH: main
BRANCH: theory/c2
PR: https://github.com/iwnlcern/utt/pull/11 — OPEN, non-draft, UNMERGED, CLEAN at e63fc3851ef54981fb1470fd6c518713204b1db7
SUBJECT: IMPLEMENTATION COMPLETE — MR17 and P1c exact knife-edge evidence pushed in PR 11; nonempty disagreement table routes to operator judgment

## Summary

Implemented the approved revision-3 plan on the isolated branch `theory/c2` from exact base `dce510ef5a8be18638fd6dc4e5718e67941a7a56`. The branch is pushed at `e63fc3851ef54981fb1470fd6c518713204b1db7`; PR #11 is open against `main`, non-draft, unmerged, and GitHub reports merge state CLEAN. Its three-dot diff contains exactly the eight all-in theory paths enumerated below.

MR17 now flows through the generator: all six backup fixtures carry `consumed_by: ["engine", "theory"]`, and the generated manifest delta is exactly six added `"engine"` lines. The new `knife_edge` module enumerates exact `p=T` points, keeps the small `sweep` helper list-returning, folds canonical exhaustive and targeted reports into deterministic aggregates, and releases each targeted board before the next solve. The CLI rejects incomplete or empty spot requests and writes the combined canonical artifact.

The canonical artifact and EVIDENCE section carry all 34 scales, 1,360 nonzero aggregate groups, the complete 840-row nonzero `agrees=false` partition, and 34 deterministic disagreement-stream digests. Neither coordinates nor samples are committed.

## Commits and TDD evidence

- `609068bb5ade818fdf3c08f3dc990e2beae107fb` — MR17 generator/test/fixture amendment. RED: `tests/test_fixtures.py` reported 1 failed and 5 passed because the fixture still carried `["theory"]`. GREEN: 6 passed; `fixtures --check` exited 0; the only fixture delta was six additions in `backup_cases.json`.
- `1ffc9fc308b1a288d7f07343c0e00dda3fb27d5b` — P1c module, CLI, and planned test suite. RED: collection failed with `ModuleNotFoundError: auction_ttt.knife_edge`. GREEN: 11/11 focused tests, then 25/25 knife-edge plus adjacent crosscheck tests.
- `7c0dfe6ec72c2c0154d75d542fc20c54c6a3b6cd` — canonical artifact and EVIDENCE section. The full command exited 0; artifact validation and the exact EVIDENCE-table reproduction check exited 0.
- `e63fc3851ef54981fb1470fd6c518713204b1db7` — post-implementation review correction. RED: a bare `--spots` flag reached report generation and failed the new guard test. GREEN: the parser requires one or more values; the guard test passed and focused knife-edge/crosscheck verification passed 26/26.

A sequential senior-review pass over `dce510e..e63fc38` found the empty-`--spots` reduction edge above; it was fixed before push. The final pass found zero Critical and zero Important issues. No merge authority is inferred from that review.

## Fresh E2 completion verification

- From `theory/`, final committed-head `uv run pytest -q` exited 0: `98 passed in 93.17s`.
- Final committed-head `uv run python -m auction_ttt fixtures --check` exited 0 with no output.
- `uv run python -m compileall -q auction_ttt tests` exited 0.
- Canonical `uv run python -m auction_ttt knife-edge --max-scale 32 --spots 64 128 --out results/knife-edge-sweep.json` exited 0.
- Artifact SHA-256: `49ad457309536c98e86aebe697ff01a0f1c228f4703530c57cca2fec33498891`; size 392,684 bytes.
- Artifact checks: `spot_target_count == 20332`; 34 per-scale records and digests; per-scale equality, group, and disagreement-group conservation all exact; `disagreement_groups` byte-structure equals the nonzero false slice of `groups`.
- The generated expected Markdown rows and the EVIDENCE section's full disagreement table compare with an empty diff; exactly 840 rows are present.
- `git diff --check dce510e..e63fc38` exited 0; final implementation-worktree status is empty.
- Fresh remote fence: `origin/main@20cb330604fae97694f6e68185aba19ea68e7ff0`; merge base remains dispatched `dce510e`; `git merge-tree --write-tree origin/main HEAD` exited 0; PR #11 head and `origin/theory/c2` equal `e63fc38`.

## Canonical P1c summary

| N | Mode | Target keys | Equality points | Agreements | Disagreements |
|---:|---|---:|---:|---:|---:|
| 1 | exhaustive | — | 4022 | 3926 | 96 |
| 2 | exhaustive | — | 14122 | 11250 | 2872 |
| 3 | exhaustive | — | 24888 | 20486 | 4402 |
| 4 | exhaustive | — | 38764 | 30688 | 8076 |
| 5 | exhaustive | — | 47782 | 38918 | 8864 |
| 6 | exhaustive | — | 63760 | 50432 | 13328 |
| 7 | exhaustive | — | 71306 | 57664 | 13642 |
| 8 | exhaustive | — | 85482 | 67946 | 17536 |
| 9 | exhaustive | — | 95256 | 76482 | 18774 |
| 10 | exhaustive | — | 110090 | 87258 | 22832 |
| 11 | exhaustive | — | 117216 | 94176 | 23040 |
| 12 | exhaustive | — | 133744 | 105884 | 27860 |
| 13 | exhaustive | — | 140806 | 112878 | 27928 |
| 14 | exhaustive | — | 154638 | 123046 | 31592 |
| 15 | exhaustive | — | 165728 | 132218 | 33510 |
| 16 | exhaustive | — | 179838 | 142632 | 37206 |
| 17 | exhaustive | — | 186766 | 149456 | 37310 |
| 18 | exhaustive | — | 202876 | 161010 | 41866 |
| 19 | exhaustive | — | 209804 | 167910 | 41894 |
| 20 | exhaustive | — | 225308 | 178936 | 46372 |
| 21 | exhaustive | — | 235864 | 187838 | 48026 |
| 22 | exhaustive | — | 249226 | 197880 | 51346 |
| 23 | exhaustive | — | 256054 | 204642 | 51412 |
| 24 | exhaustive | — | 273052 | 216598 | 56454 |
| 25 | exhaustive | — | 281210 | 224114 | 57096 |
| 26 | exhaustive | — | 294594 | 234066 | 60528 |
| 27 | exhaustive | — | 304336 | 242568 | 61768 |
| 28 | exhaustive | — | 318710 | 253030 | 65680 |
| 29 | exhaustive | — | 325682 | 259858 | 65824 |
| 30 | exhaustive | — | 343484 | 272270 | 71214 |
| 31 | exhaustive | — | 350238 | 279112 | 71126 |
| 32 | exhaustive | — | 364462 | 289448 | 75014 |
| 64 | targeted | 20332 | 722676 | 574984 | 147692 |
| 128 | targeted | 20332 | 1450932 | 1154716 | 296216 |

Exhaustive N<=32 totals are 5,869,108 exact equality points and 1,194,488 disagreements (20.3521%). N=1 is a small-population outlier at 96/4,022; from N=2 onward the rate oscillates around 20%. Targeted N64 is 147,692/722,676 (20.4368%); targeted N128 is 296,216/1,450,932 (20.4156%), a slight decrease of about 0.0213 percentage points. The observed solved-scale trend is stable near 20.4%, not decaying toward an empty disagreement set.

The complete nonzero aggregate disagreement table is carried in `theory/results/knife-edge-sweep.json` as `disagreement_groups` and reproduced without omission in `theory/results/EVIDENCE.md` under “Complete nonzero disagreement partition.” It contains every 840 nonzero row keyed by scale, mode, tie owner, state class, discrete result, `agrees=false`, and count. Draw and opponent-win disagreements remain separate.

**Operator-judgment flag:** ANY nonempty disagreement table is flagged as an operator-judgment item in the completion report, routed via s1.orchestrator-planner; never absorbed into the convention.

Solved scales remain P1a exact-discrete-oracle territory. The disagreement table measures the fidelity boundary of the separately ratified P1b tie-owner-wins convention at exactly representable equality; this implementation does not re-ratify, replace, or silently amend that convention.

## Boundary contract and scope

Writes: generated backup `consumed_by` tags; deterministic P1c aggregate JSON; EVIDENCE methodology, counts, full partition, digests, and interpretation.
Reads: landed continuous nodes, exact discrete values/reachability masks, N=32 `select_spot_targets`, and board-scoped `solve_targeted`.
Target entity: theory-owned fixture contract and committed P1c evidence bundle.
Downstream consumer: engine fixture loader for MR17; theory Planner/orchestrator/operator for P1c interpretation.
Contract: six backup cases carry exactly `["engine", "theory"]`; P1c JSON carries methodology, per-scale records, full groups, exact false-slice disagreement groups, and deterministic stream digests.
Proof: E2 generator reproducibility, RED-GREEN tests, canonical CLI run, exact JSON conservation, full-table reproduction, and open PR snapshot.
No-consumer action: defer engine consumer implementation to its owning lane; route P1c product semantics to the operator via s1.orchestrator-planner.

SCOPE_DIFF:
- theory/auction_ttt/fixtures_gen.py -> in
- theory/tests/test_fixtures.py -> in
- theory/fixtures/backup_cases.json -> in
- theory/auction_ttt/knife_edge.py -> in
- theory/auction_ttt/__main__.py -> in
- theory/tests/test_knife_edge.py -> in
- theory/results/knife-edge-sweep.json -> in
- theory/results/EVIDENCE.md -> in
SCOPE_DIFF_RESULT: all-in

Out-of-scope preserved: no engine, harness, UI, solver-oracle, design, plan, dependency, LFS, external-storage, compression, shard, deployment, release, or merge bytes/actions. The linked worktree is preserved for PR feedback.

## Escalation and next gate

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local deterministic theory package; no credential or permission surface
- migration/backfill/destructive-write/canonical-data-repair: no — generated fixture regeneration is deterministic and reviewable
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets only
- AI-or-automation-acts-downstream: no — local evidence generation does not act on external systems
- worker/scheduler/queue/retry/async-side-effect: no — synchronous local batch computation only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — MR17 changes the engine-consumed fixture contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-facing control changed
- test-runtime-role-mismatch: no — tests and canonical CLI exercise the real solver paths
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — nonempty P1c disagreements require operator judgment and are not absorbed locally
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Next requested action: `theory.planner` reconcile PR #11 and this implementation report, then route the explicit nonempty-disagreement operator-judgment item through `s1.orchestrator-planner`. Preserve the branch/worktree for review. Merge remains a separate addressed gate and is not authorized here.

ACTIONS_GIT_REF: theory/c2@e63fc3851ef54981fb1470fd6c518713204b1db7; commits 609068bb5ade818fdf3c08f3dc990e2beae107fb, 1ffc9fc308b1a288d7f07343c0e00dda3fb27d5b, 7c0dfe6ec72c2c0154d75d542fc20c54c6a3b6cd, e63fc3851ef54981fb1470fd6c518713204b1db7; origin/theory/c2@e63fc3851ef54981fb1470fd6c518713204b1db7; PR #11 head e63fc3851ef54981fb1470fd6c518713204b1db7; exact eight-path diff above; no merge, deployment, release, or external artifact action claimed
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree

