import { useState } from 'react'

import GameView from './app/GameView'
import Home from './app/Home'
import type { GameRecord } from './log/gameRecord'
import './App.css'

function App() {
  const [game, setGame] = useState<GameRecord | null>(null)
  const resetGame = () => {
    window.history.replaceState(null, '', `${window.location.pathname}${window.location.search}`)
    setGame(null)
  }

  return game === null
    ? <Home onLoaded={setGame} />
    : <GameView game={game} onExit={resetGame} />
}

export default App
