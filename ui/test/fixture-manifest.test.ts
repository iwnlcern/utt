import { readdir, readFile } from 'node:fs/promises'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { fixtureManifest } from '../fixtures/manifest'

const fixturesDirectory = resolve(process.cwd(), 'fixtures')

const expectedFixtureNames = [
  'both-zero',
  'chip-count',
  'double-fault-retry',
  'exact-tie-draw',
  'fault-single',
  'ghost-divergence',
  'hello-fault',
  'malformed-info',
  'malformed-interior',
  'missing-game-end',
  'no-info',
  'post-auction-recovery-eof',
  'recovery-fault-abort',
  'recovery-fault-post-resolve',
  'success-macro-win',
  'trailing-recovery',
  'trailing-recovery-xo',
  'truncated-line',
  'void-triple-double-fault',
]

describe('schema-v1 fixture manifest', () => {
  it('enumerates every JSONL fixture and leaves no orphan fixture file', async () => {
    const fixtureFiles = (await readdir(fixturesDirectory))
      .filter((name) => name.endsWith('.jsonl'))
      .sort()

    expect(fixtureManifest.map((fixture) => fixture.name).sort()).toEqual(
      expectedFixtureNames,
    )
    expect(fixtureManifest.map((fixture) => fixture.path).sort()).toEqual(
      fixtureFiles,
    )
  })

  it('raw-splits complete JSONL lines while inventorying the intentional malformed cases', async () => {
    for (const fixture of fixtureManifest) {
      const text = await readFile(
        resolve(fixturesDirectory, fixture.path),
        'utf8',
      )
      const completeLines = text.split('\n').slice(0, -1)
      const malformedLines: number[] = []

      for (const [index, line] of completeLines.entries()) {
        try {
          expect(JSON.parse(line)).toBeTypeOf('object')
        } catch {
          malformedLines.push(index + 1)
        }
      }

      expect(malformedLines).toEqual(
        fixture.expected.rawJsonl === 'malformed-interior' ? [2] : [],
      )
    }
  })
})
