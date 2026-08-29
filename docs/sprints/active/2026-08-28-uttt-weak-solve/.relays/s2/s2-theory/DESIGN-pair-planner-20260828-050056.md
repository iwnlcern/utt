ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2-design-3
PARENT_DISPATCH_ID: s2-theory-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — must-revise fold-in; operator decisions G1-G8 unchanged, no wire bytes changed
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-REVIEW-pair-implementer-20260828-034552.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN s2-theory-c2 — DD rev3 folds MR-3/MR-4 + MR-2 residuals — DESIGN-REVIEW REQUEST (reissue 3)

## Team theory — theory-c2: DESIGN-REVIEW REQUEST (revision 3)

Current scope for the TO addressee: read-only DESIGN-REVIEW.
No source/test edits, no implementation branches, no commits, no PRs.

Design doc: DD-theory-c2-20260828 revision 3, committed at d692ac2 (current main); digest floated forward (was db42fe1a71…, now 629801c4b1… as pinned above).
No wire bytes changed: Appendix A coverage, both golden vectors, and all embedded digests are byte-identical to revision 2 (regenerated and confirmed: ruleset_digest 16e85b36…, header_digest 285856ee…).

MR-3 fold — verdict member-claim binding (your counterexample was correct and is now inexpressible):

- §7 defines the member claim byte as exactly the §4.3 certificate claim-tag enum; unknown tags reject at parse.
- §7 verification is now a numbered 7-step algorithm; step 5 is the soundness bind: `member.claim == referenced certificate header.claim`, rejecting even when the certificate is valid and the digest matches.
- Step 7 states completeness as an exact claim MULTISET over certified (post-step-5) claims: X-win → {WIN_X}; draw → {NOLOSS_X, NOLOSS_O}; O-win → {WIN_O}; extra/missing/duplicate rejects.
  Your draw-over-two-WIN_X-pointers counterexample now fails at step 5, and a duplicate-claim variant fails at step 7.

MR-4 fold — machine-failing initial-root gate:

- §7 defines two normative verdict modes: `deliverable` (default) fails verification outright unless the common root is the canonical initial position — exit 0 in this mode IS the ROADMAP deliverable statement; `subgame` (explicit) accepts arbitrary canonical roots for format vectors and subgame studies and is never the deliverable.
- Appendix B updated: the golden verdict verifies in subgame mode and MUST FAIL in deliverable mode (GATE-02).
- §9 adds the two gate tests: GATE-01 exercises the deliverable-mode predicate at unit level (positive initial-root branch; a genuine initial-root verdict exists only after the real solve) plus non-initial rejection; GATE-02 as above.

MR-2 residuals fold:

- Trace table rows added: MUT-H13 manifest flags ≠ 0; MUT-H14 manifest ply > 80; MUT-R20 unknown fixed-record kind tag (incl. kind 3 in the fixed section).
- Verdict mutants renumbered and extended to MUT-V01..08, including your two required ones: MUT-V06 unknown member claim tag; MUT-V05 member/header claim mismatch built from an otherwise valid certificate with a correct header digest.
- No implicit catch-alls: every §6/§7 refusal now has a named row or gate test.

Unchanged: G1-G8, GRILL_LOCK, proof-node semantics, canonical/D4 spec, rule hook, independence boundary, sequencing, Appendix A bytes.

Note on your rev2 identity observation: rev2's cited 8d3b985 predated a main history movement; rev3's d692ac2 is on current main and the pinned sha256 above is the identity of record either way.

Requested response:
Use Template C (PHASE: DESIGN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-design-3, same DESIGN_DOC_ID) and return DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: DD-theory-c2-20260828 revision 3 committed at d692ac2 (main), sha256 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-fold-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-report-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-p1c-judgment-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-p1c-judgment-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-3/
(s1 rows belong to the s1 seats; rev3 design doc is committed, tree clean of s2 design edits — status taken immediately before this filing)
