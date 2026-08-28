ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-13
PARENT_DISPATCH_ID: engine-c4-design-13
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — review-12's proof-carrier and wire-contract blockers are closed under ratified semantics; the next authorized transition is the successor PLAN review cycle and PR #10 remains held
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-13/DESIGN-planner-20260827-164246.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1
DESIGN_REVIEW_VERDICT: approve
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW approve — C8a ceiling/extremizer certificate and analyze rejection ABI are executable

## Verdict

Approve revision 13 of `DD-engine-math-c4-20260826` at digest `cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1` for the successor PLAN phase.

Revision 13 closes both review-12 blockers. The certified action now requires a stable exact integer ceiling over the complete sound critical-fraction enclosure plus a dominance proof for the published extremizing child; every unresolved case falls back to the ordinary matrix action. Analyze now has an ordered, cardinality-preserving success/rejection JSONL ABI with explicit continuation and fitter behavior.

This approval authorizes only the documented successor PLAN transition. It does not authorize source/test/design edits by this seat, implementation, fold-in, branch movement, commit, PR action, merge, publication, deployment, or release. PR #10 remains open and unmerged pending the successor plan, PLAN-REVIEW, and a separate valid implementation dispatch.

## Review-12 dispositions

### M1 — closed: the C8a action carrier is mechanically certified

The crossed enclosure is sound: on the interval-proven ordered branch, `r=(b-a)/(1-a+b)` decreases in `a` and increases in `b`, so `[r_down(a.hi,b.lo), r_up(a.lo,b.hi)]` contains the true critical fraction. Exact bit-decomposition ceiling on both endpoints and equality of the resulting integers pins `ceil(r*M)` for every value in the enclosure by monotonicity. This directly excludes the review-12 `r=1/4, M=4` straddle.

The dominance predicate separately proves action identity. `selected.hi <= other.lo` implies the selected X child attains the true minimum, including equality ties; the symmetric lower/upper comparison proves the O maximum. Interval-proven `a.lo > b.hi` plus dominance on the actual tie-owner side correctly handles the F-C3 zugzwang bid-0 branch. Full-width soundness, cancellation fallback, and matrix fallback prevent an incomplete or ambiguous carrier from publishing a certificate.

The acceptance battery is sufficient and mutation-sensitive: it covers the prior midpoint witness, ceiling straddle, overlapping child intervals, and one positive stable-ceiling/dominant-child publication case. — E1.

### M2 — closed: the analyze rejection path has an executable ABI

Every readable input line yields exactly one ordered stdout JSON object. Success objects retain the existing schema; rejected lines use `error` as an exclusive discriminator and carry the 1-based line number; per-line rejection is silent on stderr, processing continues, and readable-stream completion exits 0. The fitter must reject any error-discriminated response while its depth-6 corpus remains success-only.

The ordered `valid, depth-13, valid` E2 case proves cardinality, ordering, line numbering, continuation, and discriminator separation. The existing adversarial-depth case separately proves the depth-12/5,000,000-node accepted-work bound and `complete:false` result contract. Unreadable-stream process failure remains an implementation-level I/O detail and does not alter the locked readable-line ABI. — E1.

## Preserved passing design

- The revision-12 outward play window remains a sound closed superset of the exact P2 band; the exact integer gate remains sole classification authority. — E1.
- The midpoint `r_root` remains heuristic matrix candidate guidance only and cannot enter the certified branch. — E1.
- Fair-coin ply-0 payoff, staged-best-move publication, seat-aware maximum-probability-first extraction, zero-total alternation policy, cancellation hardening, quantization discriminator, and the full A1-A11 ledger remain governed as previously passed. — E1.
- G1-G6, the theory fixtures/contracts, TT contract, evaluator-quality mapping, and the no-runtime-rational decision remain unchanged. — E1.

## Successor PLAN obligations

The successor PLAN must carry the full revision-13 lock and review-panel must-fix ledger. In particular, its task/test steps must:

1. Spell the primitive-by-primitive directed formulas for `r_down`/`r_up` under section 2, clip their bid-fraction enclosure to `[0,1]` before exact endpoint decomposition, and prove by exact-rational test oracle that it contains the true `r`.
2. Implement ceiling invariance and X/O extremizer dominance as fail-closed predicates; assert in the certified branch that C8a affordability makes the `[0, own stack]` clamp an identity, with any failed assertion/predicate routing to matrix fallback rather than publishing a changed `k`.
3. Carry the four certificate mutation cases and the ordered analyze success/rejection/success process test as named E2 acceptance items.
4. Preserve the complete panel must-fix ledger and rerun A10/A11 as required by the incoming relay.

These are mechanical PLAN obligations derived from the approved design, not new design decisions or implementation authority.

## Boundary contract

Writes: one legal integer bid/move action plus root value/quality metadata; analyze emits one ordered success-or-error JSON object per readable input line.
Reads: sound root aggregate/child intervals, RootContext budgets, theory F-C8a/F-P2, and analyze JSONL requests.
Target entity: `EnginePolicy` published action and `uttt_engine analyze` stdout stream.
Downstream consumer: referee game loop, UI/log metadata, evaluator fitter, and P1b analysis checks.
Contract: certified actions require invariant exact ceiling plus dominant extremizer; all other forced states use matrix/staged fallback; analyze success and error objects are discriminator-separated and cardinality-preserving.
Proof: E2 certificate mutation battery, full-window/P2 equivalence, ordered JSONL process test, bounded analyze case; A10/A11 retain E3 obligations.
No-consumer action: reject — no speculative output surface is introduced.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — deterministic game-search policy, and this review grants no runtime action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory C8a and exposes the analyze JSONL ABI
- user-visible-control-with-materializer/downstream-consumer: yes — the eventual engine publishes live bids/moves and analysis metadata
- test-runtime-role-mismatch: no — certified and fallback carriers now match their proof/quality roles
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — ratified semantics and existing downstream E2/E3 gates remain intact
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Incoming routing, parent, design identity, locator, and digest match committed `main@205ac09`; the exact design digest is `cfbc0e84671a22ecbda0eca989df1f2bc24ee7dc5d3635b44717e2390e1effc1`. — E1.
- Reviewed the complete revision-12-to-13 diff, review-12 findings, F-C8a proof, P2/window math, PR #10 policy/analyze/fitter seams, and preserved A1-A11 ledger. — E1.
- Incoming exact-file lint passes with `--no-freshness`; `git diff --check` is clean on the revision-13 design commit and implementation worktree; the implementation worktree is clean at `5907f937f07fd0309e141e3db706064780822d88`. — E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-pair-implementer-20260827-165143.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
