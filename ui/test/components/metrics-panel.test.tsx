import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen } from '@testing-library/react'
import { afterEach, describe, expect, it } from 'vitest'

import { MetricsPanel } from '../../src/components/MetricsPanel'
import type { AnalysisEntry } from '../../src/analysis/extract'
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

describe('MetricsPanel', () => {
  afterEach(cleanup)

  it('renders a bound interval in dual form with its quality and search provenance', () => {
    render(<MetricsPanel analyses={{ X: bound }} position={position()} />)

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

    render(<MetricsPanel analyses={{ X: bound }} position={zeroPosition} />)

    expect(screen.getByTestId('budget-units').textContent).toBe('0 / 0 units')
    expect(screen.getByText('T: n/a — both budgets exhausted')).not.toBeNull()
    expect(screen.getByText('p: n/a — both budgets exhausted')).not.toBeNull()
    expect(screen.getByText('margin p−T: n/a — both budgets exhausted')).not.toBeNull()
    expect(screen.getByText('both budgets exhausted')).not.toBeNull()
  })

  it('shows a seat selector only when both seats have usable analysis', () => {
    const oEntry: AnalysisEntry = { kind: 'ok', t: 0.5, degraded: [] }
    const { rerender } = render(<MetricsPanel analyses={{ X: bound }} position={position()} />)

    expect(screen.queryByLabelText('analysis seat')).toBeNull()

    rerender(<MetricsPanel analyses={{ X: bound, O: oEntry }} position={position()} />)
    const selector = screen.getByLabelText('analysis seat') as HTMLSelectElement
    expect(selector).not.toBeNull()
    expect(selector.value).toBe('X')
    fireEvent.change(selector, { target: { value: 'O' } })
    expect(screen.getByText((_, element) => element?.tagName === 'P'
      && element.textContent === 'T: 50.00% (500\u202f000\u202f000 units)')).not.toBeNull()
  })

  it('renders unavailable analysis reasons verbatim', () => {
    render(<MetricsPanel analyses={{ O: { kind: 'unavailable', why: 'engine declined this position' } }} position={position()} />)

    expect(screen.getByText('engine declined this position')).not.toBeNull()
  })

  it.each([
    ['X favored', 0.5, { X: 600_000_000, O: 400_000_000 }],
    ['O favored', 0.7, { X: 600_000_000, O: 400_000_000 }],
    ['knife-edge at p = T', 0.6, { X: 600_000_000, O: 400_000_000 }],
  ])('labels the sign of p−T as %s', (label, t, budgets) => {
    render(<MetricsPanel analyses={{ X: { kind: 'ok', t, degraded: [] } }} position={position(budgets)} />)

    expect(screen.getByText(label)).not.toBeNull()
  })
})
