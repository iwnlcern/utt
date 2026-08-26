import { useEffect, useMemo, useReducer, useState } from 'react'

import { extractAnalysis } from '../analysis/extract'
import { buildTPSeries } from '../analysis/series'
import { Board, type BoardAnnotations } from '../components/Board'
import { MetricsPanel } from '../components/MetricsPanel'
import { TPChart } from '../components/TPChart'
import { Timeline } from '../components/Timeline'
import { formatUnits } from '../format/money'
import type { GameRecord } from '../log/gameRecord'
import type { Mark } from '../log/types'
import { deriveReplayModel } from '../replay/derive'
import { createCursorState, cursorReducer } from './cursorReducer'

export interface GameViewProps {
  game: GameRecord
  notice?: string | null
  onExit?: () => void
}

function isReplayShortcutTarget(target: EventTarget | null): boolean {
  if (!(target instanceof Element)) return true
  return target.closest('input, select, textarea, [contenteditable]:not([contenteditable="false"])') === null
}

function GameView({ game, notice, onExit }: GameViewProps) {
  const model = useMemo(() => deriveReplayModel(game), [game])
  const [showLosingIntent, setShowLosingIntent] = useState(true)
  const [preferredAnalysisSeat, setPreferredAnalysisSeat] = useState<Mark>('X')
  const [cursorState, dispatch] = useReducer(
    cursorReducer,
    createCursorState(model.positions.length - 1, window.location.hash),
  )
  const reduceMotion = window.matchMedia?.('(prefers-reduced-motion: reduce)').matches ?? false

  useEffect(() => {
    const hydrate = () => dispatch({ type: 'hydrate', hash: window.location.hash })
    const hash = `#cursor=${cursorState.cursor}`
    if (window.location.hash !== hash) window.history.replaceState(null, '', hash)
    window.addEventListener('hashchange', hydrate)
    return () => window.removeEventListener('hashchange', hydrate)
  }, [cursorState.cursor])

  useEffect(() => {
    const navigate = (event: KeyboardEvent) => {
      if (
        event.defaultPrevented
        || event.altKey
        || event.ctrlKey
        || event.metaKey
        || event.shiftKey
        || !isReplayShortcutTarget(event.target)
      ) return
      const action = (() => {
        switch (event.key) {
          case 'ArrowLeft': return { type: 'step', delta: -1 } as const
          case 'ArrowRight': return { type: 'step', delta: 1 } as const
          case 'PageUp': return { type: 'step', delta: -5 } as const
          case 'PageDown': return { type: 'step', delta: 5 } as const
          case 'Home': return { type: 'jump', cursor: 0 } as const
          case 'End': return { type: 'jump', cursor: model.positions.length - 1 } as const
          default: return null
        }
      })()
      if (action === null) return
      event.preventDefault()
      dispatch(action)
    }
    window.addEventListener('keydown', navigate)
    return () => window.removeEventListener('keydown', navigate)
  }, [model.positions.length])

  const position = model.positions[cursorState.cursor]
  const pendingStep = model.auctions[cursorState.cursor]
  const pendingAnalyses = pendingStep === undefined ? {} : extractAnalysis(pendingStep)
  const usableAnalysisSeats = (['X', 'O'] as const).filter((seat) => pendingAnalyses[seat]?.kind === 'ok')
  const selectedAnalysisSeat = usableAnalysisSeats.includes(preferredAnalysisSeat)
    ? preferredAnalysisSeat
    : usableAnalysisSeats[0] ?? preferredAnalysisSeat
  const selectedAnalysis = pendingAnalyses[selectedAnalysisSeat]
  const ghostAnalysis = selectedAnalysis?.kind === 'ok' ? selectedAnalysis : undefined
  const chartSeries = useMemo(
    () => buildTPSeries(model, selectedAnalysisSeat),
    [model, selectedAnalysisSeat],
  )
  const justResolved = cursorState.cursor > 0
    ? model.auctions[cursorState.cursor - 1]
    : undefined
  if (position === undefined) throw new Error('cursor must select a logged replay position')

  let annotations: BoardAnnotations | undefined
  if (justResolved?.outcome === 'resolved') {
    const losingSeat: Mark = justResolved.resolution.winner === 'X' ? 'O' : 'X'
    const losingMove = justResolved.attempts.at(-1)?.turns[losingSeat].move
    annotations = {
      lastApplied: justResolved.resolution.move,
      ...(losingMove === undefined ? {} : { losingIntent: { seat: losingSeat, move: losingMove } }),
    }
  }

  return (
    <main className="game-view">
      <header className="game-view__header">
        <div>
          <p className="game-view__eyebrow">Logged game · {game.start.game_id.slice(0, 8)}</p>
          <h1>Game replay</h1>
        </div>
        {onExit !== undefined && (
          <button aria-label="Open another log" onClick={onExit} type="button">Open another log</button>
        )}
      </header>
      {notice !== null && notice !== undefined && <p role="alert">{notice}</p>}
      <div className="game-view__toolbar">
        <p aria-label="replay position" className="game-view__position" role="status">
          Position <strong>{cursorState.cursor}</strong> of {cursorState.maxCursor}
        </p>
        <nav aria-label="Replay controls" className="game-view__controls">
          <button
            aria-label="First position"
            disabled={cursorState.cursor === 0}
            onClick={() => dispatch({ type: 'jump', cursor: 0 })}
            type="button"
          >
            First
          </button>
          <button
            aria-label="Previous position"
            disabled={cursorState.cursor === 0}
            onClick={() => dispatch({ type: 'step', delta: -1 })}
            type="button"
          >
            Previous
          </button>
          <button
            aria-label="Next position"
            disabled={cursorState.cursor === cursorState.maxCursor}
            onClick={() => dispatch({ type: 'step', delta: 1 })}
            type="button"
          >
            Next
          </button>
          <button
            aria-label="Last position"
            disabled={cursorState.cursor === cursorState.maxCursor}
            onClick={() => dispatch({ type: 'jump', cursor: cursorState.maxCursor })}
            type="button"
          >
            Last
          </button>
        </nav>
        <p className="game-view__shortcuts">←/→ step · Home/End jump · Page keys ±5</p>
      </div>
      <div className="game-view__workspace">
        <section aria-labelledby="position-board-heading" className="game-view__board-column">
          <div className="game-view__section-heading">
            <h2 id="position-board-heading">Position</h2>
            <span>{pendingStep === undefined ? 'Final logged state' : `Ply ${pendingStep.ply} pending`}</span>
          </div>
          <div
            className="game-view__board-stage"
            data-revealing={cursorState.revealVersion > 0 && !reduceMotion || undefined}
            data-reveal-version={cursorState.revealVersion}
            key={cursorState.revealVersion}
            data-testid="board-stage"
          >
            <Board
              annotations={annotations}
              pending={pendingStep === undefined ? undefined : {
                forced: pendingStep.pre.forced,
                ...(ghostAnalysis === undefined ? {} : {
                  conditionalGhosts: {
                    ...(ghostAnalysis.pvIfWin === undefined ? {} : { X: ghostAnalysis.pvIfWin }),
                    ...(ghostAnalysis.pvIfLose === undefined ? {} : { O: ghostAnalysis.pvIfLose }),
                  },
                }),
              }}
              position={position}
              showLosingIntent={showLosingIntent}
            />
          </div>
          {justResolved?.outcome === 'resolved' && (
            <div className="game-view__resolution">
              <div>
                <span>Just resolved</span>
                <p data-testid="last-payment">payment: {formatUnits(justResolved.resolution.payment)} units</p>
              </div>
              {annotations?.losingIntent !== undefined && (
                <button
                  aria-label={`${showLosingIntent ? 'Hide' : 'Show'} losing intent`}
                  aria-pressed={showLosingIntent}
                  onClick={() => setShowLosingIntent((shown) => !shown)}
                  type="button"
                >
                  {showLosingIntent ? 'Hide' : 'Show'} losing intent
                </button>
              )}
            </div>
          )}
        </section>
        <aside aria-label="Replay analysis" className="game-view__analysis-column">
          <details className="game-view__analysis-card" open>
            <summary>Analysis &amp; chart</summary>
            <MetricsPanel
              analyses={pendingAnalyses}
              onSelectedSeatChange={setPreferredAnalysisSeat}
              position={position}
              selectedSeat={selectedAnalysisSeat}
            />
            {ghostAnalysis !== undefined && (
              <div aria-label="Conditional move legend" className="game-view__ghost-legend">
                <span className="game-view__ghost-legend-X">● X · if X wins</span>
                <span className="game-view__ghost-legend-O">■ O · if O wins</span>
              </div>
            )}
            <TPChart cursor={cursorState.cursor} series={chartSeries} />
          </details>
          <section className="game-view__timeline-card">
            <h2>Timeline</h2>
            <Timeline model={model} onSelect={(cursor) => dispatch({ type: 'jump', cursor })} />
          </section>
        </aside>
      </div>
    </main>
  )
}

export default GameView
