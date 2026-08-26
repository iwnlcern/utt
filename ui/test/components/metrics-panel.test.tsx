import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'
import { useState } from 'react'

import { cleanup, fireEvent, render, screen } from '@testing-library/react'
import { afterEach, describe, expect, it } from 'vitest'

import { MetricsPanel } from '../../src/components/MetricsPanel'
import type { AnalysisEntry } from '../../src/analysis/extract'
import type { Mark } from '../../src/log/types'
import { parseGameLog } from '../../src/log/validate'
import { deriveReplayModel } from '../../src/replay/derive'
import type { Position } from '../../src/replay/model'

const position = (budgets: Position['budgets'] = { X: 600_000_000, O: 400_000_000 }): Position => ({
  board: Array.from({ length: 9 }, () => '.........'),
  budgets,
  forced: null,
  tieOwner: null,
  closed: {},
})

const bound: AnalysisEntry = {
  kind: 'ok',
  t: 0.625,
  criticalBid: 100_000_000,
  pvIfWin: [0, 0],
  pvIfLose: [0, 1],
  quality: 'bound',
  lo: 0.5,
  hi: 0.75,
  depth: 12,
  complete: false,
  degraded: [],
}

function MetricsPanelHarness({
  analyses,
  selectedSeat: initialSeat = 'X',
  ...props
}: Omit<React.ComponentProps<typeof MetricsPanel>, 'selectedSeat' | 'onSelectedSeatChange'> & {
  selectedSeat?: Mark
}) {
  const [preferredSeat, setPreferredSeat] = useState<Mark>(initialSeat)
  const usableSeats = (['X', 'O'] as const).filter((seat) => analyses[seat]?.kind === 'ok')
  const definedSeats = (['X', 'O'] as const).filter((seat) => analyses[seat] !== undefined)
  const selectedSeat = usableSeats.includes(preferredSeat)
    ? preferredSeat
    : usableSeats[0] ?? (analyses[preferredSeat] !== undefined ? preferredSeat : definedSeats[0] ?? preferredSeat)
  return (
    <MetricsPanel
      {...props}
      analyses={analyses}
      onSelectedSeatChange={setPreferredSeat}
      selectedSeat={selectedSeat}
    />
  )
}

describe('MetricsPanel', () => {
  afterEach(cleanup)

  it('renders a bound interval in dual form with its quality and search provenance', () => {
    render(<MetricsPanelHarness analyses={{ X: bound }} position={position()} />)

    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'T: 62.50% (625\u202f000\u202f000 units)')).not.toBeNull()
    expect(screen.getByText('p: 60.00%')).not.toBeNull()
    expect(screen.getByText('margin p−T: -2.50%')).not.toBeNull()
    expect(screen.getByText('O favored')).not.toBeNull()
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'critical bid: 100\u202f000\u202f000 units (10.00%)')).not.toBeNull()
    expect(screen.getByText('quality: bound')).not.toBeNull()
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'interval [50.00% (500\u202f000\u202f000 units), 75.00% (750\u202f000\u202f000 units)]')).not.toBeNull()
    expect(screen.getByText('depth: 12 · incomplete')).not.toBeNull()
    expect(screen.getByText('If X wins: unavailable — awaiting harness artifact pin')).not.toBeNull()
    expect(screen.getByText('If O wins: unavailable — awaiting harness artifact pin')).not.toBeNull()
  })

  it('keeps exact zero units visible for the both-zero fixture while marking percentage metrics not applicable', () => {
    const text = readFileSync(resolve(import.meta.dirname, '../../fixtures/both-zero.jsonl'), 'utf8')
    const zeroPosition = deriveReplayModel(parseGameLog(text)).positions.find((candidate) =>
      candidate.budgets.X === 0 && candidate.budgets.O === 0,
    )
    if (zeroPosition === undefined) throw new Error('both-zero fixture must contain a zero-budget position')

    render(<MetricsPanelHarness analyses={{ X: bound }} position={zeroPosition} />)

    expect(screen.getByTestId('budget-units').textContent).toBe('0 / 0 units')
    expect(screen.getByText('T: 62.50%')).not.toBeNull()
    expect(screen.getByText('p: n/a — both budgets exhausted')).not.toBeNull()
    expect(screen.getByText('margin p−T: n/a — both budgets exhausted')).not.toBeNull()
    expect(screen.getByText('both budgets exhausted')).not.toBeNull()
  })

  it('distinguishes a malformed t from an absent t', () => {
    render(<MetricsPanelHarness analyses={{ X: { kind: 'ok', criticalBid: 100_000_000, degraded: ['t'] } }} position={position()} />)

    expect(screen.getByText('T: unavailable — malformed t in logged analysis')).not.toBeNull()
    expect(screen.getByText('margin p−T: unavailable — malformed t in logged analysis')).not.toBeNull()
    expect(screen.getByRole('status', { name: 'Malformed logged analysis fields ignored: t' })).not.toBeNull()
    expect(screen.queryByText('margin p−T: n/a — both budgets exhausted')).toBeNull()

    cleanup()
    render(<MetricsPanelHarness analyses={{ X: { kind: 'ok', criticalBid: 100_000_000, degraded: [] } }} position={position()} />)

    expect(screen.getByText('T: unavailable — t not present in analysis')).not.toBeNull()
    expect(screen.getByText('margin p−T: unavailable — t not present in analysis')).not.toBeNull()
    expect(screen.queryByRole('status', { name: /malformed logged analysis fields/i })).toBeNull()
  })

  it('gives a zero combined budget precedence for p and margin when t is missing', () => {
    render(<MetricsPanelHarness analyses={{ X: { kind: 'ok', criticalBid: 1, degraded: ['t'] } }} position={position({ X: 0, O: 0 })} />)

    expect(screen.getByText('T: unavailable — malformed t in logged analysis')).not.toBeNull()
    expect(screen.getByText('p: n/a — both budgets exhausted')).not.toBeNull()
    expect(screen.getByText('margin p−T: n/a — both budgets exhausted')).not.toBeNull()
  })

  it('names ignored fields using the logged analysis schema', () => {
    render(<MetricsPanelHarness analyses={{
      X: {
        kind: 'ok',
        t: 0.5,
        degraded: ['criticalBid', 'quality', 'lo', 'hi', 'complete'],
      },
    }} position={position()} />)

    const diagnostic = screen.getByRole('status', {
      name: 'Malformed logged analysis fields ignored: critical_bid, quality, lo, hi, complete',
    })
    expect(diagnostic.textContent).toBe('Malformed logged analysis fields ignored: critical_bid, quality, lo, hi, complete')
    expect(screen.queryByText(/criticalBid/)).toBeNull()
  })

  it('keeps bound interval percentages visible when both budgets are zero', () => {
    render(<MetricsPanelHarness analyses={{ X: bound }} position={position({ X: 0, O: 0 })} />)

    expect(screen.getByText('interval [50.00%, 75.00%]')).not.toBeNull()
    expect(screen.queryByText(/both budgets exhausted \(0 units\)/)).toBeNull()
  })

  it('shows a seat selector only when both seats have usable analysis', () => {
    const oEntry: AnalysisEntry = { kind: 'ok', t: 0.5, degraded: [] }
    const { rerender } = render(<MetricsPanelHarness analyses={{ X: bound }} position={position()} />)

    expect(screen.queryByLabelText('analysis seat')).toBeNull()

    rerender(<MetricsPanelHarness analyses={{ X: bound, O: oEntry }} position={position()} />)
    const selector = screen.getByLabelText('analysis seat') as HTMLSelectElement
    expect(selector).not.toBeNull()
    expect(selector.value).toBe('X')
    fireEvent.change(selector, { target: { value: 'O' } })
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'T: 50.00% (500\u202f000\u202f000 units)')).not.toBeNull()
  })

  it('falls back to the remaining usable seat when the remembered seat becomes unavailable', () => {
    const oEntry: AnalysisEntry = { kind: 'ok', t: 0.5, degraded: [] }
    const { rerender } = render(<MetricsPanelHarness analyses={{ X: bound, O: oEntry }} position={position()} />)
    const selector = screen.getByLabelText('analysis seat')
    fireEvent.change(selector, { target: { value: 'O' } })
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'T: 50.00% (500\u202f000\u202f000 units)')).not.toBeNull()

    rerender(<MetricsPanelHarness analyses={{ X: bound, O: { kind: 'unavailable', why: 'engine stopped analysis' } }} position={position()} />)
    expect(screen.queryByLabelText('analysis seat')).toBeNull()
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'T: 62.50% (625\u202f000\u202f000 units)')).not.toBeNull()
  })

  it('renders unavailable analysis reasons verbatim', () => {
    render(<MetricsPanelHarness analyses={{ O: { kind: 'unavailable', why: 'engine declined this position' } }} position={position()} />)

    expect(screen.getByText('engine declined this position')).not.toBeNull()
  })

  it.each([
    ['X favored', 0.5, { X: 600_000_000, O: 400_000_000 }],
    ['O favored', 0.7, { X: 600_000_000, O: 400_000_000 }],
    ['no displayed edge', 0.6, { X: 600_000_000, O: 400_000_000 }],
  ])('labels the sign of p−T as %s', (label, t, budgets) => {
    render(<MetricsPanelHarness analyses={{ X: { kind: 'ok', t, degraded: [] } }} position={position(budgets)} />)

    expect(screen.getByText(label)).not.toBeNull()
    expect(screen.queryByText(/p = T/i)).toBeNull()
  })

  it('uses only the displayed basis-point margin for arithmetic noise and sub-basis-point differences', () => {
    const { rerender } = render(<MetricsPanelHarness analyses={{ X: { kind: 'ok', t: 0.2 + 0.1, degraded: [] } }} position={position({ X: 3, O: 7 })} />)

    expect(screen.getByText('margin p−T: 0.00%')).not.toBeNull()
    expect(screen.getByText('no displayed edge')).not.toBeNull()
    expect(screen.queryByText(/p = T/i)).toBeNull()

    rerender(<MetricsPanelHarness analyses={{ X: { kind: 'ok', t: 0.30004, degraded: [] } }} position={position({ X: 3, O: 7 })} />)
    expect(screen.getByText('margin p−T: 0.00%')).not.toBeNull()
    expect(screen.getByText('no displayed edge')).not.toBeNull()
    expect(screen.queryByText(/favored|p = T/i)).toBeNull()

    rerender(<MetricsPanelHarness analyses={{ X: { kind: 'ok', t: 0.30006, degraded: [] } }} position={position({ X: 3, O: 7 })} />)
    expect(screen.getByText('margin p−T: -0.01%')).not.toBeNull()
    expect(screen.getByText('O favored')).not.toBeNull()
    expect(screen.queryByText(/p = T/i)).toBeNull()
  })

  it('formats T and bound units from the raw share rather than rounded display basis points', () => {
    const thirds: AnalysisEntry = {
      kind: 'ok', t: 1 / 3, quality: 'bound', lo: 1 / 3, hi: 2 / 3, degraded: [],
    }
    render(<MetricsPanelHarness analyses={{ X: thirds }} position={position({ X: 500_000_000, O: 500_000_000 })} />)

    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'T: 33.33% (333\u202f333\u202f333 units)')).not.toBeNull()
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'interval [33.33% (333\u202f333\u202f333 units), 66.67% (666\u202f666\u202f667 units)]')).not.toBeNull()
  })

  it('uses an explicit not-applicable status instead of zero-valued progress bars at both-zero', () => {
    render(<MetricsPanelHarness analyses={{ X: bound }} position={position({ X: 0, O: 0 })} />)

    expect(screen.queryByRole('progressbar', { name: 'X budget share' })).toBeNull()
    expect(screen.queryByRole('progressbar', { name: 'O budget share' })).toBeNull()
    expect(screen.getByRole('status', { name: 'X budget share: n/a — both budgets exhausted' })).not.toBeNull()
    expect(screen.getByRole('status', { name: 'O budget share: n/a — both budgets exhausted' })).not.toBeNull()
  })
})
