ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — rev 4 folds the three rev-3 residuals; all are algorithm-direction, concurrency-publication, and manifest-schema corrections under existing rulings
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 77d248f4c3c4134637bb6bab8f7dca78534ea6211d3950f37086432bdf83e6ac
GRILL_REQUIRED: yes
OWNER: solver
REPO: utt
SUBJECT: DESIGN-REVIEW REQUEST rev 4 — df-pn direction, TT pair publication, psm-v1 topology/recovery folded

## Team solver — s2-solver-c2: DESIGN-REVIEW REQUEST (rev 4)

Current scope for the `TO` addressee: read-only DESIGN-REVIEW. No source/test edits, no implementation branches, no commits, no PRs.

Forward amendment (same DESIGN_DOC_ID; digest floats; rev-3 digest was 7db7df40…). Architecture, GRILL decisions, and all closed folds untouched. Fold map:

- df-pn direction folded (§3): the switch signal is now proof-hardness — fire when PN(root) ≥ saturating_mul(10, DN(root)) in u64 saturating arithmetic, larger favoring suspension of the proof attempt; ∞/0 boundaries never reach the heuristic (PN=∞/DN=0 = disproven, DN=∞/PN=0 = proven, both handled by the outcome machine); two-checkpoint rule and scheduling-only/nonbinding semantics retained.
- TT pair publication folded (§5): entry + sidecar are one unit under a striped mutex taken by every reader and writer in every mode — the reviewed simple option; the mixed-generation A→B interleaving is stated in the doc as the reason no entry-ordering trick suffices; the 16-byte layout is retained as wire format only, and any future lock-free/seqlock protocol must arrive as its own reviewed amendment backed by c4 contention measurements. Tests extended (§10): paused A→B replacement interleaving proving matched-generation-or-miss for both probes, plus a c4 TSAN stress pass.
- psm-v1 topology and recovery folded (§6, §5, §10): literal complete schema — exact key names and nesting for build/canonicalizer/theory-contract objects, fixed four-entry claims array with normalized_to cross-field rule, layers array (ascending ply, at most one per ply, psl-header cross-check), log_segments with durable_bytes ≡ 0 mod 48, scalar grammars (hex32/64/74, fname regex + store-confinement + uniqueness, claim enum, exact inclusive uint ranges), fail-closed parsing incl. wrong-type and cross-field refusals; no canonical-byte claim (integrity rides the pointer digest). Manifests are now immutable versioned files manifest-<epoch>.psm plus an atomically published CURRENT pointer (psc-v1: name + xxh128), published only after the manifest and every named file are durable; recovery = CURRENT if valid, else highest-epoch fully-validating manifest (corrupt-new/valid-old wins), else explicitly-named uninitialized fresh start; retention keeps the previous generation until successor manifest AND pointer are durable, GC spares the newest two valid generations; §5 compaction retention aligned. Recovery tests added (corrupt-new/valid-old, missing CURRENT, first-checkpoint case).

Design doc to review:
- DESIGN_DOC_ID: DD-solver-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 above)
- Selected option: unchanged.
- Rejected alternatives: unchanged, plus single-mutable-manifest-path (named in §6 with the unlink reason).
- Boundary contract: unchanged.
- Acceptance criteria draft: design-lock impact updated — stripe-locked pair publication and psm-v1/psc-v1 schema, publication, recovery, and retention are named acceptance criteria.
- Operator decisions/defaults: unchanged; no fresh operator items.
- Open questions: none.

Requested response:
Use Template C (`PHASE: DESIGN-REVIEW`, `FROM: solver.implementer`, `TO: solver.planner`, `PARENT_DISPATCH_ID: s2-solver-c2`, same `DESIGN_DOC_ID`) and return `DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required`.

ACTIONS_GIT_REF: designs/DD-solver-c2-20260828.md amended in place (rev 4, sha256 77d248f4c3c4134637bb6bab8f7dca78534ea6211d3950f37086432bdf83e6ac); still uncommitted pending orchestrator batch commit
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-185204.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-implementer-20260828-185514.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-planner-20260828-175603.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-173615.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260828-193007.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192257.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192509.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(this seat's artifact: the design doc line; relay rows are daemon-rendered filings)
