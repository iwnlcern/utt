# DD-engine-math-c4-20260826 — Engine search math: interval threshold search, TT, widening, root bid layer, evaluator

DESIGN_DOC_ID: DD-engine-math-c4-20260826
Revision: 12 (2026-08-27; folds engine-c4-design-review-11 M1–M3 — the forced-branch bid becomes a CERTIFIED action carrier gated on exact-quality aggregates (the midpoint-anchor bid was unsound, witness recorded), the play-mode window gets its literal outward-rounded construction with the exact gate retained as sole classification authority, and the analyze mode gets a locked resource contract; rev 11 was amendment 2 folding the four-lens panel on PR #10 — ply-0 fair-coin tie semantics per R1, staged-best-move publication, and the acceptance-hardening block; G1–G6 and all solved-scale exactness are unchanged)
Author seat: engine.planner (dispatch engine-c4, s1.orchestrator-planner, 2026-08-26)
Status: awaiting DESIGN-REVIEW (engine.implementer, successor request)

## 1. Scope and normative sources

This design settles the engine's search mathematics deferred from DD-engine-rules-c1 (split option a): the Poorman threshold backup operator, cutoff/bound math, the transposition-table contract, selective widening, the root bid layer, the evaluator, time management, and the acceptance-criteria design.

Normative sources, consumed as-is at their landed states:

- `theory/FINDINGS.md` on main @ digest `79ee0c14d4bc973c5db79394818c7e5cbe8ab7751603073d1455d6a2dbd44ef5` (merged at 1211639; claims C1–C11, ratifications P1a/P1b/P1c, P2, P3). Referenced below as F-C1 … F-P3.
- `theory/fixtures/**` on main (canonical schema per s1-closure-ruling-1; consumed via the conforming runner landed in engine-c3).
- DD-engine-rules-c1-20260825 locked seams: §5 Zobrist key inputs and collision policy, §9 RootContext and budget plumbing, §10 value-quality metadata shape, §7 oracle tolerance (±1 fixed-point unit, engine-side-only, theory-subordinate).
- Operator ratifications carried in the dispatch: R4 integer-unit rounding with ±1–2 unit exploration; 27 s soft clock; F-C10 regret-reporting contract.

Out of scope: implementation; any change to landed rules-core, protocol, or fixture contracts; ML distillation (post-s1); play-vs-engine UI.

Base context: main @ `d7acb6a883d73ad5406c42fb7d329f2b77320b56` (supersedes the dispatch's 18a6be1 by two receipt filings; no engine or theory surface differs).

## 2. Value representation: directed-rounding double intervals (operator decision G1)

Every threshold value in the search is a `TInterval { double lo, hi; }` with the invariant `0.0 <= lo <= hi <= 1.0` and the meaning: the true continuous threshold T lies in `[lo, hi]`.

Soundness discipline (M1 fold — whole-expression ulp stepping is REJECTED as the mechanism): all interval arithmetic is built from four primitive directed-rounding operations — `sub_down/sub_up`, `add_down/add_up`, `mul_down/mul_up`, `div_down/div_up` — each computed in default round-to-nearest and stepped ONE ulp outward via `std::nextafter` (one ulp strictly bounds the at-most-half-ulp rounding error of a single IEEE-754 primitive).
Every composite (F, its two preimages, aggregation) is evaluated primitive-by-primitive on interval endpoints, so composition of enclosures stays an enclosure even under catastrophic cancellation; relative stepping of a whole expression fails exactly there (witness: the X-preimage at `b = 0.25, t = binary64(0.2)` evaluates to `0.0` in floats while the exact value of those binary64 inputs is `1/14411518807585588 ≈ 6.94e-17` — a two-ulp step around `0.0` cannot contain it).
The plan must carry the one-ulp-per-primitive invariant as a property test with NAMED cancellation and endpoint vectors, including the witness above (section 11, row A5); randomized rationals alone are not the proof.

A point value (an exactly-known terminal) is the degenerate interval `lo == hi`.
Exact-rational arithmetic appears only on the test side (section 11) for fixture and oracle comparisons; the engine runtime never allocates rationals.

## 3. Backup operator (theory-normative: F-C1 … F-C6)

Scalar definition, verbatim from the lock: with `a` = min over X-marked children of T, `b` = max over O-marked children of T,

- ordered branch `a <= b`: `T = F(a,b) = b / (1 - a + b)`; critical bid fraction `r = (b - a) / (1 - a + b)` (F-C1, F-C2);
- zugzwang branch `a > b`: both bid zero, `T = a` when tie owner `h = X`, `T = b` when `h = O`; critical bid `0` (F-C3);
- tie-owner transition: `h' = opponent(actual mover)` after every applied mark (F-C4);
- terminals: macro win `T = 0` (X) / `T = 1` (O); all-closed chip comparison `T = 1/2` with final payment ordered before the comparison (F-C5);
- root `h = null`: solve the two conditional games `T(s,X)` and `T(s,O)` and report the envelope `[min, max]`; no value is claimed for the simultaneous hidden-coin game (F-C6).

Interval lifting, justified by F-C7 monotonicity (`dF/da >= 0`, `dF/db >= 0` on the unit square):

- ordered branch on intervals: `F([a],[b]) = [ F_down(a.lo, b.lo), F_up(a.hi, b.hi) ]` (endpoint pairing uncrossed because F is nondecreasing in BOTH arguments, F-C7), with the literal directed sequences — a lower quotient consumes an UPPER-rounded denominator and vice versa:
  `F_down(a,b) = div_down(b, add_up(sub_up(1,a), b))` and `F_up(a,b) = div_up(b, add_down(sub_down(1,a), b))` (input endpoints are exact doubles and are never themselves rounded);
- branch selection on intervals: if `a.hi <= b.lo` take the ordered branch; if `a.lo > b.hi` take the zugzwang branch (`[a]` for `h=X`, `[b]` for `h=O`); otherwise take the interval hull of both branch results;
- the hull is sound and tight at the seam because both definitions coincide at `a = b` (F-C7's boundary continuity), so no jump is lost between the branches.

Child aggregation on intervals: `a = [ min over X-children of lo, min over X-children of hi ]` and `b = [ max over O-children of lo, max over O-children of hi ]`, each over the visited child set (section 6 governs unvisited children).

## 4. Cutoff and bound math (first-class deliverable)

The pruning layer is an alpha-beta analog in T-space built on three facts: F is monotone in both arguments (F-C7), `a` is a min-selection, and `b` is a max-selection.

Window transport: a node receives a target window `W = [alpha, beta]` meaning "the parent's decision changes only if this node's T lands inside W; outside W, any sound bound suffices".
The root window is `[0,1]` in analysis mode; in play mode the root layer narrows it to the P2 decision band around `bx/M` (section 7).

Child-level cutoffs (each stated with its soundness argument in-plan; all four derive from min/max selection plus F-monotonicity):

1. Min-side dominance: while aggregating `a`, a child whose interval satisfies `child.lo >= a_visited.hi` cannot become the selected minimum in any way that lowers `a.hi`; it still contributes to `a.lo` only through the value `a_visited.lo`, so its own search can stop at any sound bound with `lo >= a_visited.hi`.
2. Max-side dominance: symmetric for `b` with `child.hi <= b_visited.lo`.
3. Parent-window cut: given the partial `[a]`,`[b]` aggregates, compute the reachable parent interval `P_reach` by substituting the extreme still-possible child values (0 or 1 for unvisited slots under full-width, or their sound bounds under widening); if `P_reach` is disjoint from `W`, stop the node and return `P_reach` — the parent cannot be affected inside its window.
4. Precision cut: when a node's backed interval width is already below the requested precision `eps_node` (plumbed down from the root's P2 needs; `0` means exact-to-terminal), stop deepening below it.

Derived child windows (M1 fold — full interval-valued preimage contract):

- X-inverse: solving `F(a,b) = t` for `a` gives `a(t,b) = 1 - b(1-t)/t`, with `da/dt = b/t^2 >= 0` (nondecreasing in `t`) and `da/db = -(1-t)/t <= 0` (nonincreasing in `b`), so the interval preimage uses CROSSED `b` endpoints: `[ a_down(t.lo, b.hi), a_up(t.hi, b.lo) ]`.
  Literal directed sequences (lower result ⇒ subtract an UPPER-rounded term; an upper quotient consumes the exact denominator `t` directly):
  `a_down(t,b) = sub_down(1, div_up(mul_up(b, sub_up(1,t)), t))` and `a_up(t,b) = sub_up(1, div_down(mul_down(b, sub_down(1,t)), t))`.
- O-inverse: solving for `b` gives `b(t,a) = t(1-a)/(1-t)`, with `db/dt >= 0` and `db/da <= 0`, again crossed: `[ b_down(t.lo, a.hi), b_up(t.hi, a.lo) ]`, with
  `b_down(t,a) = div_down(mul_down(t, sub_down(1,a)), sub_up(1,t))` and `b_up(t,a) = div_up(mul_up(t, sub_up(1,a)), sub_down(1,t))`.
- Totality (guard branch table — the preimage functions are TOTAL): any endpoint evaluation whose guard fires — division by `t = 0` in the X-inverse, or by `1 - t = 0` (i.e. `t = 1`) in the O-inverse — returns the CONSERVATIVE FULL preimage endpoint (`0` for a lower endpoint, `1` for an upper endpoint) for that side; this is always sound (the full preimage merely disables pruning through that window) and never divides.
  Consequently the point windows `t = [0,0]` and `t = [1,1]` yield the full `[0,1]` preimage (both sides guarded), and an interval merely touching 0 or 1 guards only the touching side; EMPTY preimages arise solely from the intersections below, never from the guard table.
- Both preimages are clipped to `[0,1]`, intersected with the ordered-branch constraint (`a <= b` respectively `b >= a` on the sound aggregate side) and with the min/max aggregate domain (`[0, a_visited.hi]` for an X-child, `[b_visited.lo, 1]` for an O-child) BEFORE use as a child window; an empty intersection means the child cannot affect the parent inside `W` and is cut outright.
- The plan must include this algebra — the directed sequences verbatim, the crossed-endpoint pairing, the guard branch table, the clipping, and the intersections — as code with named tests, not prose.
Zugzwang interaction: inside the hull case (branch undecided), cuts 1–3 apply per branch and the node returns the hull; a window cut may fire only if BOTH branch results are disjoint from `W` on the same side.

## 5. Transposition table

Key: 64-bit Zobrist over cells × players, forced-or-ANY, tie state — exactly the rules-DD §5 inputs; budgets are excluded per the spec's budget independence of `T(s,h)`.

Entry (32 bytes, fixed layout):

- `tag` (u32): independent 32-bit verification fold per rules-DD §5.
- `lo`, `hi` (f64 each): the stored TInterval.
- `move_x`, `move_o` (u8 each, 0–80): best X-marked child move and best O-marked child move — both conditional moves, feeding the ui's if-X-wins / if-O-wins analysis.
- `depth` (u8): completed search horizon below this node.
- `gen` (u8): generation for aging.
- `flags` (u8): `occupied` 1 bit, quality 2 bits (exact / bound / estimate per section 8 mapping), `complete` 1 bit (move set exhausted), branch note 1 bit (hull case, diagnostic); remaining 3 bits zero.

Byte contract (every byte an explicit field or named padding; `static_assert(sizeof == 32 && alignof == 16)` — the entry is `alignas(16)` so both 16-byte halves are genuinely atomically loadable later — plus per-field `offsetof` asserts; the 4-way bucket is `alignas(64)` so its 128 bytes span exactly two cache lines as claimed):

- `tag` @0 (u32) — the independent 32-bit Zobrist fold of rules-DD §5 (separate random table, not a truncation of the 64-bit key).
- `smp_word` @4 (u32) — RESERVED for the future SMP publication word; written as literal zero and ignored on probe in s1.
  Its derivation is explicitly DEFERRED to the future SMP design amendment, which must define a coherence check covering EVERY semantically published byte of BOTH 16-byte halves (a scheme binding only one half cannot detect a torn cross-half read and is rejected in advance); s1 makes no coverage claim.
- `lo` @8, `hi` @16 (f64 each).
- `move_x` @24, `move_o` @25 (u8; legal values 0–80; sentinel `0xFF` = no conditional move, written ON STORE when absent).
- `depth` @26, `gen` @27, `flags` @28 (u8 each).
- `pad` @29–31 (3 bytes, written zero on store, never read).

Empty state: `occupied` bit clear — zero-initialization therefore IS the valid empty entry, and empty entries are never interpreted for moves or values (the `0xFF` sentinels appear only in occupied entries; no contradiction between zero bytes and sentinel semantics).

Buckets: 4-way, 128 bytes = two cache lines; replacement prefers (in order) empty slot, stale generation, shallower depth, then narrower usefulness (wider stored interval evicted first among equal depth).
Store rule: never overwrite a same-ENTRY store with a strictly worse one (shallower AND wider); merge same-entry intervals by intersection when both are sound at equal depth.
Store-side entry identity (collision-aware): in play mode, "same entry" is bucket + 32-bit tag; in full-key mode, "same entry" is bucket + tag PLUS a fieldwise match against the snapshot AT THAT WAY — a tag match with a snapshot mismatch is a COLLISION at store time: the scan continues to find a semantic match, otherwise selects a distinct empty or replacement-policy way, so two positions colliding in both key and tag genuinely occupy two ways.
Only a semantic match may use the merge/protection rule; sidecar snapshot writes and invalidation are bound atomically (single-threaded: same operation) to the entry write or replacement they describe.
Probe rule: a hit may be used when `entry.depth >= remaining depth` OR the stored interval already satisfies the node's window/precision request.
Fixture/acceptance (full-key) mode storage mechanism (per-ENTRY, not per-key): the play entry carries no Position identity, so full-key mode maintains a SIDECAR array with the SAME (bucket, way) geometry as the table itself, holding one identity snapshot `{x, o, forced, tie}` per way.
Every store/replacement writes the storing position's snapshot into ITS OWN (bucket, way) slot in the same operation; the PRODUCTION probe seam, when configured in full-key mode, performs the rules-DD §5 fieldwise comparison against the snapshot AT THE PROBED WAY before returning any hit, and treats a mismatch as a miss plus a recorded collision event.
Because the binding is per way, two entries colliding in both the 64-bit key and the 32-bit tag occupy different ways with different snapshots, and each probe verifies against its own — a key-indexed map cannot do this and is rejected.
The sidecar exists only in full-key mode (a TT construction flag), has the TT's lifetime, and is absent — zero cost — in play mode; oracle-equality runs can therefore never be polluted by a collision, through the same probe path production uses.
SMP-readiness (operator decision G2, honestly scoped): s1 locks only the PRECONDITIONS a future lock-free publication needs — the 32-byte size, 16-byte entry alignment (two atomically loadable halves), and the reserved `smp_word` — while the publication discipline itself (coherence derivation, read/write validation sequence) is deferred to a future design amendment; s1 asserts the frozen offsets and alignment, not sufficiency of any particular checksum.
Sizing: configurable power-of-two; the default is set at PLAN time after profiling (plan task, not a design constant).

## 6. Selective widening

At free-choice nodes (forced board closed or won → ANY), the full both-player move set can reach 2 × 81 candidates; widening searches a schedule of top-k candidates per side, ordered by the rules-core tactical masks (macro win, local win, block, fork from the local table) then routing quality (destination board's tactical richness), with a guaranteed minimum coverage floor per side.
Forced-board nodes (≤ 9 legal cells per side) are always full-width.

Quality semantics under widening (locked shape, rules-DD §10):

- Visited-subset propagation is one-sided sound: the min over a subset of X-children is an UPPER bound on `a`, and the max over a subset of O-children is a LOWER bound on `b`; pushing these through F (monotone) yields a sound one-sided bound on the node's T. When only such sound one-sided information exists, the node reports `quality = bound` with the sound side tight and the other side the trivial 0 or 1 (or an inherited sound bound).
- The unsound side may be sharpened for SEARCH GUIDANCE ONLY by evaluator estimates (section 8); any value influenced by an evaluator estimate reports `quality = estimate` and its `[lo, hi]` reverts to the widest sound enclosure available.
- A node whose move set was not exhausted can NEVER report `exact` (rules-DD §10, restated here as a hard invariant with a dedicated acceptance row).

`k`-schedule values and the coverage floor are PLAN-time tunables with committed defaults; the design constraint is only the ordering source, the floor's existence, and the quality semantics above.

## 7. Root bid layer

Inputs: the root position's conditional interval(s) from search, `RootContext { seat, budget_x, budget_o }` (rules-DD §9), `M = budget_x + budget_o`, `E(s)` = remaining empty-cell count.

Forced-classification gate (F-P2, tighten-only; M2 fold — exact conversion contract, no floating multiplication anywhere at the gate):

- Endpoint decomposition: each binary64 endpoint is decomposed EXACTLY from its bit pattern into `t = m * 2^e` with integer mantissa `m < 2^53` and integer exponent `e` (via `std::frexp`/bit extraction; NaN/Inf are structurally impossible for a clipped TInterval and asserted).
- X side (uses the UPPER endpoint `T.hi = m * 2^e`): classify X-forced iff `budget_x - E(s) > T.hi * M`, decided exactly as `(budget_x - E) * 2^{-e} > m * M` for `e <= 0` (the only case for a value in `(0,1)`; `t = 1` is handled as the literal integer comparison `budget_x - E > M`, and `t = 0` as `budget_x > E`).
  Overflow bounds: `m * M < 2^53 * 2^32 = 2^85` fits `__int128`; the left shift `(budget_x - E) << (-e)` is overflow-checked — if the shifted value exceeds 128 bits the inequality is decided by magnitude (the left side is positive and astronomically larger, so X-forced holds iff `budget_x - E > 0`).
- O side with the LOWER endpoint `T.lo = m * 2^e`, stated literally (no “same scheme” deferral): O-forced iff `T.lo * M - budget_x > E(s)`, decided exactly as `m * M > (budget_x + E) * 2^{-e}` for `e <= 0`; literal endpoint cases: `t = 0` ⇒ never O-forced (`0 - budget_x > E` is impossible); `t = 1` ⇒ O-forced iff `M - budget_x > E`.
  O-side overflow rule: if the shift `(budget_x + E) << (-e)` exceeds 128 bits, the right-hand side is astronomically large, so the inequality is FALSE and the state is not O-forced — the overflowing shift is never evaluated, mirroring the X side where an overflowing `(budget_x - E) << (-e)` with `budget_x - E > 0` makes X-forced TRUE.
- Both comparisons are STRICT and use the sound endpoint AGAINST the classification (hi for X, lo for O), so a rounding-widened interval can only fail to classify, never misclassify.
- A near-band interval that cannot prove the strict inequality falls through to the matrix path; there is no epsilon heuristic and no float product anywhere at this gate (operator decision G1's purpose).
  Witness for why bit-pattern decomposition is REQUIRED: binary64 `1e-9` exceeds exact `10^-9`, yet the float product `1e-9 * 10^9` rounds to exactly `1.0`, so `ceil` of the float product returns 1 while the exact scaled value has ceiling 2 — a naive conversion under-estimates an upper endpoint and can emit an unsound forced classification (acceptance row A7 carries this vector).

Bid matrix path (inside the band, or whenever no forced classification holds):

- Action space (M1 fold — the engine plays the SAME game as theory's reference, `lp_reference.py::_action_matrix`): each side's actions are the CARTESIAN pairs `(bid, move)`.
  F-C9a's bid-only reduction is NOT used at the root: it requires the winner's move to be the extremizer for the post-payment discrete budget state, and the TT's single budget-independent conditional move is not proven to be that extremizer inside the band, so the move dimension stays explicit.
- Candidate bids per side (theory-parity anchors, `_candidate_bids`): with `S` = the acting side's stack and `k* = round(r_root * M)` from the analytic critical fraction `r` of F-C2 (`r = 0` in the zugzwang branch): `{ 0, S, floor(S/4), floor(S/2), floor(3*S/4) } ∪ { k* + d : d in {-2,-1,0,+1,+2} }`, filtered to `[0, S]`, deduplicated, sorted — the reference's exact semantics.
- Candidate moves per side (theory-parity, `_candidate_moves`): `dedup([preferred] + legal_moves)[:4]`, where `preferred` is that side's conditional best move from the root search; the ttt3 acceptance instantiation replicates the reference BYTE-FOR-BYTE, including cell-index-ascending legal-move order and the opening-board quadruple `(0,1,4,8)`.
  Production UTTT uses the same construction with the tactical-mask ordering ranking `legal_moves` — a documented divergence that is safe because A8 parity is asserted exactly at the ttt3 fixture states, where the constructions coincide.
- Payoff entries (theory-parity semantics): the higher bid wins (tie → `h` moves); the child is the winner's move applied; the entry is the child's outcome at the POST-PAYMENT budgets.
  Ply-0 coin tie (amendment 2 — panel finding; replaces the implemented own-seat substitution, which was unratified optimism that scores a real coin flip as a certain win): when the root tie state is `null` (move 1), a BID-TIE matrix entry is the FAIR-COIN EXPECTATION `0.5 · E_X + 0.5 · E_O`, where `E_X`/`E_O` are the entry values of the two conditional continuations (X resp. O wins the tied auction, each with its F-C4 successor tie owner) — derived from R1's 50/50 coin and consistent with F-C6's refusal to value the simultaneous game by fiat; the averaged entry is exact iff BOTH branch values are exact, else estimate-tainted.
  P2 forced-branch ACTION policy (amendment 2, corrected per review-11 M1 — F-C8a proves `ceil(r · M)` only for the TRUE backup critical fraction and its matching extremizing child, never for the heuristic midpoint anchor; witness on this design's own carriers: sound aggregates `a = [0,0]`, `b = [0, 0.3]` with true extremes give `T = r = 3/13`, the gate fires at `bx = 25, E = 1, M = 100` since `24 > (3/13)·100`, yet the midpoint bid `ceil((3/23)·100) = 14` lets O bid 15 and reach `25 − 0.3·85 = −0.5` — certificate destroyed; the C8a bid is 24):
  - CERTIFIED branch: the C8a bid `k = ceil(r · M)` (exact rational reading; ceiling, distinct from the matrix `k*`'s half-up; clamped to `[0, own stack]`, affordability per C8a) plus the matching extremizing conditional move is published ONLY when the certificate carrier is established — both root aggregates `a` and `b` are exact-quality (complete, degenerate within the stated ulp slack) with their recorded extremizers, so `r = (b − a)/(1 − a + b)` is the true critical fraction within ulps; X and O sides symmetric on their respective certificates; zugzwang branch (`a > b`): bid 0 with the tie-owner move per F-C3.
  - HEURISTIC branch: when the gate fires but the aggregates are NOT exact-quality, the forced classification is still REPORTED (the gate's value claim is sound on the adverse endpoint regardless of action), but the published ACTION comes from the ordinary matrix path within the reserve — never from a midpoint-anchor bid dressed as a certificate; matrix incompleteness falls back to staged publication as everywhere else.
  - Acceptance: a mutation-sensitive certificate-preservation battery pins BOTH branches, including the inexact-aggregate witness above (must route to the matrix, must NOT publish 14) and an exact-aggregate case publishing the true `ceil(r · M)`; an injected-point `r` check alone is insufficient.
  Staged publication (amendment 2 — clarification the panel's Critical exposed): "publication of the previously staged root result" INCLUDES the staged conditional best move for the acting seat (`best_x` for X, `best_o` for O, legality-preferred) — never an arbitrary legal move; the bid falls back to `0` on an in-band deadline exhaustion.
  Play-mode root window (amendment 2, literal construction per review-11 M2): production play passes `Window W = [w_lo, w_hi]` built from the integer coordinates `(bx, M, E)` as
  `w_lo = (bx − E <= 0) ? 0.0 : div_down(double(bx − E), double(M))` stepped one further ulp DOWN, and `w_hi = (bx + E >= M) ? 1.0 : div_up(double(bx + E), double(M))` stepped one further ulp UP (the section-2 directed primitives; int64→double conversions are exact for the validated `M < 2^32` domain), clipped to `[0, 1]`; `M = 0` yields the full `[0, 1]` window.
  Outward rounding makes `W` a strict SUPERSET of the exact P2 band `[(bx−E)/M, (bx+E)/M]`, so a window-cut return disjoint from `W` is strictly outside the band; sufficiency does not rest on that alone — the EXACT gate remains the SOLE classification authority and re-checks every returned sound endpoint by integer comparison, and any bound too weak to decide simply yields `InBand` and the matrix path (safe fallback, never unsound).
  Acceptance: exact window bytes asserted at the lower boundary, upper boundary, both clipped endpoints, and `M = 0`; cuts proven non-vacuously enabled THROUGH `EnginePolicy` (nonzero cut counters in a production-shaped run); and the P2 verdict proven identical to full-window search on the sample.
  Analyze-mode resource contract (amendment 2, per review-11 M3): `uttt_engine analyze` rejects `depth > 12` as a per-line error (error object emitted, processing continues with the next line); accepted requests run with a fixed node cap of `5,000,000` and no other stop source; an over-budget search terminates at the cap and returns the SCHEMA-VALID result of the last completed iteration with `complete: false` (the fitter and P1b consumers already key on the existing fields; no shape change); a named E2 case at an adversarial depth proves bounded termination and that contract.
  Carrier contract (M3 fold): the common production matrix carrier and the RM+ input are FINITE binary64 values in `[-1, +1]` (asserted finite and in-range on every store); the EXACT providers (terminal, P2-gate, solved-scale discrete oracle) are restricted to the exactly representable values `-1.0`, `0.0`, `+1.0` — the reference's `{0,1,2} - 1` mapping — widened LOSSLESSLY into the common carrier; the in-band production entries of amendment 1 occupy the continuum; A8 compares the exact provider's ordinal and coordinates before/alongside that lossless widening, and A9 carries a mutation that quantizes an in-band graded value to an integer and must turn a named test red.
  Provider seams (rev-5 fold — TWO distinct providers, split exactly along theory's own solver boundary; a continuous threshold evaluator is NOT a discrete payoff oracle):
  the matrix construction consumes (i) an ACTION-ANCHOR provider supplying `r`, `best_x`, `best_o`, and (ii) a CHILD-PAYOFF provider keyed by the exact coordinates `(child, successor_h, post_payment_bx, post_payment_bo)`; the identical engine action-construction, winner-resolution, payment, and transition code runs regardless of binding.
  Production bindings: anchors from the root interval search (deterministic `r_root` rule below; `best_x`/`best_o` = the root's conditional best moves); payoffs from the TInterval backend — a child forced by this section's exact gate at the post-payment budgets (or a terminal) yields an exact ordinal entry `{−1, 0, +1}`.
  In-band production payoff (amendment 1, operator-ratified G5 — replaces the rev-5 sound-midpoint sign, which is information-free when the sound interval is `[0,1]`): an in-band child yields the CONTINUOUS payoff `clamp(κ · (p − t_est_child), −1, +1)` with `p = bx2 / M2` and `κ` a PLAN-committed scaling constant (default `κ = 8`, saturating at `|p − t_est| ≥ 0.125`), computed in plain double arithmetic — permissible here because the entry is declared `exact = false` and estimate-quality guidance, never a classification (the exact-comparison machinery remains mandatory at the P2 gate and for exact ordinals); any such entry taints the root report to `quality = estimate`, never relabeled exact.
  Zero-total totality (M1 fold; policy operator-ratified as G6, superseding an interim `p := 1/2` convention the operator overturned): terminal authority applies FIRST (a terminal child classifies exactly regardless of budgets).
  For a NONTERMINAL child at the reachable post-payment coordinate `M2 = 0` (an all-in winning bid pays the entire remaining stack), the game REDUCES EXACTLY to standard alternating-move UTTT from that state: every remaining auction is a forced 0–0 tie, the tie owner moves, and the zero-zero chain strictly alternates per F-C4 — so the position is simply won, lost, or drawn, and no chip-share/threshold comparison describes it (the exact zero-scale oracle disagrees with a `p = 1/2` threshold reading on 2,664 of 9,040 reachable states, per the review-7 calibration).
  The payoff entry is therefore the ALTERNATION-GAME EVALUATION, computed by a DEDICATED alternating solver — NOT the threshold search (review-8 M1 fold):

  - Locked recurrence, X-centric `V(s, h)`: terminal X macro win `+1`; terminal O macro win `-1`; all-closed at zero chips `0` (exact chip equality); nonterminal `h = X`: `V(s, X) = max over m in legal_X(s) of V(apply_X(s, m), O)`; nonterminal `h = O`: `V(s, O) = min over m in legal_O(s) of V(apply_O(s, m), X)`.
    Only the tie owner's marked child set is generated at each ply; no threshold interval aggregation, `F`, zugzwang branch, P2 gate, bid candidates, `p`, or `κ` participates anywhere inside this sub-search.
  - Horizon score (locked transformation of the existing evaluator): `V_horizon(s) = clamp(1 − 2 · eval_estimate(s), −1, +1)` — the threshold estimate `t ∈ (0,1)` maps to the X-centric outcome scale (`t → 0` X-favored → `+1`; `t → 1` → `-1`; `t = 1/2` → `0`), finite/range-checked, independent of `h` (the budget-independent evaluator sees position features only; `h` enters through whose turn the recurrence reached the horizon — disclosed estimate behavior), quality `estimate`.
  - Exactness: the backed value is EXACT (ordinal `-1/0/+1`) only when the full-width sub-search resolved every leaf to a terminal; any horizon evaluation or widening taints it to `estimate` per the section-8 table.
  - Semantic isolation (review-8 M2 fold; identity per review-9 M1): the alternation solver NEVER reads or writes the persistent threshold TT — its memo is a PRIVATE per-matrix-construction table, discarded with the matrix; the frozen 32-byte threshold-TT layout and its value domain remain untouched, and A9 proves the threshold table's contents are bit-identical before/after a zero-total sub-search.
    Memo identity (collision-safe): the COMPLETE entry identity is the full semantic position identity PLUS `h` PLUS `remaining_depth` — production uses the `PosId {x, o, forced, tie}` fieldwise seam (`Position::identity_equal`), the ttt3 acceptance model uses its full board identity; the 64-bit hash serves ONLY as the bucket selector, and semantic equality is checked before EVERY hit (a hash match with a semantic mismatch is a miss, never a value).
    Reuse rule: a memo entry is reusable only on exact identity match including `remaining_depth`; the stored `depth` is therefore part of the key, behavioral by construction, never advisory metadata (budgets are correctly omitted — every state in the sub-game sits at the constant coordinate `(0, 0)`).
  - Cost and cancellation (review-8 M2 fold): at most ONE sub-search per unique `(child, successor_h)` per matrix construction, cached in that private memo and reused across every action pair that reaches the same coordinate; the node/depth budget (the same per-child limits as sibling positive-total child searches, charged PER UNIQUE CHILD, never per matrix entry) and the matrix's hard deadline/stop callback bound every invocation; an incomplete or cancelled sub-search marks the matrix construction INCOMPLETE, causing publication of the previously staged root result — a default or partial payoff is never synthesized, stored, or fed to RM+.

  The payoff function is thereby TOTAL on every legal coordinate, every stored value is validated finite in `[-1, +1]`, and no division by zero exists on any path.
  `t_est` (estimate center): `SearchResult` additionally carries `double t_est` — for exact/bound-quality nodes it is the sound-interval midpoint; for estimate-quality nodes it is the evaluator-informed center propagated through the live search by the SAME point recurrence section 3 defines on scalars (min over X-children, max over O-children, ordered `F(a_est, b_est)`, zugzwang selection by tie owner, branch decided by direct comparison with equality coinciding at `a_est == b_est` per F-C7's seam continuity); a pure TT hit, whose 32-byte entry stores no estimate center (frozen layout), uses its stored interval midpoint as `t_est` — conservative and disclosed — and the standing TT usability invariant is PRESERVED and named as a test: estimate-quality entries are never reusable as pure hits, so live matrix-child searches always reach evaluator leaves and estimator starvation cannot occur; the root matrix children are searched live, so their `t_est` values carry real evaluator information.
  Averaged-profile action extraction (amendment 1, seat-aware — M2 fold): the matrix is X-centric (`+1` = X win), so the two seats compare in OPPOSITE directions.
  Among the X/row actions of maximum averaged probability, prefer the LARGEST expected payoff `Σ_j A[i][j] · y[j]` against O's averaged profile `y`; among the O/column actions of maximum averaged probability, prefer the SMALLEST expected payoff `Σ_i x[i] · A[i][j]` against X's averaged profile `x` (O minimizes the X-centric value).
  Only on exact equality of those expected payoffs — compared on the actual finite binary64 values the solver produced — fall back to the lower bid, then the lower move index; fully deterministic.
  A9 carries symmetric mutation-sensitive fixtures: equal maximum probabilities with unequal expected payoffs proving X selects the larger and O the smaller, plus an exact expected-payoff tie proving the bid/move fallback.
  Solved-scale ttt3 acceptance bindings (A8): anchors from the EXACT CONTINUOUS solver node (theory's `continuous.Node` semantics: exact-rational `r`, `best_x`, `best_o`), so action labels match theory byte-for-byte; payoffs from an EXACT DISCRETE budget-indexed value oracle — a test-side backward-induction reproduction of theory's `solve_discrete(scale)` table (or the injected theory table itself) looked up at the exact post-payment coordinates.
  The exact CONTINUOUS evaluator (A3's containment reference) supplies only continuous metadata and is NEVER the payoff or classification authority; P1a authority at solved scales is the exact DISCRETE oracle, per F-P1a.
  The landed fixtures witness why the split is mandatory: `threshold-a-equals-b-zero-critical` carries continuous `T = 1/2` with a discrete DRAW at equal budgets, while the conditional root fixtures carry continuous `T = 1/2` at equal budgets yet resolve to different discrete winners by `h` — no threshold-plus-equality rule reproduces the discrete table.
  Production `r_root` derivation (deterministic, locked): `a_mid = (a.lo + a.hi)/2`, `b_mid = (b.lo + b.hi)/2` (plain double arithmetic); `r_root = 0` if `a_mid > b_mid`, else `(b_mid - a_mid) / (1 - a_mid + b_mid)`; `k* = half_up(r_root * M)` replicating `_round_fraction`'s half-up semantics (`floor((2n + d) / 2d)` on the rational reading).
  This midpoint-seeded anchor selection changes only WHICH candidates are considered inside the locked construction (anchors, dedup, ordering, Cartesian product are never locally changed); it carries no exactness claim — production anchor adequacy is the accepted heuristic inside P2's no-exact-solver band, and reported value quality continues to reflect value provenance alone.
  Any narrowing of the action construction is a governing amendment (it changes the canonical spec and the theory cross-check target), never a local engine choice.
- Solver: regret matching plus (RM+) over the candidate matrix; the reported strategy is the AVERAGED profile with its average regret and an exploitability figure computed by best response over the candidate set — exactly the F-C10 contract; last-iterate convergence is never asserted. Iteration budget is a PLAN-time constant bounded by the clock layer.
- LP cross-check: test-time only. Theory's exact LP reference values (the landed conditional root fixtures and any matrix fixtures) are compared against the engine matrix path in acceptance tests; the engine runtime never calls an LP.

Knife-edge classification: at solved scales the exact oracle is authoritative (F-P1a) and appears only in tests; at larger scales an exactly representable `p = T` is classified for the tie owner per F-P1b, and every surface that prints it — analysis metadata included — must label it `convention`, never established optimal play.
Root `h = null` reporting follows F-C6: both conditional values, envelope lo/hi, no simultaneous-game claim.

## 8. Evaluator and quality mapping (operator decision G3)

The evaluator returns a threshold ESTIMATE in `[0,1]` for a horizon node, budget-independent (T's budget independence), built from local-table features already computed by the rules core: per-board status and tactical masks (win/block/fork counts), macro-line potential over open boards, and routing quality of the forced-board graph.
Form: a linear model over the feature vector squashed to `(0,1)`; weights live in one committed header with a documented hand-initialized set.

Offline fitting seam: a committed script regresses the weights to deep-search interval midpoints over a committed sampled UTTT position corpus (deep-search bootstrap); the fit run is a plan task whose output is recorded, and it is non-blocking if the hand weights already clear the tournament bar (G3, operator-ratified).

Quality mapping (single normative table for rules-DD §10):

- `exact` — full-width to terminal resolution below the node; the reported interval is NOT degenerate (outward rounding accumulates ulps) but its width is bounded by the stated per-depth ulp slack; `exact` describes the search completeness, never zero width.
- `bound` — every contribution is sound (terminal, exact, or one-sided widening bound); no evaluator value entered.
- `estimate` — any evaluator value or unsound sharpening entered anywhere below the node.

`depth` is the completed-iteration horizon; `complete` is whether the reported iteration finished before the soft clock; iterative deepening publishes only the last fully completed iteration (rules-DD §10).

## 9. Time management

Iterative deepening on horizon depth; soft clock 27 s (dispatch-locked), hard abort at 29 s with the previously completed iteration's result already staged, so an abort can never publish a partial iteration.
The search checks the clock at node-count intervals (PLAN-time constant) rather than per node.
Root matrix iterations run inside the same budget after the final completed deepening iteration, with a reserved minimum slice (PLAN-time constant) so the matrix path always executes when the gate falls through to it.

## 10. Module decomposition (engine/ additions; names final at PLAN)

- `engine/src/search/tvalue.hpp` — TInterval, outward rounding, F and its preimage algebra (pure, exhaustively unit-testable).
- `engine/src/search/backup.hpp` — branch selection, child aggregation, hull rule (pure).
- `engine/src/search/tt.hpp` — entry layout, buckets, probe/store rules.
- `engine/src/search/search.cpp` — DFS driver, windows/cuts, widening schedule, deepening loop, clock.
- `engine/src/search/game_model.hpp` — the `GameModel` concept: children enumeration with mover marks, terminal classification onto {X-macro, O-macro, all-closed}, tie transition; instantiated by UTTT (production, over Position) and by a test-only ttt3 auction model (acceptance vs theory's solved scales).
- `engine/src/eval/eval.hpp` (+ committed weights header, + `tools/fit_eval.py` offline seam).
- `engine/src/root/bid.cpp` — P2 gate, candidate set, RM+, reporting.
- Tests extend the existing doctest suite plus theory-fixture runner; no harness or theory byte changes.

## 11. Acceptance-criteria design (rows for the c4+ PLANs; each row green only by cited output)

- A1 backup mathematics (M5 fold — owner-honest): the engine-owned CONTRACT rows run against the fixtures whose `consumed_by` includes `engine` (`thresholds_ttt3.json`, `closure_routing.json`, `legality.json`, `terminal.json` engine-tagged records); the `backup_cases.json` records remain `consumed_by: ["theory"]` and are used ONLY as explicitly labeled non-contract reference vectors (duplicated as engine-authored synthetic constants citing the fixture ids), unless and until the theory owner amends the tag under the outstanding MR17 decision, which this design routes to s1.orchestrator-planner and does NOT presuppose; engine intervals must CONTAIN each exact rational with width ≤ 8 ulps in pure-F composition — E2.
- A2 solved-scale oracle equality (F-P1a): the ttt3 GameModel instantiation, full-width, must reproduce the landed conditional root fixtures `threshold-root-conditional-h-x/-o` (engine-tagged in `thresholds_ttt3.json`) — interval containing the exact rational and the fixed-point comparison within the locked ±1 unit tolerance at the fixture scales; equality-point classification defers to the oracle records — E2.
- A3 interval soundness properties: on sampled positions, (i) deeper-search intervals are contained in shallower ones (with TT off), (ii) full-width intervals contain the exact-rational test evaluator's value on small boards — E2.
- A4 cutoff correctness, split per cut with non-vacuity witnesses (M5 fold): named fixtures force EACH mechanism to fire — min-side dominance, max-side dominance, each parent-window side, the precision cut, and the hull restriction — proven by nonzero per-cut counters (or equivalent mutation-sensitive evidence) in the run that asserts them.
  Full-window (`[0,1]`) zero-precision search must equal the unpruned full-width oracle exactly (within the stated ulp slack); bounded-window returns assert CONTAINMENT of the true value, correct-side window exclusion, and parent-decision neutrality (the parent's backed interval and chosen moves match the unpruned parent); precision-cut returns assert the requested-width semantics; the hull case asserts no cut fired on one branch alone — byte-identical intervals are NOT the oracle for bounded-window or precision returns — E2.
- A5 directed-rounding enclosure and totality: property test that every interval primitive and every composite (F both endpoints, both preimages with crossed endpoints, aggregation) contains the exact rational result, on randomized rationals PLUS named cancellation/endpoint vectors including the X-preimage witness `b = 0.25, t = binary64(0.2)` (exact value `1/14411518807585588`), `t.lo = 0`, `t.hi = 1`, the POINT windows `t = [0,0]` and `t = [1,1]` (total, no division, conservative full preimage returned), and `a` at the Sterbenz boundary `0.5`; a mutation flipping ONE intermediate rounding direction (e.g. `add_up` → `add_down` inside `F_down`) must turn a named enclosure test red — E2.
- A6 TT neutrality and collision honesty (per-entry): search with TT equals search without on the sample; layout static-asserts (size 32, every `offsetof`, alignment) compile-checked; full-key mode stores TWO positions colliding in BOTH the 64-bit bucket key and the 32-bit tag while differing in exactly one semantic Position field (`x`, `o`, `forced`, or `tie`) into two ways of one bucket, then proves each probe verifies against ITS OWN way's snapshot — the matching probe hits, the mismatching probe misses with the collision event counter incremented — THROUGH the production probe seam configured in full-key mode (a test-only predicate bypassing probe is insufficient); the STORE path is mutation-guarded — replacing the store-side snapshot comparison with tag-only matching must turn a named test red (the second colliding store would then merge into the first way); the zero-initialized empty entry (occupied bit clear) is asserted unreadable as a value or move — E2.
- A7 P2 gate (live mutation targets only — every mutation is a change the LOCKED algorithm can actually undergo): constructed near-band cases prove no forced classification unless the exact integer margin holds on the SOUND endpoint; the band case returns interval metadata and takes the matrix path; conversion-cliff vectors include `T.hi = binary64(1e-9)` at `M = 10^9`; mutation guards flip each of: (i) substituting the forbidden float-product comparison for the exact bit-pattern comparison (caught by the cliff vector), (ii) strict → non-strict inequality, (iii) hi/lo endpoint swap per side, (iv) each `t = 0` and `t = 1` literal branch, (v) the X overflow-by-magnitude branch, (vi) the O overflow-by-magnitude branch, and (vii) the matrix fallback — each flip must turn a named test red — E2.
- A8 root matrix parity (M3 fold): the engine's candidate action set must equal the theory reference's action labels EXACTLY on the fixture states, and the payoff matrices must agree entry-wise (exact rational comparison at fixture scales) BEFORE solver outputs are compared; the solved-scale bindings are the section-7 split providers — action anchors (`r`, `best_x`, `best_o`) from the exact continuous node so labels match theory byte-for-byte, and every payoff entry (including equality/knife-edge entries) from the exact DISCRETE budget-indexed oracle, never from float intervals or the continuous threshold; the test additionally proves the shared engine winner/payment/transition path REQUESTS exactly the coordinates `(child, successor_h, post_payment_bx, post_payment_bo)` for every entry and that every emitted payoff byte equals theory's; then averaged-profile exploitability ≤ a PLAN-committed epsilon and agreement with theory's exact LP values within the locked tolerance — E2.
- A9 metadata honesty: a widened node never reports `exact` (mutation-guarded); estimate taint propagates; P1b outputs carry the literal `convention` label; amendment 1 additions (M1/M2/M3/M4 folds): `t_est` propagation is BRANCH-COMPLETE mutation-sensitive at every locked seam — X-child minimum, O-child maximum, ordered `F(a_est, b_est)`, zugzwang for BOTH tie owners, the `a_est == b_est` equality seam, estimate-taint propagation, the TT midpoint fallback, and the named no-estimate-pure-hit usability invariant; the graded in-band payoff is tested two-sided with MAGNITUDES (`p` slightly above/below `t_est` yields small-magnitude opposite-sign payoffs; `|p − t_est| ≥ 1/κ` saturates at `±1`; all such entries `exact = false` with root `quality = estimate`); nonterminal `(0, 0)` post-payment children are tested for BOTH successor tie owners proving finite payoff, finite RM+ outputs, and deterministic publication, with the G6 alternation solver bound to its PRIMARY ORACLE (review-8 M3 fold): full-width alternating search equals theory's exact `discrete.solve(0)` on ALL materialized ttt3 `(board, h)` states with the count asserted explicitly (the review-8 probe: 22,186/22,186 including all 9,040 reachable, zero mismatches); named mutations each turn a row red — freeze `h`, swap the max/min polarity, admit the non-mover's child set, substitute the threshold backup for the recurrence; named horizon-score vectors pin `V_horizon = clamp(1 − 2·eval_estimate, −1, +1)` including the clip; threshold-TT isolation is proven by bit-identical table contents before/after a zero-total sub-search; per-unique-child counters prove at most one sub-search per `(child, successor_h)` per matrix; memo collision safety is proven by forcing two DISTINCT states onto the same hash bucket (injected-hash seam) and asserting each retrieves its own value, with a mutation removing the semantic-equality guard turning the test red, and unequal requested depths proving the depth component of the identity is behavioral; and a cancellation test proves an incomplete sub-search publishes the staged root result with NO partial payoff entering RM+; amendment 2 additions (panel fold): the seat-aware extraction fixtures use NON-UNIFORM averaged strategies so the maximum-averaged-probability primary filter is discriminated (a near-zero-probability action with marginally better expected payoff must NOT be selected), plus an exact-tie vector discriminating the lower-BID fallback; the deadline-exhaustion test asserts the STAGED BEST MOVE is published (the prior test pinning `legal.front()` is corrected); the zero-total alternation binding is exercised THROUGH `EnginePolicy` (a real matrix entry at post-payment `(0,0)` — reachable at E3 because the referee burns chips — with the policy's own diagnostics proving the solver ran and the threshold-TT stats snapshot unchanged across it, replacing the tautological standalone-table test); the quantization battery gains a vector whose double product rounds ACROSS the `.5` boundary so the float-product mutant is actually discriminated; the alternation cancellation battery gains a mid-loop case (stop firing after ≥ 1 child evaluated, no partial parent memoization); the ply-0 coin-tie expectation and the C8a forced-bid are each pinned by named two-sided tests; and a production-shaped run asserts the play-mode root window enables the cut machinery; an integer-quantization mutation of an in-band graded value must turn a named test red; and the seat-aware extraction fixtures of section 7 (X selects larger, O selects smaller, exact-tie fallback) are asserted — E2.
- A10 tournament (E3, via referee on main): ≥ 90% points (win 1, draw ½) against EACH of the four harness baseline bots over 50 paired seat-swapped games per bot at a fast time control, with the 27 s/30 s clock discipline separately spot-checked at full time control (operator decision G4) — E3.
- A11 clock discipline: no move exceeds 30 s at E3; the published value is always a completed iteration — E3 spot-check plus E2 unit test of the staging rule.

## 12. GRILL_LOCK

GRILL_LOCK_ID: GRILL-engine-c4-20260826
GRILL_REQUIRED: yes
GRILL_SOURCE:
- plan/design/audit relay read: engine-c4 DESIGN dispatch (engine-c4/DESIGN-orchestrator-planner-20260826-175144.md); theory landing receipt; engine-criterion1-rerun-1
- code/docs inspected: theory/FINDINGS.md @ 79ee0c14; DD-engine-rules-c1 §2/3/5/9/10/12; DD-harness-c1 baseline-bot and tournament-runner sections; notes/engine-gated-math-draft-20260825.md
- questions answered from codebase: tournament-runner shape and baseline-bot list (DD-harness-c1); metadata carrier semantics (rules-DD §10); Zobrist/collision policy (rules-DD §5); LP reference location (F-C10: theory-side, test-time)
- questions asked operator: 4 (below) plus the design-shape approval

Resolved decisions:
- G1 T numerics — directed-rounding double intervals; exact rationals test-side only — soundness of the P2 gate without exact-arithmetic runtime cost — source operator.
- G2 parallelism — single-threaded s1; TT entry layout SMP-publishable, publication discipline deferred — determinism for oracle-equality acceptance — source operator.
- G3 evaluator supervision — hand weights + deep-search bootstrap fit (committed corpus + script); fit non-blocking if hand weights clear A10 — source operator.
- G4 tournament bar — ≥ 90% points vs each baseline bot, 50 paired seat-swapped games per bot, fast TC with full-clock spot-check — source operator.
- G5 (amendment 1, 2026-08-27) graded estimate payoff — in-band production matrix entries are the continuous `clamp(κ · (p − t_est_child), −1, +1)` with the evaluator-informed estimate center, inside the unchanged RM+ averaged-profile machinery — resolves engine-c4-impl-blocker-1 (the sound-midpoint sign is information-free at real horizons, so RM+ correctly bid ~0 and A10 failed) — source operator.
- G6 (amendment 1 zero-total policy, 2026-08-27) alternation-game estimate — nonterminal `M2 = 0` matrix children are evaluated by the existing search on the degenerate zero-budget model (tie owner moves, `h` alternates per F-C4, evaluator at horizon), the entry being that game's outcome value directly — chosen after the operator identified the exact reduction to standard alternating UTTT and OVERTURNED the interim `p := 1/2` chip-equality convention (which mislabels 2,664/9,040 exact zero-scale states per the review-7 calibration) — source operator.
- Design shape (7 sections as presented) — approved — source operator.

Rejected alternatives:
- Plain double + epsilon at the P2 gate — heuristic epsilon is not a sound bound; P2 is tighten-only — rejected with G1.
- Exact rationals (capped) in the runtime — denominator growth infeasible; the cap reintroduces the soundness question — rejected with G1.
- Lazy SMP in s1 — nondeterminism taxes P1a acceptance and triage; strength bar does not require it — rejected with G2.
- Self-play SPSA tuning in s1 — wall-clock heavy; adds orchestration machinery — rejected with G3.
- 100%-sweep or ≥75% tournament bars — flaky versus unselective — rejected with G4.
- Best-first / proof-number search shape and Monte-Carlo tree shapes — no sound interval bounds, conflicting with P2 — rejected at design-shape approval.
- Out-of-profile explicit bid policy (amendment 1 alternative) — a handcrafted bidder bypassing the averaged profile would abandon the F-C10 contract as the action source and add an unprincipled policy surface — rejected with G5.
- `p := 1/2` zero-total chip-equality convention (interim, briefly ratified) — anchors a fictitious chip share where the real object is the deterministic alternation game; ~29% exact-oracle disagreement — overturned by the operator with G6.

Still operator-owned:
- None; PLAN-time tunables (TT size, k-schedule, iteration budgets, matrix epsilon, corpus size) are engineering defaults inside locked semantics.

Design-lock impact:
- Sections 2–9 are the locked semantics; section 11 rows A1–A11 bind the successor PLANs; the rules-DD §10 quality mapping is sharpened by section 8's normative table.

## 13. Boundary contracts

Writes: engine search/eval/root modules and tests (section 10); the offline fit script and its corpus; nothing outside `engine/` except the fit tooling committed beside it.
Reads: theory FINDINGS @ 79ee0c14 and fixtures on main (consumer, verbatim); harness protocol/log schema @ c935c29c (consumer; the metadata carrier remains harness-owned); rules-core surfaces at their landed state.
Target entity: the engine's search core, evaluator, and root bid layer conforming to this document.
Downstream consumer: harness game log and ui analysis view (metadata semantics of section 8), theory cross-validation (A2/A8), referee tournaments (A10).
Contract: no rules-core, protocol, or fixture contract change; quality metadata per the section 8 table; P1b always labeled convention.
Owner routing (M5): the outstanding MR17 decision — whether `backup_cases.json` gains `engine` in `consumed_by` — belongs to the theory owner and routes through s1.orchestrator-planner; until amended, those fixtures are non-contract reference vectors only (A1), and the theory file and tag are preserved untouched.
Proof: E2 rows A1–A9; E3 rows A10–A11.
No-consumer action: not applicable — consumers named.

## 14. Risks

- Interval width growth with depth could starve the P2 gate near the band; mitigated by TT exact terminals, the precision cut, and deepening — monitored by an A3 width statistic; if widths prove unusable the fallback is a wider fixed-point representation, which is a design AMENDMENT, not a silent change.
- The ttt3 GameModel is new test surface; scoped small (single 3×3 board, direct rules) and validated against theory fixtures before use as an oracle bridge (A2 ordering inside the PLAN).
- RM+ epsilon and candidate-set adequacy at knife-edges: bounded by A8's LP agreement row; material disagreement routes to the operator per F-P1c's calibration spirit.
- The zugzwang hull may be pessimistically wide where branches straddle; acceptable for s1 (quality degrades to bound/estimate honestly); recorded as a possible successor refinement.
