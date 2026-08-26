import { splitJsonl } from './jsonl'
import type { GameRecord } from './gameRecord'
import type { LogEvent } from './types'

type Value = Record<string, unknown>

const faults = new Set([
  'timeout', 'eof_or_crash', 'invalid_utf8', 'invalid_json', 'schema_violation',
  'wrong_request_id', 'extra_protocol_line', 'oversize_line', 'illegal_bid', 'illegal_move',
])

export class LogError extends Error {
  readonly line: number
  readonly event_index: number
  readonly reason: string

  constructor(line: number, event_index: number, reason: string) {
    super(reason)
    this.name = 'LogError'
    this.line = line
    this.event_index = event_index
    this.reason = reason
  }
}

const object = (value: unknown): value is Value =>
  typeof value === 'object' && value !== null && !Array.isArray(value)
const string = (value: unknown): value is string => typeof value === 'string'
const bool = (value: unknown): value is boolean => typeof value === 'boolean'
const integer = (value: unknown, minimum = Number.NEGATIVE_INFINITY, maximum = Number.POSITIVE_INFINITY): boolean =>
  typeof value === 'number' && Number.isInteger(value) && value >= minimum && value <= maximum
const oneOf = (value: unknown, values: readonly string[]): boolean => string(value) && values.includes(value)
const has = (value: Value, key: string): boolean => Object.hasOwn(value, key)

function seatMap(value: unknown, item: (item: unknown) => boolean): boolean {
  return object(value) && has(value, 'X') && has(value, 'O') && item(value.X) && item(value.O)
}

const budgets = (value: unknown) => seatMap(value, (item) => integer(item, 0))
const move = (value: unknown): boolean =>
  Array.isArray(value) && value.length === 2 && value.every((cell) => integer(cell, 0, 8))
const raw = (value: unknown): boolean =>
  object(value) && string(value.b64) && bool(value.truncated) && integer(value.bytes_total, 0)

function hello(value: unknown): boolean {
  if (!object(value) || !oneOf(value.validation, ['ok', ...faults])) return false
  if (!integer(value.elapsed_ms, 0)) return false
  if (has(value, 'name') && !string(value.name)) return false
  if (has(value, 'version') && !string(value.version)) return false
  if (has(value, 'author') && !string(value.author)) return false
  return value.validation === 'ok'
    ? string(value.name) && string(value.version) && !has(value, 'raw')
    : raw(value.raw)
}

function turn(value: unknown): boolean {
  if (!object(value) || !oneOf(value.validation, ['ok', ...faults]) || !integer(value.elapsed_ms, 0)) return false
  if (has(value, 'bid') && !integer(value.bid, 0)) return false
  if (has(value, 'move') && !move(value.move)) return false
  if (has(value, 'info') && !object(value.info)) return false
  return value.validation === 'ok'
    ? integer(value.bid, 0) && move(value.move) && !has(value, 'raw')
    : raw(value.raw)
}

function engine(value: unknown): boolean {
  return object(value) && string(value.engine_id) && Array.isArray(value.cmd) && value.cmd.length > 0 && value.cmd.every(string)
}

function attempt(value: unknown): boolean {
  return object(value) && string(value.request_id) && integer(value.attempt, 1, 3) && seatMap(value.turns, turn)
}

function resolution(value: unknown): boolean {
  if (!object(value) || !oneOf(value.winner, ['X', 'O']) || !oneOf(value.reason, ['higher_bid', 'tie_last_mover', 'tie_coin', 'fault'])) return false
  if (has(value, 'coin') && value.coin !== null && !oneOf(value.coin, ['X', 'O'])) return false
  if (!integer(value.payment, 0) || !move(value.move) || !Array.isArray(value.closures)) return false
  if (!value.closures.every((closure) => object(closure) && integer(closure.local, 0, 8) && oneOf(closure.result, ['X', 'O', 'full']))) return false
  if (value.macro_line !== null && !(Array.isArray(value.macro_line) && value.macro_line.length === 3 && value.macro_line.every((cell) => integer(cell, 0, 8)))) return false
  return value.forced_next === null || integer(value.forced_next, 0, 8)
}

function gameStart(value: Value): boolean {
  return value.event === 'game_start' && value.log_version === 1 && value.protocol === 1 && value.rules === 'poorman-uttt-v1'
    && string(value.game_id) && (value.tournament_id === null || string(value.tournament_id))
    && seatMap(value.engines, engine) && string(value.pair_seed) && /^[0-9a-f]{64}$/.test(value.pair_seed)
    && string(value.game_seed) && /^[0-9a-f]{64}$/.test(value.game_seed) && oneOf(value.pair_coin_seat, ['X', 'O'])
    && seatMap(value.hellos, hello) && object(value.time_control)
    && integer(value.time_control.time_ms, 0) && integer(value.time_control.grace_ms, 0)
    && integer(value.time_control.hello_timeout_ms, 0) && integer(value.time_control.shutdown_grace_ms, 0) && budgets(value.budgets)
}

function auction(value: Value): boolean {
  if (value.event !== 'auction' || !integer(value.ply, 0) || !oneOf(value.outcome, ['resolved', 'voided', 'aborted_recovery_fault'])) return false
  if (!Array.isArray(value.attempts) || value.attempts.length === 0 || !value.attempts.every(attempt) || !budgets(value.budgets_after)) return false
  if (!Array.isArray(value.post_board) || value.post_board.length !== 9 || !value.post_board.every((row) => string(row) && /^[.XO]{9}$/.test(row))) return false
  return value.outcome === 'resolved' ? has(value, 'resolution') && resolution(value.resolution) : !has(value, 'resolution')
}

function recovery(value: Value): boolean {
  return value.event === 'recovery' && oneOf(value.seat, ['X', 'O']) && integer(value.ply, 0)
    && string(value.trigger_request_id) && string(value.fault) && faults.has(value.fault) && hello(value.hello)
}

function gameEnd(value: Value): boolean {
  const stderr = (item: unknown) => object(item) && string(item.path) && integer(item.bytes_total, 0) && bool(item.truncated)
  return value.event === 'game_end' && oneOf(value.result, ['X', 'O', 'draw', 'void'])
    && oneOf(value.reason, ['macro_win', 'chip_count', 'exact_tie_draw', 'hello_fault', 'recovery_fault', 'triple_double_fault_void'])
    && budgets(value.budgets) && integer(value.budget_margin) && integer(value.plies, 0)
    && seatMap(value.delivery, (item) => oneOf(item, ['ok', 'failed'])) && (!has(value, 'stderr') || seatMap(value.stderr, stderr))
}

function validEvent(value: unknown): value is LogEvent {
  return object(value) && (gameStart(value) || auction(value) || recovery(value) || gameEnd(value))
}

export function parseGameLog(text: string): GameRecord {
  const split = splitJsonl(text)
  if (split.lines.length === 0) throw new LogError(0, 0, 'first event must be game_start')

  const events: LogEvent[] = []
  for (let index = 0; index < split.lines.length; index++) {
    const line = split.lines[index]
    let parsed: unknown
    try {
      parsed = JSON.parse(line.raw)
    } catch {
      throw new LogError(line.line, index, 'invalid JSON event')
    }
    if (index === 0 && object(parsed) && parsed.event === 'game_start' && parsed.log_version !== 1) {
      throw new LogError(line.line, index, 'unsupported log version')
    }
    if (!validEvent(parsed)) throw new LogError(line.line, index, 'event fails schema validation')
    events.push(parsed)
  }

  const start = events[0]
  if (start.event !== 'game_start') throw new LogError(split.lines[0].line, 0, 'first event must be game_start')
  if (start.log_version !== 1) throw new LogError(split.lines[0].line, 0, 'unsupported log version')

  const end = events.findLast((event) => event.event === 'game_end')
  return { start, events, end, truncated: split.discardedTail !== null || end === undefined }
}
