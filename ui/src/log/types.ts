export type Mark = 'X' | 'O'

export type FaultClass =
  | 'timeout'
  | 'eof_or_crash'
  | 'invalid_utf8'
  | 'invalid_json'
  | 'schema_violation'
  | 'wrong_request_id'
  | 'extra_protocol_line'
  | 'oversize_line'
  | 'illegal_bid'
  | 'illegal_move'

export type Validation = 'ok' | FaultClass

export interface RawRecord {
  b64: string
  truncated: boolean
  bytes_total: number
  [key: string]: unknown
}

export interface OkHelloRecord {
  validation: 'ok'
  elapsed_ms: number
  name: string
  version: string
  author?: string
  raw?: never
  [key: string]: unknown
}

export interface FaultHelloRecord {
  validation: FaultClass
  elapsed_ms: number
  name?: string
  version?: string
  author?: string
  raw: RawRecord
  [key: string]: unknown
}

export type HelloRecord = OkHelloRecord | FaultHelloRecord

export type Move = [number, number]

export interface OkTurnRecord {
  validation: 'ok'
  elapsed_ms: number
  bid: number
  move: Move
  info?: object
  raw?: never
  [key: string]: unknown
}

export interface FaultTurnRecord {
  validation: FaultClass
  elapsed_ms: number
  bid?: number
  move?: Move
  info?: object
  raw: RawRecord
  [key: string]: unknown
}

export type TurnRecord = OkTurnRecord | FaultTurnRecord

export interface Attempt {
  request_id: string
  attempt: number
  turns: Record<Mark, TurnRecord>
  [key: string]: unknown
}

export interface Budgets {
  X: number
  O: number
  [key: string]: unknown
}

export interface EngineRecord {
  engine_id: string
  cmd: string[]
  [key: string]: unknown
}

export interface Closure {
  local: number
  result: Mark | 'full'
  [key: string]: unknown
}

export interface Resolution {
  winner: Mark
  reason: 'higher_bid' | 'tie_last_mover' | 'tie_coin' | 'fault'
  coin?: Mark | null
  payment: number
  move: Move
  closures: Closure[]
  macro_line: [number, number, number] | null
  forced_next: number | null
  [key: string]: unknown
}

interface AuctionBase {
  event: 'auction'
  ply: number
  attempts: Attempt[]
  budgets_after: Budgets
  post_board: string[]
  [key: string]: unknown
}

export interface ResolvedAuctionEvent extends AuctionBase {
  outcome: 'resolved'
  resolution: Resolution
}

export interface UnresolvedAuctionEvent extends AuctionBase {
  outcome: 'voided' | 'aborted_recovery_fault'
  resolution?: never
}

export type AuctionEvent = ResolvedAuctionEvent | UnresolvedAuctionEvent

export interface GameStartEvent {
  event: 'game_start'
  log_version: 1
  protocol: 1
  rules: 'poorman-uttt-v1'
  game_id: string
  tournament_id: string | null
  engines: Record<Mark, EngineRecord>
  pair_seed: string
  game_seed: string
  pair_coin_seat: Mark
  hellos: Record<Mark, HelloRecord>
  time_control: {
    time_ms: number
    grace_ms: number
    hello_timeout_ms: number
    shutdown_grace_ms: number
    [key: string]: unknown
  }
  budgets: Budgets
  [key: string]: unknown
}

export interface RecoveryEvent {
  event: 'recovery'
  seat: Mark
  ply: number
  trigger_request_id: string
  fault: FaultClass
  hello: HelloRecord
  [key: string]: unknown
}

export interface StderrRecord {
  path: string
  bytes_total: number
  truncated: boolean
  [key: string]: unknown
}

export interface GameEndEvent {
  event: 'game_end'
  result: Mark | 'draw' | 'void'
  reason:
    | 'macro_win'
    | 'chip_count'
    | 'exact_tie_draw'
    | 'hello_fault'
    | 'recovery_fault'
    | 'triple_double_fault_void'
  budgets: Budgets
  budget_margin: number
  plies: number
  delivery: Record<Mark, 'ok' | 'failed'>
  stderr?: Record<Mark, StderrRecord>
  [key: string]: unknown
}

export type LogEvent =
  | GameStartEvent
  | AuctionEvent
  | RecoveryEvent
  | GameEndEvent
