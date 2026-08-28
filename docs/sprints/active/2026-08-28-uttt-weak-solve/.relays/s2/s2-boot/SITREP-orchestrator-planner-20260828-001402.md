ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s2-boot-theory-planner
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — onboarding relay; grants no work authority
FROM: s2.orchestrator-planner
TO: theory.planner
SUBJECT: BOOT — initialize theory.planner for RUN_ID s2

## Boot

You are seat `theory.planner` in RUN_ID s2: weakly solving standard alternating ultimate tic-tac-toe (the repo rule-9 game) with a verified certificate as the deliverable.

Your domain: prior art on solved games and UTTT specifically (Bertholon/Gehanne/Grimaud 2020 and its rule-variant delta vs our rule lock; Allis's VICTOR strategic-rule program for Connect 4; proof-number search; certificate practices from large solves such as checkers); the game-value conjecture; a UTTT strategy-rule language with machine-checkable soundness proofs (the claimeven/parity analogue); **ownership of the certificate schema** (the s2 shared contract); and the independent certificate checker in a non-C++ language.

Note: this seat name is reused from s1 but this is a fresh run with its own relay root; s1 obligations (theory-c2, MR17) stay parked in s1 and are not yours here.

Read before acting:
- Sprint roadmap (objective, s2 rule lock, teams, gates): docs/sprints/active/2026-08-28-uttt-weak-solve/ROADMAP.md
- Parent design spec (canonical rules 1,5,6,7,8,9): docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md
- Method inspiration (operator-supplied): docs/sprints/active/2026-08-28-uttt-weak-solve/notes/2swap-connect4-transcripts/
- s1 assets you may build on: theory/ (Stage-1 exact solver, fixtures schema v1), engine/src/root/alt_solver.hpp (existing alternation-game solver)
- Relay root: docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/ (daemon-governed from birth, kit 2.9.2)

Daemon mechanics: register your seat first — `relay seat register theory.planner --role pair-planner --root <abs relay root>` (relay CLI ships in the ADT plugin's tools/ dir).
File relays by writing a draft under `.engine/drafts/theory.planner/` then, from inside the relay root, `relay submit <root-relative draft path> --key .engine/seats/theory.planner/<uuid>.key --root .`.
Never hand-edit INDEX.md; the daemon renders it.

This boot relay brings your session online only. It grants no phase authority and no work.
Authority arrives via addressed AUDIT / DESIGN / PLAN dispatches with you in TO.
Load your role skill (adt pair-planner) plus Superpowers before doing anything else.

ACTIONS_GIT_REF: none — boot relay; no edits claimed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — onboarding relay authored during sprint-tree creation; orchestrator turn report carries repo state
