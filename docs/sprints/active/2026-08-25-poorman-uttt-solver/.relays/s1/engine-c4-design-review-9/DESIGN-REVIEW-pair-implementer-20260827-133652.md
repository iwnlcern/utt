ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c4-design-review-9
PARENT_DISPATCH_ID: engine-c4-design-9
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — G6 and the alternating-game product semantics are settled; the remaining memo identity/reuse correction is technical
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, theory.planner
IN_REPLY_TO: engine-c4-design-9/DESIGN-planner-20260827-133200.md
DESIGN_DOC_ID: DD-engine-math-c4-20260826
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-engine-math-c4-20260826
DESIGN_SHA256: ac6121ab15ee5aeeeaa740a87a939a2277227b5f7b880296505bc585258f8d22
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: engine-c4
OWNER: engine pair
SUBJECT: DESIGN-REVIEW must-revise — alternation carrier is sound, but the private memo key is collision-unsafe

## Verdict

Must revise revision 9 of `DD-engine-math-c4-20260826` at digest `ac6121ab15ee5aeeeaa740a87a939a2277227b5f7b880296505bc585258f8d22`.

Revision 9 correctly folds the rev-8 recurrence, horizon mapping, semantic isolation, cost/cancellation, and acceptance-oracle findings. The dedicated solver is now distinct from threshold search, generates only the current mover's children, backs an X-centric value by max/min, maps the existing threshold evaluator into `[-1,+1]`, never touches the persistent threshold TT, caches unique matrix children, and aborts the whole matrix without publishing a partial payoff. The fresh exact probe again matches `discrete.solve(0)` on all 22,186 materialized nonterminal `(board,h)` states, including all 9,040 reachable states, with zero mismatches.

One technical carrier remains unsafe. The private memo is locked as keyed by `(state key, h)`. In the preserved engine, `Position::key` is a 64-bit Zobrist hash, and this design itself treats such keys as collision-prone: the persistent threshold TT requires an independent tag and, in full-key mode, a fieldwise semantic snapshot. A private memo hit selected by the 64-bit key plus `h` alone can silently return a score, exactness bit, and depth belonging to a different position. Carrying `depth` in the value does not define when that entry is reusable.

No fresh human gate is required. The standing IMPL STOP remains in force. This review grants no PLAN amendment, source/test edit, dependency action, branch movement, commit, PR, implementation dispatch, implementation, merge, publication, deployment, or release authority.

## Must-revise finding

### M1 — Make the private alternating memo collision-safe and depth-qualified

Replace `(state key, h)` as the complete memo identity with a collision-safe contract. Either key by the full semantic position identity plus `h`, or use the hash only for bucket selection and require semantic equality before every hit. The existing `PosId {x, o, forced, tie}` / `Position::identity_equal` seam is sufficient for the production state; the ttt3 acceptance model needs the equivalent full board identity. Omitting budgets is correct because every state in this sub-game is at the constant coordinate `(0,0)`.

Also lock the horizon-depth reuse rule. The simplest executable rule is to include `remaining_depth` in the identity, or to reuse a complete memo entry only when its semantic identity and `h` match and its stored depth satisfies an explicitly stated rule. The acceptance row must force two distinct states to share the memo's hash key and prove that each retrieves its own value; a mutation removing the semantic equality guard must turn the test red. It must also exercise unequal requested depths so the stored `depth` field is behavioral rather than dead metadata.

Evidence: `Position::key` is `uint64_t` in `engine/src/core/position.hpp:29`; `zobrist_full` produces that hash from forced/tie/cells in `engine/src/core/zobrist.cpp:39-49`; semantic equality is separately defined at `engine/src/core/position.cpp:91-93`; the generic model already exposes collision-aware `TTKey` and `PosId` at `engine/src/search/game_model.hpp:15-27,77-83`; and the design's own threshold-TT contract requires collision detection at sections 5/A6. — E1.

## Review disposition

- Dedicated alternating recurrence: pass. Terminal signs, max on X, min on O, mover-only child generation, and exclusion of threshold machinery are explicit. — E1.
- Horizon mapping: pass. `clamp(1 - 2 * eval_estimate, -1, +1)` has the correct X-centric polarity, finite/range checks, explicit estimate quality, and disclosed `h`-independence. A positional cutoff evaluator need not directly consume the side to move when that limitation is disclosed. — E1.
- Exactness and oracle: pass. Full-width terminal resolution alone yields exact ordinals; the fresh recurrence probe matched 22,186/22,186 states and all 9,040 reachable states. — E2.
- Persistent-TT isolation: pass. The dedicated solver never reads or writes the threshold TT, and A9 demands bit-identical contents. — E1.
- Cost/cancellation and matrix publication: pass. One search per unique `(child, successor_h)`, per-unique-child budgets, the shared stop/deadline, and whole-matrix fallback without synthetic payoff are locked. — E1.
- Private memo identity/reuse: must-revise M1. A collision-prone Zobrist key is not semantic identity, and the carried depth has no locked hit-validity rule. — E1.
- Retained design closures: pass. G1-G6, P2, graded positive-total payoff, seat-aware extraction, exact-provider widening, and A8 remain unchanged. — E1.
- Carrier integrity: pass. The incoming relay is addressed to this seat, parents to review 8, carries the same design identity, and its digest matches committed `main@da1b345642b41f0604da0284a50ea3fe80a70ac9`; incoming exact-file lint and `git diff --check` are clean. — E1/E2 instrument evidence.

## Downstream transition

`engine.planner` should make the narrow memo-identity/depth correction and return a successor DESIGN relay with a new digest parented to this review. No PLAN amendment or IMPL continuation is authorized before an approving DESIGN-REVIEW of those exact bytes.

The daemon is ready with zero pending renders. Its one conflict remains the inherited historical INDEX divergence; this seat does not repair or reinterpret foreign projection history.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only DESIGN-REVIEW
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game chips only
- AI-or-automation-acts-downstream: no — must-revise enables only a successor design
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — theory's exact zero-scale oracle remains the acceptance authority
- user-visible-control-with-materializer/downstream-consumer: yes — the matrix payoff selects published bids and moves
- test-runtime-role-mismatch: no — rev 9 now supplies a dedicated alternating solver; M1 concerns memo correctness inside that role
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — G6 settles product semantics and the correction is bounded
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Evidence and actions

- Reviewed the complete committed revision-9 design, exact revision-8-to-9 delta, incoming relay, review-8 findings, exact theory solver, and preserved engine position/key/search/policy interfaces. — E1.
- Re-ran the exact alternating recurrence against `discrete.solve(0)`: 22,186 states, zero all-state mismatches; 9,040 reachable states, zero reachable mismatches. Also ran incoming exact-file lint, digest verification, daemon status, `git diff --check`, and fresh worktree status checks. — E2 instrument evidence.
- Main at `da1b345642b41f0604da0284a50ea3fe80a70ac9` carried only concurrent foreign theory-plan paths before daemon admission. The isolated implementation worktree remains at `afb91d529258f1e38e27484007b9d5a090b796af` with the pre-existing untracked Task-14 gauntlet paths; this review did not alter either set. — E1/E2 instrument evidence.
- No source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release edit/action was performed by this seat.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/DESIGN-REVIEW-engine-c4-design-review-9.md` — daemon admission pending at authoring
- source/tests/design/plan=none — review-only; no source, test, design, plan, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
