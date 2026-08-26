import type { GameRecord } from '../log/gameRecord'

export const RECENTS_STORAGE_KEY = 'poorman-uttt-replay.recents.v1'

export type RecentResult = 'X' | 'O' | 'draw' | 'void' | 'incomplete'

export type RecentGame = {
  name: string
  opened_at: string
  result: RecentResult
  log_text: string
}

type SaveRecentsResult = {
  recents: RecentGame[]
  persisted: boolean
}

let sessionOnlyRecents: RecentGame[] | null = null

const isRecentResult = (value: unknown): value is RecentResult =>
  value === 'X' || value === 'O' || value === 'draw' || value === 'void' || value === 'incomplete'

const isRecentGame = (value: unknown): value is RecentGame => {
  if (typeof value !== 'object' || value === null || Array.isArray(value)) return false

  const candidate = value as Record<string, unknown>
  return typeof candidate.name === 'string'
    && typeof candidate.opened_at === 'string'
    && isRecentResult(candidate.result)
    && typeof candidate.log_text === 'string'
}

const storage = (): Storage | null => typeof window === 'undefined' ? null : window.localStorage

export function readRecents(): RecentGame[] {
  if (sessionOnlyRecents !== null) return sessionOnlyRecents

  try {
    const saved = storage()?.getItem(RECENTS_STORAGE_KEY)
    if (saved === null || saved === undefined) return []
    const parsed: unknown = JSON.parse(saved)
    return Array.isArray(parsed) && parsed.every(isRecentGame) ? parsed : []
  } catch {
    return []
  }
}

export function recentForGame(name: string, logText: string, game: GameRecord, openedAt = new Date().toISOString()): RecentGame {
  return {
    name,
    opened_at: openedAt,
    result: game.end?.result ?? 'incomplete',
    log_text: logText,
  }
}

export function saveRecent(recent: RecentGame): SaveRecentsResult {
  const recents = [recent, ...readRecents().filter((item) => item.name !== recent.name)]

  try {
    storage()?.setItem(RECENTS_STORAGE_KEY, JSON.stringify(recents))
    sessionOnlyRecents = null
    return { recents, persisted: true }
  } catch {
    sessionOnlyRecents = recents
    return { recents, persisted: false }
  }
}

export function clearRecents(): void {
  sessionOnlyRecents = null
  try {
    storage()?.removeItem(RECENTS_STORAGE_KEY)
  } catch {
    // The UI state is still clear when a browser blocks storage mutation.
  }
}
