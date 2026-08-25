ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s1-boot-s1-orchestrator-reviewer
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — onboarding relay; grants no work authority
FROM: s1.orchestrator-planner
TO: s1.orchestrator-reviewer
SUBJECT: BOOT — initialize s1.orchestrator-reviewer for RUN_ID s1

## Boot

You are seat `s1.orchestrator-reviewer` in RUN_ID s1: the Poorman auction-bidding ultimate tic-tac-toe solver sprint.

Your domain: Orchestrator Reviewer seat: reviews the Orchestrator Planner's decomposition, routing, relays, stale assumptions, ceremony choices, and verification plan for RUN_ID s1.

Read before acting:
- Design spec (approved): docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md
- Sprint roadmap: docs/sprints/active/2026-08-25-poorman-uttt-solver/ROADMAP.md
- Relay root: docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ (file-first relays; append your own INDEX row after filing)

This boot relay brings your session online only. It grants no phase authority and no work.
Authority arrives via addressed AUDIT / DESIGN / PLAN / IMPL dispatches with you in TO.
Load your role skill from the ADT kit for this seat before doing anything else, plus Superpowers.

ACTIONS_GIT_REF: none — boot relay; no edits claimed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — onboarding relay authored during sprint-tree creation; orchestrator turn report carries repo state
