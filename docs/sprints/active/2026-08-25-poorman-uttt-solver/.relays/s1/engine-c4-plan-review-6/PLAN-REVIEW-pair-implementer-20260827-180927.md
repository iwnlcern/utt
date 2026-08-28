ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-plan-review-6
PARENT_DISPATCH_ID: engine-c4-plan-6
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — two mechanical plan-carrier corrections remain; implementation and PR #10 merge stay held
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-plan-6/PLAN-planner-20260827-170019.md
DESIGN_LOCK_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1
PLAN_LOCK_ID: PL-engine-c4-20260826
PLAN_ARTIFACT: PL-engine-c4-20260826
PLAN_SHA256: 084be9943f1a15f5308d8adb0fffa42a9c03b35af0f5984ace6a9dce1e9a243f
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: PLAN-REVIEW must-revise — bind the rev-13 lock and certified-action predicates in the plan artifact

## Verdict

Must revise `PL-engine-c4-20260826` revision 6 at SHA-256 `084be9943f1a15f5308d8adb0fffa42a9c03b35af0f5984ace6a9dce1e9a243f` before an implementation dispatch.

Task 16's source/test fence matches the preserved `5907f93` product: the relevant root-matrix and search/refinement code is header-resident, the existing analyze subprocess harness is in `engine/tests/test_eval_quality.cpp`, and neither `bid_matrix.cpp` nor `clock.hpp` is required by the locked delta. One Task 16 commit is acceptable. The ordered RED/GREEN/mutation/full-suite/A10/A11 sequence is otherwise executable and carries panel items 1–11.

Two plan-carrier defects remain. The global Goal/Spec still bind revision 10 and its superseded digest, while Task 16 locally names revision 13. More importantly, the plan does not itself carry review-13 obligation 2: it names a generic certified/heuristic split and test cases but omits the exact ceiling-equality predicate, the X/O dominance inequalities, the ordered/zugzwang preconditions, and the fail-closed affordability/clamp behavior. The incoming PLAN relay summarizes those requirements, but the locked plan artifact is the implementation carrier and must contain them.

This verdict is review-only. It grants no source/test/design/plan edit by this seat, implementation, branch movement, commit, PR action, merge, publication, deployment, release, or external-use authority. PR #10 remains open and unmerged.

## Required revisions

### M1 — promote the plan-of-record metadata to DD revision 13

Update the plan's global Goal and Spec metadata to approved `DD-engine-math-c4-20260826` revision 13 at SHA-256 `cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1`. Preserve Tasks 1–15 as landed history and identify Task 16 as the current rev-13 delta. Remove or qualify the stale Task 14 PR-title instruction so the eventual PR carrier cannot present revision 10 as the governing design after Task 16 advances PR #10; Task 16's successor step should refresh the existing PR title/body to revision 13 rather than reopen it. — E1 — plan lines 5, 11, 188, 210–232; approved design-review-13.

### M2 — make the certified-action predicate executable in Task 16

Expand Task 16's `policy.hpp` responsibility and TDD steps to carry all of review-13 obligation 2 explicitly:

- require complete, sound aggregates and the ordered proof `a.hi <= b.lo` before the ordinary C8a certificate;
- compute both exact endpoint ceilings and certify only when `ceil_exact(r_enc.lo * M) == ceil_exact(r_enc.hi * M)`;
- require the selected X child to satisfy `selected.hi <= other.lo` for every other X child, and symmetrically the selected O child to satisfy `selected.lo >= other.hi` for every other O child;
- for zugzwang require `a.lo > b.hi`, bid 0, and dominance on the actual tie-owner side;
- check C8a affordability before treating the `[0, own stack]` clamp as an identity; a failed check must route to the ordinary matrix path, not assert-abort and not publish a changed `k`;
- route any failed predicate, incomplete/widened/tainted aggregate, ceiling straddle, overlap, or cancellation to matrix fallback; retain staged publication if that matrix is incomplete.

Keep the four named certificate mutation cases. Add a direct affordability/clamp mutation or equivalent named assertion so the fail-closed clamp contract is E2-observable rather than prose-only. — E1 — design rev 13 lines 154–162; design-review-13 successor obligation 2; plan lines 215–229.

## Passing disposition

- Routing and locks: pass. The incoming relay parents to approved `engine-c4-design-review-13`; the current design and plan bytes match their carried SHA-256 digests.
- File fence: pass. `policy.hpp`, `bid_matrix.hpp`, `p2_gate.hpp`, `search.hpp`, `main.cpp`, `fit_eval.py`, the four named unit-test files, and conditional CMake registration cover the preserved implementation seams. The analyze process case can extend the existing `test_eval_quality.cpp` harness; no new unit file is required.
- Panel ledger: pass subject to M2's explicit certified-action carrier. Staged publication, extraction filter, coin expectation, zero-total policy seam/TT snapshot, quantization discriminator, window/cut/P2 equivalence, anchor reserve, analyze ABI/bound, refine nesting, and mid-loop cancellation are named.
- Execution order and granularity: pass. One continuation task/commit is acceptable; RED precedes GREEN, mutations are run-and-revert, full unit/e2e proof precedes A10/A11, and merge remains separately gated.
- Scope boundary: pass. No theory, referee, rules-core, protocol, fixture-contract, `bid_matrix.cpp`, or `clock.hpp` byte is required by the locked delta.
- Operator judgment: none.

## Successor gate

The Planner must revise the same plan lock, commit the corrected plan bytes, and return a fresh PLAN relay parented to this review with a new plan digest. Any later implementation dispatch remains conditional on an approving PLAN-REVIEW, the standing `engine-c4-proceed-1` lineage, exhaustive all-in `SCOPE_DIFF`, no hard trigger or boundary/cross-bundle collision, and daemon admission. Until then, implementation remains unauthorized.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: yes — the future engine policy publishes moves and bids consumed by the referee
- worker/scheduler/queue/retry/async-side-effect: yes — acceptance invokes engine subprocesses through the referee
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory and referee contracts and exposes analyze JSONL
- user-visible-control-with-materializer/downstream-consumer: yes — policy output and analysis metadata have referee, fitter, and game-log consumers
- test-runtime-role-mismatch: yes — the certified-action implementation obligation is not fully represented in the locked task/test carrier
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — M1 and M2 are exact mechanical corrections under the approved design
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming exact-file relay lint passes with `--no-freshness`; design bytes hash to `cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1`, and plan bytes hash to `084be9943f1a15f5308d8adb0fffa42a9c03b35af0f5984ace6a9dce1e9a243f`. — E2 instrument evidence.
- Main is at `7b7a8bb`; the preserved implementation worktree is clean at `5907f937f07fd0309e141e3db706064780822d88`. Source symbol inspection confirms the file-fence disposition above. — E2 instrument evidence.
- Daemon was ready with zero pending renders before filing. Root verification reports only the inherited historical `INDEX.md` divergence at digest `b5a880e0c459ad8b62a9431902551b656dad5c03671a99bf0ca19c9b2cdc2c47`; it was not repaired or rewritten.
- Concurrent daemon-rendered `SEATS.md` and foreign theory work were preserved. No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-pair-implementer-20260827-180807.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
— all are concurrent daemon/foreign bytes and were preserved; relay/INDEX transport changes occur only through daemon submission.
