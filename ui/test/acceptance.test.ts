import { readFileSync, readdirSync } from 'node:fs'
import { resolve } from 'node:path'
import { createElement } from 'react'

import { cleanup, fireEvent, render, screen, within } from '@testing-library/react'
import { afterEach, beforeEach, describe, expect, it } from 'vitest'

import { fixtureManifest } from '../fixtures/manifest'
import { PV_PIN, PV_UNAVAILABLE_MESSAGE } from '../src/analysis/extract'
import GameView from '../src/app/GameView'
import { formatUnits } from '../src/format/money'
import type { FaultClass, LogEvent } from '../src/log/types'
import { LogError, parseGameLog } from '../src/log/validate'
import { deriveReplayModel } from '../src/replay/derive'

const fixtureText = (path: string) =>
  readFileSync(resolve(import.meta.dirname, '../fixtures', path), 'utf8')

const realTranscriptText = () =>
  readFileSync(resolve(import.meta.dirname, '../../docs/protocol/transcript-v1.jsonl'), 'utf8')

const conformanceRoot = resolve(import.meta.dirname, '../../referee/tests/fixtures')
const conformanceFixtures = [
  'fault-eof_or_crash.jsonl',
  'fault-extra_protocol_line.jsonl',
  'fault-illegal_bid.jsonl',
  'fault-illegal_move.jsonl',
  'fault-invalid_json.jsonl',
  'fault-invalid_utf8.jsonl',
  'fault-oversize_line.jsonl',
  'fault-schema_violation.jsonl',
  'fault-timeout.jsonl',
  'fault-wrong_request_id.jsonl',
  'parity-even/game-0001-r0001-g1.jsonl',
  'parity-even/game-0001-r0001-g2.jsonl',
  'parity-odd/game-0001-r0001-g1.jsonl',
  'parity-odd/game-0001-r0001-g2.jsonl',
  'recovery-both-fail.jsonl',
  'recovery-double-one-fail.jsonl',
  'recovery-single-fail.jsonl',
  'success.jsonl',
  'terminal-fault-won-no-recovery.jsonl',
] as const
const faultClasses = [
  'timeout',
  'eof_or_crash',
  'invalid_utf8',
  'invalid_json',
  'schema_violation',
  'wrong_request_id',
  'extra_protocol_line',
  'oversize_line',
  'illegal_bid',
  'illegal_move',
] as const satisfies readonly FaultClass[]

const conformanceText = (path: string) => readFileSync(resolve(conformanceRoot, path), 'utf8')

const conformanceFilesOnDisk = (directory = conformanceRoot, prefix = ''): string[] =>
  readdirSync(directory, { withFileTypes: true }).flatMap((entry) => {
    const relative = prefix === '' ? entry.name : `${prefix}/${entry.name}`
    return entry.isDirectory()
      ? conformanceFilesOnDisk(resolve(directory, entry.name), relative)
      : entry.name.endsWith('.jsonl') ? [relative] : []
  })

const fixtureGame = (path: string) => parseGameLog(fixtureText(path))

const renderFixture = (path: string) => {
  const game = fixtureGame(path)
  const model = deriveReplayModel(game)
  const rendered = render(createElement(GameView, { game }))
  return { game, model, ...rendered }
}

const expectImmediatelyBefore = (markerGroup: HTMLElement, auctionRow: HTMLElement) => {
  expect(markerGroup.parentElement).toBe(auctionRow.parentElement)
  expect(markerGroup.nextElementSibling).toBe(auctionRow)
  expect(auctionRow.previousElementSibling).toBe(markerGroup)
}

const expectImmediatelyAfter = (auctionRow: HTMLElement, markerGroup: HTMLElement) => {
  expect(markerGroup.parentElement).toBe(auctionRow.parentElement)
  expect(auctionRow.nextElementSibling).toBe(markerGroup)
  expect(markerGroup.previousElementSibling).toBe(auctionRow)
}

const bothZeroGameWithAnalysis = () => {
  const source = fixtureGame('both-zero.jsonl')
  const pending = source.events.find(
    (event): event is Extract<LogEvent, { event: 'auction' }> =>
      event.event === 'auction' && event.ply === 35,
  )
  const finalAttempt = pending?.attempts.at(-1)
  if (finalAttempt === undefined) throw new Error('both-zero fixture must carry pending ply 35')
  finalAttempt.turns.X.info = {
    complete: false,
    critical_bid: 0,
    depth: 12,
    hi: 0.75,
    lo: 0.5,
    quality: 'bound',
    t: 0.625,
  }
  return parseGameLog(`${source.events.map((event) => JSON.stringify(event)).join('\n')}\n`)
}

const viewableFixtures = fixtureManifest.filter(
  ({ expected }) => expected.rawJsonl !== 'malformed-interior',
)

describe('UI v1 composed acceptance', () => {
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

  it.each(viewableFixtures)(
    'opens $name through parse, derive, and GameView',
    ({ path, expected }) => {
      const { model } = renderFixture(path)

      expect(screen.getByRole('heading', { name: 'Game replay' })).not.toBeNull()
      expect(within(screen.getByRole('region', { name: 'UTTT board' })).getAllByRole('button')).toHaveLength(81)
      expect(screen.getByRole('region', { name: 'auction timeline' })).not.toBeNull()
      expect(model.positions.length).toBeGreaterThan(0)

      if ('terminal' in expected && expected.terminal !== undefined) {
        expect(screen.getByTestId(`terminal-${expected.terminal.reason}`).textContent)
          .toContain(`(${expected.terminal.result})`)
      }
    },
  )

  it('pins the complete landed harness/referee conformance inventory and every fault class', () => {
    expect(conformanceFilesOnDisk().sort()).toEqual([...conformanceFixtures].sort())
    expect(
      conformanceFixtures.filter((path) => path.startsWith('fault-')).sort(),
    ).toEqual(faultClasses.map((fault) => `fault-${fault}.jsonl`).sort())
  })

  it.each(conformanceFixtures)(
    'opens owner conformance %s through parse, derive, and GameView',
    (path) => {
      const game = parseGameLog(conformanceText(path))
      const model = deriveReplayModel(game)

      render(createElement(GameView, { game }))

      expect(screen.getByRole('heading', { name: 'Game replay' })).not.toBeNull()
      expect(within(screen.getByRole('region', { name: 'UTTT board' })).getAllByRole('button')).toHaveLength(81)
      expect(screen.getByRole('region', { name: 'auction timeline' })).not.toBeNull()
      expect(model.positions.length).toBeGreaterThan(0)
    },
  )

  it('accepts malformed-interior only as the normative structured loader rejection', () => {
    expect(() => fixtureGame('malformed-interior.jsonl')).toThrow(
      expect.objectContaining({
        line: 2,
        event_index: 1,
        reason: 'invalid JSON event',
      } satisfies Partial<LogError>),
    )
  })

  it('opens the real referee-emitted protocol transcript through the composed replay view', () => {
    const game = parseGameLog(realTranscriptText())
    const model = deriveReplayModel(game)

    render(createElement(GameView, { game }))

    expect(model.terminal).toBeDefined()
    expect(screen.getByRole('heading', { name: 'Game replay' })).not.toBeNull()
    expect(within(screen.getByRole('region', { name: 'UTTT board' })).getAllByRole('button')).toHaveLength(81)
    expect(screen.getByRole('region', { name: 'auction timeline' })).not.toBeNull()
  })

  it('keeps a no-info game fully replayable with honest unavailable analysis', () => {
    renderFixture('no-info.jsonl')

    expect(screen.getByRole('region', { name: 'analysis metrics' }).textContent).toContain('no info in log')
    expect(screen.getByRole('img', { name: /current t: unavailable; current p: 50\.00%/i })).not.toBeNull()

    const next = screen.getByRole('button', { name: 'Next position' })
    expect((next as HTMLButtonElement).disabled).toBe(false)
    fireEvent.click(next)
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 1 of 69')
    fireEvent.click(screen.getByRole('button', { name: 'Last position' }))
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 69 of 69')
    expect(within(screen.getByRole('region', { name: 'UTTT board' })).getAllByRole('button')).toHaveLength(81)
  })

  it('renders every logged success analysis value without inventing a replacement', () => {
    const { game } = renderFixture('success-macro-win.jsonl')
    const firstAuction = game.events.find(
      (event): event is Extract<LogEvent, { event: 'auction' }> => event.event === 'auction',
    )
    const info = firstAuction?.attempts[0]?.turns.X.info as Record<string, unknown> | undefined
    if (info === undefined) throw new Error('success fixture must carry X analysis')

    const metrics = screen.getByRole('region', { name: 'analysis metrics' })
    expect(info).toEqual({
      complete: false,
      critical_bid: 100_000_000,
      depth: 12,
      hi: 0.75,
      lo: 0.5,
      pv_if_lose: [0, 1],
      pv_if_win: [0, 0],
      quality: 'bound',
      t: 0.625,
    })
    expect(metrics.textContent).toContain('T: 62.50%')
    expect(metrics.textContent).toContain(`critical bid: ${formatUnits(100_000_000)} units (5.00%)`)
    expect(metrics.textContent).toContain('quality: bound')
    expect(metrics.textContent).toContain('interval [50.00% (1\u202f000\u202f000\u202f000 units), 75.00% (1\u202f500\u202f000\u202f000 units)]')
    expect(metrics.textContent).toContain('depth: 12 · incomplete')
    expect(metrics.textContent).toContain(`If X wins: ${PV_UNAVAILABLE_MESSAGE}`)
    expect(metrics.textContent).toContain(`If O wins: ${PV_UNAVAILABLE_MESSAGE}`)
    expect(screen.getByText(PV_UNAVAILABLE_MESSAGE)).not.toBeNull()
    expect(PV_PIN).toEqual({ pinned: false, source: null })
  })

  it('renders both pre-auction retry recoveries with their auction in raw X then O order', () => {
    const { model } = renderFixture('double-fault-retry.jsonl')
    const row = screen.getByTestId('auction-row-0')
    const markers = screen.getByTestId('pre-recoveries-0')
    const markerText = [...markers.querySelectorAll('p')].map((node) => node.textContent)

    expect(model.auctions[0]?.recoveries.map((recovery) => recovery.seat)).toEqual(['X', 'O'])
    expect(markerText).toEqual([
      'recovery before ply 0: X · invalid_json · restart hello: stub',
      'recovery before ply 0: O · invalid_json · restart hello: stub',
    ])
    expectImmediatelyBefore(markers, row)
    expect(row.textContent).toContain('Attempts (2)')
  })

  it('attaches a single-fault recovery backward and renders it below its resolved auction', () => {
    const { model } = renderFixture('fault-single.jsonl')
    const row = screen.getByTestId('auction-row-0')
    const markers = screen.getByTestId('post-recoveries-0')

    expect(model.auctions[0]?.recoveries).toHaveLength(1)
    expect(model.auctions[1]?.recoveries).toEqual([])
    expect(screen.getByTestId('resolution-0').textContent).toContain('fault: illegal_move')
    expect(markers.textContent).toContain('recovery after ply 0: X · illegal_move · restart hello: stub')
    expectImmediatelyAfter(row, markers)
  })

  it('keeps a post-auction recovery at EOF attached instead of trailing', () => {
    const { model } = renderFixture('post-auction-recovery-eof.jsonl')

    expect(model.trailingRecoveries).toEqual([])
    expect(model.auctions[0]?.recoveries).toHaveLength(1)
    expect(screen.queryByTestId('trailing-recoveries')).toBeNull()
    expect(screen.getByTestId('post-recoveries-0').textContent)
      .toContain('restart hello: eof_or_crash fault')
    expectImmediatelyAfter(
      screen.getByTestId('auction-row-0'),
      screen.getByTestId('post-recoveries-0'),
    )
    expect(screen.getByText('log ends mid-game')).not.toBeNull()
  })

  it.each([
    ['trailing-recovery.jsonl', ['X']],
    ['trailing-recovery-xo.jsonl', ['X', 'O']],
  ] as const)('keeps %s trailing, ordered, and non-advancing', (path, seats) => {
    const { model } = renderFixture(path)
    const tail = screen.getByTestId('trailing-recoveries')

    expect(model.auctions).toEqual([])
    expect(model.positions).toHaveLength(1)
    expect(model.trailingRecoveries.map((recovery) => recovery.seat)).toEqual(seats)
    expect([...tail.querySelectorAll('p')].map((node) => node.textContent?.match(/^trailing recovery: ([XO])/)?.[1]))
      .toEqual(seats)
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 0')
    expect((screen.getByRole('button', { name: 'Next position' }) as HTMLButtonElement).disabled).toBe(true)
    expect(within(screen.getByRole('region', { name: 'UTTT board' })).getAllByRole('button', { name: /empty$/ }))
      .toHaveLength(81)
    expect(screen.queryByTestId('last-payment')).toBeNull()
  })

  it('renders the aborted recovery-fault terminal shape without advancing', () => {
    const { model } = renderFixture('recovery-fault-abort.jsonl')

    expect(model.auctions[0]?.outcome).toBe('aborted_recovery_fault')
    expect(model.positions).toHaveLength(1)
    expect(screen.getByTestId('resolution-0').textContent).toContain('aborted_recovery_fault')
    expect(screen.getByTestId('pre-recoveries-0').textContent).toContain('restart hello: eof_or_crash fault')
    expectImmediatelyBefore(
      screen.getByTestId('pre-recoveries-0'),
      screen.getByTestId('auction-row-0'),
    )
    expect(screen.getByTestId('terminal-recovery_fault').textContent).toBe('terminal: recovery_fault (O)')
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 0')
  })

  it('renders the resolved recovery-fault terminal shape and failed post-recovery annotation', () => {
    const { model } = renderFixture('recovery-fault-post-resolve.jsonl')

    expect(model.auctions[0]?.outcome).toBe('resolved')
    expect(model.positions).toHaveLength(2)
    expect(screen.getByTestId('resolution-0').textContent).toContain('fault: invalid_json')
    expect(screen.getByTestId('post-recoveries-0').textContent).toContain('restart hello: eof_or_crash fault')
    expectImmediatelyAfter(
      screen.getByTestId('auction-row-0'),
      screen.getByTestId('post-recoveries-0'),
    )
    expect(screen.getByTestId('terminal-recovery_fault').textContent).toBe('terminal: recovery_fault (O)')
    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 1 of 1')
  })

  it('renders complete both-zero metric totality through the composed GameView surface', () => {
    window.history.replaceState(null, '', '#cursor=35')
    const game = bothZeroGameWithAnalysis()
    const model = deriveReplayModel(game)
    const pending = model.auctions[35]
    if (pending === undefined) throw new Error('both-zero fixture must derive pending ply 35')

    expect(pending.pre.budgets).toEqual({ X: 0, O: 0 })
    render(createElement(GameView, { game }))

    const metrics = screen.getByRole('region', { name: 'analysis metrics' })
    expect(screen.getByTestId('budget-units').textContent).toBe('0 / 0 units')
    expect(screen.getByRole('status', { name: 'X budget share: n/a — both budgets exhausted' })).not.toBeNull()
    expect(screen.getByRole('status', { name: 'O budget share: n/a — both budgets exhausted' })).not.toBeNull()
    expect(metrics.textContent).toContain('T: n/a — both budgets exhausted')
    expect(metrics.textContent).toContain('p: n/a — both budgets exhausted')
    expect(metrics.textContent).toContain('margin p−T: n/a — both budgets exhausted')
    expect(metrics.textContent).toContain('critical bid: 0 units (n/a — both budgets exhausted)')
    expect(metrics.textContent).toContain(
      'interval [n/a — both budgets exhausted (0 units), n/a — both budgets exhausted (0 units)]',
    )
    expect(metrics.textContent).not.toContain('T: 62.50%')
    expect(metrics.textContent).not.toContain('interval [50.00%')
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toContain('Position 35 of')
  })

  it('renders both viewable truncation shapes and rejects the malformed interior shape', () => {
    const { rerender } = renderFixture('missing-game-end.jsonl')
    expect(screen.getByText('log ends mid-game')).not.toBeNull()

    const truncatedGame = fixtureGame('truncated-line.jsonl')
    expect(() => deriveReplayModel(truncatedGame)).not.toThrow()
    rerender(createElement(GameView, { game: truncatedGame }))
    expect(screen.getByText('log ends mid-game (truncated final line discarded)')).not.toBeNull()

    expect(() => fixtureGame('malformed-interior.jsonl')).toThrow(LogError)
  })

  it('keeps zero-auction hello faults at the setup terminal', () => {
    window.history.replaceState(null, '', '#cursor=9')
    const { model } = renderFixture('hello-fault.jsonl')

    expect(model.auctions).toEqual([])
    expect(model.positions).toHaveLength(1)
    expect(screen.getByTestId('terminal-hello_fault').textContent).toBe('terminal: hello_fault (O)')
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 0')
    for (const name of ['First position', 'Previous position', 'Next position', 'Last position']) {
      expect((screen.getByRole('button', { name }) as HTMLButtonElement).disabled).toBe(true)
    }
  })

  it('renders persisted logged closures at a later replay cursor', () => {
    window.history.replaceState(null, '', '#cursor=50')
    const { model } = renderFixture('success-macro-win.jsonl')

    expect(model.positions[45]?.closed).toEqual({ 0: 'X' })
    expect(model.positions[50]?.closed).toEqual({ 0: 'X', 5: 'O' })
    expect(screen.getByTestId('closed-local-0').textContent).toBe('X closed')
    expect(screen.getByTestId('closed-local-5').textContent).toBe('O closed')
  })

  it('keeps conditional ghosts on the pending pre-auction position only', () => {
    PV_PIN.pinned = true
    PV_PIN.source = 'test-only Task 15 acceptance seam'
    renderFixture('ghost-divergence.jsonl')

    expect(screen.getAllByTestId(/conditional-ghost-[XO]/)).toHaveLength(2)
    expect(within(screen.getByRole('button', { name: 'local 0, cell 0, empty' }))
      .getByTestId('conditional-ghost-X')).not.toBeNull()
    expect(within(screen.getByRole('button', { name: 'local 0, cell 1, empty' }))
      .getByTestId('conditional-ghost-O')).not.toBeNull()

    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))
    expect(screen.queryByTestId('conditional-ghost-X')).toBeNull()
    expect(screen.queryByTestId('conditional-ghost-O')).toBeNull()
  })

  it('contains no float-literal equality against budget values in production sources', () => {
    const budgetVariable = String.raw`(?:\b(?:units|combined|budget\w*|budgets?)\b(?:\s*\.\s*[A-Za-z_$][\w$]*)?)`
    const budgetFloatEquality = new RegExp(
      String.raw`${budgetVariable}\s*(?:===|==)\s*[+-]?\d+\.\d+|[+-]?\d+\.\d+\s*(?:===|==)\s*${budgetVariable}`,
    )
    const sourceRoot = resolve(import.meta.dirname, '../src')
    const sourceFiles = (directory: string): string[] => readdirSync(directory, { withFileTypes: true })
      .flatMap((entry) => {
        const path = resolve(directory, entry.name)
        return entry.isDirectory()
          ? sourceFiles(path)
          : path.endsWith('.ts') || path.endsWith('.tsx') ? [path] : []
      })

    const violations = sourceFiles(sourceRoot).filter((path) =>
      budgetFloatEquality.test(readFileSync(path, 'utf8')),
    )
    expect(violations).toEqual([])
  })
})
