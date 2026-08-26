# Task 6 report — resolved replay fold and positions

## Scope

Implemented the Task 6 resolved-only replay fold on `ui-c1-impl` from base
`602509c5571cc70691464ba54d9ab8c84adf6c2a`.

## RED

Before production files existed:

```text
$ cd ui && npm test -- test/replay/derive.test.ts
FAIL test/replay/derive.test.ts
Failed to resolve import "../../src/replay/derive"
Test Files 1 failed; Tests no tests
```

The new test file covered the synthetic non-sequential wire-ply regression
first, fixture-backed resolved position provenance, cumulative logged closure
persistence, and terminal forwarding.

## GREEN

```text
$ cd ui && npm test -- test/replay/derive.test.ts
Test Files 1 passed; Tests 4 passed

$ cd ui && npm test
Test Files 6 passed; Tests 34 passed

$ cd ui && npm run build
tsc -b && vite build
built in 110ms

$ cd ui && npm run lint
eslint .
exit 0

$ git diff --check
exit 0
```

## Files

- `ui/src/replay/model.ts` — typed replay model and future totality seams.
- `ui/src/replay/derive.ts` — sequential resolved-auction fold.
- `ui/test/replay/derive.test.ts` — synthetic and fixture-backed regression tests.

## Self-review

- `position_0` reads only start budgets and uses the fixed empty-board,
  forced-board, tie-owner, and closure-map constants.
- Each resolved post-position takes board, budgets, and forced route directly
  from its event; no board rules run in the UI fold.
- Tie ownership is the logged winner's opponent, and `closed` is only the
  persistent fold of logged closure entries.
- Non-sequential plies throw `LogError` with the event index, derived JSONL
  line coordinate, expected ply, and received ply.
- Terminal and parsed truncation state are forwarded unchanged.

## Concern / forward seam

Task 6 deliberately rejects non-resolved auctions after validating their wire
ply. Task 7 owns the total fold: recovery attachment, voided/aborted outcomes,
trailing recoveries, and zero-auction setup terminals. The model already
carries the fields needed for that extension.

## Fix Round 1 — discriminated `AuctionStep`

Changed `AuctionStep` into a union with a shared base:

- `outcome: 'resolved'` requires both `resolution` and `post`.
- `outcome: 'voided' | 'aborted_recovery_fault'` forbids both fields with
  optional `never` fields, preserving the Task 7 extension seam.

The focused test derives the real first fixture step, narrows on
`outcome === 'resolved'`, then reads its winner and post-board without optional
guards. That use failed typechecking before the model change and passes now.

### RED

```text
$ cd ui && ./node_modules/.bin/tsc --ignoreConfig --noEmit --target es2023 --lib es2023,dom --module esnext --moduleResolution bundler --allowImportingTsExtensions --verbatimModuleSyntax --moduleDetection force --skipLibCheck --types vite/client,node --jsx react-jsx test/replay/derive.test.ts
test/replay/derive.test.ts(36,12): error TS18048: 'step.resolution' is possibly 'undefined'.
test/replay/derive.test.ts(37,12): error TS18048: 'step.post' is possibly 'undefined'.
```

### GREEN

```text
$ cd ui && npm test -- test/replay/derive.test.ts
Test Files 1 passed; Tests 5 passed

$ cd ui && ./node_modules/.bin/tsc --ignoreConfig --noEmit --target es2023 --lib es2023,dom --module esnext --moduleResolution bundler --allowImportingTsExtensions --verbatimModuleSyntax --moduleDetection force --skipLibCheck --types vite/client,node --jsx react-jsx test/replay/derive.test.ts
exit 0

$ cd ui && npm test
Test Files 6 passed; Tests 35 passed

$ cd ui && npm run build
tsc -b && vite build
built in 133ms

$ cd ui && npm run lint
eslint .
exit 0

$ git diff --check
exit 0
```

Deferred by instruction: the fault-single tie-owner assertion and explicit
truncated-forwarding regression were not changed in this fix round.
