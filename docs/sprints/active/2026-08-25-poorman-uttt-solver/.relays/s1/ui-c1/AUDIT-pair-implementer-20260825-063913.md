ROLE: Pair Implementer
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: ui-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — orchestrator reconciliation can proceed; listed product-semantics decisions must resolve before UI design lock
FROM: ui.implementer
TO: s1.orchestrator-planner
CC: s1.orchestrator-reviewer
IN_REPLY_TO: s1/ui-c1/AUDIT-orchestrator-planner-20260825-062558.md
SUBJECT: AUDIT RETURN — independent UI recon, boundary needs, surfaces, and toolchain
RELAY_PATH: s1/ui-c1/AUDIT-pair-implementer-20260825-063913.md

# AUDIT return — ui.implementer / ui-c1

Audit posture: independent read-only recon. I did not open or consume the paired `ui.planner` return. No product code, scaffold, design lock, plan, branch, commit, PR, merge, deployment, or release was created. — evidence E1 — source: addressed dispatch lines 20-34 and repository status.

## 4-bucket verdict

PRIMARY_BUCKET: still-open
still-open: The tracked repository has no `ui/`, package manifest, TypeScript, or production surface; play, replay, analysis, and both browser-facing boundary schemas remain to be designed and built after their gates. — evidence E1 — source: `git ls-tree -r --name-only HEAD` produced no `ui/`, `package.json`, Vite, or TypeScript paths; design spec lines 100-118.
already-closed: None in this repository. External commit `0c05c6f4605b430b8c1a1d64e6d573894cb1b350` contains reusable concepts for nested-board layout and move-history navigation, but it is neither local implementation nor compatible auction-bidding state. — evidence E1 — source: pinned upstream files cited below.
product-overlapped: Conceptual overlap exists only in the external app's 3×3 CSS-grid board, responsive analysis layout, move list, and previous/next controls; the overlap narrows visual exploration but closes no UTT workspace deliverable. — evidence E1 — source: upstream `BigBoard.module.css` lines 1-5, `AnalysisGame.module.css` lines 1-47, and `AnalysisGameDisplay.tsx` lines 26-77.
recommended-next: Reconcile the protocol/game-log obligations below into the harness-owned pre-lock artifacts, resolve the six operator UX decisions, then dispatch UI DESIGN after the harness protocol lock; retain layout inspiration only and reject source/state reuse. — evidence E1 — source: design spec lines 83-118 and findings UI-A01 through UI-A07.

## Prior-art judgment

Reference inspected: `https://github.com/ultimate-ttt/ultimate-ttt` at commit `0c05c6f4605b430b8c1a1d64e6d573894cb1b350` (`2021-09-11T10:44:43+02:00`, `Prepare for Maintenance Mode`). The checkout remote and commit were verified with `git -C /tmp/uttt-ref remote -v` and `git -C /tmp/uttt-ref rev-parse HEAD`; its worktree was clean. — evidence E2 — source: command output.

Verdict: confirm the approved “layout inspiration only, no code reuse” judgment.

- Keep the conceptual component hierarchy: `BigBoard` maps nine `SmallBoard`s and each local board maps button-backed `Tile`s; nested CSS grids express both 3×3 levels cleanly. — evidence E1 — source: [BigBoard.tsx lines 47-99](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/components/Board/BigBoard/BigBoard.tsx#L47-L99), [SmallBoard.tsx lines 38-92](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/components/Board/SmallBoard/SmallBoard.tsx#L38-L92), and [BigBoard.module.css lines 1-5](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/components/Board/BigBoard/BigBoard.module.css#L1-L5).
- Keep the conceptual analysis composition: a move list, previous/next buttons with arrow-key handling, board highlighting, stacked mobile layout, and side-by-side wide layout. — evidence E1 — source: [AnalysisGameDisplay.tsx lines 26-77](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/views/AnalysisGame/AnalysisGameDisplay.tsx#L26-L77), [ArrowButtons.tsx lines 36-77](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/components/ArrowButtons/ArrowButtons.tsx#L36-L77), and [AnalysisGame.module.css lines 1-47](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/views/AnalysisGame/AnalysisGame.module.css#L1-L47).
- Reject state and interaction reuse: one click directly registers a move, writes a mark, flips `currentPlayer`, recalculates the board, and advances active boards; no auction-resolution state exists between intent and move. — evidence E1 — source: [Game.tsx lines 22-53](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/views/Game/Game.tsx#L22-L53) and [moveValidationSaga.ts lines 15-27](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/state/currentGame/moves/moveValidationSaga.ts#L15-L27).
- Reject replay-model reuse: upstream history stores only player, move number, board, and tile; replay reconstructs alternating-turn board/current-player state from the move prefix. It has no bids, budgets, losing intent, tie owner, coin result, auction reason, or analysis values. — evidence E1 — source: [AppState.ts lines 10-34 and 91-99](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/state/AppState.ts#L10-L34), [AppState.ts lines 91-99](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/state/AppState.ts#L91-L99), and [analysisGameReducer.ts lines 31-52](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/state/analysisGame/analysisGameReducer.ts#L31-L52).
- Reject framework/dependency reuse: the reference pins React 17, `react-scripts` 3.4.4, Redux/Redux Saga, and TypeScript 4.3-era dependencies, while this sprint explicitly calls for Vite/React/TypeScript. — evidence E1 — source: [package.json lines 6-24 and 37-70](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/package.json#L6-L24) and design spec lines 100-105.
- Accessibility concept to keep, implementation to improve: tiles are real buttons with focus-visible treatment, but upstream emulates disabled behavior via `aria-disabled` plus an event guard rather than native `disabled`; the new design should specify true disabled semantics where the control is unavailable. — evidence E1 — source: [Tile.tsx lines 42-64](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/components/Board/Tile/Tile.tsx#L42-L64) and [Tile.module.css lines 21-32](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/components/Board/Tile/Tile.module.css#L21-L32).

## Boundary contract needs before harness protocol lock

The design currently specifies stateless engine JSONL with full state in and only `{bid, move}` out, while separately requiring browser play through a local referee bridge, replayable JSON logs, and five analysis outputs. The exact browser bridge, game-log, and analysis carriers are therefore open contract work rather than UI-local details. — evidence E1 — source: design spec lines 83-104.

### Protocol/local-bridge schema required by UI

- Version and identity: protocol version, game/session id, monotonic turn/revision id, player-to-mark mapping, and perspective rules for `you`/`opp` versus X/O. These prevent stale submission and perspective ambiguity. — evidence E1 — source: design spec lines 42-53 and 85-94.
- Canonical state snapshot: 81-cell board encoding, closed/local-winner state or an unambiguous derivation rule, `forced` board with an explicit ANY representation, both absolute integer budgets, last mover/tie owner, first-turn status, legal moves, terminal status, winner/draw reason, and time allowance/deadline semantics. — evidence E1 — source: canonical rules lines 19-35 and protocol example lines 85-91.
- Human auction intent: one atomic submission carrying turn/revision id, integer bid, intended move, and idempotency/correlation id; validation errors must distinguish stale turn, bid outside `[0, budget]`, illegal destination, closed board, and protocol mismatch. — evidence E1 — source: canonical rules lines 20-31 and exact-tie risk at line 131.
- Sealed lifecycle: explicit `awaiting-human`, `human-locked`, `engine-pending`, `resolved`, and `terminal/error` states; engine bid and losing intended move remain hidden until resolution, then the receipt exposes both bids, both intended moves, winner, resolution reason (`higher-bid`, `tie-owner`, or `first-turn-coin`), paid amount, applied move, and resulting state. — evidence E1 — source: canonical rules lines 22-35 and referee obligation lines 96-98.
- Failure/recovery: structured timeout, engine exit, malformed response, illegal engine action, bridge disconnect, retry/resume, and authoritative-state resync behavior. The browser must not guess whether a sealed submission was consumed. — evidence E1 — source: 30-second constraint at lines 13 and 96, plus the UI bridge requirement at line 102.
- Analysis carrier: a state/turn-keyed payload for threshold `T`, actual share `p`, signed budget margin, critical bid (fraction and integer-unit recommendation with rounding semantics), conditional best move if X wins, conditional best move if O wins, and availability/quality metadata such as depth, bound/exactness, or unavailable reason. The current `{bid, move}` response does not carry these values, so harness/orchestrator must choose a versioned optional protocol envelope or a separate bridge endpoint before lock. — evidence E1 — source: protocol response lines 88-91 versus UI analysis requirement lines 100-105.
- Transport: define how browser events map onto the harness's process-level JSONL contract (HTTP request plus polling, SSE, or WebSocket), including ordering and reconnection semantics. A browser cannot consume a spawned engine's stdin/stdout directly. — evidence E1 — source: stateless process JSONL lines 83-94 versus local bridge line 102.

### Replayable game-log schema required by UI

- Log header: schema version, game id, rules/protocol version, creation time, player/engine identity and versions, X/O mapping, initial budgets and fixed-point scale, initial forced board, and recorded first-turn coin/tie-owner outcome. — evidence E1 — source: canonical rules lines 19-35 and referee/tournament requirements lines 96-98.
- Ordered auction record per turn: turn id, pre-state revision, both sealed bids, both intended moves, resolution reason, auction winner/mover, paid bid, applied move, post-budget totals, next forced board, next tie owner/last mover, timing/timeout data, and resulting terminal status. Losing intent is necessary to explain a simultaneous auction even though only the winner's move is applied. — evidence E1 — source: canonical rules lines 22-35 and replay requirement lines 96 and 103.
- Replay determinism: the log must be self-contained and ordered so a consumer can reconstruct every displayed state without rerunning an engine or coin flip; unknown additive fields should be ignorable under the versioning rule, while invalid/truncated logs receive a structured error. — evidence E1 — source: “replayable JSON game log” at line 96 and UI replay at line 103.
- Analysis attachment: either optional per-turn analysis keyed by state/turn id or a separate analysis stream with the same stable key; the schema must distinguish absent/not-computed from exact, bounded, or estimated values. — evidence E1 — source: analysis requirements lines 42-53, 78-80, and 104.
- Rules-fixture alignment: use the theory-owned fixtures' same board coordinates, forced/ANY representation, legal-move set, closure, terminal, threshold, and critical-bid conventions so UI fixture tests do not translate a third dialect. — evidence E1 — source: design spec lines 62-69 and roadmap lines 21-24.

BOUNDARY CONTRACT:
Writes: atomic human auction intent `{turn/revision, bid_units, intended_move, idempotency_id}` to the referee bridge.
Reads: authoritative bridge state/resolution/error events; versioned game logs; theory rules fixtures; state-keyed analysis payloads.
Target entity: UI play-session state, replay timeline, board highlights, budget display, and analysis panel.
Downstream consumer: referee bridge consumes human intent; UI is the downstream consumer of bridge, log, fixture, and analysis data.
Contract: versioned identities, coordinate/perspective conventions, sealed-auction lifecycle, deterministic resolution events, recovery semantics, and analysis provenance listed above.
Proof: required future E2 schema round-trip/fixture tests across harness writer and UI reader; this AUDIT supplies E1 contract inventory only.
No-consumer action: defer UI implementation until protocol and log consumers have locked fixtures; route missing analysis carrier through `s1.orchestrator-planner`.

## UI surfaces implied by the approved spec

- App shell: three primary destinations—Play, Replay, Analysis—with persistent connection/game status and an explicit route from a completed game to its replay/analysis. — evidence E1 — source: design spec lines 100-105; upstream route separation is conceptual evidence at [App.tsx lines 10-23](https://github.com/ultimate-ttt/ultimate-ttt/blob/0c05c6f4605b430b8c1a1d64e6d573894cb1b350/src/App.tsx#L10-L23).
- Play board: nested 3×3 board; distinguish legal, forced, closed, won, selected-intent, last-applied, conditional-X, and conditional-O cells without relying on color alone; surface whose auction it is, tie owner/first-turn coin state, budgets, and actual share. — evidence E1 — source: canonical rules lines 19-35 and upstream highlight/clickability concepts at `BigBoard.tsx` lines 40-90 and `Tile.tsx` lines 42-64.
- Bid composer: exact integer input paired with a convenience slider, zero/all-in shortcuts, remaining-budget bounds, selected intended move, and one lock/submit action covering both fields. A slider alone cannot express all 1-unit values across a 10^9-unit budget or provide precise keyboard entry. — evidence E1 — source: fixed-point and sealed-submission rules at lines 20-23.
- Sealed pending/resolution: after lock, disable mutation; show pending without leaking the engine bid; after resolution reveal both bids, explain tie/coin outcome and payment, animate only the applied move, update budgets/forced board, and expose recoverable timeout/protocol errors. — evidence E1 — source: canonical rules lines 22-35 and referee duties line 96.
- Replay: import/open a versioned log; auction-oriented timeline; previous/next, direct turn selection, Home/End and arrow-key navigation; board, forced-board, budgets, bids, resolution reason, and applied/losing intents at every turn. — evidence E1 — source: UI replay line 103 plus upstream conceptual move list and key controls in `MoveList.tsx` lines 18-78 and `ArrowButtons.tsx` lines 36-77.
- Analysis: board plus a metrics panel for X-centric `T`, `p`, signed `p-T` margin, critical bid in fraction/units, and two conditional best moves; conditional moves need distinct labels/toggles or overlays because both may target different cells in the same position. — evidence E1 — source: threshold definitions lines 42-53 and UI requirement line 104.
- Responsive/accessibility: wide layout can place timeline/metrics beside the board; narrow layout should stack them while keeping the bid lock and current budgets visible. Board cells, slider/numeric input, timeline, and conditional moves need keyboard and screen-reader names, focus persistence after each resolution, and non-color state cues. — evidence E1 — source: upstream responsive grid lines 1-47 and button/focus concepts in `Tile.tsx` lines 42-64 and `Tile.module.css` lines 21-32.

## Operator-judgment items

1. Live analysis policy: Is threshold/critical-bid guidance visible during human play, or only in replay/post-game analysis? Live guidance materially changes what “play against the engine” means. — evidence E1 — source: play and analysis are separate requirements at design lines 102-104, with no visibility policy.
2. Human sealed-bid flow: Must the human choose the intended move before setting/locking the bid, may either field be edited until one atomic submit, and is a confirmation step required for all-in or unusually large bids? — evidence E1 — source: simultaneous bid-plus-intended-move rule lines 22-23; ordering/confirmation is unspecified.
3. Reveal policy: After resolution, should the losing intended move be shown by default, on demand, or only in analysis? Rules determine application but not presentation. — evidence E1 — source: canonical rules lines 22-28 and replay/analysis requirements lines 103-104.
4. Exact bid control: Choose the slider's coarse step/presets and whether the numeric unit field is primary, since exact equality is strategic but the stack has up to 10^9 units. — evidence E1 — source: rules lines 20-28 and risk line 131.
5. Replay acquisition/persistence: Is s1 local file import/export only, bridge-served recent games, or both? Authentication/cloud history is not in scope and must not be inferred. — evidence E1 — source: “replay game logs” line 103 and absence of persistence requirements in lines 100-118.
6. Metric perspective/precision: Keep theory's X-centric `T`/`p` labels, or transform to human/engine perspective; choose percent/decimal precision and explicit equality language at `p = T`. — evidence E1 — source: threshold definition lines 42-53 and unresolved equality ruling lines 55-60.

No canonical-rule change is recommended. — evidence E1 — source: the UI gaps above are transport, logging, analysis-carrier, and presentation questions; canonical rules lines 19-35 are sufficient for audit consumption.

## Toolchain feasibility

No scaffold was created. Commands were run from `/Users/jack/Programming/utt` and wrote no repository files. — evidence E1 — source: final git status.

- `node --version` → `v25.2.1`; `npm --version` → `11.6.2`. — evidence E2 — source: command output.
- `npm view create-vite@latest version engines --json` → create-vite `9.2.0`, Node `^20.19.0 || >=22.12.0`; `npm view vite@latest version engines --json` → Vite `8.2.2`, same Node range. Installed Node `25.2.1` satisfies both declarations. — evidence E2 — source: command output.
- `npx --yes create-vite@latest --help` completed successfully and listed `react-ts` as an available template. — evidence E2 — source: command output.
- Registry compatibility snapshot: React `19.2.8`; TypeScript `7.0.2` (Node `>=16.20.0`); `@vitejs/plugin-react` `6.1.0` (Node `^20.19.0 || >=22.12.0`, Vite peer `^8.0.0`). — evidence E2 — source: `npm view react@latest`, `npm view typescript@latest`, and `npm view @vitejs/plugin-react@latest` command output.
- Recommendation: the toolchain is feasible; the future design/plan should pin a reproducible Node major and exact package policy rather than silently binding to today's `latest` tags. — evidence E2 — source: successful compatibility queries above; exact versions remain a DESIGN/PLAN choice.

## Finding ledger for reconciliation

- UI-A01 — confirm inspiration-only boundary; explicit rejection of upstream source/state reuse — target: UI DESIGN gate — evidence: pinned upstream files above.
- UI-A02 — define browser bridge lifecycle, identities, state, validation, and recovery before lock — target: harness protocol artifact — evidence: design lines 83-103.
- UI-A03 — define deterministic auction-oriented game-log schema before UI fixtures — target: harness game-log artifact — evidence: design lines 96 and 103.
- UI-A04 — supply a state-keyed analysis carrier absent from current `{bid, move}` response — target: orchestrator-routed harness/engine boundary decision — evidence: design lines 88-104.
- UI-A05 — carry theory fixture coordinate/forced/terminal conventions directly into UI tests — target: theory fixture gate plus UI consumer test — evidence: design lines 62-69 and roadmap lines 21-24.
- UI-A06 — retain Play/Replay/Analysis surface inventory and accessibility requirements — target: UI DESIGN artifact — evidence: UI surface section above.
- UI-A07 — close operator items 1-6 before UI design lock — target: operator decision record — evidence: operator-judgment section above.
- UI-A08 — toolchain feasibility closed at E2; pinning remains plan-owned — target: future UI PLAN/toolchain manifest — evidence: Node/npm/npm-registry commands above.

## Escalation scan

ESCALATION_SCAN:
- authz/tenant/RLS/permissions/secrets: no — no authentication or remote tenancy is specified; replay persistence beyond local scope is explicitly an operator question.
- migration/backfill/destructive-write/canonical-data-repair: no — greenfield read-only audit.
- money/inventory/orders/planning/accounting/trust-critical-state: no — game chip budgets are simulation state, not real money.
- AI-or-automation-acts-downstream: no — engine recommendation/display is game-local; no external action is authorized.
- worker/scheduler/queue/retry/async-side-effect: no — audit only; future bridge recovery semantics are contract requirements, not implemented side effects.
- cross-repo/service-contract/generated-schema/shared-API-event: yes — harness-owned protocol/game-log and theory-owned fixtures are shared contracts consumed by UI.
- user-visible-control-with-materializer/downstream-consumer: yes — sealed bid and intended-move controls feed the referee bridge.
- test-runtime-role-mismatch: no — no runtime or tests exist yet.
- broad-scope-expansion/ambiguous-product-semantics/residual-risk/live-verify-skip: yes — six UX/product-semantics questions require operator decisions before design lock.
ESCALATION_SCAN_RESULT: trigger-present
PRE_SCAN_PRESSURE: none
OPERATOR_WAIVER:
- status: none
- scan_presented_to_operator: no — no ceremony downgrade or waiver requested
- operator_reply_after_scan: none
- valid_waiver: no
WAIVED_RISK_ACCEPTANCE: none

## Actions and final state

ACTIONS_GIT_REF: this audit relay plus exactly this seat's appended INDEX row; no product/source/test edits; external reference inspected read-only at `/tmp/uttt-ref@0c05c6f4605b430b8c1a1d64e6d573894cb1b350`
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? .claude/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-pair-implementer-20260825-063848.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-planner-20260825-063250.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-pair-implementer-20260825-063616.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/AUDIT-pair-implementer-20260825-063801.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-pair-implementer-20260825-063913.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-planner-20260825-063204.md

Tests / verification: E2 toolchain commands above; exact relay lint passes. INDEX lint remains red only on inherited boot-row inversions (`061900` after `061944`, `062046` after `062125`); this seat's corrected `063913` row is unique, correctly ordered, and at the live tail when filed. No product tests exist or were run.
Out of scope preserved: no scaffold, source, design lock, plan, branch, commit, PR, merge, deployment, release, or canonical-rule reinterpretation.
Remaining risk: harness lock could omit the browser bridge, game-log auction data, or analysis carrier; UI design must not proceed past those missing consumers.
Next requested action: orchestrator reconcile UI-A01 through UI-A08, route UI-A02 through UI-A05 to contract owners before lock, obtain operator decisions UI-A07, then dispatch UI DESIGN only after the harness protocol gate is satisfied.
