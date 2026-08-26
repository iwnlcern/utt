ROLE: Orchestrator Planner
PHASE: MERGE-GATE
AUTHORITY: merge-gated
DISPATCH_ID: harness-c4-merge-1
PARENT_DISPATCH_ID: harness-c4-merge-rec-1
RUN_ID: s1
SUBJECT: MERGE GRANT — harness-c4 fault-stream partition via PR 9 @ 793944d
CEREMONY_TIER: small
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — operator standing merge-now + remote/PR ruling; pair reconciliation on record
FROM: s1.orchestrator-planner
TO: harness.implementer
CC: harness.planner, engine.planner, s1.orchestrator-reviewer
IN_REPLY_TO: harness-c4-merge-rec-1/SITREP-planner-20260826-155630.md

## Authority

Operator standing ruling (remote + PR, merge now). Cycle record: planner-executed two-lens review, both approve; c4-A1..A7 independently re-verified (exact 23/15 partition census, 15/15 relocation byte-identity at 100% renames, suite 208 passed / 1 sanctioned skip, --check clean on both roots); scope exactly 3 modifications + 15 renames per the adopted option-C ruling.

## Granted actions (exactly this sequence)

1. Fresh fetch; reconcile any remote publish-lag as in prior grants (out-of-range files must be byte-identical to origin/main or the halt discipline applies); rebase or merge-forward PR 9's branch onto current origin/main if needed, re-running the suite if non-trivial.
2. Merge PR 9 (merge commit preferred) at verified exact head 793944d7d753e9c939b5169ffcc209efec3bd2da or its authorized merge-forward successor, body citing the option-C ruling and reconciliation lineage.
3. File the merge claim via daemon admission with this same DISPATCH_ID, ACTIONS_GIT_REF carrying PR number + merge sha + post-merge suite status.

## Cascade on landing (no new authority)

engine: criterion-3 round-trip rerun fires under the standing obligation against the now-conforming 23-stream engine corpus; with theory's landing (in flight under its own grant), the criterion-1 rerun and the full truth-table report close engine's standing obligations together.

DISPATCH MERGE

ACTIONS_GIT_REF: none — grant relay; no merge performed by this seat
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted grant relay; repo state carried in the orchestrator turn report
