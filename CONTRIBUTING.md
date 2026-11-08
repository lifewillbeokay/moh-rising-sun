# Contributing

AI-assisted work is welcome. Contributors are responsible for verifying proposed source against the original instructions, data and available symbols. Record AI assistance and unresolved assumptions in the change description.

- Keep original executables, disc images, assets, complete debug exports and tool binaries under ignored `orig/`, `build/` or `scratch/`. Commit scripts, source and focused evidence rather than bulk dumps.
- Distinguish recovered names from inferred descriptive names. A symbol's name or address does not establish a complete class layout or original source-file boundary.
- Choose compiler settings from evidence at source-unit or library scope. Do not patch instructions, insert assembly to substitute for compiled C/C++, or exclude differences to manufacture a match.
- Preserve the complete-image comparison when introducing source. Record what was compiled, which toolchain and settings were used, and which exact target bytes matched. Original-object relinking earns no decompilation credit.
- Keep source readable. Put compiler experiments and unresolved hypotheses in evidence notes, not misleading comments about original intent.
- Before accepting reconstructed units, review their behavior, types, boundaries, data ownership and comparison coverage. A byte match alone does not establish source fidelity.

Open an issue to coordinate a unit or research topic, then submit a focused pull
request. Include the target revision, compiler/settings, commands run, match result,
progress change and remaining uncertainties. Documentation and tooling contributions
are useful even without a local game image; clearly label anything not verified.

For source, header, manifest or build-tool changes, run the complete source build
and tests, then refresh the public snapshot with
`python3 tools/progress_report.py --capture`. See [Progress.md](docs/Progress.md).
CI checks the snapshot and runs tests that do not require originals; local
original-dependent tests skip on public runners. Maintainers must review snapshot
updates and reproduce byte verification before accepting new source credit.

Pseudonymous contributions are welcome. Check your commit author/email before
committing and use your own GitHub noreply address if desired. Never remove upstream
author or license notices. AI assistance must be disclosed in the PR, along with
the checks you performed; generated output is a proposal until verified.

Original contributions are submitted under [CC0 1.0 Universal](LICENSE), only for
rights you are entitled to dedicate. Imported material and adaptations retain
their applicable upstream terms: preserve notices and record provenance, including
any licensing uncertainty. See [licensing scope and third-party notices](docs/Licensing.md).

## Partial source and incremental linking

Useful partial reconstructions are welcome in normal source files. Incompleteness
alone is not a review failure. Time-box attempts to resolve compiler differences;
submit readable, measured source with evidence and remaining uncertainties rather
than leaving useful work indefinitely in ignored scratch directories.

Use three distinct states:

- **Partial:** the complete function is compiled and compared, with differences
  reported. Its original bytes remain in the verified image.
- **Exact, unlinked:** the complete function bytes, address and size match; data,
  dependencies and ownership pass comparison checks. It earns exact match credit,
  but earns no linked credit until integration passes the complete-image build.
- **Linked and verified:** the reviewed source range replaces original context
  and the complete rebuilt image still matches.

Register measured work in `config/GR8E69/comparisons.json`; keep its source under
`src/` and shared interfaces under `include/`. Use `DECOMP_COMPARE` for unfinished
bodies/helpers that must stay out of the exact build. The comparison tool reuses
an accepted unit's compiler profile; the define selects bodies, not optimization
settings. Do not patch generated instructions, hide differences, or use a high
similarity score as proof of correctness. Unknown types and behavior still need
review and explicit evidence.

Run `python3 tools/comparisons.py` while iterating. Submit the full source build,
tests and refreshed snapshot as usual; capture recompiles all registered
comparisons. See [the comparison contract and promotion steps](docs/Progress.md#partial-source-workflow).
Exact functions can be integrated as reviewed fragments while other functions in
the same original file remain partial. Whole-file perfection is not a prerequisite.
