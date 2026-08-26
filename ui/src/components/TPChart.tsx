import type { TPSeries } from '../analysis/series'
import { formatPercent } from '../format/money'
import '../styles/tokens.css'

export interface TPChartProps {
  series: TPSeries
  /** Replay cursor, expressed as the corresponding position index. */
  cursor: number
}

type SeriesName = 't' | 'p'

const WIDTH = 360
const HEIGHT = 180
const PADDING = { top: 16, right: 16, bottom: 32, left: 40 }
const PLOT_WIDTH = WIDTH - PADDING.left - PADDING.right
const PLOT_HEIGHT = HEIGHT - PADDING.top - PADDING.bottom

function pointX(index: number, length: number): number {
  return PADDING.left + (length <= 1 ? PLOT_WIDTH / 2 : (index / (length - 1)) * PLOT_WIDTH)
}

function pointY(value: number): number {
  return PADDING.top + (1 - value) * PLOT_HEIGHT
}

function segments(values: readonly (number | null)[]): Array<Array<{ index: number; value: number }>> {
  const result: Array<Array<{ index: number; value: number }>> = []
  let current: Array<{ index: number; value: number }> = []

  values.forEach((value, index) => {
    if (value === null) {
      if (current.length > 0) result.push(current)
      current = []
      return
    }
    current.push({ index, value })
  })
  if (current.length > 0) result.push(current)
  return result
}

function valueLabel(value: number | null): string {
  return value === null ? 'unavailable' : formatPercent({ kind: 'ok', value })
}

function SeriesLines({ name, values, maxLength }: {
  name: SeriesName
  values: readonly (number | null)[]
  maxLength: number
}) {
  return (
    <g aria-label={`${name} series`} className={`tp-chart__series tp-chart__series--${name}`} data-series={name}>
      {segments(values).map((segment) => (
        <polyline
          data-testid={`tp-line-${name}`}
          fill="none"
          key={`${name}-${segment[0].index}`}
          points={segment.map(({ index, value }) => `${pointX(index, maxLength)},${pointY(value)}`).join(' ')}
        />
      ))}
    </g>
  )
}

export function TPChart({ series, cursor }: TPChartProps) {
  const maxLength = Math.max(series.t.length, series.p.length, 1)
  const currentT = series.t[cursor] ?? null
  const currentP = series.p[cursor] ?? null
  const ariaLabel = `T and p over replay plies. Current T: ${valueLabel(currentT)}; current p: ${valueLabel(currentP)}.`

  return (
    <figure className="tp-chart">
      <figcaption>T and p by replay position</figcaption>
      <svg aria-label={ariaLabel} role="img" viewBox={`0 0 ${WIDTH} ${HEIGHT}`}>
        <g aria-hidden="true" className="tp-chart__axes">
          <line x1={PADDING.left} x2={PADDING.left} y1={PADDING.top} y2={HEIGHT - PADDING.bottom} />
          <line x1={PADDING.left} x2={WIDTH - PADDING.right} y1={HEIGHT - PADDING.bottom} y2={HEIGHT - PADDING.bottom} />
          <text x={PADDING.left - 6} y={PADDING.top + 4} textAnchor="end">100%</text>
          <text x={PADDING.left - 6} y={HEIGHT - PADDING.bottom + 4} textAnchor="end">0%</text>
          <text x={WIDTH / 2} y={HEIGHT - 6} textAnchor="middle">Replay position</text>
        </g>
        <SeriesLines maxLength={maxLength} name="t" values={series.t} />
        <SeriesLines maxLength={maxLength} name="p" values={series.p} />
        <g aria-hidden="true" className="tp-chart__cursor" data-cursor={cursor}>
          <line x1={pointX(cursor, maxLength)} x2={pointX(cursor, maxLength)} y1={PADDING.top} y2={HEIGHT - PADDING.bottom} />
          {currentT !== null && <circle cx={pointX(cursor, maxLength)} cy={pointY(currentT)} data-series="t" data-testid="tp-cursor-marker" r="4" />}
          {currentP !== null && <circle cx={pointX(cursor, maxLength)} cy={pointY(currentP)} data-series="p" data-testid="tp-cursor-marker" r="4" />}
        </g>
      </svg>
      <div aria-hidden="true" className="tp-chart__legend">
        <span className="tp-chart__legend-item tp-chart__legend-item--t">T threshold</span>
        <span className="tp-chart__legend-item tp-chart__legend-item--p">p actual share</span>
      </div>
    </figure>
  )
}
