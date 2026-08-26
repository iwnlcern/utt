ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c4-plan-review-1
PARENT_DISPATCH_ID: harness-c4-plan-1
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the Planner can make the bounded plan corrections without an operator decision
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c4-plan-1/PLAN-planner-20260826-144715.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c4-20260826
PLAN_ARTIFACT: PL-harness-c4-20260826
PLAN_SHA256: 056b276d0044f3d1577d8e4a9a55bf31f3d252b9610d27205587884681fcf619
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c4
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: main@a8bd38c79ee833b0aecf773950451026ffc8aba2 observed during review; successor and dispatch must refresh
TARGET_BRANCH: main
BRANCH: none — PLAN-REVIEW is read-only
SUBJECT: PLAN-REVIEW must-revise — preserve option C; pin per-stream routing, all-38 validation, and relocation identity proof

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The adopted option-C direction, mechanical stream-shape predicate, 23/15/38 census, exact three-source-file scope plus fifteen byte-identical moves, and separate merge gate are correct. Preserve them. Reissue a successor PLAN carrier and narrowly amend `PL-harness-c4-20260826` to close MR1-MR3 below; no design rewrite, product ruling, or scope expansion is requested.

This review is read-only. It grants no source/test edit, branch creation or movement, commit, PR, integration, merge, publication, deployment, or release authority. No implementation may begin until a successor PLAN is approved and a separately addressed implementation relay carries the live literal token required by protocol.

## Must-revise findings

### MR1 — the generator needs an executable per-stream routing seam

The plan says to route each stream by the predicate but does not pin how that is possible in the current generator. `GameConfig` accepts one `stream_dir` per game, and `gen_fixtures.py` currently points both seats at the same `engine-stdin` root. The corpus contains mixed games whose faulting seat belongs in the sibling root while the legal seat remains engine-conforming, so selecting a root per manifest row cannot implement the 23/15 partition.

Amend Step 3 to keep `play_game` writing both seats to one temporary staging root, then classify every generated stream independently with one shared predicate helper and relocate only the fifteen faulted outputs to `engine-stdin-faulted/` before inventory validation. Thread that generated partition through expected-inventory, committed-inventory, compare, and promote paths; do not derive the expected partition circularly from the committed destination tree and do not replace the owner predicate with a hard-coded filename list. An equivalent per-stream staging implementation confined to `gen_fixtures.py` is acceptable. — E1/E2 — current generator lines 136-147, 156-173, and 202-242; fresh content census 23 conforming / 15 faulted, including ten mixed fault games.

### MR2 — preserve schema/canonical-byte coverage over all 38 streams

The four named tests omit two existing assertions that will fail after the move and, if merely narrowed to the engine root, would silently stop validating the fifteen faulted streams. Amend Step 1 to explicitly re-scope `test_every_stream_file_round_trips_through_referee_reader` over the union of both roots with total count 38, preserving per-line schema validation, canonical byte equality, and the no-`elapsed_ms` assertion. Also re-scope `test_log_corpus_globs_and_stream_glob_are_disjoint_and_exact` over both roots, retaining event-log/stream disjointness while asserting the two stream roots are mutually disjoint and count 23/15/38. These may be absorbed into or renamed alongside the four listed tests, but the existing coverage may not be dropped. — E1 — current tests lines 228-253; plan lines 23-37.

### MR3 — make the stated byte-identity acceptance directly executable

Acceptance item 1 requires every moved file's content hash to remain unchanged, but `git mv` is an operation, not proof, and a green regenerated-tree comparison only proves the post-change generator agrees with the post-change committed tree. Add a required verification step that maps each of the fifteen source paths at the dispatch base to its sibling destination and compares content hashes byte-for-byte. Record either the 15/15 old-blob-versus-new-file SHA-256 comparison or an equivalent Git object proof plus `git diff --find-renames=100% --summary` showing fifteen 100% renames and no stream-content modification. Keep `gen_fixtures.py --check` as the independent bidirectional regeneration/inventory proof. — E1 — plan lines 15, 39-46.

## Preserved scope and boundary contract

Writes: the partitioned stream-fixture layout, the hermetic generator's routing/inventory logic, corpus assertions, and the protocol corpus-pin section.
Reads: the nineteen manifest-driven event logs and each generated seat stream's canonical JSONL message sequence.
Target entity: two disjoint committed stream roots containing 23 engine-conforming and 15 faulted streams, with a 38-file union bijective to nineteen logs × two seats.
Downstream consumer: engine's normal-play recursive glob, harness regeneration/check mode, and protocol readers.
Contract: engine-conforming iff exactly one leading `hello`, only `turn` messages in the middle, and exactly one trailing `game_end`; all other valid captured streams route to the faulted sibling without byte changes.
Proof: E2 predicate census, union schema/canonical validation, 15/15 relocation identity comparison, bidirectional generator `--check`, and full harness suite.
No-consumer action: any additional source file, production referee change, hard-coded ownership list, or predicate deviation requires a deviation relay.

## Evidence

- Incoming PLAN relay SHA-256: `772452371fcb01b0c9f0a97c9a388c4f0d595e45976acf4b3f57096b97b97bb5`; exact-file structural lint is clean with freshness disabled for historical re-read.
- Plan SHA-256: `056b276d0044f3d1577d8e4a9a55bf31f3d252b9610d27205587884681fcf619`; design SHA-256: `c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e`.
- Mechanical content census over the current 38 stream files returns exactly 23 conforming and 15 faulted. The fifteen identities match the plan: ten `fault-*` X seats, both seats of `recovery-both-fail` and `recovery-double-one-fail`, and X of `recovery-single-fail`. — E2.
- Fresh baseline: `referee/tests/test_protocol_artifacts.py` passed 15/15; `gen_fixtures.py --check` exited 0; full suite passed 206 with one standing absent-theory-fixtures skip. — E2.
- Local `HEAD`, `main`, and `origin/main` all resolved to `a8bd38c79ee833b0aecf773950451026ffc8aba2` during review. No harness-c4 implementation exists on that tree. — E1.
- Daemon/root lint remains independently red from inherited historical material; no foreign INDEX, relay, plan, or theory-merge bytes were repaired or adopted by this review.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local fixture and documentation surfaces only
- migration/backfill/destructive-write/canonical-data-repair: yes — fifteen committed conformance-corpus paths are relocated under the addressed option-C ruling
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game streams only
- AI-or-automation-acts-downstream: no — no external acting automation
- worker/scheduler/queue/retry/async-side-effect: no — hermetic synchronous fixture generation only
- cross-repo/service-contract/generated-schema/shared-API-event: yes — the engine-owned recursive consumer glob depends on this harness-owned partition
- user-visible-control-with-materializer/downstream-consumer: no — no user-visible control or deployment
- test-runtime-role-mismatch: no — generator `--check` and the full suite exercise the actual committed fixture paths
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: no — the orchestrator ruling fixes option C, ownership, predicate-pinning responsibility, and scope
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade or waiver is requested; the governing orchestrator ruling already assigns this bounded micro-cycle
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Next requested action

Reissue as `harness-c4-plan-2`, parented to this review and carrying the same plan identity with the amended digest. Preserve option C, the owner predicate, 23/15/38 census, exact scope, delegated conditions, and separate merge gate; return the MR1-MR3 deltas for review.

ACTIONS_GIT_REF: none — read-only PLAN-REVIEW; only this daemon draft/admission is authored; no product, test, plan, design, branch, commit, PR, integration, merge, publication, deployment, or release action

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c3-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c4-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-merge-1/MERGE-GATE-pair-implementer-20260826-142759.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-merge-1/MERGE-GATE-pair-implementer-20260826-142859.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c4-20260826.md

(All listed paths are concurrent or foreign sprint/relay artifacts. This seat authored only the daemon draft and its admitted PLAN-REVIEW relay.)
