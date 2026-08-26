ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: theory-c1-fold-report-4
PARENT_DISPATCH_ID: theory-c1-fold-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the operator ratification is folded and the math lock is closed; merge remains separately fenced on engine-c3
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-fold-3/REVIEW-FOLD-pair-planner-20260826-135747.md
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
BASE: origin/main@48fadffc9709af92a22c0dc3f5cd770489a9e20d
TARGET_BRANCH: main
BRANCH: theory/stage1-c1
PR: https://github.com/iwnlcern/utt/pull/7 — OPEN, non-draft, CLEAN, and UNMERGED at d421e14533bd4cc572578d6e483b2291b90a6694
SUBJECT: REVIEW-FOLD COMPLETE / MATH LOCK CLOSED — P1 amended-ratified and P2/P3 ratified at d421e14; P1c deferred non-blockingly; engine-c3 merge fence remains closed

## Fold result

The addressed ratification was folded onto the existing PR branch in commit `d421e14533bd4cc572578d6e483b2291b90a6694` (`theory: record math-lock ratification`) and pushed without force. The explicit-path commit contains only `theory/FINDINGS.md`. PR #7 points at that exact head, and its description records the closed math lock and retained merge fence. No new PR was opened and no merge was attempted.

The old `proposed-for-ratification` state is absent from all three P sections. Because the unchanged reference test validates epistemic claim categories rather than governance decisions, each section now records both axes explicitly: `Ratification status` carries the operator decision, while `Status: defined` keeps the policy claim in the existing findings taxonomy.

## Ratified findings

### P1 — ratified as amended

- P1a: on the solved populations—exhaustive N<=32 and the targeted N=64/128 spots—the exact discrete oracle remains authoritative at exact equality. No convention overrides computed truth.
- P1b: at larger scales, including canonical 10^9-unit play, an exactly representable `p=T` is treated as won by tie owner `h`. FINDINGS labels this as an operator-ratified convention, not a theorem or established optimal-play fact, and requires every engine-analysis or documentation presentation to preserve that label.
- P1c: a later dedicated sweep will enumerate exactly representable `p=T` masked points at solved scales, grouped by tie owner and state class, and report agreements and disagreements. Material disagreements route back to the operator. This is new work outside PL-theory-c1, a theory-c2 candidate after PR #7 lands, and is non-blocking for the math lock, merge, and engine consumption of the lock.

### P2 — ratified as written

The exact `d>E(s)` gate remains the engine's theorem-backed forced-classification boundary. In-band play uses interval or estimate metadata plus the root bid-matrix path and is not labeled as a forced winner. The absence of a canonical in-band exact solver remains explicit.

### P3 — ratified as written

The C3 one-step proof plus the exact synthetic harness is the accepted Stage-1 validation basis for the zugzwang branch. The residual absence of a reachable UTTT witness is knowingly retained; the undischarged C11 lemma remains census-only, and synthetic backup fixtures do not become canonical game states.

Math lock: CLOSED on these folded bytes and the operator ratification in the parent dispatch.

## Verification and carrier evidence

- Pre-edit worktree: clean branch `theory/stage1-c1` at the dispatched `34da9c8aa0b831726592d29ec844598d8f8f589a`; PR #7 pointed at the same head.
- Pre-edit FOLD_SCOPE contained only `theory/FINDINGS.md`; the resulting commit contains exactly that path.
- Targeted findings-reference test: `cd theory && uv run pytest -q tests/test_findings_refs.py` exited 0 with `1 passed in 0.01s`.
- Fresh exact committed-head full suite: `cd theory && uv run pytest -q` exited 0 with `85 passed in 87.03s`.
- `git diff --check`, cached-diff check, and exact path assertions exited 0 before commit.
- Remote branch was refreshed immediately before push and still pointed at dispatched parent `34da9c8`; the normal push advanced it to `d421e14` without force.
- Post-push PR snapshot: OPEN, non-draft, base `main`, head `d421e14533bd4cc572578d6e483b2291b90a6694`, merge-state `CLEAN`. This is evidence only and grants no merge authority.

## Fixture consumers and live merge fence

No addressed MR17 engine-tag authorization has landed. `theory/fixtures/backup_cases.json` remains normative `consumed_by: ["theory"]` and was not changed by this fold.

The consumer fence was refreshed after the push. Engine-c3 remains NOT LANDED on `origin/main@48fadffc9709af92a22c0dc3f5cd770489a9e20d`: `engine/tests/test_fixtures.cpp` still lacks `move_by`, still reads `mover`, still parses closure records as `vector<int>`, and still reduces `expected_terminal` to a boolean. The complete GitHub PR census contains no engine-c3 implementation PR.

Therefore PR #7 MUST remain unmerged until engine-c3 lands or is included in the same merge window and a later theory merge grant verifies the consumer fence. This handoff reports the authorized findings fold only; it does not request or exercise merge, deployment, or release authority.

FOLD_SCOPE:
- theory/FINDINGS.md -> in
FOLD_SCOPE_RESULT: all-in

ACTIONS_GIT_REF: origin/theory/stage1-c1@d421e14533bd4cc572578d6e483b2291b90a6694; commit `d421e14533bd4cc572578d6e483b2291b90a6694`; pushed without force; PR #7 https://github.com/iwnlcern/utt/pull/7 updated, OPEN, and UNMERGED; no merge, deployment, or release action
FINAL_GIT_STATUS_SHORT: none — clean implementation worktree at d421e14533bd4cc572578d6e483b2291b90a6694; relay-root main worktree contains concurrent foreign INDEX and dispatch bytes which this seat did not alter except through daemon admission of this owned successor
