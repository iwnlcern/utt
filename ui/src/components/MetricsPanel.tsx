import { PV_PIN, PV_UNAVAILABLE_MESSAGE, hasLoggedInfo, type AnalysisEntry } from '../analysis/extract'
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
  selectedSeat: Mark
  onSelectedSeatChange: (seat: Mark) => void
}

const seats: readonly Mark[] = ['X', 'O']
const MISSING_THRESHOLD_MESSAGE = 'unavailable — t not present in analysis'
const MALFORMED_THRESHOLD_MESSAGE = 'unavailable — malformed t in logged analysis'
const loggedFieldNames: Readonly<Record<string, string>> = {
  criticalBid: 'critical_bid',
  pvIfWin: 'pv_if_win',
  pvIfLose: 'pv_if_lose',
}
const loggedFieldOrder = [
  't',
  'critical_bid',
  'pv_if_win',
  'pv_if_lose',
  'quality',
  'lo',
  'hi',
  'depth',
  'complete',
] as const

function dualShare(value: number, combined: number): string {
  const basisPoints = percentBasisPoints({ kind: 'ok', value }) ?? 0
  if (combined === 0) {
    return formatPercentBasisPoints(basisPoints)
  }
  return `${formatPercentBasisPoints(basisPoints)} (${formatUnits(roundShareToUnits(value, combined))} units)`
}

function favoredLabel(marginBasisPoints: number | undefined): string | undefined {
  if (marginBasisPoints === undefined) return undefined
  if (marginBasisPoints > 0) return 'X favored'
  if (marginBasisPoints < 0) return 'O favored'
  return 'no displayed edge'
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

export function MetricsPanel({ position, analyses, selectedSeat, onSelectedSeatChange }: MetricsPanelProps) {
  const loggedSeats = seats.filter((seat) => hasLoggedInfo(analyses[seat]))
  const entry = analyses[selectedSeat] ?? { kind: 'unavailable', why: 'no analysis in this log' }
  const combined = position.budgets.X + position.budgets.O
  const p = share(position.budgets.X, combined)

  return (
    <section aria-label="analysis metrics" className="metrics">
      <h2>Analysis</h2>
      <BudgetBars budgets={position.budgets} />
      {loggedSeats.length === 2 && (
        <label>
          Analysis seat
          <select aria-label="analysis seat" onChange={(event) => onSelectedSeatChange(event.target.value as Mark)} value={selectedSeat}>
            {loggedSeats.map((availableSeat) => <option key={availableSeat} value={availableSeat}>{availableSeat}</option>)}
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
  const thresholdUnavailable = entry.degraded.includes('t')
    ? MALFORMED_THRESHOLD_MESSAGE
    : MISSING_THRESHOLD_MESSAGE
  const degradedFieldSet = new Set(entry.degraded.map((field) => loggedFieldNames[field] ?? field))
  const degradedFields = [
    ...loggedFieldOrder.filter((field) => degradedFieldSet.delete(field)),
    ...degradedFieldSet,
  ]
  const degradationMessage = degradedFields.length === 0
    ? undefined
    : `Malformed logged analysis fields ignored: ${degradedFields.join(', ')}`
  const displayedMargin = p.kind === 'ok' && threshold !== undefined
    ? percentBasisPoints({ kind: 'ok', value: p.value - threshold })
    : undefined
  const margin = p.kind === 'na'
    ? formatPercent({ kind: 'na', why: 'both budgets exhausted' })
    : threshold === undefined
      ? thresholdUnavailable
      : formatPercentBasisPoints(displayedMargin ?? 0)
  const favored = favoredLabel(displayedMargin)
  const thresholdText = threshold === undefined
    ? thresholdUnavailable
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
      {degradationMessage !== undefined && (
        <p aria-label={degradationMessage} className="metrics__unavailable" role="status">
          {degradationMessage}
        </p>
      )}
    </div>
  )
}
