import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { PV_PIN, PV_UNAVAILABLE_MESSAGE, extractAnalysis } from '../../src/analysis/extract'
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
    expect(PV_UNAVAILABLE_MESSAGE).toBe('unavailable — awaiting harness artifact pin')
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

  it('suppresses earlier analysis when the final attempt carries no info', () => {
    const events = fixtureText('success-macro-win.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const auction = events.find((event) => event.event === 'auction')
    const retry = structuredClone(auction.attempts[0])
    retry.attempt = 2
    retry.request_id = `${retry.request_id}-retry`
    delete retry.turns.X.info
    auction.attempts.push(retry)
    const step = deriveReplayModel(parseGameLog(`${events.map(JSON.stringify).join('\n')}\n`)).auctions[0]
    if (step === undefined) throw new Error('success fixture must contain an auction')

    expect(extractAnalysis(step).X).toEqual({ kind: 'unavailable', why: 'no info in log' })
  })

  it('marks every seat unavailable when a log has no info objects', () => {
    expect(extractAnalysis(firstStep('no-info.jsonl'))).toEqual({
      X: { kind: 'unavailable', why: 'no info in log' },
      O: { kind: 'unavailable', why: 'no info in log' },
    })
  })

  it('keeps malformed advisory info out of the otherwise valid replay', () => {
    expect(extractAnalysis(firstStep('malformed-info.jsonl'))).toEqual({
      X: { kind: 'unavailable', why: 'malformed info in log', loggedInfo: true },
      O: { kind: 'unavailable', why: 'no info in log' },
    })
  })

  it.each([
    ['missing quality', undefined],
    ['exact quality', 'exact'],
    ['estimate quality', 'estimate'],
  ] as const)('degrades logged lo and hi when bound quality is %s', (_name, quality) => {
    const entry = extractAnalysis(firstStepWithInfo({
      t: 0.5,
      ...(quality === undefined ? {} : { quality }),
      lo: 0.25,
      hi: 0.75,
    })).X

    expect(entry).toMatchObject({
      kind: 'ok',
      t: 0.5,
      ...(quality === undefined ? {} : { quality }),
      degraded: ['lo', 'hi'],
    })
    expect(entry).not.toHaveProperty('lo')
    expect(entry).not.toHaveProperty('hi')
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

  it.each([
    ['negative', -0.001],
    ['greater than one', 1.001],
  ])('degrades a %s threshold share while retaining other usable fields', (_name, t) => {
    expect(extractAnalysis(firstStepWithInfo({ t, critical_bid: 4_000_000_000 })).X).toEqual({
      kind: 'ok',
      criticalBid: 4_000_000_000,
      degraded: ['t'],
    })
  })

  it.each([
    ['negative lower endpoint', -0.001, 0.75],
    ['upper endpoint greater than one', 0.25, 1.001],
  ])('degrades the whole bound family for a %s', (_name, lo, hi) => {
    const entry = extractAnalysis(firstStepWithInfo({
      t: 0.5,
      quality: 'bound',
      lo,
      hi,
    })).X

    expect(entry).toMatchObject({ kind: 'ok', t: 0.5 })
    if (entry?.kind !== 'ok') throw new Error('valid t must keep the entry usable')
    expect(entry).not.toHaveProperty('quality')
    expect(entry).not.toHaveProperty('lo')
    expect(entry).not.toHaveProperty('hi')
    expect(entry.degraded).toEqual(expect.arrayContaining(['quality', 'lo', 'hi']))
  })

  it('degrades a negative critical bid without imposing an upper bound', () => {
    expect(extractAnalysis(firstStepWithInfo({ t: 0.5, critical_bid: -1 })).X).toEqual({
      kind: 'ok',
      t: 0.5,
      degraded: ['criticalBid'],
    })
    expect(extractAnalysis(firstStepWithInfo({ critical_bid: 4_000_000_000 })).X).toEqual({
      kind: 'ok',
      criticalBid: 4_000_000_000,
      degraded: [],
    })
  })

  it.each([
    ['both endpoints', { t: 0.375, quality: 'bound' }],
    ['the lower endpoint', { t: 0.375, quality: 'bound', hi: 0.75 }],
    ['the upper endpoint', { t: 0.375, quality: 'bound', lo: 0.5 }],
  ])('degrades a bound quality missing %s', (_missing, info) => {
    expect(extractAnalysis(firstStepWithInfo(info)).X).toEqual({
      kind: 'ok',
      t: 0.375,
      degraded: ['quality', 'lo', 'hi'],
    })
  })

  it('does not report a bare bound quality as usable analysis', () => {
    expect(extractAnalysis(firstStepWithInfo({ quality: 'bound' })).X).toEqual({
      kind: 'unavailable',
      why: 'malformed info in log',
      loggedInfo: true,
    })
  })

  it('degrades an inverted bound interval rather than reporting a contradictory bound', () => {
    expect(extractAnalysis(firstStepWithInfo({
      t: 0.375,
      quality: 'bound',
      lo: 0.75,
      hi: 0.5,
    })).X).toEqual({
      kind: 'ok',
      t: 0.375,
      degraded: ['quality', 'lo', 'hi'],
    })
  })
})
