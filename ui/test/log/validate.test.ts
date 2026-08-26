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
  })

  it('marks a complete prefix without a game end as truncated', () => {
    const record = parseGameLog(fixtureText('missing-game-end.jsonl'))
    expect(record.end).toBeUndefined()
    expect(record.truncated).toBe(true)
  })

  it('rejects a wrong log version on the first event', () => {
    const text = fixtureText('success-macro-win.jsonl').replace('"log_version":1', '"log_version":2')
    expect(() => parseGameLog(text)).toThrow(/version/i)
  })

  it('accepts unknown keys at every schema level', () => {
    const text = fixtureText('success-macro-win.jsonl').replace(
      '"event":"game_start"',
      '"event":"game_start","future_start":true',
    )
    expect(parseGameLog(text).start.event).toBe('game_start')
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
