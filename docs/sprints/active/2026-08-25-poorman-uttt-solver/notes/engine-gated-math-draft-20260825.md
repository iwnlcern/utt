# WORKING NOTE — engine gated-math draft (NOT a design record)

This file is a parked, non-normative draft.
It is not a DESIGN_DOC_ID, carries no lock, grants no authority, and binds no reviewer.
Its content feeds the successor engine search-math DESIGN_DOC_ID authored after theory's findings doc and Stage-1 solver land (orchestrator amendment `.relays/s1/engine-c1/DESIGN-orchestrator-planner-20260825-073330.md`, split option a).
Origin: gated sections 10–13 of the withdrawn single-document draft DD-engine-c1-20260825 (engine.planner, 2026-08-25).

## Draft — backup operator implementation

Per spec: `a = min over X-marked children of T`, `b = max over O-marked children of T`; if `a ≤ b`, `T = b / (1 - a + b)` and critical bid fraction `r = (b - a) / (1 - a + b)`; if `a > b`, zugzwang: `T = a` if tie owner is X else `b`.
Every interior node evaluates up to 2× the legal-move count of marked children (both c1 audits, independently).
`T(s, NULL_FIRST_MOVE)` (root, move 1, 50/50 coin) is theory-owned (R1); the engine implements whatever theory's findings doc specifies.
Locks only on theory's findings confirming the operator and zugzwang handling for this variant's chip-leader-draw terminal.

## Draft — cutoff and bound math (first-class deliverable of the successor DD)

Both c1 audits concluded bound propagation is CORE to useful pruning, not a later optimization.
Direction: `F(a, b) = b / (1 - a + b)` is monotone nondecreasing in both arguments on [0,1]² (theory findings must state and prove the exact claim); a running bound on `a` (min over X children so far) and on `b` (max over O children so far) gives sound bounds on T, enabling early subtree stop once a child's interval cannot change the parent's decision, ordering-driven narrowing, and an alpha-beta-analog window in T-space.
Successor-DD deliverables: the pruning rule statement, its soundness argument (citing theory's monotonicity), and the interaction with zugzwang branch selection.

## Draft — TT entry contract

Entry: 32-bit verify tag; value payload (double T, or [lo, hi] interval — decided with the cutoff math); bound type {exact, lower, upper}; searched depth/horizon; completeness flag (full-width vs widened); both conditional best moves (best X-marked child, best O-marked child — the ui's if-X-wins / if-O-wins needs both); generation/age.
Replacement: bucketed (4-way), depth-and-age aware; sizing configurable, default at PLAN after profiling.
Locks with the cutoff math because value semantics depend on it.

## Draft — selective widening

At free-choice nodes: order by tactical masks (rules DD section 3) plus routing quality; search a schedule of top-k both-player candidates with guaranteed minimum coverage; tag any node whose move set was not exhausted.
A widened node's value can never be reported `exact` (quality degrades to `estimate`, or `bound` when the searched subset yields a sound one-sided bound) — the metadata SHAPE is already locked in DD-engine-rules-c1-20260825 section 10.
