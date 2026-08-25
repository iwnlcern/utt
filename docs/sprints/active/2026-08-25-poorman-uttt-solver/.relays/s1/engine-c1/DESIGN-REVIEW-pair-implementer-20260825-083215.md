ROLE: Pair Implementer
PHASE: DESIGN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c1-design-review-3
PARENT_DISPATCH_ID: engine-c1-design-3
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — the remaining defect is a technical acceptance-threshold correction within engine.planner authority
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: s1/engine-c1/DESIGN-planner-20260825-082247.md
DESIGN_DOC_ID: DD-engine-rules-c1-20260825
DESIGN_ARTIFACT: DD-engine-rules-c1-20260825
DESIGN_SHA256: 9b846063d3a23b978b97ad7d9faa274ca7cead1d788f3690aeae55e56d6d1f99
DESIGN_REVIEW_VERDICT: must-revise
SUBJECT: DESIGN-REVIEW revision 3 must-revise — durable performance guard has no pass/fail threshold

## Verdict

Revision 3 closes MR1, makes the GRILL_LOCK evidence ledger truthful, unifies `RootContext`, and consumes the approved harness schema through the addressed orchestrator route. One narrow blocker remains: acceptance criterion 4 is now a measurement instruction rather than a falsifiable acceptance gate because it defines no allowed candidate/baseline ratio or absolute ceiling.

This verdict reviews the design bytes only. It authorizes no PLAN lock, implementation, dependency vendoring, branch, commit, PR, merge, publication, deployment, or relay-root sequencing waiver.

## Target and integrity

- Reviewed `docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-engine-rules-c1-20260825.md` revision 3 at `main@adba0f9`. The working-tree and committed blobs match the declared SHA-256 `9b846063d3a23b978b97ad7d9faa274ca7cead1d788f3690aeae55e56d6d1f99`. — E1 — exact-file read, `git show`, `sha256sum`.
- The requesting relay is byte-identical to `adba0f9`, replies to `engine-c1-design-review-2`, and passes exact-file relay lint. — E1 — `git show`, `sha256sum`, relay-lint.
- The harness owner edge is legitimate: `DD-harness-c1-20260825` is Implementer-approved at digest `11ac4efc8520d4baa306dbb4f7d902bbcfe5b5738afc1fd0a71941b3e7890440`, and `engine-c1/SITREP-orchestrator-planner-20260825-082114.md` is addressed to engine.planner with the concrete consumer delta. — E1 — exact relay reads.

## Must-revise finding

### MR2 residual — The named benchmark baseline has no verdict threshold

Section 2 now correctly requires the committed harness to measure a fixed reference path first, store the value/environment as a named artifact, and prohibit the candidate from serving as its own baseline. But §13 criterion 4 merely requires the candidate playout to be “compared against” that artifact. Any result, including an arbitrarily slower candidate, satisfies comparison; the former `within 2x` predicate was removed. That is not a hard acceptance criterion or a regression guard. — E1 — design lines 40-45 and 180-186; revision-2-to-3 diff.

Required revision: state a falsifiable pass/fail predicate and what it proves. For example, retain `candidate/reference <= 2.0` if that tolerance is still intended, or choose another evidence-backed ratio/absolute ceiling. Pin the statistic and aggregation rule (for example median candidate ns/ply divided by median reference ns/ply over the documented repeated runs) so the named artifact and candidate measurement compose into one deterministic verdict. Keep the fixed-reference-first ordering, stored environment, DCE guards, and ban on candidate-as-own-baseline.

## Closed findings and retained design

- MR1: closed. Local reachability is necessary and sufficient: no-win boards are monotone-reachable; won boards require exactly one winner and a candidate closing mark intersecting every completed line whose removal eliminates all lines. Dual winners and same-player disjoint lines are rejected, and the independent exhaustive oracle plus named fixtures enforce both classes.
- MR2 ledger: closed. The table payload is correctly 48 bits; the isolated copy probe is explicitly non-evidence; the playout comparison is directional; value-copy is a simplicity/default decision; make/unmake is not claimed measured.
- Consistency corrections: closed in substance. `RootContext` is used consistently, and the approved harness digest plus orchestrator acknowledgment now bind hello/turn/game_end envelopes, X/O + `you`, `tie_owner`, `request_id`, `legal`, reply membership, and `info` without transferring schema ownership.
- M1/M2/M4, fork semantics, collision policy, value-quality shape, operator decisions, and the rules/search-math split remain approved as in the prior reviews.
- Search math remains outside this lock; no backup/cutoff/TT-value/widening semantics are approved here.

## Minor consistency cleanup required in the same successor

- Rename the §6 heading `abstract boundary only; concrete wire bytes deferred`: the body now pins and consumes the concrete approved owner contract and says contract shape is no longer deferred. — E1 — design lines 80-85.
- Replace §6's `info in the current harness draft` wording with `info in the approved harness schema`. — E1 — design line 96.

## Design-review checklist

- Identity, locator, digest, lineage, target entity, and split lock: pass. — E1.
- Rules/representation totality and exhaustive proof seam: pass. — E1.
- Harness boundary ownership and routed consumer contract: pass, subject only to the heading/word cleanup above. — E1.
- Operator decisions, rejected alternatives, and GRILL_LOCK evidence truth: pass. — E1.
- Acceptance criteria: must-revise only at performance criterion 4; criteria 1–3 and gated criterion 5 remain testable. — E1.
- Operator-judgment items: none. A technical threshold must be selected and justified; if the Planner instead asks to accept a non-falsifiable performance gate, that residual-risk choice must route to the operator.

## Escalation record

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — rules design review only
- migration/backfill/destructive-write/canonical-data-repair: no — read-only review
- money/inventory/orders/planning/accounting/trust-critical-state: no — game chips are simulation state, not real monetary state
- AI-or-automation-acts-downstream: no — no implementation/runtime action authorized
- worker/scheduler/queue/retry/async-side-effect: no — no runtime mutation
- cross-repo/service-contract/generated-schema/shared-API-event: yes — engine consumes harness protocol and theory fixtures
- user-visible-control-with-materializer/downstream-consumer: no — no user-facing control changed
- test-runtime-role-mismatch: no — benchmark/test obligations are design-only here
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — a non-falsifiable regression guard would accept residual performance risk
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Requested successor

Return the same `DESIGN_DOC_ID` with a fresh digest and `IN_REPLY_TO` naming this review. Add the benchmark pass/fail predicate and aggregation rule, perform the two §6 wording cleanups, and preserve every closure listed above plus `GRILL_LOCK_ID: GRILL-engine-c1-20260825`. No implementation or PLAN lock is authorized by this verdict.

ACTIONS_GIT_REF:
- relay=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-083215.md`
- index=`docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md` row for `20260825-083215`
- source/tests/design=none — review-only; no source, test, design-doc, branch, commit, PR, merge, publication, or deployment actions

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/DESIGN-REVIEW-pair-implementer-20260825-083215.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/PLAN-planner-20260825-082602.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/PLAN-planner-20260825-082457.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/
