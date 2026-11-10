# Frontend, AI, audio and rendering batch

This batch adds 1,163 exact, linked functions covering 129,628 code bytes:
1,076,936 to 1,206,564 of 2,492,032 (43.215175% to 48.416874%). All of it is
reconstructed game code. Claude Code assisted the reconstruction.

## Evidence

Only 16 of these functions (1,724 bytes) have a matched counterpart in
kyleckroeger's [Medal of Honor: Frontline decompilation](https://github.com/kyleckroeger/moh-frontline-decomp),
pinned at **`444ddde017580366b133a92b2d89109829738e12`** (CC0 1.0). It served as a
semantic reference there; no CodeWarrior-specific declarations were imported.
Everything else comes from the pinned GR8E69 executable: its disassembly, symbol
names and data. Names found in the symbol table are evidence. Member, local and
helper names are inferred, and the subsystem documents separate the two.

A byte match establishes the generated code, not the original statements or types.
Some store orders were found by searching source orderings against the target. That
search only reordered existing statements.

## Subsystem notes

- [Frontend](../Frontend.md): `CGameSettings`, FEng lists, packages, objects and scripts,
  `ShellManager`, `CPauseMenu`.
- [Behaviors](../Behaviors.md): AI behaviors, bots, area path nodes and path finding.
- [Sound](../Sound.md), [Memory](../Memory.md), [Rendering](../Rendering.md),
  [Particle recipes](../ParticleRecipes.md), [Animation](../Animation.md) and
  [Scene objects](../SceneObjects.md): the remaining groups.

Unit and fragment file names are project choices, because functions that are not
contiguous can't share a unit. No Dolphin or game runtime test was performed.

## Second batch

A follow-up adds 720 exact, linked functions covering 131,840 code bytes: 1,206,564
to 1,338,404 of 2,492,032 (48.416874% to 53.707336%). Again only 5 functions (1,432
bytes) have a Frontline counterpart; the same pinned revision applies. The rest come
from the executable. The evidence rules above apply, and [Frontend](../Frontend.md),
[Behaviors](../Behaviors.md), [Animation](../Animation.md), [Loading](../Loading.md),
[Sound](../Sound.md), [Rendering](../Rendering.md) and [Memory](../Memory.md) record
what is verified and inferred. No Dolphin or game runtime test was performed.

## Review corrections

The MAD variable-length decoder now reads the zigzag index in both operands of
the coefficient assignment and increments the position in a separate statement.
This removes an unsequenced read/write of the temporary index while preserving
the submitted unit's complete object and linked-file hashes (456 exact code
bytes). The `gchw_vd_setters` manifest also uses the documented RCMP-wide
`-fno-strength-reduce` profile; its two setters still match all 92 code bytes.
Neither correction changes exact coverage. Codex assisted the review and these
corrections; the working profile does not establish the historical build flags.
