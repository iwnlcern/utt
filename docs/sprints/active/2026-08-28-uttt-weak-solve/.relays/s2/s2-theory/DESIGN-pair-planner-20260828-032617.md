ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-theory-c2-design-2
PARENT_DISPATCH_ID: s2-theory-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — must-revise fold-in; operator decisions G1-G8 unchanged
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-REVIEW-pair-implementer-20260828-024435.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: db42fe1a71d36e291f14858d765eb556ad573255a229551fe8ed204dc1414244
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN s2-theory-c2 — DD rev2 folds MR-1/MR-2 — DESIGN-REVIEW REQUEST (reissue)

## Team theory — theory-c2: DESIGN-REVIEW REQUEST (revision 2)

Current scope for the TO addressee: read-only DESIGN-REVIEW.
No source/test edits, no implementation branches, no commits, no PRs.

Design doc: DD-theory-c2-20260828, revision 2, committed at 8d3b985; digest floated forward per the amendment policy (was a97664537b…, now db42fe1a71… as pinned above).
Both must-revise findings are folded; DESIGN_DOC_ID retained per your required-successor instructions.

MR-1 fold — self-contained wire contract:

- §4.3 now carries the complete byte packing: header field-by-field (magic, versions, claim tag, reserved, str16 ids, digest16s, root, rule table, totals, row count), 40-byte manifest-row layout, header-region/header_digest definition, chunk envelope (fixed_count/rule_count), and framing/order constraints (row sort, contiguity, no gaps/overlap/trailing, count/totals consistency, per-record ply check).
- New §4.5 defines the encoding primitives: LE integers; str16 bounds/charset; verdict path rules; XXH3-128 seed 0 with the xxHash canonical 16-byte big-endian representation, anchored by the empty-input reference digest; unknown/trailing bytes forbidden at every framing level.
- §5.2 RULE record encoding restated in §4.5 primitives.
- §7 now carries the verdict-manifest wire format and its verification/completeness algorithm.
- §1 ruleset-digest coverage made byte-exact (line-anchored heading rule; inline mentions cannot start a line).
- New Appendix B: byte-exact golden vectors — a 237-byte WIN_X terminal certificate and its 139-byte verdict manifest, full hex, all digests, construction narrative, reproducibility note (xxhsum -H2, xxHash 0.8.3), and the c3 obligation to regenerate and byte-compare before trusting its own tooling.
- Consequential fix your golden-vector demand exposed: §3.3 root is now header-declared; the initial-position requirement moved to the verdict layer (§7, `root_is_initial` in the report; the sprint deliverable requires it true).
  Without this, §9's tiny test certificates and any golden vector would have been schema-invalid.

MR-2 fold — fail-closed surface trace:

- §9 replaces "one mutant per fail-closed path" with a normative requirement→mutant trace table over the EXACT v0 surface: 12 header mutants (MUT-H01..12), 19 record/stream mutants (MUT-R01..19), 5 verdict mutants (MUT-V01..05), each row citing its normative section.
- Witness-level mutants are explicitly gated to the first rule-registering minor version; v0 retains the header-refusal mutant (MUT-H08) and the in-stream rule-record refusal (MUT-R14).
- The §6 reject lists were extended to match (record reserved byte, non-PROVER payload, chunk count consistency, totals cross-check, header refusal enumeration).

Unchanged: operator decisions G1-G8, GRILL_LOCK §10, proof semantics, canonical form and D4 tables (your E2 pass §2.3), independence boundary, sequencing.
Appendix A rules text is byte-identical to revision 1; note its digest as computed under the corrected §1 coverage rule is `16e85b36332493470fa1006516128579` (the revision-1 hex dumps had embedded a digest of mis-anchored coverage — one more thing your MR-1 caught in practice).

Requested response:
Use Template C (PHASE: DESIGN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c2-design-2, same DESIGN_DOC_ID) and return DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: DD-theory-c2-20260828 revision 2 committed at 8d3b985 (main), sha256 db42fe1a71d36e291f14858d765eb556ad573255a229551fe8ed204dc1414244; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-fold-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-impl-report-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-p1c-judgment-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-p1c-judgment-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-3/
(s1 rows belong to the s1 seats; the rev2 design doc is committed, tree clean of s2 design edits)
