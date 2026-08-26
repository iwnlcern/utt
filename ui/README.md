# Poorman UTTT replay UI

This Vite/React application opens referee JSONL game logs and renders replay and
analysis views entirely from logged facts. It does not execute game rules and
does not provide live play or a browser bridge.

## Requirements and local run

- Node.js 22 (see `.nvmrc`)
- npm, using the committed `package-lock.json`

From the repository root:

```sh
cd ui
npm ci
npm run dev
```

Open the URL printed by Vite. Use the file picker, drop a `.jsonl` file, open a
recent local log, or choose the bundled sample. Loaded logs stay in the browser;
the UI makes no network request for replay data.

Create and inspect a production build with:

```sh
cd ui
npm run build
npm run preview
```

## Tests and checks

Run the complete UI verification from the repository root:

```sh
cd ui
npm test
npm run lint
npm run build
```

Run only the composed fixture acceptance sweep with:

```sh
cd ui
npm test -- test/acceptance.test.ts
```

The acceptance sweep sends every viewable manifest fixture through
`parseGameLog` → `deriveReplayModel` → `GameView`. The intentionally malformed
interior fixture is successful only when parsing rejects it with the specified
structured `LogError`; it must never reach derivation or rendering.

## Fixture provenance

[`fixtures/manifest.ts`](fixtures/manifest.ts) is the fixture census and
provenance record. Every entry names its harness-owned legal base or referee
generation path, its UI-only derivation when applicable, and the exact
owner-side validation command. The sources of record are:

- the normative log schemas in [`../docs/protocol/schema/`](../docs/protocol/schema/);
- the replay protocol in
  [`../docs/protocol/poorman-uttt-protocol-v1.md`](../docs/protocol/poorman-uttt-protocol-v1.md);
- referee conformance logs under `../referee/tests/fixtures/`; and
- referee-generated games produced through
  `poorman_referee.referee.GameConfig` / `play_game`.

For a complete fixture, run its manifest `validation` command from the
repository root. The command validates every event against the matching Draft
2020-12 owner schema, then calls the harness-owned `read_log` and
`replay_frames` path. The six deliberately partial or malformed UI fixtures
instead cite and validate their complete legal base before applying their named
UI-only truncation or corruption.

The current corpus is an interim UI corpus. When harness conformance fixtures
are published as the adopted corpus, replace harness-derived fixtures wholesale.
Regenerate `ghost-divergence`, `no-info`, and `malformed-info` only as UI overlays
on an adopted legal transcript; do not hand-repair game events or add UI-side
rules validation.

## Conditional-PV artifact pin

Conditional move coordinates are ruled to use canonical X/O coordinates, but
rendering remains disabled until the harness publishes the exact canonical
pre-state-hash artifact (input bytes, algorithm, and encoding). The production
constant in [`src/analysis/extract.ts`](src/analysis/extract.ts) must remain:

```ts
export const PV_PIN = { pinned: false, source: null }
```

After that harness artifact is approved and lands, flip the pin in one cited
change only:

1. Verify the adopted protocol/conformance artifact defines all hash bytes,
   algorithm, and encoding and binds analysis to that state key.
2. Change only `PV_PIN.pinned` to `true` and set `PV_PIN.source` to the exact
   repository-relative artifact path plus its approved version or digest. Never
   use an uncited prose assertion or infer a hash convention in the UI.
3. Run the focused pin/association checks and then the complete checks:

   ```sh
   cd ui
   npm test -- test/analysis/extract.test.ts test/components/board.test.tsx test/app/game-view.test.tsx test/acceptance.test.ts
   npm test
   npm run lint
   npm run build
   ```

No pin is applied in this implementation; conditional PVs continue to show
`unavailable — awaiting harness artifact pin`.

## Bundled-sample regeneration follow-up

`public/sample-game.jsonl` currently matches `fixtures/success-macro-win.jsonl`.
After the real referee and adopted harness conformance corpus are final:

1. Produce a real referee-emitted engine-vs-engine log through the documented
   `poorman_referee` CLI; do not synthesize or edit its game events.
2. Validate the candidate with the same owner schema + `read_log` +
   `replay_frames` procedure recorded in the fixture manifest.
3. Add or adopt the validated transcript in `fixtures/manifest.ts`, then copy
   those exact bytes to `public/sample-game.jsonl`.
4. Prove byte identity and rerun the full UI checks:

   ```sh
   cmp ui/public/sample-game.jsonl ui/fixtures/<adopted-sample>.jsonl
   cd ui
   npm test
   npm run lint
   npm run build
   ```

This follow-up stays open until the harness-owned real-log artifact exists.
