## Team theory — theory-c3: PROCEED TO PLAN (amended scope ruling)

ROLE: Orchestrator Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c3-proceed-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — scope disposition is orchestrator authority; merge remains the standing operator gate
FROM: s2.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/SITREP-pair-planner-20260830-005916.md
PARENT_DISPATCH_ID: s2-theory-c2
APPROVED_DESIGN_DOC_ID: DD-theory-c2-20260828
APPROVING_DESIGN_REVIEW_DISPATCH_ID: s2-theory-c2-design-review-4
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@7c254bf
TARGET_BRANCH: main
DELEGATED_DISPATCH_AUTHORITY: yes
DELEGATED_DISPATCH_CONDITIONS: Implementer plan review = approve; pair-Planner dispatch PARENT_DISPATCH_ID points to that approve relay; that review parents to this pair-Planner PLAN; SCOPE_DIFF_RESULT = all-in; no hard trigger; no boundary-contract deviation; no cross-bundle collision.
SUBJECT: PROCEED-TO-PLAN (rev 2) — theory-c3 scope ruling: NARROW; supersedes proceed-1's scope text

## Scope ruling (answers s2-theory-c3-scope-2; MR-4 disposition)

Ruling: option 1 — NARROW. The theory-c3 bundle scope excludes the resume journal and the alternating fixture profile.

- The reviewer's mechanical point is upheld: proceed-1's paraphrase did not and could not amend the approved DD §11 bytes; scope ambiguity is resolved here, at the dispatching seat, not by pair interpretation.
- Resume journal → c6 ledger. Its purpose (resumable verification of certificates too large for one pass) is meaningless at corpus scale and is already c6's own §11 line ("external-memory verified-set, resume-under-interruption proof"). DD §4.6 semantics are unchanged and land with c6.
- Alternating fixture profile → as-needed ledger. The checker's fixture tests consume v1 through the consumed_by/compatibility matrix the landed corpus already pinned; the profile adds bytes without adding verification power in this bundle. Ruling R-s2-4 (v1 frozen, s2 additions additive) is untouched; the profile remains the named mechanism if a future bundle needs auction-free fixtures.
- Both exclusions are dispatch-level scope dispositions; the design doc's bytes and locks are not amended.

## Amended bundle scope (replaces proceed-1's scope paragraph; all else in proceed-1 stands)

theory-c3 = DD-theory-c2 §11 c3 slice MINUS the resume journal and alternating fixture profile: the independent checker in C3 (newest stable, idiomatic; grill decision of record), fresh rules implementation per R-s2-3 with the R-s2-2 independence boundary, parser, canonical state + D4 identity, streaming/chunk verification (single-pass; no resume journal), and the full gate battery — reject all 43 landed mutants for their intended reasons, accept the byte-pinned positives — plus the carried c2 backlog (multi-reply OPPONENT positive P4 + dropped-reply mutant; catalogue count pin; R17 reason wording).

Sequencing: fold this ruling by id into the successor PLAN relay; plan review remains the plan gate; the delegated conditions above are unchanged, and the standing cross-pair contract trigger still routes the dispatch decision to this seat as before.

ACTIONS_GIT_REF: none — sequencing/ruling relay; committed by the orchestrator turn (see repo log)
FINAL_GIT_STATUS_SHORT: unavailable — sequencing relay; the orchestrator turn report carries post-commit repo state
