# Property loading and spline-path management

This reconstruction follows the [BPD setup](BPD.md#property-setup-and-cleanup) and the
contributed [PS2 gameplay-layout research](research/ps2/bpd-format.md). The loading and management batch covers **11 functions and 1,484 executable
bytes**; segment evaluation and traversal add **21 functions and 2,100 bytes**.
Both were independently reconstructed from the pinned GameCube GR8E69 executable
with Codex assistance. The PS2 notes identified the path table but did not
establish these GameCube layouts or bodies.

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
path-generation implementation remains original code. Segment evaluation and
traversal fragments are recorded below.

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
destruction, generation and lookup. Its segment pointer is at zero, the last segment index at `+4`, and the ID at
`+8`. The original generator stores input point count minus one at `+4`;
`GetLastPoint` uses it as an index. The reconstructed path destructor calls
`CAISplinePathSegment` destructors through the first pointer. Ordinary C++ `new[]` and
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
Its coefficient generation remains unreconstructed; the segment evaluation
methods it calls are now accepted source.

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

## Segment evaluation and traversal

The second batch accepts these fragments, all associated with the original
`AISplinePath.cpp` file record (index 992):

| Manifest | Functions | Code bytes | Generated data |
| --- | ---: | ---: | --- |
| `ai_spline_breakdown_lifecycle` | 2 | 60 | 4 bytes at `0x802a2e68` |
| `ai_spline_breakdown_crossed` | 1 | 112 | 8 bytes at `0x802a2e74` |
| `ai_spline_segment_lifecycle` | 2 | 72 | 4 bytes at `0x802a2e7c` |
| `ai_spline_segment_math` | 4 | 736 | 12 bytes at `0x802a2e80` |
| `ai_spline_path_lifecycle` | 2 | 144 | None |
| `ai_spline_last_point` | 1 | 56 | 4 bytes at `0x802a2eec` |
| `ai_spline_traversal_lifecycle` | 3 | 172 | None |
| `ai_spline_traversal_next` | 1 | 312 | 16 bytes at `0x802a2ef0` |
| `ai_spline_traversal_setup` | 3 | 356 | 16 bytes at `0x802a2f00` |
| `ai_spline_traversal_getters` | 2 | 80 | 4 bytes at `0x802a2f24` |

### Segment layout and arithmetic

The generator's allocation/construction loop and the path destructor independently
establish an **80-byte segment stride**. `Set` writes a float at `+4`, used as a
parameter step by traversal and the original closest-parameter search. Four
16-byte vectors lie at `+0x10`, `+0x20`, `+0x30`, and `+0x40`. `Set`, `Expand`,
and both derivative routines independently establish their component offsets.
The words at zero and `+8` through `+0x0f` remain opaque. `cubic`, `quadratic`,
`linear`, `constant`, and `parameterStep` are descriptive names.

For coefficient vectors A, B, C, D, `Expand(t)` evaluates
`((A * t + B) * t + C) * t + D`. The first derivative uses
`(A * (1.5 * t) + B) * (2 * t) + C`; the second uses
`(A * (3 * t) + B) * 2`. The source preserves this grouping and the original
intermediate output writes. It does not collapse the expressions into alternative
polynomial forms or promise safe aliasing with coefficient storage. Only x, y,
and z are changed. `Set` uses the existing vector assignment, including its
by-value temporary, and leaves each destination's fourth word untouched.

The segment constructor default-constructs the four vectors, setting their fourth
words to 1 without initializing coefficients or the parameter step. Its destructor
has no explicit work. The path constructor also initializes nothing; its
destructor deletes the segment array and clears the pointer. Ordinary `delete[]`
reproduces the original 80-byte reverse destruction loop and compiler cookie.

`GetLastPoint` evaluates `segments[lastSegment]` at **zero**, not at one. The
original generator allocates one segment per input point and stores point count
minus one as that index. No empty-path or bounds check is added.

### Traversal storage and behavior

The traversal declaration is a **prefix through `+0x2f`**, not a proven complete
allocation. Repeated accesses establish state at zero, parameter at `+4`, previous
squared distance at `+8`, segment index at `+0x0c`, path pointer at `+0x10`, segment
pointer at `+0x14`, and a breakdown-point vector at `+0x20`. Bytes `+0x18` through
`+0x1f` remain opaque. The breakdown-point declaration likewise exposes only its
observed vector prefix; it does not prove historical inheritance or full size.
The navigation declaration is a method-only interface, not an allocatable class.
Field names, ordinary return types and access control are reconstruction choices
consistent with the observed uses; symbols preserve the method names and argument
types, not a complete historical declaration.

- Construction clears state and the path pointer, then constructs the embedded
  breakdown point. It leaves other fields uninitialized. Destruction destroys
  that point without deleting the borrowed path or segment.
- `WasCrossedDistance` computes XY squared distance. At or below 16 it reports
  crossing when distance is at or below 0.25 or is no longer at or below the
  previous value. Otherwise it updates the previous value and returns false.
  The explicit negated comparisons preserve the original unordered branches;
  the previous value is not written on a successful crossing.
- `GetCurrentParameterValue` delegates to the original segment
  `GetClosestParameter`. `SetupNextSegmentForward` starts that search at zero,
  advances the segment pointer/index while the result is below zero or above
  one, stores the accepted parameter, evaluates the point, and makes the modified
  next point. It adds no end-of-array check.
- `PrepareForTraversalForward` installs the path and its first segment, resets
  index and parameter, and evaluates at zero. State becomes 2 only when the
  caller requests A-star setup and the original navigation method succeeds;
  otherwise it becomes 1. Previous distance is set to the largest finite float.
  `RestartForwardTraversal` repeats this setup on the stored path with A-star
  setup enabled.
- `MakeModifiedNextPoint` previews `parameter + 4 * parameterStep`. When that is
  not at or below one, it sets the stored parameter to one, evaluates the endpoint,
  obtains its derivative, normalizes it unless its length is zero, scales it by
  0.25, and adds it to the endpoint. Otherwise it evaluates the preview parameter
  without updating the stored parameter. It does not clamp all inputs or invent
  a direction for a zero derivative.
- `GetFinalPointForward` delegates to the path's last-point method.
  `GetCurrentDerivative` always evaluates at **one**, independently of the stored
  traversal parameter.

The original `GetClosestParameter` uses an iterative derivative-based search and
falls back to sampling. A private reconstruction still differs in generated
arithmetic scheduling/register use and receives no source credit. Its full body,
`GenerateTestSplinePath`, A-star continuation, and the larger `GetNextPointForward`
remain original dependencies. The new types and evaluated segments provide
verified interfaces for continuing those functions.

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
section, diagnostic pools and float constant pools are verified. No instructions are
patched, assembly bodies substituted, or generated differences discarded. Data
and original dependencies earn no code-progress credit.

The complete rebuilt 2,860,576-byte analysis image matches, including its header,
allocated ELF bytes, entry point and BSS extent. The [progress workflow](Progress.md)
records that local result; public CI checks the snapshot rather than rebuilding
the proprietary game. Runtime/emulator behavior remains untested, and neither
the GameCube BPD/PBSP file bytes nor the complete level-loading path are validated.

The original BSP fixups, navigation import, trigger initialization, closest-parameter
search, forward-point traversal and `CAISplinePath::GenerateTestSplinePath` remain
useful next targets. The verified
setup and manager interfaces now provide callers and storage evidence for them.
