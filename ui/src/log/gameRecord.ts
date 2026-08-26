import type { GameEndEvent, GameStartEvent, LogEvent } from './types'

export type Truncation = 'discarded_final_line' | 'missing_game_end' | null

export interface GameRecord {
  start: GameStartEvent
  events: LogEvent[]
  end?: GameEndEvent
  truncated: boolean
  truncation: Truncation
}
