ROLE: Pair Planner
PHASE: PLAN
AUTHORITY: plan-only
DISPATCH_ID: s2-theory-c3-plan-1
PARENT_DISPATCH_ID: s2-theory-c2-design-review-4
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — plan gate is the Implementer plan review; merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/PLAN-orchestrator-planner-20260829-150549.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-theory-c2-20260828
DESIGN_SHA256: 629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c
PLAN_LOCK_ID: PL-theory-c3-20260829
PLAN_ARTIFACT: PL-theory-c3-20260829
PLAN_SHA256: 53951b4d8d59edd0a8875ada15b368b38b5a89b33af52075bc645c9356d9bfc5
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BASE: main@1003b0b
TARGET_BRANCH: main
SUBJECT: PLAN s2-theory-c3 — independent C3 checker + corpus backlog — PLAN-REVIEW REQUEST

## Team theory — theory-c3: PLAN for review

Current scope for the TO addressee: read-only PLAN-REVIEW.
No implementation until a separate dispatch; delegated conditional dispatch authority rides the orchestrator proceed relay (s2-theory-c3-proceed-1) and binds only after your approve + mechanical SCOPE_DIFF all-in.

Locked design: DD-theory-c2-20260828 rev3 (governing design per the proceed relay — its §8 independence boundary, §9 gate/mutant battery, §11 c3 slice; approving review s2-theory-c2-design-review-4; no new DESIGN phase for this bundle).
Plan: docs/sprints/active/2026-08-28-uttt-weak-solve/plans/PL-theory-c3-20260829.md, committed at 1003b0b (sha256 pinned above), 9 tasks.

Scope (locked):

1. Backlog first (Python, corpus final before checker work): P4 multi-reply OPPONENT positive (exact masks and both reply chains specified and paper-verified in the plan), MUT-R22 dropped NON-FIRST-reply mutant, catalogue pinned at 51 rows, MUT-R17 reason reworded.
2. `theory/checker/` — fresh idiomatic C3 project (c3c 0.8.3 pinned, newest stable per G8; toolchain researched: project.json layout, @test/c3c test, stdlib json/file/bitorder/HashMap confirmed; stdlib has NO xxhash, so XXH3-128 is hand-implemented against 12 pinned length-class vectors + a golden-header corpus anchor).
3. Normative check order + reason-code enum (in the plan, load-bearing: the battery's per-mutant expectations depend on it) covering DD §6 header/streaming/end stages and §7 verdict steps with deliverable/subgame modes (GATE-01 predicate unit-tested both branches; GATE-02 = golden verdict subgame-pass/deliverable-fail).
4. CLI + JSON report (verdict/code/root_is_initial/versions; exit 0 pass, 1 reject, 2 usage-or-IO) and the Python conformance battery: 4 positives accept, ALL 44 mutants reject each with exactly its table code, catalogue-coverage cross-check, hard-fail (never skip) when c3c is absent.

Acceptance criteria: the plan's 6 numbered criteria (positives+gates; 44/44 intended-reason rejections; XXH3 vectors + corpus anchor; C3 fixtures-v1 matrix; deterministic backlog artifacts; full suites green with no out-of-list paths).
Out of scope (anti-half-fix): no DD/fixtures/engine//solver//referee/ edits; no c6 scale work (external-memory set, sharding); no rule-leaf validators; explicitly flagged scope note — DD §11's c3 sketch names a resume journal and the alt fixture profile, the binding proceed-relay scope line does not, so both are excluded here and pulling either forward is an orchestrator scope ruling, not a silent addition.
Boundary contract: Writes: theory/checker (verifier of the shared contract) + two new corpus artifacts. Reads: DD, fixtures v1 (frozen), landed corpus bytes. Downstream: the s2 deliverable verification path itself; solver emitter round-trips at solver-c5. Proof: E2 battery + C3 unit suites.
Ceremony: tier stays large; no downgrade requested.
Operator-judgment items: none — G8 (C3, newest stable, idiomatic) and R-s2-2/3 govern; census consumption note from the proceed relay is acknowledged (read-only from solver branch, cited by sha, not copied — relevant to O6, not to this bundle's file lists).

Review lenses I flag proactively:

- The check order and expectation table are the contract's most fragile surface — please adversarially check that each of the 44 rows' intended defect is reachable FIRST under the stated order (the corpus adversary's measured first-failures from the c2 panel are the reference; MUT-R16 maps to E_CHUNK_LAYOUT because file-size/layout validation happens at header stage).
- P4/MUT-R22 constructions: verify the two-reply enumeration claim ((5,7) first, (5,8) non-first) and parity bookkeeping (o[4]={0} avoids the x[4] cell-4 collision).
- C3 API names came from v0.8.3 research (stdlib source-verified); the plan licenses idiomatic call-site adaptation with the contract held fixed — flag if that freedom is too loose anywhere.

Requested response:
PHASE: PLAN-REVIEW, FROM: theory.implementer, TO: theory.planner, PARENT_DISPATCH_ID: s2-theory-c3-plan-1, PLAN_LOCK_ID: PL-theory-c3-20260829, verdict approve | must-revise | reject-narrow | human-decision-required.

ACTIONS_GIT_REF: plan committed at 1003b0b (main), sha256 53951b4d8d59edd0a8875ada15b368b38b5a89b33af52075bc645c9356d9bfc5; this relay; no source/test edits
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
(harness lock only; taken immediately before this filing)
