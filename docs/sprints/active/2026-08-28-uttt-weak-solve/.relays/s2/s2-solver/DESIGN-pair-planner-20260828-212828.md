ROLE: Pair Planner
PHASE: DESIGN
AUTHORITY: design-only
DISPATCH_ID: s2-solver-c2
PARENT_DISPATCH_ID: s2-solver-c2
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E1
HUMAN_GATE_REQUIRED: no — rev 5 folds the three rev-4 representation residuals; all are TT-layout and store-schema corrections under existing rulings
FROM: solver.planner
TO: solver.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-solver/DESIGN-REVIEW-pair-implementer-20260828-194010.md
DESIGN_DOC_ID: DD-solver-c2-20260828
DESIGN_RECORD_KIND: design-doc
DESIGN_ARTIFACT: DD-solver-c2-20260828
DESIGN_SHA256: 5998cd3c6a6d6e76f6f8d9f37ec6db788b8068acc9d12a2cd13b91517783e616
GRILL_REQUIRED: yes
OWNER: solver
REPO: utt
SUBJECT: DESIGN-REVIEW REQUEST rev 5 — tte-v1 entry encoding, ply-0 root layer, segment-digest rule folded

## Team solver — s2-solver-c2: DESIGN-REVIEW REQUEST (rev 5)

Current scope for the `TO` addressee: read-only DESIGN-REVIEW. No source/test edits, no implementation branches, no commits, no PRs.

Forward amendment (same DESIGN_DOC_ID; digest floats; rev-4 digest was 77d248f4…). Architecture, GRILL decisions, and all closed folds untouched; only the three named representations were repaired. Fold map:

- MR-4A-layout folded (§5): complete tte-v1 16-byte encoding — word 0 = tag 56 | generation 8; word 1 = kind 2 | claim 2 | move 7 | pn 24 | dn 24 | reserved 5 (=0), with kind enum (empty / A-heuristic / B-exact-bound / reserved-refuses), move 0..80 + 127-none + 81..126-refusal band, and pn/dn as 24-bit saturating projections (0 literal, 16777214 finite-saturated, 16777215 ∞). The bit-capacity contradiction is resolved by placement, not compression tricks: the §3 switch heuristic reads root PN/DN from kernel A's in-memory root node at full u64 width — TT projections are ordering/threshold hints only and nothing exact rides an A-heuristic entry; B's exact bounds ride kind-2 entries under the sidecar rule. Round-trip/boundary tests added (§10). Per-way budget unchanged (~56 B).
- MR-4B-root folded (§5, §6): ply range is 0..81 in both the psl header and psm layers[]; ply 0 is exactly the root's layer (empty state popcount 0; a completed root proof always exceeds K, so it is durable), and the root's compaction/probe/extraction path is the ordinary per-ply path at ply 0. Ply-0 compaction/probe/resume/extraction test added.
- MR-4B-log folded (§5, §6, §10): log_segments entries now carry digest_xxh128 covering exactly bytes [0, durable_bytes); one integrity rule used consistently across pin list, publication, recovery, and tests — the durable range must verify on resume, and bytes beyond durable_bytes are salvaged per-record via psr-v1 checksums, truncating at the first invalid/partial record (torn tail defined). durable_bytes alignment and bounds preserved.

Design doc to review:
- DESIGN_DOC_ID: DD-solver-c2-20260828
- Location: docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md (sha256 above)
- Selected option: unchanged.
- Rejected alternatives: unchanged.
- Boundary contract: unchanged.
- Acceptance criteria draft: design-lock impact updated — tte-v1 encoding, ply-0..81 layer range, and the segment-digest-over-durable_bytes rule are named acceptance criteria.
- Operator decisions/defaults: unchanged; no fresh operator items.
- Open questions: none.

Requested response:
Use Template C (`PHASE: DESIGN-REVIEW`, `FROM: solver.implementer`, `TO: solver.planner`, `PARENT_DISPATCH_ID: s2-solver-c2`, same `DESIGN_DOC_ID`) and return `DESIGN_REVIEW_VERDICT: approve | must-revise | reject-narrow | human-decision-required`.

ACTIONS_GIT_REF: designs/DD-solver-c2-20260828.md amended in place (rev 5, sha256 5998cd3c6a6d6e76f6f8d9f37ec6db788b8068acc9d12a2cd13b91517783e616); still uncommitted pending orchestrator batch commit
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-174655.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-183530.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-190904.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-REVIEW-pair-implementer-20260828-194010.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-173611.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-180344.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-185204.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-solver/DESIGN-pair-planner-20260828-193327.md
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
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-implementer-20260828-193940.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192257.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/REVIEW-FOLD-pair-planner-20260828-192509.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-solver-c2-20260828.md
(this seat's artifact: the design doc line; relay rows are daemon-rendered filings)
