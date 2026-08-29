# UTTT certificate-schema conformance corpus

This emitter-side package materializes the certificate vectors, verdict, named
mutants, and catalogue required by §9 of the UTTT weak-solve certificate design.
It is shared test truth for the cycle-c3 checker and the solver-side emitter.

The schema source of record is
`docs/sprints/active/2026-08-28-uttt-weak-solve/designs/DD-theory-c2-20260828.md`
at SHA-256
`629801c4b1cfc69f66390c47b72296d93867e46ac22ea142e9bfd2bdc8f41f0c`.

The generator requires the `xxhsum` CLI from xxHash 0.8 or newer. The test suite
pins the canonical XXH3-128 empty-input vector, the DD Appendix B certificate
and verdict bytes, and fresh-regeneration equality.

Regenerate from `theory/`:

```sh
uv run python -m certschema.generate
```

Generation is deterministic: the committed vectors, 42 mutant files, adjacent
verdict-member certificate, and sorted JSON catalogue must byte-equal a fresh
run.

This package is emitter-side test tooling only. The cycle-c3 checker must not
import it or share its game logic, preserving the independent implementations
required by rulings R-s2-2 and R-s2-3.
