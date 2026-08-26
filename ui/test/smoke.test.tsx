import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen, waitFor } from '@testing-library/react'
import { afterEach, beforeEach, describe, expect, it } from 'vitest'

import App from '../src/App'

describe('App', () => {
  beforeEach(() => {
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

  afterEach(cleanup)

  it('renders the Poorman UTTT replay title', () => {
    render(<App />)

    expect(
      screen.getByRole('heading', { name: 'Poorman UTTT Replay' }),
    ).toBeTruthy()
  })

  it('wires a loaded Home game into GameView and returns to the loader', async () => {
    render(<App />)
    const text = readFileSync(resolve(import.meta.dirname, '../fixtures/hello-fault.jsonl'), 'utf8')

    fireEvent.drop(screen.getByLabelText('Drop a JSONL game log'), {
      dataTransfer: { files: [new File([text], 'hello-fault.jsonl')] },
    })

    await waitFor(() => expect(screen.getByRole('heading', { name: 'Game replay' })).toBeTruthy())
    expect(screen.getByRole('status', { name: 'replay position' }).textContent).toBe('Position 0 of 0')

    fireEvent.click(screen.getByRole('button', { name: 'Open another log' }))
    expect(screen.getByRole('heading', { name: 'Poorman UTTT Replay' })).toBeTruthy()
  })
})
