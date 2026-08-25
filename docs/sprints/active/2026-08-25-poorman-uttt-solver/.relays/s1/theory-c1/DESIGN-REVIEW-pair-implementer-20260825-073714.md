ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-1
PARENT_DISPATCH_ID: theory-c1-design-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — Planner revision is next; operator ratification remains a later math-lock gate
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/theory-c1/DESIGN-planner-20260825-072751.md
SUBJECT: DESIGN-REVIEW must-revise — restore knife-edge gate and make root/band/fixture contracts executable
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: d09bdfdbf9c4698795a042eaa4ff186b250b3ec2f8ef988f6d8664db29ecdf6f
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

The overall package shape is right: claim-numbered findings, exact-Fraction continuous DP, an integer-budget oracle, an LP root reference, and an early versioned fixture contract. C9 is correctly positioned as a hard math-lock blocker rather than assumed true, and N <= 32 plus 64/128 spots is an acceptable initial sweep target because the N=24 fallback remains orchestrator-gated.

The current artifact is not lockable. It omits an addressed amendment, overstates the hidden-coin root result, leaves the granularity band non-falsifiable, and does not yet define a verbatim fixture boundary shared with harness.

## Must-revise findings

### MR1 — Restore the amended knife-edge obligations and later human gate

The reconciliation says R4 selects numeric types only and preserves two theory-owned proposals for later operator ratification: exact representable `p = T` classification and a 10^9-unit approximation/fallback contract (`RECONCILE.md:19-20,30`). The addressed successor amendment repeats both obligations and the later math-lock gate (`.relays/s1/theory-c1/DESIGN-orchestrator-planner-20260825-073330.md:17-21`).

The design instead calls the knife edge merely engine-internal, supplies no tolerance/fallback contract, and records no remaining operator-owned decision (`designs/DD-theory-c1-20260825.md:40-42,148-149`). Revise C8, verification, risks, and GRILL_LOCK so they contain both proposals and explicitly preserve operator ratification at the math lock. Do not attribute the equality closure to R4.

Recommended proposal shape: exact discrete play is authoritative at representable equality; the continuous value makes no universal equality-winner claim. Define an a priori integer-distance band and the exact-discrete/fallback behavior before running the cross-check, then make Stage-1 evidence calibrate that contract rather than define the band after observing disagreements.

### MR2 — Make the hidden-coin root contract internally consistent

D2 selected two conditional thresholds plus band analysis and explicitly rejected a probabilistic root solver (`designs/DD-theory-c1-20260825.md:134-137`). C6 nevertheless claims a `1/2` win probability at knife-edge optimal play (`:34-37`), while `discrete.py` carries `h = null`, uses ternary outcomes, and resolves ties by saying `h` wins (`:54-59`). At `h = null`, no player is the tie owner: R1 requires a hidden coin only if the first bid ties (`docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md:28-34`).

Revise the two oracles so ordinary recursion is explicitly over `h in {X,O}`. Treat `T_lo/T_hi` as robust conditional bounds for the root, not an exact solution of the hidden-coin simultaneous game. Inside the envelope, state only what the two conditional games establish; remove the unsupported `1/2` optimal-win-probability claim. Root fixtures should carry both realized coin branches. If an exact hidden-coin value is required, that is the probabilistic root solver D2 rejected and must return to the operator.

### MR3 — Define C8's granularity band before testing it

C8 and `crosscheck.py` currently say only that band occupants are logged and disagreements outside the band must be zero (`designs/DD-theory-c1-20260825.md:40-42,61,103-104`). With no formula for band membership, the check can classify every disagreement as in-band after the fact; finite N <= 32 evidence also cannot establish the unqualified statement that continuous `T` classifies all exact integer-budget play outside an unspecified band.

Specify, before implementation: the exact distance metric in integer units, whether `scale` means initial chips per side or current total chips, the band-width rule at each scale, the convergence statistic and pass threshold, and the 10^9-unit extrapolation/fallback rule. Phrase C8 as the bounded computational claim the sweep can prove unless a separate theorem proves the general statement.

### MR4 — Make C9's determinacy test precise

`maximin = minimax over the full integer bid matrix` is ambiguous between mixed strategies (equality is the generic zero-sum theorem) and pure actions (the load-bearing deterministic claim). The action submitted by each player is `(bid, intended move)`, not bid alone; the bid-only reduction is valid only after proving that the winning move can be optimized independently of the losing bid under Poorman payment.

State that C9 compares pure maximin and pure minimax, define the ordered terminal utility including draw, scope it to deterministic `h in {X,O}`, and add the bid-plus-move reduction lemma or enumerate the full action matrix. A failure must retain the existing hard escalation (`designs/DD-theory-c1-20260825.md:43-44,58-60,119`).

### MR5 — Resolve the two-oracle independence contradiction

The purpose promises two independent exact oracles (`designs/DD-theory-c1-20260825.md:11-13`), but both import one rules implementation. A shared rules bug can self-confirm; line 54 currently says the opposite, while the risk section correctly calls this common-mode (`:54,121`).

Either narrow the claim to independently implemented value algorithms sharing one transition kernel, or separate the transition resolvers. In either case, require a spec-derived golden transition suite that does not generate expected outcomes through `rules.py`, including tie-owner persistence, paid ties, both-zero alternation, terminal-payment ordering, and both root coin branches. Property tests whose expectations come from the same shared implementation are not independent evidence.

### MR6 — Lock one executable fixture schema and reconcile harness

The theory design uses `forced: "any"` (`designs/DD-theory-c1-20260825.md:76-85`); the concurrently reviewed harness contract uses integer-or-`null` and says theory owns the convention (`designs/DD-harness-c1-20260825.md:57-60,136-140`). Thus the current claim of wire-matching conventions is false. The envelope plus category prose also does not yet define the per-category required object shapes that C++, Python, and TypeScript can consume verbatim.

Choose one canonical free-choice encoding and route the boundary acknowledgment through `s1.orchestrator-planner`. Add normative per-fixture fields/types (including stable `id`, state, inputs, expectations, and `consumed_by`), and specify the trace representation for a lazily consulted root coin. Explicitly retain the audit's `a < b`, `a = b` with zero critical bid, and `a > b` cases; define the reported critical bid in the zugzwang branch. Then make schema validation refer to a concrete schema artifact, not only `SCHEMA.md` prose.

### MR7 — Complete the LP certificate contract and dependency list

`lp_reference.py` names SciPy HiGHS but the environment list says only numpy plus pytest (`designs/DD-theory-c1-20260825.md:51-52,62`). Re-solving equalizer equations on the support is not by itself an optimality certificate: the exact check must also verify normalization, non-negativity, on-support equalities, every off-support primal/dual inequality, and the resulting exploitability/value bounds for both players.

Add SciPy to the pinned project dependencies and spell out the complete exact certificate, including failure behavior when the double LP returns a degenerate or numerically unstable support.

## Findings placement check

- F1/F2/F3: recurrence, partisan/zugzwang proof obligations, and chip-terminal boundary are present, but C5 must rely on the local one-step lemmas plus DAG induction; finite convergence evidence alone is not a proof of the whole threshold theory (`designs/DD-theory-c1-20260825.md:20-33`).
- F4: not yet discharged; MR1 and MR3 are required.
- F5: correctly placed in C4 and fixtures, subject to the independent golden traces in MR5.
- F6: correctly split between theory's LP reference and engine's averaged-strategy/exploitability contract, subject to MR7.
- F7/F8: prior-art positioning and Python toolchain are represented; SciPy remains missing.

## Acceptance for the successor

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a new digest, and an acknowledged locator. The revised design must close MR1-MR7, preserve the later operator ratification explicitly, and cite the orchestrator-routed fixture-convention acknowledgment. No implementation or prototype is authorized by this review.

Operator-judgment items: exact representable `p = T` classification and the 10^9-unit approximation/fallback contract remain operator-ratified at the math lock; an exact probabilistic hidden-coin root value would be an additional decision only if reintroduced.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay and INDEX row authored; no source/test edits; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/DESIGN-REVIEW-pair-implementer-20260825-073619.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/DESIGN-REVIEW-pair-implementer-20260825-073714.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-rules-c1-20260825.md
