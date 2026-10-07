# Property loading and spline-path management

This extension follows the [BPD setup](BPD.md#property-setup-and-cleanup) and the
contributed [PS2 gameplay-layout research](research/ps2/bpd-format.md). It adds
**11 functions and 1,484 executable bytes**, independently reconstructed from
the pinned GameCube GR8E69 executable with Codex assistance. The PS2 notes
identified the path table but did not establish these GameCube layouts or bodies.

## Accepted fragments

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `bpd_load_properties` | `LoadPropertyBPD` | 272 | None |
| `bpd_load_bsp` | `LoadPropBSPTree` | 84 | None |
| `ai_filter_splines` | `CAIFilterGlobal::CreateSplinePathManager`, `ResetSplinePathManager`, `ShutdownSplinePathManager`, `GetSplinePath` | 468 | 40-byte allocation-label pool at `0x802a227c` |
| `ai_spline_manager` | `CAISplinePathManager` constructor/destructor, `AllocateGenerateBuffers`, `FreeGenerateBuffers`, `GenerateSplinePath` | 660 | 48-byte allocation-label pool at `0x802a2f28` |

The manifests preserve original function symbols, addresses, sizes, bindings,
external references and the corresponding `propdat.cpp`, `AIFilter.cpp` and
`AISplinePath.cpp` file records. These are accepted fragments, not claims to
complete original translation units. All five named `CAISplinePathManager`
functions in the target inventory are covered; the separate `CAISplinePath`
interpolation implementation remains original code.

## BPD loading and reset

`LoadPropertyBPD(char *, bool)` takes two different paths:

- With the flag clear, load the file through `TLT_LoadFileFromLevelBigFile` and
  convert its header. Set the legacy trigger count, then add the header base to
  the legacy-record, path and light-volume pointers. Run `PatchUpAllPropertyData`,
  create the spline-path manager, and call `AIPathFinding::ImportAreaPathNodeNetwork`
  only when header word zero equals 13. Finally publish the legacy-record and
  light-volume pointers in their existing globals.
- With the flag set, reuse the existing `g_pBPDHeader`, refresh the trigger count
  and call `ResetSplinePathManager`. No file load, endian conversion, pointer
  relocation or manager allocation is repeated on this path.

Both paths then initialize triggers, clear machine-gun trigger objects, find
machine-gun points, cache buddy-lead triggers, deregister machine-gun triggers and
register them again, in that order. Those operations remain external originals.
The source adds no failed-load checks, format rejection or bounds checks.

The globals `g_pBPDHeader`, `g_pNumTriggers`, `g_pRawPropertyData` and
`g_pLightVolumes` retain their original ownership. Descriptive flag and field
names do not establish historical spelling or the meaning of header word zero.
The method-only interfaces to the other AI systems establish calls, not class
layouts or allocations.

## GameCube path records and manager storage

The conversion loop independently establishes a **12-byte `BPDPolyPath`** record:
signed ID at `+0`, signed point count at `+4`, and `PropVec3 *` at `+8`. It converts
those three fields in order, then adds the supplied file base to the point
pointer without a null-offset check. Each point has three floats at 0, 4 and 8
and a **12-byte stride**. The original spline generator also reads this stride.
`PropVec3` is distinct from the game's padded 16-byte `CVector3`.

`CAIFilterGlobal` keeps the spline-manager pointer at `+0x0c`. Creation, shutdown
and lookup independently use that field. Its public declaration exposes only
that prefix; the original singleton occupies 20 bytes, so the prefix must not
be used to allocate a complete instance. Earlier storage remains opaque.

The manager allocation requests **24 bytes**. Its constructor writes the count,
path pointer and four scratch-buffer pointers, establishing the six-word layout:

| Offset | Field | Evidence |
| --- | --- | --- |
| `+0x00` | unsigned path count | Constructor doubles the requested count; lookup compares unsigned |
| `+0x04` | `CAISplinePath *` | Array construction/destruction, generation and indexed lookup |
| `+0x08` | `buffer08` | 16,384-byte scratch allocation |
| `+0x0c` | `buffer0c` | 4,096-byte scratch allocation |
| `+0x10` | `buffer10` | 16,384-byte scratch allocation |
| `+0x14` | `buffer14` | 16,384-byte scratch allocation |

Scratch-buffer element types and original field names are not declared here.
The allocation calls use a null label and flag value 256; this does not by itself
prove an alignment contract. Manager and path-array allocations use their original
source-location labels and flag value 1024.

Each `CAISplinePath` occupies **12 bytes**, corroborated by array construction,
destruction, generation and lookup. Its segment pointer is at zero, an opaque
word at `+4`, and the ID at `+8`. The original path destructor calls
`CAISplinePathSegment` destructors through the first pointer. Path and segment
constructors/destructors remain external in this batch. Ordinary C++ `new[]` and
`delete[]` reproduce the original array cookie, construction loop and reverse
destruction loop; no manual array-cookie layout is substituted.

## Preserved spline behavior

`CreateSplinePathManager` skips all work when the signed input count is zero. A
nonzero count allocates a manager, allocates its four temporary buffers, converts
each path/point record using signed loop bounds, generates the paths, then frees
the temporary buffers. It does not replace that test with `count > 0`, delete a
previous manager, or add failure handling. The point pointer and count are
reloaded during conversion as in the original.

The manager constructor allocates twice the requested number of paths.
`GenerateSplinePath` generates the first path with the direction flag clear and
stores the supplied ID. It generates the second at `index + pathCount / 2` with
the flag set and stores `ID + 0x01000000`. The original
`CAISplinePath::GenerateTestSplinePath` independently shows that the set flag
copies the input points in reverse order; the clear flag copies them forward.
Its spline mathematics remains unreconstructed.

`GetSplinePath` applies the same ID increment for a reverse lookup, then searches
by ID. It preserves the original **one-past-end pointer when no ID matches** and
assumes the manager exists; no null or not-found fallback is added. Its return
address is established, while historical return-type const spelling is unknown.

`FreeGenerateBuffers` frees all four pointers in offset order and clears them.
The manager destructor deletes the path array and clears its pointer; it does
not free those scratch buffers itself. `ShutdownSplinePathManager` deletes the
manager when present and always clears the owning pointer. `ResetSplinePathManager`
ignores its three arguments and calls the original `CAreaSearchNode::Reset` and
`CAIObject::ResetGlobalList` in order, without recreating the manager.

## BSP loader and remaining fixups

`LoadPropBSPTree` always calls the level-file loader, passing no size-output
pointer. It stores the result in `g_pPBSPHeader`, computes `g_pPBSPHead` from the
32-bit offset at header `+0x24`, and invokes `PatchUpPropBSPTreeNode` only with the
flag clear. `PropertyBSPHeaderView` is a descriptive prefix for this one access;
it does not establish a full header format or file size. The flag's name describes
skipping relocation, not proof that every supplied buffer is already relocated.

The recursive node and leaf fixups were investigated but **are not reconstructed**.
Read from the original instructions only:

- Node fixup adds the file base to both child words at `+4` and `+8`, without
  testing zero. Byte flags at `+0x0c` and `+0x0d` select leaf or node recursion,
  visiting the first child before the second.
- Leaf fixup visits words `+0x20`, `+0x2c`, `+0x28`, `+0x1c`, `+0x24`, in that order.
  Their conditions are respectively the word at zero, the `+0x2c` word itself,
  and unsigned halfwords at `+6`, `+8`, `+4`. A true condition adds the file base;
  otherwise the word receives the address of its own storage, not null.
- The original `CheckLeafsTriggers` (`0x80123e74`) independently treats the first
  word as an unsigned count and `+0x20` as a pointer to two-byte trigger entries.
  Other list meanings and complete node/leaf layouts remain unestablished here.

Candidate bodies still differ in generated load/register/branch scheduling and
receive no source credit. No candidate node/leaf layout is promoted to the public
headers merely to force those matches.

## Verification and next work

All accepted objects use ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. Every emitted function, allocated
section and both complete diagnostic pools are verified. No instructions are
patched, assembly bodies substituted, or generated differences discarded. Data
and original dependencies earn no code-progress credit.

The complete rebuilt 2,860,576-byte analysis image matches, including its header,
allocated ELF bytes, entry point and BSS extent. The [progress workflow](Progress.md)
records that local result; public CI checks the snapshot rather than rebuilding
the proprietary game. Runtime/emulator behavior remains untested, and neither
the GameCube BPD/PBSP file bytes nor the complete level-loading path are validated.

The original BSP fixups, navigation import, trigger initialization and
`CAISplinePath::GenerateTestSplinePath` remain useful next targets. The verified
setup and manager interfaces now provide callers and storage evidence for them.
