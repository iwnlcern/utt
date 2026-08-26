import { readFileSync, readdirSync } from 'node:fs'
import { resolve } from 'node:path'

import fc from 'fast-check'
import { describe, expect, it } from 'vitest'

import { compareRawShares, formatPercent, formatPercentBasisPoints, formatUnits, percentBasisPoints, roundShareToUnits, share } from '../../src/format/money'

describe('money formatting', () => {
  it('represents a zero combined budget as typed not-applicable', () => {
    const result = share(0, 0)

    expect(result).toEqual({ kind: 'na', why: 'both budgets exhausted' })
    expect(formatPercent(result)).toBe('n/a — both budgets exhausted')
  })

  it('formats a share as a two-decimal percentage', () => {
    const result = share(634_100_000, 1_000_000_000)

    expect(result).toEqual({ kind: 'ok', value: 0.6341 })
    expect(formatPercent(result)).toBe('63.41%')
  })

  it('uses rounded display basis points for float-resilient percentage comparisons', () => {
    expect(percentBasisPoints(share(3, 10))).toBe(3000)
    expect(percentBasisPoints({ kind: 'ok', value: 0.2 + 0.1 })).toBe(3000)
    expect(formatPercentBasisPoints(0)).toBe('0.00%')
    expect(formatPercentBasisPoints(-250)).toBe('-2.50%')
  })

  it('compares raw shares with only machine-scale tolerance', () => {
    expect(compareRawShares(3 / 10, 0.2 + 0.1)).toBe(0)
    expect(compareRawShares(3 / 10, 0.30004)).toBe(-1)
  })

  it('rounds raw shares to integer unit values without using display basis points', () => {
    expect(roundShareToUnits(1 / 3, 1_000_000_000)).toBe(333_333_333)
    expect(roundShareToUnits(1 / 3, 3)).toBe(1)
  })

  it('groups units with narrow no-break spaces', () => {
    expect(formatUnits(634_100_000)).toBe('634\u202f100\u202f000')
  })

  it('rounds every valid integer share to a finite percentage within 0.005pp', () => {
    const budgetPair = fc.integer({ min: 1, max: 2_000_000_000 }).chain((combined) =>
      fc.integer({ min: 0, max: combined }).map((units) => [units, combined] as const),
    )

    fc.assert(
      fc.property(budgetPair, ([units, combined]) => {
        const result = share(units, combined)
        if (result.kind !== 'ok') throw new Error('positive combined budget must be applicable')

        const rendered = formatPercent(result)
        const percent = Number.parseFloat(rendered)
        expect(Number.isFinite(percent)).toBe(true)
        expect(Math.abs(percent - (units / combined) * 100)).toBeLessThanOrEqual(0.005 + 1e-9)
      }),
    )
  })

  it('guards source budget variables against equality with float literals', () => {
    // The guard targets budget-like identifiers/properties on either side of ==/===
    // and a decimal literal beginning with 0.; integer zero comparisons remain valid.
    const budgetVariable = String.raw`(?:\b(?:units|combined|budget\w*|budgets?)\b(?:\s*\.\s*[A-Za-z_$][\w$]*)?)`
    const budgetFloatEquality = new RegExp(
      String.raw`${budgetVariable}\s*(?:===|==)\s*0\.\d+|0\.\d+\s*(?:===|==)\s*${budgetVariable}`,
    )

    expect('if (combined === 0.5) return;').toMatch(budgetFloatEquality)
    expect('if (budget.X == 0.25) return;').toMatch(budgetFloatEquality)
    expect('if (count === 0.5) return;').not.toMatch(budgetFloatEquality)

    const sourceRoot = resolve(import.meta.dirname, '../../src')
    const sourceFiles = (directory: string): string[] =>
      readdirSync(directory, { withFileTypes: true }).flatMap((entry) => {
        const path = resolve(directory, entry.name)
        return entry.isDirectory() ? sourceFiles(path) : path.endsWith('.ts') || path.endsWith('.tsx') ? [path] : []
      })
    const violations = sourceFiles(sourceRoot).flatMap((path) => {
      const text = readFileSync(path, 'utf8')
      return budgetFloatEquality.test(text) ? [path] : []
    })

    expect(violations).toEqual([])
  })
})
