# Property, memory-card and animated-texture sweep

This batch reconstructs **45 complete functions across three areas**: 41 exact
functions / **9,640 executable bytes**, plus four maintained partial functions /
**1,044 target bytes**. Partial bytes are not linked into the verified image.
The aim is to amortize full rebuilds across a larger set of related functions:
probe fragments independently, keep useful partials measurable, then verify the
accumulated exact fragments together. Codex assisted reconstruction and review.

## Frontline reuse and property readers

Thanks to kyleckroeger for the [Frontline property-reader reconstruction](https://github.com/kyleckroeger/moh-frontline-decomp/blob/b7b6fffaba0edd2989a8a8b02977c1236c72b68b/src/game/bsmachin_props.cpp).
The thirteen `src/bpd/property_read_*.cpp` fragments and `PropertyReaders.h` adapt
that source under its [CC0 1.0 dedication](https://github.com/kyleckroeger/moh-frontline-decomp/blob/b7b6fffaba0edd2989a8a8b02977c1236c72b68b/LICENSE).
The source revision is pinned above; the same CC0 text is supplied by the project
[license](../../LICENSE). Attribution and provenance are retained here and in the
source comments. This is reconstructed game code, not restored-library credit.

The reference supplied the field-switch structure and offset names. Independent
GR8E69 instruction checks establish the following target differences:

- Core selectors 18 and 20 read the two high bits of the word at `+0x34`.
  The reference reads the low bits of a byte. The shared core view now overlays
  those bits with the existing byte/halfword fields; existing offsets are retained.
- Waypoint selectors 22–28 read bits 31–25 of the word at `+0x70`, and selector
  29 reads the low bit of the byte there. These are big-endian target accesses.
- Rising Sun has fourteen dispatch IDs; the Frontline particle reader and its
  dispatch case are absent here. IDs 9–14 therefore select projectile generator,
  projectile target, camera shaker, player path, animated light and fog.
- Converted signed halfwords return IEEE single-precision **bits**, not a numeric
  integer conversion. Each conversion divides by 16. Local float arrays retain
  distinct conversion slots, reproducing the target's separate stack stores.
  These arrays model local scratch storage; no historical declaration is asserted.

The thirteen readers and dispatcher match **7,744 bytes**. Names of property
members are offset labels, not claims of recovered semantic names. The views
extend existing BPD prefixes and do not establish a complete file parser or
allocation size for every property kind. Unsupported selectors return zero.
Record pointers must satisfy the original validity and type contracts.

The fog reader's **528 instruction bytes match in a local probe**, but its
native 16-byte constant pool includes four alignment bytes where the original
already starts `"Unknown property type\n"`. That string belongs to the accepted
`PatchUpAllPropertyData` fragment. The fog draft stays in ignored scratch and
receives **no exact, partial or linked credit**. Solving neighboring data ownership
is a follow-up; neither padding nor the string is clipped or duplicated.

## Animated textures

Fourteen exact functions / **1,084 bytes** cover shape lookup, name encoding,
hex conversion, CRC controls, global-list insertion/removal, frame selection,
frame-step/update wrappers and shape-offset resolution. The latter retains the
original name `Decompress`; its current body resolves a frame offset and marks
pending texture bindings dirty. It does not itself decode compressed pixels.

`AnimShape.h` combines the formerly separate scene-helper and static-mesh views.
The 60-byte stride agrees with the existing static-mesh traversal and container
copy/destruction evidence. Observed storage includes a five-byte name, state and
dirty bytes, CRC, update frame, frame count/rate/position, two shape pointers,
ShapeFile pointer, 16-byte TAR-list storage view and next link at `+0x38`.
The TAR allocator word remains opaque. The separately scoped STL container view
is unchanged; neither view establishes every historical member declaration.

Encoding uses four six-bit digits and a terminator; values differing above bit 23
share an encoded name. Lookup returns the first five-byte name match. Reset walks
all matching numeric CRCs. List removal unlinks the first occurrence and clears
its next pointer; absent entries leave the list unchanged. The target hex parser
shifts for every character, including invalid ones, and adds only valid digits.

`SetFrame` retains the original floating comparisons, including its handling of
unordered values and its inclusive upper bound at `frameCount`. Valid shape data
and frame indices remain caller requirements; this reconstruction adds no bounds
repair. `Update` retains the target float multiplier rather than replacing it
with an assumed frame duration.

Two functions remain partial:

| Function | Target/native bytes | Objdiff similarity | Outstanding difference |
| --- | ---: | ---: | --- |
| `AdvanceFrames` | 844 / 840 | 99.431280% | Native control flow/register scheduling; full body retained |
| `RebindTARs` | 128 / 120 | 78.156250% | TAR-list traversal/storage addressing and scheduling |

`AdvanceFrames` models forward/reverse looping, one-shot and ping-pong states,
repeated boundary crossing, default clamping, and decompression only when the
integer frame changes. `RebindTARs` swaps pending/bound shapes, clears dirty state,
then calls each TAR's `SwapShape`. No runtime rendering claim is made.

## Memory-card devices

Thirteen exact functions / **812 bytes** cover the two-slot state poller,
change-of-state consumption, device lookup/index/name, insertion/removal flag
consumption, availability/selectability, status text and statistics.
The 160-byte device stride is independently present in indexed lookup and static
initialization. The view preserves the unknown table word and does not implement
or assert the complete virtual interface or historical inheritance.

Polling returns immediately while the manager is busy. A `-1` probe result does
not replace the remembered status. Forced refresh updates remembered nonbusy
results and caches both devices without an insertion flag. Ordinary polling
compares each settled result with its previous value, replaces the global change
flag, and caches only changed devices. `ChangeOfState` reads and clears the flag.
Device query methods retain the original fixed count and unchecked indexing.

Status text owns the original 128-byte static buffer and complete four-byte
single-space literal. It maps six status values to the original localized message
IDs and passes total/free counts in their observed order. Missing or unsupported
messages leave the single-space result. The existing fixed buffer and formatting
contracts are preserved, not made safe for arbitrary localized format strings.

`Cache` remains partial: **48/52** target/native bytes, with **81.666664%** objdiff
similarity from an extra temporary register move. It uses the ordinary shared
ProDG profile; no instructions are patched. Its body is guarded by
`DECOMP_COMPARE` and its original bytes remain in the verified image. `Invalidate`
was partial here (**24/24** bytes, **97.000000%**) and is now exact in the
`mc_getname` unit, with the store order that matches.

## Verification

All exact fragments use ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`; comparisons reuse accepted unit
profiles. Each complete function, generated allocated section, symbol binding,
private-file scope and dependency is checked. Original constants/BSS outside
accepted ownership remain original context and earn no source-code credit.

`tests/test_broad_source_sweep.py` uses the pinned original as an independent
PowerPC oracle. It compares returned bits, changed memory and call ordering for
property selectors and signed/flag patterns, dispatch IDs, card busy/probe/refresh
transitions and status messages, all animated-frame states and dirty TAR lists.
External card/localization/rendering services are controlled stubs. These checks
are separate from full-image byte verification and do not exercise the game in
Dolphin or on hardware. The full rebuild verifies all **2,046 accepted units** and the complete
2,860,576-byte analysis image. All **2,020 prior units** retain their object and
linked-output hashes, and all **22 prior comparison units** retain their measured
function results. The repository suite passes **99 tests** with Unicorn enabled;
the seven added methods cover 7,596 differential/dispatch cases. The pinned-target
audit and original-object baseline pass separately. The public snapshot is
regenerated from these verified inputs; CI validates that snapshot rather than
rebuilding the proprietary target.
