import { describe, expect, it } from 'vitest'

import { splitJsonl } from '../../src/log/jsonl'

describe('splitJsonl', () => {
  it('returns every newline-terminated record with its one-based line number', () => {
    expect(splitJsonl('{"event":"game_start"}\n{"event":"game_end"}\n')).toEqual({
      lines: [
        { raw: '{"event":"game_start"}', line: 1 },
        { raw: '{"event":"game_end"}', line: 2 },
      ],
      discardedTail: null,
    })
  })

  it('keeps a parseable final record when it lacks a terminating newline', () => {
    expect(splitJsonl('{"event":"game_start"}\n{"event":"auction"}')).toEqual({
      lines: [
        { raw: '{"event":"game_start"}', line: 1 },
        { raw: '{"event":"auction"}', line: 2 },
      ],
      discardedTail: null,
    })
  })

  it('discards only an unterminated final record that is not JSON', () => {
    expect(splitJsonl('{"event":"game_start"}\n{"event":"auction"')).toEqual({
      lines: [{ raw: '{"event":"game_start"}', line: 1 }],
      discardedTail: '{"event":"auction"',
    })
  })

  it('keeps newline-terminated invalid JSON for Task 4 validation', () => {
    expect(splitJsonl('{"event":"game_start"}\nnot-json\n')).toEqual({
      lines: [
        { raw: '{"event":"game_start"}', line: 1 },
        { raw: 'not-json', line: 2 },
      ],
      discardedTail: null,
    })
  })

  it('returns no records for an empty file', () => {
    expect(splitJsonl('')).toEqual({ lines: [], discardedTail: null })
  })
})
