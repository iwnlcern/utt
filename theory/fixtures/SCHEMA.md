# Rules fixture schema v1

This contract incorporates the forced-board encoding acknowledged in
`s1/theory-c1/SITREP-orchestrator-planner-20260825-080619.md`: `forced` is an
integer from 0 through 8, or `null` for free choice.

Each category file is a JSON envelope with `schema_version`, `game`, and a
`fixtures` array. Every fixture repeats the envelope's game discriminator and
has a stable `id` plus a non-empty `consumed_by` list. Consumers MUST skip a
fixture unless their own name occurs in that list.

UTTT cells and local boards use row-major indices 0 through 8. A UTTT board is
an array of nine nine-character strings over `.`, `X`, and `O`; a ttt3 board is
one such string. UTTT moves are `[local, cell]`, while ttt3 moves are cell
indices. Budgets and bids are non-negative integer units. Exact theory values
are reduced rational strings such as `"3/4"`; fixture files never use floats.

Closure records are ordered by local-board index and retain the closing result
(`X`, `O`, or `full`). `expected_terminal` records the terminal kind, not a
boolean. Auction traces record the move actually applied after resolving a bid;
the first-move coin appears only when a root tie consults it. Terminal chip
margins are measured after the winning bid has been paid.

`schema-v1.json` is normative. This prose summarizes its wire conventions and
does not independently extend them.
