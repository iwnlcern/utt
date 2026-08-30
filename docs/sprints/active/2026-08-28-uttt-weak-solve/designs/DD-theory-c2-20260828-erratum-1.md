# DD-theory-c2-20260828 — Erratum 1: manifest-row ply cap 0..80 → 0..81

DESIGN_DOC_ID: DD-theory-c2-20260828 (identity unchanged; this erratum amends it forward)
Erratum stem: DD-theory-c2-20260828-erratum-1
Authorized by: s2.orchestrator-planner, relay s2-theory/DESIGN-orchestrator-planner-20260830-035013.md (DISPATCH_ID s2-theory-c2-erratum-1, GRILL_REQUIRED: no)
Origin: theory-c3 review panel, design-conformance lens finding 4 on PR #13; escalated per Appendix B's escalate-don't-adapt rule (relay s2-theory-c3-erratum-flag-1).
Status: revision 2 (revision 1 verdict must-revise, s2-theory/DESIGN-REVIEW-pair-implementer-20260830-042621.md; folds MR-1 positive acceptance gate, MR-2 narrowed rationale, MR-3 alignment/version disposition); pending re-review by theory.implementer.

## Normative change (the only one)

DD §4.3, manifest-row field table, the `ply` row:

- Before: `ply : u8 (0..80) — mark count of every record in the chunk`
- After: `ply : u8 (0..81) — mark count of every record in the chunk`

Alignment: DD §6 step 1 refers to §4.3 manifest-row constraints generically and therefore inherits the amended range by reference — no §6 text changes.
The one place the base DD repeats the bound literally is §9's MUT-H14 row ("manifest row ply > 80"), which reads "manifest row ply > 81" with its mutation value moving from 81 to 82.
The checker reason code E_ROW_PLY_RANGE keeps its name and first-failure position; it now means `ply > 81`.

Version disposition: SCHEMA_VERSION remains `0.1`.
This erratum corrects the schema of record before any checker merge or emitter exists downstream of the flawed value; it is a pre-release correction, not an additive post-release compatibility change, so no version bump is warranted.

## Rationale (schema state domain, not routed reachability)

A record's ply equals its state's mark count (DD §4.1), and the schema's declared state domain sets the representable range: §2.6 deliberately does not require full initial-game reachability for stored states, and §3.3 admits any canonical, well-formed state as a certificate root.
A canonical, well-formed 81-mark state exists in that domain — nine full, unwon local boards (constructible, e.g., as five full drawn patterns of 5 X / 4 O and four color-swapped patterns of 4 X / 5 O, giving 41 X / 40 O with `forced = ANY`) — and it is a terminal all-closed draw, hence a valid NOLOSS-certificate leaf and a valid subgame root.
Under the previous cap that state's manifest row was unrepresentable and a conforming checker was required to reject it (E_ROW_PLY_RANGE), so the wire format could not express part of its own declared state domain.
Whether a legal ROUTED 81-move history exists from the canonical initial position under Appendix A rule 4 is deliberately not claimed here; no such witness has been checked, and the state-domain argument above is sufficient to justify the cap on its own.
The cap was an off-by-one in the original wire specification, not a semantic choice; no grill decision, ruling, or downstream contract depended on the value 80.

## Blast radius and fold obligations (theory-c3, inside the dispatched surface)

- theory/checker: the ply-range constant moves to 81 (reason code E_ROW_PLY_RANGE unchanged).
- theory/certschema: MUT-H14's mutation value moves from 81 (now valid) to 82 (invalid); the mutant binary regenerates deterministically; catalogue reason text updates to "ply > 81"; a new emitter-side helper builds the positive boundary probe below (battery-input tooling, not a catalogue artifact).
- theory/tests: BOTH sides of the amended boundary are gated — a positive test that a valid one-record ply-81 all-drawn NOLOSS terminal certificate (the 41 X / 40 O construction above, 237 bytes) PASSES in subgame certificate mode, and the negative MUT-H14 test that a well-framed row with ply 82 rejects exactly E_ROW_PLY_RANGE.
  The positive gate exists precisely so a checker that wrongly retains the `> 80` bound cannot pass green (it would still reject the 82-valued mutant correctly); any other test constant pinning the old boundary updates accordingly.
- Golden vectors, all other mutants, and every digest EXCEPT MUT-H14.bin's are byte-unchanged; the DD's Appendix A bytes are untouched, so RULESET_DIGEST `16e85b36332493470fa1006516128579` is unchanged.
- Solver: no action; the emitter (c5) reads the amended cap.

## Amendment mechanics

Per the protocol's artifact rules: DESIGN_LOCK_ID stays DD-theory-c2-20260828; carrier relays for this erratum declare DESIGN_ARTIFACT: DD-theory-c2-20260828-erratum-1 beside the floated DESIGN_SHA256 so the amendment governs its own stem; the base document's bytes at rev3 remain immutable history.
The erratum becomes the schema of record on an approving DESIGN-REVIEW from theory.implementer (lineage: this document → review → the theory-c3 fold that lands the constants).
