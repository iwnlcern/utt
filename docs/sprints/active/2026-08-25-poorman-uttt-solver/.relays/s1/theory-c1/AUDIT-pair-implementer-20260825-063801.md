ROLE: Pair Implementer
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: theory-c1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: yes — the exact nonterminal p=T semantics and any accepted continuous approximation of fixed-point play require an operator ruling before the engine math lock
FROM: theory.implementer
TO: s1.orchestrator-planner
CC: s1.orchestrator-reviewer
SUBJECT: AUDIT return — Poorman recurrence supported, variant and fixed-point boundaries still open
IN_REPLY_TO: theory-c1/AUDIT-orchestrator-planner-20260825-062558.md

## Scope and result

This was an independent read-only audit. No pair findings were consulted. The repository's product surfaces were not edited or prototyped; only this required relay and its INDEX row are filed.

The standard continuous Poorman recurrence is supported, including the displayed critical-bid algebra. The sprint variant is not yet math-lockable: the literature found does not directly prove the partizan `a > b` branch, chip-valued draw terminal, or exact integer-budget reduction to a budget-independent `T(s,h)`. Those need the Stage-1 continuous oracle, an exact discrete-budget oracle, and one operator ruling at the knife edge.

## Published-source findings

1. Standard continuous Poorman average — confirmed for the ordinary graph-game model (E1).
   - Lazarus, Loeb, Propp, Stromquist, and Ullman define `avg_P(x,y)=y/(1-x+y)` for `0 <= x <= y` and prove existence of Poorman costs. Source: https://www.cs.umd.edu/~gasarch/BLOGPAPERS/richman.pdf, section 3; DOI record: https://doi.org/10.1006/game.1998.0676.
   - With the spec's `a=min T(X-child)` and `b=max T(O-child)`, this is exactly `T=b/(1-a+b)` when `a<=b` (E1 derivation against design lines 47-50).
   - Normalizing pre-bid total budget to 1, an equal critical bid `r` gives X-win constraint `p >= a+r(1-a)` and O-win constraint `p >= b(1-r)`. Equating them gives `r=(b-a)/(1-a+b)` and `T=b/(1-a+b)` (E1 derivation).
   - A read-only `fractions.Fraction` check covered 968 rational `0<=a<=b<=1` cases and verified both post-payment equalities and `a<=T<=b` (E2 command output: `poorman_identity_cases=968`).

2. Threshold existence and equality behavior — standard reachability support, variant gap remains (E1).
   - Avni, Henzinger, and Ibsen-Jensen state that threshold ratios exist for Poorman reachability games and explicitly note that the winner at the exact threshold depends on bidding-tie resolution. Source: https://arxiv.org/abs/1804.04372 and author-hosted paper https://livrepository.liverpool.ac.uk/3029560/1/arxiv.pdf.
   - Avni, Henzinger, and Zikelic restate the continuous double-reachability formula `Th(v)=Th(v+)/(1+Th(v+)-Th(v-))`; their graph model gives both players the same neighbor set, so its min threshold cannot exceed its max threshold. Source: https://arxiv.org/abs/1905.03835.
   - UTTT is partizan at the state-transition level: X-winning and O-winning auctions place different marks and therefore have different child sets. Published same-neighbor reachability results do not by themselves prove the `a>b` case (E1 comparison to design lines 22-35 and 47-53).

3. Zero-bid zugzwang branch — algebraically coherent but not closed by the cited literature (E1/E2).
   - When `a>b`, the standard critical-bid expression is negative, so the positive-bid equilibrium construction is inapplicable. Each player can bid zero to avoid voluntarily taking the unfavorable move; the deterministic tie owner is then forced to move. This yields threshold `a` when `h=X` and `b` when `h=O` (E1 one-step minimax derivation).
   - Read-only exact-rational checks exercised `(a,b)=(3/4,1/4)` and `(2/3,1/3)` for both tie owners (E2 command output recorded during audit).
   - This is a derivation for this ruleset, not a published theorem located in the cited Poorman graph-game papers. Stage-1 must prove it by exhaustive one-step matrices and recursive small games before the engine consumes it.

4. Chip-valued draw terminal — correct as a boundary definition, not yet a published extension theorem (E1).
   - At an already-terminal drawn board, design lines 33-38 make X win for `p>1/2`, O win for `p<1/2`, and draw at equality; therefore `T=1/2` is the correct strict-threshold boundary (E1 direct rules proof).
   - The continuous backward algebra accepts boundary numbers in `[0,1]`, but the located existence/strategy results use fixed reachability targets. No source found directly proves pure-strategy sufficiency and uniqueness for a terminal payoff whose winner depends on remaining budgets (E1 literature-scope comparison).
   - The 2023 discrete-Poorman paper specifically treats exact leftover-budget objectives as future/complex-objective territory rather than as a corollary of its reachability result. Source: https://research-explorer.ista.ac.at/download/14518/14529/2023_FAIA_Avni.pdf, conclusion.

5. Fixed-point play is not exactly captured by a budget-independent continuous threshold (E1).
   - Avni, Meggendorfer, Sadhukhan, Tkadlec, and Zikelic define discrete threshold as a function `T_v(B_2)`, prove pure determinacy for their standard reachability/tie model, and show that continuous ratios only bound/approximate discrete thresholds on DAGs. Source: https://arxiv.org/abs/2307.15218; official PDF: https://research-explorer.ista.ac.at/download/14518/14529/2023_FAIA_Avni.pdf.
   - Their Pipe theorem is evidence that granularity error shrinks with budget; it is not an exact identity, and its hypotheses do not directly include partizan children plus the chip-valued terminal (E1).
   - Therefore design lines 20-21 and 52-53 are defensible as a continuous-search approximation away from the boundary, but not as an exact statement of canonical 10^9-unit play. The exact outcome near `p=T` can depend on both integer stacks/total scale, not only `(s,h)` (E1 inference from the discrete threshold function).

6. Tie state must be updated from the actual mover, not blindly toggled (E1).
   - The rule is `h = player who did not move last` (design lines 24-28). After winner `w` moves, the child state is `h'=opponent(w)`. If the previous last mover wins again, `h` stays the same; it toggles only when the previous tie owner moves (E1 direct state-transition proof).
   - The both-zero chain alternates because every zero tie makes `w=h`, but paid-bid consecutive moves do not imply unconditional alternation. Any implementation of line 52 as `h = !h` after every ply would be wrong.

7. Regret matching is usable only with an explicit zero-sum convergence contract (E1).
   - Hart and Mas-Colell's original regret-matching result converges empirical play to correlated equilibrium, not generally a last-iterate Nash equilibrium. Source: https://www.ma.huji.ac.il/~hart/abs/adapt.html.
   - Zinkevich et al. show that in a two-player zero-sum game, if both players' average regret is below epsilon, the average strategy is a `2*epsilon` equilibrium. Source: https://papers.nips.cc/paper_files/paper/2007/file/08d98638c6fcd194a4b1e6992063e944-Paper.pdf, theorem 2.
   - For the proposed 24-48-action root matrix, the design must return averaged strategies and report exploitability/regret. A small linear program is preferable as the deterministic Stage-1/reference oracle; regret matching can remain the time-bounded engine method (E1 recommendation).

## Required rules-fixture edge cases

Each item is an owned theory-fixture obligation unless explicitly routed otherwise.

- First auction: both possible coin outcomes, center-local-board-only legal moves, and replay-stable recording of the realized coin result (E1, design lines 24-29).
- Tied bids: winner pays the tied amount; non-winner pays zero; winner's intended move is applied and loser's intended move ignored (E1, lines 22-28).
- `0-0` after a nonterminal state: tie owner moves; child tie owner is the opponent of the actual mover (E1).
- Both budgets zero: at least a multi-ply chain proving ordinary alternation until termination, including forced-board routing (E1, line 35).
- Same player wins consecutive paid auctions: prove `h` persistence when that player was also the previous mover (E1 finding 6).
- `a<b`, `a=b` with zero critical bid, and `a>b` with both `h=X` and `h=O`; include expected `T`, critical bid, chosen child, and child tie owner (E1/E2 findings 1 and 3).
- One stack zero, both stacks positive, exact tied all-in, and a winning all-in that creates the both-zero state (E1 rules coverage).
- Forced board open; forced board closed by a win with empty cells left; forced board closed by fullness; each free-choice case must enumerate every empty cell in every open board (E1, lines 30-31).
- Local win closes immediately; local full draw closes; macro win terminates immediately before another auction (E1, lines 30-34).
- Nine-board closure with X chip lead, O chip lead, and exact equal chips; compare chips after the auction winner's payment and move (E1, lines 23 and 33-38).
- Representable `p=T`, one unit above, and one unit below at several total-budget scales; continuous result and exact discrete result must be separate expected fields (E1 finding 5).
- Negative contract fixtures after harness protocol lock: winning response lacks a move, winning move is illegal, and intended move targets a closed/non-forced board. Ownership routes to harness, with theory supplying canonical legality expectations (E1 boundary routing).

## Duplicate / already-built gate

- Local repository: 24 tracked files; `git ls-files` reports zero files under `theory/`, `engine/`, `referee/`, and `ui/`. Local implementation remains absent (E2 command proof).
- Develin and Payne completely analyze ordinary 3x3 bidding Tic-Tac-Toe for continuous and discrete Richman payment, including continuous threshold `133/256`. This is valuable move-tree and tie/granularity prior art, but its transfer-to-opponent payment rule is not a target-value oracle for Poorman. Source: https://doi.org/10.37236/357 and https://arxiv.org/abs/0801.0579 (E1).
- Kalai reports an informal Poorman adaptation of ordinary auction Tic-Tac-Toe with an opening budget threshold near 101.84 against 100. It is useful as a reproduction target, not authoritative proof or code. Source: https://gilkalai.wordpress.com/2013/07/09/auction-based-tic-tac-toe-solution/ (E0 until independently reproduced).
- Avni et al. report a Java implementation of exact discrete Poorman threshold algorithms, but the paper says code is available on demand; no public repository was located. Its symmetric graph/reachability model still needs adaptation. Source: https://research-explorer.ista.ac.at/download/14518/14529/2023_FAIA_Avni.pdf (E1).
- `nelhage/ultimattt` is an open-source Rust ordinary-UTTT minimax/solver implementation. It can serve as an external board/routing comparison, but has no auction budgets, Poorman recurrence, chip-valued terminal, or this tie rule. Source: https://github.com/nelhage/ultimattt (E1).
- Indexed searches found no drop-in open-source exact solver for this Poorman auction-TTT/UTTT variant. This search is not proof of nonexistence; retain the independent implementation requirement (E1 bounded-search result).

## Python toolchain

- `python3 --version` -> `Python 3.14.6` (E2).
- `sys.executable` -> `/opt/homebrew/opt/python@3.14/bin/python3.14` (E2).
- `numpy.__version__` -> `2.5.2` (E2).
- `pytest.__version__` and `python3 -m pytest --version` -> `9.0.2` (E2).
- Standard-library `fractions.Fraction` is available and was used for the recurrence identity check (E2).

## Boundary contract

Writes: future canonical theory fixtures and continuous/discrete Stage-1 oracle outputs; no such product artifact written in AUDIT.
Reads: approved rules/design, integer budgets, realized first-move coin/tie owner, and published threshold results.
Target entity: theory math lock and the versioned canonical rules-fixture corpus.
Downstream consumer: engine search/TT/root bidder, harness referee, and UI analysis tests.
Contract: fixtures must carry board, forced board or ANY, budgets, last mover or realized tie owner, legal moves, closure/terminal result, continuous threshold/critical bid where defined, and separately named exact-discrete expectations.
Proof: E2 cross-check of continuous rational DP against exact integer-budget matrices; E2 consumer tests in Python/C++/TypeScript against the same versioned fixtures.
No-consumer action: reject any fixture field or oracle output with no named engine, harness, or UI assertion.

## Finding disposition map

- F1 standard recurrence -> verified closure for `a<=b`; evidence: Lazarus section 3 plus 968-case Fraction identity check.
- F2 partizan `a>b` -> owned obligation at theory Stage-1 continuous proof/oracle gate; evidence: one-step derivation and named fixtures above.
- F3 chip-valued terminal -> owned obligation at theory math-lock proof plus discrete oracle gate; evidence: direct terminal rule and literature-scope gap.
- F4 integer granularity/knife edge -> owned obligation at exact discrete-budget oracle gate and operator ruling; evidence: Avni 2023 threshold-function/Pipe result.
- F5 tie-state transition -> owned obligation in canonical rules fixtures and theory solver state transition; evidence: design lines 24-28.
- F6 regret solver contract -> overlap edge to engine design/math lock; evidence: Hart-Mas-Colell and Zinkevich et al.
- F7 external prior art -> verified closure of the duplicate-search gate for this audit; evidence: Develin-Payne, Avni et al., Kalai, and `nelhage/ultimattt` entries above.
- F8 local toolchain -> verified closure; evidence: Python/numpy/pytest E2 command outputs above.

## Four-bucket verdict

PRIMARY_BUCKET: still-open
still-open: no local solver or fixture implementation exists, and the partizan zugzwang, chip-valued terminal, exact-discrete boundary, and knife-edge semantics are not fully discharged.
already-closed: the standard continuous `a<=b` Poorman average and critical-bid algebra are established; Python/numpy/pytest prerequisites are available.
product-overlapped: Richman Tic-Tac-Toe analyses, the Avni discrete-Poorman graph algorithm, and ordinary UTTT solvers cover useful subproblems but not the target ruleset.
recommended-next: reconcile both independent audits, obtain the `p=T` ruling, then have theory Planner design a two-oracle Stage-1 package: exact-rational continuous DP plus exact integer-budget brute force, with the named fixtures and an LP reference for root matrices.

## Operator-judgment items

1. Define the authoritative nonterminal behavior at exactly representable `p=T`: forced X win, forced O win, draw/non-win for both, or tie-mechanism-dependent classification. Published work leaves exact-threshold outcome to tie resolution; design lines 44 and 59 deliberately leave equality open.
2. If the engine may treat canonical 10^9-unit play as continuous away from the root, approve an explicit error/tolerance and an exact-discrete fallback zone. Without that approval, fixed-point gameplay remains authoritative and budget-sensitive state cannot be collapsed to `(s,h)` for exact claims.

ACTIONS_GIT_REF: read-only audit on product surfaces; governance-only relay and INDEX row are the sole authored artifacts, with all concurrent foreign paths preserved in FINAL_GIT_STATUS_SHORT
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
?? .claude/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-pair-implementer-20260825-063848.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/engine-c1/AUDIT-planner-20260825-063250.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-pair-implementer-20260825-063616.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/harness-c1/AUDIT-planner-20260825-063841.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c1/AUDIT-pair-implementer-20260825-063801.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-pair-implementer-20260825-063913.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/ui-c1/AUDIT-planner-20260825-063204.md

Not done: no product edits, scaffolding, design lock, plan, branch, commit, PR, merge, or publication.
Next requested action: orchestrator reconciliation of both independent theory audits and routing of the two operator-judgment items before theory DESIGN.
