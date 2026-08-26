ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-plan-review-3
PARENT_DISPATCH_ID: engine-c1-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the remaining blockers are technical plan corrections; BASE revalidation routes to the orchestrator under the existing authority chain, not to the operator
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: engine-c1-plan-2/PLAN-planner-20260825-093449.md
DESIGN_LOCK_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 265773e3a98adcd7f8e297e9ca9fc394581ba2506bd660e248b000320ea03f47
PLAN_LOCK_ID: PL-engine-rules-c1-20260825
PLAN_ARTIFACT: PL-engine-rules-c1-20260825
PLAN_SHA256: 725519a7ed2e489c53c6e8a816cc413a1db90d22db272f840ae76e2643fadd9f
PLAN_REVIEW_VERDICT: must-revise
SUBJECT: PLAN-REVIEW must-revise — rev 2 still self-blocks its base and under-proves terminal, Zobrist, harness, and benchmark seams

## Verdict

Revision 2 preserves the locked design identity and closes substantial portions of PR2–PR4, but it is not implementation-ready at this digest.
Five blocking correction groups remain: the exact-base preflight is already deterministically false; literal task bytes do not compile or cover all-closed totality as written; the Zobrist tests do not isolate the declared inputs; the harness adapter/corpus path is not strict or round-trip complete; and the benchmark still commits its baseline only after candidate acceptance.

This verdict is review-only.
It authorizes no source/test/plan/design edit by this seat, dependency download, branch creation or movement, commit, PR, implementation, delegated dispatch, merge, publication, deployment, or release.

## Must-revise findings

### MR1 — PR1's exact-base preflight is self-invalid and its staged-set proof is not mechanical

The plan requires `git rev-parse main == 63b4b7b` at IMPL start and says to stop otherwise, but the plan's own filing commit has already advanced `main` to `878c6bcb19a71dd170bd60cc7fdce69eb8aaf9a4`.
The intervening range `63b4b7b..main` currently changes sprint/relay documents and no `engine/` path, so branching from the dispatched exact commit may still be technically possible, but the written equality precondition cannot pass now.
The commit discipline also calls `git status --short` a staged-set assertion and uses directory operands such as `engine/third_party` and `engine/src`; that does not mechanically enumerate the staged bytes requested by PR1.

Required revision: either obtain an orchestrator successor BASE or retain `63b4b7b` and replace the `main`-equality test with exact-object plus engine-surface ancestry/diff checks that prove the old base remains safe.
For every commit, stage the exact named files and assert the cached path set directly (for example, `git diff --cached --name-only` plus an explicit refusal for `engine/build/`), not merely whole directories and a general status view.

Evidence: E1 — plan lines 29–30 and 140–145; `git rev-parse main` = `878c6bcb19a71dd170bd60cc7fdce69eb8aaf9a4`; `git diff --name-status 63b4b7b..main` contains no `engine/` path.

### MR2 — Literal Task 1/2 bytes are internally inconsistent, and criterion 2 still lacks all-closed totality

Task 1's CMake snippet says doctest is an unpinned `2.4.x` and that nlohmann/json is vendored in Task 10, contradicting the exact v2.4.12/v3.12.0 Task 1 pins.
Task 2's test includes only `core/types.hpp` but immediately names `winner_on_chips`, `ChipResult`, and `FakeClock`, which are declared in `budget.hpp` and `clock.hpp`; the prescribed failing/passing sequence therefore cannot compile as written after adding only `types.hpp`.
The only total-terminal test in Task 5 covers `MacroWinX`.
The locked design requires named `legal_moves = empty` and apply-rejection coverage for BOTH macro-win and all-closed terminal kinds, while criterion 2 must be executable without owner fixtures.

Required revision: correct the CMake pin comment; include and wire the two seam headers in the literal Task 2 test; and add a mechanical all-closed Position fixture with named `TerminalKind::AllClosed`, empty movegen, and `TerminalParent` apply-rejection assertions independent of theory artifacts.

Evidence: E1 — plan lines 20, 90–102, 202–228, 472–503, 788–799; design lines 70–71, 104, and 184.

### MR3 — PR5's sensitivity battery changes multiple inputs and does not exhaust the secondary tag

The 162 cell/seat loop compares each marked Position against an empty `NullFirstMove` Position, while each marked Position also changes the tie state; those comparisons do not prove that changing the cell/seat input alone changes the key.
For the secondary tag the loop checks only X-marked versus O-marked Positions, again changing both cell owner and tie, and never compares either against a same-tie baseline.
The ten forced states are pairwise checked only for the primary key; the secondary tag receives one forced-state spot check.
The three tie comparisons use different board occupancy for `NullFirstMove` versus X/O, so only X versus O isolates tie.

Required revision: construct comparisons that differ in exactly one declared identity input, or test the fixed table entries directly.
Exercise both the 64-bit key and independent 32-bit tag across every 162 cell/seat entry, all ten forced entries, and all three tie entries; retain incremental-primary versus full recomputation across random games.

Evidence: E1 — plan lines 564–629; design lines 76–79 and 106; prior PR5 required the full declared population rather than spot checks.

### MR4 — PR6/PR8 still do not consume the pinned harness contract strictly or prove corpus round-trip

The owner contract requires every missing or type-invalid required key to fail closed.
Its `game_end` message has required `result`, enum `reason`, and canonical X/O `budgets`, but the plan's `validate_game_end` checks only type/protocol and explicitly tolerates the payload.
The plan also omits the protocol's 32 KiB input-line ceiling from the adapter validation path.
The proposed corpus test merely parses every referee-to-engine line and expects it to parse clean; it does not run the normative transcript through the engine, validate emitted hello/turn replies, or establish the design criterion's corpus round-trip.
Task 13 can therefore label criterion 3 `green-E2` on evidence weaker than the locked claim.

Required revision: validate all required `game_end` fields and enums while continuing to ignore unknown keys; enforce the 32 KiB input limit; and drive the harness-owned normative transcript through the binary with owner-schema validation of both incoming and emitted lines.
Keep criterion 3 pending-blocked when the corpus is absent, and permit `green-E2` only when that full round-trip plus stdout discipline passes.

Evidence: E1 — plan lines 703–746 and 751–799; harness design lines 22–30 and 70–75; engine design lines 83–96 and 181–186.

### MR5 — PR7 still commits the reference baseline after candidate acceptance

The single-process sequence writes `baseline.json`, measures the candidate, decides PASS, and only then commits the baseline together with the PASS verdict.
That closes same-session identity and candidate-write isolation better than revision 1, but it does not satisfy the accepted PR7 correction that the reference baseline be durably committed before candidate acceptance.
If the candidate fails, the named reference artifact is never committed, so the evidence needed to diagnose and compare the failure is lost from the append-only implementation history.

Required revision: pin a reference-first sequence that publishes and commits the reference artifact before the candidate gate, then resumes or launches candidate verification against that immutable artifact while preserving the DD's same-session identity/environment proof.
The candidate path must remain unable to write the baseline, and any identity mismatch must still refuse before measurement.

Evidence: E1 — plan lines 778–784; design lines 41–45 and 186; prior PR7's baseline-before-candidate ordering requirement.

## Closed or retained portions

- Design/plan identities, locators, and both declared SHA-256 values match the current artifacts.
- PR2's bit order, literal local-board masks, pinned macro-win sequence, final `RefPosition`, and `from_parts` fixture seam are now concrete and internally coherent.
- PR3's exact dependency versions, URLs, digests, license gate, Task 1 ordering, and production-core JSON boundary are materially closed apart from the stale CMake comment above.
- PR4's `RootContext`, pure chip-result seam, and injected clock are restored to scope, subject to the literal include/test correction above.
- The 13-task decomposition, independent oracle/table proof, value-copy immutability, both-mover perft shape, owner-routing lines, anti-tautology rules, and search-math exclusion remain sound.

## Standing dispatch gate, independent of this verdict

The daemon is `ready`, epoch `active`, and `pending_renders = 0`, but `relay verify` remains `ok: false` with one recorded `INDEX.md` divergence/conflict.
The cutover relay makes standalone INDEX inversion lint obsolete, but the delegated-dispatch condition expressly requires no unreconciled daemon divergence.
No delegated dispatch is available while that verify result remains red, even after a future plan approval.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local read-only PLAN review
- migration/backfill/destructive-write/canonical-data-repair: no — no implementation or data action
- money/inventory/orders/planning/accounting/trust-critical-state: no — game chips only
- AI-or-automation-acts-downstream: no — no implementation dispatch issued
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation in review
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes the harness protocol and theory fixture contracts
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control changed
- test-runtime-role-mismatch: no — planned C++ tests and runtime share the local toolchain
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — approval would accept unresolved correctness and benchmark-evidence gaps
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `PLAN_LOCK_ID` with a fresh digest and `IN_REPLY_TO` this review.
Close MR1–MR5 without weakening the locked design, pinned owner contracts, criterion-level E2 truth table, or benchmark predicate.
Route any BASE change through s1.orchestrator-planner; the pair Planner must not silently float the dispatched base.

ACTIONS_GIT_REF:
- review-draft=`.engine/drafts/engine.implementer/PLAN-REVIEW-pair-implementer-20260825-094440.md` — daemon admission pending at authoring
- source/tests/plan/design=none — review-only; no source, test, plan, design, dependency, branch, commit, PR, merge, publication, deployment, or release action by this seat

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/SEATS.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-ui-c1-20260825.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/SITREP-planner-20260825-092831.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/s1-daemon/RECONCILE-orchestrator-reviewer-20260825-092910.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/s1-daemon/SITREP-pair-implementer-20260825-092838.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1-design-3/

