import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

const tokensCss = readFileSync(resolve(import.meta.dirname, '../../src/styles/tokens.css'), 'utf8')
const indexCss = readFileSync(resolve(import.meta.dirname, '../../src/index.css'), 'utf8')

function declarations(source: string, pattern: RegExp): string {
  const match = pattern.exec(source)
  if (match?.[1] === undefined) throw new Error(`missing token block matching ${pattern}`)
  return match[1]
}

function token(block: string, name: string): string {
  const match = new RegExp(`--${name}:\\s*([^;]+);`).exec(block)
  if (match?.[1] === undefined) throw new Error(`missing --${name} token`)
  return match[1].trim()
}

function luminance(hex: string): number {
  const channels = [1, 3, 5].map((offset) => Number.parseInt(hex.slice(offset, offset + 2), 16) / 255)
  const linear = channels.map((channel) => channel <= 0.04045
    ? channel / 12.92
    : ((channel + 0.055) / 1.055) ** 2.4)
  return 0.2126 * linear[0] + 0.7152 * linear[1] + 0.0722 * linear[2]
}

function contrast(first: string, second: string): number {
  const lighter = Math.max(luminance(first), luminance(second))
  const darker = Math.min(luminance(first), luminance(second))
  return (lighter + 0.05) / (darker + 0.05)
}

describe('focus token', () => {
  const light = declarations(tokensCss, /:root\s*{([\s\S]*?)}/)
  const dark = declarations(tokensCss, /@media \(prefers-color-scheme: dark\)\s*{\s*:root\s*{([\s\S]*?)}/)

  it.each([
    ['light', light],
    ['dark', dark],
  ])('is opaque and at least 3:1 against every adjacent %s surface', (_, block) => {
    const focus = token(block, 'focus')
    expect(focus).toMatch(/^#[0-9a-f]{6}$/i)

    for (const surface of ['page', 'surface', 'surface-subtle']) {
      expect(contrast(focus, token(block, surface)), `${surface} contrast`).toBeGreaterThanOrEqual(3)
    }
  })

  it('keeps the focus-visible outline at three CSS pixels', () => {
    expect(indexCss).toMatch(/:focus-visible[\s\S]*?outline:\s*3px solid var\(--focus\);/)
  })
})
