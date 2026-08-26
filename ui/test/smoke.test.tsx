import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen, waitFor } from '@testing-library/react'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

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
    vi.restoreAllMocks()
    window.history.replaceState(null, '', '/')
  })

  it('renders the Poorman UTTT replay title', () => {
    render(<App />)

    expect(
      screen.getByRole('heading', { name: 'Poorman UTTT Replay' }),
    ).toBeTruthy()
  })

  it('gives the browser document a meaningful title', () => {
    const html = readFileSync(resolve(import.meta.dirname, '../index.html'), 'utf8')
    const page = new DOMParser().parseFromString(html, 'text/html')

    expect(page.title).toBe('Poorman UTTT Replay')
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

  it('keeps a quota warning visible in replay and clears it for a later persisted load', async () => {
    const setItem = vi.spyOn(window.localStorage, 'setItem').mockImplementationOnce(() => {
      throw new DOMException('Quota exceeded', 'QuotaExceededError')
    })
    const game = readFileSync(resolve(import.meta.dirname, '../fixtures/ghost-divergence.jsonl'), 'utf8')
    render(<App />)

    fireEvent.drop(screen.getByLabelText('Drop a JSONL game log'), {
      dataTransfer: { files: [new File([game], 'quota-game.jsonl')] },
    })

    await waitFor(() => expect(screen.getByRole('heading', { name: 'Game replay' })).toBeTruthy())
    expect(screen.getByRole('alert').textContent).toMatch(/session only/i)

    fireEvent.click(screen.getByRole('button', { name: 'Open another log' }))
    expect(screen.queryByRole('alert')).toBeNull()

    fireEvent.drop(screen.getByLabelText('Drop a JSONL game log'), {
      dataTransfer: { files: [new File([game], 'persisted-game.jsonl')] },
    })

    await waitFor(() => expect(screen.getByRole('heading', { name: 'Game replay' })).toBeTruthy())
    expect(setItem).toHaveBeenCalledTimes(2)
    expect(screen.queryByRole('alert')).toBeNull()
  })
})
