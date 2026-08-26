import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen } from '@testing-library/react'
import { afterEach, describe, expect, it, vi } from 'vitest'

import { Timeline } from '../../src/components/Timeline'
import { formatUnits } from '../../src/format/money'
import { parseGameLog } from '../../src/log/validate'
import { deriveReplayModel } from '../../src/replay/derive'

const fixtureModel = (name: string) => deriveReplayModel(parseGameLog(
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8'),
))

describe('Timeline', () => {
  afterEach(cleanup)

  it('renders both logged setup hello outcomes before a zero-auction terminal', () => {
    render(<Timeline model={fixtureModel('hello-fault.jsonl')} onSelect={vi.fn()} />)

    const setup = screen.getByTestId('setup-row')
    const terminal = screen.getByTestId('terminal-hello_fault')
    expect(setup.textContent).toContain('X hello: eof_or_crash fault')
    expect(setup.textContent).toContain('O hello: ok · stub')
    expect(setup.compareDocumentPosition(terminal) & Node.DOCUMENT_POSITION_FOLLOWING).not.toBe(0)
  })

  it('selects cursor zero from the first row without subtracting the zero-based wire ply', () => {
    const onSelect = vi.fn()
    render(<Timeline model={fixtureModel('success-macro-win.jsonl')} onSelect={onSelect} />)

    expect(screen.getByTestId('auction-row-0').textContent).toContain('ply 0')
    fireEvent.click(screen.getByRole('button', { name: 'select pending ply 0' }))

    expect(onSelect).toHaveBeenCalledWith(0)
  })

  it('selects the middle row pending cursor from its one-based ordinal', () => {
    const onSelect = vi.fn()
    render(<Timeline model={fixtureModel('success-macro-win.jsonl')} onSelect={onSelect} />)

    fireEvent.click(screen.getByRole('button', { name: 'select pending ply 4' }))

    expect(onSelect).toHaveBeenCalledWith(4)
  })

  it('selects the pending cursor from bid and resolution content without hijacking details', () => {
    const onSelect = vi.fn()
    render(<Timeline model={fixtureModel('success-macro-win.jsonl')} onSelect={onSelect} />)

    fireEvent.click(screen.getByTestId('bid-X-4'))
    expect(onSelect).toHaveBeenLastCalledWith(4)

    fireEvent.click(screen.getByTestId('resolution-4'))
    expect(onSelect).toHaveBeenLastCalledWith(4)
    expect(onSelect).toHaveBeenCalledTimes(2)

    fireEvent.click(screen.getAllByText('Attempts (1)')[4]!)
    expect(onSelect).toHaveBeenCalledTimes(2)
    expect((screen.getAllByText('Attempts (1)')[4]!.parentElement as HTMLDetailsElement).open).toBe(true)
  })

  it('keeps a terminal unresolved row on its pending cursor without exceeding the position range', () => {
    const model = fixtureModel('void-triple-double-fault.jsonl')
    const onSelect = vi.fn()
    const lastRow = model.auctions.length - 1
    render(<Timeline model={model} onSelect={onSelect} />)

    fireEvent.click(screen.getByRole('button', { name: `select pending ply ${lastRow}` }))

    expect(onSelect).toHaveBeenCalledWith(model.positions.length - 1)
    expect(onSelect).not.toHaveBeenCalledWith(model.positions.length)
    expect(screen.getByTestId(`auction-row-${lastRow}`).textContent).toContain('voided')
  })

  it('renders logged bids, intents, resolution, payment, budgets, and forced next without rule recomputation', () => {
    const model = fixtureModel('chip-count.jsonl')
    const first = model.auctions[0]
    if (first === undefined || first.outcome !== 'resolved') throw new Error('fixture must begin resolved')
    render(<Timeline model={model} onSelect={vi.fn()} />)

    expect(screen.getByTestId('bid-X-0').getAttribute('title')).toBe(`${formatUnits(first.attempts[0]?.turns.X.bid ?? 0)} units`)
    expect(screen.getByTestId('bid-O-0').getAttribute('title')).toBe(`${formatUnits(first.attempts[0]?.turns.O.bid ?? 0)} units`)
    expect(screen.getByTestId('intent-X-0').textContent).toContain(`[${first.attempts[0]?.turns.X.move?.join(', ')}]`)
    expect(screen.getByTestId('intent-O-0').textContent).toContain(`[${first.attempts[0]?.turns.O.move?.join(', ')}]`)
    expect(screen.getByTestId('resolution-0').textContent).toContain(first.resolution.reason)
    expect(screen.getByTestId('payment-0').textContent).toContain(`${formatUnits(first.resolution.payment)} units`)
    expect(screen.getByTestId('post-budgets-0').textContent).toBe(
      'post budgets: X 33.33% (500\u202f000\u202f000 units) · O 66.67% (1\u202f000\u202f000\u202f000 units)',
    )
    expect(screen.getByTestId('forced-next-0').textContent).toContain(String(first.resolution.forced_next))
  })

  it('uses n/a post-budget shares while retaining exact zero units at both-zero', () => {
    render(<Timeline model={fixtureModel('both-zero.jsonl')} onSelect={vi.fn()} />)

    expect(screen.getByTestId('post-budgets-34').textContent).toBe(
      'post budgets: X n/a — both budgets exhausted (0 units) · O n/a — both budgets exhausted (0 units)',
    )
  })

  it('uses the combined pre-auction budget for each bid when one seat has zero budget', () => {
    const model = fixtureModel('success-macro-win.jsonl')
    const first = model.auctions[0]
    if (first === undefined) throw new Error('fixture must begin with an auction')
    first.pre = { ...first.pre, budgets: { X: 1, O: 0 } }
    const xBid = first.attempts.at(-1)?.turns.X.bid
    const oBid = first.attempts.at(-1)?.turns.O.bid
    if (xBid === undefined || oBid === undefined) throw new Error('fixture must contain logged bids')
    first.attempts.at(-1)!.turns.X.bid = 1

    render(<Timeline model={model} onSelect={vi.fn()} />)

    const bid = screen.getByTestId('bid-O-0')
    expect(screen.getByTestId('bid-X-0').textContent).toBe('X: 100.00% (1 units)')
    expect(bid.textContent).toBe('O: 0.00% (0 units)')
    expect(bid.getAttribute('title')).toBe(`${formatUnits(oBid)} units`)
    expect(bid.textContent).not.toContain('both budgets exhausted')
  })

  it('renders tie bids with logged inline units and shared-budget percentages', () => {
    render(<Timeline model={fixtureModel('chip-count.jsonl')} onSelect={vi.fn()} />)

    expect(screen.getByTestId('bid-X-45').textContent).toBe('X: 30.77% (4 units)')
    expect(screen.getByTestId('bid-O-45').textContent).toBe('O: 30.77% (4 units)')
    expect(screen.getByTestId('bid-X-45').getAttribute('title')).toBe('4 units')
    expect(screen.getByTestId('bid-O-45').getAttribute('title')).toBe('4 units')
  })

  it('names each auction article with its ply and outcome', () => {
    const { rerender } = render(<Timeline model={fixtureModel('no-info.jsonl')} onSelect={vi.fn()} />)

    expect(screen.getByTestId('auction-row-0').getAttribute('aria-label')).toBe('ply 0: tie_coin: X')

    rerender(<Timeline model={fixtureModel('void-triple-double-fault.jsonl')} onSelect={vi.fn()} />)
    expect(screen.getByTestId('auction-row-0').getAttribute('aria-label')).toBe('ply 0: voided')
  })

  it('renders raw-ordered pre-auction recoveries above an expandable two-attempt retry', () => {
    const model = fixtureModel('double-fault-retry.jsonl')
    render(<Timeline model={model} onSelect={vi.fn()} />)

    const row = screen.getByTestId('auction-row-0')
    const preMarkers = screen.getByTestId('pre-recoveries-0')
    expect(preMarkers.textContent).toContain('X · invalid_json')
    expect(preMarkers.textContent).toContain('O · invalid_json')
    expect(preMarkers.compareDocumentPosition(row) & Node.DOCUMENT_POSITION_FOLLOWING).not.toBe(0)

    fireEvent.click(screen.getByText('Attempts (2)'))
    expect(screen.getAllByTestId(/attempt-0-/)).toHaveLength(2)
    expect(screen.getByTestId('attempt-0-1').textContent).toContain('X: invalid_json (raw captured)')
    expect(screen.getByTestId('attempt-0-1').textContent).toContain('O: invalid_json (raw captured)')
  })

  it('renders a keyed post-auction recovery below its resolved row', () => {
    render(<Timeline model={fixtureModel('fault-single.jsonl')} onSelect={vi.fn()} />)

    const row = screen.getByTestId('auction-row-0')
    const postMarkers = screen.getByTestId('post-recoveries-0')
    expect(postMarkers.textContent).toContain('X · illegal_move')
    expect(row.compareDocumentPosition(postMarkers) & Node.DOCUMENT_POSITION_FOLLOWING).not.toBe(0)
    expect(screen.getByTestId('resolution-0').textContent).toContain('fault: illegal_move')
  })

  it('uses the final resolving attempt for a fault label after a double-fault retry', () => {
    const events = readFileSync(resolve(import.meta.dirname, '../../fixtures/double-fault-retry.jsonl'), 'utf8')
      .trimEnd()
      .split('\n')
      .map((line) => JSON.parse(line) as Record<string, unknown>)
    const auction = events.find((event) => event.event === 'auction') as {
      attempts: Array<{ turns: { X: Record<string, unknown> } }>
      resolution: Record<string, unknown>
    } | undefined
    if (auction === undefined) throw new Error('fixture must contain an auction')
    const finalAttempt = auction.attempts[1]
    if (finalAttempt === undefined) throw new Error('fixture must contain a retry')
    finalAttempt.turns.X = {
      validation: 'illegal_move',
      elapsed_ms: 0,
      raw: { b64: '', truncated: false, bytes_total: 0 },
    }
    auction.resolution.reason = 'fault'

    render(<Timeline model={deriveReplayModel(parseGameLog(`${events.map(JSON.stringify).join('\n')}\n`))} onSelect={vi.fn()} />)

    expect(screen.getByTestId('resolution-0').textContent).toContain('fault: illegal_move')
    expect(screen.getByTestId('resolution-0').textContent).not.toContain('invalid_json')
  })

  it('renders trailing recovery markers and the incomplete-log tail notice', () => {
    render(<Timeline model={fixtureModel('trailing-recovery.jsonl')} onSelect={vi.fn()} />)

    expect(screen.getByTestId('trailing-recoveries').textContent).toContain('X · invalid_json')
    expect(screen.getByText('log ends mid-game')).not.toBeNull()
  })

  it('distinguishes a discarded malformed final line from a complete prefix that lacks game_end', () => {
    const { rerender } = render(<Timeline model={fixtureModel('truncated-line.jsonl')} onSelect={vi.fn()} />)

    expect(screen.getByText('log ends mid-game (truncated final line discarded)')).not.toBeNull()

    rerender(<Timeline model={fixtureModel('missing-game-end.jsonl')} onSelect={vi.fn()} />)
    expect(screen.getByText('log ends mid-game')).not.toBeNull()
    expect(screen.queryByText('log ends mid-game (truncated final line discarded)')).toBeNull()
  })

  it.each([
    ['success-macro-win.jsonl', 'macro_win'],
    ['chip-count.jsonl', 'chip_count'],
    ['exact-tie-draw.jsonl', 'exact_tie_draw'],
    ['hello-fault.jsonl', 'hello_fault'],
    ['recovery-fault-abort.jsonl', 'recovery_fault'],
    ['void-triple-double-fault.jsonl', 'triple_double_fault_void'],
  ])('renders the %s terminal reason distinctly', (fixture, reason) => {
    render(<Timeline model={fixtureModel(fixture)} onSelect={vi.fn()} />)

    expect(screen.getByTestId(`terminal-${reason}`).textContent).toContain(reason)
  })
})
