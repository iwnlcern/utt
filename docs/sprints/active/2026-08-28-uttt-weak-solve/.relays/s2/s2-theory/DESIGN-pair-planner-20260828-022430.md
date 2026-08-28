ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2-design-1
PARENT_DISPATCH_ID: s2-theory-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — operator decisions were taken live in the grill (GRILL-theory-c2-20260828); this relay requests the pair Implementer design review
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-orchestrator-planner-20260828-010351.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: a97664537b93615f04e55a1bcb85d8feec2f30845a6392b682033c8d8ee55e5c
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN s2-theory-c2 — DD-theory-c2-20260828 (certificate schema v0) — DESIGN-REVIEW REQUEST

## Team theory — theory-c2: DESIGN-REVIEW REQUEST

Current scope for the TO addressee: read-only DESIGN-REVIEW.
No source/test edits, no implementation branches, no commits, no PRs.

Design doc to review:

- DESIGN_DOC_ID: DD-theory-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-theory-c2-20260828.md (sha256 above)
- Grill: GRILL_REQUIRED was yes on the dispatch; the grill ran against the operator and GRILL-theory-c2-20260828 is embedded as doc §10.

Selected option (summary; the doc is normative):

- Proof semantics: dual boolean certificates — claims WIN_X/WIN_O/NOLOSS_X/NOLOSS_O, one claim per artifact; verdict manifest assembles the game value (draw = NOLOSS_X + NOLOSS_O); PROVER nodes declare one legal move, OPPONENT nodes are checker-enumerated total, terminals recomputed, never trusted.
- Canonical state: 37-byte serialization of (x[9], o[9], forced), side derived from mark parity (R-s2-1); D4-only canonical form = lex-min over the 8 jointly-acting transforms (tables spelled out in doc §2.3); node identity = full canonical bytes, no hash-as-identity; moves stored in the parent's canonical frame; color swap excluded from identity.
- Container: leveled-DAG, ply-descending chunks (child-before-parent by construction), fixed 40-byte records + variable rule-leaf section, header with ruleset id/digest + manifest; XXH3-128 digests, integrity-only (soundness never rests on digests); fully materialized, checker never invokes a solver.
- Rule hook: RULE leaves carry (rule_id, version, witness); v0 registry is empty and fails closed at the header; registration = soundness artifact + checker validator + minor version bump via orchestrator.
- Independence: checker in C3 (newest stable, idiomatic — operator ruling), fresh rules per R-s2-3, throughput justification in §8 against the 1e6–1e12 bracket; shared artifacts = design doc + fixtures only.
- Tests: fixtures v1 frozen (R-s2-4); additive alternating profile + hand-authored positive certificates + named-mutant catalogue MUT-01..15 (doc §9).
- Sequencing: c3 checker v0 → c4 smallest rule kernel → c5 rule-leaf extension + emitter interop → c6 scale verification; value-conjecture memo event-driven (doc §11).

Rejected alternatives (full reasons in GRILL_LOCK §10): exact-minimax three-valued DAG; color-swap in identity; SHA-256/BLAKE3/gxhash; regenerable chunks; merged multi-claim container; Python/Go/dual-checker.

Boundary contract: doc §12 (schema is the cross-pair contract; solver emits, checker verifies, changes route through the orchestrator per ROADMAP gate 1).

Acceptance criteria draft (from the dispatch, restated as review lenses):

- Emit-and-verify from the document alone — check §§2–7 for any oral-context dependency.
- Every leaf class names trust base + verification obligation — §3.2, §5.1.
- Draw semantics survive adversarial read — §3.1/§7: no encoding accepts a weaker claim as stronger.
- Canonical form exact enough for byte-for-byte agreement of independent implementations — §2.2–2.4 (please check the D4 tables independently; a wrong permutation table here is the most expensive possible bug).

Operator decisions/defaults: grill decisions G1–G8 in §10; still-operator-owned: none for this design.

Open questions: none from my side; flag anything as must-revise findings.

Requested response:
Use Template C (PHASE: DESIGN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-design-1, same DESIGN_DOC_ID) and return DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: authored docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-theory-c2-20260828.md (untracked at status time, sha256 a97664537b93615f04e55a1bcb85d8feec2f30845a6392b682033c8d8ee55e5c; commit follows this filing) and this relay; no source/test edits
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
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/
(s1 rows belong to the s1 seats; the designs/ row is this design's deliverable)
