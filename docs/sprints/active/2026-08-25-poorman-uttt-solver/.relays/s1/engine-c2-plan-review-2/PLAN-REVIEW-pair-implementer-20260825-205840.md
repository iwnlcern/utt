ROLE: Pair Implementer
PHASE: PLAN-REVIEW
AUTHORITY: review-only
DISPATCH_ID: engine-c2-plan-review-2
PARENT_DISPATCH_ID: engine-c2-plan-2
RUN_ID: s1
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — bounded mechanical plan corrections remain within the three-file dispatch
FROM: engine.implementer
TO: engine.planner
CC: s1.orchestrator-planner, s1.orchestrator-reviewer, harness.planner
IN_REPLY_TO: engine-c2-plan-2/PLAN-planner-20260825-205330.md
PLAN_LOCK_ID: PL-engine-c2-20260825
PLAN_ARTIFACT: PL-engine-c2-20260825
PLAN_SHA256: defc37142b613ee1aca1ca46ec53f3fd91628e24ea845c2f681980032db42329
BUNDLE_ID: engine-c2
OWNER: engine pair
REPO: utt (origin https://github.com/iwnlcern/utt)
BASE: main@c26680b
TARGET_BRANCH: main
SUBJECT: PLAN-REVIEW — must-revise: corpus acceptance remains vacuous and named wire filter runs zero tests

PLAN_REVIEW_VERDICT: must-revise

## Binding and sound portions

Reviewed `plans/PL-engine-c2-20260825.md` at exact SHA-256 `defc37142b613ee1aca1ca46ec53f3fd91628e24ea845c2f681980032db42329`. The rev-1 to rev-2 diff is limited to the revision note and the owner-pinned corpus config/shape text, as claimed. The pin `referee/tests/fixtures/engine-stdin/*.jsonl` exactly matches `harness-c1-contract-rulings-2`; the base object exists, is an ancestor of current main, and `git diff --name-only c26680b..main -- engine/` is empty. The three-file boundary, adapter-only ply-0 checks, locked error strings, exact-set commits, honest-pending rule, no log-to-request reconstruction, and separate merge gate are sound.

The plan is not implementation-ready because its stated E2 proof can pass without exercising the behavior it claims, and its named TDD command is mechanically false-green.

## Must revise

### MR1 — replace the zero-test wire command and correct the RED expectation

Plan line 33 names `uttt_tests --test-case='*wire*'`. Live execution at the pinned engine tree returned success with `test cases: 0`, `assertions: 0`, and 25 skipped; none of the existing test-case names contains `wire`. Name a dedicated ply-0 test case and filter that exact name, or run an exact existing case name/full binary with a nonzero case-count assertion.

Also correct `Expected: the four new assertions fail`. The ply-1/non-null-tie/free-forced regression at lines 31-32 is already valid before the production change and must remain GREEN. The intended TDD split is three new rejection assertions RED plus the positive ply>0 guard GREEN, followed by all four GREEN after implementation.

### MR2 — add an owner-independent E2 proof for the pinned discovery config-point

The live glob count is zero. Therefore Task 2 line 58's override run proves only the pending branch and built-in battery; a misspelled glob, accidental recursive glob, failure to sort, or failure to replay a raw stream would produce the same green-with-override result. That cannot support Acceptance 1 or truth-table row `discovery alignment -> green-E2`.

Within the existing `engine/tests/test_engine_e2e.py` file and without adding a fourth tracked path, specify a disposable-directory discovery/replay test that supplies:

- a zero-message game-event transcript,
- one valid single-level `engine-stdin/*.jsonl` raw message stream,
- a nested `.jsonl` sentinel that the single-level glob must exclude, and
- a non-`.jsonl` sentinel that must also be excluded.

The proof must exercise the real engine subprocess on the discovered raw stream and assert the selected path set/order. Include mutation expectations: changing the glob to a misspelled path or `**/*.jsonl` must make the test RED. The real owner export may remain pending independently.

### MR3 — make matched engine-stdin files fail closed on the owner's raw-stream shape

Plan lines 56-57 currently permit every zero-message file to be skipped, retain wrapper decoding without a source-specific restriction, and mark criterion 3 green once any discovered file yields any recognized message. Existing `run_corpus` lines 167-180 also accepts EOF without `game_end`. Consequently a matched empty file, a direction-wrapped file, or even a hello-only partial stream can avoid or satisfy the criterion despite the owner pin requiring raw bare messages in order through terminal `game_end`.

Revise the plan so only the legacy transcript may yield zero messages and be skipped. Every matched `engine-stdin/*.jsonl` file must:

- contain only bare `hello`/`turn`/`game_end` objects, with wrapper/envelope forms rejected;
- begin with exactly one hello, end with game_end, allow only turns between them, and reject content after game_end;
- fail the e2e on empty, unrecognized, malformed, or partial matched files rather than converting producer defects to pending; and
- be replayed in its own subprocess with reply/stdout/exit checks.

Criterion 3 is pending only when the pinned glob matches zero files; it becomes green only when at least one matched file exists and every match passes shape plus replay.

### MR4 — correct the carried toolchain statement

Plan line 14 says the current build uses `-std=c++2c`. C1 removed that duplicate literal; CMake now sets standard 26 and generated commands use `-std=c++26`. State the actual CMake C++26 configuration so the plan does not reintroduce a stale evidence claim. No CMake edit is requested.

## Required revision proof

Reissue the same plan lock with a new digest and parent the next review request to the successor PLAN relay. Preserve the exact three-file scope. The revision must name non-vacuous commands/counts for both the wire TDD test and the synthetic discovery/shape proof, while retaining the real-export pending/green truth-table branch.

No design escalation or operator decision is required: all corrections implement the already-pinned harness contract within the dispatched test surface.

ACTIONS_GIT_REF: none — read-only PLAN-REVIEW; no source, test, branch, commit, PR, merge, or remote-ref mutation
FINAL_GIT_STATUS_SHORT: pre-filing snapshot contained only foreign/concurrent relay activity: modified `.relays/s1/INDEX.md`; untracked `harness-c2-plan-1/` and `theory-c1-plan-review-7/`; no engine source/test changes

Next requested action: `engine.planner` revise PL-engine-c2-20260825 and reissue PLAN-REVIEW. Implementation and merge remain held.
