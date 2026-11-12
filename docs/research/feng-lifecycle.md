# FEng input, lifecycle and readers

This follow-up uses the frontend interfaces and callees contributed by
kyleckroeger in [PR #64](https://github.com/lifewillbeokay/moh-rising-sun/pull/64)
and [PR #65](https://github.com/lifewillbeokay/moh-rising-sun/pull/65), together
with the [animation/list-box](feng-animation-lists.md) and
[package/localization](feng-packages.md) batches. Those shared layouts made it
possible to reconstruct the larger update and input paths without introducing
separate class views. Thank you for that groundwork.

The bodies were reconstructed with AI assistance from the pinned GR8E69 symbols,
instructions and data. Ghidra 12.1.4 exports were independently checked against
original function boundaries and loaded bytes. No additional Frontline or other
game source was copied. Original binaries, bulk exports and experimental builds
remain ignored.

## Scope and accounting

The batch covers **16 functions / 14,748 original code bytes**. Two functions
are exact and registered for linking: `FEngine::PushPackage` (460 bytes) and
`FETypeLib::Startup` (752 bytes). The other **14 functions / 13,536 bytes** are
maintained partials in 12 comparison units:

- Controller aggregation/repeats, mouse presses/releases and hover selection,
  directional button search and context-aware button positions.
- Engine update steps, response dispatch, queued package commands, package
  construction/destruction and per-object script updates.
- Locale tables, script/key/event tags, and base/image type construction.

Original code stays in the verified image for every partial. These partials
receive measured similarity, not exact or linked credit. The shared FEng profile
is ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates`;
comparisons inherit `fecode_setcellcolor.json` and guard their bodies with
`DECOMP_COMPARE`. No instruction patches, per-function flags, discarded target
functions or clipped data pools are used.

## Evidence and preserved behavior

`FEJoyPad.h` shares the already verified 136-byte controller declaration.
`FEngine` retains the old byte-array aliases and adds descriptive views for its
input-repeat mask at `+0x48`, 19 repeat amounts at `+0x4c`, and 19 pressed-object
pointers at `+0x98`. The `FEButtonWrapMode` enum tag is symbol evidence; its mask
values remain unnamed apart from the inferred zero value. These names describe
observed use, not recovered original member names.

The type-library name globals are pointer variables, not inline strings. Their
original file-local binding is recorded in the manifest. Full generated pools
are verified at `0x8029860c` (base type), `0x80298664` (image type) and
`0x80298684` (startup). Navigation's complete constants start at `0x80297148`;
`DirectionVectors`, `PassOffsets` and `PassWrapMode` remain external. The locale
reader's pool starts at `0x802978d8`. Integer-to-double conversion constants for
mouse input and object updates belong to `0x80297c08` and `0x80297e90`.

Several behaviors are intentionally left as the original implements them:

- Package push tries the full name and then a backward basename scan with a
  signed-byte length. With no separator that scan can pass the byte before the
  name. Localization runs before the loaded-package null check, and a negative
  localization result returns before freeing the supplied buffer.
- Engine update processes active-pad input before package updates, consumes
  repeat times on all pads, and divides elapsed time into steps of at most 20.
  Zero and negative elapsed values still execute one step. Executing updates
  cache the next package before calling it; the nonexecuting traversal does not.
- Input preserves the accept-button alias, separate pressed/released/held port
  masks, 120/320 repeat delays, directional precedence and the first accepted
  impulse's immediate return. The held-message path uses the original pressed
  port mask. A navigation change releases stored button objects before selecting
  the next one. Mouse hit testing distinguishes an exact return of one from a
  general nonzero result on the scan path.
- Directional navigation keeps the five wrap passes, disabled-button filter,
  distance/angle cost and unordered floating-point branches. NaN is not silently
  converted to a rejected candidate. Position comes from the live position field
  and the game-interface context transform when available.
- Object scripts advance by the package's stored delta, which is distinct from
  the elapsed argument passed to list/group/movie updates. Clamp, loop, ping-pong,
  chained scripts, event windows and dirty-flag retention keep the original order.
- Response dispatch preserves target sentinels, remembered-button fallback,
  conditional branches, per-port pushes and marker commands. Package commands
  preserve parent links, control-mask transfers and port remapping. A failed push
  during a switch still has the original null dereference; this is not repaired
  by the reconstruction.
- Script tags preserve little-endian words, signed-byte track offsets, the
  24-bit key-time field, base/delta keys, reference-list detachment, sorted track
  insertion and event arrays. Key-data and event parsing retain their original
  do-loops; malformed zero-entry event records are not made safe here.
- Locale parsing accepts the final filled line even when its reader returns
  zero. It retains the header/language search, metadata offsets, label indexing
  and per-language wide-string assignment. Package teardown preserves array
  cookies, pending-node owned strings and member destruction order.

## Validation and limits

`tests/test_feng_lifecycle.py` adds **15 test groups / 2,186 scenarios**, covering
all 16 new functions with original/native PowerPC execution under optional
Unicorn 2.1.4. The cases include inactive/combined controller masks, repeat
thresholds, input disabling during message processing, held/pressed/released
buttons, hover changes, every response opcode implemented here, package command
sequences, loading failures, teardown, script timing/flags, all implemented script
tags, track insertion positions, locale headers/metadata/final lines, and type
field defaults.

Memory, integer returns and callback ordering compare exactly. Floating values
use the shared six-decimal normalization. Allocation and selected engine/game
callbacks are stubbed. These checks do not establish equivalence for arbitrary
callback mutations, aliasing, malformed files, allocation failure or untested
numeric inputs. They do not execute the game; runtime remains untested.

Original-object baseline, complete source-image verification, native comparison
measurements and behavioral tests are separate checks. Public CI validates a
locally verified snapshot and exports the report; it does not rebuild with the
proprietary compiler or have access to original game files.

Local validation passed the original-object baseline (2,860,576-byte normalized
DOL), the full source rebuild of **2,604 accepted units**, all **108 comparison
units**, and **175 tests** with Unicorn available and no skipped tests. The
rebuilt image is byte-identical. Linked source is **1,341,048 bytes** across
7,049 functions: 948,832 reconstructed-game bytes and 392,216 restored-library
bytes. Including the existing 488 exact-unlinked bytes, exact source totals
**1,341,536 / 2,492,032 bytes (53.833017%)**. Snapshot capture recompiles
comparisons and records the verified inputs.

## Follow-up

The track overload of `FEInterpLinear` remains research only: its null-key branch
needs further review before a useful maintained reconstruction. Static initializer
wrappers also remain out of this batch. Closer compiler scheduling for
`FEPackage::UpdateObject` and the constructor, and independent promotion of the
previous batch's exact-unlinked helpers, are narrower follow-up opportunities.

## Recorded comparison measurements

Complete-function measurements from objdiff 3.8.3, weighted by original function
size within each fragment. Differing bytes are a separate byte/size comparison;
a high similarity score is not a claim of exactness or behavioral equivalence.

| Source fragment | Functions | Original bytes | Similarity | Differing bytes |
| --- | ---: | ---: | ---: | ---: |
| `febutton_navigation` | 2 | 820 | 62.69% | 654 |
| `felocale_read_data` | 1 | 960 | 80.77% | 751 |
| `fengine_mouse` | 1 | 1,440 | 85.29% | 1,269 |
| `fengine_package_commands` | 1 | 728 | 78.23% | 549 |
| `fengine_pads` | 1 | 3,092 | 29.78% | 2,725 |
| `fengine_responses` | 1 | 1,172 | 83.14% | 946 |
| `fengine_update` | 1 | 752 | 84.04% | 618 |
| `fepackage_lifetime` | 2 | 628 | 97.22% | 175 |
| `fepackage_update_object` | 1 | 1,144 | 95.02% | 90 |
| `fereader_script_tags` | 1 | 2,132 | 33.18% | 1,855 |
| `fetype_create_base` | 1 | 452 | 74.21% | 333 |
| `fetype_create_image` | 1 | 216 | 63.30% | 192 |
