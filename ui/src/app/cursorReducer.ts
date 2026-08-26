import type { Mark } from '../log/types'

export interface CursorState {
  cursor: number
  maxCursor: number
  revealVersion: number
  showLosingIntent: boolean
  preferredAnalysisSeat: Mark | null
}

export type CursorAction =
  | { type: 'step'; delta: number }
  | { type: 'jump'; cursor: number }
  | { type: 'hydrate'; hash: string }
  | { type: 'toggle-losing-intent' }
  | { type: 'select-preferred-analysis-seat'; seat: Mark }

function clamp(cursor: number, maxCursor: number): number {
  return Math.min(Math.max(cursor, 0), maxCursor)
}

function cursorFromHash(hash: string): number {
  const match = /^#cursor=(-?\d+)$/.exec(hash)
  return match === null ? 0 : Number(match[1])
}

export function createCursorState(maxCursor: number, hash: string): CursorState {
  const normalizedMax = Number.isInteger(maxCursor) && maxCursor > 0 ? maxCursor : 0
  return {
    cursor: clamp(cursorFromHash(hash), normalizedMax),
    maxCursor: normalizedMax,
    revealVersion: 0,
    showLosingIntent: true,
    preferredAnalysisSeat: null,
  }
}

export function cursorReducer(state: CursorState, action: CursorAction): CursorState {
  if (action.type === 'toggle-losing-intent') {
    return { ...state, showLosingIntent: !state.showLosingIntent }
  }

  if (action.type === 'select-preferred-analysis-seat') {
    return { ...state, preferredAnalysisSeat: action.seat }
  }

  if (action.type === 'hydrate') {
    return { ...state, cursor: clamp(cursorFromHash(action.hash), state.maxCursor) }
  }

  const requested = action.type === 'step' ? state.cursor + action.delta : action.cursor
  const cursor = clamp(requested, state.maxCursor)
  const reveals = action.type === 'step' && cursor > state.cursor

  return {
    ...state,
    cursor,
    revealVersion: reveals ? state.revealVersion + 1 : state.revealVersion,
  }
}
