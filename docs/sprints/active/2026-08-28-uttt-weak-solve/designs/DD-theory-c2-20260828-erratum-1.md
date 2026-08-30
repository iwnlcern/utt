# DD-theory-c2-20260828 — Erratum 1: manifest-row ply cap 0..80 → 0..81

DESIGN_DOC_ID: DD-theory-c2-20260828 (identity unchanged; this erratum amends it forward)
Erratum stem: DD-theory-c2-20260828-erratum-1
Authorized by: s2.orchestrator-planner, relay s2-theory/DESIGN-orchestrator-planner-20260830-035013.md (DISPATCH_ID s2-theory-c2-erratum-1, GRILL_REQUIRED: no)
Origin: theory-c3 review panel, design-conformance lens finding 4 on PR #13; escalated per Appendix B's escalate-don't-adapt rule (relay s2-theory-c3-erratum-flag-1).
Status: pending erratum DESIGN-REVIEW by theory.implementer.

## Normative change (the only one)

DD §4.3, manifest-row field table, the `ply` row:

- Before: `ply : u8 (0..80) — mark count of every record in the chunk`
- After: `ply : u8 (0..81) — mark count of every record in the chunk`

Consequential normative alignment (same single value, restated where the DD repeats it): the §6 header-stage refusal "manifest row ply > 80" reads "manifest row ply > 81", and §9's MUT-H14 row ("manifest row ply > 80") reads "manifest row ply > 81" with its mutation value moving from 81 to 82.

## Rationale

A record's ply equals its state's mark count (DD §4.1), and the range of reachable mark counts is 0 (the canonical initial position, the deliverable root) through 81.
The 81-mark state is reachable: a local board closes only when won or full (Appendix A rule 3), so a game in which every local board fills without being won places all 81 marks and ends in the all-closed draw (rule 6).
That terminal is a legitimate NOLOSS-certificate leaf; under the previous cap its manifest row was unrepresentable and a conforming checker was required to reject it (E_ROW_PLY_RANGE), making certain draw proofs inexpressible.
The cap was an off-by-one in the original wire specification, not a semantic choice; no grill decision, ruling, or downstream contract depended on the value 80.

## Blast radius and fold obligations (theory-c3, inside the dispatched surface)

- theory/checker: the ply-range constant moves to 81 (reason code E_ROW_PLY_RANGE unchanged).
- theory/certschema: MUT-H14's mutation value moves from 81 (now valid) to 82 (invalid); the mutant binary regenerates deterministically; catalogue reason text updates to "ply > 81".
- theory/tests: the battery's MUT-H14 expectation is unchanged in code (E_ROW_PLY_RANGE); any test constant pinning 80/81 boundary values updates accordingly.
- Golden vectors, all other mutants, and every digest EXCEPT MUT-H14.bin's are byte-unchanged; the DD's Appendix A bytes are untouched, so RULESET_DIGEST `16e85b36332493470fa1006516128579` is unchanged.
- Solver: no action; the emitter (c5) reads the amended cap.

## Amendment mechanics

Per the protocol's artifact rules: DESIGN_LOCK_ID stays DD-theory-c2-20260828; carrier relays for this erratum declare DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1 beside the floated DESIGN_SHA256 so the amendment governs its own stem; the base document's bytes at rev3 remain immutable history.
The erratum becomes the schema of record on an approving DESIGN-REVIEW from theory.implementer (lineage: this document → review → the theory-c3 fold that lands the constants).
