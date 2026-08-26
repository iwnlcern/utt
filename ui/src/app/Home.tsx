import { useRef, useState } from 'react'

import type { GameRecord } from '../log/gameRecord'
import { LogError, parseGameLog } from '../log/validate'
import { clearRecents, readRecents, recentForGame, saveRecent, type RecentGame } from './recents'

type FileSource = {
  kind: 'file'
  name: string
  file: File
}

type TextSource = {
  kind: 'text'
  name: string
  text: string
}

type SampleSource = {
  kind: 'sample'
  name: 'sample-game.jsonl'
}

type LoadFailure = {
  source: FileSource | TextSource | SampleSource
  error: LogError | Error
}

export type HomeProps = {
  onLoaded: (game: GameRecord) => void
}

const readFile = (file: File): Promise<string> => new Promise((resolve, reject) => {
  const reader = new FileReader()
  reader.onerror = () => reject(reader.error ?? new Error(`Could not read ${file.name}`))
  reader.onload = () => typeof reader.result === 'string'
    ? resolve(reader.result)
    : reject(new Error(`Could not read ${file.name}`))
  reader.readAsText(file)
})

const isJsonlFile = (file: File): boolean => file.name.toLowerCase().endsWith('.jsonl')

function Home({ onLoaded }: HomeProps) {
  const inputRef = useRef<HTMLInputElement>(null)
  const [recents, setRecents] = useState<RecentGame[]>(readRecents)
  const [notice, setNotice] = useState<string | null>(null)
  const [failure, setFailure] = useState<LoadFailure | null>(null)

  const openSource = async (source: FileSource | TextSource): Promise<void> => {
    try {
      const text = source.kind === 'text' ? source.text : await readFile(source.file)
      const game = parseGameLog(text)
      const saved = saveRecent(recentForGame(source.name, text, game))
      setRecents(saved.recents)
      setNotice(saved.persisted ? null : 'Browser storage is full. Recent games are available for this session only.')
      setFailure(null)
      onLoaded(game)
    } catch (error) {
      setFailure({ source, error: error instanceof Error ? error : new Error('Could not open this game log') })
    }
  }

  const openFile = (file: File | undefined): void => {
    if (file === undefined) return
    if (!isJsonlFile(file)) {
      setFailure({ source: { kind: 'file', name: file.name, file }, error: new Error('Choose a .jsonl game log') })
      return
    }
    void openSource({ kind: 'file', name: file.name, file })
  }

  const openSample = async (): Promise<void> => {
    const source: SampleSource = { kind: 'sample', name: 'sample-game.jsonl' }
    try {
      const response = await fetch('/sample-game.jsonl')
      if (!response.ok) throw new Error('Could not load the bundled sample game')
      await openSource({ kind: 'text', name: source.name, text: await response.text() })
    } catch (error) {
      setFailure({
        source,
        error: error instanceof Error ? error : new Error('Could not load the bundled sample game'),
      })
    }
  }

  if (failure !== null) {
    const { error, source } = failure
    return (
      <main className="home home--failure">
        <h1>Could not open {source.name}</h1>
        <p role="alert">{error instanceof LogError ? 'This log is malformed.' : error.message}</p>
        {error instanceof LogError && (
          <dl>
            <div><dt>Line</dt><dd>Line {error.line}</dd></div>
            <div><dt>Event index</dt><dd>Event {error.event_index}</dd></div>
            <div><dt>Reason</dt><dd>{error.reason}</dd></div>
          </dl>
        )}
        <button
          type="button"
          onClick={() => source.kind === 'sample' ? void openSample() : void openSource(source)}
        >
          Retry {source.name}
        </button>
        <button type="button" onClick={() => setFailure(null)}>Choose another file</button>
      </main>
    )
  }

  return (
    <main className="home">
      <h1>Poorman UTTT Replay</h1>
      <p>Open a recorded game to replay its logged moves and analysis.</p>
      {notice !== null && <p role="alert">{notice}</p>}
      <input
        ref={inputRef}
        aria-label="Choose a JSONL game log"
        type="file"
        tabIndex={-1}
        accept=".jsonl,application/jsonl"
        onChange={(event) => openFile(event.currentTarget.files?.[0])}
      />
      <section
        aria-label="Drop a JSONL game log"
        onDragOver={(event) => event.preventDefault()}
        onDrop={(event) => {
          event.preventDefault()
          openFile(event.dataTransfer.files[0])
        }}
      >
        <p>Drop a .jsonl game log here</p>
        <button type="button" onClick={() => inputRef.current?.click()}>Choose a .jsonl file</button>
      </section>
      <button type="button" onClick={() => void openSample()}>Open bundled sample game</button>
      <section aria-labelledby="recent-games-heading">
        <h2 id="recent-games-heading">Recent games</h2>
        {recents.length === 0 ? <p>No recent games yet.</p> : (
          <ul>
            {recents.map((recent) => (
              <li key={`${recent.name}-${recent.opened_at}`}>
                <button type="button" onClick={() => void openSource({ kind: 'text', name: recent.name, text: recent.log_text })}>
                  Open recent {recent.name} ({recent.result})
                </button>
              </li>
            ))}
          </ul>
        )}
        <button
          type="button"
          disabled={recents.length === 0}
          onClick={() => {
            clearRecents()
            setRecents([])
            setNotice(null)
          }}
        >
          Clear recent games
        </button>
      </section>
    </main>
  )
}

export default Home
