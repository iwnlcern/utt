import type { GameRecord } from '../log/gameRecord'
import type { Closure, Mark, RecoveryEvent } from '../log/types'
import { LogError } from '../log/validate'
import type { AuctionStep, Position, ReplayModel } from './model'

const EMPTY_BOARD = [
  '.........', '.........', '.........',
  '.........', '.........', '.........',
  '.........', '.........', '.........',
]

const opponent = (mark: Mark): Mark => mark === 'X' ? 'O' : 'X'

export function deriveReplayModel(game: GameRecord): ReplayModel {
  const positions: Position[] = [{
    board: EMPTY_BOARD,
    budgets: { X: game.start.budgets.X, O: game.start.budgets.O },
    forced: 4,
    tieOwner: null,
    closed: {},
  }]
  const auctions: AuctionStep[] = []
  const recoveries = game.events.flatMap((event, eventIndex) => event.event === 'recovery'
    ? [{ recovery: event, eventIndex }]
    : [],
  )
  let expectedPly = 0
  let unresolvedPly: number | undefined
  const requestIds = new Set<string>()

  for (const [eventIndex, event] of game.events.entries()) {
    if (event.event !== 'auction') continue

    if (unresolvedPly !== undefined) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `auction at event ${eventIndex} follows unresolved ply ${unresolvedPly}; an unresolved auction must be final`,
      )
    }
    if (event.ply !== expectedPly) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `non-sequential auction ply at event ${eventIndex}: expected ${expectedPly}, received ${event.ply}`,
      )
    }
    expectedPly += 1

    for (const [attemptIndex, attempt] of event.attempts.entries()) {
      const expectedAttempt = attemptIndex + 1
      if (attempt.attempt !== expectedAttempt) {
        throw new LogError(
          eventIndex + 1,
          eventIndex,
          `attempt ordinals must be 1..N in logged order at event ${eventIndex}: expected ${expectedAttempt}, received ${attempt.attempt}`,
        )
      }
      if (requestIds.has(attempt.request_id)) {
        throw new LogError(
          eventIndex + 1,
          eventIndex,
          `attempt request_id values must be unique at event ${eventIndex}: duplicate ${attempt.request_id}`,
        )
      }
      requestIds.add(attempt.request_id)
    }

    const pre = positions.at(-1)
    if (pre === undefined) throw new Error('replay positions must start with position_0')

    const stepRecoveries: RecoveryEvent[] = []
    for (const { recovery, eventIndex: recoveryEventIndex } of recoveries) {
      if (recovery.ply !== event.ply) continue
      if (!event.attempts.some((attempt) => attempt.request_id === recovery.trigger_request_id)) {
        throw new LogError(
          recoveryEventIndex + 1,
          recoveryEventIndex,
          `recovery trigger_request_id does not match an attempt at ply ${event.ply}`,
        )
      }
      stepRecoveries.push(recovery)
    }

    if (event.outcome !== 'resolved') {
      auctions.push({
        ply: event.ply,
        pre,
        attempts: event.attempts,
        recoveries: stepRecoveries,
        outcome: event.outcome,
      })
      unresolvedPly = event.ply
      continue
    }

    const closed: Record<number, Closure['result']> = { ...pre.closed }
    for (const closure of event.resolution.closures) closed[closure.local] = closure.result

    const post: Position = {
      board: event.post_board,
      budgets: event.budgets_after,
      forced: event.resolution.forced_next,
      tieOwner: opponent(event.resolution.winner),
      closed,
    }
    positions.push(post)
    auctions.push({
      ply: event.ply,
      pre,
      attempts: event.attempts,
      recoveries: stepRecoveries,
      outcome: event.outcome,
      resolution: event.resolution,
      post,
    })
  }

  const trailingRecoveryRecords = recoveries.filter(
    ({ recovery }) => !auctions.some((auction) => auction.ply === recovery.ply),
  )
  for (const { recovery, eventIndex } of trailingRecoveryRecords) {
    if (game.end !== undefined) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `recovery at event ${eventIndex} references missing auction ply ${recovery.ply} in a complete log`,
      )
    }
    if (recovery.ply !== expectedPly) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `recovery at event ${eventIndex} references missing auction ply ${recovery.ply}; next expected absent auction ply is ${expectedPly}`,
      )
    }
  }

  let trailingTriggerRequestId: string | undefined
  for (const [trailingIndex, { recovery, eventIndex }] of trailingRecoveryRecords.entries()) {
    if (trailingIndex >= 2) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `trailing recovery prefix at event ${eventIndex} exceeds two events`,
      )
    }
    const expectedSeat: Mark = trailingIndex === 0 ? 'X' : 'O'
    if (recovery.seat !== expectedSeat) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        trailingIndex === 0
          ? `trailing recovery prefix at event ${eventIndex} must start with seat X`
          : `trailing recovery prefix at event ${eventIndex} must continue with seat O`,
      )
    }
    if (requestIds.has(recovery.trigger_request_id)) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `trailing recovery trigger_request_id at event ${eventIndex} reuses prior auction request_id ${recovery.trigger_request_id}`,
      )
    }
    if (trailingIndex === 0) {
      trailingTriggerRequestId = recovery.trigger_request_id
    } else if (recovery.trigger_request_id !== trailingTriggerRequestId) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `trailing recovery prefix at event ${eventIndex} must share trigger_request_id ${trailingTriggerRequestId}`,
      )
    }
  }

  return {
    setup: { start: game.start },
    auctions,
    trailingRecoveries: trailingRecoveryRecords.map(({ recovery }) => recovery),
    terminal: game.end,
    positions,
    truncated: game.truncated,
    truncation: game.truncation,
  }
}
