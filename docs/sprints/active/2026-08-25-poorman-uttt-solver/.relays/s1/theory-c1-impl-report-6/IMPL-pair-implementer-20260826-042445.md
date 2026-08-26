ROLE: Pair Implementer
PHASE: IMPL
AUTHORITY: implementation
DISPATCH_ID: theory-c1-impl-report-6
PARENT_DISPATCH_ID: theory-c1-impl-6
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — P1, P2, and P3 require planner/operator ratification; merge remains separately gated and is not requested from this seat
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-impl-6/IMPL-pair-planner-20260826-023144.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
PLAN_LOCK_ID: PL-theory-c1-20260825
PLAN_ARTIFACT: PL-theory-c1-20260825
PLAN_SHA256: a07441e43dcb21e58ca95103800783dd47321d490e0d44d195a2d72973b355fd
BUNDLE_ID: theory-c1
OWNER: theory
REPO: /Users/jack/Programming/utt
BASE: main@d89f006
TARGET_BRANCH: main
BRANCH: theory/stage1-c1
PR: https://github.com/iwnlcern/utt/pull/7 — OPEN, non-draft, UNMERGED at a8bea8f2525dedcf4735ef07df0191880f4b8973
SUBJECT: IMPLEMENTATION COMPLETE / MATH-LOCK HANDOFF — Stage-1 exact solver, fixture publication, findings, and E2 bundle are pushed in PR #7; P1-P3 await ratification; engine-c3 keeps merge fence closed

## Implementation and publication result

Tasks 11–13 of PL-theory-c1-20260825 revision 5 are complete on `theory/stage1-c1`. The branch is pushed to `origin` at `a8bea8f2525dedcf4735ef07df0191880f4b8973`, and PR #7 is open against `main` without merge. Its three-dot change set is restricted to the authorized `theory/` package, tests, fixtures, findings, and result bundle.

The completed package supplies the exact continuous and masked discrete Poorman auction-TTT oracles, C9a bid-only validation, the C11 census, an exact-certified LP reference with deterministic fallback seam, canonical schema-v1 fixtures, generator check mode, FINDINGS C1–C11, and 34 committed BandReports for exhaustive N=1..32 plus deterministic targeted N=64 and N=128.

Canonical evidence is:

- `theory/FINDINGS.md`
- `theory/results/EVIDENCE.md`
- `theory/results/crosscheck-N1.json` through `crosscheck-N32.json`
- `theory/results/crosscheck-N64.json` and `crosscheck-N128.json`
- `theory/fixtures/schema-v1.json`, `SCHEMA.md`, and the seven fixture manifests

## E2 completion evidence

- Exact committed-tree suite: from `theory/`, `uv run pytest -q` exited 0 with `82 passed in 65.60s`.
- Fixture reproducibility: `uv run python -m auction_ttt fixtures --check` exited 0 with no diff.
- Canonical sweeps: `uv run python -m auction_ttt sweep --max-scale 32` and `uv run python -m auction_ttt sweep --spots 64 128` exited 0 and wrote the committed reports.
- Report integrity: all expected scales N=1..32,64,128 are present and every report has zero out-of-band mismatches; no `NonDeterminacy` occurred.
- LP emitted cases carry exact `Fraction` certificates; the forced-failure seam executed the independent exact-rational fallback successfully; no `LPEscalation` occurred.
- `git diff --check origin/main...HEAD` exited 0. The GitHub PR snapshot was OPEN, non-draft, base `main`, head `a8bea8f2525dedcf4735ef07df0191880f4b8973`, with merge-state `CLEAN`; that status is evidence only and grants no merge authority.

## Math-lock findings and ratification asks

### C8a — discharged

The rounding-surplus induction is closed: replacing each continuous critical bid by its integer ceiling loses strictly less than one unit of threshold surplus per applied mark, while every applied mark decreases `E(s)` by one. Therefore exact distance `d>E(s)` is a sufficient outside-band classification margin on any finite mark-accumulating game DAG, including UTTT. It deliberately does not classify in-band states or equality.

### C11 extra-own-mark lemma — undischarged

The attempted monotonicity induction does not close. Adding an own mark removes a legal action that can be the uniquely extremizing move in the unaugmented board, and an actually played mark also changes the tie owner while the static comparison wants `h` fixed. C11 therefore remains a computational census result: 22,186 nonterminal `(board,h)` nodes, zero reachable `a>b` nodes, with `a=b` attained. The residual UTTT no-reachable-witness gap remains explicit.

### P1 — ratification requested

Accept the recorded equality boundaries: referee arithmetic classifies realized sequences; the exact discrete oracle classifies only the solved populations; there is no complete 10^9-unit in-band optimal classifier and no continuous winner is assigned at `p=T`. Forbid presenting a continuous equality convention as exact discrete optimal play.

### P2 — ratification requested

Accept `d>E(s)` as the engine forced-classification gate. Inside the band, require interval/estimate quality metadata and the root bid-matrix path rather than a forced-winner label; retain the explicit absence of a canonical in-band exact solver.

### P3 — ratification requested

Knowingly accept the C3 proof plus exact synthetic harness as the Stage-1 validation basis for the zugzwang branch, including the residual absence of a reachable UTTT witness. This does not convert synthetic backup fixtures into canonical game states.

## Fixture consumer state and merge sequencing

No addressed MR17 engine-tag authorization landed before generation. `theory/fixtures/backup_cases.json` therefore remains normative `consumed_by: ["theory"]`; no engine consumer claim is made for it.

The publication-first ruling remains binding. As of this handoff:

- harness-c3 is LANDED: PR #6 merged at `ecccb55a229198bcaa912761597a2eeb5cee2ddf` into `origin/main` via merge commit `f1c146b`; `referee/tests/test_fixture_loader.py` honors `consumed_by`.
- engine-c3 is NOT LANDED: the latest engine report `engine-c2/SITREP-planner-20260826-040211.md` verifies that `origin/main:engine/tests/test_fixtures.cpp` lacks `move_by`, and no engine-c3 implementation branch or PR exists.

Therefore PR #7 MUST remain unmerged until engine-c3 lands (or is included in the same merge window) and the later theory merge grant verifies both consumer repairs. This report requests math-lock ratification only. It does not request or exercise merge, deployment, release, or publication beyond the already authorized branch push and unmerged PR.

SCOPE_DIFF:
- theory/pyproject.toml -> in
- theory/.gitignore -> in
- theory/uv.lock -> in
- theory/auction_ttt/ -> in
- theory/tests/ -> in
- theory/fixtures/ -> in
- theory/results/ -> in
- theory/FINDINGS.md -> in
SCOPE_DIFF_RESULT: all-in

ACTIONS_GIT_REF: origin/theory/stage1-c1@a8bea8f2525dedcf4735ef07df0191880f4b8973; pushed without force; PR #7 https://github.com/iwnlcern/utt/pull/7 OPEN and UNMERGED; implementation commits c91dbc9..a8bea8f; no merge, deployment, or release action
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree at a8bea8f2525dedcf4735ef07df0191880f4b8973; relay-root main worktree contains concurrent foreign UI/INDEX bytes which this seat did not alter except through daemon admission of this owned relay
