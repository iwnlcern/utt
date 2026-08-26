# SDD ledger — plan: docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-ui-c1-20260825.md

Dispatch: ui-c1-impl-2, direct orchestrator authority, plan rev 4 digest d083f61e1dc1fdc0071933330b4b6494aa6c16fb7fe88b1c704f1b7f52c1705a.
Worktree: /Users/jack/Programming/utt/.worktrees/ui-c1-impl on branch ui-c1-impl.
Rebase: Task 1 replayed onto origin/main@6d5bd299eb5536839150c96abbb488b08dc2f708 as 8fe306b.
Baseline: npm test -- --run (1/1), npm run lint, npm run build all passed at 2026-08-25 21:01 PDT.
Scope: ui/** only for implementation; sprint-tree reads and daemon-owned final report are governance artifacts.
Task 1: complete (commit 8fe306b, prior TDD cycle preserved through clean rebase; baseline green).

## Preflight self-consistency scan

| Task | Tests against implementation and file list | Finding |
|---|---|---|
| 2 | Manifest raw-split test exercises fixture inventory before parser/types exist; all outputs stay under ui/fixtures. | coherent; owner schemas and at least 19 harness conformance JSONL artifacts are landed |
| 3 | Splitter tests directly distinguish terminated, parseable unterminated, and malformed unterminated tails; types and splitter files match task list. | coherent; schema bytes confirm attempts.turns and recovery.hello |
| 4 | Parser tests cover fail-closed validation, tolerated crash tail, missing terminal, version, and unknown keys; files match. | coherent |
| 5 | Literal formatter cases plus property/mutation-oriented float-equality guard cover money behavior; files match. | coherent |
| 6 | Derivation tests cover logged positions, sequential ply, tie owner, closures, terminals; model/derive files match. | coherent |
| 7 | Extension tests cover all recovery associations, non-advancing outcomes, zero-auction, total enums; modifies only Task-6 surface. | coherent |
| 8 | Extraction/series tests cover metadata, degradation, PV gate, gaps, and cursor alignment; files match. | coherent |
| 9 | Board tests prove log-derived closures, overlays, forced/ghost states, disabled-button accessibility; files match. | coherent |
| 10 | Timeline tests prove cursor arithmetic, attempts, recovery ordering, tail, and terminal labels; files match. | coherent |
| 11 | Metrics/budget tests prove full metadata, both-zero totality, selector, unavailable states, favored labels; files match. | coherent |
| 12 | SVG tests cover series gaps, cursor, and accessible summary; files match. | coherent; dataviz skill unavailable in this harness, DD criteria remain binding |
| 13 | Home/recents tests cover file/drop, persistence fallback, sample, and structured errors; files match. | coherent; sample remains gated by Task-2 legality provenance |
| 14 | Reducer/composed tests cover cursor, keyboard, hash, row mapping, PV timing, zero-auction; files match. | coherent; frontend-design skill unavailable in this harness, DD criteria remain binding |
| 15 | Composed acceptance and schema-drift checks consume all earlier surfaces and update README only. | coherent |

## Preflight shared-file and interface scan

| Producer -> consumer | Interface checked | Finding |
|---|---|---|
| 2 -> 3/4/6/7/8/9/10/11/13/14/15 | schema-v1 fixtures and manifest | coherent; provenance must name harness fixture or harness validation command |
| 3 -> 4 | LogEvent types and splitJsonl | coherent; Task 4 owns semantic validation |
| 4 -> 6/7/13/15 | GameRecord, LogError, truncation | coherent |
| 5 -> 8/10/11/12 | Share and unit/percent formatting | coherent; no float equality |
| 6 -> 7/8/9/10/14/15 | ReplayModel, Position, AuctionStep, cursor positions | coherent; Task 7 extends recovery totality without replacing resolved fold |
| 7 -> 10/14/15 | attached and trailing recoveries, terminal shapes | coherent |
| 8 -> 9/11/12/14/15 | AnalysisEntry, TP series, PV_PIN | coherent; PV remains disabled until owner pin |
| 9 -> 14 | Board props and overlays | coherent |
| 10 -> 14/15 | Timeline cursor and rendered recovery/terminal rows | coherent |
| 11 -> 14/15 | MetricsPanel and BudgetBars | coherent |
| 12 -> 14 | TPChart props and tokens | coherent |
| 13 -> 14 | Home onLoaded(GameRecord), recents, sample | coherent |
| 14 -> 15 | wired App/GameView composed surface | coherent |

Ruling: The design says Playwright E2E scope is PLAN-owned, but the approved plan supplies no Playwright dependency, task, exact pin, or acceptance command; execute the explicit E2 Vitest/build gates and report browser E2E as not performed rather than inventing scope — cost if wrong: a browser-only integration defect may remain until a separately planned E3/browser check.
Ruling: For Task 2, use landed harness-generated conformance logs as legal bases and validate every derived UI fixture with the harness-owned Draft 2020-12 schema/replay command; do not add a UI rules validator — cost if wrong: an advisory-info or truncation derivative could preserve schema shape yet miss a semantic legality assumption, so provenance must retain its exact legal base.

Task 2: complete (commits 8fe306b..5723753, review clean).
Task 2 verification: controller reran owner Draft 2020-12 validation plus replay_frames over 13 complete UI fixtures; fixture-manifest focused suite 2/2 passed.
Task 3: minor (deferred): broad budget fields remain TypeScript number; Task 4 owns runtime integer validation.
Task 3: fix round 1/5 (3 addressed, 0 open — bounded Cell type, extensible SeatMap, newline-invalid handoff regression; commits 4c090ca..b0d6369).
Task 3: complete (commits 5723753..b0d6369, review clean).
Task 4: minor (deferred): malformed-interior test checks coordinates but not LogError instance/reason; final review must triage.
Task 4: fix round 1/5 (3 addressed, 0 open — one-based empty-prefix coordinate, nested unknown-key proof, named partial-prefix coverage; commits cfa4140..11e8136).
Task 4: complete (commits b0d6369..11e8136, review clean).
Task 5: complete (commits 11e8136..602509c, review clean).
Task 6: minor (deferred): add explicit fault-single tieOwner regression.
Task 6: minor (deferred): add explicit truncated forwarding regression.
Task 6: fix round 1/5 (1 addressed, 0 open — AuctionStep discriminated union; commits 4c65ca3..0cb3136).
Task 6: complete (commits 602509c..0cb3136, review clean).
Task 7: fix round 1/5 (1 addressed, 0 open — compound ply/trigger_request_id recovery key with fail-closed stale-ID regression; commits f71f956..23e4051).
Task 7: complete (commits 0cb3136..23e4051, review clean).
Task 7 residual risk: live referee/conformance-log E3 validation was not performed; local fixture coverage and explicit E2 gates are green.
Task 8: fix round 1/5 (1 important + 1 minor addressed, 0 open — bound interval contract and focused gate/cursor regressions; commits 37ab47a..4666bbc).
Task 8: complete (commits 23e4051..4666bbc, review clean).
Task 8 residual risk: PV_PIN remains intentionally false until the harness publishes the exact state-hash/convention artifact; all consumers must preserve that gate.
