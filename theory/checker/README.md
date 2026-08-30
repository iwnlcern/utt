# UTTT independent checker

This is the independent C3 checker specified by `DD-theory-c2-20260828.md`
sections 6 and 7. It verifies one UTTT certificate or a verdict manifest and
prints exactly one machine-readable JSON report. Deliverable mode is the
default and additionally requires the canonical initial root; use subgame mode
to verify the committed non-initial-root conformance vectors.

## Toolchain

Install C3 with Homebrew and confirm the pinned compiler before building:

```sh
brew install c3c
c3c --version
```

The required version line is:

```text
C3 Compiler Version:       0.8.3
```

Every JSON report embeds that exact line. A different compiler version is not
part of this verification target.

## Build, test, and run

Run these commands from `theory/checker`:

```sh
c3c build
c3c test
build/uttt-checker cert ../certschema/vectors/golden-terminal-winx.utc
build/uttt-checker verdict ../certschema/vectors/golden-winx.utv --mode subgame
build/uttt-checker verdict ../certschema/vectors/golden-winx.utv
```

The executable is written to `build/uttt-checker`. Certificate and verdict
passes exit 0, semantic or structural rejects exit 1, and usage or top-level
I/O errors exit 2. `verdict` accepts `--mode deliverable|subgame` and defaults
to `deliverable`.

From `theory`, run the corpus-wide checker battery or the complete theory suite:

```sh
uv run pytest tests/test_checker_battery.py -v
uv run pytest
```

## Independence and scope

The checker is a fresh implementation under rulings R-s2-2 and R-s2-3. Its
only shared inputs are the design document, fixtures, and committed corpus
bytes. Checker code must not import `certschema`, `engine`, or referee logic.
The Python battery may use `certschema` only as emitter-side tooling to author
checker input, such as the valid orphan-bearing certificate.

The normative first-failure order and reason-code table are recorded in
`PL-theory-c3-20260829.md` and implemented in `src/verify.c3`. The checker uses
an in-memory verified-state set. External-memory verification, sharding, and
performance work belong to theory-c6 and are intentionally outside this
project.
