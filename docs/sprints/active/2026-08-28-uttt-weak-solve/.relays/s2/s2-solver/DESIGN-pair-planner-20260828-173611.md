ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — design drafted with operator brainstorm + grill already folded; next transition is the Implementer's read-only DESIGN-REVIEW
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-orchestrator-planner-20260828-010352.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 39cf01f6dbd9d03b17671d5c018966faad5bb1f3d2f95e1837adbcfee45685d3
GRILL_REQUIRED: yes
OWNER: solver
REPO: utt
SUBJECT: DESIGN-REVIEW REQUEST — DD-solver-c2-20260828 (pluggable-kernel proof engine)

## Team solver — s2-solver-c2: DESIGN-REVIEW REQUEST

Current scope for the `TO` addressee: read-only DESIGN-REVIEW. No source/test edits, no implementation branches, no commits, no PRs.

Design doc to review:
- DESIGN_DOC_ID: DD-solver-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 above)
- Selected option: layered proof engine, local-first, with a `ProofKernel` interface carrying both df-pn (A) and exact-WDL negamax (B); pre-registered A-vs-B spike decides primary (≥2× median time-to-proof or B wins; ~3-day timebox); truth isolated in a durable hand-rolled ProvenStore (append-only 48 B records, work-threshold logging K≈10³, log-before-claim, per-ply compacted mmap layers); lossy 16 B-entry working TT; manifest-pinned checkpoint/resume with disposable kernel state; D4-only canonicalization at store boundaries per R-s2-1; census tool committed to ply 10 (O5); abstract CertificateSink until DD-theory-c2 locks.
- Rejected alternatives: RocksDB/LMDB store; negamax-without-spike; BFS layer pipeline as the solve; log-everything and ply-ceiling durability; day-one threading; retrograde tables; color-swap in v1 (all with reasons in the doc's GRILL_LOCK).
- Boundary contract: in the doc (§Boundary contract) — certificate seam theory-owned, census format for theory's proof-size estimation, seam friction routes to the orchestrator.
- Acceptance criteria draft: spike decision rule falsifiable and pre-registered (§4); every persistent byte owned with a crash story (§5–6: log-before-claim, checksum-truncate resume, manifest refusal); census format defined and anchored to audited numbers (§8); each decision traceable to an audit finding, ruling, measured number, or grill answer (traceability lines per section).
- Operator decisions/defaults: nine grill/brainstorm decisions recorded in GRILL_LOCK GRILL-solver-c2-20260828 (storage, color-swap, local-first, architecture, timebox, census depth, durability policy, objective order, threading); still-operator-owned items listed there (cloud vendor, ply-12 census, switch-ratio constant).
- Open questions: none blocking review; D4 normative convention and schema digest bind after DD-theory-c2 locks (manifest carries placeholders).

Requested response:
Use Template C (`PHASE: DESIGN-REVIEW`, `FROM: solver.implementer`, `TO: solver.planner`, `PARENT_DISPATCH_ID: s2-solver-c2`, same `DESIGN_DOC_ID`) and return `DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required`.

ACTIONS_GIT_REF: design doc created at docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 39cf01f6dbd9d03b17671d5c018966faad5bb1f3d2f95e1837adbcfee45685d3); left uncommitted per s2 orchestrator batch-commit practice
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-120356.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-140310.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-REVIEW-pair-implementer-20260828-144316.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-115015.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-134929.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-142748.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/PLAN-pair-planner-20260828-154729.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(only the last line is this seat's artifact; theory-pair filings and the harness lockfile are concurrent)
