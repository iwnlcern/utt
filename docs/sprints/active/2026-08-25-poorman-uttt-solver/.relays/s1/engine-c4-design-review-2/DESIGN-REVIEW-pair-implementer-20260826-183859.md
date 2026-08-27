ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-2
PARENT_DISPATCH_ID: engine-c4-design-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the residuals are technical contract corrections within the retained operator choices
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-2/DESIGN-planner-20260826-182459.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: a0cbc3cbba751d1dcff6f7efa71db561a3be87f4d7f86119d9094d9b527cdb20
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — rev 2 closes most findings but matrix, TT, interval-totality, P2-test, and carrier residuals remain

## Verdict

Revision 2 substantively closes the root bid-anchor set, owner-honest backup-fixture boundary, per-cut non-vacuity proof, and the unsafe whole-expression/P2 float-product choices. Those closures may be retained. Approval is still blocked by five narrow contradictions: the engine and theory root action spaces remain different despite the new parity claim; the full-key TT sidecar cannot identify entries under the collision A6 requires and its empty/layout bytes contradict each other; the preimages are not total at point endpoint windows and their primitive direction sequences remain undefined; A7 asks to mutate ceil/floor operations that the revised P2 algorithm removed; and the successor DESIGN carrier again omits the canonical locator/digest fields.

No operator product choice is needed. These are exact technical and carrier repairs within F-C1–C10, P2, G1/G2, and the already-approved canonical action set.

## Target and integrity

- Reviewed revision 2 at `main@301b147fdb76382b760093e9e6ddfff05341dbe4`; its on-disk SHA-256 is `a0cbc3cbba751d1dcff6f7efa71db561a3be87f4d7f86119d9094d9b527cdb20`. — E1 — `git log`, `sha256sum`, full-file and revision-1-to-2 diff reads.
- The incoming relay is addressed to `engine.implementer`, parents to `engine-c4-design-review-1`, carries the same `DESIGN_DOC_ID`, and passes exact-file relay lint. — E1.
- The embedded grill and G1–G4 are unchanged. No grill decision is reopened. — E1 — design section 12 and exact delta.
- Review-only: no source, test, design-doc, branch, commit, PR, merge, publication, or deployment action was authorized or performed. — E1 — phase/authority and final status.

## Fold disposition

- Prior M1: partly closed. Primitive-directed interval arithmetic replaces whole-expression stepping and preserves the cancellation witness; residual M3 below blocks total/executable semantics.
- Prior M2: arithmetic direction closed by exact bit-pattern comparison; residual M4 is acceptance/mechanical completeness only.
- Prior M3: bid anchors closed; residual M1 shows the move dimension still prevents theory parity.
- Prior M4: sentinel/layout/sidecar concepts added; residual M2 shows internal contradictions and collision-indexing failure.
- Prior M5: closed. A1 preserves the theory-only tag and labels copied constants non-contract; A4 now has per-cut activation evidence and appropriate relational oracles.

## Must-revise residuals

### M1 — Restored bid anchors do not make the engine and theory matrices the same game

Design lines 133–137 still define a bid-only engine matrix whose winner uses one `move_x`/`move_o` from the budget-independent threshold TT. The landed theory reference constructs each row/column action as a Cartesian product `{bid, move}` and evaluates the selected move at the post-payment discrete budgets (`lp_reference.py` lines 417–441). F-C9a permits bid-only reduction only after replacing the winner's move by the extremizer for the RESULTING BUDGET STATE; it does not prove that the continuous TT's single conditional move is that discrete extremizer inside P2's band. A8's exact action-label equality is therefore impossible as written: one side labels bids, the other labels `(bid,move)` pairs. — E1 — design lines 133–137/184; `theory/FINDINGS.md` C9a lines 239–247; `lp_reference.py` lines 403–441.

Required revision: choose one mechanically identical comparison surface. Either (a) give the engine root matrix the same `(bid,move)` candidate actions as theory, or (b) define an executable per-bid, post-payment discrete extremization seam and make theory emit that same reduced matrix. A single budget-independent TT move is not (b). A8 must compare identical action labels and payoff bytes after this choice.

### M2 — The full-key sidecar cannot identify the entry whose value it verifies, and the frozen bytes contradict initialization

The sidecar is keyed only by the 64-bit Zobrist value (lines 99–100), while A6 deliberately requires two semantic positions colliding in that key and the 32-bit tag. A key-only map can retain only one identity for both bucket ways; it cannot bind a snapshot to the particular entry being probed. Depending on store/probe order, this creates either false misses or lets one way be checked against another way's identity. Full-key verification must be per stored entry, not per already-colliding key. — E1/E2 — design lines 96–100/182; key-overwrite map witness.

The byte contract is also inconsistent: line 89 says one spare byte, line 91 assigns `spare@29–31` plus an unnamed XOR-complement at bytes 4–7, and line 94 says all entries are zero-initialized while lines 92/94 define empty entries by `move_x=move_o=0xFF`. A zero-initialized entry instead carries legal move zero and fails its own empty-state definition. The future complement/checksum field is not an explicit field even though offsets are declared frozen. — E1 — design lines 81–101.

Required revision: attach the semantic snapshot to bucket/way (or another immutable entry identity) and update/clear it atomically with store/replacement in full-key mode. Define every byte as an explicit field/padding range, reconcile one versus three spare bytes, give the future publication word one unambiguous derivation, and specify a default constructor/empty flag whose actual bytes contain the `0xFF` sentinels. A6 must inject two colliding ways and prove each probe checks its own snapshot.

### M3 — Directed composites and endpoint guards are still not a total algorithm

Lines 27–28 name directed primitives, but `F_down`, `F_up`, `a_down/up`, and `b_down/up` never lock the direction of each intermediate. For example, a lower quotient requires an upper-rounded denominator; “primitive-by-primitive” alone does not select that sequence. The inverse guards also cover only `t.lo=0` for the X lower endpoint and `t.hi=1` for the O upper endpoint. Point windows `t=[0,0]` and `t=[1,1]` still send the other endpoint through division by zero, and the feasible-versus-empty cases depend on the paired child interval and ordered constraint. A5 names `t.lo=0`/`t.hi=1` but not these point-window totality cases. — E1 — design lines 27–29, 46, 66–74, 181.

Required revision: write the literal directed-operation sequence for all four composite endpoints, including which denominator enclosure each division consumes. Add a total branch table for `t=[0,0]`, `t=[1,1]`, and intervals merely touching 0/1, distinguishing conservative full/point/empty preimages before intersection. Extend A5 with both point windows and mutations of an intermediate rounding direction.

### M4 — A7 retains mutation targets absent from the revised P2 implementation

The new gate compares the exact binary rational directly by mantissa/exponent shifts and contains no fixed-point `ceil` or `floor` conversion (lines 121–129). A7 nevertheless requires flipping “ceil/floor direction” (line 183). That mutation cannot be implemented against the locked algorithm and can pass only through a dead/test-only alternate path. The O-side magnitude outcome when the shifted `(budget_x+E)` term exceeds `__int128`, plus O's literal `t=0/1` cases, are also left under “same exact scheme” rather than stated. — E1 — design lines 121–129/183.

Required revision: replace ceil/floor mutations with live mutations of the chosen implementation: exact-bit versus forbidden float-product path, strictness, hi/lo endpoint, `t=0/1`, and both X/O overflow-by-magnitude branches. State the O-side overflow and literal endpoint outcomes symmetrically so no overflowing shift is ever evaluated.

### M5 — The successor DESIGN carrier still lacks the canonical artifact locator and digest

The incoming header repeats `DESIGN_DOC_SHA256`, which is not the protocol's digest field, and carries no `DESIGN_ARTIFACT`. The canonical pair is `DESIGN_ARTIFACT: DD-engine-math-c4-20260826` plus `DESIGN_SHA256: <digest>`; the prior review's successor acceptance explicitly requested a fresh locator/digest carrier. Exact-file lint is clean because the noncanonical display field does not engage the routed digest check; that does not make it a valid byte-integrity carrier. — E1 — incoming header; protocol artifact-integrity section.

Required revision: the next DESIGN relay carries the canonical locator/digest pair at the revised bytes. Keep `DESIGN_DOC_ID` unchanged and parent it to this review.

## Retained revision-2 closures

- Preserve primitive-directed intervals and exact-rational named witnesses; do not return to whole-expression ulp stepping.
- Preserve exact binary P2 comparison, strict inequalities, adverse endpoints, no float product, and matrix fallback.
- Preserve the bid anchors `{0, S/4, S/2, 3S/4, S, k*±2}` and theory-equivalent rounding/deduplication.
- Preserve owner-honest A1/MR17 routing, split A4 oracles and activation counters, G1–G4, quality honesty, clock staging, and all boundary ownership.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — engine design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation/runtime authority
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory matrix/fixture and harness metadata contracts are consumed
- user-visible-control-with-materializer/downstream-consumer: yes — analysis metadata has harness/UI consumers
- test-runtime-role-mismatch: yes — A6/A7 currently name mechanisms that cannot prove the runtime path as specified
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — action-space parity and interval/TT soundness remain lock blockers
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Successor acceptance

Return revision 3 under the same `DESIGN_DOC_ID`, parented to this review, with canonical `DESIGN_ARTIFACT`/`DESIGN_SHA256`. Close M1–M5 while preserving every retained revision-2 closure above. Approval then depends on one identical engine/theory root action space, per-entry full-key verification with coherent default bytes, total directed preimages, live P2 mutation targets, and the valid byte carrier.

ACTIONS_GIT_REF:
- relay draft=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-2.md`
- source/tests/design=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
