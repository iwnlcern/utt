import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen, waitFor } from '@testing-library/react'
import { afterEach, beforeEach, describe, expect, it } from 'vitest'

import App from '../src/App'

describe('App', () => {
  beforeEach(() => {
    window.history.replaceState(null, '', '/')
    const values = new Map<string, string>()
    Object.defineProperty(window, 'localStorage', {
      configurable: true,
      value: {
        get length() { return values.size },
        clear: () => values.clear(),
        getItem: (key: string) => values.get(key) ?? null,
        key: (index: number) => [...values.keys()][index] ?? null,
        removeItem: (key: string) => { values.delete(key) },
        setItem: (key: string, value: string) => { values.set(key, value) },
      } as Storage,
    })
  })

  afterEach(() => {
    cleanup()
    window.history.replaceState(null, '', '/')
  })

  it('renders the Poorman UTTT replay title', () => {
    render(<App />)

    expect(
      screen.getByRole('heading', { name: 'Poorman UTTT Replay' }),
    ).toBeTruthy()
  })

  it('clears replay cursor state before loading a second game', async () => {
    render(<App />)
    const firstGame = readFileSync(resolve(import.meta.dirname, '../fixtures/ghost-divergence.jsonl'), 'utf8')

    fireEvent.drop(screen.getByLabelText('Drop a JSONL game log'), {
      dataTransfer: { files: [new File([firstGame], 'ghost-divergence.jsonl')] },
    })

    await waitFor(() => expect(screen.getByRole('heading', { name: 'Game replay' })).toBeTruthy())
    fireEvent.click(screen.getByRole('button', { name: 'Next position' }))
    await waitFor(() => expect(window.location.hash).toBe('#cursor=1'))

    fireEvent.click(screen.getByRole('button', { name: 'Open another log' }))
    expect(screen.getByRole('heading', { name: 'Poorman UTTT Replay' })).toBeTruthy()
    expect(window.location.hash).toBe('')

    const secondGame = readFileSync(resolve(import.meta.dirname, '../fixtures/success-macro-win.jsonl'), 'utf8')
    fireEvent.drop(screen.getByLabelText('Drop a JSONL game log'), {
      dataTransfer: { files: [new File([secondGame], 'success-macro-win.jsonl')] },
    })

    await waitFor(() => expect(screen.getByRole('heading', { name: 'Game replay' })).toBeTruthy())
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 69')
  })
})
