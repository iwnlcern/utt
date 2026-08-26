# Stage-1 theory findings and proposed engine math lock

This record applies to the canonical rules in the approved Poorman UTTT design
and to the ordinary 3x3 auction-TTT validation game. A threshold `T(s,h)` is the
critical fraction of total live chips required by X. `a` is the minimum
threshold after X wins the auction and chooses a move; `b` is the maximum
threshold after O wins and chooses a move. Non-root tie owner `h` is `X` or
`O`. Exact equality is not silently assigned a continuous winner.

The literature establishes useful symmetric token-game results, but it does
not cover this whole variant: auction winners place different marks, the tie
owner alternates from the actual mover, and a no-macro terminal values leftover
chips. Those extensions are proved or bounded explicitly below.

## C1 — Ordered-child Poorman backup

Status: `proven-here`

For `a <= b`,

`T = b / (1 - a + b)`.

The token-game recurrence is Theorem 4 of Avni and Henzinger's updated bidding
games survey and is also the `avg_P(x,y) = y/(1-x+y)` Poorman recurrence of
Lazarus, Loeb, Propp, Stromquist, and Ullman. Existence and uniqueness of
threshold ratios for continuous Poorman reachability games are treated by
Avni, Henzinger, and Ibsen-Jensen (WINE 2018, arXiv:1804.04372).

The partisan extension follows by induction on remaining empty cells. Once the
auction winner is known, X selects a child attaining `a` and O selects one
attaining `b`; all children are already valued by the induction hypothesis.
The current node is therefore the one-step Poorman game between two threshold
payoffs, so the same algebra applies. Marks only accumulate, making this
backward induction finite and avoiding any fixed-point assumption. The
`fixture:backup-ordered-children-positive-critical` case pins the formula.

References: Avni and Henzinger, *An Updated Survey of Bidding Games on Graphs*,
MFCS 2022, Theorem 4; Avni, Henzinger, and Ibsen-Jensen, *Infinite-Duration
Poorman-Bidding Games*, arXiv:1804.04372; Lazarus et al., *Combinatorial Games
under Auction Play*, Games and Economic Behavior 1999, section 3 and
doi:10.1006/game.1998.0676.

## C2 — Critical bid and post-payment identities

Status: `proven-here`

For `a <= b`, the critical bid as a fraction of combined budget is

`r = (b - a) / (1 - a + b)`.

Substitution gives both required post-payment identities:

- `(T-r)/(1-r) = a` when X pays `r` and moves;
- `T/(1-r) = b` when O pays `r` and moves.

Conversely, solving these identities gives the displayed `T` and `r`; thus the
bid equalizes the two optimally selected successor thresholds. The exhaustive
`Fraction` identity grid checks at least 968 ordered rational pairs. Lazarus et
al.'s Richman optimal-bid derivation is the structural precedent, but this
Poorman closed form is proved here. In the `a > b` branch the reported critical
bid is defined to be zero. See
`fixture:backup-ordered-children-positive-critical` and
`fixture:threshold-a-equals-b-zero-critical`.

## C3 — Zero-bid zugzwang branch

Status: `proven-here`

If `a > b`, both players optimally bid zero. The tie owner must move, so
`T=a` for `h=X` and `T=b` for `h=O`; the critical bid is `0/1`.

One-step proof: suppose `h=X`. At share above `a`, X bids zero. A tied zero bid
forces X to the best X child at threshold `a`; if O bids positively, O pays and
moves to an O child whose threshold is at most `b<a`, which cannot hurt X. At
share below `a`, O bids zero. A zero tie forces X into an X child whose
threshold is at least `a`; a positive X bid only lowers X's post-payment share.
The `h=O` argument is symmetric around `b`. Applying the already-valued child
argument inductively proves the branch on the finite DAG.

No cited paper covers this alternating-tie partisan branch. Its computational
check is deliberately synthetic because C11 finds no reachable ttt3 witness:
exact one-step threshold-payoff games at `(a,b)=(3/4,1/4)`, `(2/3,1/3)`, and
`(1,0)`, both owners, and N=8/16/32/64 have equal pure maximin/minimax, match
the branch outside the one-auction band, and admit `(0,0)` as an optimal bid
pair. These are algebra tests, not canonical terminal or reachability evidence.
The named fixtures are
`fixture:backup-zugzwang-three-quarters-h-x`,
`fixture:backup-zugzwang-three-quarters-h-o`,
`fixture:backup-zugzwang-two-thirds-h-x`, and
`fixture:backup-zugzwang-two-thirds-h-o`.

## C4 — Tie-owner transition

Status: `defined`

After every applied mark, `h' = opponent(actual mover)`. Therefore `h` toggles
only when the current owner wins and moves; consecutive paid wins by one player
preserve the same next tie owner. This is the amended canonical rule, not a
blind per-ply toggle. Root `h=null` is handled only as described in C6. The
behavior is pinned by
`fixture:auction-consecutive-paid-x-wins-preserve-h`,
`fixture:auction-zero-zero-chain-alternates`, and
`fixture:auction-both-zero-alternation-with-routing`.

## C5 — Chip-comparison terminal and backward induction

Status: `proven-here`

When every local board is closed without a macro winner, `T=1/2`: X wins with
more remaining chips, O wins with fewer, and exact equality is a half-half draw.
Macro wins remain `T=0` for X and `T=1` for O.

Proof is by remaining-cell induction. These three terminal values form the
base. At a nonterminal node, C1–C3 uniquely value the one-step auction from
already-valued children, including the ternary terminal. Each move adds one
mark, so recursion reaches a base and never invokes a potentially non-unique
fixed point. Discrete convergence is corroboration, not the proof. Fixtures
`fixture:terminal-all-closed-chip-margin-plus-one`,
`fixture:terminal-all-closed-chip-margin-minus-one`, and
`fixture:terminal-all-closed-chip-margin-zero` pin the three chip comparisons;
`fixture:terminal-final-payment-before-chip-comparison` pins payment ordering.

## C6 — Hidden-coin root is a conditional envelope

Status: `defined`

`null` is valid only before move one. The two oracles solve the conditional
games `T(s,X)` and `T(s,O)` and report
`T_lo=min(T(s,X),T(s,O))`, `T_hi=max(T(s,X),T(s,O))`. If `p>T_hi`, X wins under
either realized coin branch; if `p<T_lo`, O wins under either. Inside the
envelope only the two conditional outcomes are asserted. This package does not
claim a value for the hidden-coin simultaneous game.

The realized rule branches are `fixture:auction-root-zero-tie-coin-x` and
`fixture:auction-root-zero-tie-coin-o`; the two conditional solver records are
`fixture:threshold-root-conditional-h-x` and
`fixture:threshold-root-conditional-h-o`.

## C7 — Monotonicity of the backup

Status: `proven-here`

For the ordered branch `F(a,b)=b/(1-a+b)`, direct differentiation on the unit
square gives
`dF/da = b/(1-a+b)^2 >= 0` and
`dF/db = (1-a)/(1-a+b)^2 >= 0`. In the zugzwang branch the result is one child
value (`a` for `h=X`, `b` for `h=O`) and is monotone in that selected value.
At the branch boundary `a=b`, both definitions equal `a=b`, so interval bounds
compose without a jump. Hence backward-inducted lower and upper child bounds
remain sound engine bounds. The ordered and equality seams are pinned by
`fixture:backup-ordered-children-positive-critical` and
`fixture:backup-equal-children-zero-critical`.

## C8 — A priori band and observed discrete agreement

Status: `computationally-verified`

At initial scale N, every reachable nonterminal point uses its current total
`M=bx+bo`. For exact `T=p/q`, define
`d=|bx*q-p*M|/q` units and `w(s)=E(s)`, the remaining empty-cell count. A point
is in-band iff `d<=w(s)`. The locked acceptance statement is only: at every
tested scale, every reachable point with `d>w(s)` has the continuous-side
classification. `M=0` points are counted but excluded, and terminal states do
not enter `d/w`.

The exhaustive N=8/16/32 calibration had zero out-of-band mismatches. At all
three scales `R_mis=1/3`, attained at `(.OOOOXXXO,O,1,0)`, and maximum in-band
ratio was `1`. In-band occupancy was respectively `170729/191086`,
`296809/483320`, and `488175/1406816`; masked positive-total populations were
1,528,688, 5,799,840, and 22,509,056. Each scale excluded 21,268 reachable
`M=0` points. Off-mask disagreements (diagnostic only) were 4,106, 7,824, and
15,918. No rise anomaly appeared from 8 to 16 or 16 to 32.

The deterministic low-empty target population contained 20,332 `(board,h)`
targets at both N=64 and N=128. Both runs had zero out-of-band mismatches,
`R_mis=1/3` at the same state, and maximum in-band ratio `1`. Their in-band
occupancies were `7376249/41873246` and `15019153/166151390`; masked
positive-total populations were 83,746,492 and 332,302,780; `M=0` exclusions
were 20,116 each; and off-mask diagnostics were 5,884 and 11,852. No 64-to-128
rise anomaly appeared.

The generated knife-edge records
`fixture:threshold-a-equals-b-zero-critical` and
`fixture:threshold-a-less-than-b-after-x-opening` keep continuous rationals and
masked discrete outcomes separate.

### C8a — Integer sufficiency margin

Status: `proven-here`

Outcome: discharged.

Let `e` bound the remaining auctions (the number of empty cells suffices). The
claim is strict: if `bx-T*M>e`, X can force the continuous-side X outcome; if
`T*M-bx>e`, O can force the O outcome. Induct on `e`.

For `a<=b`, let a player bid `k=ceil(r*M)` using C2's `r`. Affordability follows
from `r<=T` for X and `r<=1-T` for O. If X wins and pays, write
`delta=k-r*M`, where `0<=delta<1`. Choosing the `a` child gives

`bx-k-a(M-k) = (bx-T*M) - (1-a)*delta`.

Thus the strict surplus loses less than one unit and remains greater than
`e-1`. If O instead wins with integer bid `j>=k`, every O child threshold is at
most `b`, and `b(M-j)<=b(1-r)M=T*M`, so X's surplus does not shrink. The O-side
argument is symmetric: on an O win the strict deficit loses `b*delta<1`; on an
X win with `j>=k`, every X child threshold is at least `a` and
`a(M-j)+j>=a(1-r)M+rM=T*M`. C7 permits replacing selected extrema by bounded
children. In the `a>b` branch, bid zero and the C3 one-step inequalities lose
no unit. Every applied mark reduces `e` by one, completing the induction.

Therefore the `E(s)` envelope is a theorem-backed sufficient margin at every
integer scale, including 10^9 units. It does not classify points inside the
band or at equality.

## C9 — Pure determinacy on the tested reachable discrete population

Status: `computationally-verified`

For deterministic `h in {X,O}`, actions are `(bid,intended move)` and X utility
is ordered `O-win=0 < draw=1 < X-win=2`. The exact oracle requires pure
maximin=minimax at every reachable state; any inequality raises
`NonDeterminacy` and blocks the math lock. Exhaustive runs through N=32 and the
deterministic N=64/128 target runs completed without that exception. This is a
finite tested-population claim, not an all-scale theorem.

### C9a — Bid-only reduction

Status: `proven-here`

Condition on auction winner and winning bid. The post-auction budgets depend
only on that winner and bid; the successor tie owner is the opponent of that
winner; and the board successor depends only on the winner's intended move.
The loser's bid and intended move cannot affect that child. Hence each winner
can replace its move by the extremizing move for the resulting budget state,
reducing the full `(bid,move)` matrix to the bid-only matrix without changing
pure maximin or minimax. The N<=4 full-matrix sweep and a seeded N=12 sample of
at least 200 unique near-band, masked-both-zero, and uniform masked states match
the reduction exactly.

## C10 — Engine root matrix and regret-reporting contract

Status: `defined`

The engine's candidate-action root matrix is validated against theory's
zero-sum LP value and averaged strategies. Its regret solver must return the
averaged profile and report average regret and exploitability; last-iterate
convergence is not asserted. Zinkevich et al. 2007, Theorem 2 establishes that
two players with average regret at most epsilon induce an averaged profile
that is a `2*epsilon` equilibrium in a two-player zero-sum game. Hart and
Mas-Colell 2000 is the regret-matching origin but proves convergence of
empirical play to correlated equilibrium, not a general last-iterate Nash
claim.

The LP reference uses scipy HiGHS only for support discovery, reconstructs and
certifies the solution exactly with `Fraction`, retries a degenerate support,
then falls back to an independent exact-rational simplex before raising the
hard-stop `LPEscalation`. Both conditional root fixtures
`fixture:threshold-root-conditional-h-x` and
`fixture:threshold-root-conditional-h-o` pin the theory inputs.

References: Zinkevich, Johanson, Bowling, and Piccione, *Regret Minimization in
Games with Incomplete Information*, NeurIPS 2007, Theorem 2; Hart and
Mas-Colell, *A Simple Adaptive Procedure Leading to Correlated Equilibrium*,
Econometrica 2000.

## C11 — No reachable zugzwang in ordinary 3x3 auction TTT

Status: `computationally-verified`

The exhaustive continuous census contains 22,186 `(board,h)` nonterminal nodes,
zero with `a>b`, and at least one with `a=b`. The regression test hard-pins that
count and the empty zugzwang set. The census is why C3 uses synthetic exact
threshold-payoff terminals and why no reachable zugzwang threshold fixture is
manufactured. `fixture:threshold-a-equals-b-zero-critical` records the census
minimum `.OXXOOOXX` equality state.

### C11 lemma — Extra-own-mark monotonicity attempt

Status: `defined`

Outcome: undischarged.

Attempt: order boards by adding an X mark and try to prove that X's threshold
cannot rise (and symmetrically for O) by induction. Corresponding children that
avoid the added cell do satisfy the desired induction comparison. The proof
does not close because adding the mark also removes that cell from both action
sets: in the unaugmented board the missing cell can be the uniquely minimizing
X move, so the minimum over X children has no matching child in the augmented
board. Moreover an actually played extra mark changes `h`, while the static
board comparison wants to hold `h` fixed. Monotonicity of the scalar backup
(C7) cannot repair either action-set or tie-state mismatch.

Thus the lemma remains explanatory intuition, not a proof of `a<=b`. C11 rests
on its exhaustive census only. This does not weaken C3's algebraic proof, but
it preserves the P3 UTTT reachability gap honestly.

## P1 — Exact equality classification

Status: `proposed-for-ratification`

Three boundaries are proposed without conflation:

1. Referee integer arithmetic classifies every realized sequence of bids and
   moves; it does not solve optimal play.
2. The exact discrete oracle classifies optimal play only on the states and
   scales it solved: exhaustive N<=32 and the deterministic N=64/128 target
   population.
3. This package does not supply a complete 10^9-unit in-band optimal classifier
   and assigns no continuous winner at `p=T`. A future exact canonical-scale
   classifier would be a separately authorized artifact.

Calibration supports keeping these statements separate: the equality fixture
`fixture:threshold-a-equals-b-zero-critical` is a draw at its selected equal
budget points, while the conditional root fixtures at equal budgets resolve to
different winners under different `h`.

Ratification ask: accept these equality boundaries and forbid a continuous
equality convention from being presented as exact discrete optimal play.

## P2 — 10^9-unit tolerance and fallback contract

Status: `proposed-for-ratification`

The engine may present a forced continuous-side classification only when the
exact integer distance `|bx-T*M|` is strictly greater than `E(s)` units. Inside
the band it must return interval/estimate quality metadata and use the root
bid-matrix path rather than label a forced winner. At 10^9 combined units,
UTTT's at-most-81-unit band is at most `8.1e-8` of total budget.

C8a discharges the outside-band rule as a sufficient-margin theorem for any
finite mark-accumulating instance, so the 3x3-to-UTTT extrapolation risk for the
outside-band guarantee is removed. C8 supplies calibration rather than the
proof: zero out-of-band errors through exhaustive N=32, `R_mis=1/3`, and no
8-to-16, 16-to-32, or targeted 64-to-128 rise. The rule is tighten-only;
evidence cannot silently widen it. No claim is made for canonical in-band
optimal classification.

Ratification ask: accept `d>E(s)` as the engine's forced-classification gate and
the in-band interval/root-matrix behavior, while retaining the explicit absence
of a canonical in-band exact solver.

## P3 — Zugzwang validation basis

Status: `proposed-for-ratification`

C3 has a one-step matrix proof, a finite-DAG induction, and exact synthetic
tests across the named abstract cases, both tie owners, and four scales. C11
nevertheless establishes that the branch the spec identified as a math-lock
risk has no reachable-state witness in Stage-1. UTTT routing may create a
reluctant-mover state that ordinary ttt3 does not.

Ratification ask: knowingly accept proof plus the synthetic harness as the
Stage-1 validation basis for the engine's zugzwang branch. Accepting P3 accepts
the residual no-reachable-witness gap for real UTTT; it does not convert the
synthetic backup fixtures into canonical game states.

## Source and evidence index

- Avni and Henzinger, *An Updated Survey of Bidding Games on Graphs*, MFCS
  2022, Theorem 4.
- Avni, Henzinger, and Ibsen-Jensen, *Infinite-Duration Poorman-Bidding Games*,
  WINE 2018, arXiv:1804.04372.
- Lazarus, Loeb, Propp, Stromquist, and Ullman, *Combinatorial Games under
  Auction Play*, Games and Economic Behavior 1999,
  doi:10.1006/game.1998.0676; Richman bid pattern in Lazarus, Loeb, Propp, and
  Ullman, *Richman Games*, arXiv:math/9502222.
- Develin and Payne, *Discrete Bidding Games*, section 6,
  doi:10.37236/357. Its Richman payment and ties-to-Bob reduction are machinery
  checks only, not a Poorman target-value oracle.
- Zinkevich et al., *Regret Minimization in Games with Incomplete Information*,
  NeurIPS 2007, Theorem 2.
- Hart and Mas-Colell, *A Simple Adaptive Procedure Leading to Correlated
  Equilibrium*, Econometrica 2000.
- Generated evidence paths are `results/crosscheck-N*.json` and
  `results/EVIDENCE.md`; Task 13 regenerates and records the final canonical
  command transcripts before the math-lock handoff.
