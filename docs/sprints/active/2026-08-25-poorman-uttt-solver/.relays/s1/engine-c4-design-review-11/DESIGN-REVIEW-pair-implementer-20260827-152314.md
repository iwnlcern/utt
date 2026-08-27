ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-11
PARENT_DISPATCH_ID: engine-c4-design-11
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the three blockers are technical carrier/totality gaps with ratified semantics already available; a successor delta DESIGN is required before PLAN
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-11/DESIGN-planner-20260827-150953.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 455ad202a1fab53a29a16e6ecd3910cfe16f5e9dc940ea05020508aa70e17cb1
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW must-revise — C8a needs a certified action carrier; play window and analyze bound remain under-specified

## Verdict

Must revise revision 11 of `DD-engine-math-c4-20260826` at digest `455ad202a1fab53a29a16e6ecd3910cfe16f5e9dc940ea05020508aa70e17cb1`.

The fair-coin ply-0 payoff, staged-best-move publication, maximum-probability-first extraction tests, production-policy zero-total seam, cancellation hardening, quantization discriminator, and the panel's implementation-conformance findings are directionally correct. Approval is blocked by one soundness defect in the new forced-action rule and two missing executable contracts.

This verdict is read-only and review-only. It authorizes no source/test/design/plan edit by this seat, implementation dispatch, implementation, fold-in, branch movement, commit, PR action, merge, publication, deployment, or release. PR #10 remains open and unmerged.

## Must-revise findings

### M1 — C8a does not prove the bid computed from midpoint heuristic anchors

Revision 11 says a forced branch publishes `ceil(r*M)` from the root anchors and cites C8a as the proof. But the same design defines production `r_root` from the midpoints of interval aggregates and explicitly calls that anchor heuristic. C8a proves `ceil(r*M)` for the true backup critical fraction and its matching extremizing child, not for an arbitrary midpoint fraction.

A compact counterexample fits the design's own carriers. Let the sound aggregates be `a=[0,0]`, `b=[0,0.3]`, with the true extremal values `a=0`, `b=0.3`. Then true `T=r=3/13`, while the midpoint anchor is `r_root=3/23`. At `M=100`, `E=1`, `bx=25`, the strict X gate fires because `24 > (3/13)*100`. Revision 11 publishes `ceil((3/23)*100)=14`. O can bid 15, leaving `M'=85`; on the true `b=0.3` child, X's margin is `25-0.3*85=-0.5`, so the claimed certificate is gone. C8a's proven bid would be `ceil((3/13)*100)=24`.

Required revision: separate the heuristic matrix anchor from the forced-branch certificate carrier. Lock a sound way to obtain both the C8a bid and its matching conditional move from incomplete interval search, or narrow forced publication to states where the exact critical fraction and extremizer are established. State the X and O rules, ordered and zugzwang branches, rounding, clamping/affordability, cancellation behavior, and quality precondition. Add a mutation-sensitive certificate-preservation test that includes an inexact aggregate interval like the witness above; merely checking the integer ceiling of an injected point `r` is insufficient.

Evidence: DD lines 134-143, 154, 160, 176, and 184-185; `theory/FINDINGS.md` C8a lines 193-218; PR #10 `production_anchors` computes the midpoint rule at `engine/src/root/bid_matrix.hpp:58-64`. — E1.

### M2 — The now-mandatory play window still has no exact sound construction

Sections 4 and 7 require production play to pass “the P2 decision band around `bx/M`”, but no endpoints, strict-boundary treatment, zero-total branch, or binary64 rounding direction are defined. This window is not diagnostic: it enables bound returns and child cuts. An inward-rounded or off-by-one band can prune a value that P2 still requires the exact gate to treat as in-band.

Required revision: write the literal root `Window` construction from integer `(bx, M, E)` for both sides, including clipping to `[0,1]`, `M=0`, strict P2 boundaries, and outward rounding (or another proved enclosure). State how the returned bound remains sufficient for the later exact P2 classifier. A9 must assert the exact window bytes/containment on lower boundary, upper boundary, clipped endpoints, and `M=0`, then prove cuts are non-vacuously enabled through `EnginePolicy` without changing the P2 verdict relative to full-window search.

Evidence: DD lines 57-65 and 156; P2 lines 345-361; PR #10 calls root search with its default full window at `engine/src/adapter/policy.hpp:101-104`. — E1.

### M3 — The required analyze-mode resource bound is absent from the design and acceptance ledger

The incoming must-fix ledger correctly identifies that `uttt_engine analyze` accepts depth through 255 while passing an unlimited node cap and no stop callback. Revision 11 does not add an analyze contract or an acceptance row, so the successor PLAN would have to invent the bound and observable incomplete/rejection behavior.

Required revision: lock the analyze resource policy: the bound source and value/domain, whether over-budget work returns a schema-valid `complete:false` result or rejects the request, and per-line continuation behavior after a bounded/incomplete request. Add a named E2 acceptance case at an adversarial depth proving bounded termination and the chosen output contract. Preserve the existing fitter/P1b consumer shape or explicitly disposition any compatibility change.

Evidence: incoming ledger item 10; PR #10 `engine/src/adapter/main.cpp:109-158`, especially unlimited nodes/no stop at lines 140-143; existing plan rev 5 lines 153-161 defines analyze as a JSONL consumer surface. — E1.

## Passing delta dispositions

- Ply-0 coin expectation: pass. R1 makes the hidden first-tie outcome 50/50 from the engine's information set; averaging the two conditional X-centric continuation values is the correct matrix entry and preserves estimate taint unless both branches are exact. — E1.
- Staged publication: pass. Publishing the staged conditional best move closes the `legal.front()` defect and matches iterative-deepening staging. — E1.
- Averaged-profile extraction tests: pass. Non-uniform profiles now discriminate the maximum-probability primary filter before seat-aware payoff tie-breaking, with deterministic lower-bid/lower-move fallback. — E1.
- Policy-seam and cancellation tests: pass. The zero-total solver is exercised through the real matrix path, persistent threshold-TT state is snapshotted at the owning seam, and mid-loop cancellation forbids partial parent memoization/publication. — E1.
- Panel items 9 and 11: no new design ruling required. The existing reserved-matrix slice/incomplete-child rule and sound interval-intersection/nesting invariants already decide them; the successor PLAN must carry direct regression tests. — E1.
- Boundary contract: conditional root search and theory supply mathematical carriers; `EnginePolicy` publishes one legal integer bid/move; RM+ consumes only complete finite matrix entries. M1 must close the forced-action writer/proof mismatch before that boundary is approvable. — E1.

## Requested successor

Return a successor DESIGN relay with the same `DESIGN_DOC_ID`, a fresh `DESIGN_SHA256`, and `IN_REPLY_TO` pointing here. Fold M1-M3 without reopening G1-G6 or the passing amendment-2 additions. The response should identify the exact revision-11-to-successor delta and request another delta DESIGN-REVIEW from `engine.implementer`.

Do not proceed to PLAN from this verdict. PR #10 remains held and unmerged.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — this review grants no runtime or implementation action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory C8a and harness R1 are normative producer contracts consumed by engine
- user-visible-control-with-materializer/downstream-consumer: yes — the eventual policy publishes live bids and moves
- test-runtime-role-mismatch: yes — M1 currently substitutes a heuristic matrix anchor for a theorem-backed certificate carrier
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — product semantics are ratified; technical successor design can close the gaps
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming address, parent, design identity, locator, and digest match committed `main@ca85049bfdeee995bd507c8d1ce60c0e2230085e`; the exact design digest is `455ad202a1fab53a29a16e6ecd3910cfe16f5e9dc940ea05020508aa70e17cb1`. — E1.
- Reviewed the complete revision-11 design, exact revision-10-to-11 diff, R1 source, F-C8a/P2 proof, PR #10 head `5907f937f07fd0309e141e3db706064780822d88`, and the production policy/search/analyze seams. — E1.
- Incoming exact-file lint passes structurally with `--no-freshness`; ordinary lint now reports only the expected historical timestamp drift. `git diff --check` is clean on main and the implementation worktree; the implementation worktree is clean. — E2 instrument evidence.
- Daemon pre-submit state: ready, epoch active, zero pending renders, one inherited historical INDEX divergence. This seat does not repair or reinterpret foreign projection history.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-pair-implementer-20260827-152149.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
