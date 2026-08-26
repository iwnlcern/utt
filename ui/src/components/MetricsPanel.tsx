import { useState } from 'react'

import { PV_PIN, PV_UNAVAILABLE_MESSAGE, type AnalysisEntry } from '../analysis/extract'
import {
  formatPercent,
  formatPercentBasisPoints,
  formatUnits,
  percentBasisPoints,
  roundShareToUnits,
  share,
  type Share,
} from '../format/money'
import type { Mark } from '../log/types'
import type { Position } from '../replay/model'
import { BudgetBars } from './BudgetBars'
import '../styles/tokens.css'

export interface MetricsPanelProps {
  position: Position
  analyses: Partial<Record<Mark, AnalysisEntry>>
}

const seats: readonly Mark[] = ['X', 'O']
const MISSING_THRESHOLD_MESSAGE = 'unavailable — t not present in analysis'

function firstUsableSeat(analyses: MetricsPanelProps['analyses']): Mark | undefined {
  return seats.find((seat) => analyses[seat]?.kind === 'ok')
}

function dualShare(value: number, combined: number): string {
  if (combined === 0) {
    return `${formatPercent(share(0, combined))} (${formatUnits(0)} units)`
  }
  const basisPoints = percentBasisPoints({ kind: 'ok', value }) ?? 0
  return `${formatPercentBasisPoints(basisPoints)} (${formatUnits(roundShareToUnits(value, combined))} units)`
}

function favoredLabel(p: Share, threshold: number | undefined): string | undefined {
  const actualBasisPoints = percentBasisPoints(p)
  const thresholdBasisPoints = threshold === undefined
    ? undefined
    : percentBasisPoints({ kind: 'ok', value: threshold })
  if (actualBasisPoints === undefined || thresholdBasisPoints === undefined) return undefined
  const marginBasisPoints = actualBasisPoints - thresholdBasisPoints
  if (marginBasisPoints > 0) return 'X favored'
  if (marginBasisPoints < 0) return 'O favored'
  return 'knife-edge at p = T'
}

function ConditionalLines({ entry }: { entry: Extract<AnalysisEntry, { kind: 'ok' }> }) {
  const conditional = [
    ['If X wins', entry.pvIfWin],
    ['If O wins', entry.pvIfLose],
  ] as const

  return conditional.map(([label, move]) => {
    if (move === undefined) return null
    const text = PV_PIN.pinned ? `[${move.join(', ')}]` : PV_UNAVAILABLE_MESSAGE
    return <p key={label}>{label}: {text}</p>
  })
}

export function MetricsPanel({ position, analyses }: MetricsPanelProps) {
  const usableSeats = seats.filter((seat) => analyses[seat]?.kind === 'ok')
  const defaultSeat = firstUsableSeat(analyses) ?? seats.find((seat) => analyses[seat] !== undefined) ?? 'X'
  const [seat, setSeat] = useState<Mark>(defaultSeat)
  const selectedSeat = analyses[seat]?.kind === 'ok' ? seat : defaultSeat
  const entry = analyses[selectedSeat] ?? { kind: 'unavailable', why: 'no analysis in this log' }
  const combined = position.budgets.X + position.budgets.O
  const p = share(position.budgets.X, combined)

  return (
    <section aria-label="analysis metrics" className="metrics">
      <h2>Analysis</h2>
      <BudgetBars budgets={position.budgets} />
      {usableSeats.length === 2 && (
        <label>
          Analysis seat
          <select aria-label="analysis seat" onChange={(event) => setSeat(event.target.value as Mark)} value={selectedSeat}>
            {usableSeats.map((availableSeat) => <option key={availableSeat} value={availableSeat}>{availableSeat}</option>)}
          </select>
        </label>
      )}
      {entry.kind === 'unavailable' ? (
        <p className="metrics__unavailable">{entry.why}</p>
      ) : (
        <Metrics entry={entry} p={p} combined={combined} />
      )}
    </section>
  )
}

function Metrics({ entry, p, combined }: {
  entry: Extract<AnalysisEntry, { kind: 'ok' }>
  p: Share
  combined: number
}) {
  const threshold = entry.t
  const actualBasisPoints = percentBasisPoints(p)
  const thresholdBasisPoints = threshold === undefined
    ? undefined
    : percentBasisPoints({ kind: 'ok', value: threshold })
  const margin = actualBasisPoints === undefined
    ? formatPercent({ kind: 'na', why: 'both budgets exhausted' })
    : thresholdBasisPoints === undefined
      ? MISSING_THRESHOLD_MESSAGE
      : formatPercentBasisPoints(actualBasisPoints - thresholdBasisPoints)
  const favored = favoredLabel(p, threshold)
  const thresholdText = threshold === undefined
    ? MISSING_THRESHOLD_MESSAGE
    : p.kind === 'na'
      ? formatPercent(p)
      : dualShare(threshold, combined)

  return (
    <div className="metrics__values">
      <p>T: {thresholdText}</p>
      <p>p: {formatPercent(p)}</p>
      <p>margin p−T: {margin}</p>
      {favored !== undefined && <p>{favored}</p>}
      {p.kind === 'na' && <p>both budgets exhausted</p>}
      {entry.criticalBid !== undefined && (
        <p>critical bid: {formatUnits(entry.criticalBid)} units ({formatPercent(share(entry.criticalBid, combined))})</p>
      )}
      {entry.quality !== undefined && <p>quality: {entry.quality}</p>}
      {entry.quality === 'bound' && entry.lo !== undefined && entry.hi !== undefined && (
        <p>interval [{dualShare(entry.lo, combined)}, {dualShare(entry.hi, combined)}]</p>
      )}
      {(entry.depth !== undefined || entry.complete !== undefined) && (
        <p>
          {entry.depth !== undefined && `depth: ${entry.depth}`}
          {entry.depth !== undefined && entry.complete !== undefined && ' · '}
          {entry.complete !== undefined && (entry.complete ? 'complete' : 'incomplete')}
        </p>
      )}
      <ConditionalLines entry={entry} />
    </div>
  )
}
