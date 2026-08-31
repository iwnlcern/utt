ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c3-fold-report-2
PARENT_DISPATCH_ID: s2-theory-c3-fold-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — fold-2 is ready for the Planner's targeted check; merge remains a separate operator/orchestrator gate
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/REVIEW-FOLD-pair-planner-20260830-175144.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1
DESIGN_SHA256: 56c74ef29768bf7b7e79901184f68703e2636dcb5ea4d68b8dcc6b58520cd7ac
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@950bc63
TARGET_BRANCH: main
BRANCH: theory/checker-c3
PR: https://github.com/iwnlcern/utt/pull/13
SUBJECT: REVIEW-FOLD report — ply-81 erratum boundaries at PR #13 head df9a90a

## Fold summary

Folded the approved erratum into PR #13. The checker now accepts manifest-row plies through 81, the emitter exposes a deterministic 237-byte one-record ply-81 all-drawn NOLOSS probe, and MUT-H14 now carries ply 82 and rejects exactly `E_ROW_PLY_RANGE`. No solver, base design, Appendix A, ruleset, unrelated corpus, merge, publication, deployment, or release change occurred.

FOLD_SCOPE:
- theory/checker/src/wire.c3 -> in
- theory/certschema/vectors.py -> in
- theory/certschema/mutants/MUT-H14.bin -> in
- theory/certschema/catalogue.json -> in
- theory/tests/test_checker_battery.py -> in
FOLD_SCOPE_RESULT: all-in

FOLD_SCOPE_NOTES:
- `theory/checker/src/wire.c3`: changed only the manifest-row range guard from `> 80` to `> 81`; first-failure ordering and reason code are unchanged.
- `theory/certschema/vectors.py`: added `build_ply81_draw_probe()`, moved MUT-H14's row value to 82, and updated its source description.
- `theory/certschema/mutants/MUT-H14.bin`: regenerated deterministically at 237 bytes with manifest ply 82.
- `theory/certschema/catalogue.json`: updated only MUT-H14's reason to `manifest ply above 81`.
- `theory/tests/test_checker_battery.py`: added the positive ply-81 real-checker gate; the existing parametrized mutant battery remains the exact-code negative gate.

The scope artifact was written before the first repository edit at `/tmp/uttt-s2-theory-c3-fold-2-scope.md`, SHA-256 `9bc59c067460f6aed7ba25405f567c2b186ee506f27cfa9c08d2e4ab638e0d1f`. The committed diff touches exactly the five all-in rows above. The optional `theory/tests/test_certschema_mutants.py` row was excluded because direct inspection showed it pins H14's changed-field shape, not its numeric value; `theory/checker/src/verify.c3` remains untouched because its `payload > 80` guard is the move-index bound.

## Folded behavior

1. Positive boundary: `build_ply81_draw_probe()` constructs five full unwon 5-X/4-O local boards plus four color-swapped boards, yielding a canonical well-formed 41-X/40-O state with `forced=ANY`, all local results `full`, terminal result `draw`, NOLOSS_X claim, one TERMINAL record, one manifest row at ply 81, and 237 total bytes.
2. Checker boundary: `parse_header` accepts ply 81 and retains `E_ROW_PLY_RANGE` at the same pre-ordering first-failure position for values above 81.
3. Negative boundary: MUT-H14 now mutates the manifest row to ply 82; its regenerated binary and catalogue wording are consistent, and the checker rejects it exactly as `E_ROW_PLY_RANGE`.
4. Corpus stability: deterministic regeneration reported only MUT-H14.bin as different before its replacement; the committed diff contains no other vector or mutant bytes.

## RED/GREEN evidence

- RED-1: the new focused positive test first failed because `build_ply81_draw_probe` did not exist, establishing the requested helper surface.
- RED-2: after adding only the helper, the same real-checker test rejected the valid probe with `E_ROW_PLY_RANGE` at byte offset 133 under the old `> 80` guard.
- RED-3: after raising the checker cap, the pre-regeneration MUT-H14 binary at ply 81 reached `E_PLY_MISMATCH`, proving that the old negative artifact no longer pinned the amended range boundary.
- GREEN: after the one-value checker change and deterministic H14 regeneration, the paired focused run passed `2 passed` — ply 81 accepted and ply 82 rejected exactly `E_ROW_PLY_RANGE`.

## Verification

- PASS E2 — `cd theory/checker && c3c test`: 19 passed, 0 failed, 0 skipped with C3 0.8.3.
- PASS E2 — `cd theory && uv run pytest tests/test_checker_battery.py -q`: 55 passed in 0.75s.
- PASS E2 — `cd theory && uv run pytest`: 198 passed in 99.38s, including committed-artifact byte comparison against a fresh deterministic generator output.
- PASS E2 — `cd theory && uv run ruff check certschema/vectors.py tests/test_checker_battery.py`: all checks passed.
- PASS E2 — `git diff --check`; exact five-file scope; feature worktree clean after commit and push.
- PASS E2 — local HEAD, `origin/theory/checker-c3`, remote branch ref, and GitHub PR #13 head all equal `df9a90a5cfe39c397896406f9a3032fe465a7375`; PR is OPEN against main.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local certificate schema/checker fold
- migration/backfill/destructive-write/canonical-data-repair: no — deterministic test artifact regeneration only
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifact only
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — shared theory-checker/solver-emitter certificate contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: no — tests execute the real C3 checker against emitter-produced bytes
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — exact approved erratum with paired boundary proof
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — large ceremony retained; no downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or used

Implementation and local verification are complete for fold-2 at E2. The standing next action is the Planner's targeted check and merge-gate recommendation specified in the parent relay. Review-fold success and a green PR branch do not grant merge authority.

ACTIONS_GIT_REF: commit df9a90a5cfe39c397896406f9a3032fe465a7375 on branch theory/checker-c3; origin/theory/checker-c3, the remote branch ref, and PR #13 head match; one fold commit over prior head 675ef22471aab8609629bcabfbdafed41316a886; no merge claimed
FINAL_GIT_STATUS_SHORT: none — clean isolated feature worktree after commit and push
