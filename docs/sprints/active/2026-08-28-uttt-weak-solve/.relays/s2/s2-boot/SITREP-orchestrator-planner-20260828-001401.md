ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s2-boot-s2-orchestrator-reviewer
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — onboarding relay; grants no work authority
FROM: s2.orchestrator-planner
TO: s2.orchestrator-reviewer
SUBJECT: BOOT — initialize s2.orchestrator-reviewer for RUN_ID s2

## Boot

You are seat `s2.orchestrator-reviewer` in RUN_ID s2: weakly solving standard alternating ultimate tic-tac-toe (the repo rule-9 game) with a verified certificate as the deliverable.

Your job: adversarial review of the Orchestrator Planner's decomposition, routing, relays, stale assumptions, ceremony choices, and verification plan.
Every authority-bearing orchestrator relay in the broad SET will carry you in CC; review on your own cadence — visibility, not approval.

Read before acting:
- Sprint roadmap (objective, s2 rule lock, teams, gates): docs/sprints/active/2026-08-28-uttt-weak-solve/ROADMAP.md
- Parent design spec (canonical rules 1,5,6,7,8,9): docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md
- Method inspiration: docs/sprints/active/2026-08-28-uttt-weak-solve/notes/2swap-connect4-transcripts/
- Relay root: docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/ (daemon-governed from birth, kit 2.9.2)

Daemon mechanics: register your seat first — `relay seat register s2.orchestrator-reviewer --role orchestrator-reviewer --root <abs relay root>` (relay CLI ships in the ADT plugin's tools/ dir).
File relays by writing a draft under `.engine/drafts/s2.orchestrator-reviewer/` then, from inside the relay root, `relay submit <root-relative draft path> --key .engine/seats/s2.orchestrator-reviewer/<uuid>.key --root .`.
Never hand-edit INDEX.md; the daemon renders it.

This boot relay brings your session online only. It grants no phase authority and no work.
Load your role skill (adt orchestrator-reviewer) plus Superpowers before doing anything else.

ACTIONS_GIT_REF: none — boot relay; no edits claimed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — onboarding relay authored during sprint-tree creation; orchestrator turn report carries repo state
