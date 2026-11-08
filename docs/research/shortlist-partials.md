# Recovered partial-source batch

This batch maintains twelve functions across eleven comparison units, covering
8,176 target executable bytes from the pinned GR8E69 Disc 1 ELF (SHA-1
`dd797412be7d631e78c98f194a1bb47981174d75`). Codex assisted recovery, reconstruction
and verification. The candidates were compiled and reviewed together before one
complete source build and snapshot refresh. They all remain partial: similarity
is separate from exact and linked coverage, and their original code remains in
the verified image.

| Group | New functions | Target bytes | Evidence |
| --- | ---: | ---: | --- |
| Loader and symbol pool | 3 | 3,508 | [Loading](../Loading.md#maintained-partial-loading-functions) |
| Stateless quaternion evaluators | 2 | 2,380 | [Animation](../Animation.md#partial-stateless-quaternion-evaluators) |
| Script built-ins | 4 | 1,428 | [Script](../Script.md#maintained-partial-built-ins) |
| Static-object and tank helpers | 3 | 860 | [Scene objects](../SceneObjects.md#maintained-partial-scene-helpers) |

Each comparison inherits an accepted ProDG 3.8.1 profile with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. There are no function-specific
flags, instruction patches, discarded functions or clipped constant pools.
All 628 generated data bytes are checked, including 32 bytes already owned by
the exact alias-playback context. That context retains its original bytes and
addresses and earns no duplicate progress. Native code size/address differences
are kept visible, including the shifted masked-quaternion entry.

The loader and animation fragments retain the pinned NFS reference attribution
and CC0 notices in their source directories. PR #31 supplied useful script
near-match leads. The three scene bodies were rebuilt from the original using
the current shared views and the attributed Frontline reference; old private
drafts were not available. Field/helper names remain descriptive and do not
establish historical source-file boundaries or complete class allocations.

## Differential checks

`tests/test_shortlist_partials.py` runs 1,023 isolated original/native PowerPC
scenarios using the optional Unicorn package (tested with 2.1.4):

| Fixtures | Scenarios | Scope |
| --- | ---: | --- |
| Dynamic lookup | 10 | Null handle, absolute/relative symbol, type mismatch, missing entry and valid/invalid callback. |
| Symbol search | 4 | Missing table, direct hit, collision and failed wraparound search. |
| Loader initialization | 14 | Contiguous/separate relocation storage, header/section/symbol swapping, string conversion, hash/list/ownership setup and unsupported section diagnostics. |
| Quaternion evaluation | 864 | Dense/sparse keys, FPS mode, previous-key histories, six times, masks and optional F3 companion. |
| Masked evaluator entry | 15 | Direct entry at its moved native address, three keys and five masks. |
| Static initialization | 4 | Initial flag patterns, reset fields, preserved fourth vector word and surrounding bytes. |
| Static animation attempt | 40 | Motion gates, negative/zero/fractional/large/NaN steps, volume callbacks and position fallback. |
| Tank weapon insertion | 36 | Three valid slots, sentinel CRC, allocation success/failure and signed property values. |
| Script expression | 24 | Current/pending/requested expressions and zero/nonzero times. |
| Alias looping | 6 | Zero/`none`/ordinary aliases and looping results. |
| Soldier query | 2 | Integer and float return paths. |
| Vibration | 4 | Single/multiplayer and player/nonplayer script nodes. |

Integer results, memory snapshots and recorded call order compare exactly.
Floating-point traces round to six decimal places; this is not bitwise floating
equivalence. Quaternion checks preserve untouched scale/translation slots and
explicitly check that skipping a constant bone does not consume its packed data.
Tests use native symbol addresses, rather than assuming later partial functions
stay at their original offsets.

External string routines, allocation, sound lookup, scene/volume queries, matrix/
quaternion callees, weapon creation, script queries and F3 evaluation are stubbed
as needed. These fixtures are not exhaustive equivalence, allocator/asset validation
or a Dolphin/game test. The public suite skips them without the pinned original,
native comparison outputs or Unicorn. To reproduce locally in that environment:

```sh
python3 tools/comparisons.py
python3 -m unittest discover -s tests -p test_shortlist_partials.py -v
```

## Remaining shortlist

The other eight script near-matches, player transform helpers, bullet-sprite
cleanup and Frontline property-field readers remain future work. Run-blender
alignment still needs explicit shared ownership of its constant pool before it
can enter this workflow. None is credited by this batch. See the topic documents
for current registrations; old research lists and historical instruction counts
can be stale.
