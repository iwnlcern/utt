export type Share =
  | { kind: 'ok'; value: number }
  | { kind: 'na'; why: 'both budgets exhausted' }

export function share(units: number, combined: number): Share {
  if (combined === 0) return { kind: 'na', why: 'both budgets exhausted' }
  return { kind: 'ok', value: units / combined }
}

export function formatPercent(result: Share): string {
  if (result.kind === 'na') return `n/a — ${result.why}`
  return formatPercentBasisPoints(percentBasisPoints(result) ?? 0)
}

// Percentages are displayed to two decimal places, so comparison consumers use
// the same integer hundredth-of-a-percent representation rather than raw floats.
export function percentBasisPoints(result: Share): number | undefined {
  if (result.kind === 'na') return undefined
  return Math.round(result.value * 10_000)
}

export function formatPercentBasisPoints(basisPoints: number): string {
  const sign = basisPoints < 0 ? '-' : ''
  return `${sign}${(Math.abs(basisPoints) / 100).toFixed(2)}%`
}

// Share comparisons preserve meaningful sub-basis-point differences. The
// tolerance covers only a few IEEE-754 rounding units at the values compared.
export function compareRawShares(left: number, right: number): -1 | 0 | 1 {
  const tolerance = Number.EPSILON * Math.max(1, Math.abs(left), Math.abs(right)) * 4
  const difference = left - right
  if (Math.abs(difference) <= tolerance) return 0
  return difference < 0 ? -1 : 1
}

// Analysis shares are carried as numbers, so unit-side display uses the raw
// share and a deterministic nearest-integer conversion (Math.round), never the
// separately rounded percentage basis points.
export function roundShareToUnits(rawShare: number, combined: number): number {
  return Math.round(rawShare * combined)
}

export function formatUnits(units: number): string {
  const sign = units < 0 ? '-' : ''
  const digits = Math.abs(units).toString()
  return `${sign}${digits.replace(/\B(?=(\d{3})+(?!\d))/g, '\u202f')}`
}
