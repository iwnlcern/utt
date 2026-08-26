# Harness c3 (loader-repair: honor consumed_by) Implementation Plan — plan-lite

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans (single bounded task).

PLAN_LOCK_ID: PL-harness-c3-20260826
DESIGN_LOCK_ID: DD-harness-c1-20260825 @ sha256 c935c29c0ee603df1750c49c40dabcd5432f70105070b60552728f1e6dc24a6e
Record of what to build: s1-closure-ruling-1 §3 (bounded small-tier consumer repair) + theory's binding schema ruling §1 (consumed_by is NORMATIVE for every consumer).

**Goal:** The harness theory-fixture integration honors `consumed_by`: fixtures whose `consumed_by` list excludes `"harness"` are skipped by the harness suite.

**Scope (EXACTLY one file, per the ruling):** `referee/tests/test_fixture_loader.py`. A second file is a deviation relay, never an absorption. The production loader (`fixture_loader.py`) already conforms on every other shape and is NOT touched.

**Cross-bundle collision analysis (vs harness-c2):** harness-c2's only touch of this file is its Task 4 YES branch (theory fixtures landed on main), which is unreachable while this repair is pending — theory's publication merge gates on this repair landing (s1-closure-ruling-1 §2), so c2's Task 4 resolves NO this window. The cycles are file-disjoint; c3 may plan/review/dispatch independently.

## Task 1 (the whole cycle)

- [ ] **Step 1: Failing tests** (added in the same file):
```python
def test_consumed_by_filter_skips_non_harness_fixtures():
    # inline fixtures: consumed_by ["engine"] -> excluded; ["harness"] and
    # ["engine","harness"] -> included by the harness suite's selection helper
def test_integration_selection_uses_consumed_by():
    # the integration test's fixture-selection path filters on "harness" in
    # consumed_by (unit-proved against the same helper the integration run uses)
```
- [ ] **Step 2: run, FAIL.** — `uv run --project referee pytest referee/tests/test_fixture_loader.py -q`
- [ ] **Step 3: Implement in the same file** — the integration test's iteration filters to fixtures with `"harness"` in `consumed_by`; the development-only skip marker semantics from c1 are unchanged (still skips when `theory/fixtures/` is absent); no production-file edit.
- [ ] **Step 4: run, PASS (full suite green: `uv run --project referee pytest referee/tests -q`).**
- [ ] **Step 5: Commit** on branch `harness-c3-impl`: `"harness: loader integration honors consumed_by"`.

## Acceptance

1. Suite green; the only permitted skip remains the theory-fixtures-absent marker.
2. `git diff --name-only` over the branch = exactly `referee/tests/test_fixture_loader.py`.
3. Exit: branch + PR + report; merge under a separate addressed grant. Theory's publication merge may then verify this repair landed.

## Operator-judgment items

None — executes an addressed orchestrator ruling verbatim.
