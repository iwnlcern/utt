import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { deriveReplayModel } from '../../src/replay/derive'
import type { AuctionEvent, GameEndEvent, ResolvedAuctionEvent } from '../../src/log/types'
import { LogError, parseGameLog } from '../../src/log/validate'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

const resolvedAuctions = (text: string) => parseGameLog(text).events.filter(
  (event): event is ResolvedAuctionEvent => event.event === 'auction' && event.outcome === 'resolved',
)

const fixtureRecord = (name: string) => parseGameLog(fixtureText(name))

describe('deriveReplayModel', () => {
  it('rejects a non-sequential wire ply with its event coordinate', () => {
    const record = parseGameLog(fixtureText('success-macro-win.jsonl').replace('"ply":1', '"ply":2'))

    expect(() => deriveReplayModel(record)).toThrow(
      expect.objectContaining({
        line: 3,
        event_index: 2,
        reason: 'non-sequential auction ply at event 2: expected 1, received 2',
      } satisfies Partial<LogError>),
    )
  })

  it('narrows a resolved derived step to its required resolution and post-position', () => {
    const step = deriveReplayModel(parseGameLog(fixtureText('success-macro-win.jsonl'))).auctions[0]

    expect(step).toBeDefined()
    if (step === undefined || step.outcome !== 'resolved') throw new Error('fixture must start with a resolved auction')

    expect(step.resolution.winner).toBe('X')
    expect(step.post.board).toEqual([
      '.........', '.........', '.........',
      '.........', '...X.....', '.........',
      '.........', '.........', '.........',
    ])
  })

  it('folds resolved fixture positions from logged fields and persists logged closures', () => {
    const text = fixtureText('success-macro-win.jsonl')
    const record = parseGameLog(text)
    const auctions = resolvedAuctions(text)
    const model = deriveReplayModel(record)

    expect(model.positions).toHaveLength(auctions.length + 1)
    expect(model.positions[0]).toEqual({
      board: [
        '.........', '.........', '.........',
        '.........', '.........', '.........',
        '.........', '.........', '.........',
      ],
      budgets: { X: 1_000_000_000, O: 1_000_000_000 },
      forced: 4,
      tieOwner: null,
      closed: {},
    })

    for (const [index, auction] of auctions.entries()) {
      const position = model.positions[index + 1]
      expect(position.board).toEqual(auction.post_board)
      expect(position.budgets).toEqual(auction.budgets_after)
      expect(position.forced).toBe(auction.resolution.forced_next)
      expect(position.tieOwner).toBe(auction.resolution.winner === 'X' ? 'O' : 'X')
    }

    expect(model.positions[45].closed).toEqual({ 0: 'X' })
    expect(model.positions[50].closed).toEqual({ 0: 'X', 5: 'O' })
    expect(model.positions.at(-1)?.closed).toEqual({
      0: 'X', 1: 'X', 2: 'X', 3: 'X', 4: 'O', 5: 'O', 6: 'O', 7: 'X', 8: 'X',
    })
  })

  it.each([
    ['chip-count.jsonl', { result: 'X', reason: 'chip_count' }],
    ['exact-tie-draw.jsonl', { result: 'draw', reason: 'exact_tie_draw' }],
  ] as const)('carries the %s terminal verbatim', (fixture, terminal) => {
    const record = parseGameLog(fixtureText(fixture))
    const model = deriveReplayModel(record)

    expect(model.terminal).toBe(record.end)
    expect(model.terminal).toMatchObject(terminal)
  })

  it('attaches both double-fault recoveries to the retrying auction in raw logged order', () => {
    const model = deriveReplayModel(fixtureRecord('double-fault-retry.jsonl'))
    const step = model.auctions[0]

    expect(step).toBeDefined()
    expect(step?.outcome).toBe('resolved')
    expect(step?.attempts).toHaveLength(2)
    expect(step?.attempts[1]?.attempt).toBe(2)
    expect(step?.recoveries.map(({ seat, ply, fault }) => ({ seat, ply, fault }))).toEqual([
      { seat: 'X', ply: 0, fault: 'invalid_json' },
      { seat: 'O', ply: 0, fault: 'invalid_json' },
    ])
  })

  it('attaches a single-fault recovery backward to its resolved auction rather than the next ply', () => {
    const model = deriveReplayModel(fixtureRecord('fault-single.jsonl'))

    expect(model.auctions[0]?.recoveries).toHaveLength(1)
    expect(model.auctions[0]?.recoveries[0]).toMatchObject({
      seat: 'X', ply: 0, fault: 'illegal_move',
    })
    expect(model.auctions[1]?.recoveries).toEqual([])
  })

  it('fails closed when a recovery trigger request id does not belong to its auction ply', () => {
    const events = fixtureText('fault-single.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const recovery = events.find((event) => event.event === 'recovery')
    recovery.trigger_request_id = 'stale-request-id'
    const record = parseGameLog(`${events.map(JSON.stringify).join('\n')}\n`)

    expect(() => deriveReplayModel(record)).toThrow(
      expect.objectContaining({
        line: 3,
        event_index: 2,
        reason: 'recovery trigger_request_id does not match an attempt at ply 0',
      } satisfies Partial<LogError>),
    )
  })

  it('retains a voided auction without advancing the replay position', () => {
    const model = deriveReplayModel(fixtureRecord('void-triple-double-fault.jsonl'))
    const finalStep = model.auctions.at(-1)

    expect(finalStep).toBeDefined()
    expect(finalStep?.outcome).toBe('voided')
    expect(finalStep).not.toHaveProperty('post')
    expect(model.positions).toHaveLength(model.auctions.length)
    expect(model.positions.at(-1)).toEqual(finalStep?.pre)
  })

  it('represents a recovery fault before resolution as an aborted auction with its terminal', () => {
    const model = deriveReplayModel(fixtureRecord('recovery-fault-abort.jsonl'))
    const finalStep = model.auctions.at(-1)

    expect(finalStep?.outcome).toBe('aborted_recovery_fault')
    expect(finalStep).not.toHaveProperty('post')
    expect(finalStep?.recoveries).toHaveLength(2)
    expect(model.terminal).toMatchObject({ reason: 'recovery_fault' })
  })

  it('keeps the resolved position when the attached post-resolution recovery faults', () => {
    const model = deriveReplayModel(fixtureRecord('recovery-fault-post-resolve.jsonl'))
    const finalStep = model.auctions.at(-1)

    expect(finalStep?.outcome).toBe('resolved')
    expect(finalStep?.recoveries).toHaveLength(1)
    expect(finalStep?.recoveries[0]).toMatchObject({ fault: 'invalid_json' })
    expect(model.positions).toHaveLength(model.auctions.length + 1)
    expect(model.terminal).toMatchObject({ reason: 'recovery_fault' })
  })

  it('derives a terminal hello fault without inventing an auction or trailing recovery', () => {
    const model = deriveReplayModel(fixtureRecord('hello-fault.jsonl'))

    expect(model.auctions).toEqual([])
    expect(model.positions).toHaveLength(1)
    expect(model.trailingRecoveries).toEqual([])
    expect(model.terminal).toMatchObject({ reason: 'hello_fault' })
  })

  it('keeps an unpaired recovery tail intact and marks the model truncated', () => {
    const model = deriveReplayModel(fixtureRecord('trailing-recovery.jsonl'))

    expect(model.trailingRecoveries).toHaveLength(1)
    expect(model.trailingRecoveries[0]).toMatchObject({
      seat: 'X', ply: 0, fault: 'invalid_json',
    })
    expect(model.trailingRecoveries[0]?.trigger_request_id).toBeTruthy()
    expect(model.trailingRecoveries[0]?.hello).toBeDefined()
    expect(model.truncated).toBe(true)
    expect(model.truncation).toBe('missing_game_end')
  })

  it('forwards discarded-final-line provenance from the validated record', () => {
    const model = deriveReplayModel(fixtureRecord('truncated-line.jsonl'))

    expect(model.truncated).toBe(true)
    expect(model.truncation).toBe('discarded_final_line')
  })

  it('preserves X then O order for multiple trailing recoveries', () => {
    const model = deriveReplayModel(fixtureRecord('trailing-recovery-xo.jsonl'))

    expect(model.trailingRecoveries.map((recovery) => recovery.seat)).toEqual(['X', 'O'])
  })

  it('attaches a post-auction recovery at EOF rather than misclassifying it as trailing', () => {
    const model = deriveReplayModel(fixtureRecord('post-auction-recovery-eof.jsonl'))

    expect(model.trailingRecoveries).toEqual([])
    expect(model.auctions.at(-1)?.recoveries).toHaveLength(1)
    expect(model.auctions.at(-1)?.recoveries[0]).toMatchObject({
      seat: 'X', fault: 'invalid_json',
    })
  })

  it('is total over every auction outcome and game-end reason represented by the fixture corpus', () => {
    const fixtures = [
      'success-macro-win.jsonl',
      'chip-count.jsonl',
      'exact-tie-draw.jsonl',
      'hello-fault.jsonl',
      'void-triple-double-fault.jsonl',
      'recovery-fault-abort.jsonl',
    ]
    const outcomes = new Set<string>()
    const reasons = new Set<string>()

    for (const fixture of fixtures) {
      const record = fixtureRecord(fixture)
      expect(() => deriveReplayModel(record)).not.toThrow()
      for (const event of record.events) {
        if (event.event === 'auction') outcomes.add(event.outcome)
      }
      if (record.end !== undefined) reasons.add(record.end.reason)
    }

    expect(outcomes).toEqual(new Set<AuctionEvent['outcome']>([
      'resolved', 'voided', 'aborted_recovery_fault',
    ]))
    expect(reasons).toEqual(new Set<GameEndEvent['reason']>([
      'macro_win', 'chip_count', 'exact_tie_draw', 'hello_fault', 'recovery_fault', 'triple_double_fault_void',
    ]))
  })
})
