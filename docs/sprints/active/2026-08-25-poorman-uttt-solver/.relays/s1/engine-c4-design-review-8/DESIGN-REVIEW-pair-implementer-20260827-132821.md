ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-8
PARENT_DISPATCH_ID: engine-c4-design-8
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — G6 settles the product choice; the remaining recurrence, evaluator, cache, TT-isolation, and cancellation seams are technical design corrections
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-8/DESIGN-planner-20260827-132221.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: 94020e7cc924619463760588fbd176250709d6900030b581ca48ef05be6e241c
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW must-revise — G6 is settled but alternation search and semantic-isolation seams are not executable

## Verdict

Must revise revision 8 of `DD-engine-math-c4-20260826` at digest `94020e7cc924619463760588fbd176250709d6900030b581ca48ef05be6e241c`.

G6 resolves the operator-owned product question correctly: at zero total, every auction is a 0–0 tie, the tie owner moves, F-C4 changes the next owner to the opponent of that mover, and the game reduces to ordinary alternating-move UTTT. The interim `p := 1/2` convention is properly rejected. The exact scale-zero theory oracle confirms the reduction.

Revision 8 is not yet executable because “the existing search runs on the degenerate zero-budget model” elides a semantic change. The preserved search is a threshold-auction solver: at every node it explores both X-marked and O-marked child sets and combines their min/max thresholds through `F`/zugzwang. An alternating game explores only the current tie owner's moves and backs up an X-centric outcome by max on X turns and min on O turns. The existing evaluator likewise returns a threshold center in `[0,1]`, not an alternating-game payoff in `[-1,+1]`. In addition, threshold TT entries are incompatible with alternating outcome values, and the per-matrix-entry wording does not prevent repeated sub-searches or define incomplete-search behavior.

The operator decision is accepted and no fresh human gate is needed. The standing IMPL STOP remains in force. This review grants no PLAN amendment, source/test edit, dependency action, branch movement, commit, PR, implementation dispatch, implementation, merge, publication, deployment, or release authority.

## Must-revise findings

### M1 — Lock the alternating solver recurrence and horizon value

The design must define the zero-total sub-search independently of the threshold `F` recurrence. Lock the X-centric value `V(s,h)` as follows:

- terminal X macro win: `+1`; terminal O macro win: `-1`; all-closed at zero chips: `0`;
- nonterminal `h = X`: `V(s,X) = max_{m in legal_X(s)} V(apply_X(s,m), O)`;
- nonterminal `h = O`: `V(s,O) = min_{m in legal_O(s)} V(apply_O(s,m), X)`.

Only the tie owner's marked child set is generated at each ply. No threshold interval aggregation, `F`, zugzwang branch, P2 gate, bid candidates, `p`, or `kappa` participates inside this sub-search.

The horizon contract must also name the actual X-centric binary64 score in `[-1,+1]`. If G6's “existing evaluator” means a transformation of `eval_estimate(s)`, state the exact transformation, finite/range checks, tie-owner treatment, and quality consequence. If it means a separate evaluator over the existing feature vector, lock that interface and weights/provenance. “Evaluator at horizon” is insufficient because the existing function returns a threshold estimate in `(0,1)` and its current search backs that value through threshold `F`.

Evidence: `engine/src/search/game_model.hpp` on preserved branch `afb91d5`, lines 35–50 requires both `children_x` and `children_o`; `engine/src/search/search.hpp` lines 576–697 searches both sets and calls `backup_node`/`guide_backup`; `engine/src/eval/eval.hpp` lines 81–87 returns a logistic threshold estimate in `(0,1)`. — E1.

### M2 — Isolate semantics and make the matrix-loop cost/cancellation bound real

The threshold TT cannot serve the alternating solver under the current key/layout: entries are keyed by state/tie but carry threshold intervals, moves, and threshold quality, with no semantic-mode discriminator. Reading or writing alternating payoffs through that table would cross-contaminate two different value domains. Lock one of: TT disabled for the zero-total solver; a distinct table instance and value carrier; or an explicit semantic namespace that cannot collide with threshold entries. The frozen 32-byte threshold-TT promise must remain honest.

The design must also require at most one zero-total alternating sub-search per unique `(child, successor_h)` within a matrix construction, cached across repeated action pairs. Every invocation shares the matrix's existing hard deadline and stop callback. An incomplete/cancelled sub-search must mark matrix construction incomplete and publish the previously staged root result; it must never synthesize or store a default payoff. State the node/depth budget and make clear it is charged per unique child, not per matrix entry. Without these requirements, a 28x28 matrix can repeat the same sub-search many times and the claimed “same per-child budget” is not a total cost bound.

Evidence: the preserved policy's current threshold cache is keyed once by `(Position,Tie)` and reused across payoff coordinates (`engine/src/adapter/policy.hpp` at `afb91d5`, lines 134–179), while `Search` owns a persistent threshold TT keyed only by model state/tie (`engine/src/search/search.hpp`, lines 61–93 and 519–548). — E1.

### M3 — Strengthen A9 to prove the new solver, not only the tie transition

The freeze-`h` mutation is necessary but not sufficient: a solver can alternate `h` while still exploring both marked child sets, applying threshold `F`, reversing max/min polarity, using the wrong terminal sign, or leaking threshold TT entries.

Required correction: add exact zero-scale ttt3 parity as the primary oracle for the new branch. Full-width alternating search must equal `theory/auction_ttt/discrete.solve(0)` on all materialized `(board,h)` states (or at minimum every reachable state with an explicit count); named mutations must freeze `h`, swap max/min polarity, admit the non-mover child set, and substitute threshold backup, each turning the row red. Add named horizon-score vectors, TT isolation/disabled proof, one-search-per-unique-child counters, and cancellation proof that no partial payoff enters RM+.

Fresh reference probe: the literal recurrence in M1 matched the exact scale-zero solver on all 22,186 nonterminal `(board,h)` states, including all 9,040 reachable states, with zero mismatches. This supplies a compact and authoritative acceptance oracle rather than relying on the freeze-`h` mutation alone. — E2.

## Review disposition

- G6 authority and semantics: pass. The successor design cites the requested operator ruling, records G6 canonically, and records the overturned half-share convention among rejected alternatives. — E1.
- Zero-total exact reduction: pass. Forced 0–0 ties plus F-C4 imply strict alternating play; the exact scale-zero probe confirms the max/min recurrence. — E1/E2.
- Zero-total executable carrier: must-revise M1. The design does not distinguish alternating max/min outcome search from the preserved threshold/F search or define the horizon payoff mapping. — E1.
- Cost and semantic isolation: must-revise M2. Per-unique-child caching, shared-deadline cancellation, incomplete-matrix fallback, and threshold-TT isolation are not locked. — E1.
- Acceptance evidence: must-revise M3. Freeze-`h` alone does not detect the main recurrence/value-domain mistakes; exact scale-zero parity is available and should bind the branch. — E1/E2.
- Exactness condition: pass once M1 is explicit. A full-width search whose leaves are all exact terminals may report the backed ordinal exact; any horizon evaluator or selective widening taints the result to estimate/bound per section 8. — E1.
- Retained M2–M4/G5 closures: pass. Seat-aware extraction, finite binary64 carrier, exact-provider widening, branch-complete `t_est`, no-estimate-pure-hit invariant, `kappa = 8` on positive total, P2, and solved-scale A8 are unchanged. — E1.
- Carrier integrity: pass. Incoming `TO`, parent, locator, and digest match the committed artifact at `main@61fffb9e1889389511f78f1452e1d4a750c30a75`; incoming exact-file lint and `git diff --check` are clean. — E1/E2 instrument evidence.

## Downstream transition

`engine.planner` must amend the G6 technical carrier and A9, then return a successor DESIGN relay with a new digest parented to this review. No PLAN amendment or IMPL continuation is authorized before an approving DESIGN-REVIEW of those exact bytes.

The daemon is ready with zero pending renders. Its conflict count remains the single historical INDEX divergence already in the projection stream; this seat does not repair or reinterpret that foreign history.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — must-revise enables only a successor design
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory's exact zero-scale oracle is the acceptance authority
- user-visible-control-with-materializer/downstream-consumer: yes — the matrix payoff selects published bids and moves
- test-runtime-role-mismatch: yes — the design names the preserved threshold search/evaluator for a different alternating outcome role without defining the adapter
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — G6 settles product semantics; the remaining corrections are technical and bounded
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Reviewed the complete committed revision-8 design, exact revision-7-to-8 design delta, incoming relay, review-7 decision request, theory F-C4/scale-zero solver, and preserved search/evaluator/policy seams. — E1.
- Ran the exact alternating-recurrence versus `discrete.solve(0)` probe, incoming exact-file lint, digest check, `git diff --check`, daemon status, and fresh status checks. — E2 instrument evidence.
- Main at `61fffb9e1889389511f78f1452e1d4a750c30a75` carried only concurrent foreign theory-plan paths before daemon admission. The isolated implementation worktree remains at `afb91d529258f1e38e27484007b9d5a090b796af` with the pre-existing untracked Task-14 gauntlet paths; this review did not alter either set. — E1/E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-8.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
