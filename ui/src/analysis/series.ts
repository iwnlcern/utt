import { share } from '../format/money'
import type { Mark } from '../log/types'
import type { ReplayModel } from '../replay/model'
import { extractAnalysis } from './extract'

export interface TPSeries {
  t: Array<number | null>
  p: Array<number | null>
}

export function buildTPSeries(model: ReplayModel, seat: Mark): TPSeries {
  const t = Array<number | null>(model.positions.length).fill(null)
  let cursor = 0

  for (const step of model.auctions) {
    const entry = extractAnalysis(step)[seat]
    if (entry?.kind === 'ok' && entry.t !== undefined) t[cursor] = entry.t
    if (step.outcome === 'resolved') cursor += 1
  }

  const p = model.positions.map(({ budgets }) => {
    const value = share(budgets.X, budgets.X + budgets.O)
    return value.kind === 'ok' ? value.value : null
  })

  return { t, p }
}
