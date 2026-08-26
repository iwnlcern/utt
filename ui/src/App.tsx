import { useState } from 'react'

import GameView from './app/GameView'
import Home from './app/Home'
import type { GameRecord } from './log/gameRecord'
import './App.css'

function App() {
  const [game, setGame] = useState<GameRecord | null>(null)
  const [loadNotice, setLoadNotice] = useState<string | null>(null)
  const resetGame = () => {
    window.history.replaceState(null, '', `${window.location.pathname}${window.location.search}`)
    setGame(null)
    setLoadNotice(null)
  }

  return game === null
    ? <Home onLoaded={(loadedGame, notice) => {
        setLoadNotice(notice)
        setGame(loadedGame)
      }} />
    : <GameView game={game} notice={loadNotice} onExit={resetGame} />
}

export default App
