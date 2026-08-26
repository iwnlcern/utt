import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { PV_PIN, extractAnalysis } from '../../src/analysis/extract'
import { deriveReplayModel } from '../../src/replay/derive'
import { parseGameLog } from '../../src/log/validate'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

const firstStep = (name: string) => {
  const step = deriveReplayModel(parseGameLog(fixtureText(name))).auctions[0]
  if (step === undefined) throw new Error(`${name} must contain an auction`)
  return step
}

const firstStepWithInfo = (info: unknown) => {
  const events = fixtureText('success-macro-win.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
  const auction = events.find((event) => event.event === 'auction')
  auction.attempts[0].turns.X.info = info
  const step = deriveReplayModel(parseGameLog(`${events.map(JSON.stringify).join('\n')}\n`)).auctions[0]
  if (step === undefined) throw new Error('success fixture must contain an auction')
  return step
}

describe('extractAnalysis', () => {
  it('extracts the full bound-quality metadata from the final attempt without remapping canonical PV cells', () => {
    const analysis = extractAnalysis(firstStep('success-macro-win.jsonl'))

    expect(analysis.X).toEqual({
      kind: 'ok',
      t: 0.625,
      criticalBid: 100_000_000,
      pvIfWin: [0, 0],
      pvIfLose: [0, 1],
      quality: 'bound',
      lo: 0.5,
      hi: 0.75,
      depth: 12,
      complete: false,
      degraded: [],
    })
    expect(analysis.O).toEqual({ kind: 'unavailable', why: 'no info in log' })
    expect(PV_PIN).toEqual({ pinned: false, source: null })
  })

  it('uses only the last attempt for a seat', () => {
    const events = fixtureText('success-macro-win.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const auction = events.find((event) => event.event === 'auction')
    const retry = structuredClone(auction.attempts[0])
    retry.attempt = 2
    retry.request_id = `${retry.request_id}-retry`
    retry.turns.X.info = { t: 0.25 }
    auction.attempts.push(retry)
    const step = deriveReplayModel(parseGameLog(`${events.map(JSON.stringify).join('\n')}\n`)).auctions[0]
    if (step === undefined) throw new Error('success fixture must contain an auction')

    expect(extractAnalysis(step).X).toEqual({ kind: 'ok', t: 0.25, degraded: [] })
  })

  it('marks every seat unavailable when a log has no info objects', () => {
    expect(extractAnalysis(firstStep('no-info.jsonl'))).toEqual({
      X: { kind: 'unavailable', why: 'no info in log' },
      O: { kind: 'unavailable', why: 'no info in log' },
    })
  })

  it('keeps malformed advisory info out of the otherwise valid replay', () => {
    expect(extractAnalysis(firstStep('malformed-info.jsonl'))).toEqual({
      X: { kind: 'unavailable', why: 'malformed info in log' },
      O: { kind: 'unavailable', why: 'no info in log' },
    })
  })

  it('retains well-typed fields while naming invalid fields as degraded', () => {
    const analysis = extractAnalysis(firstStepWithInfo({
      t: 0.375,
      critical_bid: 'not fixed-point units',
      pv_if_win: [0, 8],
      complete: 'unknown',
    }))

    expect(analysis.X).toEqual({
      kind: 'ok',
      t: 0.375,
      pvIfWin: [0, 8],
      degraded: ['criticalBid', 'complete'],
    })
  })
})
