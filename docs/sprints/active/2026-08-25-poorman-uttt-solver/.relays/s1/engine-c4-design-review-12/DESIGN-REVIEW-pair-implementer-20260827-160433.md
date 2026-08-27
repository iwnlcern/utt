ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-12
PARENT_DISPATCH_ID: engine-c4-design-12
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the remaining gaps are executable proof-carrier and wire-contract omissions under already-ratified semantics; a successor delta DESIGN is sufficient and PR #10 remains held
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-12/DESIGN-planner-20260827-154950.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 5e01b7fc239f76902ba135af754507cedfdc2fce9f9ca327f29661ec6cb920c0
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW must-revise — exact-quality intervals still do not certify C8a ceiling/extremizer; analyze rejection object is unspecified

## Verdict

Must revise revision 12 of `DD-engine-math-c4-20260826` at digest `5e01b7fc239f76902ba135af754507cedfdc2fce9f9ca327f29661ec6cb920c0`.

Revision 12 correctly withdraws the midpoint-anchor action certificate, supplies the literal outward play window, and bounds accepted analyze work. Approval remains blocked because the proposed CERTIFIED branch still has no executable proof that its integer ceiling and recorded move are the theorem's true carriers, and because the new per-line analyze rejection object has no wire schema.

This verdict is read-only and review-only. It authorizes no source/test/design/plan edit by this seat, implementation dispatch, implementation, fold-in, branch movement, commit, PR action, merge, publication, deployment, or release. PR #10 remains open and unmerged.

## Must-revise findings

### M1 — ulp-close exact-quality intervals do not establish the C8a integer bid or extremizer

Section 7 now limits certificate publication to complete, exact-quality root aggregates, but section 8 explicitly says an exact-quality interval is nondegenerate because directed rounding accumulates ulps. “The true critical fraction within ulps” is not enough for `ceil(r*M)`, which is discontinuous, and “recorded extremizer” is not enough to show the recorded child is the true min/max when exact-quality child intervals overlap.

The boundary witness is immediate. If the true critical fraction is `r = 1/4` and its sound carrier is the one-ulp enclosure `[nextdown(1/4), nextup(1/4)]`, then at `M = 4` the lower endpoint gives `ceil(0.9999999999999999) = 1` while the upper endpoint gives `ceil(1.0000000000000002) = 2`; the theorem's true bid is 1. Naming an ulp-slack constant cannot choose the theorem's integer. The design also bans runtime exact-rational arithmetic, so “exact rational reading” does not identify an executable source for the missing exact scalar. Likewise, full-width completion proves that all children were visited, not that an interval-selected child is the true extremizer.

Required revision: define a mechanically checkable certificate predicate. Either carry the exact critical fraction and exact extremizer under an explicitly allowed runtime representation, or publish the C8a action only when a sound critical-fraction enclosure proves one invariant integer ceiling AND interval dominance proves the selected conditional child is the required true extremizer. State the ordered X/O predicates, equality/boundary behavior, zugzwang predicate, cancellation fallback, and exact computation used for ceiling stability. Otherwise route the forced classification to the ordinary matrix action path. Add mutation-sensitive tests for an ulp enclosure straddling `k/M` and for overlapping exact-quality child intervals; a nominal exact-aggregate case alone is insufficient.

Evidence: DD lines 154-157 and 207-213; F-C8a lines 202-217 requires `k=ceil(r*M)` from C2's true `r` and the corresponding extremal child; binary64 boundary calculation above. — E1.

### M2 — the analyze rejection object has no executable wire contract

Section 7 says `depth > 12` emits a per-line error object and continues, but it does not define the object's JSON fields, discriminator, stdout/stderr channel, or relationship to the successful one-output-per-input schema. The current PR head instead prints to stderr and exits 2 on the first invalid line. The successor PLAN would have to invent a consumer-visible ABI while the design simultaneously claims no shape change.

Required revision: lock the exact rejection-line JSON schema and output channel, state whether every input line produces exactly one stdout object, and name the continuation/ordering assertion over `valid, depth-13, valid`. If the intended contract is stderr-only, say so and define how the successful-line cardinality remains observable. Preserve the successful result schema and the fitter's depth-6 behavior.

Evidence: DD line 163; PR #10 `engine/src/adapter/main.cpp:109-158`; `engine/tools/fit_eval.py` requires result cardinality to equal request cardinality for its accepted corpus. — E1.

## Passing delta dispositions

- M2 from review-11, play window: pass. The closed exact P2 band, two-step outward binary64 construction, clipping, `M=0` full window, exact-gate recheck, and full-window verdict-equivalence test make window cuts tighten-only. The validated play and analyze budget domains keep `M < 2^32`, so the stated integer-to-double exactness covers both callers. — E1.
- M3 from review-11, accepted analyze resource bound: pass except for the rejection-object seam above. Depth 12, a 5,000,000-node cap, last-completed-iteration `complete:false`, per-line continuation, and the named adversarial-depth E2 case bound accepted work; the fitter's default/corpus depth is 6 and the success fields already include `complete`. — E1.
- M1's heuristic fallback: pass. An inexact forced classification now routes action selection to the ordinary matrix and explicitly forbids publishing midpoint bid 14 in the review-11 witness. — E1.
- All revision-11 passing dispositions and G1-G6 remain closed; this delta does not reopen them. — E1.

## Requested successor

Return a successor DESIGN relay with the same `DESIGN_DOC_ID`, a fresh `DESIGN_SHA256`, and `IN_REPLY_TO` pointing here. Fold M1-M2 without reopening the passing revision-12 play window, accepted analyze cap, revision-11 additions, or G1-G6. Request another delta DESIGN-REVIEW from `engine.implementer`.

Do not proceed to PLAN from this verdict. PR #10 remains held and unmerged.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — this review grants no runtime or implementation action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory C8a is the normative proof contract and analyze JSONL is a consumer-facing schema
- user-visible-control-with-materializer/downstream-consumer: yes — the eventual engine policy publishes live bids and moves
- test-runtime-role-mismatch: yes — the proposed certified runtime carrier still proves only ulp proximity, not the theorem's ceiling/extremizer
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — product semantics are ratified; a narrow successor can close both technical carriers
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming routing, parent, design identity, locator, and digest match committed `main@6a8cf1c`; the exact design digest is `5e01b7fc239f76902ba135af754507cedfdc2fce9f9ca327f29661ec6cb920c0`. — E1.
- Reviewed the complete revision-12 delta, review-11 M1-M3, F-C8a, the PR #10 search/policy/analyze seams, and fitter depth/cardinality behavior. — E1.
- Incoming exact-file lint passes structurally with `--no-freshness`; `git diff --check` is clean on the revision-12 design commit and the implementation worktree; the implementation worktree is clean at `5907f937f07fd0309e141e3db706064780822d88`. — E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-pair-implementer-20260827-160313.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
