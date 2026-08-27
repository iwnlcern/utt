ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-7
PARENT_DISPATCH_ID: engine-c4-design-7
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — the operator must ratify or replace the nonterminal zero-total `p := 1/2` estimate policy; F-C5 does not derive it
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-7/DESIGN-planner-20260827-130451.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: e72f0e2f30f064a062cd52b264db8ac28ea8541c1f8d04bc75f1ab0bbd689399
DESIGN_REVIEW_VERDICT: human-decision-required
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW human-decision-required — rev 7 closes M2-M4 but zero-total half-share is a new estimate convention

## Verdict

Human decision required for revision 7 of `DD-engine-math-c4-20260826` at digest `e72f0e2f30f064a062cd52b264db8ac28ea8541c1f8d04bc75f1ab0bbd689399`.

Revision 7 closes review-6 M2, M3, and M4 exactly: extraction is seat-polarized against the X-centric matrix; the provider/matrix/RM+ carrier is finite binary64 end to end with exact ordinals widened losslessly; G5 and its rejected alternative are canonical; and A9 is branch-complete and mutation-sensitive. It also makes M1 mechanically total and finite.

The remaining issue is authority, not arithmetic. The new nonterminal `M2 == 0` rule `p := 1/2` is described as derived from F-C5. F-C5 defines `T = 1/2` only at the all-closed terminal chip comparison and then uses that terminal as a backward-induction base. It does not define a budget share when the denominator is zero at a nonterminal state. Theory's comparison contract likewise explicitly excludes zero-total nonterminal points. Therefore `p := 1/2` may be a reasonable estimate-only convention, but it is a fresh product/search-policy choice outside the recorded G5 formula, whose `p = bx2/M2` presupposes positive total.

Per the incoming relay's explicit fork, this review returns `human-decision-required` rather than silently treating the extension as derived. The standing IMPL STOP remains in force. This review grants no PLAN amendment, source/test edit, dependency action, branch movement, commit, PR, implementation dispatch, implementation, merge, publication, deployment, or release authority.

## Operator decision requested

Choose and record one zero-total policy for nonterminal matrix children:

1. Ratify `p := 1/2` as an estimate-only convention under G5 (or a named successor decision), with terminal authority first, finite `[-1,+1]` validation, estimate taint, the rev-7 A9 coverage, and no claim that it is exact zero-scale game truth; or
2. Direct a different finite zero-total payoff authority, such as a dedicated no-bid/tie-owner evaluation rule, with its carrier, cost bound, provenance, and acceptance tests locked in a successor design.

The operator need not revisit M2-M4, `kappa = 8`, the P2 gate, exact solved-scale A8, or the TT layout. If option 1 is ratified, a narrow successor design should change the rationale from “derived from F-C5” to “operator-ratified estimate convention motivated by symmetric chip equality,” record that decision canonically, and return for delta DESIGN-REVIEW.

## Evidence

- F-C5 is terminal-scoped: `theory/FINDINGS.md` lines 105–121 state the all-closed chip comparison, macro-win terminals, and backward-induction base; no nonterminal zero-denominator share is defined. — E1.
- The exact comparison package deliberately excludes zero-total nonterminal points: `theory/auction_ttt/crosscheck.py` lines 78–89 calls its domain “masked nonterminal positive-total points” and tracks `M == 0` separately as excluded. Its test pins that exclusion at `theory/tests/test_crosscheck.py` lines 39–80. — E1/E2.
- Fresh exact-scale calibration with `theory/.venv/bin/python` ran `continuous.solve()` against `discrete.solve(0)`. Across 9,040 reachable nonterminal `(board,h)` states, interpreting `p = 1/2` by threshold side with the P1b tie-owner equality rule matched 6,376 and disagreed with 2,664 exact zero-scale outcomes; 2,104 exact outcomes were draws. This does not prove the estimate convention is weak, but it proves F-C5 does not make it exact or mechanically derived. — E2.
- G5 at design line 238 ratifies graded `clamp(kappa * (p - t_est_child), -1,+1)` but does not ratify a value for undefined `0/0`; rev 7 introduces that value separately at line 158. — E1.

## Review disposition

- Review-6 M1 totality mechanics: closed conditionally. Terminal-first ordering, finite range validation, no division on the zero branch, both successor tie owners, RM+ finiteness, estimate taint, and deterministic-publication tests are all locked. Authority for the chosen `p` remains operator-owned. — E1.
- Review-6 M2: closed. X maximizes row expected payoff, O minimizes column expected payoff, and only exact expected-payoff equality reaches lower bid/lower move fallback; symmetric mutation fixtures bind polarity. — E1 — design lines 160–163 and A9.
- Review-6 M3: closed. The common carrier and RM+ input are finite binary64 in `[-1,+1]`; exact ordinals remain exact and losslessly widened; A8 observes the exact provider coordinates/ordinal and A9 kills integer quantization. — E1 — design lines 152–156 and A8/A9.
- Review-6 M4: closed. G5 and the rejected out-of-profile policy are in the canonical GRILL_LOCK; scalar `t_est` recurrence, equality, both zugzwang owners, TT midpoint fallback, and no-estimate-pure-hit usability are acceptance-bound. — E1 — design lines 159, 219, and 223–251.
- Retained amendment semantics: pass. `kappa = 8`, saturation at `0.125`, exact P2 authority, A8 discrete provider authority, quality taint, and frozen TT layout survive the narrow delta. — E1.
- Carrier integrity: pass. Incoming `TO`, parent, locator, and digest match the committed artifact at `main@cbb08bb36863dca62677b15a4ebeb50fe8621924`; incoming exact-file lint and `git diff --check` are clean. — E1/E2 instrument evidence.

## Downstream transition

`engine.planner` must route the zero-total decision to the operator. After a decision, return either the operator-authored ruling plus a matching successor design carrier or a successor design that cites the ruling and floats the design digest. No PLAN amendment or IMPL continuation is authorized before an approving DESIGN-REVIEW of the resulting exact bytes.

The daemon is ready with zero pending renders. Its conflict count remains the single historical INDEX divergence already in the projection stream; this seat does not repair or reinterpret that foreign history.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — verdict requests a design ruling only
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory payoff semantics and harness consumers bound the design
- user-visible-control-with-materializer/downstream-consumer: yes — the policy selects published bids and moves
- test-runtime-role-mismatch: no — rev 7 now aligns continuous design values with a continuous runtime carrier
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — nonterminal zero-total payoff semantics require a fresh operator decision
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — this relay is the route requesting the decision; no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Reviewed the complete committed revision-7 design, exact revision-6-to-7 design delta, incoming relay, review-6 M1-M4, theory continuous/discrete/cross-check sources, and preserved implementation seams. — E1.
- Ran the exact scale-zero calibration above, incoming exact-file lint, digest check, `git diff --check`, daemon status, and fresh status checks. — E2 instrument evidence.
- Main was clean at `cbb08bb36863dca62677b15a4ebeb50fe8621924` before daemon admission. The isolated implementation worktree remains at `afb91d529258f1e38e27484007b9d5a090b796af` with the pre-existing untracked Task-14 gauntlet paths; this review did not alter them. — E1/E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-7.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
none — clean main tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
