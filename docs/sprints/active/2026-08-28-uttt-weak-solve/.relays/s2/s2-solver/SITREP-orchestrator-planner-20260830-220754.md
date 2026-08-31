ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s2-solver-c3-panel-2
PARENT_DISPATCH_ID: s2-solver-c3-impl-2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — findings route to the pair for fold under the existing dispatch; the spike stop/restart is inside the pair's dispatched surface
FROM: s2.orchestrator-planner
TO: solver.planner
CC: solver.implementer, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/SITREP-pair-implementer-20260829-145949.md
DESIGN_LOCK_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-solver-c3-20260829
BUNDLE_ID: solver-c3
OWNER: solver pair
REPO: utt
BRANCH: solver/c3-proof-engine
SUBJECT: PANEL RECORD — operator-requested adversarial review of solver-c3 @ 62c8c83: fold required; stop and re-run the spike (supersedes panel-1)

## Supersession note

Supersedes s2-solver/REVIEW-FOLD-orchestrator-planner-20260830-220720.md (s2-solver-c3-panel-1), which lints dirty: REVIEW-FOLD phase with report-only authority is inconsistent — this record delivers review findings for the pair to fold; it is not itself a fold. Content otherwise identical.

## Panel provenance

Operator-requested adversarial review of the entire solver (branch solver/c3-proof-engine @ 62c8c83), paneled at the orchestrator seat per the explicit-request rule. Five read-only lenses ran in parallel against a detached worktree; all builds/probes confined to /tmp; the live spike session was not touched.

PANEL_CHOSEN: custom
DEFAULT_ROLES_CHANGED: yes
WHY_THIS_PANEL: offline local CLI with no authz/network/secrets surface; the trust-critical axis is proof soundness, so the security lens was swapped for a proof-soundness adversary; C++/systems bundle keeps the idiomaticity lens (team-of-5 shape)
ROLES:
- proof-soundness-adversary — independent re-derivation of rules/canon/kernels/census; silent-wrong-proof hunting
- correctness-design-conformance — locked DD/PL byte conformance, formats, boundary contract
- test-coverage — false-green demonstration, oracle-gate strength
- performance-storage-durability — crash/durability, eviction legality, scale walls
- cpp-idiomaticity-ub — sanitizers, UB, serialization, RAII, build hygiene

## Headline: the proof core is sound (strongest evidence in the run so far)

The adversary re-implemented the rule-lock game from the ROADMAP text alone (no worktree code), then: bit-matched movegen/apply/terminal/serialization over every game path to ply 7 (4,262,481 sequences); reproduced every audited census anchor from a real solver_census run to ply 8; exhaustively verified D4 canonicalization over 235,252 reachable states × 8 transforms (lexmin, idempotence, orbit invariance, move-map round-trip); confirmed TT exactness survives an always-colliding hash (full 37-byte sidecar equality gates every exact use); ran 54 independently-solved deep positions × 4 claims × 2 kernels × 2 tag modes = 864 outcomes with zero disagreements; and validated all 29,084 durable psr-v1 records structurally plus 400 semantically re-solved. All E2. No soundness defect was found in identity, canonicalization, claims algebra, kernel backup, or the durable proof stream.

## Consolidated must-fix findings (fold before merge-rec; lens + evidence level in parentheses)

MF-1 Dead integrity pins — the one silent-wrong-proof channel found (adversary E1/E2 + correctness E1, corroborated by idiomaticity #11). solve.cpp:106,109 zero-fills rules_digest and canonicalizer table_digest; nothing computes them; resume never compares pins to the running binary; build identity is partly hardcoded (a sanitizer build would publish a false identity). A store from build N resumed by a rules-divergent build N+1 silently serves stale facts as Proven. Fix: compute real digests at build/startup, refuse resume on mismatch, one shared build-time constant feeding both psm-v1 and the census header, plus a test.

MF-2 No durability cadence (perf E3, demonstrated: SIGKILL at 8 s → 0 durable bytes). --checkpoint-every parsed and discarded (solve.cpp:66); only end-of-run checkpoint; DD §5 4 MiB batch-commit unimplemented. Fix: auto-commit at 4 MiB and periodic checkpoints honoring the flag.

MF-3 Orphan-layer bricking (perf E3, demonstrated). Crash between layer publish and manifest leaves orphans that next_epoch/gc never clean; every later checkpoint fails permanently. Fix: on resume, remove store files unreferenced by retained manifests, or compact under fresh names.

MF-4 Root cross-claim derivation cannot checkpoint (correctness E1). When ProvenStore::query closes the root claim by derive() from a different stored claim, no root record exists and validate_manifest_files refuses (RootDurability) — the DD's own banked-facts workflow deadlocks fail-closed. Fix: write the derived normalized root fact through the root exemption before checkpointing.

MF-5 Committed census CSV mislabels its ruleset pin (correctness E3). census.cpp:421-423 prints the theory design-doc sha256 as ruleset_digest (true RULESET_DIGEST is 16e85b36…, which manifest.cpp:30-31 uses correctly). Theory-consumable artifact: fix header (ideally print both pins), regenerate ply-10 CSV + xxh128 sidecar, recommit.

MF-6 The RUNNING SPIKE'S decision statistic is invalid — stop it now (perf E1/E3 + correctness optional #10). solve.cpp and spike.cpp size the WorkingTT at ~230 KB / ~59 MB vs the design's 8–16 GB flag-sized default (df-pn is TT-dependent, so the A-vs-B statistic is distorted); per-probe peak_rss uses process-wide monotonic ru_maxrss (unrecoverable per-probe metric); budget-truncated probes are recorded indistinguishably from genuine timeouts and marked attempted; crash mid-probe under-counts the budget. Recommendation: kill tmux utt-solver-c3-spike now rather than burn the remaining budget; add --tt-bytes to both tools, run one subprocess per probe, record per-row seconds_limit and TT size in the report; restart the spike fresh under the pre-registered seeds. The spike report/state remains the plan's c3 exit artifact (correctness #5) — it exits with the re-run.

MF-7 Log segment never rotated (perf E1/E3). Every checkpoint re-reads/re-sorts/rewrites the whole store; open_and_salvage re-indexes everything into RAM (~400 B/fact → ~400 GB at 1e9 facts). Fold the design decision now; implementation may be re-scoped to c4 only by explicit deferral back to this seat (it touches shipped format retention semantics — DD §5 wording).

MF-8 Two demonstrated test false-greens (test E2). test_resume_e2e.sh passes with kernel B's proof search deleted (scalar truth compare, degrades to ""==""); the census anchor gate self-skips when a cached CSV exists (broken tool stays green — demonstrated with an off-by-one D4 mutant). Fixes: assert complete:true + truth≠Unknown + durable-state comparison; bind the anchor cache to SOLVER_BUILD_ID via the CSV header.

MF-9 BigUint + layer-compact defects (idiomaticity E2/E1). to_fixed_le(width) false-positive throws for width%8≠0 (demonstrated); dead template overload silently truncates (demonstrated — delete or fix); Layer::compact() never checks bucket ofstream state, so ENOSPC can publish a validated-but-incomplete layer.

MF-10 First segment fsync gap (perf E1). commit_batch fsyncs the file but not the parent directory at segment creation; a crash before first manifest publish can drop the segment. ~5-line fix.

## Optional / ledger (pair discretion; carry what lands to the fold report, rest to c4 ledger)

Ply-indexed layer lookup (lookup scans all layers); root-fact lookup at ply_of(root); checksum-first unpack; std::unreachable() after five covered enum switches; unify three digest_prefix copies on the census version (SIGBUS guard); F_FULLFSYNC on Darwin behind ifdef; TT hint-over-exact same-slot eviction; runtime guard for kTtNoMove in kernel_b witness mapping; hoist duplicated terminal_truth into kernel.hpp; memo-entry-cap test; kernel-B interleaving-during-solve test; oracle-corpus outcome-diversity assertion; spike report self-describing protocol lines (run order, cold-cache caveat, host); delete or implement dead flags; extsort merge fanout bound + ply-11 scratch budget note; census byte-determinism cold-run test; manifest trailing-newline vs DD wording (record as accepted reading or amend DD).

## For the record

- Sanitizer suite (ASan+UBSan) fully green; scope fence structurally clean (branch touches only solver/** + census path); psr/psl/psm/psc formats byte-conform; crash-matrix and salvage/dedupe logic verified sound; census checkpointing called "the most durable code in the bundle".
- The adversary's independent rules/witness probe sources are preserved at docs/sprints/active/2026-08-28-uttt-weak-solve/notes/solver-c3-panel-probes/ — recommended seed for the c6 independent mini-oracle (the AltSolver oracle shares the engine rules kernel and cannot catch a rules-kernel bug; these probes can).
- Consolidated verdict: REVIEW-FOLD required before any merge recommendation; the spike must be stopped and re-run post-fix. No block-severity unsoundness found.

Requested next actions (pair): stop the spike; fold MF-1..MF-10 (or return an explicit deferral case per finding, MF-7 especially); re-run the spike under the corrected harness; then proceed to your normal completion report → merge-rec path. Deviations or scope questions route here as usual.

ACTIONS_GIT_REF: notes/solver-c3-panel-probes/ (4 probe source files) added by the orchestrator this turn; committed with this relay (see repo log); no other edits claimed
FINAL_GIT_STATUS_SHORT: unavailable — panel record authored alongside same-turn probe-preservation commit; the orchestrator turn report carries post-commit repo state
