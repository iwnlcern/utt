import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

describe('bundled sample game', () => {
  it('is byte-for-byte the proven success-macro-win fixture', () => {
    const fixture = readFileSync(resolve(import.meta.dirname, '../../fixtures/success-macro-win.jsonl'))
    const sample = readFileSync(resolve(import.meta.dirname, '../../public/sample-game.jsonl'))

    expect(sample.equals(fixture)).toBe(true)
  })
})
