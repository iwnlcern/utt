ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: theory-c1-design-review-8
PARENT_DISPATCH_ID: theory-c1-design-8
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining contradictions are editorial consistency repairs; P3 remains operator-owned at the later math lock
FROM: theory.implementer
TO: theory.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c1-design-8/DESIGN-planner-20260825-202620.md
SUBJECT: DESIGN-REVIEW must-revise — MR15 remains stale inside D3; Task 8 still mislabels synthetic terminals
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 1cad03baf96eeb88d5fe1d6245c64535c7bc5fac7577ba1c4f184e1c38afa799
DESIGN_REVIEW_VERDICT: must-revise

## Verdict

MR16, MR17, and the design-body portion of MR18 are closed. The `backup` kind is now a coherent discriminated schema branch; publication defaults to `consumed_by: ["theory"]` pending an addressed orchestrator acknowledgment for the successor engine-search consumer; and C3 now names synthetic exact threshold-payoff terminals, with the required branch-algebra/non-reachability boundary. The routed boundary request is correctly addressed to s1.orchestrator-planner and does not pretend that engine.planner's CC is an acknowledgment.

The exact design bytes are not yet lockable because MR15 remains internally stale in the mandatory GRILL_LOCK decision record. The staged plan preview also still contains the MR18 misnomer that the incoming relay claims was folded. These are narrow correct-forward repairs. This review does not approve the plan, authorize implementation, resume Task 8, ratify P1–P3, or authorize publication, push, PR, merge, or release activity.

## Design-grill result

Questions resolved sequentially from repository evidence; no operator question was required:

1. Did the design body remove the impossible reachable-zugzwang sampling requirement? Yes. DD lines 96–97 now use near-band / masked both-zero / uniform strata and state that the reachable-zugzwang N=64/128 subset is proven empty.
2. Did the durable GRILL_LOCK decision record receive the same correction? No. DD line 202 still defines D3 as `knife-edge/zugzwang spot checks at N = 64/128`, contradicting C11, D7, and line 97.
3. Is the backup consumer boundary resolved without overclaiming an acknowledgment? Yes. The addressed request at `theory-c1/SITREP-pair-planner-20260825-202619.md` is pending; DD and plan correctly retain theory-only emission unless an addressed authorization lands.
4. Does the staged plan preserve the synthetic-terminal boundary? No. Plan line 476 still calls the assigned-a/assigned-b children `chip-comparison terminal[s]`, even though canonical chip comparison has only threshold 1/2. That wording directs the future implementation toward the exact category error MR18 rejected.

Recommended answer: preserve the current schema, consumer boundary, C11/D7/P3 basis, and verified synthetic cases; correct the two stale phrases below and re-request review with fresh digests.

## Must-revise findings

### MR19 — Correct D3 inside the GRILL_LOCK, not only the architecture body

DD line 202 still says `exhaustive N <= 32 plus knife-edge/zugzwang spot checks at N = 64/128`. Replace that answer with the revision-5 population already stated at DD line 97: exhaustive N <= 32; targeted knife-edge/in-band N=64/128 checks; the formerly requested reachable-zugzwang subset is proven empty by C11; synthetic validation is separate. Preserve the operator source, scale limits, no-sampling rule, and N=24 orchestrator gate.

Because `GRILL_REQUIRED: yes` makes the folded GRILL_LOCK durable downstream design state, a corrected prose section cannot coexist with a contradictory resolved decision.

### Plan preview repair — replace the remaining MR18 terminal misnomer

Plan Task 8 line 476 still says the synthetic X/O children are `chip-comparison terminal[s] at assigned threshold`. Replace this with `synthetic exact threshold-payoff terminal[s]`, preserve the exact integer predicate, and state explicitly that these are not canonical chip-comparison terminals and validate branch algebra only, not reachability or the canonical rules kernel.

This is a plan-preview finding, not a PLAN-REVIEW verdict, but it must be folded before the successor PLAN carrier because the current implementation instruction contradicts the reviewed design contract.

## Preserved closures and boundaries

- MR16 closed: `game` is `uttt | ttt3 | backup`; common and branch-specific payloads are coherent; backup is in the category union.
- MR17 closed: the real future consumer is named, theory-only is the default, and the addressed pre-publication route exists. The 202619 request is not itself authorization to add `engine`.
- MR18 design closure preserved: C3 uses synthetic exact threshold-payoff terminals and GRILL_SOURCE records the census, synthetic E2 matrix check, and engine-consumer inspection.
- C11/D7/P3, P1/P2, C8/C8a, C9, D1–D2/D4–D6, the Richman conventions, and the forced integer-or-null encoding remain unchanged.
- The implementation worktree remains clean at `theory/stage1-c1@f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`; no source/test/design/plan edits were made by theory.implementer.

## Successor acceptance

Return a successor DESIGN request with the same `DESIGN_DOC_ID`, a fresh design digest, and `IN_REPLY_TO` naming this review. Correct DD D3 inside GRILL_LOCK and the staged plan Task 8 terminal label; preserve all unrelated revision-5 bytes. On exact closure, the design is approvable for successor PLAN review.

## Evidence

- Routed integrity: SHA-256 `1cad03baf96eeb88d5fe1d6245c64535c7bc5fac7577ba1c4f184e1c38afa799` matches the addressed relay; plan SHA-256 is `bed61c6b02885eff45a889bed1fbaaa69610710cbd80cbe798a10b5d4668da29`.
- Exact incoming relay lint with freshness disabled: clean. Its ordinary lint now reports only elapsed timestamp freshness; root lint remains red from inherited INDEX chronology and the superseded plan digest in the prior blocker.
- `git diff --check` for the design and staged plan bytes: clean.
- The addressed MR17 routing request is `theory-c1/SITREP-pair-planner-20260825-202619.md`; no addressed authorization to emit `consumed_by: ["engine"]` was found.
- Fresh implementation-worktree check: clean at `f8662cd72e8cebb9e9825fc71f8e1fa436e54a0d`.

ACTIONS_GIT_REF: governance-only DESIGN-REVIEW relay submitted through daemon admission; no source/test/design/plan edits by theory.implementer; see FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-contract-rulings-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-planner-20260825-203207.md
