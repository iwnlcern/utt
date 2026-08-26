import { formatPercent, formatUnits, share } from '../format/money'
import type { Mark, RecoveryEvent, TurnRecord } from '../log/types'
import type { AuctionStep, ReplayModel } from '../replay/model'
import '../styles/tokens.css'

export interface TimelineProps {
  model: ReplayModel
  onSelect: (cursor: number) => void
}

function intent(turn: TurnRecord): string {
  return turn.move === undefined ? 'none' : `[${turn.move.join(', ')}]`
}

function Bid({ seat, turn, budget, ply }: {
  seat: Mark
  turn: TurnRecord
  budget: number
  ply: number
}) {
  if (turn.bid === undefined) {
    return <span data-testid={`bid-${seat}-${ply}`} title="no bid recorded">{seat}: n/a</span>
  }

  return (
    <span data-testid={`bid-${seat}-${ply}`} title={`${formatUnits(turn.bid)} units`}>
      {seat}: {formatPercent(share(turn.bid, budget))}
    </span>
  )
}

function faultClass(step: AuctionStep): string | undefined {
  const finalAttempt = step.attempts.at(-1)
  if (finalAttempt === undefined) return undefined
  for (const seat of ['X', 'O'] as const) {
    const validation = finalAttempt.turns[seat].validation
    if (validation !== 'ok') return validation
  }
  return undefined
}

function resolutionLabel(step: AuctionStep): string {
  if (step.outcome !== 'resolved') return step.outcome
  if (step.resolution.reason === 'tie_coin') return `tie_coin: ${step.resolution.coin ?? 'none'}`
  if (step.resolution.reason === 'fault') return `fault: ${faultClass(step) ?? 'unknown'}`
  return step.resolution.reason
}

function recoveryLabel(recovery: RecoveryEvent): string {
  const restart = recovery.hello.validation === 'ok'
    ? recovery.hello.name
    : `${recovery.hello.validation} fault`
  return `${recovery.seat} · ${recovery.fault} · restart hello: ${restart}`
}

function RecoveryMarkers({ recoveries, placement, ply }: {
  recoveries: readonly RecoveryEvent[]
  placement: 'pre' | 'post'
  ply: number
}) {
  if (recoveries.length === 0) return null
  return (
    <div data-testid={`${placement}-recoveries-${ply}`}>
      {recoveries.map((recovery, index) => (
        <p key={`${recovery.seat}-${recovery.trigger_request_id}-${index}`}>
          recovery {placement === 'pre' ? 'before' : 'after'} ply {ply}: {recoveryLabel(recovery)}
        </p>
      ))}
    </div>
  )
}

function splitRecoveries(step: AuctionStep): { pre: RecoveryEvent[]; post: RecoveryEvent[] } {
  if (step.outcome !== 'resolved') return { pre: [...step.recoveries], post: [] }

  const finalRequestId = step.attempts.at(-1)?.request_id
  return {
    pre: step.recoveries.filter((recovery) => recovery.trigger_request_id !== finalRequestId),
    post: step.recoveries.filter((recovery) => recovery.trigger_request_id === finalRequestId),
  }
}

function AttemptRows({ step }: { step: AuctionStep }) {
  return (
    <details>
      <summary>Attempts ({step.attempts.length})</summary>
      {step.attempts.map((attempt, index) => (
        <p data-testid={`attempt-${step.ply}-${index + 1}`} key={attempt.request_id}>
          Attempt {attempt.attempt} · X: {attempt.turns.X.validation}
          {attempt.turns.X.raw !== undefined ? ' (raw captured)' : ' (no raw capture)'} · O: {attempt.turns.O.validation}
          {attempt.turns.O.raw !== undefined ? ' (raw captured)' : ' (no raw capture)'}
        </p>
      ))}
    </details>
  )
}

function AuctionRow({ step, index, onSelect }: {
  step: AuctionStep
  index: number
  onSelect: TimelineProps['onSelect']
}) {
  const finalAttempt = step.attempts.at(-1)
  const recoveries = splitRecoveries(step)
  const cursor = index

  return (
    <>
      <RecoveryMarkers recoveries={recoveries.pre} placement="pre" ply={step.ply} />
      <article data-testid={`auction-row-${step.ply}`}>
        <button aria-label={`select pending ply ${step.ply}`} onClick={() => onSelect(cursor)} type="button">
          ply {step.ply}
        </button>
        {finalAttempt !== undefined && (
          <>
            <p>
              <Bid budget={step.pre.budgets.X} ply={step.ply} seat="X" turn={finalAttempt.turns.X} />
              {' · '}
              <Bid budget={step.pre.budgets.O} ply={step.ply} seat="O" turn={finalAttempt.turns.O} />
            </p>
            <p>
              <span data-testid={`intent-X-${step.ply}`}>Intent X: {intent(finalAttempt.turns.X)}</span>
              {' · '}
              <span data-testid={`intent-O-${step.ply}`}>Intent O: {intent(finalAttempt.turns.O)}</span>
            </p>
          </>
        )}
        <p data-testid={`resolution-${step.ply}`}>resolution: {resolutionLabel(step)}</p>
        {step.outcome === 'resolved' && (
          <>
            <p data-testid={`payment-${step.ply}`}>payment: {formatUnits(step.resolution.payment)} units</p>
            <p data-testid={`post-budgets-${step.ply}`}>
              post budgets: X {formatUnits(step.post.budgets.X)} units · O {formatUnits(step.post.budgets.O)} units
            </p>
            <p data-testid={`forced-next-${step.ply}`}>
              forced_next: {step.resolution.forced_next ?? 'any'}
            </p>
          </>
        )}
        <AttemptRows step={step} />
      </article>
      <RecoveryMarkers recoveries={recoveries.post} placement="post" ply={step.ply} />
    </>
  )
}

export function Timeline({ model, onSelect }: TimelineProps) {
  return (
    <section aria-label="auction timeline">
      {model.auctions.map((step, index) => (
        <AuctionRow index={index} key={step.ply} onSelect={onSelect} step={step} />
      ))}
      {model.trailingRecoveries.length > 0 && (
        <div data-testid="trailing-recoveries">
          {model.trailingRecoveries.map((recovery, index) => (
            <p key={`${recovery.seat}-${recovery.trigger_request_id}-${index}`}>
              trailing recovery: {recoveryLabel(recovery)}
            </p>
          ))}
        </div>
      )}
      {model.truncation === 'discarded_final_line' && <p>log ends mid-game (truncated final line discarded)</p>}
      {model.truncation === 'missing_game_end' && <p>log ends mid-game</p>}
      {model.terminal !== undefined && (
        <p data-testid={`terminal-${model.terminal.reason}`}>
          terminal: {model.terminal.reason} ({model.terminal.result})
        </p>
      )}
    </section>
  )
}
