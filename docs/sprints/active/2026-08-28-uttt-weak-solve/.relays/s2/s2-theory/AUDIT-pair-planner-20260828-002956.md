ROLE: Pair Planner
PHASE: AUDIT
AUTHORITY: read-only
DISPATCH_ID: s2-theory-c1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — audit return; the next transition (paired-audit reconcile, then DESIGN dispatch) is the orchestrator's standing lifecycle
FROM: theory.planner
TO: s2.orchestrator-planner
CC: s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/AUDIT-orchestrator-planner-20260828-001406.md
SUBJECT: AUDIT return s2-theory-c1 — theory.planner: variant unsolved, kernels rule-clean, certificate+rule-language are the work

## Independence note

Filed without coordination with theory.implementer per dispatch instruction.
Research method: four parallel read-only research passes (arXiv:2006.02353 full PDF; solved-games/certificate literature; repo asset audit with file:line; 2swap transcript corpus), synthesized by this seat.

## Reject/narrow gate (checked first): does NOT fire

The rule-lock game is not solved in published literature.
arXiv:2006.02353 solves a materially different variant (see delta table below).
The standard free-opening variant with our closure rule is explicitly unsolved: Nelson Elhage's `ultimattt` (blog.nelhage.com/post/solving-ultimate-ttt/, github.com/nelhage/ultimattt) targets exactly that closure rule, reports positions solved only ~20 ply deep, and estimates a full solve at a few hundred million CPU-hours.
No source at all — paper, preprint, repo, or community post — studies the center-forced-first-move variant (searched arXiv, Semantic Scholar citation graph of 2006.02353, GitHub, Wikipedia, HN; absence-of-evidence, so E1 on each checked source, E0 on the universal claim).

## Scope item 1 — prior art delta and transfer

Correction for the record: arXiv:2006.02353 ("At most 43 moves, at least 29", 2020) is by Bertholon, Géraud-Stewart, Kugelmann, Lenoir, and Naccache (ENS/PSL) — the dispatch's "Bertholon/Gehanne/Grimaud" attribution is wrong.

Itemized rule delta vs the s2 rule lock (paper §1, pp. 1–2, full PDF read):

| Rule-lock item (ROADMAP.md:14-24) | arXiv:2006.02353 variant |
|---|---|
| First move forced into center local board | Free choice among all 81 cells |
| Board closed when won OR full; cells of closed boards dead | Only FULLNESS removes a board from play; won boards with empty cells remain playable — and forcing ("it is not possible to win an already won field", p. 2; Fig. 2: Olivia forced to move in a field Xavier already won) |
| Sent to closed board → any empty cell in any open board | Sent to a full field → free choice; sent to a won-but-not-full field → MUST play there |
| All nine boards closed with no macro winner → draw | Full 81-cell board with no macro winner → draw |
| X moves first, players alternate | Same |

Transfer assessment:
- Their first-player-win result does NOT transfer.
  The constructive strategy (open center-of-center, then shuttle Olivia between fields 0 and 8, pp. 3–4) depends on won-board cells staying playable; under our lock the center board dies when won and the shuttle construction is illegal.
- Their 43/29 bounds and the optimal-first-move-is-a-double lemmas are variant-specific; no numeric result carries over.
- What does transfer: the proof FORM — a weak solution presented as an explicit strategy plus human/machine-checkable invariants (their P1–P6 induction), which is the shape our certificate generalizes.
- Method note: the paper is entirely pen-and-paper; no search, no computer assistance, no state-space counts, no certificate artifact.
- Follow-ups: only Diamond arXiv:2207.06239 (randomized-opening mitigation, no solve); no citing work solves any UTTT variant.

Allis's VICTOR (Connect 4, 1988 thesis, read via tromp.github.io/c4/connect4_thesis.pdf):
- A "rule" is a formal claim with a Required clause (structural precondition on empty squares) and a Solutions clause (the set of opponent groups/threat-lines it refutes); nine rules (claimeven, baseinverse, vertical, aftereven, lowinverse, highinverse, baseclaim, before, specialbefore).
- Rules combine under a 9×9 pairwise-compatibility table; the load-bearing invariant is zugzwang-parity preservation (combined rule square-sets must release an even number of squares).
- A position is CERTIFIED (searchless) when a pairwise-compatible rule set refutes every opponent group; checking is enumeration only, no search.
- Soundness is unconditional even if the applier misjudges zugzwang control: wrong application just fails to cover all groups ("no harm in trying", §8.1) — a property our rule language should preserve.

Proof-number family: PN → PN2 → df-pn (Nagai) is the workhorse for large solves (checkers top-level manager, Fanorona, 9×9 Hex).
UTTT-relevant good news: marks only accumulate, so the game graph is a finite DAG — no cycles, no repetitions, hence the GHI problem (Kishimoto/Müller AAAI 2004) does not arise and df-pn completeness on finite DAGs holds.
Remaining real issue: proof-number double-counting where the DAG re-converges, and re-expansion overhead (1+ε trick, Pawlewicz/Lew CG 2006); parallel df-pn and job-level PN exist (Pawlewicz/Hayward CG 2013; Wu et al. CG 2010).
This is solver-pair territory; cited here as landscape.

Certificate practice in large solves:
- Checkers (Schaeffer et al., Science 2007): endgame DBs 3.9×10^13 positions/237 GB + PN proof-tree manager + df-pn solvers; stored proof frontier only ~10^7 positions; the full proof tree was NEVER materialized and the forward search was never independently re-verified — only the ≤9-piece DBs (plus ~30% of 10-piece) were independently recomputed (Ed Gilbert/KingsRow).
  Cautionary precedent: our bar (end-to-end independent verification) is deliberately higher.
- Nine Men's Morris (Gasser 1996): ~10^10-state retrograde DBs verified by a SEPARATE simpler program sweeping every position for local consistency with successors — the canonical streaming, bounded-memory verification pattern; directly reusable.
- Qubic (Patashnik 1980): strategy = move choices for 2,929 strategic positions, independently verified by Ken Thompson; re-solved by Allis/Schoo 1992 with PN.
- Othello (Takizawa, arXiv:2310.19387, weak draw): no independently checkable certificate published — a contrast to avoid, not follow.
- SAT-community practice is the strongest modern prior art for huge certificates: Pythagorean triples 200 TB DRAT proof distributed as a 68 GB regenerable certificate (Heule et al. 2016); Schur Number Five 2 PB proof checked by a FORMALLY VERIFIED tiny checker (Heule 2018).
  Lessons: partition the certificate into independently checkable shards; emit hint-rich formats so the checker never searches (DRAT→LRAT); keep the trusted checker small.
- QBF caveat: certificate-format choice constrains strategy extractability (QRAT extraction is PSPACE-hard); pick a format where the strategy is explicit, not implicit.

## Scope item 2 — value conjecture

Conjecture: X win, LOW confidence (~55% X win / ~40% draw / ~5% O win).
This is genuinely open; no publication and no repo artifact states a value for the rule-lock game (repo explicitly disclaims Bertholon coverage: s1 relay `.relays/s1/theory-c1/AUDIT-planner-20260825-064131.md:62-63`).

Reasoning:
- For X: first-move initiative in a threat-race game; the forced center-board opening is plausibly what optimal free-opening play would choose anyway (the center macro square intersects 4 macro lines), so the constraint likely costs X little relative to free-opening UTTT.
- Against certainty: strategy-stealing does NOT apply — the send mechanism makes an extra move a potential liability (your mark's cell dictates where the opponent is sent next; a "free" move can hand routing control away), so the classic first-player-advantage argument fails and O win, while unlikely, is not excluded.
- Toward draw: our closure rule is the drawish one — dead cells cap threat density, sent-to-closed grants the defender free moves (a growing late-game resource), and the all-closed draw terminal is structurally reachable.
- Center-forced delta vs free-opening: it removes 72 of X's 81 root options, shrinking the proof obligation ~9× at the root, and it PRESERVES full D4 symmetry at the root (the center board maps to itself), so symmetry reduction survives move one — a free-opening solve would lose most root symmetry immediately.

What resolves it: solver-pair shallow full-width probes and df-pn runs on the real game; theory pair updates this conjecture memo as probe data arrives.

## Scope item 3 — state-space and proof-size estimates

Honest brackets; no verified reachable count for UTTT exists in the literature (the Bertholon paper contains none).

- Raw upper bounds: 3^81 ≈ 4.4×10^38; refined per-board bound 5,478^9 ≈ 2.3×10^33 (legal single-board states, ignoring closure/parity/reachability coupling); true reachable-under-rule-lock count is orders below and currently UNKNOWN — an early solver-pair enumeration/perft-to-depth task should bracket it (repo has both-mover perft pins 18/320/5632/97920 at engine/tests/test_perft.cpp:24-31 but no alternating-game perft baseline).
- Symmetry: D4 (order 8) acts jointly on macro and micro grids and is preserved by the center-forced first move; asymptotic factor ~8, effective ~7-ish after symmetric-position overcounting; C4 comparison: mirror fusion removed just under half of nodes (one per pair).
- Effort anchor: Elhage estimates a full solve of the FREE-OPENING variant (same closure rule) at a few hundred million CPU-hours with 2020-era df-pn tooling; our game is not a subgame of his search (different root constraint) but is the same order of difficulty minus the ~9× root restriction and ~8× symmetry.
- Implication: pure brute force is NOT plausible as the sole method even at the operator's unlimited-budget ruling — 10^8 CPU-hours is a queue-management problem, not a plan.
  The plan-shaping conclusion: rule-based compression is load-bearing twice — for the solve (df-pn with rule-closed leaves prunes what search cannot) and for the CERTIFICATE (the checker must re-verify everything; a raw search-scale proof is uncheckable).
- Weak-solution DAG brackets: 2swap compressed C4 (4.5×10^12 positions) to a 4,550-node weak DAG (~10^9 compression) via rule leaves; a raw UTTT weak solution (all opponent replies, no rule leaves) is storage-infeasible (average O-branching ^ half-depth ≈ 10^15+ nodes), so plausible shipped-certificate brackets are 10^4–10^7 nodes WITH a strong rule language, 10^9–10^11 with only weak closure rules — the latter forces the checker-throughput question flagged below.

## Scope item 4 — strategy-rule language design space (landscape, not design)

From the 2swap corpus (notes/2swap-connect4-transcripts/) and VICTOR, with the C4→UTTT transfer analysis:

What does not transfer: claimeven's literal form, row parity, undercutting, dead-space zugzwang calculus — all artifacts of gravity and fixed 42-move game length.
UTTT has no gravity and variable game length (dead cells), so GLOBAL move-count parity is not a stable invariant.

Candidate UTTT rule families and their soundness obligations:

1. Send-legality pairing rules (the claimeven analogue).
   The deep idea in claimeven is a pairing argument; the gravity-specific part is only legality of the paired reply.
   In UTTT the sent-board mechanism plays gravity's role: a rule pairs opponent moves with responses and must PROVE the paired response is legal under the send it induces.
   Obligation: legality-under-send + coverage of all opponent moves + the claimed cell-set wins/blocks; hardest but highest-value family.
2. Local-board fact rules (the VICTOR "group refutation" analogue).
   Macro lines are the groups; rules refute a macro line for a player by proving a constituent local board unwinnable for them.
   Obligation: exhaustively checkable over 3^9 local states — the repo's LocalTable already enumerates all 3^9 with status/wins/forks (engine/src/core/local_table.cpp:54-79), so these soundness proofs are cheap and machine-checkable today.
3. Routing-control / dead-board-arithmetic rules.
   Sending the opponent to a closed board grants them a free move; free-move supply is a countable tempo currency.
   Rules of the form "player P can perpetually deny free moves / confine opponent to boards where all their lines are refuted".
   Obligation: an inductive invariant checked over all opponent replies (machine-checkable as a finite fixed-point computation on an abstraction).
4. Local parity/zugzwang rules.
   Global parity dies, but per-board fill parity and forced-send tempo survive: who can be forced to move into a poisoned board.
   The 2swap zugzwang framing ("control the toggle by spending odd-count resources") is the right SHAPE; its C4 instantiation is not.
5. Rule combination.
   VICTOR's pairwise-compatibility table is the template: disjointness conditions on claimed cell-sets plus send-interaction constraints.
   Obligation: a decidable pairwise-compatibility predicate, so a diagram = rule-set + compatibility certificate.

Cross-cutting soundness bar: 2swap's diagrams are asserted-by-exhaustion and partly left as "an open puzzle" (greedy-toggle correctness; full-graph reduction unfinished mid-series) — the transcripts supply rule shapes, NOT a verifier.
Our language must make every rule a predicate with an explicit finite verification obligation, and mechanize the check that a diagram covers every opponent reply at every reachable state under the strategy (claimeven's fill-in/undercut static check is the model: the certifying predicate is the full check, never the superficial pattern — the transcripts document positions where the superficial pattern holds and the rule loses).

## Scope item 5 — certificate design space (landscape, not schema)

- Core shape: canonicalized AND-OR proof DAG — exactly one move at our-to-move (MAX) nodes, ALL legal replies at opponent (MIN) nodes; edges may carry D4 transforms mapping child positions to canonical representatives.
- Leaf classes: (a) terminal positions; (b) rule-closed leaves — a diagram (rule-set + compatibility evidence) whose validity is a finite local check; (c) optionally endgame-database hits, which then require their own Gasser-style local-consistency verification sweep as part of certificate checking.
- Checker independence requirements: different pair, non-C++ (dispatch/ROADMAP bar); the checker needs its own rules-of-the-game implementation — candidate reuse: referee/poorman_referee/rules.py (99 LOC, fixture-consistent, non-C++), flagged as an operator/orchestrator independence question below.
- Verification at scale: single-pass streaming with bounded memory (topological order or shard-local order + a hash-linked manifest); shards independently checkable and parallelizable (SAT-certificate lesson); hint-rich encoding so the checker NEVER searches — every claim in the certificate carries what's needed to check it locally (DRAT→LRAT lesson).
- Integrity: canonical position encoding + content hashes per shard; the certificate must pin the rule-language version and carry (or reference) the machine-checked soundness proofs of every rule it uses, else the checker's trust base silently grows.
- Size-vs-checker-throughput coupling: a Python-order checker sustains ~10^8–10^9 node-checks in practical wall time; if the DAG lands in the 10^9–10^11 bracket the checker language question becomes load-bearing (Rust/Go are also "non-C++") — flagged for operator/orchestrator.

## Scope item 6 — duplicate gate (mandatory 4-bucket per asset)

Asset A: theory/auction_ttt (Stage-1 solver + rules.py).
- What it is: single-board 3×3 auction tic-tac-toe, NOT UTTT — rules.py (84 LOC) has no macro grid, no closure/routing, no center-forced rule (theory/auction_ttt/rules.py:5-78); exact-rational threshold solver over 5,478 boards × 2 tie owners (continuous.py:47-76) + discrete brute force (discrete.py:218-226) + LP cross-check ladder.
- Bucket: product-overlapped.
  Reuse the DISCIPLINE (exact arithmetic, independent-implementation crosscheck, evidence bundles, fixture generation) not the code; its discrete solver at zero budgets is a TTT3 alternating micro-oracle.
- Note: the dispatch phrase "theory/auction_ttt (Stage-1 solver, rules.py)" invites the false assumption a Python UTTT oracle lives there; the actual non-C++ UTTT rules are referee/poorman_referee/rules.py.

Asset B: theory/fixtures schema v1.
- What it is: JSON Schema 2020-12 (theory/fixtures/schema-v1.json) + 33 fixtures in 6 files; directly covers rule-lock behaviors — center-forced opening, sent-to-closed free choice, closure propagation, macro-precedes-all-closed, all-closed terminals; already consumed by engine tests (engine/tests/test_fixtures.cpp:321).
- Bucket: already-closed as rules-conformance anchors; recommended-next (extend) for s2 — the state shape REQUIRES auction fields `budgets`/`h`, so pure-alternating fixtures need a v2/alternating profile; as a CERTIFICATE schema it covers nothing — that is new work.

Asset C: engine/src/root/alt_solver.hpp (s1 G6).
- What it is: memoized negamax over the rule-9 alternating game, values as double with an Estimate horizon fallback (alt_solver.hpp:19,132-142), memo keyed (PosId, h, remaining_depth) (:62-67,113-124), in-RAM only, no symmetry, no persistence, no proof emission; exhaustively validated only at TTT3 scale (22,186 states, engine/tests/test_alt_solver.cpp:86-109); has NEVER produced a UTTT-scale result.
- Bucket: product-overlapped for s2's solver (promote as a semantic reference and small-subgame test oracle); the at-scale solver is still-open — depth-keyed doubles, RAM-only storage, and no W/D/L or proof output fail every s2 requirement.
  (Rebuild-vs-extend is the solver pair's audit/design call, not ours.)

Asset D: engine/src/core rules kernels (~1,260 LOC).
- What it is: bit-packed Position, LocalTable enumerating all 3^9 local boards with reachability filtering (local_table.cpp:54-79,16-30), move gen/apply/terminal, differential-tested against an independent naive reference, perft-pinned, fixture-conformant.
- Bucket: already-closed for rules semantics; recommended-next: promote/reuse as the s2 solver's rules core (ROADMAP gate 3 anticipates exactly this).
  Gaps for s2 (still-open work items, not defects): no side-to-move field, no D4 canonicalization, no alternating-game perft baseline.

Rule-lock conformance check (dispatch acceptance criterion): all six rule-lock items verified item-by-item against BOTH the C++ kernels and the referee Python rules with file:line; NO mismatches found.
Evidence table lives in the audit working notes; key cites: forced=4 initial (engine/src/core/position.hpp:27; referee/poorman_referee/rules.py:37), sent-to-closed→kForcedAny (position.cpp:79-81), closure on won-or-full with dead cells (position.cpp:73-78,49-51), macro-wins-outright ordering (position.cpp:28-31), all-closed draw (position.cpp:31; rules.py:90-91).

Cross-cutting: no repo artifact claims a value for the alternating game; no certificate/proof-emission code exists (two false friends: lp_reference.py's `certificate=` tag string; adapter/policy.hpp's play-time "certified forced action" gate); no D4 symmetry/canonicalization code exists in any language.

## 4-bucket verdict (overall)

PRIMARY_BUCKET: still-open
still-open: the weak solve itself; certificate schema; independent checker; strategy-rule language + machine-checked soundness; D4 canonicalization; value determination; alternating fixture profile.
already-closed: rule-lock game semantics (C++ kernels + referee Python rules + fixtures, verified conformant, no mismatches); 3^9 local-board enumeration (LocalTable).
product-overlapped: theory/auction_ttt (methodology only); alt_solver.hpp (oracle/reference only); fixtures schema v1 (conformance anchors, not a certificate schema).
recommended-next: proceed to theory DESIGN cycle on the certificate schema (the s2 shared contract, ROADMAP gate 1), with the rule-language design following; escalations A1/Q1-Q4 below to the orchestrator first.

Duplicate/already-built gate: completed above per asset; nothing s2 needs is silently already built; nothing already built should be rebuilt.

## Boundary contract

Not applicable at audit time (per dispatch).
Downstream restated: theory owns the certificate schema (writes: schema spec + checker; readers: solver-pair emitter, our checker); schema changes route through the orchestrator.

## Recommended cycle decomposition (theory pair)

- theory-c2 (DESIGN, grill triggers likely present: cross-pair boundary contract, hard-to-reverse format decision, many downstream choices): certificate schema v0 + D4 canonical-form spec — the shared contract everything else consumes; includes the shard/streaming/hint decisions and the rule-language versioning hook.
- theory-c3: independent checker v0 (language per Q2 ruling) + an end-to-end micro-solve round-trip fixture (alternating TTT3 via the existing oracle, or a closed UTTT subgame) proving solver-emit → checker-verify before scale.
- theory-c4: strategy-rule language v0 + machine-checkable soundness framework, starting from the cheap exhaustive family (local-board facts over LocalTable-style 3^9 enumeration), then send-legality pairing rules; gates solver leaf closure per ROADMAP gate 2.
- Continuous thread: value-conjecture memo, updated from solver-pair probe data; early ask to solver pair (via orchestrator): alternating-game perft + reachable-count bracketing, which items 2–3 above need.

## Risks / reject-or-narrow gates

- Reject/narrow: does not fire (variant unsolved — evidence at top).
- R1: reachable-state count unknown by orders of magnitude; mitigated by early solver-pair enumeration (above).
- R2: the rule language may not compress enough to reach a checkable certificate size; fallback is a bigger DAG + higher-throughput checker (Q2), and shard-parallel checking; this risk is why the schema must not hard-code a size assumption.
- R3: certificate trust base creep — if rule soundness proofs are not themselves machine-checked artifacts referenced by the certificate, the "verified end to end" bar silently degrades to checkers-2007-style partial verification.
- R4: Elhage-scale compute; per operator ruling budget is unlimited, but the plan should treat compute as an engineering surface (checkpointed, resumable, sharded) — solver pair's domain.

## Questions for operator/orchestrator (operator-judgment items)

- Q1 (escalation A1, cross-pair — rule-lock ambiguity clause): "X moves first" exists only in the s2 ROADMAP; no s1 implementation pins it.
  The C++ Position has no side-to-move (position.hpp:20-29); alternation is imposed externally; the root carries auction-semantics `tie=NullFirstMove` (position.hpp:28), meaningless in the alternating game.
  Request an orchestrator ruling on the canonical s2 state definition (side-to-move derived from mark parity, root = X to move) so solver and theory encode identically — this lands inside the certificate's canonical position encoding, hence raised now.
- Q2: checker language — is a fast non-C++ language (e.g. Rust/Go) acceptable if the certificate lands in the 10^9+ node bracket, or is Python mandated?
  Recommendation: mandate independence properties (different pair, different language, no shared game-logic code with the emitter), not a specific language.
- Q3: may the checker reuse referee/poorman_referee/rules.py (fixture-anchored, non-C++, but same-repo code originally authored under s1) or must theory write fresh rules for maximal independence?
  Recommendation: fresh minimal rules inside the checker, cross-checked against the fixtures — the fixtures are the shared truth, not shared code.
- Q4 (record correction, no action): dispatch and roadmap attribute arXiv:2006.02353 to "Bertholon/Gehanne/Grimaud"; correct authors are Bertholon, Géraud-Stewart, Kugelmann, Lenoir, Naccache.
- Q5 (flag only, theory-owned): fixtures schema v1 hard-requires auction fields; the alternating fixture profile lands in the c2/c3 design, no ruling needed unless the orchestrator wants schema v1 frozen.

## Evidence by claim (summary)

- Paper rule delta and results: E1 (arXiv:2006.02353 full PDF, page-cited above).
- "Target variant unsolved": E1 per checked source (Elhage blog/repo; Wikipedia; Semantic Scholar citation graph; Diamond arXiv:2207.06239), E0 as a universal claim (absence of evidence).
- Repo asset findings and rule-lock conformance: E1 (file:line cited per item).
- VICTOR / PN family / certificate practice: E1 (primary documents read: Allis thesis PDF, Winands PN chapter, Science 2007 paper, Gasser paper, Heule SAT papers, 2swap explanation page + transcript corpus with per-file timestamps).
- Value conjecture and DAG-size brackets: E0 by construction (conjecture, clearly labeled, with resolution path).

ACTIONS_GIT_REF: none — read-only audit; artifacts are this relay file and daemon-generated seat registration under .relays/s2/.engine/ and s2-seat/ (occupant 14b24e3c-edca-4edd-b7e9-1258b08c2f26)
FINAL_GIT_STATUS_SHORT:
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/INDEX.md
 M docs/sprints/active/2026-08-25-poorman-uttt-solver/plans/PL-theory-c2-20260827.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/SEATS.md
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-2/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-1/
?? docs/sprints/active/2026-08-25-poorman-uttt-solver/.relays/s1/theory-c2-plan-review-2/
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-boot/SITREP-orchestrator-reviewer-20260828-002232.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-seat/
(s1 rows belong to the s1 seats; s2 rows are daemon/seat-registration state plus the orchestrator-reviewer boot, none authored by this audit)
