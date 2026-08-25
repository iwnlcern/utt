ROLE: Pair Implementer
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: engine-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — orchestrator reconciliation can proceed; named operator decisions gate later DESIGN choices
FROM: engine.implementer
TO: s1.orchestrator-planner
CC: s1.orchestrator-reviewer
SUBJECT: Independent AUDIT return — engine feasibility, toolchain, representation, and prior art
IN_REPLY_TO: engine-c1/AUDIT-orchestrator-planner-20260825-062558.md

## Independence and scope

- This is the `engine.implementer` independent return; I did not inspect the contents of the separately filed `engine.planner` return. Evidence E0 (process claim) — this session's action history; the INDEX row was observed only to preserve append order.
- The repository was inspected read-only; no source, test, scaffold, branch, commit, or plan was created. The only authored paths are this required AUDIT relay and the required INDEX append. Evidence E2 — `git status --short` in `FINAL_GIT_STATUS_SHORT` plus the action reference below.
- Repo production state is greenfield: branch `main` at `c7fb088`, 24 tracked files, and `engine/` absent. Evidence E2 — `/usr/bin/git branch --show-current`; `/usr/bin/git log -5 --oneline --decorate`; `/usr/bin/git ls-files | wc -l`; `[ -d engine ]` probe, output `engine/: absent`.

## Four-bucket duplicate verdict

PRIMARY_BUCKET: still-open
still-open: The Poorman-bidding C++ engine, budget-aware root policy, threshold backup, protocol adapter, and tests are absent from this repository; new work is required after the relevant gates. Evidence E2 — repo-shape commands above; E1 — design spec lines 71-80 and roadmap lines 10-24.
already-closed: Standard alternating-turn UTTT representation and search techniques already exist externally in `nelhage/ultimattt` (Apache-2.0) and `TheGustafson/ai-ultimate-tictactoe` (MIT), including compact bitboards, mini-board tables, iterative deepening, TT, tactical ordering, and threat pruning; they do not close this product. Evidence E1 — https://github.com/nelhage/ultimattt/tree/9193fbb2aec97acae1338a2aaedb5f9e9a354444 and https://github.com/TheGustafson/ai-ultimate-tictactoe/tree/cf7c006e5cb2aea1d2352adf1fde85a87048a2ee.
product-overlapped: External prior art substantially overlaps the standard-UTTT rules/search substrate, but not sealed simultaneous Poorman bidding, the specified first-move/tie/budget rules, threshold backup, or JSONL contract. Evidence E1 — external sources above compared with design spec lines 19-60 and 76-98.
recommended-next: Proceed to DESIGN with a clean-room C++ rules core and a benchmarkable Position/table boundary; keep search-math decisions gated on theory, and route protocol decisions through the harness-owned contract. Treat all prior art as reference-only unless the orchestrator separately approves reuse after license review. Evidence E1 — design spec lines 55-69, 93-98, and 109-125; roadmap lines 19-25; incoming audit lines 27-44.

## Toolchain recon

### Host

- Host is arm64 macOS on an Apple M5 Pro with 48 GB RAM. Evidence E2 — `uname -a`; `system_profiler SPHardwareDataType | rg '^      (Chip|Memory):'`, output `Chip: Apple M5 Pro`, `Memory: 48 GB`.

### Installed tools

- Apple compiler is Apple Clang 17.0.0 (`clang-1700.4.4.1`), target `arm64-apple-darwin25.5.0`. Evidence E2 — `/usr/bin/clang++ --version`.
- Homebrew LLVM is present at `/opt/homebrew/opt/llvm/bin/clang++`, Homebrew Clang 22.1.8, target `arm64-apple-darwin25.5.0`. Evidence E2 — `/opt/homebrew/opt/llvm/bin/clang++ --version`; `brew list --versions llvm`, output `llvm 22.1.8`.
- CMake 4.3.1 and Ninja 1.13.2 are present in `/opt/homebrew/bin`. Evidence E2 — `cmake --version`, `ninja --version`, and `brew list --versions cmake ninja`.

### Effective `-std=c++2c` support

- Both compilers accept `-std=c++2c`, advertise `__cplusplus 202400L`, and pass a warning-clean constexpr `std::array` syntax probe. Evidence E2 — stdin-only commands `<compiler> -std=c++2c -Wall -Wextra -Werror -fsyntax-only -x c++ -`; both output `PASS`.
- Both compile and link a stdin-only `std::expected` plus `std::println` program under `-std=c++2c`. Evidence E2 — `<compiler> -std=c++2c -Wall -Wextra -Werror -x c++ - -o /dev/null`; both output `PASS (expected + println compile/link)`.
- The environment does not provide complete C++26 library support: both standard libraries fail `#include <inplace_vector>` with `fatal error: 'inplace_vector' file not found`. Evidence E2 — the same stdin-only compile/link probe under `-std=c++2c`.
- Homebrew Clang uses `/opt/homebrew/Cellar/llvm/22.1.8/include/c++/v1`; Apple Clang uses the Xcode SDK libc++ headers. Evidence E2 — each compiler's `-std=c++2c -v -E -x c++ -` header-search output.
- Realistic ceiling: use Homebrew Clang 22.1.8 as the canonical compiler, C++2c language mode, and only feature-tested library facilities; describe the environment as C++23 library coverage plus partial C++26, not full C++26 conformance. Evidence E2 — the positive and negative probes above. This is not a silent downgrade: the build still targets `-std=c++2c` as required by design spec lines 71-74.

## Architecture feasibility verdict

### Rules core and local tables

- The proposed two 9-by-9-bit cell masks plus per-mini-board lookup are feasible and appropriately small for this host. `3^9` is 19,683 legal ternary local states; the exact table pattern is independently demonstrated by `TheGustafson`'s `MiniTable`. Evidence E1 — design spec line 76; https://github.com/TheGustafson/ai-ultimate-tictactoe/blob/cf7c006e5cb2aea1d2352adf1fde85a87048a2ee/src/lookup.rs#L4-L35.
- A compact immutable Position is realistic; Nelson Elhage reports a 40-byte UTTT position and documents packed per-row bitmasks, cheap value copies, and whole-mini-board lookup techniques. Evidence E1 — https://minimax.dev/docs/ultimate/efficient-representation/ (especially the mutability, representation, and lookup-table sections).
- Do not lock the physical layout from the phrase `9 x 9-bit locals`: `std::array<uint16_t,9>` per player is simple and direct-indexable, while packed rows or 128-bit masks reduce size but add extraction cost. The DESIGN phase should choose using `sizeof(Position)` assertions and make/copy/move-generation microbenchmarks, not intuition. Evidence E1 — prior-art layouts above; E0 — performance recommendation pending benchmarks.
- Precompute semantic data, not only win masks: closed/result, empties, immediate wins for each player, forks/threats, and local potential can share the same 19,683-state table. Evidence E1 — `TheGustafson` lookup table computes winning/fork moves and potential at lines 88-118 of the source above.

### Threshold search and the 30-second target

- A time-bounded competitive engine is feasible on this machine, but a strong depth or win-rate is not yet proved. The 27-second soft clock leaves referee margin, while M5 Pro/48 GB and compact state/table designs remove obvious platform blockers. Evidence E2 — host/toolchain proof; E1 — design spec lines 13 and 71-81; E0 — strength/throughput remains unbenchmarked.
- The threshold backup has two conditional child sets at every node: the minimum over X moves and maximum over O moves. At a free-choice state with about 70 legal cells, a direct full-width implementation may evaluate about 140 marked children before recursion, not about 70. Evidence E1 — derivation from design spec lines 47-50 and risk line 130. This is the main feasibility pressure.
- Consequently, interval bounds/cutoffs may be required in the first viable search design rather than safely deferred as a later optimization. Move ordering only saves work if the backup can stop after bounds prove the remaining children irrelevant; otherwise it mainly improves anytime selective widening. Evidence E1 — derived from the backup equations at lines 47-50 and the proposed ordering/widening at line 77; E0 — exact cutoff formulation awaits theory lock.
- Iterative deepening should publish only the last fully completed iteration and retain separate conditional PV moves for X-wins-auction and O-wins-auction. Evidence E1 — UI requires both conditional moves at design line 104; an established UTTT implementation preserves completed iterations and a PV at https://github.com/TheGustafson/ai-ultimate-tictactoe/blob/cf7c006e5cb2aea1d2352adf1fde85a87048a2ee/src/search.rs#L94-L205.
- Selective widening needs an explicit contract: ordering, widening schedule, minimum coverage, whether returned `T` is heuristic or a sound interval, and how incomplete nodes are tagged in TT. Without that contract, missing one opponent reply can produce a falsely favorable threshold. Evidence E1 — design spec lines 77-79, 104, and 122-125; E0 — risk analysis.
- Tactical routing analysis is high leverage: macro wins, local wins/blocks/forks, sending the opponent to a critical board, and avoiding free choice should be computed as masks and used for both ordering and safe dominance pruning where proven. Evidence E1 — design spec line 77; https://minimax.dev/docs/ultimate/pruning/ documents critical-square bitmask pruning; `TheGustafson` demonstrates tactical/threat ordering at https://github.com/TheGustafson/ai-ultimate-tictactoe/blob/cf7c006e5cb2aea1d2352adf1fde85a87048a2ee/src/search.rs#L311-L424.
- Exact solution is not a 30-second target: Nelson Elhage reports that an optimized standard-UTTT proof solver handles roughly 20-ply positions in hours on a Ryzen 3900X and estimates the full game vastly larger. Evidence E1 — https://minimax.dev/docs/ultimate/ lines 28-34. The sprint's time-bounded approximate engine remains plausible because it does not claim to solve full UTTT.

### TT key and entry semantics

- After the theory gate, `(cells, normalized forced-board-or-ANY, deterministic tie owner)` appears sufficient as the budget-independent state identity: macro ownership/closure is derivable from cell masks because closed boards receive no further marks, and budgets enter only when translating `T` into root action. Evidence E1 — canonical rules lines 30-35; threshold state lines 42-53; E0 — conditional on theory validating budget independence.
- The proposed one-bit tie owner is not sufficient for the initial protocol state by itself. `last_mover: null` means the first tied auction is resolved by a future coin flip, so neither player deterministically owns the tie yet. DESIGN must choose either a root-only chance split into two one-bit states or a third `UNRESOLVED_FIRST_TIE` value; the protocol must say whether the referee pre-samples and exposes the first tie owner. Evidence E1 — rules lines 24-28 and JSONL example lines 85-96.
- TT entry contents are underspecified even if the key is correct. Entries need at least search depth/horizon, exact/lower/upper or interval semantics, generation/age, completeness/selective-widening status, and both conditional best moves; collision policy must also be explicit. Evidence E1 — design line 77 and UI line 104; prior art stores depth, bound, and best move at https://github.com/TheGustafson/ai-ultimate-tictactoe/blob/cf7c006e5cb2aea1d2352adf1fde85a87048a2ee/src/search.rs#L123-L145 and #L441-L467.
- A fixed-size cache-conscious TT is feasible, but memory bandwidth and replacement policy matter more than maximum allocation on a 48 GB host. Nelson documents four-bucket probing, work-aware replacement, and a one-byte hash index to avoid main-entry reads. Evidence E1 — https://minimax.dev/docs/ultimate/pn-dfpn/tt/ lines 21-31. Start single-threaded/deterministic; add concurrency only after profiling because shared-table synchronization changes the design. Evidence E0 — recommendation.
- A 64-bit Zobrist-only signature admits silent collisions; DESIGN must decide whether competitive search accepts that risk or stores a stronger verification tag/full compact key for fixture-grade exactness. Evidence E1 — https://minimax.dev/docs/ultimate/pn-dfpn/tt/ lines 21-26 explicitly describes the space/collision tradeoff; design acceptance lines 122-125 require exact cross-checks on reachable reference positions.

## Open representation questions for DESIGN

1. Position physical layout: nine `uint16_t` masks per player, packed rows, or paired 128-bit masks; cached macro masks versus derivation; immutable value-copy versus make/unmake. Evidence E1 — design line 76 and prior-art representation sources above.
2. Position logical state: exact encoding of forced `0..8`/`ANY`, terminal normalization, deterministic tie owner, and the unresolved-first-tie sentinel/root chance split. Evidence E1 — rules lines 24-35 and threshold state lines 42-53.
3. Local table contract: ternary-index encoding cost, entry width, legal/invalid-state behavior, and which tactical masks are canonical. Evidence E1 — design line 76 and `TheGustafson` lookup source.
4. Search value type: `double`, fixed point, rational, or interval; NaN/sentinel policy; comparison tolerance; exact `p == T`; conversion of analytic critical-bid fractions to integer units. Evidence E1 — design lines 43-60 and 79; theory gate remains controlling.
5. Budget plumbing outside Position: use unsigned 64-bit intermediates for budget sums/products; define zero-total handling; keep `BudgetContext`, perspective mapping, and candidate root bids out of TT state unless theory disproves budget independence. Evidence E1 — rules lines 20-21 and 35; threshold lines 42-60; E0 — type recommendation.
6. TT contract: key construction and normalization, signature width/collision check, entry depth/bounds/completeness, replacement/aging, two conditional moves, size configuration, and single- versus multi-thread ownership. Evidence E1 — design lines 77 and 104; TT prior art above.
7. Free-choice control: whether widening is approximate or bound-preserving, dual-player coverage schedule, cutoffs, extensions, and completed-iteration fallback at the clock. Evidence E1 — design lines 77-79 and 127-131.
8. Protocol adapter boundary: strict JSONL parser dependency policy; schema version rejection; board/perspective mapping; `last_mover: null`; coordinate validation; legal intended move; bid range; one response per line; stdout reserved for protocol and diagnostics to stderr; malformed input/EOF/time-budget behavior. Evidence E1 — design lines 83-98; contract ownership at lines 93-94.
9. Test seam: Position constructor/import validation, theory-owned fixture format, protocol round-trip corpus, perft/property tests, threshold equality tolerances, deterministic clock injection, and benchmark baselines on this exact laptop. Evidence E1 — design lines 62-69 and 120-125.

## Prior art — reference only

1. Nelson Elhage, `nelhage/ultimattt` and accompanying write-up (Apache-2.0): compact immutable positions, bitboard layout, SIMD/table ideas, Zobrist hashing, proof-search TT design, and critical-square pruning. Evidence E1 — https://github.com/nelhage/ultimattt/tree/9193fbb2aec97acae1338a2aaedb5f9e9a354444; https://minimax.dev/docs/ultimate/efficient-representation/; https://minimax.dev/docs/ultimate/pn-dfpn/tt/; https://minimax.dev/docs/ultimate/pruning/.
2. `TheGustafson/ai-ultimate-tictactoe` at `cf7c006e5cb2aea1d2352adf1fde85a87048a2ee` (MIT): exact 19,683-state mini table, immutable per-board masks, iterative-deepening PVS, depth/bound TT, history/tactical ordering, threat extensions, and handcrafted evaluation. Evidence E1 — https://github.com/TheGustafson/ai-ultimate-tictactoe/tree/cf7c006e5cb2aea1d2352adf1fde85a87048a2ee; repository metadata and source files linked above.
3. Neither source implements this sprint's sealed simultaneous Poorman bidding, first-move center restriction plus unresolved coin flip, budget-determined drawn-board outcome, or threshold backup. Evidence E1 — their documented standard alternating rules compared with design spec lines 19-60. No code reuse is authorized by this audit.

## Boundary contract disposition

- Interop protocol remains harness-owned; this audit proposes questions only and no schema change. Engine is a consumer through a narrow parse/validate/canonicalize adapter. Evidence E1 — incoming audit lines 40-44 and design lines 83-98.
- Rules fixtures remain theory-owned; the engine should expose a rules-core test seam and consume fixture bytes without defining their shared schema. Evidence E1 — design lines 62-69 and roadmap lines 19-24.
- No writer-without-reader or reader-without-writer build is proposed in AUDIT. Evidence E1 — roadmap ownership and design verification lines 120-125.

## Operator-judgment items

1. Decide, with harness ownership routed through the orchestrator, whether the initial coin outcome is pre-sampled/exposed or remains an unresolved chance at the engine root. This affects the protocol and whether Position needs a third tie state. Evidence E1 — rules lines 24-28 and protocol lines 85-96.
2. Decide what the user-visible analysis may claim when selective widening stops before full coverage: point estimate, explicitly approximate estimate, or sound interval. This is product semantics and affects UI acceptance. Evidence E1 — design lines 77-79, 104, and 122-125.
3. Any adaptation/copying from the Apache-2.0 or MIT prior-art repositories requires a fresh orchestrator/operator authorization after license and provenance review; this audit authorizes reference study only. Evidence E1 — incoming audit line 29 and repository license metadata above.

## Escalation scan

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — read-only local game-engine audit
- migration/backfill/destructive-write/canonical-data-repair: no — no data or production writes
- money/inventory/orders/planning/accounting/trust-critical-state: no — game budgets are simulated state, not real money
- AI-or-automation-acts-downstream: no — recommendations only; no downstream actions
- worker/scheduler/queue/retry/async-side-effect: no — no runtime work
- cross-repo/service-contract/generated-schema/shared-API-event: yes — harness-owned JSONL and theory-owned fixtures are shared contracts; no change proposed
- user-visible-control-with-materializer/downstream-consumer: no — no control or materializer work
- test-runtime-role-mismatch: no — no implementation or test execution beyond compiler probes
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — initial coin representation and approximate-analysis labeling require later decisions
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none

OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none — no ceremony downgrade requested

## Actions and verification

ACTIONS_GIT_REF: uncommitted audit-report filing at `docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-pair-implementer-20260825-063848.md` plus own append-only row in `docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md`; no source/test/build edits
Tests / verification: E2 stdin-only compiler syntax/link probes and repo-shape commands; exact-file relay lint `OK`; INDEX exact own row present once at line 29 and non-decreasing against its predecessor; INDEX lint remains red on inherited boot-row inversions at lines 17 and 19 (`061900 < 061944`; `062046 < 062125`) plus a later concurrent foreign append inversion at line 31 (`063841 < 063913`)
Out-of-scope preserved: no `engine/`, source, test, build, design, plan, branch, commit, PR, or shared-contract artifact created or changed
Remaining risk: threshold math, knife-edge behavior, and budget independence remain theory-gated; 30-second strength remains E0 until benchmark/tournament evidence
Next requested action: orchestrator reconciliation of both independent engine audits, then an addressed DESIGN dispatch that preserves the theory and harness contract gates

FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? .claude/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-pair-implementer-20260825-063848.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-planner-20260825-063250.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-pair-implementer-20260825-063616.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-planner-20260825-063841.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/AUDIT-pair-implementer-20260825-063801.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/AUDIT-planner-20260825-064131.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-pair-implementer-20260825-063913.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-planner-20260825-063204.md
