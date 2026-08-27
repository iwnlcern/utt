ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-3
PARENT_DISPATCH_ID: engine-c4-design-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the residuals are technical contract corrections within the retained operator choices
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-3/DESIGN-planner-20260826-184427.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 47689243ac655937ec1e01460aaf213477fb5c82ccda015d13a1f61e9a5ac176
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW must-revise — rev 3 closes prior M1–M5 but matrix exactness and two TT mechanics remain

## Verdict

Revision 3 closes the five residuals returned in `engine-c4-design-review-2`: the root action labels are now Cartesian `(bid, move)` pairs with an explicit production ordering; the sidecar is per bucket/way and the empty/sentinel bytes are coherent; every composite interval endpoint and guard path is literal and total; A7 mutates only live P2 mechanisms; and the DESIGN carrier has the canonical locator/digest pair. Those closures may be retained.

Approval remains blocked by three narrow execution contradictions exposed by the repaired text: A6 requires two same-key/same-tag positions to survive together, but the store rule never consults the per-way full-key snapshot before applying its same-key merge/overwrite policy; the supposedly frozen SMP-publication bytes do not bind the second 16-byte half and are guaranteed only 8-byte alignment; and A8's exact payoff parity rests on the false claim that primitive-outward full-width backup produces degenerate intervals at fixture scales.

No operator product choice is needed. The production midpoint fallback itself is an explicitly estimate-tainted policy inside P2's accepted no-exact-solver band; the blocker is the document's separate claim of exact solved-scale matrix parity and the missing TT mechanics.

## Target and integrity

- Reviewed revision 3 at `main@1fb6288`; its on-disk SHA-256 is `47689243ac655937ec1e01460aaf213477fb5c82ccda015d13a1f61e9a5ac176`. — E1 — `git log`, `sha256sum`, full-file and revision-2-to-3 diff reads.
- The incoming relay is addressed to `engine.implementer`, parents to `engine-c4-design-review-2`, carries the canonical `DESIGN_ARTIFACT`/`DESIGN_SHA256` pair, and passes exact-file relay lint. — E1.
- The embedded grill and G1–G4 are unchanged. No grill decision is reopened. — E1 — design section 12 and exact delta.
- Review-only: no source, test, design-doc, branch, commit, PR, merge, publication, or deployment action was authorized or performed. — E1 — phase/authority and final status.

## Fold disposition of review-2 residuals

- Prior M1: closed. Section 7 now uses the theory-shaped Cartesian action labels and defines the production UTTT candidate ordering. Residual M3 below concerns exact payoff construction, not action-space parity.
- Prior M2: mostly closed. The per-way sidecar and occupied/sentinel layout fix the returned collision-index and empty-byte defects. Residuals M1–M2 below concern the still-unstated store decision and the separate future publication contract.
- Prior M3: closed. All six directed composite endpoints are literal, and the zero/one guard table is total without division.
- Prior M4: closed. The O-side exact comparison and overflow result are literal, and A7 contains only live mutation targets.
- Prior M5: closed. The incoming carrier's canonical locator resolves to the reviewed bytes and its digest matches.

## Must-revise residuals

### M1 — The per-way sidecar fixes probe identity but not collision-aware store identity

Section 5 lines 104–109 says same-key entries are merged/protected by the store rule, every store writes the chosen way's snapshot, and two positions colliding in both the 64-bit key and 32-bit tag occupy different ways. It specifies the full-key fieldwise comparison only on PROBE. For the second colliding store, the table-visible key/tag are identical to the first entry and no Position identity lives in the 32-byte entry. Without a store-side comparison against the snapshot at each candidate way, the same-key rule can only treat the second position as the first and merge, protect, or overwrite it; merely writing the new snapshot after choosing that way destroys the first binding rather than allocating a second way. A6's two-way setup is therefore asserted but not constructible from the locked store algorithm. — E1 — design lines 104–110/198; rules-DD section 5 fieldwise full-key policy.

Required revision: in full-key mode, define a same-entry store match as bucket/tag plus a fieldwise match against the snapshot at THAT way. A tag match with a snapshot mismatch is a collision: continue scanning for another semantic match or select a distinct empty/replacement way; only a semantic match may use the same-key merge/protection rule. Bind sidecar invalidation/write ordering to entry replacement, and make A6 mutation-sensitive to replacing this store-side comparison with tag-only matching.

### M2 — The frozen layout is not yet publishable as the promised two atomic halves

The byte table defines `smp_word@4` as `tag XOR low32(bits(lo))` (line 96), while line 111 separately says the tag is XOR-folded over the value half. With halves `[tag,smp_word,lo]` and `[hi,moves,depth,gen,flags,pad]`, the defined word covers only data in its own first half and leaves the entire second half unbound. A reader can therefore combine a current first half with a stale second half and still pass that word; it cannot establish a coherent entry publication. The layout also promises two 16-byte atomic halves while asserting only `alignof >= 8`, which does not guarantee either half is 16-byte aligned; the 128-byte “two cache lines” bucket likewise has no locked cache-line alignment. Deferring atomics is permitted by G2, but the current bytes/alignment do not substantiate the stronger claim that adding only a publication discipline later is sufficient. — E1 — design lines 93–111.

Required revision: give `smp_word` one consistent future meaning that detects cross-half mismatch (for example, a version/check over every semantically published byte of the second half, with a locked read/write validation sequence), or explicitly defer its derivation without making the contradictory coverage claim. Lock 16-byte entry/half alignment and the bucket alignment needed for the stated cache-line geometry, with corresponding static assertions. Preserve the 32-byte offsets if desired; no operator choice is reopened.

### M3 — Directed rounding prevents the degenerate fixture intervals on which exact A8 payoff parity relies

Section 7 line 150 says full-width fixture search yields degenerate child intervals, so every post-payment matrix payoff is exact. That conflicts with sections 2–3: every nonterminal composite steps every primitive outward by one ulp. Even the simple ordered backup `F(0,1/2)` produces the nondegenerate enclosure `[0x1.5555555555553p-2, 0x1.5555555555557p-2]` under the literal section-3 sequence, not `lo == hi`. Section 8 itself calls exact search “degenerate up to accumulated ulps,” which is not the degenerate-interval condition defined at line 31. If a solved-scale post-payment share lies inside such an enclosure, the exact gate cannot classify it; the midpoint fallback is estimate-tainted and cannot establish A8's entry-wise exact discrete payoff equality or P1a authority. — E1 — design lines 25–32/46–47/149–155/167/200; direct evaluation of the locked primitives.

Required revision: make the solved-scale A8 payoff path explicit and total without assuming degenerate floating intervals. Either inject the authoritative exact discrete outcome table/oracle test-side for every post-payment child while still exercising the engine's action construction and transition semantics, or define another executable exact fallback and prove it is used for every ambiguous entry. A separation-only proof is acceptable only if named fixture evidence covers every matrix entry, including equality/knife-edge cases. Keep the production midpoint fallback estimate-tainted; do not relabel it exact.

## Retained revision-3 closures

- Preserve Cartesian `(bid, move)` actions, theory-equivalent ttt3 ordering, production tactical ordering, post-payment budgets, and estimate taint for production midpoint payoffs.
- Preserve the per-(bucket, way) sidecar geometry, occupied bit, explicit 32-byte offsets, occupied-only `0xFF` move sentinels, and zero-initialized empty entry.
- Preserve the literal F/X/O directed sequences, total point-window guards, clipping/intersections, and A5 rounding-direction mutation.
- Preserve the exact binary P2 gate, literal O endpoint/overflow rules, live A7 mutations, owner-honest fixture boundary, per-cut A4 evidence, G1–G4, and the canonical carrier fields.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — engine design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — no implementation/runtime authority
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory matrix/fixture and harness metadata contracts are consumed
- user-visible-control-with-materializer/downstream-consumer: yes — analysis metadata has harness/UI consumers
- test-runtime-role-mismatch: yes — A6's collision setup and A8's exact matrix proof are not executable from the stated runtime/test mechanics
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — TT collision/publication honesty and solved-scale payoff exactness remain lock blockers
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Successor acceptance

Return revision 4 under the same `DESIGN_DOC_ID`, parented to this review, with a fresh canonical locator/digest carrier. Close M1–M3 while preserving every retained revision-3 closure above. Approval then depends on a collision-aware store path that can construct A6's two ways, a genuinely future-publishable or honestly deferred SMP word/alignment contract, and an exact solved-scale payoff source that makes every A8 matrix entry executable without treating outward-rounded intervals as degenerate.

ACTIONS_GIT_REF:
- relay draft=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-3.md`
- source/tests/design=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
none — clean tree before daemon admission; daemon-rendered relay and INDEX projection are transport artifacts reported after submission
