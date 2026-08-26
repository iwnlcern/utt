import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { fixtureManifest } from '../../fixtures/manifest'
import { parseGameLog, LogError } from '../../src/log/validate'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

describe('parseGameLog', () => {
  it('assembles every complete fixture into a non-truncated record', () => {
    for (const fixture of fixtureManifest) {
      if (fixture.expected.rawJsonl !== 'complete') continue
      const record = parseGameLog(fixtureText(fixture.path))
      expect(record.start.event).toBe('game_start')
      expect(record.end?.reason).toBe(fixture.expected.terminal?.reason)
      expect(record.truncated).toBe(false)
    }
  })

  it('reports a newline-terminated malformed interior line and event index', () => {
    expect(() => parseGameLog(fixtureText('malformed-interior.jsonl'))).toThrow(
      expect.objectContaining({ line: 2, event_index: 1 }),
    )
  })

  it('keeps the valid prefix when the final unterminated bytes are discarded', () => {
    const record = parseGameLog(fixtureText('truncated-line.jsonl'))
    expect(record.end?.event).toBe('game_end')
    expect(record.truncated).toBe(true)
    expect(record.truncation).toBe('discarded_final_line')
  })

  it.each([
    ['an empty input', ''],
    ['a single discarded malformed tail', 'not-json'],
  ])('uses line one for %s without a complete event', (_name, text) => {
    expect(() => parseGameLog(text)).toThrow(expect.objectContaining({ line: 1, event_index: 0 }))
  })

  it('marks a complete prefix without a game end as truncated', () => {
    const record = parseGameLog(fixtureText('missing-game-end.jsonl'))
    expect(record.end).toBeUndefined()
    expect(record.truncated).toBe(true)
    expect(record.truncation).toBe('missing_game_end')
  })

  it.each(['post-auction-recovery-eof.jsonl', 'trailing-recovery.jsonl', 'trailing-recovery-xo.jsonl'])(
    'marks the %s complete prefix as truncated',
    (fixture) => {
      const record = parseGameLog(fixtureText(fixture))
      expect(record.end).toBeUndefined()
      expect(record.truncated).toBe(true)
      expect(record.truncation).toBe('missing_game_end')
    },
  )

  it('rejects a wrong log version on the first event', () => {
    const text = fixtureText('success-macro-win.jsonl').replace('"log_version":1', '"log_version":2')
    expect(() => parseGameLog(text)).toThrow(/version/i)
  })

  it('accepts unknown keys throughout nested schema records', () => {
    const complete = fixtureText('success-macro-win.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const start = complete[0]
    const auction = complete.find((event) => event.event === 'auction')
    const closureAuction = complete.find((event) => event.event === 'auction' && event.resolution.closures.length > 0)
    const end = complete.at(-1)
    start.future = true
    start.engines.X.future = true
    start.hellos.X.future = true
    start.time_control.future = true
    start.budgets.future = true
    auction.future = true
    auction.attempts[0].future = true
    auction.attempts[0].turns.future = true
    auction.attempts[0].turns.X.future = true
    auction.budgets_after.future = true
    auction.resolution.future = true
    closureAuction.resolution.closures[0].future = true
    end.future = true
    end.budgets.future = true
    end.delivery.future = true
    end.stderr.future = true
    end.stderr.X.future = true

    const recovery = fixtureText('recovery-fault-post-resolve.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const faultRecovery = recovery.find((event) => event.event === 'recovery')
    faultRecovery.future = true
    faultRecovery.hello.future = true
    faultRecovery.hello.raw.future = true

    expect(parseGameLog(`${complete.map(JSON.stringify).join('\n')}\n`).end?.event).toBe('game_end')
    expect(parseGameLog(`${recovery.map(JSON.stringify).join('\n')}\n`).end?.event).toBe('game_end')
  })

  it.each([
    ['non-integer elapsed time', '"elapsed_ms":0', '"elapsed_ms":0.5'],
    ['out-of-range move cell', '"move":[4,3]', '"move":[9,3]'],
    ['invalid board string', '"post_board":["........."', '"post_board":["........Z"'],
    ['missing required X seat', '"budgets":{"O":1000000000,"X":1000000000}', '"budgets":{"O":1000000000}'],
    ['fault hello without raw capture', '"validation":"ok","version":"1"', '"validation":"timeout","version":"1"'],
    ['resolved auction without resolution', '"resolution":{"closures":[]', '"future_resolution":{"closures":[]'],
  ])('rejects %s', (_name, from, to) => {
    const text = fixtureText('success-macro-win.jsonl').replace(from, to)
    expect(() => parseGameLog(text)).toThrow(LogError)
  })
})
