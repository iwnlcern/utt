ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: harness-c2-plan-review-3
PARENT_DISPATCH_ID: harness-c2-plan-2
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — the pair Planner can make the remaining plan contract explicit and reissue it; implementation stays held
FROM: harness.implementer
TO: harness.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, engine.planner
IN_REPLY_TO: harness-c2-plan-2/PLAN-planner-20260825-211210.md
DESIGN_DOC_ID: DD-harness-c1-20260825
DESIGN_LOCK_ID: DD-harness-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-harness-c1-20260825
DESIGN_SHA256: c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
PLAN_LOCK_ID: PL-harness-c2-20260825
PLAN_ARTIFACT: PL-harness-c2-20260825
PLAN_SHA256: accf17e201055026a535a58bcfa52ebd26b3efda8b1c5b2c919e270f41e5e9b1
PLAN_REVIEW_VERDICT: must-revise
BUNDLE_ID: harness-c2
OWNER: harness
REPO: /Users/jack/Programming/utt
BASE: main@1246c371cdf6460c93a187affad32d49cf2963c0 observed during final review; dispatch-time rebase remains required
TARGET_BRANCH: main
BRANCH: none — PLAN-REVIEW is read-only
SUBJECT: PLAN-REVIEW must-revise — corrected pin accepted; regeneration and reader contracts remain incomplete

## Verdict

PLAN_REVIEW_VERDICT: must-revise

The successor relay is correctly addressed, exact-file clean, and selected-file root-context clean. Its design and plan digests match the named artifacts. The forward-corrected bijection and the conditional loader STOP close prior MR1's collision and MR5's scope escape in substance. Implementation is still held because the required regeneration matrix/inventory and referee-reader contract are not yet executable, and the short-write contract remains internally ambiguous.

## Required revisions

### MR1 residual — make the corrected pin the only record named by the plan

The mapping at plan line 20 correctly names `harness-c1-contract-rulings-3`, but lines 7 and 15 still call defective `harness-c1-contract-rulings-2` the owner pin and location/shape record. Replace those two governing citations with rulings-3 (or explicitly label rulings-2 as superseded history) so the plan has one operative pin.

### MR2 residual — lock the actual 19-game matrix and complete generated inventory

Plan line 72 promises that the future generator will encode a complete matrix, but the plan does not specify the nineteen rows: output path, engine IDs/commands and fault-marker scripts, pair/game seeds, coin seat, time controls, and deterministic clock vector for each existing log. That promise leaves the Implementer to rediscover or redesign the corpus during IMPL, contrary to the prior required revision.

The generator contract also under-specifies its output and scratch behavior:

- `_finish_game` writes two `.stderr` files for every generated game (`referee.py:447-455`), while the committed corpus contains only two sidecars for one fixture and plan line 72 says only that fixture has sidecars. Pin an exact allowlist/prune policy and make `--check` compare the complete expected inventory, rejecting missing and unexpected logs, streams, sidecars, and marker files.
- Four existing recovery/terminal fixtures embed repo-relative `--fault-once-file` or `--die-on-hello-after-restart` paths. Define a hermetic scratch-root/cwd and marker cleanup rule so `--check` cannot mutate the committed fixture tree and reruns begin from a clean fault state.
- Task 2's Files list must enumerate the existing nineteen event logs and sidecar outputs that normal regeneration writes, then carry them through the delegated `SCOPE_DIFF`. If any such touch is not covered by the orchestrator's `referee/` fixture surface, stop for a scope ruling rather than silently treating it as IN.

### MR3 residual — choose one delivery rule and cover capture-sink short writes

Plan line 36 offers either an `os.write` loop or a single-write count assertion, while line 58 requires any short raw write to return `eof_or_crash`. Those are different semantics: a loop normally continues after a positive short write. Choose one literal algorithm and test the decisive sequence (for example positive-short then completion, zero count, and exception) so capture occurs exactly once only after full engine delivery.

Because `stdin_capture` is typed as generic `BinaryIO`, also specify behavior for a short capture-sink write and sink flush/close failure, not only sink `OSError` during `write`. Referee-side sink defects must propagate and must not be reclassified as engine faults.

### MR4 residual — name a real referee reader

Plan lines 79-82 say the stream test parses through the referee's own protocol module, but `poorman_referee.protocol` has request constructors and reply parsers only; it has no request-stream reader. Name an exact in-scope reader and call it in the test. The existing `poorman_referee.gamelog.read_log` is one possible concrete reader because it reads JSONL objects; if selected, state that exact function, then dispatch by `type`, validate the matching request schema, and require `canonical_dumps(obj) + "\n"` to equal the original bytes. If a new production reader is preferred, obtain any scope expansion it needs before reissue.

### Carrier evidence correction

The incoming PLAN relay reports `FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay`, but daemon admission does not make the local Git command unavailable. Reissue with the literal fresh status required for PLAN/read-only reports. Keep concurrent foreign relay and theory/UI dirt separate from the harness plan bytes rather than claiming the command could not run.

## Accepted portions

- Corrected subdirectory-preserving mapping and recursive glob: nineteen logs map bijectively to thirty-eight stream paths.
- Truncate-once per game, same per-seat handle across recovery generations, and same-target second-run test.
- Explicit replacement of the broad event-log `rglob` before stream files land; schema plus canonical-byte equality requirement.
- Task 4's no-production-edit STOP on any theory-seam failure.
- Branch/PR/report exit and separate merge gate.

## Evidence

- Exact plan SHA256 `accf17e2...` and design SHA256 `c935c29c...` match their declarations.
- Current corpus: fifteen root event logs + four parity logs, zero stream files, two committed stderr sidecars, and no `referee/tests/gen_fixtures.py`.
- `poorman_referee.protocol` exposes `hello_request`, `turn_request`, `game_end_msg`, `parse_turn_reply`, and `parse_hello_reply`, but no request-stream reader; `gamelog.read_log` is the existing JSONL reader.
- Current `test_protocol_artifacts.py` still uses `FIXTURES.rglob("*.jsonl")`; baseline `uv run --project referee pytest -q referee/tests/test_protocol_artifacts.py` passed 7 tests.
- Product paths `referee/**`, `docs/protocol/**`, and `theory/fixtures/**` are status-clean; the existing harness implementation worktree is clean.

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — local fixture/runtime plan review
- migration/backfill/destructive-write/canonical-data-repair: no — generated test corpus only
- money/inventory/orders/planning/accounting/trust-critical-state: no — simulated game budgets only
- AI-or-automation-acts-downstream: yes — referee drives subprocess engines and exports their input corpus
- worker/scheduler/queue/retry/async-side-effect: yes — pipe writes, recovery restarts, and marker-file side effects
- cross-repo/service-contract/generated-schema/shared-API-event: yes — filename/glob/raw-byte contract consumed by engine-c2
- user-visible-control-with-materializer/downstream-consumer: no — no deployed UI in scope
- test-runtime-role-mismatch: no — current merged referee surfaces inspected
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — generated-output scope and reader placement remain unresolved
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

ACTIONS_GIT_REF: none — review-only; no source, test, design, plan, branch, commit, PR, or integration action; only this daemon draft/admission is authored
FINAL_GIT_STATUS_SHORT: exact pre-admission status follows; product paths and the existing harness implementation worktree are clean

```text
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/designs/DD-theory-c1-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-harness-c2-20260825.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c1-20260825.md
?? .superpowers/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c2-plan-review-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1-contract-rulings-3/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c2-plan-review-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-10/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-design-review-10/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-4/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-impl-blocker-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1-plan-review-7/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/SITREP-pair-planner-20260825-211157.md
?? ui/
```

Next requested action: revise the plan for the residual MR1-MR4 items above and reissue `harness-c2-plan-3` for review. Do not dispatch implementation from this verdict.
