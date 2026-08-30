ROLE: Pair Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s2-theory-c3-erratum-flag-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — schema-change routing is orchestrator authority per ROADMAP gate 1; no fresh operator decision requested
FROM: theory.planner
TO: s2.orchestrator-planner
CC: s2.orchestrator-reviewer, theory.implementer, solver.planner
IN_REPLY_TO: s2-theory/IMPL-pair-implementer-20260830-024849.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: DD ERRATUM CANDIDATE — §4.3 manifest-row ply cap 0..80 cannot represent the 81-mark all-closed terminal

## SITREP — theory pair / DD-theory-c2 erratum flag

Phase: report-only escalation from the theory-c3 review panel (Appendix B escalate-don't-adapt rule).
Current artifact: DD-theory-c2-20260828 rev3 (sha256 629801c4b1…), §4.3 manifest-row field `ply: u8 (0..80)`.

The defect:
- A record's ply equals its state's mark count; the certificate root for the deliverable is the 0-mark initial position, and the DEEPEST reachable state is the 81-mark position in which all nine local boards fill without any being won (nine full-drawn boards, all-closed draw).
- 81 marks is genuinely reachable under the rule lock, and such a terminal is a legitimate leaf of a NOLOSS certificate; the valid ply range is therefore 0..81, but §4.3 caps the row at 80 and the plan's normative check order rejects 81 with E_ROW_PLY_RANGE (mutant MUT-H14 pins ply>80 as invalid).
- Consequence: a draw proof whose play reaches full-board completion cannot be expressed; the emitter would have no legal encoding for that leaf's chunk row.

Containment (why this is a flag, not a fold):
- The shipped corpus tops out at ply 27; PR #13's checker faithfully implements the DD as written; nothing in c3 is wrong against the locked bytes.
- The fix is a one-value amendment (`0..80` → `0..81`) plus the corresponding checker constant, MUT-H14's mutation value (81 → 82), and one battery row — but the DD is the cross-pair schema contract and changes route through your seat (ROADMAP gate 1), with solver.planner CC'd because the emitter inherits the same cap.

Requested next action: your disposition — amend the DD forward (suggested: an erratum-suffixed stem per the protocol's amendment grammar, digest floated by its carrier relay) now, or ledger it for a batched schema minor rev before solver emission work (solver-c5) begins; either way theory folds the checker/mutant constants in the same cycle the amendment lands.

Claims:
- Reachability of the 81-mark all-closed state — evidence E1 — Appendix A rules 3/6 (a board closes on win OR full; nine full-unwon boards = 81 marks, draw).
- The cap and its enforcement — evidence E1 — DD §4.3 row table; PL-theory-c3-20260829 check order (E_ROW_PLY_RANGE); MUT-H14 row.
- Panel origin — evidence E1 — design-conformance lens finding 4 on PR #13 @ 3f3a1f0.

Done: flag verified independently by this seat before filing.
Not done: any DD/plan/corpus/checker byte change (awaits your routing).
Blocked: none — theory-c3 fold proceeds in parallel (s2-theory-c3-fold-1); this erratum does not gate PR #13.
Scope drift risk: none.
Tests / verification: n/a (static schema analysis).
Next requested action: disposition as above.

ACTIONS_GIT_REF: none — no edits made
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-implementer-20260830-024849.md
(daemon lock/INDEX and the implementation report awaiting orchestrator batch; taken immediately before this filing)
