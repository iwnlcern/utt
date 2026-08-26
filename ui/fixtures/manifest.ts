type Terminal = {
  result: 'X' | 'O' | 'draw' | 'void'
  reason:
    | 'macro_win'
    | 'chip_count'
    | 'exact_tie_draw'
    | 'hello_fault'
    | 'recovery_fault'
    | 'triple_double_fault_void'
}

type FixtureExpected = {
  terminal?: Terminal
  rawJsonl: 'complete' | 'truncated-tail' | 'missing-game-end' | 'malformed-interior'
}

type FixtureProvenance = {
  legalBase: string
  validation: string
  derivation?: string
}

const ownerReplayValidation = (path: string) =>
  `cd referee && uv run python -c "import json,sys; from pathlib import Path; from jsonschema import Draft202012Validator; from poorman_referee.gamelog import read_log,replay_frames; schemas=Path('../docs/protocol/schema'); events=read_log(sys.argv[1]); names={'game_start':'log_game_start','auction':'log_auction','recovery':'log_recovery','game_end':'log_game_end'}; [Draft202012Validator(json.loads((schemas / (names[event['event']] + '.schema.json')).read_text(encoding='utf-8'))).validate(event) for event in events]; replay_frames(events)" ${path}`

const ownerBaseValidation = (path: string) => ownerReplayValidation(path)

export const fixtureManifest = [
  {
    name: 'both-zero',
    path: 'both-zero.jsonl',
    expected: { terminal: { result: 'draw', reason: 'exact_tie_draw' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/poorman_referee/referee.py GameConfig/play_game; random_bot seeds X=27, O=1027',
      validation: ownerReplayValidation('../ui/fixtures/both-zero.jsonl'),
    },
  },
  {
    name: 'chip-count',
    path: 'chip-count.jsonl',
    expected: { terminal: { result: 'X', reason: 'chip_count' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/poorman_referee/referee.py GameConfig/play_game; fraction_bot X seed=46 fraction_ppb=500000000 vs random_bot O seed=1046',
      validation: ownerReplayValidation('../ui/fixtures/chip-count.jsonl'),
    },
  },
  {
    name: 'double-fault-retry',
    path: 'double-fault-retry.jsonl',
    expected: { terminal: { result: 'X', reason: 'macro_win' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/poorman_referee/referee.py GameConfig/play_game, matching referee/tests/test_referee_e2e.py::test_one_double_fault_recovers_both_then_retries_attempt_two',
      validation: ownerReplayValidation('../ui/fixtures/double-fault-retry.jsonl'),
    },
  },
  {
    name: 'exact-tie-draw',
    path: 'exact-tie-draw.jsonl',
    expected: { terminal: { result: 'draw', reason: 'exact_tie_draw' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/parity-even/game-0001-r0001-g2.jsonl',
      validation: ownerReplayValidation('../ui/fixtures/exact-tie-draw.jsonl'),
    },
  },
  {
    name: 'fault-single',
    path: 'fault-single.jsonl',
    expected: { terminal: { result: 'O', reason: 'macro_win' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/fault-illegal_move.jsonl',
      validation: ownerReplayValidation('../ui/fixtures/fault-single.jsonl'),
    },
  },
  {
    name: 'ghost-divergence',
    path: 'ghost-divergence.jsonl',
    expected: { terminal: { result: 'X', reason: 'macro_win' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/parity-even/game-0001-r0001-g1.jsonl',
      derivation: 'Added advisory X info with both pv cells distinct from the logged applied move; game events otherwise retained verbatim.',
      validation: ownerReplayValidation('../ui/fixtures/ghost-divergence.jsonl'),
    },
  },
  {
    name: 'hello-fault',
    path: 'hello-fault.jsonl',
    expected: { terminal: { result: 'O', reason: 'hello_fault' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/poorman_referee/referee.py GameConfig/play_game, matching referee/tests/test_referee_e2e.py::test_hello_crash_is_forfeit_and_failed_terminal_delivery',
      validation: ownerReplayValidation('../ui/fixtures/hello-fault.jsonl'),
    },
  },
  {
    name: 'malformed-info',
    path: 'malformed-info.jsonl',
    expected: { terminal: { result: 'X', reason: 'macro_win' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/parity-even/game-0001-r0001-g1.jsonl',
      derivation: 'Added schema-valid advisory info object whose UI metadata fields have intentionally unusable types.',
      validation: ownerReplayValidation('../ui/fixtures/malformed-info.jsonl'),
    },
  },
  {
    name: 'malformed-interior',
    path: 'malformed-interior.jsonl',
    expected: { rawJsonl: 'malformed-interior' },
    provenance: {
      legalBase: 'referee/tests/fixtures/success.jsonl',
      derivation: 'Inserted newline-terminated non-JSON bytes after the legal game_start event for UI fail-closed parsing.',
      validation: ownerBaseValidation('../referee/tests/fixtures/success.jsonl'),
    },
  },
  {
    name: 'missing-game-end',
    path: 'missing-game-end.jsonl',
    expected: { rawJsonl: 'missing-game-end' },
    provenance: {
      legalBase: 'referee/tests/fixtures/success.jsonl',
      derivation: 'Complete-event prefix with the legal base terminal event removed for UI partial-log handling.',
      validation: ownerBaseValidation('../referee/tests/fixtures/success.jsonl'),
    },
  },
  {
    name: 'no-info',
    path: 'no-info.jsonl',
    expected: { terminal: { result: 'X', reason: 'macro_win' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/parity-even/game-0001-r0001-g1.jsonl',
      validation: ownerReplayValidation('../ui/fixtures/no-info.jsonl'),
    },
  },
  {
    name: 'post-auction-recovery-eof',
    path: 'post-auction-recovery-eof.jsonl',
    expected: { rawJsonl: 'missing-game-end' },
    provenance: {
      legalBase: 'referee/tests/fixtures/recovery-single-fail.jsonl',
      derivation: 'Complete-event prefix ending after its resolved auction and keyed recovery, with only terminal removed.',
      validation: ownerBaseValidation('../referee/tests/fixtures/recovery-single-fail.jsonl'),
    },
  },
  {
    name: 'recovery-fault-abort',
    path: 'recovery-fault-abort.jsonl',
    expected: { terminal: { result: 'O', reason: 'recovery_fault' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/recovery-double-one-fail.jsonl',
      validation: ownerReplayValidation('../ui/fixtures/recovery-fault-abort.jsonl'),
    },
  },
  {
    name: 'recovery-fault-post-resolve',
    path: 'recovery-fault-post-resolve.jsonl',
    expected: { terminal: { result: 'O', reason: 'recovery_fault' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/recovery-single-fail.jsonl',
      validation: ownerReplayValidation('../ui/fixtures/recovery-fault-post-resolve.jsonl'),
    },
  },
  {
    name: 'success-macro-win',
    path: 'success-macro-win.jsonl',
    expected: { terminal: { result: 'X', reason: 'macro_win' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/tests/fixtures/parity-even/game-0001-r0001-g1.jsonl',
      derivation: 'Added advisory X info with quality=bound, lo, hi, depth, and complete; game events otherwise retained verbatim.',
      validation: ownerReplayValidation('../ui/fixtures/success-macro-win.jsonl'),
    },
  },
  {
    name: 'trailing-recovery',
    path: 'trailing-recovery.jsonl',
    expected: { rawJsonl: 'missing-game-end' },
    provenance: {
      legalBase: 'referee/tests/fixtures/recovery-double-one-fail.jsonl',
      derivation: 'Complete-event prefix ending after the first pre-auction double-fault recovery; referenced auction is absent.',
      validation: ownerBaseValidation('../referee/tests/fixtures/recovery-double-one-fail.jsonl'),
    },
  },
  {
    name: 'trailing-recovery-xo',
    path: 'trailing-recovery-xo.jsonl',
    expected: { rawJsonl: 'missing-game-end' },
    provenance: {
      legalBase: 'referee/tests/fixtures/recovery-double-one-fail.jsonl',
      derivation: 'Complete-event prefix ending after pre-auction X then O double-fault recoveries; referenced auction is absent.',
      validation: ownerBaseValidation('../referee/tests/fixtures/recovery-double-one-fail.jsonl'),
    },
  },
  {
    name: 'truncated-line',
    path: 'truncated-line.jsonl',
    expected: { rawJsonl: 'truncated-tail' },
    provenance: {
      legalBase: 'referee/tests/fixtures/success.jsonl',
      derivation: 'Appended unterminated non-JSON bytes after the legal complete transcript for UI crash-tail handling.',
      validation: ownerBaseValidation('../referee/tests/fixtures/success.jsonl'),
    },
  },
  {
    name: 'void-triple-double-fault',
    path: 'void-triple-double-fault.jsonl',
    expected: { terminal: { result: 'void', reason: 'triple_double_fault_void' }, rawJsonl: 'complete' },
    provenance: {
      legalBase: 'referee/poorman_referee/referee.py GameConfig/play_game, matching referee/tests/test_referee_e2e.py::test_three_double_faults_void_the_game',
      validation: ownerReplayValidation('../ui/fixtures/void-triple-double-fault.jsonl'),
    },
  },
] as const satisfies readonly {
  name: string
  path: string
  expected: FixtureExpected
  provenance: FixtureProvenance
}[]
