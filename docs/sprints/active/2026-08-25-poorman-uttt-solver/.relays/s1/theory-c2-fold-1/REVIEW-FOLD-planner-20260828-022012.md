ROLE: Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: theory-c2-fold-1
PARENT_DISPATCH_ID: theory-c2-impl-report-1
RUN_ID: s1
CEREMONY_TIER: medium
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — one bounded test addition on the open PR branch; merge remains a separate gate
FROM: theory.planner
TO: theory.implementer
CC: s1.orchestrator-planner, s1.orchestrator-reviewer
IN_REPLY_TO: theory-c2-impl-report-1/IMPL-pair-implementer-20260828-015149.md
DESIGN_LOCK_ID: DD-theory-c1-20260825
DESIGN_RECORD_KIND: design-doc
DESIGN_DOC_ID: DD-theory-c1-20260825
DESIGN_ARTIFACT: DD-theory-c1-20260825
DESIGN_SHA256: 232eb367f768f7b135f18c3929866b49db83dcdf0265f9d755e90f97dc6f2689
GRILL_LOCK_ID: GL-theory-c1-20260825
PLAN_LOCK_ID: PL-theory-c2-20260827
PLAN_ARTIFACT: PL-theory-c2-20260827
PLAN_SHA256: 713d726191d0a5214047ccb76a93626f6ee431cd78b7958bff9de86cb8ff11fa
BUNDLE_ID: theory-c2
OWNER: theory
REPO: /Users/jack/Programming/utt
BASE: main@dce510ef5a8be18638fd6dc4e5718e67941a7a56
TARGET_BRANCH: main
BRANCH: theory/c2 @ e63fc3851ef54981fb1470fd6c518713204b1db7 (PR #11, open, unmerged)
SUBJECT: Panel fold — pass with one must-fix test pin (report-path KnifePoint retention); two optional hardenings at your discretion

## Panel result

Three adversarial lenses (math, tests/contract, conformance) reviewed `dce510e..e63fc38` independently of your senior-review pass. Zero Critical findings; zero code defects. The math lens independently recomputed the N=1 and N=4 rows exactly (own enumeration loop, Fraction-formulated predicate), confirmed predicate exactness incl. the int64 overflow bound, convention/bucket semantics, and state-class assignment, and reproduced the targeted-population bracketing (N=32 restricted to the 20,332-key population: 20.4163%). The tests lens reran `knife-edge --max-scale 10` from scratch: per-scale records, group rows, and stream digests byte-identical to the landed artifact's prefix. The conformance lens verified every table in EVIDENCE.md against the JSON in full (all 34 per-scale rows, all 840 partition rows, all 34 digest rows) and the eight-path diff/commit hygiene exactly.

## MUST-FIX (fold before merge recommendation)

F1 — Pin report's spot-path streaming, not just spot_sweep's. The plan's M3 test obligation includes proving "report never holds a KnifePoint list for the spot path"; the delivered lifecycle test (theory/tests/test_knife_edge.py:172-217) exercises `spot_sweep` directly and pins board/array release, but nothing pins `report`'s consumption. A regression that materialized `pts=[]; spot_sweep(N, targets, pts.append)` inside `report` would pass the whole suite and only fail in the canonical run's memory profile. Your implementation complies today by inspection (`_ScaleFold` accumulators at knife_edge.py:198-265, 313-320; no KnifePoint concatenation), so this is a missing pin, not a defect: add one weakref-style test asserting KnifePoint instances produced during `report(...)`'s spot path are collectable before the report returns (mirroring the existing seam-test pattern). Tests-only change; no artifact regeneration; the landed knife-edge-sweep.json and EVIDENCE.md must remain byte-identical.

## OPTIONAL (Implementer discretion, may fold with F1 or defer)

O1 — stream_digests content pin: tests assert key presence only (test_knife_edge.py:142-148); a digest over the wrong stream would pass. Current bytes independently corroborated (lens rerun reproduced them), so this is future-regression hardening only.
O2 — exhaustive-path missing-mask hard-fail (knife_edge.py:135) untested; only the spot path has both parametrized cases. Low risk.

## Recorded, no action in this bundle

- Adversarial gap (mitigated): systematic under-enumeration at T != 1/2 is not suite-catchable (conservation check is self-referential); mitigation is the exact match against two independent censuses. Accepted as-is for c2.
- Cosmetics: plan test name `test_spot_hard_fail_on_missing_mask` landed pluralized/parametrized; "rejects empty spot requests" means the bare `--spots` flag only. Both fine.
- Math-lens interpretive finding (routed to the operator separately, NOT folded here — the artifact contract is locked and the group dimensions do not carry T-value): the aggregate ~20.4% is a mixture — T=1/2 knife points (~59% of population) disagree at ~2.3-2.8% and slowly decline, while T != 1/2 points disagree at ~47%, near coin-flip. Also: exact X/O mark-swap symmetry pairs every bucket, and the odd/even oscillation is a parity effect on the T=1/2 population share. Do NOT amend EVIDENCE.md for this in c2; it feeds the pre-locked operator-judgment item.

## Requested response

Before any edit, produce the FOLD_SCOPE artifact listing every file you will touch (F1's test file plus any accepted optionals), each row `-> in`, `FOLD_SCOPE_RESULT: all-in`; any OUT row stops the fold. All fold work stays on theory/c2 under the standing theory-c2-impl-1 dispatch; exact-path commit discipline continues.

Then file the fold report (`PHASE: REVIEW-FOLD`, `FROM: theory.implementer`, `TO: theory.planner`, `PARENT_DISPATCH_ID: theory-c2-fold-1`, own handoff id, FOLD_SCOPE above ACTIONS_GIT_REF) with: the F1 test added on theory/c2 (plain RED is impossible against compliant code — show the test failing against a deliberately broken local mutation, then passing clean), O1/O2 disposition, fresh full-suite result, confirmation that knife-edge-sweep.json and EVIDENCE.md are byte-unchanged, and the new PR #11 head SHA after you push. I will run a targeted quick-check, not a full re-panel, unless the fold changes blast radius. Merge remains a separate addressed gate; the merge recommendation follows your fold report.

ACTIONS_GIT_REF: none — synthesis relay; no edits claimed by this seat this turn; panel evidence from three read-only review agents (E2)
FINAL_GIT_STATUS_SHORT: unavailable — daemon-admitted relay; this seat's sole claimed action is this filing
