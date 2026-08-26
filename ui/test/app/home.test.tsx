import { readFileSync } from 'node:fs'
import { resolve } from 'node:path'

import { cleanup, fireEvent, render, screen, waitFor } from '@testing-library/react'
import userEvent from '@testing-library/user-event'
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

  it('tabs directly to the visible file chooser and activates its native input with Enter and Space', async () => {
    const user = userEvent.setup()
    render(<Home onLoaded={vi.fn()} />)
    const input = screen.getByLabelText('Choose a JSONL game log')
    const chooser = screen.getByRole('button', { name: 'Choose a .jsonl file' })
    let pickerActivations = 0
    input.addEventListener('click', () => { pickerActivations += 1 })

    await user.tab()

    expect(document.activeElement).toBe(chooser)
    await user.keyboard('{Enter}')
    expect(pickerActivations).toBe(1)
    await user.keyboard(' ')
    expect(pickerActivations).toBe(2)
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

  it('re-fetches the bundled sample after a rejected fetch and opens it after recovery', async () => {
    const onLoaded = vi.fn()
    const fetchSample = vi.fn()
      .mockRejectedValueOnce(new Error('sample unavailable'))
      .mockResolvedValueOnce(new Response(fixtureText('success-macro-win.jsonl')))
    vi.stubGlobal('fetch', fetchSample)
    render(<Home onLoaded={onLoaded} />)

    fireEvent.click(screen.getByRole('button', { name: 'Open bundled sample game' }))

    expect(await screen.findByRole('heading', { name: 'Could not open sample-game.jsonl' })).toBeTruthy()
    expect(screen.getByRole('alert').textContent).toMatch(/sample unavailable/i)
    fireEvent.click(screen.getByRole('button', { name: 'Retry sample-game.jsonl' }))

    await waitFor(() => expect(fetchSample).toHaveBeenCalledTimes(2))
    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(1))
  })

  it('re-fetches the bundled sample after a rejected response body and opens it after recovery', async () => {
    const onLoaded = vi.fn()
    const fetchSample = vi.fn()
      .mockResolvedValueOnce({
        ok: true,
        text: vi.fn().mockRejectedValueOnce(new Error('sample body unavailable')),
      } as unknown as Response)
      .mockResolvedValueOnce(new Response(fixtureText('success-macro-win.jsonl')))
    vi.stubGlobal('fetch', fetchSample)
    render(<Home onLoaded={onLoaded} />)

    fireEvent.click(screen.getByRole('button', { name: 'Open bundled sample game' }))

    expect(await screen.findByRole('heading', { name: 'Could not open sample-game.jsonl' })).toBeTruthy()
    expect(screen.getByRole('alert').textContent).toMatch(/sample body unavailable/i)
    fireEvent.click(screen.getByRole('button', { name: 'Retry sample-game.jsonl' }))

    await waitFor(() => expect(fetchSample).toHaveBeenCalledTimes(2))
    await waitFor(() => expect(onLoaded).toHaveBeenCalledTimes(1))
  })

  it('renders a structured LogError and retains the malformed file for retry', async () => {
    const onLoaded = vi.fn()
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([fixtureText('malformed-interior.jsonl')], 'bad-log.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open bad-log.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 2')).toBeTruthy()
    expect(screen.getByText('Event 1')).toBeTruthy()
    expect(screen.getByText('invalid JSON event')).toBeTruthy()
    fireEvent.click(screen.getByRole('button', { name: 'Retry bad-log.jsonl' }))
    expect(await screen.findByText('Line 2')).toBeTruthy()
    expect(screen.getByText('Event 1')).toBeTruthy()
    expect(onLoaded).not.toHaveBeenCalled()
  })

  it('rejects a schema-valid nonsequential ply before saving or opening the game', async () => {
    const onLoaded = vi.fn()
    const nonsequential = fixtureText('success-macro-win.jsonl').replace('"ply":1', '"ply":2')
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([nonsequential], 'nonsequential.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open nonsequential.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 3')).toBeTruthy()
    expect(screen.getByText('Event 2')).toBeTruthy()
    expect(screen.getByText('non-sequential auction ply at event 2: expected 1, received 2')).toBeTruthy()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
    expect(onLoaded).not.toHaveBeenCalled()
  })

  it('rejects a stale recovery association and retains its source for retry', async () => {
    const onLoaded = vi.fn()
    const events = fixtureText('fault-single.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const recovery = events.find((event) => event.event === 'recovery')
    recovery.trigger_request_id = 'stale-request-id'
    const staleRecovery = `${events.map(JSON.stringify).join('\n')}\n`
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([staleRecovery], 'stale-recovery.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open stale-recovery.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 3')).toBeTruthy()
    expect(screen.getByText('Event 2')).toBeTruthy()
    expect(screen.getByText('recovery trigger_request_id does not match an attempt at ply 0')).toBeTruthy()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
    expect(onLoaded).not.toHaveBeenCalled()

    fireEvent.click(screen.getByRole('button', { name: 'Retry stale-recovery.jsonl' }))

    expect(await screen.findByText('recovery trigger_request_id does not match an attempt at ply 0')).toBeTruthy()
    expect(onLoaded).not.toHaveBeenCalled()
  })

  it('shows a structured error for a complete log with an orphan recovery', async () => {
    const onLoaded = vi.fn()
    const events = fixtureText('hello-fault.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const recovery = JSON.parse(fixtureText('trailing-recovery.jsonl').split('\n')[1])
    recovery.game_id = events[0].game_id
    events.splice(1, 0, recovery)
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([`${events.map(JSON.stringify).join('\n')}\n`], 'orphan-recovery.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open orphan-recovery.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 2')).toBeTruthy()
    expect(screen.getByText('Event 1')).toBeTruthy()
    expect(screen.getByText('recovery at event 1 references missing auction ply 0 in a complete log')).toBeTruthy()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
    expect(onLoaded).not.toHaveBeenCalled()
  })

  it('shows a structured error for invalid attempt ordinals before saving or opening', async () => {
    const onLoaded = vi.fn()
    const events = fixtureText('double-fault-retry.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const auction = events.find((event) => event.event === 'auction')
    auction.attempts[1].attempt = 3
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([`${events.map(JSON.stringify).join('\n')}\n`], 'bad-attempts.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open bad-attempts.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 4')).toBeTruthy()
    expect(screen.getByText('Event 3')).toBeTruthy()
    expect(screen.getByText(
      'attempt ordinals must be 1..N in logged order at event 3: expected 2, received 3',
    )).toBeTruthy()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
    expect(onLoaded).not.toHaveBeenCalled()
  })

  it('shows a structured error when a later auction reuses an earlier request id', async () => {
    const onLoaded = vi.fn()
    const events = fixtureText('success-macro-win.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const auctions = events.filter((event) => event.event === 'auction')
    auctions[1].attempts[0].request_id = auctions[0].attempts[0].request_id
    const duplicateRequestId = auctions[0].attempts[0].request_id
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([`${events.map(JSON.stringify).join('\n')}\n`], 'duplicate-request.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open duplicate-request.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 3')).toBeTruthy()
    expect(screen.getByText('Event 2')).toBeTruthy()
    expect(screen.getByText(
      `attempt request_id values must be unique at event 2: duplicate ${duplicateRequestId}`,
    )).toBeTruthy()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
    expect(onLoaded).not.toHaveBeenCalled()
  })

  it('shows a structured error for an auction after a final unresolved auction', async () => {
    const onLoaded = vi.fn()
    const events = fixtureText('recovery-fault-abort.jsonl').trimEnd().split('\n').map((line) => JSON.parse(line))
    const laterAuction = JSON.parse(fixtureText('success-macro-win.jsonl').split('\n')[1])
    laterAuction.ply = 1
    events.splice(-1, 0, laterAuction)
    render(<Home onLoaded={onLoaded} />)

    await dropFile(new File([`${events.map(JSON.stringify).join('\n')}\n`], 'post-unresolved.jsonl'))

    expect(await screen.findByRole('heading', { name: 'Could not open post-unresolved.jsonl' })).toBeTruthy()
    expect(screen.getByText('Line 5')).toBeTruthy()
    expect(screen.getByText('Event 4')).toBeTruthy()
    expect(screen.getByText(
      'auction at event 4 follows unresolved ply 0; an unresolved auction must be final',
    )).toBeTruthy()
    expect(window.localStorage.getItem(RECENTS_STORAGE_KEY)).toBeNull()
    expect(onLoaded).not.toHaveBeenCalled()
  })
})
