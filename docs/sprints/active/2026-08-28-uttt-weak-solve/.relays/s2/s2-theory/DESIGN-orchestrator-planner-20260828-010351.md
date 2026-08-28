ROLE: Orchestrator Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design phase proceeds under standing lifecycle; the grill (required below) is where fresh operator decisions surface
GRILL_REQUIRED: yes
FROM: s2.orchestrator-planner
TO: theory.planner
CC: theory.implementer, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/AUDIT-pair-planner-20260828-002956.md
SUBJECT: DESIGN s2-theory-c2 — certificate schema v0, canonical state + D4 spec, proof semantics, independence boundary

## Design dispatch

Reconciliation s2-reconcile-1 (RECONCILE.md section R1) closed your c1 audit; obligations O1, O2-sequencing, O3-sequencing, and O6 land here.
Superpowers brainstorming owns the how; this dispatch locks the what.

### Scope — DD-theory-c2 must decide and lock

1. Certificate schema v0: the s2 shared contract. Node/edge encoding for a canonicalized AND-OR proof DAG (one declared move at our-to-move nodes, all legal replies at opponent nodes), leaf classes (terminal; rule-closed with rule id/version/witness; each with explicit trust base), chunk/manifest/streaming layout (child-before-parent or obligation-table order), content addressing and integrity commitments, header (schema version, ruleset digest, root, claimed value, symmetry convention), non-normative solver-telemetry sidecar.
2. Three-valued proof semantics: exact forms for X-win, O-win, and draw (dual boolean certificates or exact-minimax nodes with both bounds); the schema must reject "one drawing move" as proof of "no winning move" and must not hard-code any value conjecture.
3. Canonical state + D4 specification (normative for both pairs, per ruling R-s2-1): state identity (marks, forced, parity-derived side), the canonicalization convention (transform set, tiebreak, forced-index transport, move mapping through the inverse transform), and the color-swap question (in or out, decided with rationale).
4. Independence boundary (rulings R-s2-2, R-s2-3): checker language selection with throughput justification against expected certificate-size brackets (audit: 1e6–1e12 nodes); what is shared (schema doc, fixtures) vs forbidden (any game-logic code shared with emitter or engine/src/core).
5. Rule-language versioning hook: how the schema references rule ids, versions, and machine-checked soundness artifacts so later rule families (theory c4+, obligation O3/O14) plug in without schema breakage; leaf classes fail closed on unknown rules.
6. Cycle sequencing for the theory pair (checker c3, rule kernel c4+, value-conjecture memo cadence per O6), reconciling your c2–c4 sketch with theory.implementer's c2–c6 sketch.

### Constraints (binding)

- Rulings R-s2-1..4 (RECONCILE.md R1) are input, not open questions — the D4 convention's content is yours to design; its existence and bindingness are not.
- Fixtures schema v1 frozen; s2 fixture needs are additive (R-s2-4).
- Schema must be checkable by streaming with bounded memory and shard-parallel; the checker never searches (hint-rich records).
- ROADMAP gates 1–2 unchanged: schema changes route through the orchestrator; solver rule-leaf closure waits on soundness proofs.

### Grill

GRILL_REQUIRED: yes — new-feature at large tier, cross-domain boundary contract, hard-to-reverse format decisions (proof semantics, canonical encoding, chunking), several downstream choices hanging on the draw-proof-form question.
Run the design-grill against the operator before DESIGN_LOCK_ID.

### Not in scope

No implementation (checker or fixtures), no strategy-rule design beyond the versioning hook, no solver-side storage decisions (solver's DD-solver-c2 owns those; coordinate only at the schema seam).

### Deliverable and lineage

DD-theory-c2-20260828 under the sprint designs/ tree; PHASE: DESIGN relay carrying DESIGN_DOC_ID; design-review request addressed TO theory.implementer (orchestrator CC only); on DESIGN_REVIEW_VERDICT: approve, report completion to the orchestrator for PROCEED-TO-PLAN.

### Acceptance criteria

- A solver engineer can emit and a checker engineer can verify from the document alone (no oral context).
- Every leaf class names its exact trust base and verification obligation.
- Draw semantics survive an adversarial read: no encoding accepts a weaker claim as a stronger one.
- Canonical-form spec is exact enough that two independent implementations must agree byte-for-byte on canonical states.

Boundary contract: Writes: certificate schema spec + canonical-state spec. Reads: rule lock (ROADMAP), rulings R-s2-1..4, c1 audit landscapes. Target entity: DD-theory-c2 design doc. Downstream consumers: solver emitter, theory checker, viz pair (later). Contract: the schema itself. Proof: design review approve + grill record. No-consumer action: not applicable — consumers are commissioned.

Operator-judgment items expected to surface in the grill: draw-proof form (dual certificates vs exact-minimax), color-swap symmetry, checker language, certificate portability vs regenerable-chunk tradeoff (theory.implementer flagged this as a possible later operator choice — surface it now instead).
