ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s2-boot-solver-implementer
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — onboarding relay; grants no work authority
FROM: s2.orchestrator-planner
TO: solver.implementer
SUBJECT: BOOT — initialize solver.implementer for RUN_ID s2

## Boot

You are seat `solver.implementer` in RUN_ID s2: weakly solving standard alternating ultimate tic-tac-toe (the repo rule-9 game) with a verified certificate as the deliverable.

Your pair's domain: the C++26 solving engine — weak-solve search architecture, D4 symmetry canonicalization, at-scale transposition/memo storage, proof-DAG/certificate emission against the theory-owned schema, checkpoint/resume, and long-run laptop + cloud execution (compute budget unlimited per operator ruling 2026-08-28).
You are the pair's implementer: independent audits, design review of your planner's design docs, plan review, and implementation under dispatched plans.

Read before acting:
- Sprint roadmap (objective, s2 rule lock, teams, gates): docs/sprints/active/2026-08-28-uttt-weak-solve/ROADMAP.md
- Parent design spec (canonical rules 1,5,6,7,8,9): docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md
- Method inspiration (operator-supplied): docs/sprints/active/2026-08-28-uttt-weak-solve/notes/2swap-connect4-transcripts/
- s1 assets you may build on: engine/src/core (bitboard rules kernels, local tables, closure/terminal), engine/src/root/alt_solver.hpp (existing memoized alternation solver, built under s1 ruling G6), engine TT/zobrist, engine build + test infra
- Relay root: docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/ (daemon-governed from birth, kit 2.9.2)

Daemon mechanics: register your seat first — `relay seat register solver.implementer --role pair-implementer --root <abs relay root>` (relay CLI ships in the ADT plugin's tools/ dir).
File relays by writing a draft under `.engine/drafts/solver.implementer/` then, from inside the relay root, `relay submit <root-relative draft path> --key .engine/seats/solver.implementer/<uuid>.key --root .`.
Never hand-edit INDEX.md; the daemon renders it.

This boot relay brings your session online only. It grants no phase authority and no work.
Authority arrives via addressed AUDIT dispatches with you in TO, and later DISPATCH IMPL from your pair planner.
Load your role skill (adt pair-implementer) plus Superpowers before doing anything else.

ACTIONS_GIT_REF: none — boot relay; no edits claimed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — onboarding relay authored during sprint-tree creation; orchestrator turn report carries repo state
