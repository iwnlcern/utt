import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen, waitFor, within } from '@testing-library/react'
import { afterEach, beforeEach, describe, expect, it } from 'vitest'

import { PV_PIN } from '../../src/analysis/extract'
import GameView from '../../src/app/GameView'
import { parseGameLog } from '../../src/log/validate'

const fixtureGame = (name: string) => parseGameLog(
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8'),
)

const gameWithBothSeatAnalysis = () => {
  const game = fixtureGame('ghost-divergence.jsonl')
  const firstAuction = game.events.find((event) => event.event === 'auction')
  if (firstAuction === undefined) throw new Error('ghost divergence fixture must contain an auction')
  firstAuction.attempts[0].turns.O.info = {
    critical_bid: 250_000_000,
    pv_if_win: [1, 0],
    pv_if_lose: [1, 1],
    quality: 'exact',
    t: 0.25,
  }
  return game
}

const gameWithBothThenXOnlyAnalysis = () => {
  const game = gameWithBothSeatAnalysis()
  const auctions = game.events.filter((event) => event.event === 'auction')
  const secondAuction = auctions[1]
  if (secondAuction === undefined) throw new Error('ghost divergence fixture must contain a second auction')
  secondAuction.attempts[0].turns.X.info = {
    pv_if_win: [2, 0],
    pv_if_lose: [2, 1],
    quality: 'exact',
    t: 0.75,
  }
  return game
}

const gameWithOOnlyAnalysis = () => {
  const game = gameWithBothSeatAnalysis()
  const firstAuction = game.events.find((event) => event.event === 'auction')
  if (firstAuction === undefined) throw new Error('ghost divergence fixture must contain an auction')
  delete firstAuction.attempts[0].turns.X.info
  return game
}

const gameWithInvalidThreshold = () => {
  const game = fixtureGame('ghost-divergence.jsonl')
  const firstAuction = game.events.find((event) => event.event === 'auction')
  if (firstAuction === undefined) throw new Error('ghost divergence fixture must contain an auction')
  firstAuction.attempts[0].turns.X.info = { t: -0.001, critical_bid: 100_000_000 }
  return game
}

describe('GameView', () => {
  beforeEach(() => {
    window.history.replaceState(null, '', '/')
    Object.defineProperty(window, 'matchMedia', {
      configurable: true,
      value: (query: string) => ({
        matches: false,
        media: query,
        onchange: null,
        addEventListener: () => undefined,
        removeEventListener: () => undefined,
        addListener: () => undefined,
        removeListener: () => undefined,
        dispatchEvent: () => true,
      }),
    })
  })

  afterEach(() => {
    cleanup()
    PV_PIN.pinned = false
    PV_PIN.source = null
    window.history.replaceState(null, '', '/')
  })

  it('hydrates from the cursor hash and advances the reducer with ArrowRight while retaining focus', async () => {
    window.history.replaceState(null, '', '#cursor=2')
    render(<GameView game={fixtureGame('success-macro-win.jsonl')} />)

    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 2 of 69')
    const next = screen.getByRole('button', { name: 'Next position' })
    next.focus()

    fireEvent.keyDown(window, { key: 'ArrowRight' })

    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 3 of 69')
    expect(document.activeElement).toBe(next)
    await waitFor(() => expect(window.location.hash).toBe('#cursor=3'))
  })

  it('maps Home, End, PageUp, and PageDown to bounded replay jumps', () => {
    window.history.replaceState(null, '', '#cursor=2')
    render(<GameView game={fixtureGame('success-macro-win.jsonl')} />)

    const readPosition = () => screen.getByRole('status', { name: 'replay position' }).textContent

    fireEvent.keyDown(window, { key: 'PageDown' })
    expect(readPosition()).toBe('Position 7 of 69')
    fireEvent.keyDown(window, { key: 'PageUp' })
    expect(readPosition()).toBe('Position 2 of 69')
    fireEvent.keyDown(window, { key: 'End' })
    expect(readPosition()).toBe('Position 69 of 69')
    fireEvent.keyDown(window, { key: 'Home' })
    expect(readPosition()).toBe('Position 0 of 69')
    fireEvent.keyDown(window, { key: 'ArrowLeft' })
    expect(readPosition()).toBe('Position 0 of 69')
  })

  it('leaves replay position and focus unchanged when a seat-select arrow key bubbles to window', () => {
    render(<GameView game={gameWithBothSeatAnalysis()} />)
    const selector = screen.getByRole('combobox', { name: 'analysis seat' })
    selector.focus()

    fireEvent.keyDown(selector, { key: 'ArrowRight' })

    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 69')
    expect(document.activeElement).toBe(selector)
  })

  it.each([
    ['input', 'input'],
    ['textarea', 'textarea'],
    ['contenteditable', '[contenteditable]'],
  ])('ignores replay shortcuts from a focused %s', (_, selector) => {
    const { container } = render(
      <>
        <GameView game={fixtureGame('ghost-divergence.jsonl')} />
        <input aria-label="test input" />
        <textarea aria-label="test textarea" />
        <div contentEditable data-testid="test-contenteditable" />
      </>,
    )
    const target = container.querySelector(selector)
    if (target === null) throw new Error(`missing editable target ${selector}`)

    fireEvent.keyDown(target, { key: 'ArrowRight' })

    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 69')
  })

  it.each(['altKey', 'ctrlKey', 'metaKey', 'shiftKey'] as const)(
    'ignores a replay shortcut with %s',
    (modifier) => {
      render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)

      fireEvent.keyDown(window, { key: 'ArrowRight', [modifier]: true })

      expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 69')
    },
  )

  it('selects the pending pre-auction position when a timeline row is clicked', () => {
    window.history.replaceState(null, '', '#cursor=9')
    render(<GameView game={fixtureGame('success-macro-win.jsonl')} />)

    fireEvent.click(screen.getByRole('button', { name: 'select pending ply 4' }))

    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 4 of 69')
  })

  it('renders position_k with the pending forced board, then the just-resolved move and payment', () => {
    render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)

    expect(screen.getByTestId('local-board-4').getAttribute('data-forced')).toBe('true')
    expect(screen.queryByTestId('last-payment')).toBeNull()

    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.getByRole('button', { name: 'local 4, cell 3, X' }).getAttribute('data-last-applied')).toBe('true')
    expect(screen.getByTestId('last-payment').textContent).toContain('payment: 0 units')
    expect(screen.getByTestId('local-board-3').getAttribute('data-forced')).toBe('true')
  })

  it('shows both test-enabled conditional ghosts only on their pending pre-auction position', () => {
    PV_PIN.pinned = true
    PV_PIN.source = 'test-only ghost association seam'
    render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)

    expect(within(screen.getByRole('button', { name: 'local 0, cell 0, empty' })).getByTestId('conditional-ghost-X')).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 0, cell 1, empty' })).getByTestId('conditional-ghost-O')).not.toBeNull()
    expect(screen.getAllByTestId(/conditional-ghost-[XO]/)).toHaveLength(2)

    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.queryByTestId('conditional-ghost-X')).toBeNull()
    expect(screen.queryByTestId('conditional-ghost-O')).toBeNull()
  })

  it('keeps a zero-auction hello fault at position zero with every step control disabled', () => {
    window.history.replaceState(null, '', '#cursor=8')
    render(<GameView game={fixtureGame('hello-fault.jsonl')} />)

    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 0')
    for (const name of ['First position', 'Previous position', 'Next position', 'Last position']) {
      expect((screen.getByRole('button', { name }) as HTMLButtonElement).disabled).toBe(true)
    }
    expect(screen.queryByRole('button', { name: /select pending ply/ })).toBeNull()
  })

  it('binds metrics to the pending auction and the whole-game chart to cursor positions', () => {
    render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)

    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('T: 50.00%')
    expect(screen.getByRole('img', { name: /current t: 50\.00%; current p: 50\.00%/i })).not.toBeNull()

    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('no info in log')
    expect(screen.getByRole('img', { name: /current t: unavailable; current p: 50\.00%/i })).not.toBeNull()
  })

  it('uses the same degraded threshold in pending metrics and the chart series', () => {
    render(<GameView game={gameWithInvalidThreshold()} />)

    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain(
      'T: unavailable — t not present in analysis',
    )
    expect(screen.getByRole('img', { name: /current t: unavailable; current p: 50\.00%/i })).not.toBeNull()
  })

  it('aligns O-selected metrics, chart, and ghosts through usable-seat cursor transitions', () => {
    PV_PIN.pinned = true
    PV_PIN.source = 'test-only selected-seat association seam'
    render(<GameView game={gameWithBothThenXOnlyAnalysis()} />)

    fireEvent.change(screen.getByRole('combobox', { name: 'analysis seat' }), { target: { value: 'O' } })

    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('T: 25.00%')
    expect(screen.getByRole('img', { name: /current t: 25\.00%; current p: 50\.00%/i })).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 1, cell 0, empty' })).getByTestId('conditional-ghost-X')).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 1, cell 1, empty' })).getByTestId('conditional-ghost-O')).not.toBeNull()

    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.queryByRole('combobox', { name: 'analysis seat' })).toBeNull()
    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('T: 75.00%')
    expect(screen.getByRole('img', { name: /current t: 75\.00%; current p: 50\.00%/i })).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 2, cell 0, empty' })).getByTestId('conditional-ghost-X')).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 2, cell 1, empty' })).getByTestId('conditional-ghost-O')).not.toBeNull()

    fireEvent.click(screen.getByRole('button', { name: 'Previous position' }))

    expect((screen.getByRole('combobox', { name: 'analysis seat' }) as HTMLSelectElement).value).toBe('O')
    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('T: 25.00%')
    expect(screen.getByRole('img', { name: /current t: 25\.00%; current p: 50\.00%/i })).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 1, cell 0, empty' })).getByTestId('conditional-ghost-X')).not.toBeNull()
  })

  it('aligns O-only metrics, chart, and ghosts without offering an unusable seat', () => {
    PV_PIN.pinned = true
    PV_PIN.source = 'test-only O-only selected-seat association seam'
    render(<GameView game={gameWithOOnlyAnalysis()} />)

    expect(screen.queryByRole('combobox', { name: 'analysis seat' })).toBeNull()
    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('T: 25.00%')
    expect(screen.getByRole('img', { name: /current t: 25\.00%; current p: 50\.00%/i })).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 1, cell 0, empty' })).getByTestId('conditional-ghost-X')).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 1, cell 1, empty' })).getByTestId('conditional-ghost-O')).not.toBeNull()
  })

  it('shows the just-resolved losing intent by default and exposes its hide toggle', () => {
    render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)
    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.getByTestId('losing-intent-ghost').textContent).toBe('O')
    const hideToggle = screen.getByRole('button', { name: 'Hide losing intent' })
    expect(hideToggle.getAttribute('aria-pressed')).toBe('true')
    fireEvent.click(hideToggle)

    expect(screen.queryByTestId('losing-intent-ghost')).toBeNull()
    expect(screen.getByRole('button', { name: 'Show losing intent' }).getAttribute('aria-pressed')).toBe('false')
  })

  it('starts a fresh reveal marker when a forward step resolves an auction', () => {
    render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)

    expect(screen.getByTestId('board-stage').getAttribute('data-revealing')).toBeNull()
    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.getByTestId('board-stage').getAttribute('data-revealing')).toBe('true')
    expect(screen.getByTestId('board-stage').getAttribute('data-reveal-version')).toBe('1')
  })

  it('skips the reveal marker when reduced motion is requested', () => {
    Object.defineProperty(window, 'matchMedia', {
      configurable: true,
      value: (query: string) => ({
        matches: query === '(prefers-reduced-motion: reduce)',
        media: query,
        onchange: null,
        addEventListener: () => undefined,
        removeEventListener: () => undefined,
        addListener: () => undefined,
        removeListener: () => undefined,
        dispatchEvent: () => true,
      }),
    })
    render(<GameView game={fixtureGame('ghost-divergence.jsonl')} />)

    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))

    expect(screen.getByTestId('board-stage').getAttribute('data-revealing')).toBeNull()
  })
})
