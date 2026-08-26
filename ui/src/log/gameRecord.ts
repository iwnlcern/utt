import type { GameEndEvent, GameStartEvent, LogEvent } from './types'

export interface GameRecord {
  start: GameStartEvent
  events: LogEvent[]
  end?: GameEndEvent
  truncated: boolean
}
