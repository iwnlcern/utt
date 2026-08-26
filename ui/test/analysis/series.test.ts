import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { buildTPSeries } from '../../src/analysis/series'
import { deriveReplayModel } from '../../src/replay/derive'
import { parseGameLog } from '../../src/log/validate'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

const modelFrom = (name: string) => deriveReplayModel(parseGameLog(fixtureText(name)))

const modelWithLaterVoidInfo = () => {
  const model = modelFrom('success-macro-win.jsonl')
  const pre = model.positions.at(-1)
  const finalAttempt = structuredClone(model.auctions.at(-1)?.attempts.at(-1))
  if (pre === undefined || finalAttempt === undefined) throw new Error('success fixture must provide a final position and attempt')
  finalAttempt.turns.X.info = { t: 0.875 }

  return {
    ...model,
    auctions: [...model.auctions, {
      ply: model.auctions.length,
      pre,
      attempts: [finalAttempt],
      recoveries: [],
      outcome: 'voided' as const,
    }],
  }
}

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

  it('places analysis from a non-advancing terminal auction at its later pre-position cursor', () => {
    const model = modelWithLaterVoidInfo()
    const series = buildTPSeries(model, 'X')

    expect(model.auctions.at(-1)?.outcome).toBe('voided')
    expect(model.positions.length).toBeGreaterThan(1)
    expect(series.t.at(-1)).toBe(0.875)
    expect(series.t.at(-2)).toBeNull()
  })
})
