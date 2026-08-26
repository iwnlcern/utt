# E2 evidence bundle for the Stage-1 math lock

Evidence was generated on branch `theory/stage1-c1` after Task 12 commit
`06a14a4`. The JSON reports beside this file are the canonical outputs of the
commands below, not copies of the earlier disposable calibration run.

## Command transcript

From `theory/`:

```text
$ uv run pytest -q
........................................................................ [ 87%]
..........                                                               [100%]
82 passed in 67.31s (0:01:07)

$ uv run python -m auction_ttt sweep --max-scale 32
# exit 0; wrote crosscheck-N1.json through crosscheck-N32.json

$ uv run python -m auction_ttt sweep --spots 64 128
# exit 0; exact deterministic target population; wrote crosscheck-N64.json
# and crosscheck-N128.json

$ uv run python -m auction_ttt fixtures --check
# exit 0; no diff output
```

Fresh completion-gate rerun after this document was authored:
`82 passed in 64.16s (0:01:04)`, exit 0.

## Population and acceptance law

`scale=N` starts at `(bx,bo)=(N,N)`. Exact reachability is propagated forward
from both conditional root owners through every legal auction payment and mark.
For N<=32, every reachable `(board,h,bx,bo)` is evaluated. The N64/N128 target
set is selected deterministically from all qualifying low-empty N32 states;
`solve_targeted` reconstructs each target's exact scale-specific root-prefix
mask and evaluates every mask-true point, without sampling.

For a nonterminal state with continuous `T=p/q`, current total `M=bx+bo`, and
`E` empty cells, the exact distance is `d=|bx*q-p*M|/q`. In-band means `d<=E`.
Acceptance requires zero masked positive-total disagreements with `d>E`.
Terminal points do not enter the ratio, and reachable `M=0` points are counted
separately. Off-mask disagreements are diagnostic only and cannot widen the
mask.

All 34 reports have zero out-of-band mismatches. No run raised
`NonDeterminacy`. `R_mis` is `1/3` at every scale; for N>=2 its attaining state
is `(.OOOOXXXO,O,1,0)`. The maximum in-band ratio is `1` for N>=2. The locked
doubling comparisons 8→16, 16→32, and 64→128 produced no anomaly finding.

## BandReports

`Pop` is masked positive-total population, `M0` is the excluded reachable
both-zero count, `Off` is off-mask diagnostic disagreements, and `Targets` is
the number of deterministic `(board,h)` spot targets.

| N | Mode | Out | R_mis | Max in-band | In-band fraction | Pop | M0 | Off | Targets | Anomaly |
|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| 1 | exhaustive | 0 | 1/3 | 2/3 | 1/1 | 34018 | 15868 | 4898 | 0 | |
| 2 | exhaustive | 0 | 1/3 | 1/1 | 14271/14291 | 114328 | 19656 | 4018 | 0 | |
| 3 | exhaustive | 0 | 1/3 | 1/1 | 120197/120975 | 241950 | 21008 | 2920 | 0 | |
| 4 | exhaustive | 0 | 1/3 | 1/1 | 33994/34499 | 413988 | 21256 | 2242 | 0 | |
| 5 | exhaustive | 0 | 1/3 | 1/1 | 305507/314425 | 628850 | 21268 | 2772 | 0 | |
| 6 | exhaustive | 0 | 1/3 | 1/1 | 210983/221565 | 886260 | 21268 | 2930 | 0 | |
| 7 | exhaustive | 0 | 1/3 | 1/1 | 182891/197701 | 1186206 | 21268 | 3512 | 0 | |
| 8 | exhaustive | 0 | 1/3 | 1/1 | 170729/191086 | 1528688 | 21268 | 4106 | 0 | |
| 9 | exhaustive | 0 | 1/3 | 1/1 | 273433/318951 | 1913706 | 21268 | 4394 | 0 | |
| 10 | exhaustive | 0 | 1/3 | 1/1 | 958619/1170630 | 2341260 | 21268 | 4900 | 0 | |
| 11 | exhaustive | 0 | 1/3 | 1/1 | 1097319/1405675 | 2811350 | 21268 | 5522 | 0 | |
| 12 | exhaustive | 0 | 1/3 | 1/1 | 411707/553996 | 3323976 | 21268 | 5890 | 0 | |
| 13 | exhaustive | 0 | 1/3 | 1/1 | 457511/646523 | 3879138 | 21268 | 6384 | 0 | |
| 14 | exhaustive | 0 | 1/3 | 1/1 | 1509451/2238418 | 4476836 | 21268 | 7120 | 0 | |
| 15 | exhaustive | 0 | 1/3 | 1/1 | 235096/365505 | 5117070 | 21268 | 7366 | 0 | |
| 16 | exhaustive | 0 | 1/3 | 1/1 | 296809/483320 | 5799840 | 21268 | 7824 | 0 | |
| 17 | exhaustive | 0 | 1/3 | 1/1 | 1915583/3262573 | 6525146 | 21268 | 8584 | 0 | |
| 18 | exhaustive | 0 | 1/3 | 1/1 | 1024853/1823247 | 7292988 | 21268 | 8942 | 0 | |
| 19 | exhaustive | 0 | 1/3 | 1/1 | 2183309/4051683 | 8103366 | 21268 | 9390 | 0 | |
| 20 | exhaustive | 0 | 1/3 | 1/1 | 1158173/2239070 | 8956280 | 21268 | 10060 | 0 | |
| 21 | exhaustive | 0 | 1/3 | 1/1 | 349924/703695 | 9851730 | 21268 | 10372 | 0 | |
| 22 | exhaustive | 0 | 1/3 | 1/1 | 2582017/5394858 | 10789716 | 21268 | 10868 | 0 | |
| 23 | exhaustive | 0 | 1/3 | 1/1 | 2714874/5885119 | 11770238 | 21268 | 11582 | 0 | |
| 24 | exhaustive | 0 | 1/3 | 1/1 | 949071/2132216 | 12793296 | 21268 | 11894 | 0 | |
| 25 | exhaustive | 0 | 1/3 | 1/1 | 2979763/6929445 | 13858890 | 21268 | 12346 | 0 | |
| 26 | exhaustive | 0 | 1/3 | 1/1 | 311230/748351 | 14967020 | 21268 | 13062 | 0 | |
| 27 | exhaustive | 0 | 1/3 | 1/1 | 3244415/8058843 | 16117686 | 21268 | 13350 | 0 | |
| 28 | exhaustive | 0 | 1/3 | 1/1 | 241187/618246 | 17310888 | 21268 | 13862 | 0 | |
| 29 | exhaustive | 0 | 1/3 | 1/1 | 3508845/9273313 | 18546626 | 21268 | 14628 | 0 | |
| 30 | exhaustive | 0 | 1/3 | 1/1 | 1820527/4956225 | 19824900 | 21268 | 14784 | 0 | |
| 31 | exhaustive | 0 | 1/3 | 1/1 | 3773279/10572855 | 21145710 | 21268 | 15244 | 0 | |
| 32 | exhaustive | 0 | 1/3 | 1/1 | 488175/1406816 | 22509056 | 21268 | 15918 | 0 | |
| 64 | targeted | 0 | 1/3 | 1/1 | 7376249/41873246 | 83746492 | 20116 | 5884 | 20332 | |
| 128 | targeted | 0 | 1/3 | 1/1 | 15019153/166151390 | 332302780 | 20116 | 11852 | 20332 | |

The normative per-scale records are `crosscheck-N1.json` through
`crosscheck-N32.json`, plus `crosscheck-N64.json` and
`crosscheck-N128.json` in this directory.

## Independent oracle and regression evidence

- Continuous: both Richman machinery checks pass exactly—draw-to-O root
  `133/256`, symmetric half-draw root `1/2`; the ordered backup identity grid
  covers at least 968 rational pairs.
- Discrete: pure maximin equals minimax throughout the reported masks. C9a is
  separately checked against the full `(bid,move)` matrix for N<=4 and a seeded
  N12 stratified sample of at least 200 unique near-band, masked-both-zero, and
  uniform masked states.
- C11: the continuous census contains 22,186 `(board,h)` nodes, zero `a>b`
  nodes, and an attained `a=b` seam. The extra-own-mark explanatory lemma is
  attempted but undischarged in `FINDINGS.md`; the census remains the evidence.
- C3: the synthetic exact threshold-payoff harness covers `(3/4,1/4)`,
  `(2/3,1/3)`, and `(1,0)`, both tie owners, at N8/N16/N32/N64. It proves no
  reachability or canonical-terminal claim.
- Fixtures: both hand-authored and generated manifests validate against JSON
  Schema v1; every fixture has non-empty `consumed_by`; generation is
  diff-clean. Backup cases remain `consumed_by: ["theory"]` because no addressed
  MR17 engine-tag acknowledgment landed.

### Kalai/E0 discrepancy

The exact ordinary-ttt3 root is `T_lo=T_hi=1/2`, hence an exact budget ratio of
`1:1`. This does not agree with the E0 contextual target of approximately
`101.84:100`; the discrepancy is material and is not presented as numerical
agreement. The locked exact solver is authoritative for Stage 1 because it
implements the specified conditional-tie-owner rules and exact backward
induction. The E0 value remains comparison context only.

## LP emitted-case certification

Every emitted matrix has entries in `{-1,0,1}` and an exact `Fraction`
certificate. The final value shown is X's matrix payoff.

| Record | Certificate | Path | Value |
|---|---|---|---:|
| root-X-N8 | exact-verified | direct | 1 |
| root-O-N8 | exact-verified | direct | -1 |
| near-band-.........-O-N8-8-8 | exact-verified | direct | -1 |
| root-X-N16 | exact-verified | direct | 1 |
| root-O-N16 | exact-verified | direct | -1 |
| near-band-.........-O-N16-16-16 | exact-verified | direct | -1 |
| root-X-N32 | exact-verified | direct | 1 |
| root-O-N32 | exact-verified | direct | -1 |
| near-band-.........-O-N32-32-32 | exact-verified | direct | -1 |
| synthetic-zugzwang-3-4-1-4-X-N16 | exact-verified | direct | -1 |

The forced-failure seam bypasses float support discovery and independently
executes the exact-rational fallback on matching pennies, returning value 0,
strategies `(1/2,1/2)`, and `exact-fallback-verified`. Failure after that stage
would raise the hard-stop `LPEscalation`; none occurred.

## Findings and ratification boundary

- C8a is discharged in `FINDINGS.md`: rounding the critical bid loses strictly
  less than one unit of threshold surplus per applied mark, so `d>E(s)` is a
  sufficient integer margin on any finite mark-accumulating game DAG. It does
  not classify points inside the band or equality.
- The C11 extra-own-mark lemma is undischarged because the added mark removes a
  potentially extremizing action and changes the played tie state. P3 therefore
  retains the honest no-reachable-zugzwang-witness gap for UTTT.
- P1, P2, and P3 remain `proposed-for-ratification`. This bundle requests
  operator ratification; it does not imply it.

The publication-first ruling permits an unmerged theory PR and math-lock
handoff now. Merge remains gated on both independently owned repairs:
engine-c3's fixture-shape reduction and harness-c3's normative `consumed_by`
filtering. No merge is attempted by this evidence bundle.
