import type {
  Attempt,
  GameEndEvent,
  GameStartEvent,
  Mark,
  RecoveryEvent,
  Resolution,
} from '../log/types'

export interface Position {
  board: readonly string[]
  budgets: Readonly<Record<Mark, number>>
  forced: number | null
  tieOwner: Mark | null
  closed: Readonly<Record<number, Mark | 'full'>>
}

export interface SetupStep {
  start: GameStartEvent
}

export interface AuctionStep {
  ply: number
  pre: Position
  attempts: Attempt[]
  recoveries: RecoveryEvent[]
  outcome: 'resolved' | 'voided' | 'aborted_recovery_fault'
  resolution?: Resolution
  post?: Position
}

export interface ReplayModel {
  setup: SetupStep
  auctions: AuctionStep[]
  trailingRecoveries: RecoveryEvent[]
  terminal?: GameEndEvent
  positions: Position[]
  truncated: boolean
}
