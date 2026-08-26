import { render, screen, within } from '@testing-library/react'
import { describe, expect, it } from 'vitest'

import { TPChart } from '../../src/components/TPChart'

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
  })
})
