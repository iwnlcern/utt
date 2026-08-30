ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: s2-theory-c3-plan-review-1
PARENT_DISPATCH_ID: s2-theory-c3-plan-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — three bounded plan repairs are mechanical; the resume-journal/alt-profile omission requires an orchestrator scope ruling, not operator judgment
FROM: theory.implementer
TO: theory.planner
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-pair-planner-20260829-152145.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
PLAN_LOCK_ID: PL-theory-c3-20260829
PLAN_ARTIFACT: PL-theory-c3-20260829
PLAN_SHA256: 53951b4d8d59edd0a8875ada15b368b38b5a89b33af52075bc645c9356d9bfc5
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@1003b0b
TARGET_BRANCH: main
SUBJECT: PLAN-REVIEW theory-c3 rev1 — checker architecture passes; field isolation, report/closure contracts, and c3 scope require revision

## Scope and identity

Reviewed all 319 lines of `PL-theory-c3-20260829` at SHA-256 `53951b4d8d59edd0a8875ada15b368b38b5a89b33af52075bc645c9356d9bfc5`, the addressed PLAN relay, the orchestrator proceed relay, locked design §§2–9 and §11, the approved design review, the landed corpus generator/tests, C3 project/stdlib source surfaces, and current daemon lineage. The plan and design digests match their relays; current HEAD is the plan commit `1003b0b1da23b2bbf419af819435e1a7510a574b`; exact-file relay lint has zero errors and zero warnings.

No source, test, fixture, design, plan, branch, commit, PR, merge, publication, deployment, release, or external-run change was made.

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The main architecture is implementable and should be preserved: fresh C3 game logic, scalar XXH3-128 with length-class anchors, child-before-parent verification, explicit first-failure codes, verdict claim binding/completeness, P4's two-reply construction, and the Python integration battery all align with the locked design. Homebrew currently identifies stable C3 as 0.8.3; the stale source binary failure and PATH repair are correctly anticipated. The current Python suite is green at 141 tests.

Implementation dispatch remains held for the four items below.

## MR-1 — MUT-R22 field-isolation expectation names an impossible field and omits real changes

Plan line 144 prescribes `{"cert.totals", "manifest.record_count", "manifest.byte_length", "manifest.digest", "record.removed"}` while simultaneously requiring the existing exact field vocabulary. `record.removed` does not exist in `_certificate_fields`. A read-only construction of the specified P4 followed by the specified non-first ply-26 record drop and `rebuild()` changes exactly:

`{"cert.totals", "chunk.fixed_count", "chunk.fixed_records", "manifest.byte_length", "manifest.byte_offset", "manifest.chunk_digest", "manifest.digest", "manifest.record_count"}`.

The later ply-25 chunk offset moves from 445 to 405 when the 40-byte record is removed, and the modified ply-26 chunk necessarily changes its fixed count, fixed-record tuple, and chunk digest. As written, Task 1's GREEN gate cannot pass while following the plan.

Required revision: replace the MUT-R22 expected set with the exact eight-field set above; explicitly extend both `_BASE_BUILDERS` and the test-side `BASES` with P4; retain the catalogue count and R17 wording checks.

## MR-2 — required verified-result and report fields are absent from the task contract

Locked DD §7 step 6 requires verdict verification to compare each verified member's schema version, ruleset id and digest, symmetry id, and root. Task 5's `VerifyResult` interface carries only status/code/claim/root/root predicate/totals/orphans, leaving Task 6 no specified verified result from which to enforce the shared fields. Task 7 then omits locked DD §6 report requirements: failure position, ruleset digest/shared identity, and wall/CPU time. The battery does not assert those fields, so all stated acceptance criteria can pass with an incomplete DD report and an ad hoc second parse for verdict fields.

Required revision: define one verified-certificate result that carries all member fields consumed by §7 and Task 7, including schema, ruleset id+digest, symmetry id, root, claim, header-region digest, totals, and orphan count. Complete the JSON contract with the locked shared identity, failure position for rejects, and wall/CPU timing; add direct C3/report or Python assertions so these are mechanically gated. Verdict comparison must consume the fields produced by the full member verification, not trust an independent unverified parse.

## MR-3 — orphan accounting is not the locked root-downward closure and is untested

DD §2.6 defines orphans as verified records outside the root's downward closure. Plan line 212 instead says records “never referenced by any parent edge,” which is indegree accounting, not reachability: it counts the root itself as an orphan and fails to count an internally connected but root-disconnected component. None of Task 5's tests asserts `orphans`, so even the golden one-node root can report 1 and still pass the written gate.

Required revision: retain the verified node/edge graph, traverse downward from the verified header root, and report `all verified nodes - root-reachable nodes`; the root is never an orphan. Add assertions that P1/P2/P3/P4 report zero and a unit-level disconnected-component case reports the exact nonzero count without making default verification fail.

## MR-4 — mechanical scope diff is not all-in until the orchestrator resolves the §11 omission

The proceed relay names the bundle as the approved `DD-theory-c2 §11 c3` slice. That locked slice explicitly contains both a resume journal and the alternating fixture profile. Plan lines 309–315 place both out of scope based on their omission from the proceed relay's later paraphrase. An omission in a paraphrase does not mechanically remove named bytes from the incorporated locked slice, and delegated dispatch authority is conditional on `SCOPE_DIFF_RESULT = all-in`.

Required revision: route this exact scope question to `s2.orchestrator-planner`. Either obtain an explicit amendment narrowing theory-c3 and cite it in the successor PLAN, or put the resume journal and alt fixture profile back into tasks, file lists, tests, and acceptance. The Pair Planner must not self-resolve the ambiguity or issue an IMPL token while the diff has OUT rows.

## Acceptance-criteria disposition

1. Positives and verdict modes: PASS at plan level; P4's two legal replies and both terminal-X chains were independently reproduced.
2. 44 intended-reason rejections: PASS at plan level, conditional on MR-1's executable corpus test expectation.
3. XXH3 vectors and header anchor: PASS at plan level; all four length classes, canonical output, and the 189-byte golden header region are pinned.
4. Fixtures-v1 matrix: PASS at plan level; the 3+1 legality, 6 closure, and 2+3 terminal rows match the landed compatibility matrix.
5. Backlog determinism: PARTIAL — P4/R22 construction is sound, but the literal changed-field gate cannot pass.
6. Full suite and scope diff: PARTIAL — current Python baseline is green, but the DD report/closure requirements lack gates and the §11 scope diff is not all-in.

## Boundary contract

Writes: only the plan-listed corpus backlog, checker, tests, README, successor plan, and relay surfaces after revision and a later addressed implementation dispatch.

Reads: locked DD bytes, frozen fixtures-v1, and landed corpus bytes; no semantic code sharing with certschema, engine, solver, or referee.

Target entity: an independent C3 certificate/verdict checker for the shared theory-owned wire contract.

Downstream consumer: the s2 proof deliverable and later solver-emitted certificates.

Contract: DD §§1–9, including root-downward orphan accounting, complete machine report, exact verdict member field binding, and fail-closed reason behavior.

Proof: C3 units plus the Python corpus battery under the pinned compiler, with exact corpus and report assertions.

No-consumer action: not applicable — the solver and final verification path are named consumers; seam or scope changes route to the orchestrator.

## Required successor and next gate

Amend `PL-theory-c3-20260829` forward without changing `PLAN_LOCK_ID`; preserve the accepted P4 construction, reason-code order, compiler pin, independence fence, and merge hold; float `PLAN_SHA256`; reissue `PHASE: PLAN` with a unique successor `DISPATCH_ID`, still parented to `s2-theory-c2-design-review-4`, and `IN_REPLY_TO` this review. Cite the orchestrator's explicit §11 scope disposition. Do not run final SCOPE_DIFF or issue implementation dispatch on this verdict.

OPERATOR_ITEMS: none — no operator semantic choice or waiver is requested; the scope ambiguity belongs to the addressed orchestrator authority

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local static checker and corpus artifacts
- migration/backfill/destructive-write/canonical-data-repair: no — additive generated test artifacts only
- money/inventory/orders/planning/accounting/trust-critical-state: no — game-proof artifacts
- AI-or-automation-acts-downstream: no — no external actuator
- worker/scheduler/queue/retry/async-side-effect: no — no worker surface in the reviewed plan; the disputed journal is local verification state
- cross-repo/service-contract/generated-schema/shared-API-event: yes — checker enforces the theory-owned contract consumed by solver and the deliverable path
- user-visible-control-with-materializer/downstream-consumer: no — CLI/report only
- test-runtime-role-mismatch: yes — current gates can pass while orphan/report contracts are incomplete, and Task 1's literal R22 expectation cannot pass
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — two locked §11 items are excluded without an explicit scope amendment
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or residual acceptance requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

Verification:
- E1: incoming relay exact-file lint with `--no-freshness` reports zero errors and zero warnings; plan and design SHA-256 values match the addressed pins.
- E1: HEAD is exactly `1003b0b1da23b2bbf419af819435e1a7510a574b`; daemon is ready/active with zero conflicts and pending renders; pre-filing `relay verify` reports `ok: true`.
- E2: read-only P4 reproduction yields counts 13/12, exactly `[(5,7),(5,8)]`, legal `(8,8)` prover moves, two terminal X wins, and five distinct canonical states.
- E2: read-only MUT-R22 rebuild changes exactly the eight fields listed in MR-1; P4 shrinks 493 to 453 bytes and shifts the later chunk offset 445 to 405.
- E2: `cd theory && uv run pytest -q` passes, 141 tests in 133.10 seconds.
- E1: Homebrew reports stable `c3c` 0.8.3; current PATH resolves to the known stale source binary and reproduces its missing-z3-dylib failure, exactly the condition Task 2 says to replace.

ACTIONS_GIT_REF: no source/test/fixture/design/plan/branch/commit/PR/merge/publication/deployment/release/external-run change claimed; authored only this PLAN-REVIEW draft for daemon filing; final status below attributes planner, daemon, and scheduler bytes separately
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M INDEX.md
?? s2-theory/PLAN-pair-planner-20260829-152145.md
