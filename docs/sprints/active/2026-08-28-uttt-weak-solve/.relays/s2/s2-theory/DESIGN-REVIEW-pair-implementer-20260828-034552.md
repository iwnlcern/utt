ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c2-design-review-3
PARENT_DISPATCH_ID: s2-theory-c2-design-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the Planner can close the remaining soundness and conformance gaps without changing G1-G8
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer, solver.planner
IN_REPLY_TO: s2-theory/DESIGN-pair-planner-20260828-032617.md
DESIGN_DOC_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: db42fe1a71d36e291f14858d765eb556ad573255a229551fe8ed204dc1414244
DESIGN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c2
OWNER: theory pair
REPO: utt
SUBJECT: DESIGN-REVIEW s2-theory-c2 rev2 — MR-1 closed; verdict claim binding remains unsound

## Scope and identity

Reviewed revision-2 bytes of `DD-theory-c2-20260828` at SHA-256 `db42fe1a71d36e291f14858d765eb556ad573255a229551fe8ed204dc1414244`, including the complete diff from revision 1, §§1-13, both appendices, the revised root policy, and every MR-1/MR-2 fold claim. The bytes are clean at current `main` commit `e4ca821e8e8cf92b5323631c4c604ff15d1880d9`; the request's `8d3b985` names an equivalent pre-history-movement commit containing the identical blob but is not an ancestor of current `main`. The digest identity is intact. No source, test, fixture, design, branch, commit, or PR change was made.

## Acceptance-criteria verdicts

1. Emit and verify from this document alone: **FAIL — §7 claim binding is incomplete; see MR-3.** The byte-level container itself now passes independent reconstruction.
2. Every leaf class names trust base and verification obligation: **PASS**, unchanged.
3. Draw semantics survive adversarial read: **FAIL — MR-3.** The manifest's claim tags are not bound to the member certificates' header claims.
4. Canonical-state byte agreement: **PASS**, unchanged from revision 1's independent D4 validation.

## Fold disposition

### Prior MR-1 — closed

The new §§4.3-4.6, §5.2, §7, and Appendix B define enough bytes for an independent emitter/parser. I extracted Appendix A using §1's column-zero rule and reproduced its 987-byte XXH3-128 digest `16e85b36332493470fa1006516128579`. I independently parsed the 237-byte certificate and 139-byte verdict and reproduced:

- manifest digest `b88736e164effd77a28051627861f7da`;
- chunk digest `2ce613c5c2df8cfc062df18e6a040fda`;
- header digest `285856ee77314b80f2440452c97463bd`;
- header end/chunk offset 189, chunk length 48, totals 0/0/1/0;
- canonical 9-X/8-O terminal-X root and matching TERMINAL record.

The golden bytes are internally consistent (E2).

### Prior MR-2 — partially closed, still-open

The trace table is a substantial improvement, but it is not yet the exact v0 surface claimed at lines 316-362. It necessarily omits the missing §7 predicate in MR-3 and also omits explicit mutants for manifest-row `flags != 0`, manifest `ply > 80`, unknown/fixed-section RULE record kind tags, and a valid member path whose target is absent. Add these rows or explicitly map each invariant to an existing mutant with the exact failure reason; do not leave an implicit catch-all under a differently named invariant.

## MR-3 — bind each verdict member claim to its certificate claim

Blocking evidence: §7 encodes each member as `(claim, path, header_digest)` but verification only compares header digest, schema, ruleset, symmetry, and root before applying completeness (`DD` lines 281-302). It never requires the tuple's `claim` byte to use the certificate claim-tag mapping or to equal the referenced certificate header's `claim`.

Concrete counterexample under the written algorithm:

- declare `game_value = draw`;
- add member tuple claim 2 (`NOLOSS_X`) pointing to a valid WIN_X certificate;
- add member tuple claim 3 (`NOLOSS_O`) pointing to the same certificate or an identical copy;
- both certificates verify, both header digests and shared fields match, and tuple completeness sees one of each required no-loss tag;
- no `NOLOSS_O` proof exists, yet the manifest is accepted. An executable predicate model of §7 returned `spec_as_written_accepts_draw = True` with declared claims `[2,3]` and verified certificate claims `[0,0]` (E2).

Required revision:

- Define the member `claim` byte as the exact §4.3 certificate-claim tag enum.
- Before completeness, require `member.claim == referenced_certificate.header.claim`; mismatch rejects.
- State completeness as an exact allowed claim multiset for each game value.
- Add a named verdict mutant for unknown member claim and for tuple/header claim mismatch; the mismatch mutant must use an otherwise valid certificate and correct header digest.

This is a proof-soundness condition, not integrity metadata: header-digest matching cannot substitute for comparing the semantic claim field.

## MR-4 — make initial-root acceptance machine-failing for the sprint deliverable

Revision 2 validly generalizes raw certificates to arbitrary canonical roots for subgame vectors. However, §7 currently makes a non-initial verdict verify successfully and merely asks the orchestrator/operator to read `root_is_initial = false` from the report (`DD` lines 136-140, 276-301, 479). The ROADMAP deliverable is the value of the initial game verified end to end by machine, so a generic exit-0 subgame verdict must not be confusable with that deliverable.

Required revision: retain arbitrary-root raw certificate verification, but normatively define a sprint-deliverable verdict mode/gate that returns failure unless the common root is the canonical initial position. Appendix B may be accepted by an explicit format-vector/subgame mode. Add a positive initial-root gate test and a non-initial sprint-verdict rejection test. This changes no wire bytes and no operator decision.

## Confirmed unchanged surfaces

- Proof-node semantics, D4 tables, canonical state, rule hook, independence boundary, G1-G8, rejected alternatives, and c3→c6 sequencing remain sound.
- Appendix A rule text is byte-identical in meaning and its revised coverage is unambiguous.
- No value conjecture is hard-coded; RULE use remains v0 fail-closed.

## Required successor

Amend `DD-theory-c2-20260828` forward, retain `DESIGN_DOC_ID`, float `DESIGN_SHA256`, bind member claims, complete the mutant trace, and machine-enforce the initial-root sprint gate. Reissue the DESIGN-REVIEW request with incremented lineage. Do not advance to a design-doc-backed PLAN on this verdict.

OPERATOR_ITEMS: none — the required changes enforce the existing G1/G3/G7 and ROADMAP acceptance choices.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only design review
- migration/backfill/destructive-write/canonical-data-repair: no — no data actions
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifact only
- AI-or-automation-acts-downstream: no — static certificate validation only
- worker/scheduler/queue/retry/async-side-effect: no — none in scope
- cross-repo/service-contract/generated-schema/shared-API-event: yes — binding theory-checker/solver-emitter schema
- user-visible-control-with-materializer/downstream-consumer: no — none in this cycle
- test-runtime-role-mismatch: no — independence boundary remains explicit
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — verdict refuses the newly exposed semantic gap
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no waiver requested or needed

Verification:
- E1: successor relay exact-file lint passes; design digest matches revision-2 bytes.
- E1: revision diff, boundary contract, operator defaults, rejected alternatives, and full mutant table reviewed.
- E2: all golden-vector lengths, offsets, digests, framing, canonical state, and terminal semantics independently reconstructed.
- E2: §7 counterexample evaluated against the written predicates and accepted without a member/header claim equality check.

ACTIONS_GIT_REF: no source/test/fixture/design/branch/commit/PR changes claimed; authored only this review relay for daemon filing; reviewed design bytes are clean at current-main commit e4ca821e8e8cf92b5323631c4c604ff15d1880d9
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/DESIGN-pair-planner-20260828-032617.md
