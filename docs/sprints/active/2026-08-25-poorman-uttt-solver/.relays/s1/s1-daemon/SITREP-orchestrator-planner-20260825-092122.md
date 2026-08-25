ROLE: Orchestrator Planner
PHASE: SITREP
AUTHORITY: report-only
DISPATCH_ID: s1-daemon-cutover-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator directed the cutover this session
FROM: s1.orchestrator-planner
TO: theory.planner, theory.implementer, engine.planner, engine.implementer, harness.planner, harness.implementer, ui.planner, ui.implementer, s1.orchestrator-reviewer
SUBJECT: DAEMON CUTOVER — s1 relay root is now engine-governed; new filing procedure for every seat

## Operator-directed daemon cutover (2026-08-25)

The operator directed activation of the relay engine for this run (the engine README's "not adopted" status line is ruled stale for kit 2.9.1).
The s1 relay root is now governed by a running relay daemon: epoch active, all 90 prior hand relays imported and marked hand-authored-import, INDEX.md and SEATS.md are daemon-rendered projections.

What changes for every seat, effective immediately:
1. STOP hand-creating relay files and STOP appending INDEX.md rows. The daemon is the only writer of the run record; direct edits are detected as divergence/bypass.
2. To file a relay: author your draft beneath the root at .engine/drafts/<your-seat>/<name>.md (canonical header first line, SUBJECT required), then run:
   <plugin-root>/tools/relay submit ".engine/drafts/<your-seat>/<name>.md" --key ".engine/seats/<your-seat>/<uuid>.key" --root <this run root>
   Your registration key is the single .key file under .engine/seats/<your-seat>/ — it was registered for you at cutover.
3. The daemon serializes admission, so INDEX append-order inversions are inexpressible going forward; the standalone relay-lint --index instrument no longer applies to the rendered INDEX. Root health instruments are now: relay status, relay verify, and relay lint --relay-root.
4. Authority semantics are unchanged: phases, TO/CC discipline, dispatch tokens, lineage gates, and all locked designs/plans/dispatch ids carry over verbatim into the imported record.
5. Dispatch-gate re-scope (supersedes the "INDEX must lint green" wording in the proceed relays): the delegated-dispatch condition is now that relay verify reports no unreconciled divergence and the dispatch relay itself is filed via daemon admission. The inversion census and monotonic-from marker question are OBSOLETE — the defect class is closed by the daemon.
6. The three orchestrator PROCEED-TO-PLAN relays were resubmitted through admission and now live in their own dispatch directories (theory-c1-proceed-1/, harness-c1-proceed-1/, engine-c1-proceed-1/); the hand originals are removed from the working tree (preserved in git history and the cutover archive).

theory.planner and harness.planner: with this gate re-scope, your approved plans' last blocking condition is resolved. Run your dispatch-time rebase + SCOPE_DIFF and, with all conditions evidenced, issue your delegated implementation dispatches — filed via daemon admission.

ACTIONS_GIT_REF: cutover + imports + seat registrations recorded in the engine ledger; repo commit follows this filing in the orchestrator turn report
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; rendered artifacts committed by the orchestrator immediately after this filing
