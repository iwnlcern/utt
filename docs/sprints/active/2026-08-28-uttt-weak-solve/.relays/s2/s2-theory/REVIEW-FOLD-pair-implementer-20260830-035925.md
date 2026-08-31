ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c3-fold-report-1
PARENT_DISPATCH_ID: s2-theory-c3-fold-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — fold is ready for the Planner's targeted check; merge remains a separate operator/orchestrator gate
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@282f504
TARGET_BRANCH: main
BRANCH: theory/checker-c3
PR: https://github.com/iwnlcern/utt/pull/13
SUBJECT: CORRECTED REVIEW-FOLD report — theory-c3 MF-1 and directed optionals at PR #13 head 675ef22

## Correction and fold summary

This report supersedes `s2-theory/REVIEW-FOLD-pair-implementer-20260830-035811.md`, whose FOLD_SCOPE rows carried explanatory text after `in` and therefore failed exact-file lint. The prior rendered artifact remains immutable; this successor uses parser-exact scope rows and carries the complete report forward.

Folded the panel's sole must-fix and every directed cheap optional into PR #13. The pathological verdict CLI path now returns exit 1 with one JSON line and code `EV_PARSE`; manifest-directory construction faults use the same mapping, and member-path append faults map to `EV_MEMBER_PATH`. No merge, publication, deployment, or release occurred.

FOLD_SCOPE:
- theory/checker/src/verify.c3 -> in
- theory/checker/README.md -> in
- theory/tests/test_checker_battery.py -> in
- theory/certschema/vectors.py -> in
FOLD_SCOPE_RESULT: all-in

FOLD_SCOPE_NOTES:
- `theory/checker/src/verify.c3`: MF-1 path fault handling and dead-counter cleanup.
- `theory/checker/README.md`: directed contract clarifications.
- `theory/tests/test_checker_battery.py`: MF-1 and long-digest regression coverage.
- `theory/certschema/vectors.py`: emitter-side long-digest probe helper.

The pre-edit scope artifact recorded exactly these four all-in paths before the first repository edit at `/tmp/uttt-s2-theory-c3-fold-1-scope.md`, SHA-256 `07e18a29e98a73d3b3292159bc0e4dbb6716dd125b451b2185311cfe1f90ac45`. Its inline annotations caused the same relay-parser formatting issue but do not alter the recorded path set or disposition. The committed diff touches exactly these four paths.

## Folded findings

1. MF-1: replaced all three unwrapped path operations. `path::tnew(manifest_path)` and `path::tnew(manifest.dirname())` faults return `EV_PARSE`; `directory.tappend(member.path)` faults return `EV_MEMBER_PATH`.
2. MF-1 regression: the battery invokes `verdict /../<absolute-vector-path> --mode subgame` and pins exit 1, `EV_PARSE`, and exactly one stdout line. RED reproduced exit 133, zero stdout bytes, and `path::INVALID_PATH` at the prior head.
3. Member append unit: no native C3 unit was added because the fault is not reachable in the POSIX test environment after the accepted member grammar excludes absolute paths, parent segments, empty segments, and backslashes. The production append catch is present for other path environments.
4. Long digest: emitter helper `build_long_digest_probe()` seals a six-record, 248-byte chunk. The battery proves the checker reaches `E_DUP_STATE`, not `E_CHUNK_DIGEST`, so pytest-only verification exercises the XXH3 >240-byte path.
5. README and battery now pin member-major verification and first-failure precedence; no step-major hoist was made.
6. README pins the verdict member length range 1..4096 to framing-level `EV_PARSE`, documents host-independent backslash rejection, and clarifies the zero-mark `forced=ANY` pseudo-root versus the canonical initial `forced=4` deliverable gate.
7. Removed dead `streamed_totals[3] += rule_count`; implementer-discretion optionals were declined to keep the fold bounded.

## Verification

- PASS — RED/GREEN focused gates: both new tests failed at prior behavior, then `2 passed` after the fold.
- PASS — `cd theory/checker && c3c test`: 19 passed, 0 failed, 0 skipped with C3 0.8.3.
- PASS — `cd theory && uv run pytest tests/test_checker_battery.py -q`: 54 passed in 0.76s.
- PASS — `cd theory && uv run pytest`: 197 passed in 99.72s.
- PASS — `git diff --check`; exact four-file scope; feature worktree clean after commit.
- PASS — local HEAD, `origin/theory/checker-c3`, and PR #13 head equal `675ef22471aab8609629bcabfbdafed41316a886`; PR is OPEN.

Implementation and local verification are complete for this fold at E2. The standing next action is the Planner's targeted check specified in the parent relay. Review/fold success does not grant merge authority.

ACTIONS_GIT_REF: commit 675ef22471aab8609629bcabfbdafed41316a886 on branch theory/checker-c3; origin/theory/checker-c3 and PR #13 head match; one fold commit over reviewed head 3f3a1f0a73771d1bf66b3e4ee6b5c37d1f4e9be6; no merge claimed
FINAL_GIT_STATUS_SHORT: clean in the isolated feature worktree; shared coordination checkout retains only the pre-existing `.claude/scheduled_tasks.lock` modification before relay filing
