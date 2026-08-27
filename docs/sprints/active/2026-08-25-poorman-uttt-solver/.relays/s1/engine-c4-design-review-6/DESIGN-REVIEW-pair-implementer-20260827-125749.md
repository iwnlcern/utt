ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-6
PARENT_DISPATCH_ID: engine-c4-design-6
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the design author can close the finite-domain, seat-polarity, carrier, and acceptance seams without expanding implementation authority; any non-derived zero-total policy choice must instead be escalated
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-6/DESIGN-planner-20260827-124648.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: f73b43e57ae8e5d8bda2c1d625736516e2548013d5158965bfc89d582e3f1d5c
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW must-revise — rev 6 graded payoff has a zero-total hole and reverses O extraction polarity

## Verdict

Must revise revision 6 of `DD-engine-math-c4-20260826` at digest `f73b43e57ae8e5d8bda2c1d625736516e2548013d5158965bfc89d582e3f1d5c`.

The graded-payoff direction is a coherent response to the accepted A10 blocker, and the default `kappa = 8` arithmetic is internally consistent: `1 / kappa = 0.125`. The retained TT rule is also implementable without estimator starvation if the existing no-estimate-hit usability rule remains unchanged: exact/bound hits use the required sound midpoint, while estimate-quality entries are not reusable as pure hits and therefore live matrix-child searches still reach evaluator leaves.

Approval is nevertheless blocked by four executable defects in the amendment carrier. First, `p = bx2 / M2` is undefined at the reachable post-payment coordinate `(bx2, bo2) = (0, 0)`. Second, the extraction tie-break says to prefer higher X-centric matrix payoff for both seats, but the O/column player minimizes that payoff. Third, the amendment introduces continuous values without locking the provider/matrix/RM+ carrier that presently accepts only integer ordinals. Fourth, the design artifact itself does not record the relay-claimed G5 decision in its GRILL_LOCK.

This verdict covers the exact revision-6 bytes only. It grants no source/test edit, dependency action, branch movement, commit, PR, implementation dispatch, implementation, merge, publication, deployment, or release authority. The standing IMPL STOP remains in force.

## Must-revise findings

### M1 — The graded formula is not total at a reachable zero-budget coordinate

Section 7 defines `p = bx2 / M2`, where `M2 = bx2 + bo2`, but supplies no `M2 == 0` branch. `(0,0)` is reachable and is requested by the shared matrix path: if one side has the entire remaining stack, bids that stack, and wins, payment leaves both sides at zero; a root already at zero total is also admitted by the existing budget/P2 domain. Plain-double `0.0 / 0.0` yields NaN, and clamping does not turn NaN into a finite payoff. This can poison RM+ regrets, strategies, exploitability, and extraction.

Required correction: make the production payoff total on every legal post-payment coordinate. Lock a finite, game-semantic `M2 == 0` rule derived from the zero-scale/tie-owner game (or explicitly ratified as estimate policy), preserve terminal authority before that branch, and require finite `[-1,+1]` validation. A9 must include nonterminal `(0,0)` children for both successor tie owners and prove finite payoff, estimate taint, finite RM+ outputs, and deterministic action publication. If G5 does not already determine the zero-total policy, route the choice to the operator rather than silently selecting one in PLAN/IMPL.

Evidence: design section 7 line 156; existing production matrix calls the child-payoff provider at exact post-payment coordinates (`engine/src/root/bid_matrix.hpp` at branch `afb91d5`, lines 191–205); the existing P2 domain permits `total == 0` (`engine/src/root/p2_gate.hpp`, lines 67–108); theory's discrete solver explicitly admits nonnegative scale including zero (`theory/auction_ttt/discrete.py`, lines 30–32, 218–226). — E1.

### M2 — O extraction must minimize the X-centric payoff

The matrix payoff is X-centric: `+1` is an X win and `-1` is an O win. RM+ therefore treats the row/X player as the maximizer and the column/O player as the minimizer. The amendment's universal rule, “prefer the higher expected payoff against the opponent's averaged profile,” is correct for X rows but reversed for O columns. On equal maximum strategy probabilities it directs O toward the action with the better outcome for X.

Required correction: lock the seat-aware formulas and polarity. For X row `i`, compare `sum_j A[i][j] * y[j]` and maximize. For O column `j`, compare `sum_i x[i] * A[i][j]` and minimize. Only after equal expected payoff apply lower bid, then lower move index. Define equality on the actual finite binary64 values used by the solver. A9 must carry symmetric mutation-sensitive fixtures where equal maximum probabilities have unequal expected payoffs, proving X chooses the larger value and O chooses the smaller value, plus exact expected-payoff ties proving the bid/move fallback.

Evidence: design section 7 lines 152 and 158; existing RM+ row regret adds `row_value - expected`, column regret adds `expected - column_value`, and exploitability uses `max(row_values) - min(column_values)` (`engine/src/root/rmplus.hpp` at `afb91d5`, lines 76–113). — E1.

### M3 — Lock the continuous payoff carrier end to end

Revision 6 declares a continuous in-band payoff, but its surrounding normative sentence still maps entries to `{-1,0,+1}`, and it does not state the shared provider/matrix/RM+ value type or finite-domain invariant. The preserved implementation seam is presently integer end to end: `PayoffResult::ordinal` is `int`, `RootMatrix::payoffs` is `vector<vector<int>>`, construction rejects values outside the three ordinals, and `solve_rmplus` accepts `vector<vector<int>>`. A PLAN could otherwise satisfy the literal preserved interface only by truncating/rounding the graded value, recreating the information collapse.

Required correction: state that the common production matrix carrier and RM+ input are finite binary64 values in `[-1,+1]` (or an equivalently explicit continuous carrier), while the exact provider remains restricted to exactly representable `-1`, `0`, `+1`. Reconcile the opening “mapped to” sentence with the split exact/estimate domains. Preserve A8 by comparing the exact provider's ordinal/coordinates before or alongside lossless widening to the common carrier; add a mutation that quantizes an in-band value to an integer and makes A9 red.

Evidence: design section 7 lines 152–159 and A8/A9 lines 213–214; existing `engine/src/root/bid_matrix.hpp` at `afb91d5`, lines 27–49 and 181–207; existing `engine/src/root/rmplus.hpp`, lines 48–50. — E1.

### M4 — Record G5 in the canonical GRILL_LOCK and make A9 branch-complete

The incoming relay says the operator-ratified amendment is “recorded here as G5,” but the canonical design artifact's GRILL_LOCK still lists only G1–G4 and Design shape, and its rejected alternatives do not record the rejected out-of-profile bid policy. The relay is a carrier for review authority; the design digest is the spec of record. The decision must survive in that artifact.

Required correction: add G5, its source/rationale, and the rejected out-of-profile policy to section 12. Expand A9 beyond one generic ancestor-shift witness so scalar propagation is mutation-sensitive at the locked point recurrence: X-child minimum, O-child maximum, ordered `F`, zugzwang for both tie owners, equality `a_est == b_est`, estimate taint, and TT midpoint fallback/no-estimate-hit interaction. This does not require a new scalar formula: section 3 already defines it; it requires the amendment to prove that the implemented `t_est` follows it at every named seam.

Evidence: incoming relay G5 claim; design section 3 lines 34–51, section 7 line 157, A9 line 214, and GRILL_LOCK lines 218–244. — E1.

## Review disposition

- Amendment direction and `kappa`: pass. The sign is X-centric and the default saturation threshold is arithmetically correct. Final tuning remains PLAN-committed and A10-governed. — E1.
- Scalar point backup semantics: pass with M4 acceptance correction. “Scalar F/zugzwang point arithmetic” is resolved by section 3's min/max and branch table; the missing piece is branch-complete mutation evidence, not a different formula. — E1.
- TT estimator availability: pass under the unchanged TT usability rule. Estimate-quality pure hits remain unusable; exact/bound hits intentionally use midpoint. The successor PLAN must preserve this invariant and name the test. — E1 — existing search usability at `engine/src/search/search.hpp` on `afb91d5`, lines 463–468.
- Graded payoff domain: must-revise M1 and M3. The formula is neither total at `M2 == 0` nor carried continuously by the preserved solver interface. — E1.
- Extraction determinism and game polarity: must-revise M2. Lower bid/move is deterministic only after the seat-correct expected-payoff comparison is defined. — E1.
- Decision carrier: must-revise M4. G5 exists in the incoming relay but not in the canonical design GRILL_LOCK. — E1.
- Carrier integrity: pass. Incoming `TO`, parent, canonical locator, and declared digest match the committed artifact at `main@1058d22311747ea912a1015020c0f782f25389fd`; the exact revision-5-to-6 diff is narrow and `git diff --check` is clean. — E1/E2 instrument evidence; semantic claims remain E1.

## Downstream transition

`engine.planner` must amend `DD-engine-math-c4-20260826` and return a successor DESIGN relay with a new digest, parented to this review. No PLAN amendment or IMPL continuation is authorized before a successor DESIGN-REVIEW approves those exact bytes.

The daemon is ready with zero pending renders. Its conflict count is the single historical INDEX divergence already present in the projection stream; this seat does not repair or reinterpret that foreign history.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — must-revise enables only a successor design
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory payoff semantics and harness metadata remain downstream contracts
- user-visible-control-with-materializer/downstream-consumer: yes — published bid/move and analysis metadata are externally consumed
- test-runtime-role-mismatch: yes — the amendment requires continuous runtime payoffs while the preserved runtime carrier is integer
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — zero-total payoff semantics are unspecified and seat polarity is wrong
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Reviewed the complete committed revision-6 design, exact revision-5-to-6 delta, incoming relay, Task-14 blocker, preserved implementation seams, theory zero-scale domain, and prior approved DESIGN-REVIEW. — E1.
- Incoming exact-file lint is clean with freshness disabled; the design digest is exact; `git diff --check` is clean. Root history retains one foreign INDEX divergence, with zero pending daemon renders. — E1/E2 instrument evidence.
- Main is clean at `1058d22311747ea912a1015020c0f782f25389fd`. The isolated implementation worktree remains at `afb91d529258f1e38e27484007b9d5a090b796af` with the pre-existing untracked Task-14 gauntlet paths; this review did not alter them. — E1/E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-6.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
none — clean main tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
