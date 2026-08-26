import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { buildTPSeries } from '../../src/analysis/series'
import { deriveReplayModel } from '../../src/replay/derive'
import { parseGameLog } from '../../src/log/validate'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

const modelFrom = (name: string) => deriveReplayModel(parseGameLog(fixtureText(name)))

describe('buildTPSeries', () => {
  it('aligns threshold and actual-share values to replay cursor positions with gaps for unavailable analysis', () => {
    const model = modelFrom('success-macro-win.jsonl')
    const series = buildTPSeries(model, 'X')

    expect(series.t).toHaveLength(model.positions.length)
    expect(series.p).toHaveLength(model.positions.length)
    expect(series.t.slice(0, 3)).toEqual([0.625, null, null])
    expect(series.p.slice(0, 3)).toEqual([0.5, 0.5, 0.5])
    expect(series.t.at(-1)).toBeNull()
  })

  it('turns an exhausted combined budget into a p-series gap rather than inventing a share', () => {
    const model = modelFrom('both-zero.jsonl')
    const series = buildTPSeries(model, 'X')

    expect(series.p).toHaveLength(model.positions.length)
    expect(series.p).toContain(null)
    expect(series.p.at(-1)).toBeNull()
  })
})
