import { formatPercent, formatUnits, share } from '../format/money'
import type { Mark } from '../log/types'

export interface BudgetBarsProps {
  budgets: Readonly<Record<Mark, number>>
}

const seats: readonly Mark[] = ['X', 'O']

export function BudgetBars({ budgets }: BudgetBarsProps) {
  const combined = budgets.X + budgets.O

  return (
    <section aria-label="budget bars" className="metrics__budgets">
      <h3>Budgets</h3>
      <p data-testid="budget-units">
        {formatUnits(budgets.X)} / {formatUnits(budgets.O)} units
      </p>
      {seats.map((seat) => {
        const budgetShare = share(budgets[seat], combined)
        return (
          <div key={seat} className={`metrics__budget metrics__budget--${seat}`}>
            <span>{seat}: {formatPercent(budgetShare)}</span>
            <progress
              aria-label={`${seat} budget share`}
              max={1}
              value={budgetShare.kind === 'ok' ? budgetShare.value : 0}
            />
          </div>
        )
      })}
    </section>
  )
}
