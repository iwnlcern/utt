import { PV_PIN, PV_UNAVAILABLE_MESSAGE } from '../analysis/extract'
import type { Mark, Move } from '../log/types'
import type { Position } from '../replay/model'
import '../styles/tokens.css'

type Ghost = {
  seat: Mark
  move: Move
}

export interface BoardAnnotations {
  lastApplied?: Move
  losingIntent?: Ghost
}

export interface PendingBoardAnnotations {
  forced: number | null
  conditionalGhosts?: Partial<Record<Mark, Move>>
}

export interface BoardProps {
  position: Position
  annotations?: BoardAnnotations
  pending?: PendingBoardAnnotations
  showLosingIntent?: boolean
}

export interface ConditionalGhostBadgesProps {
  seats: readonly Mark[]
  coincident?: boolean
}

const locals = Array.from({ length: 9 }, (_, index) => index)
const cells = Array.from({ length: 9 }, (_, index) => index)

function sameMove(move: Move | undefined, local: number, cell: number): boolean {
  return move?.[0] === local && move[1] === cell
}

// This pure badge seam is gated by PV_PIN in Board; it gives the enabled path
// one explicit representation for same-cell X/O conditional moves.
export function ConditionalGhostBadges({
  seats,
  coincident = false,
}: ConditionalGhostBadgesProps) {
  if (coincident) {
    return (
      <span
        className="board__ghost board__ghost--conditional board__ghost--split"
        data-testid="conditional-ghost-split"
        aria-label="conditional best move: X and O"
      >
        X/O
      </span>
    )
  }

  return seats.map((seat) => (
    <span
      className={`board__ghost board__ghost--conditional board__ghost--${seat}`}
      data-testid={`conditional-ghost-${seat}`}
      key={seat}
      aria-label={`conditional best move: ${seat}`}
    >
      {seat}
    </span>
  ))
}

export function Board({
  position,
  annotations,
  pending,
  showLosingIntent = true,
}: BoardProps) {
  const conditionalGhosts = PV_PIN.pinned ? pending?.conditionalGhosts : undefined
  const conditionalUnavailable = pending?.conditionalGhosts !== undefined && !PV_PIN.pinned

  return (
    <section className="board" aria-label="UTTT board">
      {conditionalUnavailable && <p className="board__unavailable">{PV_UNAVAILABLE_MESSAGE}</p>}
      <div className="board__macro-grid">
        {locals.map((local) => {
          const closed = position.closed[local]
          const forced = pending?.forced === local
          const localBoard = position.board[local] ?? '.........'

          return (
            <section
              className="board__local"
              data-forced={forced || undefined}
              data-testid={`local-board-${local}`}
              key={local}
              aria-label={`local board ${local}`}
            >
              <div className="board__cell-grid">
                {cells.map((cell) => {
                  const mark = localBoard[cell] ?? '.'
                  const occupied = mark !== '.'
                  const losingIntent = annotations?.losingIntent
                  const showLosingGhost = showLosingIntent
                    && sameMove(losingIntent?.move, local, cell)
                    && !occupied
                  const conditionalSeats = (['X', 'O'] as const).filter((seat) =>
                    sameMove(conditionalGhosts?.[seat], local, cell),
                  )
                  const label = `local ${local}, cell ${cell}, ${occupied ? mark : 'empty'}${
                    showLosingGhost && losingIntent !== undefined ? `, losing intent ${losingIntent.seat}` : ''
                  }`

                  return (
                    <button
                      className="board__cell"
                      data-last-applied={sameMove(annotations?.lastApplied, local, cell) || undefined}
                      disabled
                      key={cell}
                      aria-label={label}
                      type="button"
                    >
                      <span className="board__mark">{occupied ? mark : ''}</span>
                      {showLosingGhost && (
                        <span className="board__ghost" data-testid="losing-intent-ghost">
                          {losingIntent?.seat}
                        </span>
                      )}
                      <ConditionalGhostBadges
                        seats={conditionalSeats}
                        coincident={conditionalSeats.length === 2}
                      />
                    </button>
                  )
                })}
              </div>
              {closed !== undefined && (
                <div className="board__closed" data-testid={`closed-local-${local}`}>
                  {closed} closed
                </div>
              )}
            </section>
          )
        })}
      </div>
    </section>
  )
}
