ROLE: Pair Implementer
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c2-fold-report-2
PARENT_DISPATCH_ID: s2-theory-c2-fold-scope-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — fold is pushed and verification-complete, but merge remains the separate operator/orchestrator gate
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/REVIEW-FOLD-pair-implementer-20260828-193007.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c2-20260828
PLAN_ARTIFACT: PL-theory-c2-20260828
PLAN_SHA256: 4558eeda83cdc6e94b2bf45e93161856b7ab1cda6ac53706926504103d4bd5f7
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
BASE: theory/certschema-c2@45994d31f2b2ca09b08b13352a2a2644eb427663
TARGET_BRANCH: main
BRANCH: theory/certschema-c2
PR: https://github.com/iwnlcern/utt/pull/12
SUBJECT: FOLD REPORT — theory-c2 MF-1/MF-2 complete at PR #12 head 4ac333c

## Summary

Folded the two addressed must-fix items and no optionals. MF-1 now binds every mutant to an exact parsed-field change set, including V02-V08; H09 is byte-equal to the p1 header prefix through the zeroed manifest_rows u32; H11 changes exactly the low byte of row-0 byte_offset and the 16 manifest-digest bytes. MF-2 adds resealed p1 mutant MUT-R21, whose X-terminal 9/8 state is marked OPPONENT with payload 0xFF; its catalogue row uses the same `§3.2/§6.4` DD trace row as MUT-R12.

The deterministic census is now 43 mutants and 49 catalogue rows. Existing generated artifacts are unchanged; only `MUT-R21.bin` is new. The strongly recommended and discretionary optionals remain deferred.

FOLD_SCOPE:
- theory/certschema/vectors.py -> in
- theory/certschema/catalogue.json -> in
- theory/certschema/mutants/MUT-R21.bin -> in
- theory/tests/test_certschema_mutants.py -> in
FOLD_SCOPE_RESULT: all-in

## RED/GREEN evidence

- RED: after the test-only edit, `uv run pytest -q tests/test_certschema_mutants.py` reported exactly 2 failures and 7 passes: the expected-id census lacked MUT-R21 and `build_mutant("MUT-R21")` raised KeyError.
- GREEN: after the one-entry vectors implementation, the same focused file reported 9 passed.
- Focused committed behavior: `uv run pytest tests/test_certschema_mutants.py tests/test_certschema_vectors.py -v` reported 16 passed in 3.64s.
- Full committed-head E2: `cd theory && uv run pytest` reported 141 passed in 101.64s.

## Generated-byte and scope proof

- Two consecutive in-place generations produced identical SHA-256 values: catalogue `8b8ebc801a25e1468389c737d23e9315c23708af958955208c2a490e32f73e8f`; MUT-R21 `ac7c48462fa3b370a7d4ce7d042fdaa4bc1a3716059be36cb62e11d37431d4ec`.
- The committed fresh-output comparison in `test_committed_artifacts_match_generator_output` passed as part of both focused and full runs.
- `git diff --check 45994d3..4ac333c` passed.
- `git diff --name-only 45994d3..4ac333c` returned exactly the four FOLD_SCOPE paths above; the new binary is 237 bytes.
- Feature worktree status is clean after commit and push.

## Acceptance status

- MF-1: PASS E2 — exact H09/H11 byte isolation and the parsed-field change table close all three demonstrated false-green escape routes.
- MF-2: PASS E2 — MUT-R21 is in the code census, catalogue, checked-in corpus, deterministic regeneration, parsed-field table, kind/payload test, and terminal/parity test.
- No DD, plan, existing vector/mutant, fixture, checker, solver, UI, merge, deployment, publication, or release change was made.

Implementation and local verification are complete and pushed. This is merge-ready input to the planner's targeted fold check; it is not a merge authorization and PR #12 remains open and unmerged.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no credential or permission surface
- migration/backfill/destructive-write/canonical-data-repair: no — deterministic local test corpus only
- money/inventory/orders/planning/accounting/trust-critical-state: no — deterministic game/certificate state only
- AI-or-automation-acts-downstream: yes — the corpus constrains a downstream automated certificate checker
- worker/scheduler/queue/retry/async-side-effect: no — no asynchronous runtime behavior
- cross-repo/service-contract/generated-schema/shared-API-event: yes — generated certificate artifacts implement the existing locked cross-pair contract
- user-visible-control-with-materializer/downstream-consumer: no — no UI or deployed consumer
- test-runtime-role-mismatch: no — checked-in bytes and parser-level target fields are exercised directly
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — both must-fixes are exact, E2 passed, and optionals were deferred
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: branch theory/certschema-c2@4ac333ce0e9ae40d92671b4b90b520553ddb98ab; origin/theory/certschema-c2@4ac333ce0e9ae40d92671b4b90b520553ddb98ab; PR #12 head 4ac333ce0e9ae40d92671b4b90b520553ddb98ab; one fold commit over 45994d31f2b2ca09b08b13352a2a2644eb427663; no merge claimed
FINAL_GIT_STATUS_SHORT: none — clean feature worktree at 4ac333ce0e9ae40d92671b4b90b520553ddb98ab
