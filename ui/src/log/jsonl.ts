export interface JsonlLine {
  raw: string
  line: number
}

export interface JsonlSplit {
  lines: JsonlLine[]
  discardedTail: string | null
}

export function splitJsonl(text: string): JsonlSplit {
  if (text === '') {
    return { lines: [], discardedTail: null }
  }

  const segments = text.split('\n')
  const isTerminated = text.endsWith('\n')
  const completeSegments = segments.slice(0, -1)
  const lines = completeSegments.map((raw, index) => ({ raw, line: index + 1 }))

  if (isTerminated) {
    return { lines, discardedTail: null }
  }

  const tail = segments.at(-1)!
  try {
    JSON.parse(tail)
    lines.push({ raw: tail, line: segments.length })
    return { lines, discardedTail: null }
  } catch {
    return { lines, discardedTail: tail }
  }
}
