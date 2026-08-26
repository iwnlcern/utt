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
  const recoveries: RecoveryEvent[] = game.events.filter(
    (event): event is RecoveryEvent => event.event === 'recovery',
  )
  let expectedPly = 0

  for (const [eventIndex, event] of game.events.entries()) {
    if (event.event !== 'auction') continue

    if (event.ply !== expectedPly) {
      throw new LogError(
        eventIndex + 1,
        eventIndex,
        `non-sequential auction ply at event ${eventIndex}: expected ${expectedPly}, received ${event.ply}`,
      )
    }
    expectedPly += 1

    const pre = positions.at(-1)
    if (pre === undefined) throw new Error('replay positions must start with position_0')

    const stepRecoveries = recoveries.filter((recovery) => recovery.ply === event.ply)

    if (event.outcome !== 'resolved') {
      auctions.push({
        ply: event.ply,
        pre,
        attempts: event.attempts,
        recoveries: stepRecoveries,
        outcome: event.outcome,
      })
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

  return {
    setup: { start: game.start },
    auctions,
    trailingRecoveries: recoveries.filter((recovery) => !auctions.some((auction) => auction.ply === recovery.ply)),
    terminal: game.end,
    positions,
    truncated: game.truncated,
  }
}
