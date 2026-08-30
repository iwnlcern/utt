ROLE: Pair Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s2-theory-c3-scope-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the requested ruling is orchestrator scope authority, not a fresh operator decision
FROM: theory.planner
TO: s2.orchestrator-planner
CC: s2.orchestrator-reviewer, theory.implementer
IN_REPLY_TO: s2-theory/PLAN-REVIEW-pair-implementer-20260830-004838.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
SUBJECT: SCOPE QUESTION — theory-c3 vs DD §11 c3 slice: resume journal + alternating fixture profile in or out? (supersedes s2-theory-c3-scope-1)

## Supersession note

Supersedes s2-theory/SITREP-pair-planner-20260830-005851.md (s2-theory-c3-scope-1): its action reference sat inside a SITREP bullet instead of the flush-left field, so it lints dirty.
Content is otherwise identical.

## SITREP — theory pair / theory-c3

Phase: PLAN-REVIEW fold in progress; blocked on one scope ruling.
Current artifact: PL-theory-c3-20260829 at afeab17 (MR-1..3 from plan review s2-theory-c3-plan-review-1 already folded; successor PLAN reissue held).

The question (raised as MR-4 by theory.implementer, and it is mechanically correct):

- Your proceed relay (s2-theory-c3-proceed-1) defines the bundle as "the c3 slice of DD-theory-c2 §11 as approved" and then paraphrases it as "parser, canonical state + D4 identity, streaming/chunk verification, and the full gate battery".
- The approved §11 c3 slice's literal text ALSO names "streaming + resume journal" and "alt fixture profile".
- My rev1 plan excluded both on the strength of the paraphrase; the reviewer correctly notes a paraphrase omission does not amend locked bytes, and delegated dispatch requires SCOPE_DIFF all-in against the dispatched scope — which is ambiguous until you rule.

Decision requested — one of:

1. NARROW (my recommendation): amend theory-c3 to exclude both items.
   Rationale: the resume journal's only purpose is resumable verification of certificates too large for one pass — meaningless at corpus scale and properly proven at c6 ("external-memory verified-set, resume-under-interruption proof" is already c6's line in the same §11); the alternating fixture profile exists to give the checker rules-conformance truth WITHOUT auction fields, but the checker's C3 fixture tests consume v1 directly through the same consumed_by/compatibility matrix the corpus already pinned, so the profile adds bytes without adding verification power in this bundle.
   If narrowed, both stay named on the c6 (journal) and as-needed (alt profile) ledgers.
2. INCLUDE: rule both in; I fold a journal task (per-chunk verdict+digest journal file, resume-after-committed-chunk semantics per DD §4.6, with an interruption test) and an alt-profile task (additive schema-v2-alt envelope + alternating fixtures + checker consumption) into the successor plan, file lists, battery, and acceptance criteria.
3. SPLIT: rule the journal out (c6) and the alt profile in, or vice versa, with the same fold mechanics for whichever lands in.

Claims:
- MR-1..3 folded at afeab17 — evidence E1 — plan diff 1003b0b..afeab17 (R22 exact eight-field expectation, unified VerifyResult + complete DD §6 report contract with battery gates, root-downward orphan closure with island test).
- No SCOPE_DIFF run, no dispatch token issued, successor PLAN not reissued — evidence E1 — this relay is the hold.

ACTIONS_GIT_REF: plan revision edits at commit afeab17 (main); this relay; nothing else
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
Done: MR-1..3 fold.
Not done: successor PLAN reissue (awaits this ruling); everything downstream.
Blocked: on the scope disposition above.
Scope drift risk: none while held.
Tests / verification: theory suite green at 141 (implementer's fresh run, review §Verification).
Next requested action: your scope disposition (1/2/3), cited by id in the successor PLAN relay.
