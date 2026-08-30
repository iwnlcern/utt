ROLE: Pair Planner
PHASE: REVIEW-FOLD
AUTHORITY: fold-in-only
DISPATCH_ID: s2-theory-c3-fold-1
RUN_ID: s2
CEREMONY_TIER: large
EVIDENCE_TARGET: E2
HUMAN_GATE_REQUIRED: no — fold direction to the Implementer; merge remains the standing operator/orchestrator gate
FROM: theory.planner
TO: theory.implementer
CC: s2.orchestrator-planner, s2.orchestrator-reviewer
IN_REPLY_TO: s2-theory/IMPL-pair-implementer-20260830-024849.md
DESIGN_LOCK_ID: DD-theory-c2-20260828
DESIGN_RECORD_KIND: design-doc
PLAN_LOCK_ID: PL-theory-c3-20260829
BUNDLE_ID: theory-c3
OWNER: theory pair
REPO: utt
BRANCH: theory/checker-c3
PR: https://github.com/iwnlcern/utt/pull/13
SUBJECT: REVIEW-FOLD theory-c3 — panel verdicts on PR #13 @ 3f3a1f0: 1 must-fix, no blockers

## Panel record

PANEL_CHOSEN: custom
DEFAULT_ROLES_CHANGED: yes
WHY_THIS_PANEL: the artifact is ~2,000 lines of fresh code in a young language plus a conformance battery; risk classes are spec drift, false-green tests, wrong-accept/crash under novel inputs, and language misuse. No authz/network/perf surface, so security and performance lenses were dropped in favor of an adversarial-input lens and a C3-idiomaticity lens.
ROLES:
- correctness/design-conformance — DD §§2-7 + plan check-order/reason-code contract vs the C3 code
- test-coverage/false-green — battery/table fidelity + mutation-probing the tests themselves
- adversarial-input — novel malformed/tricky certificates and verdicts against the built binary
- C3-idiomaticity — 0.8.3 idiom conformance, panic-on-malformed-input class, allocator hygiene

Planner pre-verification (this seat): scope diff 282f504..3f3a1f0 = exactly the plan surface (22 files); frozen surfaces untouched; both suites reproduced independently (c3c test exit 0 under brew c3c 0.8.3; pytest 195 green).

## Verdicts

- design-conformance: optional. Check order, reason codes, VerifyResult/report contracts, orphan closure, D4/serialization/rules, XXH3 structure all conformant; long-input XXH3 cross-checked against reference xxhsum at 9 lengths up to 65536; independence import census clean; P4/R22/catalogue verified.
- test-coverage/false-green: approve. Battery table 44/44 identical to the plan; five planted checker defects all caught; C3 units genuine (runtime fixture parsing proven); per-session rebuild proven.
- adversarial-input: optional. ~85 crafted attacks: zero wrong-accepts, zero crashes on certificate/verdict BYTES; XXH3 differential clean; first-failure order held in every two-defect probe.
- C3-idiomaticity: must-fix. The byte-content boundary is fuzz-clean (~3,000 mutated inputs, all clean 0/1/2 exits), but the CLI PATH-ARGUMENT boundary can panic.

Consolidated: NO blockers. One must-fix before the merge recommendation; directed optionals below; the rest Implementer-discretion per protocol.

## Must-fix (fold before merge)

MF-1 — path-argument fault handling (idiomaticity findings 1-2; verify.c3:584, :585, :594):
- Live repro: `uttt-checker verdict /../<abs>/golden-winx.utv` aborts via an unwrapped `path::INVALID_PATH` fault — SIGTRAP, exit 133, no JSON — violating the one-JSON-report and 0/1/2 exit contracts.
- `tnew(manifest.dirname())!!` and `directory.tappend(member.path)!!` are the same class: unreachable on POSIX only through an implicit coupling between `valid_member_path` and `normalize`'s reserved-char table, and outright faulting under the WIN32 path environment.
- Fold: `if (catch)` all three sites — manifest-path construction failure maps to a clean reject (EV_PARSE, or exit-2 io_error at the main.c3 boundary; pick one and state it in README), member-path append failure maps to EV_MEMBER_PATH.
- Regression gates: a battery test invoking the verdict mode with a pathological path (e.g. `/../` ascent) asserting a clean exit with a one-line JSON report; a C3 unit for the member-append fault path if reachable in-test.

## Directed optionals (cheap, fold with MF-1; each is a one-liner or a doc line)

- Battery blind spot >240-byte digests (test lens): add one battery probe whose digested region exceeds 240 bytes (emitter-side helper emitting a ≥6-record chunk) so a pytest-only run also guards the XXH3 long path.
- Verdict member-major vs step-major first-failure (design lens): accept/reject is unchanged; pin the per-member interpretation as the ruling with one README sentence and a battery comment (hoisting resolution step-major is NOT requested).
- Path-length code mapping (design lens): document EV_PARSE as the framing-level code for the 1..4096 length bound (or move the bound into valid_member_path — either, documented).
- Backslash-in-path tightening (design lens): DD §4.5 does not forbid `\` in a segment; keep the stricter check but record it as a documented tightening in README, or drop it.
- 0-mark `forced=ANY` pseudo-root (adversarial lens): README note that Appendix A rule 2 is modeled solely via the initial `forced=4` state and the deliverable-mode root gate; no legality change.
- Drop the dead `streamed_totals[3] += rule_count` line (design lens nit).

## Implementer-discretion optionals (no direction)

Unify the reader error idiom (dead `io::EOF` fault channel vs pre-checked `!!`); `fn String? read_ascii16` signature; drop the redundant `lookup_key.copy` / consider replacing the `verified` set with the `indices` map (pays off at c6 scale); load-in-main for the verdict path to give unreadable-file exit 2; UTF-8 validator strictness comment.

## Ledger notes (no code change in this PR)

- DD ERRATUM CANDIDATE escalated separately to the orchestrator (s2-theory-c3-erratum-flag-1): the DD §4.3 manifest-row ply cap `0..80` cannot represent an 81-mark all-closed-draw terminal, which is a reachable state (nine full-drawn boards); the checker faithfully implements the DD and the current corpus is unaffected, per Appendix B's escalate-don't-adapt rule.
- XXH3 long-path assurance now rests on three independent legs: the 12 pinned vectors, the design lens's reference cross-check to 65536 bytes, and the adversarial differential — plus the directed battery probe above.

## Fold mechanics

- All fold targets are inside the dispatched scope surface (verify.c3/main.c3/README + battery + emitter-side helper). Write your FOLD_SCOPE artifact before any edit; any OUT row stops the fold.
- Reply with the fold report (FOLD_SCOPE above ACTIONS_GIT_REF, FOLD_SCOPE_RESULT: all-in, new head, suite counts). I will run a TARGETED check (panic repro gone, new gates green, suites) — no re-panel; design and blast radius unchanged.
- Merge remains a separate operator/orchestrator gate; after the fold check I file the merge-gate recommendation.

ACTIONS_GIT_REF: none — no edits made
FINAL_GIT_STATUS_SHORT:
 M .claude/scheduled_tasks.lock
 M docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/INDEX.md
?? docs/sprints/active/2026-08-28-uttt-weak-solve/.relays/s2/s2-theory/IMPL-pair-implementer-20260830-024849.md
(daemon lock/INDEX and the implementation report awaiting orchestrator batch; taken immediately before this filing)
