ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-solver-c3-plan-1
PARENT_DISPATCH_ID: s2-solver-c2-design-review-6
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan gate is the Implementer's PLAN-REVIEW; merge remains the standing operator/orchestrator gate
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-orchestrator-planner-20260829-022827.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: e5cd2ae3113e0f158480571e21b99e3da87ac36c7a8eda8db6cf870f9b1502c2
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 9b4835d517cdd9a7550aedd2fc7e49d366639f2b40c7031be6e6c06ae178de13
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN s2-solver-c3-plan-1 — c3 proof-engine slice; PLAN-REVIEW requested

## Team solver — solver-c3: PLAN + PLAN-REVIEW REQUEST

Current scope for the `TO` addressee: read-only PLAN-REVIEW of the locked plan below. No source/test edits, no branches, no commits, no PRs until a live dispatch token exists.

Locked design: DD-solver-c2-20260828 rev 6 @ sha256 above, committed at main@8bb972b; approving review s2-solver-c2-design-review-6 (this relay's parent). Grill decisions ride the doc's GRILL_LOCK.
Plan of record: docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md @ sha256 above — 17 TDD tasks with file/function targets, exact interfaces, test code, commands, and commit points.

Scope (locked, = proceed relay boundary = DD §11 c3):
- solver/ CMake project + uttt_rules_core reuse target + vendored xxHash (T1).
- Claim lattice, SolverState adapter (37-byte identity, parity-derived apply), theory-fixture conformance, d4-lexmin-v1 canonicalizer (T2–T5).
- Storage stack exactly per DD §5–6: digests, psr-v1 codec, ProvenLog with durable-visibility split and salvage, psl-v1 deterministic compaction + claim-lattice refusal, psm-v1/psc-v1 manifests with fail-closed recovery/GC, ProvenStore facade with K policy + root exemption (T6–T11).
- tte-v1 working TT: stripe-locked entry+sidecar, derivation-table exact hits, forced-collision tests (T12).
- Kernel B negamax + AltSolver exact-corpus gate; solve CLI with crash-safe checkpoint/resume e2e (T13–T14).
- solver-census-v1 tool, audited anchors, ply-10 deliverable run committed + pointer relayed (O5) (T15).
- Kernel A df-pn + shared gates; spike harness + pre-registered A-vs-B execution at exit (~3-day timebox, primary/fallback tiers, decision rule verbatim) (T16–T17).

In-scope file/dir list (for SCOPE_DIFF at dispatch): solver/** (new tree, incl. solver/build untracked), docs/sprints/active/2026-08-28-uttt-weak-solve/census/** (new census artifact), docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md (plan-of-record edits only via reissue), plus IMPL-report relay filings under .relays/s2. Nothing else — engine/**, theory/**, ui/** are OUT.

Out of scope: engine/theory edits; certificate emission (c5); threads/soak/sharding (c4); RULE leaves; cloud runs; constant retuning outside flags.

Boundary contract: Writes: solver/ tree, census CSV (consumer: theory proof-size estimation, format solver-census-v1 per DD §8), spike report. Reads: DD-theory-c2-20260828 @ 629801c4… (identity/D4/claims), theory fixtures v1. Target entity: green c3 suites + census artifact + kernel decision. Downstream consumer: theory (census), c4/c5 cycles (engine). Contract: DD §5/§6/§8 formats verbatim. Proof: E2 (ctest suites + anchor reproduction + e2e crash tests + spike report). No-consumer action: not applicable.

Acceptance criteria: the seven testable criteria in the plan's "Acceptance criteria" section, carried verbatim from the DD design-lock impact line (spike pre-registration, log-before-claim, root exemption, collision safety, deterministic compaction, manifest recovery + durability gate, census anchors, oracle gates). Verification target: E2. Anti-half-fix guards: plan "Out of scope" section; engine suite must stay green as the no-engine-edits tripwire.

Ceremony downgrade: none (large tier retained). Operator-judgment items: none.

Delegated conditional dispatch: the proceed relay (s2-solver-c2-proceed-1) grants DELEGATED_DISPATCH_AUTHORITY: yes with conditions (Implementer plan review = approve; dispatch parents to that approve; that review parents to this PLAN; SCOPE_DIFF all-in; no hard trigger; no boundary deviation; no cross-bundle collision). On your approve, I will run the mechanical SCOPE_DIFF against the in-scope list above and, if all-in, issue the dispatch token in a separate relay.

Requested response: PHASE: PLAN-REVIEW, FROM: solver.implementer, TO: solver.planner, PARENT_DISPATCH_ID: s2-solver-c3-plan-1, same PLAN_LOCK_ID, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plans/PL-solver-c3-20260829.md authored by this seat, untracked at sha256 9b4835d517cdd9a7550aedd2fc7e49d366639f2b40c7031be6e6c06ae178de13; no other edits, commits, branches, or PRs; git status --short below
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
?? docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
