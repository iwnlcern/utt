ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-solver-c3-plan-review-1
PARENT_DISPATCH_ID: s2-solver-c3-plan-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can repair the execution, test, scope, and escalation defects within the approved design; any triggered delegated-dispatch condition routes back to the orchestrator already in CC
FROM: solver.implementer
TO: solver.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/PLAN-pair-planner-20260829-023829.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
PLAN_LOCK_ID: PL-solver-c3-20260829
PLAN_ARTIFACT: PL-solver-c3-20260829
PLAN_SHA256: 9b4835d517cdd9a7550aedd2fc7e49d366639f2b40c7031be6e6c06ae178de13
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BASE: main@8bb972b
TARGET_BRANCH: main
BRANCH: solver/c3-proof-engine
SUBJECT: PLAN-REVIEW solver-c3 rev1 — must-revise build graph, locked tests, oracle bounds, and dispatch scan

## Scope and identity

Reviewed all 338 lines of `PL-solver-c3-20260829` at SHA-256 `9b4835d517cdd9a7550aedd2fc7e49d366639f2b40c7031be6e6c06ae178de13`, the addressed PLAN relay, approving revision-6 design review, locked design, orchestrator proceed relay, current engine build/test surfaces, AltSolver implementation, prior measured scale evidence, and daemon lineage. The plan and design digests match their relays, the PLAN correctly parents to the approving design review, and exact-file plus engine-root lint report no errors. No source, test, fixture, design, plan, branch, commit, PR, or external-run change was made.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The architecture, component fence, pinned theory identity, state/D4/claim contracts, packed formats, dual kernels, fixed spike seeds and rule, census schema, and separate merge gate are accepted and should be preserved. Implementation dispatch remains held because the written steps cannot produce the claimed test target as specified, defer the design's early census, omit several locked truth-boundary tests, prescribe an infeasible exact-oracle corpus, and assert no hard trigger without the required scan despite a named cross-pair consumer contract.

## Acceptance-criteria disposition

1. §4 spike pre-registration: PARTIAL — seeds, tiers, per-probe timeout, censoring, floor, and decision rule are pinned; global three-day/resume behavior and the report's in-scope durable location are not.
2. §5 durability/visibility and root exemption: FAIL — the facade queries only the durable log index, contradicting immediate in-process search visibility, and the terminal/easy-root checkpoint matrix is absent.
3. §5 collision safety and Kind-2 semantics: FAIL — forced tag collision is present, but the locked A→B replacement-interleaving test and exhaustive polarity/witness state matrix are absent.
4. §5 psl-v1 deterministic compaction: PASS as an approach — exact header, ordering, dedupe, contradiction, and byte-determinism checks are named; crash-safe publication coverage remains incomplete under criterion 5.
5. §6 checkpoint/recovery: FAIL — happy/fallback/schema cases are planned, but the required mid-compaction and mid-manifest-replace crash matrix and predecessor-retention proof are absent.
6. §8 census: PARTIAL — schema, anchors, digest, and deterministic output are named; it is sequenced late, has no executable resume contract, leaves arbitrary-width arithmetic unspecified, and names scratch outside the dispatched path set.
7. §10 oracle gates: FAIL — fixture and cross-kernel shapes are sound, but the AltSolver corpus is not executable and claims unconditional completion that the API does not provide.

## MR-1 — make the build graph and commit hygiene executable

Plan lines 62–78 promise `solver_core`, a doctest-main `solver_tests`, and a passing first configure, but the only listed test translation unit contains no `DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN`; the current repository obtains main from `engine/tests/test_smoke.cpp:1`, while the plan's new smoke file starts directly with the include. The CMake step creates only `uttt_rules_core` and a `solver_tests` executable containing `test_smoke.cpp`; it does not create `solver_core` or any tool target. Tasks 2–17 then create source and test files without listing `solver/CMakeLists.txt` as modified or adding those bytes to a target, so the final `ctest` claim would exercise only the smoke test.

The build command creates `solver/build`, the repository does not ignore that path (`git check-ignore solver/build/CMakeCache.txt` returned no match), and Task 1 immediately runs `git add solver/`; that stages generated build bytes.

Required revision:

1. Add an explicit doctest-main translation unit or macro and prove Task 1 can link.
2. Define when `solver_core` is created and update `solver/CMakeLists.txt` in every task that adds source, test, shell test, or tool bytes; list the exact target membership and `add_test` registrations.
3. Add `solver/.gitignore` for `build/` or build outside `solver/`, and replace broad staging commands with explicit source/artifact paths plus a staged-file fence.
4. Keep generated build/store/scratch bytes untracked while still allowing the governed deliverables.

## MR-2 — restore c3 sequencing and give long runs an in-scope lifecycle

The locked design §11 and orchestrator proceed relay sequence adapter + canonicalizer + the ply-10 census deliverable early, before the proof kernels; the plan places census at Task 15 after the complete storage stack, Kernel B, and solve CLI. Sharing external-sort machinery does not authorize deferring theory's R1 mitigation to the end of that work.

Global constraint line 24 says every census ply ≥9 and spike probe is checkpointed/resumable, but Tasks 15 and 17 define no checkpoint artifact, resume option, recovery rule, or interruption test. The census promises unbounded decimal integers without naming an arbitrary-precision accumulator. The spike writes `spike-report.json` at an unspecified path even though the relay's in-scope paths are `solver/**`, sprint `census/**`, the plan, and relays; the census command likewise leaves `<internal-drive dir>` outside the mechanical scope list. A per-run two-hour timeout does not itself enforce the locked approximately three-day aggregate timebox across primary plus fallback.

Required revision:

1. Reorder the census immediately after adapter/canonicalizer and the minimum reusable digest/external-sort primitives, then run and relay ply 10 before kernel work proceeds.
2. Specify the arbitrary-width counter implementation and test values beyond 64 bits.
3. Define checkpoint/resume artifacts and crash/resume tests for both census and spike, including a process-wide three-day deadline whose remaining budget survives resume.
4. Put the spike report and every persistent scratch/store path under an explicitly dispatched path, or return the proposed additions to the orchestrator before SCOPE_DIFF.

## MR-3 — close the storage/TT truth boundaries with the locked tests

Plan line 225 says `ProvenStore::lookup` reads layers then the **durable** log index. DD §5 requires a qualifying appended fact to become search-visible immediately in the in-process batch while staying checkpoint/extraction-invisible until `fdatasync`. The Task 8 counters do not give Task 11 a lookup path, and no test proves append-before-commit is queryable but absent from a checkpoint.

The reduced test list also omits locked DD §10 cases:

- the same-tag A→B replacement paused between sidecar/payload updates, with probes for both identities proving matched-generation hit or miss;
- all Kind-2 true/false query results at prover-to-move, opponent-to-move, and terminal states, with witness legality independent of query polarity;
- K−1/K/K+1 plus terminal/easy-root and K-greater-than-proof-cost checkpoint/resume cases;
- kill mid-batch, mid-compaction, and mid-manifest replacement, proving no durable fact lost or fabricated and predecessor files retained until successor manifest and CURRENT are durable;
- the integrated ply-0 compaction/probe/resume path and psc-v1 pointer refusal matrix.

Task 7 additionally describes canonical state and witness rules as MUSTs but gives `pack` no error path and tests only numeric bands. A durable-record ingress must reject or make invalid canonicality, state well-formedness, and opponent/terminal witness combinations inexpressible.

Required revision: add an active-batch search index to the facade contract; make checkpoint flush/order explicit; add the full locked cases above at the correct API/integration layers; and include record-ingress state/canonical/witness validation. Preserve c4-only TSAN/threaded stress as out of scope.

## MR-4 — replace the impossible AltSolver corpus with a bounded exact gate

Task 13 says 500 random positions at plies 10–30 with `max_depth = empties` are “always complete+Exact”; Task 16 repeats 200. `AltSolver::dfs` may return `complete=false` on node cap or stop and exhaustively iterates every child without alpha-beta cutoff (`engine/src/root/alt_solver.hpp:145–180`). Setting depth to remaining empties ensures terminal-grounded `Quality::Exact` **if the run completes**; it does not ensure completion.

The audited center-root depth-7 run already retained 3,934,422 entries, took 522.9 MiB, and the evidence-only fit placed depth 9 around 41 GiB (`AUDIT-pair-implementer-20260828-003733.md:86–104`). A random ply-10 state can have dozens of moves remaining, so this plan can hang or exhaust the machine before the first TDD gate.

Required revision: define a deterministic near-terminal or otherwise demonstrably bounded corpus; set node/time/memory caps; compare only returned `complete && Quality::Exact` rows; report incomplete/out-of-resource rows separately; and set a predeclared minimum completed-exact count. Keep the API-shape exclusion of estimates and the cross-kernel completed-pair rule.

## MR-5 — complete the hard-trigger scan before delegated dispatch

The PLAN relay declares “no hard trigger” but carries no completed `ESCALATION_SCAN`. Its boundary writes the theory-consumed census contract and reads a pinned theory-owned identity/fixture contract; the orchestrator proceed relay explicitly CCs `theory.planner` and routes seam friction through the orchestrator. This is at least a shared cross-pair contract trigger, not an all-no scan.

Required revision: carry the complete scan in the successor PLAN relay. Because delegated authority is conditioned on “no hard trigger,” do not issue a pair-Planner implementation token if the scan remains trigger-present; route the reviewed plan back to `s2.orchestrator-planner` for an explicit direct dispatch or condition amendment. No schema change or scope widening may be inferred from the existing proceed relay.

## Required successor

Amend `PL-solver-c3-20260829` forward without changing `PLAN_LOCK_ID`; preserve the accepted architecture, design digest, fixed spike seeds/rule, source fence, and merge hold; float `PLAN_SHA256`; reissue `PHASE: PLAN` with `DISPATCH_ID: s2-solver-c3-plan-2`, parented to the same approving design review and `IN_REPLY_TO` this review. Do not run SCOPE_DIFF or issue implementation dispatch on this verdict.

OPERATOR_ITEMS: none — the revision is design-preserving; the known cross-pair trigger is for the orchestrator to route under its existing governance, not a fresh product-semantics decision

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local game-solver code and artifacts
- migration/backfill/destructive-write/canonical-data-repair: no — immutable local proof-store formats and disposable scratch only
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts, no operational ledger
- AI-or-automation-acts-downstream: no — no external actuator or user action
- worker/scheduler/queue/retry/async-side-effect: no — c3 is explicitly single-threaded; foreground crash recovery is local file processing
- cross-repo/service-contract/generated-schema/shared-API-event: yes — census output is consumed by theory and solver consumes a theory-owned identity/fixture contract
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control
- test-runtime-role-mismatch: yes — as written, ctest does not compile or register Tasks 2–17 and the oracle assumes completion the runtime cannot guarantee
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — spike/scratch artifact locations are outside or absent from the dispatched path list, and accepting the omitted locked tests would accept residual risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or residual-risk acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: incoming PLAN relay exact-file lint passed; its PLAN_SHA256 exactly matches the current 338-line plan.
- E1: engine-root lint reports zero errors; its only warning is the informational daemon sweep of record-known relays not re-judged.
- E1: daemon was ready and active with zero conflicts and zero pending renders before filing.
- E1: current engine doctest main is supplied by `engine/tests/test_smoke.cpp:1`; the planned new smoke translation unit omits it, and the planned CMake target contains no alternative main.
- E1: no ignore rule covers `solver/build`, while Task 1 stages all of `solver/` after configuring there.
- E1: plan-to-design comparison found the locked replacement-interleaving, polarity/witness, root-status, and crash-publication cases absent.
- E1: AltSolver source and the prior depth-7 measurement refute unconditional completion of the named random corpus.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/external-run changes claimed; authored only this PLAN-REVIEW relay for daemon filing; concurrent/untracked relay, plan, and scheduler bytes remain attributed by final status
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/PLAN-pair-planner-20260829-023829.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/MERGE-GATE-pair-planner-20260829-033317.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-solver-c3-20260829.md
