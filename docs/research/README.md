# Research notes

Research contributions help identify formats, recover names and choose useful
GameCube reconstruction targets. Thanks to [kyleckroeger](https://github.com/kyleckroeger)
for sharing seven investigations from a separate PS2 remake effort, following
[issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4).

## PS2 investigations and GameCube comparisons

These notes target PS2 SLUS-20753 and include scoped comparisons against the pinned
GameCube GR8E69 executable. Each document records its evidence, confidence and open
questions. The contributor used Claude to help prepare and re-check the notes;
maintainer review and corrections used Codex. Preserve that attribution when using
the research.

| Topic | What it helps investigate | Contribution |
| --- | --- | --- |
| [FlexProp names and CRCs](ps2/flexprop-crc-names.md) | 241 candidate or corroborated setting names, signed key ordering and class fields | [PR #5](https://github.com/lifewillbeokay/moh-rising-sun/pull/5) |
| [Particle recipes](ps2/particle-recipes.md) | Recipe entry indices, field types and links to GameCube getters | [PR #6](https://github.com/lifewillbeokay/moh-rising-sun/pull/6) |
| [Script instructions and built-in functions](ps2/script-opcodes.md) | 33 instruction slots and the 583-entry built-in function table | [PR #7](https://github.com/lifewillbeokay/moh-rising-sun/pull/7) |
| [`.sin` class descriptions](ps2/sin-class-descriptions.md) | Script classes, states, events and variable records | [PR #8](https://github.com/lifewillbeokay/moh-rising-sun/pull/8) |
| [`.bpd` gameplay data](ps2/bpd-format.md) | Header tables, light volumes, path finding and endian conversion | [PR #9](https://github.com/lifewillbeokay/moh-rising-sun/pull/9) |
| [Pathfinder music rules](ps2/pathfinder-music.md) | Music events, `.MPF` rules and script-to-music calls | [PR #10](https://github.com/lifewillbeokay/moh-rising-sun/pull/10) |
| [Update ticks and runtime rules](ps2/runtime-rules.md) | Motion timing, animated lights, particles, camera FOV and scene behavior | [PR #11](https://github.com/lifewillbeokay/moh-rising-sun/pull/11) |

## Related EA GameCube references

The [shared-code comparison](ea-shared-code.md) records pinned Rogue Agent,
European Assault and NFS Underground symbol inventories, their limits, and the
Rising Sun reconstructions they helped prioritize.

## Applying the notes

The [explosion and grenade update follow-up](explosion-grenade-updates.md)
records GR8E69 behavior and unresolved matching work for the explosion helpers
and `CThrownBullet::CommitUpdate`, building on PR #47. It also identifies the
sound-event dependencies now accepted as matching source.

The [recovered partial-source batch](shortlist-partials.md) follows that work with
twelve loader, quaternion, script and scene functions. It records complete native
comparisons, shared-context ownership and reproducible isolated PowerPC checks,
while keeping partial similarity separate from exact and linked coverage.

The script dispatch table and named built-in functions provide concrete places to
start reconstructing the script interpreter and small wrappers. The `.sin` and
`.bpd` notes help interpret records used by class lookup and endian-conversion
routines. FlexProp and particle research can extend the already matching
[lookup path](../FlexProp.md) and [recipe accessors](../ParticleRecipes.md).

The first source contribution based on these notes now reconstructs [16 script
handlers, event lookup and three music built-ins](../Script.md), with each accepted
body checked independently against GameCube. That evidence is separate from the
broader PS2 interpretations in the research documents.

The [BPD reconstruction](../BPD.md) also includes matching conversion routines for
the header, property records, animated lights and light volumes, plus a lighting-volume
point test and scalar conversion wrappers. Its scoped views preserve unknown fields
and do not establish complete GameCube file formats.

The [camera reconstruction](../Camera.md) applies the runtime notes' half-angle
finding to matching projection setters, FOV getters, projection-matrix updates
and transform wrappers. Each function and accessed field is checked against
GameCube independently of the PS2 interpretation.

PS2 offsets, endianness and file contents do not automatically establish GameCube
layouts. A CRC match is a naming lead, and a shared symbol or call is evidence for
an interface, not a complete implementation. Check each proposed GameCube function
and storage view against GR8E69 before accepting source. Original PS2 file claims
remain attributed to the contributor where maintainer review could not reproduce
them independently.

These documents earn no matching-source credit. Accepted source must pass the
[complete-image verification and snapshot process](../Progress.md). Keep game
files and bulk research dumps in ignored local directories; see the
[contribution guide](../../CONTRIBUTING.md).

The [PR #59 integration review](pr59-review.md) records the file-wide compiler
profiles, three partial-to-exact replacements, source-review boundaries and
remaining tooling leads from the larger Frontline-guided contribution.

The [broader property/card/animated-texture sweep](broad-source-sweep.md) adapts
the Frontline property readers and records exact, partial and data-blocked work
from one accumulated source batch.

The [Frontline collision batch](collision-batch.md) adapts box, triangle, sphere,
line and capsule tests as complete measured bodies, with exact interval helpers
and explicitly documented numerical limits in the capsule partial.

The [collision distances and database traversal batch](collision-distance-traversal.md)
adds eleven measured partial functions, with shared distance helpers, box queries,
projected visibility bounds and collision-tree traversal. It records target
differences and the unresolved box/capsule intersection contact behavior.

The [frontend, AI, audio and rendering batch](rising-sun-only-batch.md) adds exact
linked functions recovered mostly from the executable itself, and records which few
used Frontline as a semantic reference.

The [FEng animation and list-box follow-up](feng-animation-lists.md) uses the
interfaces contributed in PRs #64 and #65 for 41 more functions, including
measured partial interpolation, key-track, matrix and list-box bodies.

The [FEng package, object and localization batch](feng-packages.md) extends the
PR #64/#65 interfaces with 39 functions covering package parsing, object/script
copying, event dispatch, localization, render traversal and cleanup.

The [FEng input, lifecycle and reader follow-up](feng-lifecycle.md) adds 16 functions
covering controller/mouse processing, package commands, update loops, script tags,
locale tables and type construction, with two exact functions and 14 partials.

The [frontend screen and co-op batch](frontend-screens.md) applies the shared
frontend interfaces to menu navigation, skin checks, keyboard entry, save loading,
briefing text and controller bookkeeping, with measured partials and isolated
PowerPC checks.
