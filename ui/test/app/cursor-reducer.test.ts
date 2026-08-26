import { describe, expect, it } from 'vitest'

import {
  createCursorState,
  cursorReducer,
  type CursorAction,
  type CursorState,
} from '../../src/app/cursorReducer'

const reduce = (state: CursorState, action: CursorAction) => cursorReducer(state, action)

describe('cursorReducer', () => {
  it('hydrates a valid cursor hash and clamps deep links to the replay range', () => {
    expect(createCursorState(12, '#cursor=7')).toMatchObject({ cursor: 7, maxCursor: 12 })
    expect(createCursorState(12, '#cursor=99')).toMatchObject({ cursor: 12, maxCursor: 12 })
    expect(createCursorState(12, '#cursor=-4')).toMatchObject({ cursor: 0, maxCursor: 12 })
  })

  it('falls back to position zero for malformed or unrelated hashes', () => {
    expect(createCursorState(12, '#cursor=3.5').cursor).toBe(0)
    expect(createCursorState(12, '#cursor=words').cursor).toBe(0)
    expect(createCursorState(12, '#other=8').cursor).toBe(0)
  })

  it('clamps steps and direct jumps to zero through R', () => {
    const middle = createCursorState(8, '#cursor=3')

    expect(reduce(middle, { type: 'step', delta: -9 }).cursor).toBe(0)
    expect(reduce(middle, { type: 'step', delta: 99 }).cursor).toBe(8)
    expect(reduce(middle, { type: 'jump', cursor: -1 }).cursor).toBe(0)
    expect(reduce(middle, { type: 'jump', cursor: 9 }).cursor).toBe(8)
  })

  it('keeps every action at zero for a zero-auction replay', () => {
    const setupOnly = createCursorState(0, '#cursor=42')

    expect(reduce(setupOnly, { type: 'step', delta: 1 }).cursor).toBe(0)
    expect(reduce(setupOnly, { type: 'jump', cursor: 5 }).cursor).toBe(0)
    expect(reduce(setupOnly, { type: 'hydrate', hash: '#cursor=9' }).cursor).toBe(0)
  })

  it('increments the reveal version only when a step moves forward', () => {
    const start = createCursorState(2, '#cursor=0')
    const forward = reduce(start, { type: 'step', delta: 1 })

    expect(forward).toMatchObject({ cursor: 1, revealVersion: 1 })
    expect(reduce(forward, { type: 'step', delta: -1 })).toMatchObject({ cursor: 0, revealVersion: 1 })
    expect(reduce(createCursorState(0, ''), { type: 'step', delta: 1 }).revealVersion).toBe(0)
  })

  it('owns losing-intent visibility and the remembered analysis seat without changing the cursor', () => {
    const start = createCursorState(8, '#cursor=3')

    expect(start).toMatchObject({
      cursor: 3,
      showLosingIntent: true,
      preferredAnalysisSeat: null,
    })

    const hidden = reduce(start, { type: 'toggle-losing-intent' })
    expect(hidden).toMatchObject({ cursor: 3, showLosingIntent: false, preferredAnalysisSeat: null })

    const preferredO = reduce(hidden, { type: 'select-preferred-analysis-seat', seat: 'O' })
    expect(preferredO).toMatchObject({ cursor: 3, showLosingIntent: false, preferredAnalysisSeat: 'O' })
  })
})
