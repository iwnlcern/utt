import type { Mark, Move } from '../log/types'
import type { AuctionStep } from '../replay/model'

export type Quality = 'exact' | 'bound' | 'estimate'

export type AnalysisEntry =
  | {
    kind: 'ok'
    t?: number
    criticalBid?: number
    pvIfWin?: Move
    pvIfLose?: Move
    quality?: Quality
    lo?: number
    hi?: number
    depth?: number
    complete?: boolean
    degraded: string[]
  }
  | { kind: 'unavailable'; why: string }

// Flip only after the harness publishes and cites its exact state-hash artifact.
export const PV_PIN: { pinned: boolean; source: string | null } = { pinned: false, source: null }
export const PV_UNAVAILABLE_MESSAGE = 'unavailable — awaiting harness artifact pin'

const marks: readonly Mark[] = ['X', 'O']

const isFiniteNumber = (value: unknown): value is number =>
  typeof value === 'number' && Number.isFinite(value)

const isInteger = (value: unknown): value is number =>
  typeof value === 'number' && Number.isInteger(value)

const isCell = (value: unknown): value is 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 =>
  isInteger(value) && value >= 0 && value <= 8

const isMove = (value: unknown): value is Move =>
  Array.isArray(value) && value.length === 2 && isCell(value[0]) && isCell(value[1])

const isQuality = (value: unknown): value is Quality =>
  value === 'exact' || value === 'bound' || value === 'estimate'

const unavailable = (why: string): AnalysisEntry => ({ kind: 'unavailable', why })

function extractEntry(info: unknown): AnalysisEntry {
  if (info === undefined) return unavailable('no info in log')
  if (info === null || Array.isArray(info) || typeof info !== 'object') {
    return unavailable('malformed info in log')
  }

  const fields = info as Record<string, unknown>
  const degraded: string[] = []
  const entry: Extract<AnalysisEntry, { kind: 'ok' }> = { kind: 'ok', degraded }
  let extracted = false
  let malformed = false

  if ('t' in fields) {
    if (isFiniteNumber(fields.t)) {
      entry.t = fields.t
      extracted = true
    } else {
      degraded.push('t')
      malformed = true
    }
  }
  if ('critical_bid' in fields) {
    if (isInteger(fields.critical_bid)) {
      entry.criticalBid = fields.critical_bid
      extracted = true
    } else {
      degraded.push('criticalBid')
      malformed = true
    }
  }
  if ('pv_if_win' in fields) {
    if (isMove(fields.pv_if_win)) {
      // The s1-m4 ruling makes logged PV coordinates canonical X/O: no remapping.
      entry.pvIfWin = fields.pv_if_win
      extracted = true
    } else {
      degraded.push('pvIfWin')
      malformed = true
    }
  }
  if ('pv_if_lose' in fields) {
    if (isMove(fields.pv_if_lose)) {
      // The s1-m4 ruling makes logged PV coordinates canonical X/O: no remapping.
      entry.pvIfLose = fields.pv_if_lose
      extracted = true
    } else {
      degraded.push('pvIfLose')
      malformed = true
    }
  }
  if ('quality' in fields) {
    if (isQuality(fields.quality)) {
      entry.quality = fields.quality
      extracted = true
    } else {
      degraded.push('quality')
      malformed = true
    }
  }
  if ('lo' in fields) {
    if (isFiniteNumber(fields.lo)) {
      entry.lo = fields.lo
      extracted = true
    } else {
      degraded.push('lo')
      malformed = true
    }
  }
  if ('hi' in fields) {
    if (isFiniteNumber(fields.hi)) {
      entry.hi = fields.hi
      extracted = true
    } else {
      degraded.push('hi')
      malformed = true
    }
  }
  if ('depth' in fields) {
    if (isInteger(fields.depth) && fields.depth >= 0) {
      entry.depth = fields.depth
      extracted = true
    } else {
      degraded.push('depth')
      malformed = true
    }
  }
  if ('complete' in fields) {
    if (typeof fields.complete === 'boolean') {
      entry.complete = fields.complete
      extracted = true
    } else {
      degraded.push('complete')
      malformed = true
    }
  }

  if (entry.quality === 'bound' && (
    entry.lo === undefined || entry.hi === undefined || entry.lo > entry.hi
  )) {
    delete entry.quality
    delete entry.lo
    delete entry.hi
    for (const field of ['quality', 'lo', 'hi']) {
      if (!degraded.includes(field)) degraded.push(field)
    }
    malformed = true
    extracted = [
      entry.t,
      entry.criticalBid,
      entry.pvIfWin,
      entry.pvIfLose,
      entry.quality,
      entry.lo,
      entry.hi,
      entry.depth,
      entry.complete,
    ].some((value) => value !== undefined)
  }

  if (!extracted) return unavailable(malformed ? 'malformed info in log' : 'no recognized analysis in log')
  return entry
}

export function extractAnalysis(step: AuctionStep): Partial<Record<Mark, AnalysisEntry>> {
  const attempt = step.attempts.at(-1)
  if (attempt === undefined) {
    return Object.fromEntries(marks.map((mark) => [mark, unavailable('no attempt in log')])) as Partial<Record<Mark, AnalysisEntry>>
  }

  return Object.fromEntries(
    marks.map((mark) => [mark, extractEntry(attempt.turns[mark]?.info)]),
  ) as Partial<Record<Mark, AnalysisEntry>>
}
