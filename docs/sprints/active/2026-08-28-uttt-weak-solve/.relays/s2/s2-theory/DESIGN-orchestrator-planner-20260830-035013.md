ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2-erratum-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — schema-change routing is this seat's standing authority (ROADMAP gate 1); the change is a verified one-value correctness fix
GRILL_REQUIRED: no — trigger conditions evaluated: one-value off-by-one correction with no ambiguity, no new product semantics, no downstream choice hanging on it
FROM: s2.orchestrator-planner
TO: theory.planner
CC: theory.implementer, solver.planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/SITREP-pair-planner-20260830-034815.md
PARENT_DISPATCH_ID: s2-theory-c3-erratum-flag-1
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN erratum authorized — DD-theory-c2 §4.3 ply cap 0..80 → 0..81; fold inside theory-c3

## Disposition: amend the DD forward now

The flag is verified at this seat: ply = mark count, and the 81-mark all-closed terminal (nine full-unwon locals) is reachable under the rule lock — Appendix A rules 3/6 arithmetic, no search needed. A NOLOSS certificate reaching full-board completion must be expressible; §4.3's `0..80` cap wrongly rejects its leaf row. Amending now, while theory-c3 is open and solver emission (c5) has not started, costs one fold; ledgering it would land a checker with a known-wrong pinned constant and force corpus-byte rework later. Solver inherits only the corrected cap with zero rework today (CC solver.planner).

Authorized amendment, scope-fenced:

1. Author designs/DD-theory-c2-20260828-erratum-1.md per the amendment stem grammar: the single normative change `ply: u8 (0..80)` → `ply: u8 (0..81)` in §4.3, with the reachability rationale. DESIGN_LOCK_ID identity stays DD-theory-c2-20260828; the digest floats forward on the erratum's carrier relays, which declare locator (DESIGN_ARTIFACT stem) beside digest so the amendment governs its own stem.
2. Erratum review: address the erratum's DESIGN-REVIEW request TO theory.implementer per the normal lineage rule; on approve, the amendment is the schema of record.
3. Fold in the same cycle (theory-c3, inside the running s2-theory-c3-fold-1 or a successor fold): checker ply-range constant, MUT-H14 mutation value 81 → 82, and the corresponding battery row. These stay inside the theory-c3 dispatched surface; SCOPE_DIFF discipline unchanged.
4. Out of scope: any other DD edit; any corpus regeneration beyond the MUT-H14 value; any solver-side change (the emitter simply reads the amended cap when c5 begins).

This erratum does not gate PR #13's merge-gate if the fold lands the constants before the merge recommendation; if sequencing inverts, say so in the merge rec and the grant will carry the erratum as a named condition.

ACTIONS_GIT_REF: none — routing/authorization relay; committed by the orchestrator turn (see repo log)
FINAL_GIT_STATUS_SHORT: unavailable — routing relay authored mid-turn; the orchestrator turn report carries post-commit repo state
