ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-1
PARENT_DISPATCH_ID: engine-c4-design-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — engine.planner can correct the technical contract; the MR17 fixture-consumer edge routes through s1.orchestrator-planner
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-1/DESIGN-planner-20260826-180802.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 43a0c7c996e683fc4b0381f9b6e018d4b58259139819d2b5979b41284e75828c
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — repair interval preimages, P2 conversion, root action parity, TT verification, and acceptance proof

## Verdict

The theory backup equations, tie-owner transition, conditional-root envelope, widening honesty, completed-iteration publication, and operator choices G1–G4 are coherent and may be retained. The exact design bytes are not yet safe to lock. The window-preimage rounding rule is demonstrably non-enclosing; the P2 fixed-point conversion is not mechanically defined; the root matrix omits action anchors present in both the canonical spec and theory reference; fixture-mode full-key TT verification has no storage mechanism; and acceptance rows A1/A4 are respectively blocked by the live consumer contract and capable of proving the wrong property.

No new operator product decision is required for M1, M2, M4, or M5. M3 should restore the already-approved action set; choosing a narrower game instead requires an explicit governing amendment because it changes the canonical spec and the theory cross-check target.

## Review target and integrity

- Reviewed `docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-math-c4-20260826.md` revision 1 at `main@59f7ab9a29c55679e691a0caac472d5ecbc9a764`. Its on-disk SHA-256 is the declared `43a0c7c996e683fc4b0381f9b6e018d4b58259139819d2b5979b41284e75828c`. — E1 — `sha256sum`, `git log`.
- The incoming relay is addressed to `engine.implementer`, parents to `engine-c4`, and passes exact-file relay lint. It has design-only authority and requests this read-only review. — E1 — exact relay read and `relay-lint --no-freshness`.
- The top-level dispatch carried `GRILL_REQUIRED: yes`; the reviewed document embeds `GRILL_LOCK_ID: GRILL-engine-c4-20260826` with G1–G4, sources, rejected alternatives, and no unresolved operator item. This review does not rerun or reopen that completed grill. — E1 — orchestrator dispatch and design section 12.
- No source, test, design-doc, branch, commit, PR, merge, publication, or deployment mutation was authorized or performed. The only authored byte is this review relay draft for daemon admission. — E1 — phase/authority plus final status below.

## Must-revise findings

### M1 — Whole-expression two-ulp stepping is unsound for the required preimage algebra

Section 2 applies a two-ulp step after every arithmetic composite, and section 4 applies that rule to `a = 1 - b(1-t)/t`. That inverse has cancellation near `a=0`; two ulps of the rounded result do not cover error accumulated in the operands. With binary64 inputs `b=0.25`, `t=0.2`, the stated evaluation yields `0.0`; stepping twice gives approximately `[-9.88e-324, 9.88e-324]`, while exact rational evaluation of those same binary64 inputs gives `a = 1/14411518807585588`, approximately `6.94e-17`, outside the interval. — E2 — local `Fraction.from_float` counterexample.

The interval-valued inverse is also underspecified: `a(t,b)` increases with `t` and decreases with `b`, so the outer endpoints require crossed `b` endpoints; the O-child inverse is `b=t(1-a)/(1-t)` and has a `t=1` guard, not the X inverse's `t=0` guard. Ordered-branch constraints and clipping must be explicit before intersecting with the min/max aggregate domain. — E1 — algebraic differentiation of the displayed formula.

Required revision: lock an actually enclosing primitive-by-primitive directed-rounding algorithm (or an algebraically stable, separately proved alternative) for both inverses, including endpoint pairing, ordered-branch intersection, clipping, `t=0`, and `t=1`. Extend A5 with named cancellation/endpoint vectors, including the counterexample above; randomized rationals alone are not the proof.

### M2 — P2's “exact integer comparison” lacks an exact endpoint-conversion contract

Section 7 names `__int128` only after a double endpoint has been “scaled to fixed-point with outward rounding”. It does not fix the scale, the exact ceil/floor operations, or how conversion avoids binary floating multiplication rounding. A naïve upper conversion is unsafe: binary64 `t=1e-9` is slightly greater than exact `1/10^9`, but binary64 `t*10^9` rounds to `1.0`; `ceil(t*10^9)` returns 1 although the exact scaled binary64 value has ceiling 2. An underestimated upper endpoint can turn a boundary X test into an unsound forced classification. — E2 — local exact-rational conversion check.

Required revision: lock the scale and exact conservative conversion from the binary64 bit pattern, e.g. `Q_hi=ceil_exact(T.hi*S)` and `Q_lo=floor_exact(T.lo*S)`, then specify the integer inequalities literally: X only if `bx*S - Q_hi*M > E*S`, O only if `Q_lo*M - bx*S > E*S`, with declared bounds proving `__int128` cannot overflow. A7 must include conversion-cliff vectors (including `1e-9`) and mutation guards for ceil/floor direction, strict `>`, endpoint choice, and matrix fallback.

### M3 — The engine and theory root matrices currently have different action sets

Section 7 defines only `{0, clamp(k* + d), d in [-2,2]}`. The canonical spec requires critical-neighbor bids plus stack fractions and all-in. Theory's landed reference implements `{0, stack, floor(stack/4), floor(stack/2), floor(3*stack/4), rounded-critical +/-2}` before adding candidate moves. A8 then asks the narrower engine matrix to agree with theory's exact LP values, but values, strategies, regret, and exploitability are not comparable across different games. — E1 — canonical spec Component 2; `theory/auction_ttt/lp_reference.py::_candidate_bids`; theory plan Task 10; design sections 7/11.

Required revision: restore the stack-fraction and all-in anchors with the same deduplication/clamping/rounding semantics as the theory reference, and make A8 compare identical action labels and payoff-matrix bytes before comparing solver outputs. Any intentional narrowing instead needs a governing amendment and a newly matched theory oracle; A8 cannot adjudicate candidate-set adequacy against a different set.

### M4 — The fixed TT entry cannot perform the promised fieldwise full-key check as specified

The 32-byte play entry carries tag, interval, two moves, depth, generation, flags, and spare byte, but no identity-bearing Position fields. Section 5 nevertheless requires fixture mode to compare `x`, `o`, `forced`, and `tie` fieldwise. No sidecar, alternate fixture entry, or ownership/lifetime rule is defined, so A6's collision-injection path has no executable storage contract. The byte layout also lacks explicit offsets/padding and a no-move sentinel although `move_x/move_o` consume all 0–80 legal values and terminal/incomplete entries can lack a conditional move. — E1 — design section 5 against rules-DD section 5.

Required revision: define the fixture/full-key storage mechanism and probe comparison, plus exact offsets, size/alignment assertions, tag/checksum derivation, initialization bytes, and move sentinel. A6 must collide the bucket key and 32-bit tag while differing in one semantic Position field, then prove rejection through the production probe seam configured in full-key mode; a test-only predicate that bypasses probe is insufficient.

### M5 — A1 and A4 are not executable, non-vacuous acceptance rows

A1 requires engine consumption of every landed `backup_*` fixture, but `theory/fixtures/backup_cases.json` still normatively declares `consumed_by: ["theory"]`; the latest theory merge/reconciliation records retain MR17 as outstanding. The current design also declares theory/fixture changes out of scope. The engine cannot silently reinterpret the owner tag as an engine contract. — E1 — fixture bytes; theory-c1 merge/reconciliation relays; design sections 1/11/13.

A4 requires window, dominance, and precision cuts to return intervals identical to unpruned full-width search. That is the wrong oracle for the specified algorithms: a window cut may return any sound outside-window bound, and a positive-precision cut intentionally stops before the full-width result. Conversely, an implementation in which no cut fires can satisfy an equality-only sample, making the row vacuous. — E1 — design sections 4 and 11.

Required revision:

1. Route an addressed MR17 decision to the theory owner and consume amended owner bytes, or rewrite A1 as an explicitly non-contract reference test that does not claim the theory-only fixtures as engine-owned acceptance. Preserve the theory file and tag until its owner changes them.
2. Split A4 by cut. Require named fixtures that make each dominance side, each parent-window side, the precision cut, and the hull restriction fire, with nonzero per-cut counters or equivalent mutation-sensitive evidence. Compare full-window/no-precision results to the exact/unpruned oracle; for bounded-window and precision returns, assert containment, correct-side exclusion/parent-decision neutrality, requested-width semantics, and no illegal hull cut rather than byte-identical intervals.

## Retained approvals

- F-C1–F-C7 backup semantics, terminal values, actual-mover tie transition, and conditional `h=null` envelope align with the landed theory record.
- G1's directed interval representation remains an acceptable operator-selected direction; M1/M2 require a correct mechanical realization, not a return to runtime rationals or heuristic epsilon.
- Selective widening's prohibition on incomplete `exact`, estimate taint, and completed-iteration publication are coherent. The PLAN should spell the actual one-sided formulas, but the trivial-side enclosure makes the design direction repairable.
- The 27 s soft / 29 s hard search clock within a 30 s referee limit and staged-last-completed result are coherent, subject to A11's injected-clock and E3 proof.
- G2's single-threaded s1 choice is retained. Future SMP publication discipline remains deferred; the revision need only make the frozen byte layout unambiguous and internally consistent.
- G3/G4 and the tournament bar remain operator-ratified. No implementation, PLAN, or merge authority follows from this verdict.

## Design-review checklist

- Identity, digest, lineage, addressee, scope, and embedded grill: pass. — E1.
- Backup/terminal/tie semantics and root conditional envelope: pass. — E1.
- Interval lifting and cutoff transport: must-revise M1/M5. — E1/E2.
- TT semantics/collision acceptance: must-revise M4. — E1.
- P2 forced-classification safety: must-revise M2. — E1/E2.
- Root matrix/theory LP parity: must-revise M3. — E1.
- A1–A11 executable and non-vacuous: must-revise M1–M5; A2/A3/A5–A11 may be retained only with the named strengthening above where applicable. — E1.
- Operator-judgment items: none unless the Planner proposes retaining the narrowed root action set or changing an owner contract without its owner.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — engine math design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — game chips are simulated inputs, not real monetary state
- AI-or-automation-acts-downstream: no — this relay grants no implementation or runtime action
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes theory-owned fixtures and harness/UI metadata contracts
- user-visible-control-with-materializer/downstream-consumer: yes — engine analysis metadata is consumed by the harness log and UI view
- test-runtime-role-mismatch: yes — full-key fixture mode lacks a concrete runtime storage/probe mechanism
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — root action-set drift and unsound cutoff/P2 mechanics block the design lock
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade is requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Successor acceptance

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh locator/digest carrier, and `IN_REPLY_TO` naming this review. Close M1–M5; preserve F-C1–F-C7, P1/P2/P3, G1–G4, quality honesty, clock staging, and all owner boundaries. The successor can be approved when the numerical transforms are mechanically enclosing, P2 conversion is exact and conservative, the engine/theory matrices share an action set, full-key TT verification is executable, and every acceptance path has a non-vacuity witness.

ACTIONS_GIT_REF:
- relay draft=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-1.md`
- source/tests/design=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
