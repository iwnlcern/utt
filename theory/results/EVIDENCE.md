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


## P1c exact knife-edge sweep

The canonical P1c artifact is `knife-edge-sweep.json` (SHA-256
`49ad457309536c98e86aebe697ff01a0f1c228f4703530c57cca2fec33498891`).
It enumerates every masked, nonterminal, positive-total coordinate satisfying
`bx*q == p_T*(bx+bo)` at exhaustive scales N=1 through N=32, then applies the
same exact predicate to the fixed 20,332-key targeted population at N=64 and
N=128. The target keys are selected once from the N=32 masks. Each distinct
target board is solved once per spot scale, only selected owners are consumed,
and the report folds one board at a time without retaining a `KnifePoint`
population. No coordinate sampling, truncation, compression, sharding, LFS, or
external artifact storage is used.

### Headline disagreement trend

The exhaustive N<=32 census contains 1,194,488 disagreements among 5,869,108
exact equality points (20.3521%). N=1 is a small-population outlier at 96/4,022
(2.3869%); from N=2 onward the rate oscillates around 20%. The locked targeted
rates are 147,692/722,676 at N=64 (20.4368%) and 296,216/1,450,932 at N=128
(20.4156%), a slight decrease of about 0.0213 percentage points. Thus the scale
trend is stable near 20.4%, not decaying toward an empty disagreement set over
the solved scales.

Solved scales are exact discrete-oracle territory under P1a. These
P1c disagreements measure the fidelity boundary of the P1b tie-owner-wins
convention at exactly representable equality; they do not by themselves
re-ratify or impeach the separately governed large-scale convention.

**Operator-judgment flag:** ANY nonempty disagreement table is flagged as an
operator-judgment item in the completion report, routed via
s1.orchestrator-planner; never absorbed into the convention.

### Per-scale counts

| N | Mode | Target keys | Equality points | Agreements | Disagreements | Exact D/E |
|---:|---|---:|---:|---:|---:|---:|
| 1 | exhaustive | — | 4022 | 3926 | 96 | 96/4022 |
| 2 | exhaustive | — | 14122 | 11250 | 2872 | 2872/14122 |
| 3 | exhaustive | — | 24888 | 20486 | 4402 | 4402/24888 |
| 4 | exhaustive | — | 38764 | 30688 | 8076 | 8076/38764 |
| 5 | exhaustive | — | 47782 | 38918 | 8864 | 8864/47782 |
| 6 | exhaustive | — | 63760 | 50432 | 13328 | 13328/63760 |
| 7 | exhaustive | — | 71306 | 57664 | 13642 | 13642/71306 |
| 8 | exhaustive | — | 85482 | 67946 | 17536 | 17536/85482 |
| 9 | exhaustive | — | 95256 | 76482 | 18774 | 18774/95256 |
| 10 | exhaustive | — | 110090 | 87258 | 22832 | 22832/110090 |
| 11 | exhaustive | — | 117216 | 94176 | 23040 | 23040/117216 |
| 12 | exhaustive | — | 133744 | 105884 | 27860 | 27860/133744 |
| 13 | exhaustive | — | 140806 | 112878 | 27928 | 27928/140806 |
| 14 | exhaustive | — | 154638 | 123046 | 31592 | 31592/154638 |
| 15 | exhaustive | — | 165728 | 132218 | 33510 | 33510/165728 |
| 16 | exhaustive | — | 179838 | 142632 | 37206 | 37206/179838 |
| 17 | exhaustive | — | 186766 | 149456 | 37310 | 37310/186766 |
| 18 | exhaustive | — | 202876 | 161010 | 41866 | 41866/202876 |
| 19 | exhaustive | — | 209804 | 167910 | 41894 | 41894/209804 |
| 20 | exhaustive | — | 225308 | 178936 | 46372 | 46372/225308 |
| 21 | exhaustive | — | 235864 | 187838 | 48026 | 48026/235864 |
| 22 | exhaustive | — | 249226 | 197880 | 51346 | 51346/249226 |
| 23 | exhaustive | — | 256054 | 204642 | 51412 | 51412/256054 |
| 24 | exhaustive | — | 273052 | 216598 | 56454 | 56454/273052 |
| 25 | exhaustive | — | 281210 | 224114 | 57096 | 57096/281210 |
| 26 | exhaustive | — | 294594 | 234066 | 60528 | 60528/294594 |
| 27 | exhaustive | — | 304336 | 242568 | 61768 | 61768/304336 |
| 28 | exhaustive | — | 318710 | 253030 | 65680 | 65680/318710 |
| 29 | exhaustive | — | 325682 | 259858 | 65824 | 65824/325682 |
| 30 | exhaustive | — | 343484 | 272270 | 71214 | 71214/343484 |
| 31 | exhaustive | — | 350238 | 279112 | 71126 | 71126/350238 |
| 32 | exhaustive | — | 364462 | 289448 | 75014 | 75014/364462 |
| 64 | targeted | 20332 | 722676 | 574984 | 147692 | 147692/722676 |
| 128 | targeted | 20332 | 1450932 | 1154716 | 296216 | 296216/1450932 |

Every record satisfies `equality_points == agreements + disagreements`. The
sum of all `groups.count` rows at a scale equals its equality population, and
the sum of all rows below at that scale equals its disagreement count.

### Disagreement stream digests

Each digest covers the deterministic newline-delimited JSON stream of
`[board,h,bx,bo]` tuples for disagreement coordinates in sweep order. The
coordinates themselves are not retained in the committed artifact.

| N | Mode | SHA-256 |
|---:|---|---|
| 1 | exhaustive | `a7a03e9b52fc15cac90e2bd6d5eb803d81519cdf08ea9bfe4e1318d4ab2f5e37` |
| 2 | exhaustive | `59d18b6a3f1ff1adca23897ec90965a36e5c9dbc408a15ac02941c0cb3ff944e` |
| 3 | exhaustive | `c19b0c9492a5f78e4025be80f328c32185c573d1b1090d181d168aba4f870ced` |
| 4 | exhaustive | `a1db4d59668e08c0c9b9264c237eb8c23f8a8cb9d1dbfe4570d1d23873c3254c` |
| 5 | exhaustive | `3bac1abd114d6d4034b1eb5cff69b3a5d2a170801ea11c1a8b9597ec3a906b2e` |
| 6 | exhaustive | `afc28a58131d5797e24fa435ae6f9cd678e57a0619ee370d1a9711d02bde4258` |
| 7 | exhaustive | `2b158b3425fe94a09409f2d146e50b7272263e34318b3d0a175dc523a0ea68d5` |
| 8 | exhaustive | `512b6d0c4f1ab3acf74a4fd650b89d925b7eff4a979a6e9a8b630038e35638b1` |
| 9 | exhaustive | `369da0c7ddffd9520e9e70a9215af30ad5cd762179d1f9eeade1b7b2b573cf54` |
| 10 | exhaustive | `2ef5e779d29e7e8e13c3a1f275554176551a6fe7d845683aa3ed76122bc880a8` |
| 11 | exhaustive | `5d4ba725d8975025277b686ed08be8a22676da3778de961d4771527507d7bc91` |
| 12 | exhaustive | `bdc22caefff1339f77d10be0b105a0142e27508f69373fde4d187a47977e9e97` |
| 13 | exhaustive | `5e417aafe2a990ddab076cd10544f0d3d270417e1dc4817b66370f1e34ed664f` |
| 14 | exhaustive | `80fa50b713e79f91854016be31fd7f127ee400787300bfd2ca7f9dc4e59e407e` |
| 15 | exhaustive | `44b27faa2d6d521e0b570dd186475bd0c54b75df7eb189e00698f381a960a3ca` |
| 16 | exhaustive | `12b32801796b4709f387beb3c8f770d3685339a846a241c36b9293824eb09d3c` |
| 17 | exhaustive | `3a2c570a119c3f60dfc2292a86bb2dbb6da378faa2ca3ecdd461ef91686fc152` |
| 18 | exhaustive | `439d4e0511b3293e8e7e4663865eb6d6b7d13e47e1cb9493a08ed451e0e4e6aa` |
| 19 | exhaustive | `4ca4c354a1385b5dcddb12860e5e0e91f9f7e4c5a8045281e95d0adf045462e3` |
| 20 | exhaustive | `31a374e846a2f5293c1d28e0f12a83656660010d09652bd717a2661ca9313663` |
| 21 | exhaustive | `0a6240c6bccccdab75122a892491eb800082926e0ad1672f5261279d927610e7` |
| 22 | exhaustive | `065ba019fc8643aaa5dd48914352f6b9486cd7613fa07f572172b493aa615a38` |
| 23 | exhaustive | `04de4023b645034e3d0f51f58f8d2d0fdb2035986531bc5d6aea93b521713a4c` |
| 24 | exhaustive | `c4ea7995aae4541320ecfe48b805483f938b14588f5e560450f16d25d41aa1d8` |
| 25 | exhaustive | `26a9097101503ba264cc917098c8b98a7b51e346bd6a70866c03efa9af10919a` |
| 26 | exhaustive | `d5879b6ebfadba28ed20bfa51062fd90899a7d5e44c7c607d44aaffbb0a7bc0c` |
| 27 | exhaustive | `5960514e4a7a6c46cc8d57f0dc95b44e41d477ae4be32b4b7677d435df7e37c0` |
| 28 | exhaustive | `c433a9dda7bb3f881686dfc89f9734f40d93329c5d932cd1cac9eb5779928c35` |
| 29 | exhaustive | `fdcb27d7cf3b628dfbdb13fc9baf75d350149e9c9310927a61ff587f6805142d` |
| 30 | exhaustive | `7edf05d10d0de38dad1f3ac5d08dcf5b06c0a61078169a0bb0620025a5888ee1` |
| 31 | exhaustive | `9afe0612b7e3b53205ce66a16417313339ab04cfaf676bc1cc5eb56b86271094` |
| 32 | exhaustive | `3e16e8efabb4e1b91e79a3626257bd57fe9da98e120bdc3daad814dfa9b94819` |
| 64 | targeted | `01113fa26f3f15c97a94998b2dfb2c2b172c7cce140fd1deb1b0912d3df97268` |
| 128 | targeted | `2db5ce03489dac3ae49bebee8cdc2796dc11846658a637204e0fe93a7d60a87f` |

### Complete nonzero disagreement partition

This is the full `disagreement_groups` array: every nonzero `agrees=false`
partition row keyed by scale, mode, tie owner, state class, and discrete oracle
outcome. Draw and opponent-win disagreements remain distinct.

| N | Mode | h | State class | Discrete | Agrees | Count |
|---:|---|---|---|---|---|---:|
| 1 | exhaustive | O | a<b/E3 | draw | false | 8 |
| 1 | exhaustive | O | a=b/E1 | draw | false | 40 |
| 1 | exhaustive | X | a<b/E3 | draw | false | 8 |
| 1 | exhaustive | X | a=b/E1 | draw | false | 40 |
| 2 | exhaustive | O | a<b/E1 | draw | false | 104 |
| 2 | exhaustive | O | a<b/E2 | X | false | 48 |
| 2 | exhaustive | O | a<b/E2 | draw | false | 96 |
| 2 | exhaustive | O | a<b/E3 | X | false | 388 |
| 2 | exhaustive | O | a<b/E3 | draw | false | 64 |
| 2 | exhaustive | O | a<b/E4 | X | false | 348 |
| 2 | exhaustive | O | a<b/E4 | draw | false | 4 |
| 2 | exhaustive | O | a<b/E5 | X | false | 264 |
| 2 | exhaustive | O | a<b/E6 | X | false | 40 |
| 2 | exhaustive | O | a=b/E1 | draw | false | 80 |
| 2 | exhaustive | X | a<b/E1 | draw | false | 104 |
| 2 | exhaustive | X | a<b/E2 | O | false | 48 |
| 2 | exhaustive | X | a<b/E2 | draw | false | 96 |
| 2 | exhaustive | X | a<b/E3 | O | false | 388 |
| 2 | exhaustive | X | a<b/E3 | draw | false | 64 |
| 2 | exhaustive | X | a<b/E4 | O | false | 348 |
| 2 | exhaustive | X | a<b/E4 | draw | false | 4 |
| 2 | exhaustive | X | a<b/E5 | O | false | 264 |
| 2 | exhaustive | X | a<b/E6 | O | false | 40 |
| 2 | exhaustive | X | a=b/E1 | draw | false | 80 |
| 3 | exhaustive | O | a<b/E1 | draw | false | 104 |
| 3 | exhaustive | O | a<b/E2 | X | false | 216 |
| 3 | exhaustive | O | a<b/E2 | draw | false | 168 |
| 3 | exhaustive | O | a<b/E3 | X | false | 464 |
| 3 | exhaustive | O | a<b/E3 | draw | false | 136 |
| 3 | exhaustive | O | a<b/E4 | X | false | 609 |
| 3 | exhaustive | O | a<b/E4 | draw | false | 8 |
| 3 | exhaustive | O | a<b/E5 | X | false | 300 |
| 3 | exhaustive | O | a<b/E6 | X | false | 76 |
| 3 | exhaustive | O | a=b/E1 | draw | false | 120 |
| 3 | exhaustive | X | a<b/E1 | draw | false | 104 |
| 3 | exhaustive | X | a<b/E2 | O | false | 216 |
| 3 | exhaustive | X | a<b/E2 | draw | false | 168 |
| 3 | exhaustive | X | a<b/E3 | O | false | 464 |
| 3 | exhaustive | X | a<b/E3 | draw | false | 136 |
| 3 | exhaustive | X | a<b/E4 | O | false | 609 |
| 3 | exhaustive | X | a<b/E4 | draw | false | 8 |
| 3 | exhaustive | X | a<b/E5 | O | false | 300 |
| 3 | exhaustive | X | a<b/E6 | O | false | 76 |
| 3 | exhaustive | X | a=b/E1 | draw | false | 120 |
| 4 | exhaustive | O | a<b/E1 | draw | false | 208 |
| 4 | exhaustive | O | a<b/E2 | X | false | 300 |
| 4 | exhaustive | O | a<b/E2 | draw | false | 352 |
| 4 | exhaustive | O | a<b/E3 | X | false | 972 |
| 4 | exhaustive | O | a<b/E3 | draw | false | 192 |
| 4 | exhaustive | O | a<b/E4 | X | false | 1126 |
| 4 | exhaustive | O | a<b/E4 | draw | false | 60 |
| 4 | exhaustive | O | a<b/E5 | X | false | 564 |
| 4 | exhaustive | O | a<b/E5 | draw | false | 24 |
| 4 | exhaustive | O | a<b/E6 | X | false | 80 |
| 4 | exhaustive | O | a=b/E1 | draw | false | 160 |
| 4 | exhaustive | X | a<b/E1 | draw | false | 208 |
| 4 | exhaustive | X | a<b/E2 | O | false | 300 |
| 4 | exhaustive | X | a<b/E2 | draw | false | 352 |
| 4 | exhaustive | X | a<b/E3 | O | false | 972 |
| 4 | exhaustive | X | a<b/E3 | draw | false | 192 |
| 4 | exhaustive | X | a<b/E4 | O | false | 1126 |
| 4 | exhaustive | X | a<b/E4 | draw | false | 60 |
| 4 | exhaustive | X | a<b/E5 | O | false | 564 |
| 4 | exhaustive | X | a<b/E5 | draw | false | 24 |
| 4 | exhaustive | X | a<b/E6 | O | false | 80 |
| 4 | exhaustive | X | a=b/E1 | draw | false | 160 |
| 5 | exhaustive | O | a<b/E1 | draw | false | 208 |
| 5 | exhaustive | O | a<b/E2 | X | false | 310 |
| 5 | exhaustive | O | a<b/E2 | draw | false | 352 |
| 5 | exhaustive | O | a<b/E3 | X | false | 1048 |
| 5 | exhaustive | O | a<b/E3 | draw | false | 264 |
| 5 | exhaustive | O | a<b/E4 | X | false | 1162 |
| 5 | exhaustive | O | a<b/E4 | draw | false | 100 |
| 5 | exhaustive | O | a<b/E5 | X | false | 644 |
| 5 | exhaustive | O | a<b/E5 | draw | false | 32 |
| 5 | exhaustive | O | a<b/E6 | X | false | 112 |
| 5 | exhaustive | O | a=b/E1 | draw | false | 200 |
| 5 | exhaustive | X | a<b/E1 | draw | false | 208 |
| 5 | exhaustive | X | a<b/E2 | O | false | 310 |
| 5 | exhaustive | X | a<b/E2 | draw | false | 352 |
| 5 | exhaustive | X | a<b/E3 | O | false | 1048 |
| 5 | exhaustive | X | a<b/E3 | draw | false | 264 |
| 5 | exhaustive | X | a<b/E4 | O | false | 1162 |
| 5 | exhaustive | X | a<b/E4 | draw | false | 100 |
| 5 | exhaustive | X | a<b/E5 | O | false | 644 |
| 5 | exhaustive | X | a<b/E5 | draw | false | 32 |
| 5 | exhaustive | X | a<b/E6 | O | false | 112 |
| 5 | exhaustive | X | a=b/E1 | draw | false | 200 |
| 6 | exhaustive | O | a<b/E1 | draw | false | 312 |
| 6 | exhaustive | O | a<b/E2 | X | false | 528 |
| 6 | exhaustive | O | a<b/E2 | draw | false | 520 |
| 6 | exhaustive | O | a<b/E3 | X | false | 1560 |
| 6 | exhaustive | O | a<b/E3 | draw | false | 392 |
| 6 | exhaustive | O | a<b/E4 | X | false | 1771 |
| 6 | exhaustive | O | a<b/E4 | draw | false | 104 |
| 6 | exhaustive | O | a<b/E5 | X | false | 993 |
| 6 | exhaustive | O | a<b/E5 | draw | false | 56 |
| 6 | exhaustive | O | a<b/E6 | X | false | 188 |
| 6 | exhaustive | O | a=b/E1 | draw | false | 240 |
| 6 | exhaustive | X | a<b/E1 | draw | false | 312 |
| 6 | exhaustive | X | a<b/E2 | O | false | 528 |
| 6 | exhaustive | X | a<b/E2 | draw | false | 520 |
| 6 | exhaustive | X | a<b/E3 | O | false | 1560 |
| 6 | exhaustive | X | a<b/E3 | draw | false | 392 |
| 6 | exhaustive | X | a<b/E4 | O | false | 1771 |
| 6 | exhaustive | X | a<b/E4 | draw | false | 104 |
| 6 | exhaustive | X | a<b/E5 | O | false | 993 |
| 6 | exhaustive | X | a<b/E5 | draw | false | 56 |
| 6 | exhaustive | X | a<b/E6 | O | false | 188 |
| 6 | exhaustive | X | a=b/E1 | draw | false | 240 |
| 7 | exhaustive | O | a<b/E1 | draw | false | 312 |
| 7 | exhaustive | O | a<b/E2 | X | false | 538 |
| 7 | exhaustive | O | a<b/E2 | draw | false | 544 |
| 7 | exhaustive | O | a<b/E3 | X | false | 1560 |
| 7 | exhaustive | O | a<b/E3 | draw | false | 500 |
| 7 | exhaustive | O | a<b/E4 | X | false | 1783 |
| 7 | exhaustive | O | a<b/E4 | draw | false | 136 |
| 7 | exhaustive | O | a<b/E5 | X | false | 944 |
| 7 | exhaustive | O | a<b/E5 | draw | false | 64 |
| 7 | exhaustive | O | a<b/E6 | X | false | 152 |
| 7 | exhaustive | O | a<b/E6 | draw | false | 8 |
| 7 | exhaustive | O | a=b/E1 | draw | false | 280 |
| 7 | exhaustive | X | a<b/E1 | draw | false | 312 |
| 7 | exhaustive | X | a<b/E2 | O | false | 538 |
| 7 | exhaustive | X | a<b/E2 | draw | false | 544 |
| 7 | exhaustive | X | a<b/E3 | O | false | 1560 |
| 7 | exhaustive | X | a<b/E3 | draw | false | 500 |
| 7 | exhaustive | X | a<b/E4 | O | false | 1783 |
| 7 | exhaustive | X | a<b/E4 | draw | false | 136 |
| 7 | exhaustive | X | a<b/E5 | O | false | 944 |
| 7 | exhaustive | X | a<b/E5 | draw | false | 64 |
| 7 | exhaustive | X | a<b/E6 | O | false | 152 |
| 7 | exhaustive | X | a<b/E6 | draw | false | 8 |
| 7 | exhaustive | X | a=b/E1 | draw | false | 280 |
| 8 | exhaustive | O | a<b/E1 | draw | false | 416 |
| 8 | exhaustive | O | a<b/E2 | X | false | 628 |
| 8 | exhaustive | O | a<b/E2 | draw | false | 704 |
| 8 | exhaustive | O | a<b/E3 | X | false | 2056 |
| 8 | exhaustive | O | a<b/E3 | draw | false | 556 |
| 8 | exhaustive | O | a<b/E4 | X | false | 2324 |
| 8 | exhaustive | O | a<b/E4 | draw | false | 212 |
| 8 | exhaustive | O | a<b/E5 | X | false | 1208 |
| 8 | exhaustive | O | a<b/E5 | draw | false | 112 |
| 8 | exhaustive | O | a<b/E6 | X | false | 208 |
| 8 | exhaustive | O | a<b/E6 | draw | false | 16 |
| 8 | exhaustive | O | a<b/E7 | draw | false | 8 |
| 8 | exhaustive | O | a=b/E1 | draw | false | 320 |
| 8 | exhaustive | X | a<b/E1 | draw | false | 416 |
| 8 | exhaustive | X | a<b/E2 | O | false | 628 |
| 8 | exhaustive | X | a<b/E2 | draw | false | 704 |
| 8 | exhaustive | X | a<b/E3 | O | false | 2056 |
| 8 | exhaustive | X | a<b/E3 | draw | false | 556 |
| 8 | exhaustive | X | a<b/E4 | O | false | 2324 |
| 8 | exhaustive | X | a<b/E4 | draw | false | 212 |
| 8 | exhaustive | X | a<b/E5 | O | false | 1208 |
| 8 | exhaustive | X | a<b/E5 | draw | false | 112 |
| 8 | exhaustive | X | a<b/E6 | O | false | 208 |
| 8 | exhaustive | X | a<b/E6 | draw | false | 16 |
| 8 | exhaustive | X | a<b/E7 | draw | false | 8 |
| 8 | exhaustive | X | a=b/E1 | draw | false | 320 |
| 9 | exhaustive | O | a<b/E1 | draw | false | 416 |
| 9 | exhaustive | O | a<b/E2 | X | false | 760 |
| 9 | exhaustive | O | a<b/E2 | draw | false | 776 |
| 9 | exhaustive | O | a<b/E3 | X | false | 2146 |
| 9 | exhaustive | O | a<b/E3 | draw | false | 636 |
| 9 | exhaustive | O | a<b/E4 | X | false | 2396 |
| 9 | exhaustive | O | a<b/E4 | draw | false | 224 |
| 9 | exhaustive | O | a<b/E5 | X | false | 1309 |
| 9 | exhaustive | O | a<b/E5 | draw | false | 112 |
| 9 | exhaustive | O | a<b/E6 | X | false | 228 |
| 9 | exhaustive | O | a<b/E6 | draw | false | 16 |
| 9 | exhaustive | O | a<b/E7 | draw | false | 8 |
| 9 | exhaustive | O | a=b/E1 | draw | false | 360 |
| 9 | exhaustive | X | a<b/E1 | draw | false | 416 |
| 9 | exhaustive | X | a<b/E2 | O | false | 760 |
| 9 | exhaustive | X | a<b/E2 | draw | false | 776 |
| 9 | exhaustive | X | a<b/E3 | O | false | 2146 |
| 9 | exhaustive | X | a<b/E3 | draw | false | 636 |
| 9 | exhaustive | X | a<b/E4 | O | false | 2396 |
| 9 | exhaustive | X | a<b/E4 | draw | false | 224 |
| 9 | exhaustive | X | a<b/E5 | O | false | 1309 |
| 9 | exhaustive | X | a<b/E5 | draw | false | 112 |
| 9 | exhaustive | X | a<b/E6 | O | false | 228 |
| 9 | exhaustive | X | a<b/E6 | draw | false | 16 |
| 9 | exhaustive | X | a<b/E7 | draw | false | 8 |
| 9 | exhaustive | X | a=b/E1 | draw | false | 360 |
| 10 | exhaustive | O | a<b/E1 | draw | false | 520 |
| 10 | exhaustive | O | a<b/E2 | X | false | 850 |
| 10 | exhaustive | O | a<b/E2 | draw | false | 896 |
| 10 | exhaustive | O | a<b/E3 | X | false | 2608 |
| 10 | exhaustive | O | a<b/E3 | draw | false | 784 |
| 10 | exhaustive | O | a<b/E4 | X | false | 3033 |
| 10 | exhaustive | O | a<b/E4 | draw | false | 300 |
| 10 | exhaustive | O | a<b/E5 | X | false | 1612 |
| 10 | exhaustive | O | a<b/E5 | draw | false | 120 |
| 10 | exhaustive | O | a<b/E6 | X | false | 260 |
| 10 | exhaustive | O | a<b/E6 | draw | false | 24 |
| 10 | exhaustive | O | a<b/E7 | draw | false | 8 |
| 10 | exhaustive | O | a<b/E9 | draw | false | 1 |
| 10 | exhaustive | O | a=b/E1 | draw | false | 400 |
| 10 | exhaustive | X | a<b/E1 | draw | false | 520 |
| 10 | exhaustive | X | a<b/E2 | O | false | 850 |
| 10 | exhaustive | X | a<b/E2 | draw | false | 896 |
| 10 | exhaustive | X | a<b/E3 | O | false | 2608 |
| 10 | exhaustive | X | a<b/E3 | draw | false | 784 |
| 10 | exhaustive | X | a<b/E4 | O | false | 3033 |
| 10 | exhaustive | X | a<b/E4 | draw | false | 300 |
| 10 | exhaustive | X | a<b/E5 | O | false | 1612 |
| 10 | exhaustive | X | a<b/E5 | draw | false | 120 |
| 10 | exhaustive | X | a<b/E6 | O | false | 260 |
| 10 | exhaustive | X | a<b/E6 | draw | false | 24 |
| 10 | exhaustive | X | a<b/E7 | draw | false | 8 |
| 10 | exhaustive | X | a<b/E9 | draw | false | 1 |
| 10 | exhaustive | X | a=b/E1 | draw | false | 400 |
| 11 | exhaustive | O | a<b/E1 | draw | false | 520 |
| 11 | exhaustive | O | a<b/E2 | X | false | 854 |
| 11 | exhaustive | O | a<b/E2 | draw | false | 896 |
| 11 | exhaustive | O | a<b/E3 | X | false | 2608 |
| 11 | exhaustive | O | a<b/E3 | draw | false | 784 |
| 11 | exhaustive | O | a<b/E4 | X | false | 3042 |
| 11 | exhaustive | O | a<b/E4 | draw | false | 340 |
| 11 | exhaustive | O | a<b/E5 | X | false | 1624 |
| 11 | exhaustive | O | a<b/E5 | draw | false | 120 |
| 11 | exhaustive | O | a<b/E6 | X | false | 260 |
| 11 | exhaustive | O | a<b/E6 | draw | false | 24 |
| 11 | exhaustive | O | a<b/E7 | draw | false | 8 |
| 11 | exhaustive | O | a=b/E1 | draw | false | 440 |
| 11 | exhaustive | X | a<b/E1 | draw | false | 520 |
| 11 | exhaustive | X | a<b/E2 | O | false | 854 |
| 11 | exhaustive | X | a<b/E2 | draw | false | 896 |
| 11 | exhaustive | X | a<b/E3 | O | false | 2608 |
| 11 | exhaustive | X | a<b/E3 | draw | false | 784 |
| 11 | exhaustive | X | a<b/E4 | O | false | 3042 |
| 11 | exhaustive | X | a<b/E4 | draw | false | 340 |
| 11 | exhaustive | X | a<b/E5 | O | false | 1624 |
| 11 | exhaustive | X | a<b/E5 | draw | false | 120 |
| 11 | exhaustive | X | a<b/E6 | O | false | 260 |
| 11 | exhaustive | X | a<b/E6 | draw | false | 24 |
| 11 | exhaustive | X | a<b/E7 | draw | false | 8 |
| 11 | exhaustive | X | a=b/E1 | draw | false | 440 |
| 12 | exhaustive | O | a<b/E1 | draw | false | 624 |
| 12 | exhaustive | O | a<b/E2 | X | false | 1072 |
| 12 | exhaustive | O | a<b/E2 | draw | false | 1128 |
| 12 | exhaustive | O | a<b/E3 | X | false | 3194 |
| 12 | exhaustive | O | a<b/E3 | draw | false | 912 |
| 12 | exhaustive | O | a<b/E4 | X | false | 3631 |
| 12 | exhaustive | O | a<b/E4 | draw | false | 396 |
| 12 | exhaustive | O | a<b/E5 | X | false | 1961 |
| 12 | exhaustive | O | a<b/E5 | draw | false | 160 |
| 12 | exhaustive | O | a<b/E6 | X | false | 336 |
| 12 | exhaustive | O | a<b/E6 | draw | false | 24 |
| 12 | exhaustive | O | a<b/E7 | draw | false | 12 |
| 12 | exhaustive | O | a=b/E1 | draw | false | 480 |
| 12 | exhaustive | X | a<b/E1 | draw | false | 624 |
| 12 | exhaustive | X | a<b/E2 | O | false | 1072 |
| 12 | exhaustive | X | a<b/E2 | draw | false | 1128 |
| 12 | exhaustive | X | a<b/E3 | O | false | 3194 |
| 12 | exhaustive | X | a<b/E3 | draw | false | 912 |
| 12 | exhaustive | X | a<b/E4 | O | false | 3631 |
| 12 | exhaustive | X | a<b/E4 | draw | false | 396 |
| 12 | exhaustive | X | a<b/E5 | O | false | 1961 |
| 12 | exhaustive | X | a<b/E5 | draw | false | 160 |
| 12 | exhaustive | X | a<b/E6 | O | false | 336 |
| 12 | exhaustive | X | a<b/E6 | draw | false | 24 |
| 12 | exhaustive | X | a<b/E7 | draw | false | 12 |
| 12 | exhaustive | X | a=b/E1 | draw | false | 480 |
| 13 | exhaustive | O | a<b/E1 | draw | false | 624 |
| 13 | exhaustive | O | a<b/E2 | X | false | 1076 |
| 13 | exhaustive | O | a<b/E2 | draw | false | 1152 |
| 13 | exhaustive | O | a<b/E3 | X | false | 3192 |
| 13 | exhaustive | O | a<b/E3 | draw | false | 920 |
| 13 | exhaustive | O | a<b/E4 | X | false | 3619 |
| 13 | exhaustive | O | a<b/E4 | draw | false | 396 |
| 13 | exhaustive | O | a<b/E5 | X | false | 1928 |
| 13 | exhaustive | O | a<b/E5 | draw | false | 192 |
| 13 | exhaustive | O | a<b/E6 | X | false | 300 |
| 13 | exhaustive | O | a<b/E6 | draw | false | 32 |
| 13 | exhaustive | O | a<b/E7 | draw | false | 12 |
| 13 | exhaustive | O | a<b/E9 | draw | false | 1 |
| 13 | exhaustive | O | a=b/E1 | draw | false | 520 |
| 13 | exhaustive | X | a<b/E1 | draw | false | 624 |
| 13 | exhaustive | X | a<b/E2 | O | false | 1076 |
| 13 | exhaustive | X | a<b/E2 | draw | false | 1152 |
| 13 | exhaustive | X | a<b/E3 | O | false | 3192 |
| 13 | exhaustive | X | a<b/E3 | draw | false | 920 |
| 13 | exhaustive | X | a<b/E4 | O | false | 3619 |
| 13 | exhaustive | X | a<b/E4 | draw | false | 396 |
| 13 | exhaustive | X | a<b/E5 | O | false | 1928 |
| 13 | exhaustive | X | a<b/E5 | draw | false | 192 |
| 13 | exhaustive | X | a<b/E6 | O | false | 300 |
| 13 | exhaustive | X | a<b/E6 | draw | false | 32 |
| 13 | exhaustive | X | a<b/E7 | draw | false | 12 |
| 13 | exhaustive | X | a<b/E9 | draw | false | 1 |
| 13 | exhaustive | X | a=b/E1 | draw | false | 520 |
| 14 | exhaustive | O | a<b/E1 | draw | false | 728 |
| 14 | exhaustive | O | a<b/E2 | X | false | 1166 |
| 14 | exhaustive | O | a<b/E2 | draw | false | 1248 |
| 14 | exhaustive | O | a<b/E3 | X | false | 3616 |
| 14 | exhaustive | O | a<b/E3 | draw | false | 1084 |
| 14 | exhaustive | O | a<b/E4 | X | false | 4196 |
| 14 | exhaustive | O | a<b/E4 | draw | false | 428 |
| 14 | exhaustive | O | a<b/E5 | X | false | 2184 |
| 14 | exhaustive | O | a<b/E5 | draw | false | 200 |
| 14 | exhaustive | O | a<b/E6 | X | false | 340 |
| 14 | exhaustive | O | a<b/E6 | draw | false | 34 |
| 14 | exhaustive | O | a<b/E7 | draw | false | 12 |
| 14 | exhaustive | O | a=b/E1 | draw | false | 560 |
| 14 | exhaustive | X | a<b/E1 | draw | false | 728 |
| 14 | exhaustive | X | a<b/E2 | O | false | 1166 |
| 14 | exhaustive | X | a<b/E2 | draw | false | 1248 |
| 14 | exhaustive | X | a<b/E3 | O | false | 3616 |
| 14 | exhaustive | X | a<b/E3 | draw | false | 1084 |
| 14 | exhaustive | X | a<b/E4 | O | false | 4196 |
| 14 | exhaustive | X | a<b/E4 | draw | false | 428 |
| 14 | exhaustive | X | a<b/E5 | O | false | 2184 |
| 14 | exhaustive | X | a<b/E5 | draw | false | 200 |
| 14 | exhaustive | X | a<b/E6 | O | false | 340 |
| 14 | exhaustive | X | a<b/E6 | draw | false | 34 |
| 14 | exhaustive | X | a<b/E7 | draw | false | 12 |
| 14 | exhaustive | X | a=b/E1 | draw | false | 560 |
| 15 | exhaustive | O | a<b/E1 | draw | false | 728 |
| 15 | exhaustive | O | a<b/E2 | X | false | 1298 |
| 15 | exhaustive | O | a<b/E2 | draw | false | 1320 |
| 15 | exhaustive | O | a<b/E3 | X | false | 3746 |
| 15 | exhaustive | O | a<b/E3 | draw | false | 1220 |
| 15 | exhaustive | O | a<b/E4 | X | false | 4304 |
| 15 | exhaustive | O | a<b/E4 | draw | false | 504 |
| 15 | exhaustive | O | a<b/E5 | X | false | 2365 |
| 15 | exhaustive | O | a<b/E5 | draw | false | 208 |
| 15 | exhaustive | O | a<b/E6 | X | false | 408 |
| 15 | exhaustive | O | a<b/E6 | draw | false | 42 |
| 15 | exhaustive | O | a<b/E7 | draw | false | 12 |
| 15 | exhaustive | O | a=b/E1 | draw | false | 600 |
| 15 | exhaustive | X | a<b/E1 | draw | false | 728 |
| 15 | exhaustive | X | a<b/E2 | O | false | 1298 |
| 15 | exhaustive | X | a<b/E2 | draw | false | 1320 |
| 15 | exhaustive | X | a<b/E3 | O | false | 3746 |
| 15 | exhaustive | X | a<b/E3 | draw | false | 1220 |
| 15 | exhaustive | X | a<b/E4 | O | false | 4304 |
| 15 | exhaustive | X | a<b/E4 | draw | false | 504 |
| 15 | exhaustive | X | a<b/E5 | O | false | 2365 |
| 15 | exhaustive | X | a<b/E5 | draw | false | 208 |
| 15 | exhaustive | X | a<b/E6 | O | false | 408 |
| 15 | exhaustive | X | a<b/E6 | draw | false | 42 |
| 15 | exhaustive | X | a<b/E7 | draw | false | 12 |
| 15 | exhaustive | X | a=b/E1 | draw | false | 600 |
| 16 | exhaustive | O | a<b/E1 | draw | false | 832 |
| 16 | exhaustive | O | a<b/E2 | X | false | 1388 |
| 16 | exhaustive | O | a<b/E2 | draw | false | 1504 |
| 16 | exhaustive | O | a<b/E3 | X | false | 4240 |
| 16 | exhaustive | O | a<b/E3 | draw | false | 1276 |
| 16 | exhaustive | O | a<b/E4 | X | false | 4817 |
| 16 | exhaustive | O | a<b/E4 | draw | false | 580 |
| 16 | exhaustive | O | a<b/E5 | X | false | 2572 |
| 16 | exhaustive | O | a<b/E5 | draw | false | 256 |
| 16 | exhaustive | O | a<b/E6 | X | false | 428 |
| 16 | exhaustive | O | a<b/E6 | draw | false | 50 |
| 16 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 16 | exhaustive | O | a=b/E1 | draw | false | 640 |
| 16 | exhaustive | X | a<b/E1 | draw | false | 832 |
| 16 | exhaustive | X | a<b/E2 | O | false | 1388 |
| 16 | exhaustive | X | a<b/E2 | draw | false | 1504 |
| 16 | exhaustive | X | a<b/E3 | O | false | 4240 |
| 16 | exhaustive | X | a<b/E3 | draw | false | 1276 |
| 16 | exhaustive | X | a<b/E4 | O | false | 4817 |
| 16 | exhaustive | X | a<b/E4 | draw | false | 580 |
| 16 | exhaustive | X | a<b/E5 | O | false | 2572 |
| 16 | exhaustive | X | a<b/E5 | draw | false | 256 |
| 16 | exhaustive | X | a<b/E6 | O | false | 428 |
| 16 | exhaustive | X | a<b/E6 | draw | false | 50 |
| 16 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 16 | exhaustive | X | a=b/E1 | draw | false | 640 |
| 17 | exhaustive | O | a<b/E1 | draw | false | 832 |
| 17 | exhaustive | O | a<b/E2 | X | false | 1392 |
| 17 | exhaustive | O | a<b/E2 | draw | false | 1504 |
| 17 | exhaustive | O | a<b/E3 | X | false | 4240 |
| 17 | exhaustive | O | a<b/E3 | draw | false | 1284 |
| 17 | exhaustive | O | a<b/E4 | X | false | 4817 |
| 17 | exhaustive | O | a<b/E4 | draw | false | 588 |
| 17 | exhaustive | O | a<b/E5 | X | false | 2580 |
| 17 | exhaustive | O | a<b/E5 | draw | false | 264 |
| 17 | exhaustive | O | a<b/E6 | X | false | 404 |
| 17 | exhaustive | O | a<b/E6 | draw | false | 50 |
| 17 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 17 | exhaustive | O | a=b/E1 | draw | false | 680 |
| 17 | exhaustive | X | a<b/E1 | draw | false | 832 |
| 17 | exhaustive | X | a<b/E2 | O | false | 1392 |
| 17 | exhaustive | X | a<b/E2 | draw | false | 1504 |
| 17 | exhaustive | X | a<b/E3 | O | false | 4240 |
| 17 | exhaustive | X | a<b/E3 | draw | false | 1284 |
| 17 | exhaustive | X | a<b/E4 | O | false | 4817 |
| 17 | exhaustive | X | a<b/E4 | draw | false | 588 |
| 17 | exhaustive | X | a<b/E5 | O | false | 2580 |
| 17 | exhaustive | X | a<b/E5 | draw | false | 264 |
| 17 | exhaustive | X | a<b/E6 | O | false | 404 |
| 17 | exhaustive | X | a<b/E6 | draw | false | 50 |
| 17 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 17 | exhaustive | X | a=b/E1 | draw | false | 680 |
| 18 | exhaustive | O | a<b/E1 | draw | false | 936 |
| 18 | exhaustive | O | a<b/E2 | X | false | 1610 |
| 18 | exhaustive | O | a<b/E2 | draw | false | 1672 |
| 18 | exhaustive | O | a<b/E3 | X | false | 4754 |
| 18 | exhaustive | O | a<b/E3 | draw | false | 1412 |
| 18 | exhaustive | O | a<b/E4 | X | false | 5438 |
| 18 | exhaustive | O | a<b/E4 | draw | false | 596 |
| 18 | exhaustive | O | a<b/E5 | X | false | 2933 |
| 18 | exhaustive | O | a<b/E5 | draw | false | 304 |
| 18 | exhaustive | O | a<b/E6 | X | false | 488 |
| 18 | exhaustive | O | a<b/E6 | draw | false | 50 |
| 18 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 18 | exhaustive | O | a=b/E1 | draw | false | 720 |
| 18 | exhaustive | X | a<b/E1 | draw | false | 936 |
| 18 | exhaustive | X | a<b/E2 | O | false | 1610 |
| 18 | exhaustive | X | a<b/E2 | draw | false | 1672 |
| 18 | exhaustive | X | a<b/E3 | O | false | 4754 |
| 18 | exhaustive | X | a<b/E3 | draw | false | 1412 |
| 18 | exhaustive | X | a<b/E4 | O | false | 5438 |
| 18 | exhaustive | X | a<b/E4 | draw | false | 596 |
| 18 | exhaustive | X | a<b/E5 | O | false | 2933 |
| 18 | exhaustive | X | a<b/E5 | draw | false | 304 |
| 18 | exhaustive | X | a<b/E6 | O | false | 488 |
| 18 | exhaustive | X | a<b/E6 | draw | false | 50 |
| 18 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 18 | exhaustive | X | a=b/E1 | draw | false | 720 |
| 19 | exhaustive | O | a<b/E1 | draw | false | 936 |
| 19 | exhaustive | O | a<b/E2 | X | false | 1614 |
| 19 | exhaustive | O | a<b/E2 | draw | false | 1696 |
| 19 | exhaustive | O | a<b/E3 | X | false | 4752 |
| 19 | exhaustive | O | a<b/E3 | draw | false | 1412 |
| 19 | exhaustive | O | a<b/E4 | X | false | 5426 |
| 19 | exhaustive | O | a<b/E4 | draw | false | 636 |
| 19 | exhaustive | O | a<b/E5 | X | false | 2885 |
| 19 | exhaustive | O | a<b/E5 | draw | false | 312 |
| 19 | exhaustive | O | a<b/E6 | X | false | 444 |
| 19 | exhaustive | O | a<b/E6 | draw | false | 50 |
| 19 | exhaustive | O | a<b/E7 | X | false | 4 |
| 19 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 19 | exhaustive | O | a=b/E1 | draw | false | 760 |
| 19 | exhaustive | X | a<b/E1 | draw | false | 936 |
| 19 | exhaustive | X | a<b/E2 | O | false | 1614 |
| 19 | exhaustive | X | a<b/E2 | draw | false | 1696 |
| 19 | exhaustive | X | a<b/E3 | O | false | 4752 |
| 19 | exhaustive | X | a<b/E3 | draw | false | 1412 |
| 19 | exhaustive | X | a<b/E4 | O | false | 5426 |
| 19 | exhaustive | X | a<b/E4 | draw | false | 636 |
| 19 | exhaustive | X | a<b/E5 | O | false | 2885 |
| 19 | exhaustive | X | a<b/E5 | draw | false | 312 |
| 19 | exhaustive | X | a<b/E6 | O | false | 444 |
| 19 | exhaustive | X | a<b/E6 | draw | false | 50 |
| 19 | exhaustive | X | a<b/E7 | O | false | 4 |
| 19 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 19 | exhaustive | X | a=b/E1 | draw | false | 760 |
| 20 | exhaustive | O | a<b/E1 | draw | false | 1040 |
| 20 | exhaustive | O | a<b/E2 | X | false | 1704 |
| 20 | exhaustive | O | a<b/E2 | draw | false | 1856 |
| 20 | exhaustive | O | a<b/E3 | X | false | 5288 |
| 20 | exhaustive | O | a<b/E3 | draw | false | 1560 |
| 20 | exhaustive | O | a<b/E4 | X | false | 6028 |
| 20 | exhaustive | O | a<b/E4 | draw | false | 744 |
| 20 | exhaustive | O | a<b/E5 | X | false | 3228 |
| 20 | exhaustive | O | a<b/E5 | draw | false | 344 |
| 20 | exhaustive | O | a<b/E6 | X | false | 508 |
| 20 | exhaustive | O | a<b/E6 | draw | false | 66 |
| 20 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 20 | exhaustive | O | a=b/E1 | draw | false | 800 |
| 20 | exhaustive | X | a<b/E1 | draw | false | 1040 |
| 20 | exhaustive | X | a<b/E2 | O | false | 1704 |
| 20 | exhaustive | X | a<b/E2 | draw | false | 1856 |
| 20 | exhaustive | X | a<b/E3 | O | false | 5288 |
| 20 | exhaustive | X | a<b/E3 | draw | false | 1560 |
| 20 | exhaustive | X | a<b/E4 | O | false | 6028 |
| 20 | exhaustive | X | a<b/E4 | draw | false | 744 |
| 20 | exhaustive | X | a<b/E5 | O | false | 3228 |
| 20 | exhaustive | X | a<b/E5 | draw | false | 344 |
| 20 | exhaustive | X | a<b/E6 | O | false | 508 |
| 20 | exhaustive | X | a<b/E6 | draw | false | 66 |
| 20 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 20 | exhaustive | X | a=b/E1 | draw | false | 800 |
| 21 | exhaustive | O | a<b/E1 | draw | false | 1040 |
| 21 | exhaustive | O | a<b/E2 | X | false | 1836 |
| 21 | exhaustive | O | a<b/E2 | draw | false | 1928 |
| 21 | exhaustive | O | a<b/E3 | X | false | 5378 |
| 21 | exhaustive | O | a<b/E3 | draw | false | 1748 |
| 21 | exhaustive | O | a<b/E4 | X | false | 6116 |
| 21 | exhaustive | O | a<b/E4 | draw | false | 808 |
| 21 | exhaustive | O | a<b/E5 | X | false | 3321 |
| 21 | exhaustive | O | a<b/E5 | draw | false | 344 |
| 21 | exhaustive | O | a<b/E6 | X | false | 552 |
| 21 | exhaustive | O | a<b/E6 | draw | false | 82 |
| 21 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 21 | exhaustive | O | a=b/E1 | draw | false | 840 |
| 21 | exhaustive | X | a<b/E1 | draw | false | 1040 |
| 21 | exhaustive | X | a<b/E2 | O | false | 1836 |
| 21 | exhaustive | X | a<b/E2 | draw | false | 1928 |
| 21 | exhaustive | X | a<b/E3 | O | false | 5378 |
| 21 | exhaustive | X | a<b/E3 | draw | false | 1748 |
| 21 | exhaustive | X | a<b/E4 | O | false | 6116 |
| 21 | exhaustive | X | a<b/E4 | draw | false | 808 |
| 21 | exhaustive | X | a<b/E5 | O | false | 3321 |
| 21 | exhaustive | X | a<b/E5 | draw | false | 344 |
| 21 | exhaustive | X | a<b/E6 | O | false | 552 |
| 21 | exhaustive | X | a<b/E6 | draw | false | 82 |
| 21 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 21 | exhaustive | X | a=b/E1 | draw | false | 840 |
| 22 | exhaustive | O | a<b/E1 | draw | false | 1144 |
| 22 | exhaustive | O | a<b/E2 | X | false | 1926 |
| 22 | exhaustive | O | a<b/E2 | draw | false | 2048 |
| 22 | exhaustive | O | a<b/E3 | X | false | 5800 |
| 22 | exhaustive | O | a<b/E3 | draw | false | 1804 |
| 22 | exhaustive | O | a<b/E4 | X | false | 6681 |
| 22 | exhaustive | O | a<b/E4 | draw | false | 840 |
| 22 | exhaustive | O | a<b/E5 | X | false | 3548 |
| 22 | exhaustive | O | a<b/E5 | draw | false | 344 |
| 22 | exhaustive | O | a<b/E6 | X | false | 556 |
| 22 | exhaustive | O | a<b/E6 | draw | false | 82 |
| 22 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 22 | exhaustive | O | a=b/E1 | draw | false | 880 |
| 22 | exhaustive | X | a<b/E1 | draw | false | 1144 |
| 22 | exhaustive | X | a<b/E2 | O | false | 1926 |
| 22 | exhaustive | X | a<b/E2 | draw | false | 2048 |
| 22 | exhaustive | X | a<b/E3 | O | false | 5800 |
| 22 | exhaustive | X | a<b/E3 | draw | false | 1804 |
| 22 | exhaustive | X | a<b/E4 | O | false | 6681 |
| 22 | exhaustive | X | a<b/E4 | draw | false | 840 |
| 22 | exhaustive | X | a<b/E5 | O | false | 3548 |
| 22 | exhaustive | X | a<b/E5 | draw | false | 344 |
| 22 | exhaustive | X | a<b/E6 | O | false | 556 |
| 22 | exhaustive | X | a<b/E6 | draw | false | 82 |
| 22 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 22 | exhaustive | X | a=b/E1 | draw | false | 880 |
| 23 | exhaustive | O | a<b/E1 | draw | false | 1144 |
| 23 | exhaustive | O | a<b/E2 | X | false | 1930 |
| 23 | exhaustive | O | a<b/E2 | draw | false | 2048 |
| 23 | exhaustive | O | a<b/E3 | X | false | 5800 |
| 23 | exhaustive | O | a<b/E3 | draw | false | 1804 |
| 23 | exhaustive | O | a<b/E4 | X | false | 6681 |
| 23 | exhaustive | O | a<b/E4 | draw | false | 840 |
| 23 | exhaustive | O | a<b/E5 | X | false | 3536 |
| 23 | exhaustive | O | a<b/E5 | draw | false | 344 |
| 23 | exhaustive | O | a<b/E6 | X | false | 556 |
| 23 | exhaustive | O | a<b/E6 | draw | false | 82 |
| 23 | exhaustive | O | a<b/E7 | draw | false | 20 |
| 23 | exhaustive | O | a<b/E9 | draw | false | 1 |
| 23 | exhaustive | O | a=b/E1 | draw | false | 920 |
| 23 | exhaustive | X | a<b/E1 | draw | false | 1144 |
| 23 | exhaustive | X | a<b/E2 | O | false | 1930 |
| 23 | exhaustive | X | a<b/E2 | draw | false | 2048 |
| 23 | exhaustive | X | a<b/E3 | O | false | 5800 |
| 23 | exhaustive | X | a<b/E3 | draw | false | 1804 |
| 23 | exhaustive | X | a<b/E4 | O | false | 6681 |
| 23 | exhaustive | X | a<b/E4 | draw | false | 840 |
| 23 | exhaustive | X | a<b/E5 | O | false | 3536 |
| 23 | exhaustive | X | a<b/E5 | draw | false | 344 |
| 23 | exhaustive | X | a<b/E6 | O | false | 556 |
| 23 | exhaustive | X | a<b/E6 | draw | false | 82 |
| 23 | exhaustive | X | a<b/E7 | draw | false | 20 |
| 23 | exhaustive | X | a<b/E9 | draw | false | 1 |
| 23 | exhaustive | X | a=b/E1 | draw | false | 920 |
| 24 | exhaustive | O | a<b/E1 | draw | false | 1248 |
| 24 | exhaustive | O | a<b/E2 | X | false | 2148 |
| 24 | exhaustive | O | a<b/E2 | draw | false | 2280 |
| 24 | exhaustive | O | a<b/E3 | X | false | 6386 |
| 24 | exhaustive | O | a<b/E3 | draw | false | 1932 |
| 24 | exhaustive | O | a<b/E4 | X | false | 7294 |
| 24 | exhaustive | O | a<b/E4 | draw | false | 920 |
| 24 | exhaustive | O | a<b/E5 | X | false | 3885 |
| 24 | exhaustive | O | a<b/E5 | draw | false | 408 |
| 24 | exhaustive | O | a<b/E6 | X | false | 648 |
| 24 | exhaustive | O | a<b/E6 | draw | false | 90 |
| 24 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 24 | exhaustive | O | a=b/E1 | draw | false | 960 |
| 24 | exhaustive | X | a<b/E1 | draw | false | 1248 |
| 24 | exhaustive | X | a<b/E2 | O | false | 2148 |
| 24 | exhaustive | X | a<b/E2 | draw | false | 2280 |
| 24 | exhaustive | X | a<b/E3 | O | false | 6386 |
| 24 | exhaustive | X | a<b/E3 | draw | false | 1932 |
| 24 | exhaustive | X | a<b/E4 | O | false | 7294 |
| 24 | exhaustive | X | a<b/E4 | draw | false | 920 |
| 24 | exhaustive | X | a<b/E5 | O | false | 3885 |
| 24 | exhaustive | X | a<b/E5 | draw | false | 408 |
| 24 | exhaustive | X | a<b/E6 | O | false | 648 |
| 24 | exhaustive | X | a<b/E6 | draw | false | 90 |
| 24 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 24 | exhaustive | X | a=b/E1 | draw | false | 960 |
| 25 | exhaustive | O | a<b/E1 | draw | false | 1248 |
| 25 | exhaustive | O | a<b/E2 | X | false | 2152 |
| 25 | exhaustive | O | a<b/E2 | draw | false | 2304 |
| 25 | exhaustive | O | a<b/E3 | X | false | 6424 |
| 25 | exhaustive | O | a<b/E3 | draw | false | 2004 |
| 25 | exhaustive | O | a<b/E4 | X | false | 7318 |
| 25 | exhaustive | O | a<b/E4 | draw | false | 1000 |
| 25 | exhaustive | O | a<b/E5 | X | false | 3924 |
| 25 | exhaustive | O | a<b/E5 | draw | false | 424 |
| 25 | exhaustive | O | a<b/E6 | X | false | 624 |
| 25 | exhaustive | O | a<b/E6 | draw | false | 98 |
| 25 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 25 | exhaustive | O | a=b/E1 | draw | false | 1000 |
| 25 | exhaustive | X | a<b/E1 | draw | false | 1248 |
| 25 | exhaustive | X | a<b/E2 | O | false | 2152 |
| 25 | exhaustive | X | a<b/E2 | draw | false | 2304 |
| 25 | exhaustive | X | a<b/E3 | O | false | 6424 |
| 25 | exhaustive | X | a<b/E3 | draw | false | 2004 |
| 25 | exhaustive | X | a<b/E4 | O | false | 7318 |
| 25 | exhaustive | X | a<b/E4 | draw | false | 1000 |
| 25 | exhaustive | X | a<b/E5 | O | false | 3924 |
| 25 | exhaustive | X | a<b/E5 | draw | false | 424 |
| 25 | exhaustive | X | a<b/E6 | O | false | 624 |
| 25 | exhaustive | X | a<b/E6 | draw | false | 98 |
| 25 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 25 | exhaustive | X | a=b/E1 | draw | false | 1000 |
| 26 | exhaustive | O | a<b/E1 | draw | false | 1352 |
| 26 | exhaustive | O | a<b/E2 | X | false | 2242 |
| 26 | exhaustive | O | a<b/E2 | draw | false | 2400 |
| 26 | exhaustive | O | a<b/E3 | X | false | 6848 |
| 26 | exhaustive | O | a<b/E3 | draw | false | 2060 |
| 26 | exhaustive | O | a<b/E4 | X | false | 7851 |
| 26 | exhaustive | O | a<b/E4 | draw | false | 1004 |
| 26 | exhaustive | O | a<b/E5 | X | false | 4204 |
| 26 | exhaustive | O | a<b/E5 | draw | false | 460 |
| 26 | exhaustive | O | a<b/E6 | X | false | 660 |
| 26 | exhaustive | O | a<b/E6 | draw | false | 114 |
| 26 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 26 | exhaustive | O | a<b/E9 | draw | false | 1 |
| 26 | exhaustive | O | a=b/E1 | draw | false | 1040 |
| 26 | exhaustive | X | a<b/E1 | draw | false | 1352 |
| 26 | exhaustive | X | a<b/E2 | O | false | 2242 |
| 26 | exhaustive | X | a<b/E2 | draw | false | 2400 |
| 26 | exhaustive | X | a<b/E3 | O | false | 6848 |
| 26 | exhaustive | X | a<b/E3 | draw | false | 2060 |
| 26 | exhaustive | X | a<b/E4 | O | false | 7851 |
| 26 | exhaustive | X | a<b/E4 | draw | false | 1004 |
| 26 | exhaustive | X | a<b/E5 | O | false | 4204 |
| 26 | exhaustive | X | a<b/E5 | draw | false | 460 |
| 26 | exhaustive | X | a<b/E6 | O | false | 660 |
| 26 | exhaustive | X | a<b/E6 | draw | false | 114 |
| 26 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 26 | exhaustive | X | a<b/E9 | draw | false | 1 |
| 26 | exhaustive | X | a=b/E1 | draw | false | 1040 |
| 27 | exhaustive | O | a<b/E1 | draw | false | 1352 |
| 27 | exhaustive | O | a<b/E2 | X | false | 2374 |
| 27 | exhaustive | O | a<b/E2 | draw | false | 2472 |
| 27 | exhaustive | O | a<b/E3 | X | false | 6938 |
| 27 | exhaustive | O | a<b/E3 | draw | false | 2132 |
| 27 | exhaustive | O | a<b/E4 | X | false | 7923 |
| 27 | exhaustive | O | a<b/E4 | draw | false | 1008 |
| 27 | exhaustive | O | a<b/E5 | X | false | 4281 |
| 27 | exhaustive | O | a<b/E5 | draw | false | 476 |
| 27 | exhaustive | O | a<b/E6 | X | false | 704 |
| 27 | exhaustive | O | a<b/E6 | draw | false | 114 |
| 27 | exhaustive | O | a<b/E7 | X | false | 2 |
| 27 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 27 | exhaustive | O | a=b/E1 | draw | false | 1080 |
| 27 | exhaustive | X | a<b/E1 | draw | false | 1352 |
| 27 | exhaustive | X | a<b/E2 | O | false | 2374 |
| 27 | exhaustive | X | a<b/E2 | draw | false | 2472 |
| 27 | exhaustive | X | a<b/E3 | O | false | 6938 |
| 27 | exhaustive | X | a<b/E3 | draw | false | 2132 |
| 27 | exhaustive | X | a<b/E4 | O | false | 7923 |
| 27 | exhaustive | X | a<b/E4 | draw | false | 1008 |
| 27 | exhaustive | X | a<b/E5 | O | false | 4281 |
| 27 | exhaustive | X | a<b/E5 | draw | false | 476 |
| 27 | exhaustive | X | a<b/E6 | O | false | 704 |
| 27 | exhaustive | X | a<b/E6 | draw | false | 114 |
| 27 | exhaustive | X | a<b/E7 | O | false | 2 |
| 27 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 27 | exhaustive | X | a=b/E1 | draw | false | 1080 |
| 28 | exhaustive | O | a<b/E1 | draw | false | 1456 |
| 28 | exhaustive | O | a<b/E2 | X | false | 2464 |
| 28 | exhaustive | O | a<b/E2 | draw | false | 2656 |
| 28 | exhaustive | O | a<b/E3 | X | false | 7432 |
| 28 | exhaustive | O | a<b/E3 | draw | false | 2296 |
| 28 | exhaustive | O | a<b/E4 | X | false | 8452 |
| 28 | exhaustive | O | a<b/E4 | draw | false | 1092 |
| 28 | exhaustive | O | a<b/E5 | X | false | 4504 |
| 28 | exhaustive | O | a<b/E5 | draw | false | 500 |
| 28 | exhaustive | O | a<b/E6 | X | false | 708 |
| 28 | exhaustive | O | a<b/E6 | draw | false | 132 |
| 28 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 28 | exhaustive | O | a=b/E1 | draw | false | 1120 |
| 28 | exhaustive | X | a<b/E1 | draw | false | 1456 |
| 28 | exhaustive | X | a<b/E2 | O | false | 2464 |
| 28 | exhaustive | X | a<b/E2 | draw | false | 2656 |
| 28 | exhaustive | X | a<b/E3 | O | false | 7432 |
| 28 | exhaustive | X | a<b/E3 | draw | false | 2296 |
| 28 | exhaustive | X | a<b/E4 | O | false | 8452 |
| 28 | exhaustive | X | a<b/E4 | draw | false | 1092 |
| 28 | exhaustive | X | a<b/E5 | O | false | 4504 |
| 28 | exhaustive | X | a<b/E5 | draw | false | 500 |
| 28 | exhaustive | X | a<b/E6 | O | false | 708 |
| 28 | exhaustive | X | a<b/E6 | draw | false | 132 |
| 28 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 28 | exhaustive | X | a=b/E1 | draw | false | 1120 |
| 29 | exhaustive | O | a<b/E1 | draw | false | 1456 |
| 29 | exhaustive | O | a<b/E2 | X | false | 2468 |
| 29 | exhaustive | O | a<b/E2 | draw | false | 2656 |
| 29 | exhaustive | O | a<b/E3 | X | false | 7432 |
| 29 | exhaustive | O | a<b/E3 | draw | false | 2304 |
| 29 | exhaustive | O | a<b/E4 | X | false | 8452 |
| 29 | exhaustive | O | a<b/E4 | draw | false | 1092 |
| 29 | exhaustive | O | a<b/E5 | X | false | 4516 |
| 29 | exhaustive | O | a<b/E5 | draw | false | 500 |
| 29 | exhaustive | O | a<b/E6 | X | false | 708 |
| 29 | exhaustive | O | a<b/E6 | draw | false | 140 |
| 29 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 29 | exhaustive | O | a=b/E1 | draw | false | 1160 |
| 29 | exhaustive | X | a<b/E1 | draw | false | 1456 |
| 29 | exhaustive | X | a<b/E2 | O | false | 2468 |
| 29 | exhaustive | X | a<b/E2 | draw | false | 2656 |
| 29 | exhaustive | X | a<b/E3 | O | false | 7432 |
| 29 | exhaustive | X | a<b/E3 | draw | false | 2304 |
| 29 | exhaustive | X | a<b/E4 | O | false | 8452 |
| 29 | exhaustive | X | a<b/E4 | draw | false | 1092 |
| 29 | exhaustive | X | a<b/E5 | O | false | 4516 |
| 29 | exhaustive | X | a<b/E5 | draw | false | 500 |
| 29 | exhaustive | X | a<b/E6 | O | false | 708 |
| 29 | exhaustive | X | a<b/E6 | draw | false | 140 |
| 29 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 29 | exhaustive | X | a=b/E1 | draw | false | 1160 |
| 30 | exhaustive | O | a<b/E1 | draw | false | 1560 |
| 30 | exhaustive | O | a<b/E2 | X | false | 2686 |
| 30 | exhaustive | O | a<b/E2 | draw | false | 2824 |
| 30 | exhaustive | O | a<b/E3 | X | false | 7986 |
| 30 | exhaustive | O | a<b/E3 | draw | false | 2524 |
| 30 | exhaustive | O | a<b/E4 | X | false | 9162 |
| 30 | exhaustive | O | a<b/E4 | draw | false | 1184 |
| 30 | exhaustive | O | a<b/E5 | X | false | 4961 |
| 30 | exhaustive | O | a<b/E5 | draw | false | 524 |
| 30 | exhaustive | O | a<b/E6 | X | false | 812 |
| 30 | exhaustive | O | a<b/E6 | draw | false | 156 |
| 30 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 30 | exhaustive | O | a=b/E1 | draw | false | 1200 |
| 30 | exhaustive | X | a<b/E1 | draw | false | 1560 |
| 30 | exhaustive | X | a<b/E2 | O | false | 2686 |
| 30 | exhaustive | X | a<b/E2 | draw | false | 2824 |
| 30 | exhaustive | X | a<b/E3 | O | false | 7986 |
| 30 | exhaustive | X | a<b/E3 | draw | false | 2524 |
| 30 | exhaustive | X | a<b/E4 | O | false | 9162 |
| 30 | exhaustive | X | a<b/E4 | draw | false | 1184 |
| 30 | exhaustive | X | a<b/E5 | O | false | 4961 |
| 30 | exhaustive | X | a<b/E5 | draw | false | 524 |
| 30 | exhaustive | X | a<b/E6 | O | false | 812 |
| 30 | exhaustive | X | a<b/E6 | draw | false | 156 |
| 30 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 30 | exhaustive | X | a=b/E1 | draw | false | 1200 |
| 31 | exhaustive | O | a<b/E1 | draw | false | 1560 |
| 31 | exhaustive | O | a<b/E2 | X | false | 2690 |
| 31 | exhaustive | O | a<b/E2 | draw | false | 2848 |
| 31 | exhaustive | O | a<b/E3 | X | false | 7984 |
| 31 | exhaustive | O | a<b/E3 | draw | false | 2524 |
| 31 | exhaustive | O | a<b/E4 | X | false | 9134 |
| 31 | exhaustive | O | a<b/E4 | draw | false | 1192 |
| 31 | exhaustive | O | a<b/E5 | X | false | 4904 |
| 31 | exhaustive | O | a<b/E5 | draw | false | 524 |
| 31 | exhaustive | O | a<b/E6 | X | false | 776 |
| 31 | exhaustive | O | a<b/E6 | draw | false | 156 |
| 31 | exhaustive | O | a<b/E7 | X | false | 2 |
| 31 | exhaustive | O | a<b/E7 | draw | false | 28 |
| 31 | exhaustive | O | a<b/E9 | draw | false | 1 |
| 31 | exhaustive | O | a=b/E1 | draw | false | 1240 |
| 31 | exhaustive | X | a<b/E1 | draw | false | 1560 |
| 31 | exhaustive | X | a<b/E2 | O | false | 2690 |
| 31 | exhaustive | X | a<b/E2 | draw | false | 2848 |
| 31 | exhaustive | X | a<b/E3 | O | false | 7984 |
| 31 | exhaustive | X | a<b/E3 | draw | false | 2524 |
| 31 | exhaustive | X | a<b/E4 | O | false | 9134 |
| 31 | exhaustive | X | a<b/E4 | draw | false | 1192 |
| 31 | exhaustive | X | a<b/E5 | O | false | 4904 |
| 31 | exhaustive | X | a<b/E5 | draw | false | 524 |
| 31 | exhaustive | X | a<b/E6 | O | false | 776 |
| 31 | exhaustive | X | a<b/E6 | draw | false | 156 |
| 31 | exhaustive | X | a<b/E7 | O | false | 2 |
| 31 | exhaustive | X | a<b/E7 | draw | false | 28 |
| 31 | exhaustive | X | a<b/E9 | draw | false | 1 |
| 31 | exhaustive | X | a=b/E1 | draw | false | 1240 |
| 32 | exhaustive | O | a<b/E1 | draw | false | 1664 |
| 32 | exhaustive | O | a<b/E2 | X | false | 2780 |
| 32 | exhaustive | O | a<b/E2 | draw | false | 3008 |
| 32 | exhaustive | O | a<b/E3 | X | false | 8480 |
| 32 | exhaustive | O | a<b/E3 | draw | false | 2580 |
| 32 | exhaustive | O | a<b/E4 | X | false | 9675 |
| 32 | exhaustive | O | a<b/E4 | draw | false | 1268 |
| 32 | exhaustive | O | a<b/E5 | X | false | 5168 |
| 32 | exhaustive | O | a<b/E5 | draw | false | 572 |
| 32 | exhaustive | O | a<b/E6 | X | false | 832 |
| 32 | exhaustive | O | a<b/E6 | draw | false | 164 |
| 32 | exhaustive | O | a<b/E7 | draw | false | 36 |
| 32 | exhaustive | O | a=b/E1 | draw | false | 1280 |
| 32 | exhaustive | X | a<b/E1 | draw | false | 1664 |
| 32 | exhaustive | X | a<b/E2 | O | false | 2780 |
| 32 | exhaustive | X | a<b/E2 | draw | false | 3008 |
| 32 | exhaustive | X | a<b/E3 | O | false | 8480 |
| 32 | exhaustive | X | a<b/E3 | draw | false | 2580 |
| 32 | exhaustive | X | a<b/E4 | O | false | 9675 |
| 32 | exhaustive | X | a<b/E4 | draw | false | 1268 |
| 32 | exhaustive | X | a<b/E5 | O | false | 5168 |
| 32 | exhaustive | X | a<b/E5 | draw | false | 572 |
| 32 | exhaustive | X | a<b/E6 | O | false | 832 |
| 32 | exhaustive | X | a<b/E6 | draw | false | 164 |
| 32 | exhaustive | X | a<b/E7 | draw | false | 36 |
| 32 | exhaustive | X | a=b/E1 | draw | false | 1280 |
| 64 | targeted | O | a<b/E1 | draw | false | 3328 |
| 64 | targeted | O | a<b/E2 | X | false | 5692 |
| 64 | targeted | O | a<b/E2 | draw | false | 6112 |
| 64 | targeted | O | a<b/E3 | X | false | 17048 |
| 64 | targeted | O | a<b/E3 | draw | false | 5340 |
| 64 | targeted | O | a<b/E4 | X | false | 19466 |
| 64 | targeted | O | a<b/E4 | draw | false | 2680 |
| 64 | targeted | O | a<b/E5 | X | false | 10416 |
| 64 | targeted | O | a<b/E5 | draw | false | 1204 |
| 64 | targeted | O | a=b/E1 | draw | false | 2560 |
| 64 | targeted | X | a<b/E1 | draw | false | 3328 |
| 64 | targeted | X | a<b/E2 | O | false | 5692 |
| 64 | targeted | X | a<b/E2 | draw | false | 6112 |
| 64 | targeted | X | a<b/E3 | O | false | 17048 |
| 64 | targeted | X | a<b/E3 | draw | false | 5340 |
| 64 | targeted | X | a<b/E4 | O | false | 19466 |
| 64 | targeted | X | a<b/E4 | draw | false | 2680 |
| 64 | targeted | X | a<b/E5 | O | false | 10416 |
| 64 | targeted | X | a<b/E5 | draw | false | 1204 |
| 64 | targeted | X | a=b/E1 | draw | false | 2560 |
| 128 | targeted | O | a<b/E1 | draw | false | 6656 |
| 128 | targeted | O | a<b/E2 | X | false | 11388 |
| 128 | targeted | O | a<b/E2 | draw | false | 12224 |
| 128 | targeted | O | a<b/E3 | X | false | 34136 |
| 128 | targeted | O | a<b/E3 | draw | false | 10744 |
| 128 | targeted | O | a<b/E4 | X | false | 38960 |
| 128 | targeted | O | a<b/E4 | draw | false | 5456 |
| 128 | targeted | O | a<b/E5 | X | false | 20936 |
| 128 | targeted | O | a<b/E5 | draw | false | 2488 |
| 128 | targeted | O | a=b/E1 | draw | false | 5120 |
| 128 | targeted | X | a<b/E1 | draw | false | 6656 |
| 128 | targeted | X | a<b/E2 | O | false | 11388 |
| 128 | targeted | X | a<b/E2 | draw | false | 12224 |
| 128 | targeted | X | a<b/E3 | O | false | 34136 |
| 128 | targeted | X | a<b/E3 | draw | false | 10744 |
| 128 | targeted | X | a<b/E4 | O | false | 38960 |
| 128 | targeted | X | a<b/E4 | draw | false | 5456 |
| 128 | targeted | X | a<b/E5 | O | false | 20936 |
| 128 | targeted | X | a<b/E5 | draw | false | 2488 |
| 128 | targeted | X | a=b/E1 | draw | false | 5120 |
