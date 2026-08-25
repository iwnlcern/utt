ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-plan-review-1
PARENT_DISPATCH_ID: engine-c1-plan-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the blockers are technical PLAN corrections within engine.planner authority; any retained design deviation must instead route through a reviewed design successor
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/engine-c1/PLAN-planner-20260825-085908.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
PLAN_SHA256: a900814524f12aca5b122a591eec243adf8c2cf57ff2236e3545529af9c0dd22
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW must-revise — execution hygiene, proof seams, owner-corpus coverage, and benchmark discipline

## Verdict

The plan has a strong decomposition and correctly preserves the rules/search-math split, but it is not implementation-ready at this digest. Eight blocking correction groups remain in branch/staging hygiene, exact task bytes, dependency ordering, locked budget/clock seams, collision verification, owner-contract conformance, and benchmark proof mechanics.

This verdict is review-only. It authorizes no source/test/plan/design edit by this seat, dependency download, branch creation or movement, commit, PR, implementation, delegated dispatch, merge, publication, or deployment. The standing red-root dispatch hold remains in force independently of this must-revise verdict.

## Must-revise findings

### PR1 — Branch and staging instructions do not preserve the dispatched base or source-only commits

The orchestrator pins `BASE: main@63b4b7b`, while the plan says to create `engine/rules-core-c1` from whatever `main` names at IMPL start. That is not an exact base lock. Task 1 then builds under `engine/build/` and runs `git add engine/`, but the repository ignores only `*.swp` and `.DS_Store`; `git check-ignore` confirms the CMake/Ninja build tree is not excluded. The first commit would therefore sweep generated build outputs. — E1 — plan lines 25, 74, 107, and 125–130; `.gitignore` exact read; orchestrator relay lines 20–22.

Required revision: pin the exact branch-creation/preflight command and ancestry check to `63b4b7b` unless the orchestrator issues a new BASE. Add an in-scope `engine/.gitignore` for the build tree before configure, replace broad staging with explicit source/dependency paths, and add a staged-set/status assertion proving no build artifact is committed. Keep the intentional `engine/bench/baseline.json` trackable.

### PR2 — Several allegedly executable tests still delegate unresolved bytes and legal sequences to the Implementer

Task 3 locks cell id to `3*row+col`, then supplies MSB-first masks and tells the Implementer to “pick” and rewrite them later. The DD already fixes cell c as the row-major cell and as the `3^c`/bit-c index; there is no remaining choice. Task 5 leaves the macro-win fixture as “work the routing out once,” Task 6 contains the undefined placeholder `Cell81`, and Task 7 leaves forced-only/tie-only legal constructions to the writer. These are precisely the cases the review request names as sink risks. — E1 — plan lines 62, 141–160, 217–268, 388–395, 448–456, and 498–506; DD sections 3, 4, and 7.

Required revision: make bit 0 = row 0/column 0 normative everywhere and provide corrected mask literals/comments now. Replace `Cell81` with the final exact interface. Pin legal move sequences or mechanical fixture builders with exact pre/postcondition assertions for macro-win, forced-only, and tie-only cases; do not leave routing invention to IMPL.

### PR3 — Dependency instructions are neither pinned nor ordered before their first consumer

The global constraint says dependencies are pinned, but Tasks 1 and 10 say “latest 2.4.x” and “latest 3.x” without exact tags, source URLs, or content digests. Task 9 consumes nlohmann/json before Task 10 vendors or wires it. The claim that JSON appears only under `engine/src/adapter/` also contradicts the JSON fixture test under `engine/tests/`. — E1 — plan lines 9, 17, 20, 76–79, 551–580, and 584–623.

Required revision: select exact doctest and nlohmann/json versions at PLAN time, pin immutable upstream URLs plus SHA-256 values, verify exact license bytes, and record the pins in build metadata. Vendor/wire nlohmann before the theory-fixture reader consumes it. State the actual boundary: production core is JSON-free; adapter and test-only fixture ingestion may depend on the pinned header.

### PR4 — The locked RootContext/budget seam is missing, while the locked clock seam is self-deferred

The DD locks `RootContext { seat, budget_x, budget_o }`, canonicalization into `Position + RootContext`, and budget-layer terminal tests for positive, negative, and equal chip margins. The plan substitutes loose fields on `TurnRequest` and proposes fixture-side sign assertions without naming any production budget-layer resolver, so the test can pass without testing engine behavior. The plan also places the DD's injected-clock seam out of scope even though the approved DD retains it in section 7; that is a design deviation, not a PLAN-level mechanical choice. — E1 — plan lines 578, 598–618, 674–700; DD lines 39, 91–92, 107–110, and 121–124.

Required revision: add the exact `RootContext` carrier and a pure production seam for all-closed budget resolution, with X-positive/O-negative/exact-draw tests against that seam. Either include the minimal injected-clock interface and fake-clock contract required by this lock, or route an explicit design successor/supplement for approval before the PLAN excludes it; do not self-defer locked bytes.

### PR5 — The proposed secondary Zobrist tag cannot detect a primary-key collision

Task 7 proposes `zobrist_tag(uint64_t key)` as a deterministic fold of the already-collided primary key. Any two positions with equal primary keys necessarily produce equal such tags, so it adds zero verification and violates the DD's “different Zobrist fold” collision policy. Its example tests also do not exhaust every cell/seat, forced-state, and tie-state input despite the DD's ANY-input sensitivity obligation. — E1 — plan lines 469–506; DD lines 76–79 and 106.

Required revision: derive the 32-bit tag independently from Position identity using a distinct seeded table/fold, not from the primary key. Pin the independent seed/construction and test the full declared input population: every cell/seat mark, all ten forced states, and all three tie states, alongside incremental-primary versus full recomputation.

### PR6 — Import validation and adapter verification do not cover the locked owner contract

`Position::from_parts` rejects invalid locals and out-of-domain forced/tie values, but the DD also requires forced-board and tie-state consistency; the plan does not explicitly reject a non-null forced board that is already closed or globally contradictory terminal/import states. The adapter defines only `parse_turn`, treats `game_end` as unvalidated bookkeeping, and tests one hand-copied example. Task 13 then maps DD criterion 3 to that example E2E, even though criterion 3 requires the harness conformance corpus. — E1 — plan lines 557–580, 592–644, and 666–670; engine DD lines 81–97 and 183–186; harness DD lines 28–75 and 155–160.

Required revision: enumerate and test every locked import-consistency rejection, including forced-to-closed and terminal/global contradictions. Add strict parsers/validators and tests for all three referee-to-engine message types (`hello`, `turn`, `game_end`) while preserving unknown-key tolerance. Consume the harness-owned schemas/transcript/conformance corpus through a named test/driver; if those artifacts have not landed, hard-report criterion 3 pending/blocked rather than counting the single example as E2.

### PR7 — Benchmark mechanics do not prove the approved aggregation/session predicate

Both modes request ten runs, but the README instruction says the first run is discarded as warmup; that leaves only nine measured runs, below the DD's minimum of ten. Reference and candidate are separate commands around a commit, with no binary/session identity check, so the plan does not prove that the stored reference and candidate used the same committed binary, flags, harness, environment, and session. — E1 — plan lines 648–662; DD line 186.

Required revision: make warmup separate from at least ten measured repetitions. Record and validate executable digest, compiler, flags, CPU/environment, fixed seed set, measured-run count, and a same-session identifier in the baseline/candidate verdict. Pin an exact reference-first command sequence in which the baseline is durably committed before candidate acceptance, candidate cannot write it, and candidate refuses any identity/environment mismatch.

### PR8 — Acceptance mapping overclaims conditional owner artifacts

The plan correctly allows local mid-development runs to warn on absent theory fixtures, but its verification target calls criteria 1–4 executable now and its acceptance map has no symmetric honest state for the absent harness corpus. Green-with-override is not E2 for criterion 1, and a copied request is not E2 for criterion 3. — E1 — plan lines 563–580, 666–670, and 698–700.

Required revision: make the final sweep a truth table: each owner artifact is either present, consumed, and green at E2, or the corresponding criterion is explicitly pending/blocked with the absence command/output. No override/skipped/copy-only path may be summarized as acceptance-complete.

## Retained strengths and scope

- Design identity, approving-review lineage, plan identity, locator, and both declared digests are correct. The on-disk and committed plan bytes match SHA-256 `a900814524f12aca5b122a591eec243adf8c2cf57ff2236e3545529af9c0dd22`. — E1.
- The 13-task decomposition, TDD intent, independent local oracle, exhaustive 19,683-entry proof, value-copy immutability, total terminal legality, both-mover perft, protocol ownership, anti-tautology principle, and rules/search-math split are directionally sound and should be retained. — E1.
- Search backup/cutoff/bound/TT-value/widening semantics, owner schema edits, multithreading/SIMD/ML, merge, publication, deployment, and release remain out of scope. — E1.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local engine PLAN review
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — fixed-point values are simulated game chips
- AI-or-automation-acts-downstream: no — no runtime or implementation action authorized
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation in this review
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes harness protocol and theory fixture contracts
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control changed
- test-runtime-role-mismatch: no — planned C++ tests and runtime use the same local toolchain
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approving the missing proof seams would accept residual correctness, collision, supply-chain, and benchmark risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `PLAN_LOCK_ID` with a fresh digest, parent the reissued PLAN to `engine-c1-design-review-4`, and set `IN_REPLY_TO` to this review. Close PR1–PR8 without weakening the DD, owner contracts, E2 target, benchmark predicates, or root-lint dispatch hold. If the clock seam remains deferred, route the design-level deviation before requesting PLAN approval.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/PLAN-REVIEW-pair-implementer-20260825-090632.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-090632`
- source/tests/plan/design=none — review-only; no source, test, plan, design, dependency, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/PLAN-REVIEW-pair-implementer-20260825-090632.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085542.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-REVIEW-pair-implementer-20260825-085947.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-085754.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-planner-20260825-090153.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-REVIEW-pair-implementer-20260825-085835.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-085328.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-planner-20260825-090146.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-085705.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-REVIEW-pair-implementer-20260825-090729.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/DESIGN-planner-20260825-090309.md
