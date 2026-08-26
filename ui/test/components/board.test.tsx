import { cleanup, render, screen } from '@testing-library/react'
import { afterEach, describe, expect, it } from 'vitest'

import { PV_UNAVAILABLE_MESSAGE } from '../../src/analysis/extract'
import { Board, ConditionalGhostBadges } from '../../src/components/Board'
import type { Position } from '../../src/replay/model'

const position = (overrides: Partial<Position> = {}): Position => ({
  board: Array.from({ length: 9 }, () => '.........'),
  budgets: { X: 1_000_000_000, O: 1_000_000_000 },
  forced: null,
  tieOwner: null,
  closed: {},
  ...overrides,
})

describe('Board', () => {
  afterEach(cleanup)

  it('renders 81 disabled cell buttons with the logged marks and exact accessible names', () => {
    render(<Board position={position({ board: ['X........', ...Array.from({ length: 8 }, () => '.........')] })} />)

    expect(screen.getAllByRole('button')).toHaveLength(81)
    expect(screen.getByRole('button', { name: 'local 0, cell 0, X' }).disabled).toBe(true)
    expect(screen.getByRole('button', { name: 'local 4, cell 2, empty' }).disabled).toBe(true)
  })

  it('uses only the logged closure map instead of recomputing an overlay from a winning board string', () => {
    render(<Board position={position({ board: ['XXX......', ...Array.from({ length: 8 }, () => '.........')] })} />)

    expect(screen.queryByTestId('closed-local-0')).toBeNull()
  })

  it('keeps an earlier logged closure overlaid at a later cursor', () => {
    render(<Board position={position({
      board: ['XXX......', ...Array.from({ length: 8 }, () => '.........')],
      closed: { 0: 'X', 5: 'full' },
    })} />)

    expect(screen.getByTestId('closed-local-0').textContent).toContain('X closed')
    expect(screen.getByTestId('closed-local-5').textContent).toContain('full closed')
  })

  it('marks the pending auction forced local board without inferring it from cells', () => {
    render(<Board position={position()} pending={{ forced: 4 }} />)

    expect(screen.getByTestId('local-board-4').getAttribute('data-forced')).toBe('true')
  })

  it('shows the just-resolved losing intent as a ghost by default and hides it when requested', () => {
    const annotations = { losingIntent: { seat: 'O' as const, move: [2, 6] as const } }
    const { rerender } = render(<Board position={position()} annotations={annotations} />)

    expect(screen.getByTestId('losing-intent-ghost').textContent).toBe('O')
    expect(screen.getByRole('button', {
      name: 'local 2, cell 6, empty, losing intent O',
    })).not.toBeNull()
    expect(screen.getByRole('button', { name: 'local 2, cell 5, empty' })).not.toBeNull()

    rerender(<Board position={position()} annotations={annotations} showLosingIntent={false} />)
    expect(screen.queryByTestId('losing-intent-ghost')).toBeNull()
    expect(screen.getByRole('button', { name: 'local 2, cell 6, empty' })).not.toBeNull()
  })

  it('keeps a losing intent in the timeline only when the current position occupies its cell', () => {
    render(<Board
      position={position({ board: ['.........', '.........', '......X..', ...Array.from({ length: 6 }, () => '.........')] })}
      annotations={{ losingIntent: { seat: 'O', move: [2, 6] } }}
    />)

    expect(screen.queryByTestId('losing-intent-ghost')).toBeNull()
    expect(screen.getByRole('button', { name: 'local 2, cell 6, X' })).not.toBeNull()
    expect(screen.queryByRole('button', { name: /local 2, cell 6, X, losing intent/i })).toBeNull()
  })

  it('keeps conditional ghosts unavailable until the harness artifact pin exists', () => {
    render(<Board position={position()} pending={{
      forced: 1,
      conditionalGhosts: { X: [0, 1], O: [8, 7] },
    }} />)

    expect(screen.queryByTestId('conditional-ghost-X')).toBeNull()
    expect(screen.queryByTestId('conditional-ghost-O')).toBeNull()
    expect(screen.getByText(PV_UNAVAILABLE_MESSAGE)).not.toBeNull()
  })

  it('renders seat-distinct badges and a single split badge for coincident enabled conditional ghosts', () => {
    const { rerender } = render(<ConditionalGhostBadges seats={['X', 'O']} />)

    expect(screen.getByTestId('conditional-ghost-X').className).toContain('board__ghost--X')
    expect(screen.getByTestId('conditional-ghost-O').className).toContain('board__ghost--O')

    rerender(<ConditionalGhostBadges seats={['X', 'O']} coincident />)
    expect(screen.queryByTestId('conditional-ghost-X')).toBeNull()
    expect(screen.queryByTestId('conditional-ghost-O')).toBeNull()
    expect(screen.getByTestId('conditional-ghost-split').textContent).toBe('X/O')
    expect(screen.getByTestId('conditional-ghost-split').className).toContain('board__ghost--split')
  })
})
