export type Share =
  | { kind: 'ok'; value: number }
  | { kind: 'na'; why: 'both budgets exhausted' }

export function share(units: number, combined: number): Share {
  if (combined === 0) return { kind: 'na', why: 'both budgets exhausted' }
  return { kind: 'ok', value: units / combined }
}

export function formatPercent(result: Share): string {
  if (result.kind === 'na') return `n/a — ${result.why}`
  return `${(result.value * 100).toFixed(2)}%`
}

export function formatUnits(units: number): string {
  const sign = units < 0 ? '-' : ''
  const digits = Math.abs(units).toString()
  return `${sign}${digits.replace(/\B(?=(\d{3})+(?!\d))/g, '\u202f')}`
}
