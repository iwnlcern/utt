import { cleanup, render, screen, within } from '@testing-library/react'
import { afterEach, describe, expect, it } from 'vitest'

import { TPChart } from '../../src/components/TPChart'

afterEach(cleanup)

describe('TPChart', () => {
  it('renders separate line segments around unavailable values without interpolation', () => {
    const { container } = render(
      <TPChart cursor={2} series={{ t: [0.2, null, 0.8], p: [0.1, 0.4, null] }} />,
    )

    const chart = screen.getByRole('img', { name: /current t: 80\.00%; current p: unavailable/i })
    expect(within(chart).getAllByTestId('tp-line-t')).toHaveLength(2)
    expect(within(chart).getAllByTestId('tp-line-p')).toHaveLength(1)
    expect(container.querySelectorAll('[data-testid="tp-cursor-marker"]')).toHaveLength(1)
  })

  it('puts the cursor marker at the current series value and describes both current values', () => {
    render(<TPChart cursor={1} series={{ t: [0.2, 0.7], p: [0.1, 0.4] }} />)

    const chart = screen.getByRole('img', { name: /current t: 70\.00%; current p: 40\.00%/i })
    const markers = within(chart).getAllByTestId('tp-cursor-marker')
    expect(markers).toHaveLength(2)
    expect(markers.map((marker) => marker.getAttribute('data-series'))).toEqual(['t', 'p'])
    expect(within(chart).getByTestId('tp-cursor-guide').getAttribute('x1')).toBe('344')
  })

  it('applies the non-color dash cue to the p trace itself', () => {
    render(<TPChart cursor={0} series={{ t: [0.2, 0.7], p: [0.1, 0.4] }} />)

    expect(screen.getByTestId('tp-line-t').getAttribute('stroke-dasharray')).toBeNull()
    expect(screen.getByTestId('tp-line-p').getAttribute('stroke-dasharray')).toBe('6 4')
  })

  it('treats nonfinite and out-of-range values as unavailable chart gaps', () => {
    const { container } = render(
      <TPChart cursor={0} series={{ t: [-0.1, 0.5, 1.1], p: [Number.NaN, Number.POSITIVE_INFINITY, 0.25] }} />,
    )

    expect(screen.getByRole('img', { name: /current t: unavailable; current p: unavailable/i })).not.toBeNull()
    expect(container.querySelectorAll('[data-testid="tp-line-t"]')).toHaveLength(1)
    expect(container.querySelectorAll('[data-testid="tp-line-p"]')).toHaveLength(1)
    expect(container.querySelectorAll('[data-testid="tp-cursor-marker"]')).toHaveLength(0)
  })

  it('omits the cursor guide and values when the cursor is outside the series domain', () => {
    const { container } = render(<TPChart cursor={3} series={{ t: [0.2, 0.7], p: [0.1, 0.4] }} />)

    expect(screen.getByRole('img', { name: /current t: unavailable; current p: unavailable/i })).not.toBeNull()
    expect(container.querySelector('[data-testid="tp-cursor-guide"]')).toBeNull()
    expect(container.querySelectorAll('[data-testid="tp-cursor-marker"]')).toHaveLength(0)
  })
})
