ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-4
PARENT_DISPATCH_ID: engine-c4-design-4
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the residuals are technical provider-contract corrections within the retained operator choices
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-4/DESIGN-planner-20260826-190041.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: ca2a8f1d96a05dde2294a3d5060bc274c1e823be38d3684eda7585d07eab04c5
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — rev 4 closes TT residuals but binds the wrong exact matrix provider

## Verdict

Revision 4 closes review-3 M1 and M2. Full-key store identity now consults the snapshot at each way before merge/protection; A6 can construct and mutation-test two colliding entries. The entry and bucket alignments are literal, `smp_word` is honestly reserved, one-half coherence schemes are rejected, and the future publication discipline is correctly left to an amendment. Those closures may be retained.

Revision 4 also correctly rejects the false degenerate-interval premise and introduces a shared payoff-provider seam. Approval is still blocked by two connected exactness gaps: the test provider named for A8 is the continuous exact-rational threshold evaluator, while theory's matrix payoff source and P1a authority are the separate budget-indexed DISCRETE oracle; and A8's action labels depend on exact continuous `r`/preferred-move metadata that the new payoff-only provider seam does not bind. The production `r_root` scalar is likewise not derived from the root's intervals anywhere in the lock.

No operator product choice is needed. This is a provider-shape correction that preserves P1a/P2, the accepted production estimate fallback, and the already-chosen Cartesian action space.

## Target and integrity

- Reviewed revision 4 at `main@844adeb`; its on-disk SHA-256 is `ca2a8f1d96a05dde2294a3d5060bc274c1e823be38d3684eda7585d07eab04c5`. — E1 — `git log`, `sha256sum`, full-file and revision-3-to-4 diff reads.
- The incoming relay is addressed to `engine.implementer`, parents to `engine-c4-design-review-3`, carries the canonical locator/digest pair, and passes exact-file relay lint. — E1.
- The embedded grill and G1–G4 are unchanged. No grill decision is reopened. — E1 — design section 12 and exact delta.
- Review-only: no source, test, design-doc, branch, commit, PR, merge, publication, or deployment action was authorized or performed. — E1 — phase/authority and final status.

## Fold disposition of review-3 residuals

- Prior M1: closed. Store-side full-key identity is per way, collision-aware, and mutation-guarded before same-entry merge/protection.
- Prior M2: closed. The 32-byte entry is `alignas(16)`, the 128-byte bucket is `alignas(64)`, and the reserved SMP word makes no premature checksum-sufficiency claim.
- Prior M3: partly closed. The production midpoint path is explicitly estimate-tainted and the matrix consumes a provider through shared winner/payment/transition code. Residuals M1–M2 below show that the chosen test provider does not yet supply the exact discrete payoffs or exact action anchors A8 requires.

## Must-revise residuals

### M1 — An exact continuous threshold evaluator is not the solved-scale discrete payoff oracle

Section 7 lines 153–156 binds the ttt3 acceptance path to “the same exact evaluator A3 already requires,” described as implementing F/zugzwang/terminal threshold semantics in rational arithmetic, and concludes that every matrix entry is therefore an exact discrete outcome. A3 line 200 confirms that evaluator's contract: it returns the exact-rational CONTINUOUS threshold contained by the engine interval. Theory deliberately keeps that object separate from the discrete oracle. `fixtures_gen.py` calls `solve_continuous()` and `solve_discrete(scale)` independently; `lp_reference.py::_action_matrix` reads `values[(child,h)][bx,bo]` from the budget-indexed discrete table, not a rational threshold; and P1a names the exact DISCRETE oracle as the solved-scale authority. P2/C8 expressly allow continuous/discrete disagreement inside the band and retain the absence of a canonical in-band exact solver. Exact arithmetic removes floating ambiguity but does not convert the continuous threshold classification into the discrete outcome. — E1 — design lines 153–156/200/206; `theory/auction_ttt/fixtures_gen.py` lines 110–111; `theory/auction_ttt/lp_reference.py` lines 417–441/489–523; `theory/FINDINGS.md` P1a/P2 lines 315–318/345–361.

The landed fixtures exhibit the separation directly: `threshold-a-equals-b-zero-critical` carries continuous `T=1/2` and a discrete DRAW at equal budgets, while the two conditional root fixtures also carry continuous `T=1/2` at equal budgets but resolve to different discrete winners by `h`. A threshold-plus-generic equality rule therefore cannot stand in for the discrete table. — E1 — `theory/fixtures/thresholds_ttt3.json` lines 9–52/113–209.

Required revision: split the solved-scale test bindings exactly along the theory boundary. Use the exact continuous solver/node only for continuous metadata, and inject or independently reproduce the budget-indexed discrete value table for each payoff lookup `(child, successor_h, post_payment_bx, post_payment_bo)`. A8 must prove that the shared engine winner/payment/transition path requests those exact coordinates and that every emitted payoff byte equals theory. Do not describe the continuous rational evaluator as P1a authority.

### M2 — A8's action anchors and production `r_root` remain unbound

The theory action labels depend not only on payoff values but on the exact continuous node's `r`, `best_x`, and `best_o`: `_candidate_bids` consumes `node.r`, and `_candidate_moves` consumes the two preferred moves. Revision 4's new seam is explicitly a CHILD-PAYOFF value provider. It does not say that the ttt3 A8 path sources those three action anchors from the exact continuous node, nor does it prove that outward-rounded interval search selects identical anchors. Consequently exact payoff injection alone cannot make the action-label equality in A8 executable. — E1 — design lines 147–156/206; `theory/auction_ttt/continuous.py` lines 47–74; `theory/auction_ttt/lp_reference.py` lines 390–427.

The production side has the symmetric executable gap: section 7 defines `k* = round(r_root*M)` but the root inputs contain only TInterval(s) and budgets, the TT stores no `r`, and no rule derives one scalar `r_root` from interval-valued `a,b` or defines the rounding when that derivation is approximate. Different local choices change the candidate action set, which line 157 says is not a local engine choice. — E1 — design lines 127–159 and TT layout lines 84–117.

Required revision: make the action-anchor provider explicit. For solved-scale A8, source `r`, `best_x`, and `best_o` from the exact continuous node (or prove exact equality from the interval path) before comparing labels. For production, lock a deterministic scalar derivation from the available interval/root-search state, including the exact half-up integer rounding semantics used by `_round_fraction`; if midpoint/estimate metadata enters, state its quality consequence. Keep the existing fixed anchors, deduplication, ordering, and Cartesian construction unchanged.

## Retained revision-4 closures

- Preserve collision-aware per-way store/probe identity, snapshot replacement binding, occupied/sentinel semantics, 32-byte offsets, `alignas(16)` entries, and `alignas(64)` buckets.
- Preserve the honestly reserved `smp_word`, the rejection of one-half coherence checks, and the future-amendment boundary for publication discipline.
- Preserve the shared action/winner/payment/transition matrix code, the production TInterval provider, exact-gate outcomes, estimate-tainted midpoint fallback, and Cartesian `(bid,move)` actions.
- Preserve all directed interval/P2/A1–A7/A9–A11 closures, G1–G4, owner-honest fixture boundaries, and canonical carrier fields.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — engine design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation/runtime authority
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory matrix/fixture and harness metadata contracts are consumed
- user-visible-control-with-materializer/downstream-consumer: yes — analysis metadata has harness/UI consumers
- test-runtime-role-mismatch: yes — A8 binds a continuous evaluator where the runtime-shaped matrix lookup requires discrete budget-indexed values
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — solved-scale payoff authority and production action-anchor derivation remain lock blockers
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Successor acceptance

Return revision 5 under the same `DESIGN_DOC_ID`, parented to this review, with a fresh canonical locator/digest carrier. Close M1–M2 while preserving every retained revision-4 closure. Approval then depends on separate exact-continuous metadata and budget-indexed discrete-payoff providers for A8, plus a deterministic production `r_root`/rounding rule; both paths must continue through the identical engine action/winner/payment/transition code.

ACTIONS_GIT_REF:
- relay draft=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-4.md`
- source/tests/design=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
