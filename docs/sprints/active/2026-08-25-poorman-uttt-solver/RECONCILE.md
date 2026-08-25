# RECONCILE — s1

Durable reconciliation projection for RUN_ID s1.
Sections are appended by PHASE: RECONCILE relays; each relay cites the exact section it appends.

## Section c1-audits — reconciliation of the four paired AUDIT cycles (2026-08-25)

Inputs: eight independent audit returns (theory/engine/harness/ui × planner/implementer), relay paths under `.relays/s1/<pair>-c1/`.
All eight: PRIMARY_BUCKET still-open; local greenfield confirmed (E1/E2).
External duplicate absence is E0: no known drop-in was found in the bounded multi-angle sweeps (which surfaced only Richman-payment or alternating-turn artifacts); absence is not provable, and the named prior-art overlap edges are preserved (corrected per reviewer correction 4).
Agreement between paired seats was high everywhere; divergences were complementary coverage, not contradiction.

### Operator rulings obtained (2026-08-25, this session)

R1. First-move tie owner is NOT disclosed to engines; `h` accepts `null` (move 1 only); coin consulted only if a first-move tie occurs. Theory owns `T(s, null)`.
R2. Fault rule: faulted submission → opponent wins the auction at no cost; double-fault → re-request up to 3 attempts; 3 double-faults → game killed/voided.
R3. Bid domain: integer units, 0 ≤ bid ≤ budget inclusive; zero and all-in legal.
R4. Numeric policy: doubles in engine, integers in referee/wire, exact rationals only in theory's Stage-1 oracle.
Correction (reviewer correction 1): R4 selects numeric types only.
It does not define the authoritative classification at representable p = T, nor an approximation tolerance / exact-discrete fallback contract; those remain owned theory DESIGN/Stage-1 obligations with the human-decision flag preserved — theory proposes, the operator ratifies at the math lock.

All four rulings are baked into the design spec (commit follows this section) — evidence: spec canonical rules 3, 4 and "Open math questions" section.

### Disposition ledger — theory-c1

- T-P claims 1–5 (backup formula confirmed for token model; critical bid re-derived; zugzwang/tie-owner/terminal not covered by literature) → owned obligation: theory DESIGN (findings doc states cited claims + own proofs; Stage-1 proves the uncovered three) — evidence checked: both returns' citations cross-agree (Avni–Henzinger survey Thm 4, arXiv:1804.04372, Lazarus §3, 968-case Fraction identity check).
- T-P edge cases 1–15 and T-I fixture list (converge, T-I adds h-persistence and knife-edge scale cases) → owned obligation: theory DESIGN fixture schema v1 — evidence: both lists compared line-by-line; union adopted.
- T-I F5 (h = opponent(actual mover), not unconditional toggle) → owned obligation: theory fixtures + a spec-reading note in the findings doc — evidence: re-derived against spec rule 4; T-I derivation correct.
- T-I F6 (regret matching needs averaged-strategy + exploitability contract; LP as reference oracle) → overlap edge to engine DESIGN (root bid matrix), theory supplies the LP reference — evidence: Zinkevich et al. Thm 2 citation.
- T-I operator items 1–2 (p=T semantics, continuous tolerance) → owned obligation: theory DESIGN/Stage-1 defines the equality classification and the tolerance + exact-discrete fallback contract; human-decision flag preserved (operator ratifies theory's proposal at the math lock) — evidence: none — reclassified per reviewer correction 1; R1/R4 do not cover these items.
- T-P operator item 1 (bid bounds) → verified closure via R3 — evidence: ruling above.
- T-P/T-I duplicate gates (dm0n3y/biddingTTT Richman oracle, Avni 2023 ECAI, Kalai 101.84 reproduction target, nelhage/ultimattt) → owned obligation: theory DESIGN lists cross-check oracles (reference-only; no license = no reuse) — evidence: URLs in returns; license check noted per artifact.
- T-P risk (fixed-point non-uniqueness precedent) → owned obligation: Stage-1 computes by backward induction on the finite DAG, flags any non-forced state — evidence: charging-paper precedent cited in return.
- Toolchain (Python 3.14.6/numpy/pytest E2) → verified closure — evidence: command output in both returns.

### Disposition ledger — engine-c1

- E-P/E-I toolchain (Homebrew LLVM 22.1.8 primary, `-std=c++2c` real, `std::simd` absent, `<inplace_vector>` absent, describe as C++23-library + partial C++26) → verified closure; pin in engine DESIGN — evidence: E2 probes in both returns agree.
- E-P/E-I convergent caveat: interval/bound cutoff math is likely CORE, not a later optimization (backup evaluates ~2× legal moves per node) → owned obligation: engine DESIGN treats cutoff/bound derivation as first-class; monotonicity of F(a,b) is a theory findings-doc deliverable — evidence: independent identical derivations in both returns.
- E-I TT findings (entry needs depth/bound-type/completeness/two conditional moves; 64-bit Zobrist collision policy explicit; selective-widening soundness contract) → owned obligation: engine DESIGN — evidence: minimax.dev TT doc + TheGustafson source cited.
- E-P/E-I first-move coin flag → verified closure via R1 (null tie-state sentinel; root case owned by theory) — evidence: ruling above.
- E-I operator item 2 (what analysis may claim under incomplete widening) → owned obligation: engine DESIGN defines value-quality metadata (exact/bound/estimate), consumed by harness log + ui — evidence: routed to harness DESIGN item 6 below.
- E-I operator item 3 (license gate on Apache-2.0/MIT prior art) → explicit rejection of code reuse for s1; reference-study only; any future adaptation needs a fresh orchestrator/operator authorization — evidence: standing dispatch rule.
- E-P representation questions 1–10 / E-I questions 1–9 → owned obligation: engine DESIGN agenda (merged list) — evidence: lists compared; complementary.
- E-P fixture-format-early request → owned obligation: theory DESIGN publishes fixture schema first; engine consumes — evidence: cross-pair sequencing set in DESIGN dispatches.

### Disposition ledger — harness-c1

- H-P protocol questions 1–15 / H-I sections 1–8 (framing, lifecycle, handshake, timeout, faults, seat/perspective, coin/tie on wire, log schema) → owned obligation: harness DESIGN protocol spec v1 + game-log schema v1; rulings R1–R3 are fixed inputs — evidence: both lists merged; no contradictions (H-P favored minimal hello, H-I favored hello handshake — DESIGN decides with afternoon-bar as tiebreak).
- H-P/H-I fault-policy recommendations → superseded by R2 (operator chose opponent-wins-free + 3×-retry instead of forfeit) — evidence: ruling above; both seats' fail-closed validation-before-comparison machinery remains required.
- H-I operator item 1 (tie-owner disclosure) → verified closure via R1 (hidden; null sentinel) — evidence: ruling above.
- H-I operator items 2–3 (double-fault result; illegal losing intent) → verified closure via R2 — evidence: ruling above.
- H-P afternoon-bar audit (flush requirement, seat identity, fault semantics, framing are the gaps; reference bot + conformance fixtures close it) → owned obligation: harness DESIGN acceptance criteria — evidence: convergent with H-I's "three message shapes" recommendation.
- H-P/H-I prior-art gates (cutechess/fastchess/CodinGame/socialgorithm/Battlesnake/OpenSpiel: imitate patterns — concurrency, paired swaps, time margin, legal-action list, collect-resolve-broadcast; reuse nothing) → verified closure of duplicate gate; pattern list feeds DESIGN — evidence: URLs verified in returns.
- H-P overlap edge (OpenSpiel UTTT + dm0n3y as theory oracles) → routed to theory DESIGN — evidence: theory returns already list them.
- Toolchain divergence: H-P saw Python 3.14.6 (Homebrew), H-I saw 3.12.5 (Framework build) — different PATHs across sessions → owned obligation: harness PLAN pins interpreter + uv-managed env — evidence: both command outputs quoted; both viable.
- H-P scope gate (SPRT etc. rejected from v1) → explicit rejection, revisit only on operator ask — evidence: return recommendation adopted.

### Disposition ledger — ui-c1

- U-P Finding 1 / U-I prior-art judgment (inspiration-only CONFIRMED, file:line evidence; MIT license so bar is technical not legal) → verified closure — evidence: convergent independent file:line audits of 0c05c6f.
- U-P Finding 2 needs 1–8 / U-I bridge+log contract needs (analysis carrier absent from `{bid, move}`; browser bridge transport; sealed lifecycle; log schema sufficiency; fixture-convention alignment) → owned obligation: forwarded into harness DESIGN as named consumer requirements (this reconciliation is the routing) — evidence: ui needs lists merged; no contradictions.
- U-P/U-I UX operator items (live-analysis policy, sealed-bid flow, reveal policy, bid control, replay persistence, metric perspective, human clock) → deferred obligation: batched operator Q&A at ui DESIGN time (ui is gated on protocol lock; asking now would front-run the contract) — evidence: none — deferral is a sequencing decision.
- U-I UI-A06 surface inventory + accessibility requirements → owned obligation: ui DESIGN artifact (when dispatched) — evidence: return section retained by reference.
- U-P/U-I toolchain (node 25.2.1, vite 8.2.2 viable; pin at PLAN) → verified closure — evidence: E2 command output both returns.
- U-P risk (analysis-channel deferral would silently degrade analysis view) → owned obligation: harness DESIGN must resolve the analysis carrier pre-lock (named acceptance criterion) — evidence: routed below.

### Cross-cutting sequencing set by this reconciliation

1. theory DESIGN and harness DESIGN dispatch now, in parallel (disjoint surfaces; the shared object — fixture schema conventions on coordinates/forced/ANY — is named in both dispatches with theory as owner).
2. engine DESIGN dispatches now, scoped to rules core + representation + adapter boundary + test seam; search-math sections stay gated on theory findings.
3. ui DESIGN waits for the harness protocol lock; ui's contract needs travel inside the harness dispatch.
4. INDEX inversions from the boot storm (rows 17/19) and one audit-wave inversion (row 31) stay registered, not repaired (scoped per reviewer correction 3).
   Design work and non-authority relays may continue, but a red relay-root lint blocks delegated dispatch, merge, and automated adapter consumption: before any pair-planner delegated DISPATCH IMPL or merge grant, the INDEX must lint green or carry an operator-ratified monotonic-from marker or explicit operator waiver.
   Marker insertion is operator-only; no seat self-inserts.
