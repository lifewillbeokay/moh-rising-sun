# BPD conversion and lighting-volume evidence

[kyleckroeger's PS2 gameplay-layout research](research/ps2/bpd-format.md) identified
the header, lighting records and relevant GameCube conversion routines. This batch
uses those leads to reconstruct **13 functions and 1,380 executable bytes** from
the pinned GR8E69 executable. The setup and cleanup extension adds **14 functions
and 1,840 executable bytes**, including six pointer helpers and light-pattern
registration. Both passes used Codex assistance. PS2 data was not substituted
for GameCube evidence, and no original files or dumps are included.

## Accepted fragments

| Fragment | Scope | Functions | Code bytes |
| --- | --- | ---: | ---: |
| `bpd_endian` | Header, property records, animated lights, light volumes and counted lists | 9 | 1,168 |
| `endian_scalar` | Float and signed-int conversion wrappers | 2 | 64 |
| `endian_short` | Signed-short conversion wrapper | 1 | 32 |
| `bpd_light_volume` | `IsPointInLightVolume` | 1 | 116 |
| `bpd_patch_all` | `PatchUpAllPropertyData` | 1 | 1,132 |
| `bpd_patch_records` | `PatchUpCore`, three empty hooks and `PatchUpAnimLight` | 5 | 420 |
| `bpd_free_properties` | `FreePropertyMemory` | 1 | 124 |
| `bpd_offset_pointer_table` | `offsetPtr<void *>` | 1 | 24 |
| `bpd_offset_property_pointers` | `offsetPtr<void/unsigned long/float/PropPlane4/BPDLight>` | 5 | 120 |
| `light_register` | `CLight::Register` | 1 | 20 |

The conversion block lies between the `propdat.cpp` and `proxtrig.cpp` compiler
markers. The point test lies between the `proxtrig.cpp` and `flexprop.cpp` markers;
the scalar wrappers lie in the `endian.cpp` marker interval. These are inferred
original file groups, not recovered translation-unit boundaries. The manifests
retain the exact function symbols, bindings, sizes and external dependencies.

## What the conversions establish

The `ChangeEndian` overloads and template names encode their argument types. Together
with each call's field address, they establish the accessed integer, halfword, float
and pointer types in `include/game/BPD.h`. Member names are reconstruction choices.
Descriptive names such as header table counts, bounding minima/maxima and animated
light colors/times follow the contributed research; conversion calls alone do not
prove those meanings. Fields without sufficient semantic evidence retain offset names.

The header view reaches `+0x6c`. Pointer types match the research's twelve table
locations, including the string pointer at `+0x54` after its integer at `+0x50`.
The word at `+0x0c` is untouched. The routine calls `ChangeEndian(int&)` on `+0x24`
**three times**; all three calls are preserved in their original order.

`xyzProperty_Struct` converts four leading fields followed by seven floats.
`MOH_core_Struct` converts selected halfwords and pointers beginning at `+0x2c`,
leaving its preceding region untouched. The mechanic, enemy, environment-modifier
and animated-light routines call the core conversion at offset zero before their
own field conversions. The `core` member models that shared prefix without claiming
historical C++ inheritance. Unknown gaps and untouched fields remain opaque.

For animated lights, the calls establish a signed halfword at `+0x72`, an
`unsigned long*` at `+0x74` and a `float*` at `+0x78`. This supports the color/time
array interpretation in the PS2 notes. Setup now independently establishes a
124-byte stride for the pattern array, also seen in `CAnimLightManager::Create`.
The legacy record walk instead uses each record's own size word. Timing units,
mode values and the on-disc encoding of packed colors are not established here.

The light-volume conversion touches six floats, three integers and pointers to
`PropPlane4` and `BPDLight`. Setup independently establishes **48-byte strides** for
both light volumes and light records. Each retains an opaque final four bytes at
`+0x2c`, untouched by these routines. `BPDLight` has an integer type at zero,
three position floats at `+4`, direction floats at `+0x10`, color floats at `+0x1c`
and intensity at `+0x28`. The [lighting consumers](LightVolumes.md) corroborate the
stride, direction, color and intensity. Field names are descriptive; a conversion
call alone does not prove a field's meaning. The later [path-loading reconstruction](Paths.md) establishes `BPDPolyPath` and
`PropVec3`; the other navigation types stay opaque. Other prefix views do not
establish complete allocations or array strides.

`EndianSwapList` converts the leading unsigned-long count, then entries 1 through
that count inclusive. It re-reads the converted count at each loop test and retains
the original unsigned comparison. It adds no malformed-input checks.

The accepted float/int/short wrappers delegate through unsigned references to
convert the underlying representation, without numeric float conversion. The
unsigned scalar routines and all `ChangeEndian<T>` implementations remain external
original code. This is a target-specific 32-bit interface, not a portable file parser.

## Independent lighting-volume check

`IsPointInLightVolume` independently reads the integer plane count at volume
`+0x18` and plane pointer at `+0x1c`. It steps through **16-byte** plane records,
using floats at offsets 0, 4, 8 and 12 to compute:

```text
plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.d
```

Every result must compare greater than or equal to zero. Negative or unordered
results return zero immediately; boundary points pass. A nonpositive plane count
returns one without examining any planes. The reconstruction preserves the original
floating-point operation order and generated fused operations.

The point remains an opaque `CVector3`, accessed through the existing
[`Vector3Components` prefix interface](Matrix.md). No complete vector size,
constructor or normalization requirement is inferred. The plane stride and four
component accesses support the shared `PropPlane4` record view.

## Property setup and cleanup

`PatchUpAllPropertyData` connects the conversion helpers to the completed
[FlexProp database loaders](FlexProp.md#database-loading-and-cleanup). Its caller,
`LoadPropertyBPD` (`0x80120af0`, [now reconstructed](Paths.md)), converts the header and adds
the header base to the path, legacy-record and light-volume pointers before
calling setup. Setup must not relocate those pointers a second time.

Setup preserves this order:

1. If `field24` is nonzero, relocate the pointer table, then convert and relocate
   each entry. Its loop uses the signed count and retains each entry reference
   across conversion.
2. If the pattern pointer is nonzero, relocate it and walk the 124-byte animated
   light records. Convert each common prefix and light record, relocate its color
   and time arrays relative to the **header**, and convert the time floats. The
   packed colors are not swapped. Register the pointer and count with `CLight`.
3. Convert each light volume, then relocate/convert its planes and lights when
   the corresponding signed count is positive. Plane conversion visits x, y, z,
   d. Light conversion visits intensity, color, direction, position, then type.
   Inner loops reload the volume table and counts, preserving the original calls'
   possible effects on memory.
4. Walk the legacy records from header `+0x28`, retaining its initial count as an
   unsigned value. Store each record in `g_pTriggerObjects`, convert its prefix,
   and select conversions/hooks using the type word at `+0x0c`. Type 0 needs no
   further conversion; mechanic, enemy, environment modifier and animated-light
   types take their specific paths. The default path uses core conversion for
   values at most 14 (including negative values); larger values print the original
   diagnostic. Advance by the
   record's unsigned size at `+4`, reloading the stored record between calls.
5. Relocate objects, class layouts and strings relative to the header; create
   `g_pStringTable`; load classes; load properties. Return the byte distance
   traversed through the legacy records.

The original `g_pTriggerObjects` symbol owns 24,000 bytes at `0x80326d68`. Setup
advances by **12 bytes** and writes the legacy pointer at `+8`, independently
supporting the existing [TriggerObject view](TriggerObject.md). The array remains
original storage. No new allocation or bounds check is introduced.

`PatchUpCore` relocates nonnull counted-list pointers in order at `+0x58`, `+0x5c`,
`+0x60`, `+0x64`, `+0x44`, and `+0x68`, calling `EndianSwapList` after each. It then
relocates character pointers at `+0x40`, `+0x4c`, `+0x54`, followed by one more
counted list at `+0x6c`. All use the **record** base. The newly exposed `+0x6c`
pointer was inside an opaque gap; shortening the following gaps preserves all
previously established derived-record offsets. The core endian routine does
**not** convert that pointer, and this reconstruction preserves that behavior.
The mechanic, environment-modifier and enemy patch hooks are empty originals.

`PatchUpAnimLight` relocates the color and time pointers using its explicit base
argument, then converts each time float using the signed halfword count. Its
legacy-record caller passes the **record** base, unlike the header-relative
pattern path. These two bases must not be conflated.

The six accepted `offsetPtr<T>` instantiations preserve null and otherwise add
an integer base to the stored 32-bit pointer representation. Their original
symbols establish the pointee types; their implementation is shared through a
private source header. The separate `ChangeEndian<void *>` routine between the
two accepted ranges remains original code; nothing is discarded to bridge it.
Other offset-helper instantiations remain uncredited.

`FreePropertyMemory` shuts down the original AI spline-path manager, deletes and
clears the string-table object, unloads classes, then unloads properties. It
closes and clears the PBSP buffer before the BPD buffer, testing each for null.
The AI interface now has a scoped spline-manager pointer view; its singleton,
both file buffers and all global storage remain original. `TLT_CloseFile` stays
external; [spline shutdown and its manager](Paths.md) are now reconstructed.

## Verification and next work

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces the
individual functions and every allocated section in these fragments. The point
test's four-byte zero constant at `0x802a6c58` and setup's complete 24-byte
`Unknown property type` diagnostic pool at `0x802a651c` are checked separately
and earn no code credit. No instructions are patched, no assembly bodies are
substituted and no generated functions or section bytes are discarded or clipped.

The complete rebuilt 2,860,576-byte analysis image also matches the original,
including its header, allocated ELF bytes, entry point and BSS extent. Follow the
[build, test and snapshot process](Progress.md) for future changes. Runtime and
emulator behavior remain untested, and the GameCube `.bpd` file bytes have not
been examined. These verified conversions and pointer operations do not by
themselves establish a complete portable file parser or level loader.

`LoadPropertyBPD`, `LoadPropBSPTree` and the spline-path manager are now
[reconstructed](Paths.md). Navigation import and the recursive PBSP fixups remain
useful next targets. [LightVolumes.md](LightVolumes.md)
covers the lighting consumers and the remaining playback/blending work. The
original compiler release remains unconfirmed.
