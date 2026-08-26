import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen, waitFor } from '@testing-library/react'
import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest'

import Home from '../../src/app/Home'
import { RECENTS_STORAGE_KEY, clearRecents } from '../../src/app/recents'

const fixtureText = (name: string) =>
  readFileSync(resolve(import.meta.dirname, '../../fixtures', name), 'utf8')

const dropFile = async (file: File) => {
  fireEvent.drop(screen.getByLabelText('Drop a JSONL game log'), {
    dataTransfer: { files: [file] },
  })
}

const makeStorage = (): Storage => {
  const values = new Map<string, string>()
  return {
    get length() { return values.size },
    clear: () => values.clear(),
    getItem: (key) => values.get(key) ?? null,
    key: (index) => [...values.keys()][index] ?? null,
    removeItem: (key) => { values.delete(key) },
    setItem: (key, value) => { values.set(key, value) },
  } as Storage
}

describe('Home', () => {
  beforeEach(() => {
    Object.defineProperty(window, 'localStorage', { configurable: true, value: makeStorage() })
    window.localStorage.clear()
    clearRecents()
  })

  afterEach(() => {
    cleanup()
    vi.restoreAllMocks()
    clearRecents()
    window.localStorage.clear()
  })

  it('opens a dropped JSONL fixture and gives its validated GameRecord to the caller', async () => {
    const onLoaded = vi.fn()
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([fixtureText('success-macro-win.jsonl')], 'macro-win.jsonl'))

    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(1))
    expect(onLoaded.mock.calls[0][0]).toMatchObject({
      start: { event: 'game_start' },
      end: { result: 'X', reason: 'macro_win' },
    })
  })

  it('persists the exact recent shape and reloads it in a new Home', async () => {
    const onLoaded = vi.fn()
    const { unmount } = render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([fixtureText('success-macro-win.jsonl')], 'macro-win.jsonl'))
    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(1))

    const saved = JSON.parse(window.localStorage.getItem(RECENTS_STORAGE_KEY) ?? '[]')
    expect(saved).toHaveLength(1)
    expect(Object.keys(saved[0]).sort()).toEqual(['log_text', 'name', 'opened_at', 'result'])
    expect(saved[0]).toMatchObject({
      name: 'macro-win.jsonl',
      result: 'X',
      log_text: fixtureText('success-macro-win.jsonl'),
    })
    expect(typeof saved[0].opened_at).toBe('string')

    unmount()
    render(<Home onLoaded={onLoaded} />)
    await dropFile(new File([fixtureText('success-macro-win.jsonl')], 'another-game.jsonl'))
    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(2))

    expect(screen.getByRole('button', { name: /open recent macro-win\.jsonl/i })).toBeTruthy()
    fireEvent.click(screen.getByRole('button', { name: /open recent macro-win\.jsonl/i }))
    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(3))

    fireEvent.click(screen.getByRole('button', { name: 'Clear recent games' }))
    expect(screen.queryByRole('button', { name: /open recent/i })).toBeNull()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
  })

  it('keeps opening a valid game in session-only mode when localStorage is full', async () => {
    const onLoaded = vi.fn()
    vi.spyOn(window.localStorage, 'setItem').mockImplementation(() => {
      throw new DOMException('Quota exceeded', 'QuotaExceededError')
    })
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([fixtureText('success-macro-win.jsonl')], 'quota-game.jsonl'))

    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(1))
    expect(screen.getByRole('alert').textContent).toMatch(/session only/i)
    expect(screen.getByRole('button', { name: /open recent quota-game\.jsonl/i })).toBeTruthy()
  })

  it('opens the bundled sample game through the same validated loader', async () => {
    const onLoaded = vi.fn()
    vi.stubGlobal('fetch', vi.fn().mockResolvedValue(new Response(fixtureText('success-macro-win.jsonl'))))
    render(<Home onLoaded={onLoaded} />)

    fireEvent.click(screen.getByRole('button', { name: 'Open bundled sample game' }))

    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(1))
    expect(onLoaded.mock.calls[0][0].end).toMatchObject({ result: 'X', reason: 'macro_win' })
  })

  it('renders a structured LogError and retains the malformed file for retry', async () => {
    render(<Home onLoaded={vi.fn()} />)

    await dropFile(new File([fixtureText('malformed-interior.jsonl')], 'bad-log.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open bad-log.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 2')).toBeTruthy()
    expect(screen.getByText('Event 1')).toBeTruthy()
    expect(screen.getByText('invalid JSON event')).toBeTruthy()
    expect(screen.getByRole('button', { name: 'Retry bad-log.jsonl' })).toBeTruthy()
  })
})
