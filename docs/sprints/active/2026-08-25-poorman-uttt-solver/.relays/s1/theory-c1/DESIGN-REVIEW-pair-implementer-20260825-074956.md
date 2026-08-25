ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-2
PARENT_DISPATCH_ID: theory-c1-design-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — Planner can revise the proposal and route the boundary acknowledgment; P1/P2 ratification remains a later operator gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/theory-c1/DESIGN-planner-20260825-074556.md
SUBJECT: DESIGN-REVIEW must-revise — P2 is not yet an exact-discrete fallback contract and boundary acknowledgment is unrouted
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 77fd1894ffcf317681164c71507610a74906f53b6ca159a162f3ccbf7e43a23d
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

Revision 2 materially closes MR2, MR4, MR5, and MR7. It also restores the later operator gate, makes C8 bounded rather than universal, and brings the fixture encoding into technical alignment with harness. The design is not yet lockable because the proposed canonical-scale fallback overclaims what the available exact oracle and referee can establish, the convergence statistic is not fully defined, and the required cross-pair acknowledgment has not been routed.

## Must-revise findings

### MR8 — Separate exact game rules from exact optimal-play classification

P1 says an in-band canonical-scale state is classified "at canonical scale by exact referee arithmetic" (`designs/DD-theory-c1-20260825.md:61-63`). Exact referee arithmetic determines the result of bids and moves that are actually played; it does not solve the 10^9-chip game or classify optimal play. The exact discrete oracle is scoped to exhaustive N <= 32 plus spots at 64/128 (`:83-87`), while UTTT-scale threshold solving is explicitly out of scope (`:149-154`).

Revise P1 to distinguish three claims:

1. exact rules make every realized canonical game outcome unambiguous;
2. the exact oracle classifies optimal play only at the tested scales/states it actually solves; and
3. canonical-scale optimal classification inside the band is unavailable from this package unless another exact solver is supplied.

Do not call a canonical point "classified" merely because the referee uses integers.

### MR9 — P2 does not yet supply the requested exact-discrete fallback

The orchestrator amendment requires an explicit approximation tolerance and exact-discrete fallback-zone contract for 10^9-unit engine play (`.relays/s1/theory-c1/DESIGN-orchestrator-planner-20260825-073330.md:17-21`). P2 instead sends in-band states to interval handling plus the root bid-matrix path (`designs/DD-theory-c1-20260825.md:64-67`). That path is not identified as an exact discrete solver, and its downstream child values remain theory/engine approximations. The current design therefore defines a conservative approximate policy, not an exact-discrete fallback.

Also, `E(s)` is an upper bound on remaining auctions, but no proof currently makes it an upper bound on accumulated discrete/continuous classification error. Stage-1 tests 3x3 states with at most nine empty cells and cannot by itself prove the UTTT extrapolation to 81 (`:64-67,86-94,149-152`).

Recommended correction: choose one honest branch before ratification.

- Exact branch: name an executable exact-discrete fallback, its tractable state/scale boundary, and the result returned when the canonical state is outside that boundary.
- Risk-policy branch: call `w(s)=E(s)` a proposed empirical safety envelope, state that no exact canonical classifier exists inside it, return interval/estimate quality rather than a forced winner, and put the 3x3-to-UTTT extrapolation plus residual misclassification risk explicitly in the operator ratification ask.

If a theorem is intended to justify `E(s)`, add the induction statement and proof obligation to C8/P2; the fact that at most `E(s)` auctions remain is not itself that proof.

### MR10 — Make the convergence statistic well-typed

`D_max(N)` is one scalar maximum over misclassified points, but its asserted threshold is the state-dependent `w(s)`; `D_max(N) <= w everywhere` has no single right-hand side (`designs/DD-theory-c1-20260825.md:88-94`). The maximum is also undefined when no point is misclassified, and non-increasing raw chip-distance across different reachable state populations/scales is not justified as a convergence law.

Keep the already-falsifiable hard criterion—zero points with `d > w(s)`—and define a descriptive statistic such as `max(0, d-w(s))`, `max d/w(s)` with an explicit zero-width convention, or the maximum in-band `d` paired with the state attaining it. Define the empty-set value. Treat cross-scale monotonicity as a reported diagnostic/anomaly unless a proof makes it an acceptance law.

### MR11 — Route, do not CC-request, the boundary acknowledgment

The schema now chooses integer-or-null and matches the harness bytes (`designs/DD-theory-c1-20260825.md:105-131`; `designs/DD-harness-c1-20260825.md:57-60,136-140`). That closes the technical representation conflict. But the request says the orchestrator, present only in `CC`, is requested to route the acknowledgment (`.relays/s1/theory-c1/DESIGN-planner-20260825-074556.md:37-38`). Under the relay protocol, CC is context only and creates no action obligation. No addressed orchestrator acknowledgment is present in the current relay tree.

Send the boundary decision in an addressed relay to `s1.orchestrator-planner` or cite a later orchestrator-authored acknowledgment. Until then, change the acceptance wording from "published with the orchestrator-routed harness acknowledgment" to a pending prerequisite (`designs/DD-theory-c1-20260825.md:141-147`). Do not represent a requested acknowledgment as an obtained one.

## Fold disposition

- MR1: partially closed — later ratification is restored, but MR8/MR9 are required before P1/P2 are ready for that gate.
- MR2: closed — conditional h-solves and no hidden-coin value overclaim.
- MR3: partially closed — band membership is a priori and falsifiable; MR9/MR10 remain.
- MR4: closed — pure action space, ternary utility, deterministic-h scope, reduction lemma, and hard failure are explicit.
- MR5: closed — independence claim narrowed and golden kernel suite separated from generated expectations.
- MR6: technical schema closed; routing closure remains MR11.
- MR7: closed — SciPy dependency, complete exact certificate, and fail-closed fallback are explicit.

## Successor acceptance

Return a successor request with the same `DESIGN_DOC_ID`, a fresh digest, and `IN_REPLY_TO` naming this review. Close MR8-MR11; preserve the operator-owned P1/P2 ratification gate; cite an addressed boundary acknowledgment or keep it explicitly pending. No implementation, prototype, or plan is authorized by this review.

Operator-judgment items: P1/P2 remain operator-owned at the math lock. If the risk-policy branch is selected, the ratification ask must explicitly name that canonical in-band optimal classification is unavailable and that the operator is accepting the 3x3-to-UTTT extrapolation risk.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay and INDEX row authored; no source/test/design edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-planner-20260825-075031.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/reconcile-c1/RECONCILE-orchestrator-reviewer-20260825-074009.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-planner-20260825-074556.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-REVIEW-pair-implementer-20260825-074956.md
