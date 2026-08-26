ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c2-plan-review-5
PARENT_DISPATCH_ID: harness-c2-plan-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes - the nineteen-log canonical baseline reset requires an addressed orchestrator ruling; CC silence is not authorization
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-plan-4/PLAN-planner-20260825-220117.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
PLAN_SHA256: ce03ccd9330a60e9bedcaa34a4965dbedac0a768c64b9454311445ca2ffa7a0e
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c2
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: main@ac0493eb0496e42b25c93e05001b443573b79537 observed during review; dispatch-time rebase remains required
TARGET_BRANCH: main
BRANCH: none - PLAN-REVIEW is read-only
SUBJECT: PLAN-REVIEW must-revise - baseline-reset authority and temp-root argv layout remain open

## Verdict

PLAN_REVIEW_VERDICT: must-revise

Plan-4 corrects the zero-clock premise, makes matrix.json independent, spells both sidecars literally, and replaces dotfile-ignore with exact inventory. Two blockers remain: the canonical event-log reset lacks addressed governing-seat authorization, and the temporary repo shape does not preserve committed stub argv. Implementation stays held.

## Required revisions

### MR2-A residual - addressed ruling required

Plan lines 74 and 78 intentionally replace timing bytes in all nineteen committed event logs. This is a canonical-corpus baseline change. The protocol classifies canonical-data repair and residual contract risk as hard triggers, while commissioning relay harness-c2 delegates dispatch only with no hard trigger and no boundary-contract deviation.

Plan-4 line 11 says HUMAN_GATE_REQUIRED: no; lines 37/78 treat CC visibility and a veto window as sufficient. CC informs; silence grants no authority. Obtain an addressed orchestrator ruling accepting the elapsed-field reset under harness-c2, or use byte-preserving clock vectors. Reissue with that ruling and consistent gate fields. Do not infer scope confirmation from the broad referee path alone.

### MR2-C residual - preserve argv layout in both modes

Plan line 79 places stub_engine.py with bot files under temporary referee/bots. Census of all nineteen logs shows every stub command invokes python referee/tests/stub_engine.py; the only non-stub paths are referee/bots/fraction_bot.py and referee/bots/random_bot.py. The proposed layout makes verbatim argv fail.

Pin path-preserving copies at tmp/referee/tests/stub_engine.py and the two referenced bot paths, or copy exact referenced scripts from the manifest. Add a test that runs unchanged argv from the temporary root.

Isolation must cover bare regeneration too. Line 79 isolates only --check, while line 74 says transient markers exist only in the temporary tree and line 79 says bare mode regenerates in place. Six argv paths target referee/tests/fixtures relative to cwd, so in-place generation creates or deletes marker state in the committed tree. Generate bare-mode artifacts in the same temporary cwd and promote only the exact allowlist after success, or pin an equally non-destructive mechanism. No mode may use committed fixtures as transient engine state.

## Accepted corrections

MR2-A's premise, MR2-B's independent manifest, MR2-C's exact inventory, and MR2-D's literal sidecars are accepted. Prior accepted pin, bijection, delivery, sink, reader, glob, STOP, exit, and separate-merge contracts remain accepted.

## Evidence

- Incoming relay SHA256 d7d9b975d27421c55e773a9f4dd2a4d77fc6d15fd963d97448d84675283ebe4e; plan SHA256 ce03ccd9330a60e9bedcaa34a4965dbedac0a768c64b9454311445ca2ffa7a0e; design SHA256 c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e.
- Incoming exact-file and selected-file root-context lint are clean. Whole-root lint is red from inherited INDEX inversions and two immutable historical merge-claim carriers.
- All nineteen logs point stub argv to referee/tests/stub_engine.py; only fraction_bot.py and random_bot.py are referenced under referee/bots.
- The commissioning relay requires no hard trigger; no later addressed reset ruling exists.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no - local fixture/runtime plan review
- migration/backfill/destructive-write/canonical-data-repair: yes - reset of nineteen committed event logs
- money/inventory/orders/planning/accounting/trust-critical-state: no - simulated budgets only
- AI-or-automation-acts-downstream: yes - referee drives subprocess engines
- worker/scheduler/queue/retry/async-side-effect: yes - recovery and marker side effects
- cross-repo/service-contract/generated-schema/shared-API-event: yes - corpus consumed by engine-c2
- user-visible-control-with-materializer/downstream-consumer: no - no deployed UI
- test-runtime-role-mismatch: no - merged referee inspected
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes - reset and marker risks remain
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no - no downgrade requested; addressed ruling required
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none - review-only; no source, test, design, plan, branch, commit, PR, or integration action; only this daemon draft/admission is authored
FINAL_GIT_STATUS_SHORT: exact pre-admission status follows; product paths and existing harness worktree are clean

~~~text
 M .superpowers/sdd/PL-ui-c1-20260825/progress.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-8/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-9/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-8/
~~~

Next requested action: obtain the reset ruling, correct script layout and bare-mode isolation, then reissue harness-c2-plan-5. Do not dispatch implementation.
 reset and transient committed-tree marker risk require explicit closure
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested; an addressed governing-seat ruling is required
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none — review-only; no source, test, design, plan, branch, commit, PR, or integration action; only this daemon draft/admission is authored
FINAL_GIT_STATUS_SHORT: exact pre-admission status follows; product paths and the existing harness implementation worktree are clean

```text
 M .superpowers/sdd/PL-ui-c1-20260825/progress.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-6/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-12/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-8/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-9/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-8/
```

Next requested action: obtain the addressed baseline-reset ruling, correct the temporary script layout and bare-mode isolation, then reissue `harness-c2-plan-5` for review. Do not dispatch implementation from this verdict.
