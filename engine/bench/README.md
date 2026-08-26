# Engine rules-core benchmark

The benchmark compares the independent naive position implementation with the optimized cached `Position` implementation by running fixed-seed full playouts in the same executable.

The executable records its own SHA-256 digest, the compiler version, compile flags, CPU brand, fixed seed list, and a fresh session UUID in the reference artifact.

Candidate mode opens the committed reference artifact read-only and refuses to measure if the executable digest, compiler, flags, CPU, seeds, or echoed session UUID differ.

Each mode discards two warmup runs and then records ten measured runs of 2,000,000 plies each.

The playout result is consumed through an inline-assembly sink so the compiler cannot eliminate the measured work.

Candidate acceptance requires both `median_cand_ns_per_ply <= median_ref_ns_per_ply` and `median_cand_ns_per_ply <= 100.0`.

The evidence history is intentionally ordered as benchmark source first, `baseline.json` second, and `candidate-verdict.json` third.

The baseline therefore remains durable even if a later candidate fails, and candidate mode has no path that writes the baseline.

The theory fixture test reads `UTTT_FIXTURES_DIR` when set and otherwise looks for `theory/fixtures` from the repository root.

`UTTT_ALLOW_MISSING_FIXTURES=1` is a development-only override and can never support an acceptance-green claim.

The adapter corpus test looks for `docs/protocol/transcript-v1.jsonl` from the repository root.

`UTTT_ALLOW_MISSING_CORPUS=1` is a development-only override and can never support an acceptance-green claim.

Acceptance runs must leave both missing-artifact overrides unset.
