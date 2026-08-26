import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { describe, expect, it } from 'vitest'

import { deriveReplayModel } from '../../src/replay/derive'
import type { ResolvedAuctionEvent } from '../../src/log/types'
import { LogError, parseGameLog } from '../../src/log/validate'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

const resolvedAuctions = (text: string) => parseGameLog(text).events.filter(
  (event): event is ResolvedAuctionEvent => event.event === 'auction' && event.outcome === 'resolved',
)

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
})
