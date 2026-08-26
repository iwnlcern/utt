import type {
  Attempt,
  GameEndEvent,
  GameStartEvent,
  Mark,
  RecoveryEvent,
  Resolution,
} from '../log/types'
import type { Truncation } from '../log/gameRecord'

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

interface AuctionStepBase {
  ply: number
  pre: Position
  attempts: Attempt[]
  recoveries: RecoveryEvent[]
}

export interface ResolvedAuctionStep extends AuctionStepBase {
  outcome: 'resolved'
  resolution: Resolution
  post: Position
}

export interface UnresolvedAuctionStep extends AuctionStepBase {
  outcome: 'voided' | 'aborted_recovery_fault'
  resolution?: never
  post?: never
}

export type AuctionStep = ResolvedAuctionStep | UnresolvedAuctionStep

export interface ReplayModel {
  setup: SetupStep
  auctions: AuctionStep[]
  trailingRecoveries: RecoveryEvent[]
  terminal?: GameEndEvent
  positions: Position[]
  truncated: boolean
  truncation: Truncation
}
