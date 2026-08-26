ROLE: Pair Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: theory-c1-fold-3
PARENT_DISPATCH_ID: theory-c1-fold-report-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the operator ratification was obtained this session and is recorded verbatim below; merge remains fenced on engine-c3
FROM: theory.planner
TO: theory.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-fold-report-2/REVIEW-FOLD-pair-implementer-20260826-051307.md
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
BRANCH: theory/stage1-c1 (fold onto 34da9c8; update PR #7)
SUBJECT: MATH LOCK RATIFIED — P1 as amended (large-scale tie-owner convention + side sweep), P2 and P3 as proposed; one small FINDINGS status fold requested

## Operator ratification (obtained this session, post-review, recorded verbatim in effect)

- P2: RATIFIED as proposed. The `d > E(s)` forced-classification gate is theorem-backed (C8a discharged and panel-validated); in-band play uses interval/estimate metadata plus the bid-matrix path, never a forced-winner label.
- P3: RATIFIED as proposed. The C3 one-step proof plus the exact synthetic harness is the accepted Stage-1 validation basis for the zugzwang branch; the operator knowingly accepts the absence of a reachable UTTT witness (C11 lemma remains undischarged, census-only).
- P1: RATIFIED AS AMENDED by the operator ("convention but only for large scales; we can run a dedicated sweep on the side too"):
  P1a. At scales the exact discrete oracle has actually solved (exhaustive N ≤ 32 plus the targeted spots), the oracle remains AUTHORITATIVE for exact-equality classification; no convention overrides computed truth.
  P1b. At larger scales — including canonical 10^9-unit play — the engine adopts the CONVENTION that at exactly representable p = T the position is treated as won by the tie owner h. This is an operator-ratified convention, not a theorem; every presentation (engine analysis output, docs) must label it as such and must never present it as established optimal play.
  P1c. Side obligation (non-blocking): a dedicated sweep enumerating exactly-representable p = T masked points at the solved scales, reporting outcomes by tie owner and state class — empirical characterization that calibrates the convention (agreements and disagreements documented; disagreements bound the convention's fidelity, and material ones route back to the operator).

## Fold requested (small, PR #7 branch)

Update theory/FINDINGS.md's P1–P3 sections from `proposed-for-ratification` to ratified status: P2/P3 `ratified` as written; P1 `ratified-as-amended` carrying P1a/P1b/P1c verbatim in substance, with the convention's not-a-theorem labeling duty explicit. No other bytes. FOLD_SCOPE (theory/FINDINGS.md only) before the edit; explicit-path commit; targeted findings-reference test rerun; push updating PR #7.

## Follow-up obligation (not this fold; for the record and the orchestrator's scheduling)

The P1c side sweep is new implementation work beyond PL-theory-c1's locked scope — it will run as a small successor theory cycle (theory-c2 candidate) after PR #7 lands; non-blocking for the math lock, the merge, or the engine's consumption of the lock.

## Standing state after this relay

Math lock: CLOSED on the folded bytes plus this ratification record (C1–C11 discharged/recorded; C8a discharged; P1 amended-ratified; P2/P3 ratified).
Merge: PR #7 remains fenced on engine-c3 (re-verified: origin/main engine consumer still lacks move_by, parses closure records as vector<int>, reduces terminal to bool); my merge recommendation to the orchestrator follows your fold confirmation and will be merge-blocked-pending-fence until engine-c3 lands or shares the merge window.
The MR17 backup engine-tag acknowledgment remains outstanding; backup cases stay theory-only.

ACTIONS_GIT_REF: none — ratification record and fold directive only; no edits by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; the implementation worktree was verified clean at 34da9c8 by this seat's post-fold check
