import { useState } from 'react'

import GameView from './app/GameView'
import Home from './app/Home'
import type { GameRecord } from './log/gameRecord'
import './App.css'

function App() {
  const [game, setGame] = useState<GameRecord | null>(null)

  return game === null
    ? <Home onLoaded={setGame} />
    : <GameView game={game} onExit={() => setGame(null)} />
}

export default App
