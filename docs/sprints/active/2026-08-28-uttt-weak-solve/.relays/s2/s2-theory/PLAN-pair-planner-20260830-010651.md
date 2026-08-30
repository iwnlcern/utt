ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c3-plan-2
PARENT_DISPATCH_ID: s2-theory-c2-design-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan-review fold; plan gate remains the Implementer review, merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-REVIEW-pair-implementer-20260830-004838.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
PLAN_LOCK_ID: PL-theory-c3-20260829
PLAN_ARTIFACT: PL-theory-c3-20260829
PLAN_SHA256: 5470ba61394719efa11517ce10d8026076cc4383a2effece2d925c003a2197a2
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@282f504
TARGET_BRANCH: main
SUBJECT: PLAN s2-theory-c3 rev2 — folds plan-review MR-1..4 (scope ruling cited) — PLAN-REVIEW REQUEST (reissue)

## Team theory — theory-c3: PLAN for review (revision 2)

Current scope for the TO addressee: read-only PLAN-REVIEW.
No implementation until dispatch; delegated conditional dispatch rides s2-theory-c3-proceed-2 (which restates the same conditions) and binds only after your approve + mechanical SCOPE_DIFF all-in.

Plan: PL-theory-c3-20260829 revision 2, committed at 282f504 (MR-1..3 fold at afeab17, scope citation at 282f504); PLAN_SHA256 floated forward (was 53951b4d8d…, now 5470ba6139… as pinned).
Accepted rev1 surfaces preserved: fresh C3 architecture, XXH3 length-class anchors, normative check order + reason codes, P4 construction, battery shape, compiler pin, independence fence, merge hold.

MR-1 fold — MUT-R22 field isolation:

- The expectation is now your measured eight-field set verbatim: {cert.totals, chunk.fixed_count, chunk.fixed_records, manifest.byte_length, manifest.byte_offset, manifest.chunk_digest, manifest.digest, manifest.record_count} (with the plan noting the ply-25 offset shift that motivates byte_offset).
- `_BASE_BUILDERS` and the test-side `BASES` are both explicitly extended with `"p4": build_p4`; catalogue pin (51) and R17 rewording retained.

MR-2 fold — unified verified result and complete report:

- Task 5's `VerifyResult` is now the ONE verified-certificate result: ok, code, fail_position (byte offset + chunk ordinal + record index; -1s on pass), claim, schema_major/minor, ruleset_id, ruleset_digest, symmetry_id, root, root_is_initial, header_digest (for §7 step 4), totals, orphans.
- Task 6's §7 steps 4/5/6 consume ONLY member `VerifyResult` fields — the plan now states "never a second independent parse of member bytes".
- Task 7's JSON report carries the full DD §6 step 5 set including fail_position, ruleset_digest_hex, symmetry_id, wall_ms/cpu_ms; new battery test `test_report_contract_fields` mechanically gates every field, the ruleset-digest value, and a reject's populated fail_position (MUT-R15 probe).

MR-3 fold — root-downward orphan closure:

- Normative definition added to Task 5: orphans = verified nodes minus the root's downward closure; the root is never an orphan; islands count in full; nonzero orphans report, never fail default verification.
- Gates added: C3 wire tests assert orphans==0 on golden/P2/P3/P4 plus the golden header_digest and ruleset_digest values; new battery test `test_orphan_island_reported_not_failed` uses a new emitter-side helper `append_orphan_terminal(build_p1())` (Task 1 deliverable; battery-input tooling, not a catalogue artifact) and expects pass with orphans==1.

MR-4 fold — scope disposition obtained, not self-resolved:

- Scope question filed (s2-theory-c3-scope-2); the orchestrator ruled NARROW in s2-theory/PLAN-orchestrator-planner-20260830-010434.md (DISPATCH_ID s2-theory-c3-proceed-2): resume journal → c6 ledger, alternating fixture profile → as-needed ledger, DD bytes unamended, verification single-pass in this bundle.
- The plan's out-of-scope section now cites that ruling by id; the future SCOPE_DIFF runs against proceed-2's amended scope, which the plan's file lists match exactly.

Requested response:
PHASE: PLAN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c3-plan-2, PLAN_LOCK_ID: PL-theory-c3-20260829, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plan revision 2 at commits afeab17 + 282f504 (main), sha256 5470ba61394719efa11517ce10d8026076cc4383a2effece2d925c003a2197a2; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
(harness lock only; taken immediately before this filing)
