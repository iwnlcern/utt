ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: engine-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design phase; operator rulings R1-R4 already obtained and baked into the spec
GRILL_REQUIRED: yes
FROM: s1.orchestrator-planner
TO: engine.planner
CC: engine.implementer, s1.orchestrator-reviewer
SUBJECT: DESIGN dispatch — engine domain, cycle engine-c1

## Context

Reconciliation of the paired c1 audits is filed: RECONCILE.md section "c1-audits" (main@a3f250c).
Operator rulings R1-R4 are baked into the design spec (docs/superpowers/specs/2026-08-25-poorman-uttt-solver-design.md, same commit) — they are fixed inputs, not design questions.
Your pair's audit returns in this cycle directory are the finding inventory; their dispositions are in the RECONCILE section.

## Scope — engine DESIGN (rules core + representation; search math stays theory-gated)

Design now (Superpowers brainstorming owns the how):
1. Rules core: Position physical layout decided by sizeof + make/copy/movegen microbenchmarks, not intuition (candidates: 9x uint16 per player, packed rows, __uint128_t pair); cached macro masks; 19,683-entry local table with semantic entries (closed/result/empties/immediate-wins/forks/threat masks); move encoding; forced in {0..8, ANY}; tie-state in {X, O, NULL_FIRST_MOVE} per ruling R1; terminal detection ordering (macro win before draw accounting).
2. Test seam: consume theory's fixture schema verbatim (they publish the schema section early — request it via the orchestrator if not landed when you need it); property tests; perft-style move counts; deterministic clock injection.
3. Protocol adapter boundary: separate translation unit, no JSON dependency in the search path; consumes harness schema as-is including R2 fault/retry semantics (a re-requested auction must be handled statelessly).
4. Toolchain lock: Homebrew LLVM 22.1.8 primary, -std=c++2c, feature-tested library facilities only (std::simd and <inplace_vector> absent — NEON intrinsics or uint64 bit-ops if profiling ever justifies SIMD); CMake + ninja; single-threaded deterministic search first.
Design as gated sections (drafted but locked only after theory findings): backup operator implementation, cutoff/bound math — both your audits independently concluded interval/bound propagation is CORE to useful pruning, so treat the cutoff derivation as a first-class deliverable consuming theory's F(a,b) monotonicity statement; TT entry contract (depth, bound-type, completeness/widening status, both conditional moves, collision policy for 64-bit Zobrist); selective-widening soundness contract with value-quality metadata (exact/bound/estimate) that harness's analysis carrier and the ui will consume; numeric policy per ruling R4 (doubles canonical; root explores ±1-2 units around the rounded bid).
License gate stands: nelhage/ultimattt and TheGustafson are reference-study only; no code reuse without fresh authorization.

## Process

GRILL_REQUIRED: yes — run the design grill before lock (new-feature still-open at medium tier; cross-domain boundary contracts; hard-to-reverse contract decisions).
Author the design doc at docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-c1-<YYYYMMDD>.md with a DESIGN_DOC_ID.
Address your design-review request TO engine.implementer (orchestrator on CC only); on DESIGN_REVIEW_VERDICT: approve, report design completion to s1.orchestrator-planner and await PROCEED-TO-PLAN.
Boundary-contract changes route through s1.orchestrator-planner.

## Not in scope

Implementation, scaffolding beyond the design doc, plan authoring, or any relaxation of the theory/harness gates named above.

FINAL_GIT_STATUS_SHORT: unavailable — dispatch relay authored at filing time between commits; no work actions claimed by this relay
