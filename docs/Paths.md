# Property loading and spline-path management

This reconstruction follows the [BPD setup](BPD.md#property-setup-and-cleanup) and the
contributed [PS2 gameplay-layout research](research/ps2/bpd-format.md). The loading
and management batch covers **11 functions and 1,484 executable bytes**; segment evaluation and traversal add **21 functions and 2,100 bytes**; walking
controls and movement updates add **16 functions and 2,212 bytes**. These batches
were independently reconstructed from the pinned GameCube GR8E69 executable
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
The navigation declaration now exposes the independently observed locomotion
pointer at `+0x20`; it remains a partial view that must not be allocated.
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
arithmetic scheduling/register use and receives no source credit. Its full body and `GenerateTestSplinePath` remain original dependencies. A-star
continuation and `GetNextPointForward` are now reconstructed as described below.

## Forward walking and movement updates

The third batch uses the established spline and vector declarations to reconstruct
these seven fragments. All allocated output matches, including 44 float-pool bytes.

| Manifest | Functions | Code bytes | Generated data |
| --- | ---: | ---: | --- |
| `ai_spline_traversal_astar` | 1 | 148 | 8 bytes at `0x802a2f10` |
| `ai_spline_traversal_forward` | 1 | 840 | 12 bytes at `0x802a2f18` |
| `ai_spline_module_walk` | 7 | 520 | 4 bytes at `0x802a2f58` |
| `ai_locomotion_access` | 4 | 96 | 4 bytes at `0x802a497c` |
| `ai_physics_orientation` | 1 | 232 | 4 bytes at `0x802a2cb8` |
| `ai_physics_position` | 1 | 188 | 4 bytes at `0x802a2cbc` |
| `ai_physics_directions` | 1 | 188 | 8 bytes at `0x802a2cc4` |

The original file records are `AISplinePath.cpp` (index 992), `AILocomotion.cpp`
(1075), and `AIPhysics.cpp` (988). The seven source files are reconstruction
fragments, not recovered original translation-unit boundaries.

### Forward traversal and script events

`GetNextPointForward` preserves the numeric states and event IDs observed in the
GameCube instructions. Historical enum/event names have not been established here.

- State 7 writes input position plus the supplied direction, sends event 53 when a
  script object exists, and returns. It sends that event on every invocation in
  this state; no one-shot guard is added.
- States 5 and 6 sample the final point and test whether it was crossed. Crossing
  changes the state to 7 and sends event 53, then proceeds to the common output
  selection. The position-plus-direction behavior starts on the next invocation.
- States 1 and 2 require a crossing before advancing; other states reaching this
  path proceed directly. A closest-parameter result replaces the stored parameter
  unless it is below zero. The explicit negation preserves unordered comparisons.
- Once the parameter is no longer below one, the function compares the unsigned
  segment index against `path->lastSegment - 1`. At that limit it selects that
  segment, resets previous squared distance to the largest finite float, changes
  states 2/4 to 6 or 1/3 to 5, and samples the final point. Before that limit it
  sends event 163, increments the index/pointer, and sets up the next segment.
- Below one, states 1/2 send event 52 and become 3/4 respectively. The function
  evaluates the current point and calls `MakeModifiedNextPoint`.
- At the common exit, state 2 updates the original A-star walker and copies its
  locomotion objective position; other states copy the spline next-point xyz.

Events use the existing `BSObjectTriggerEvent` interface with both pointer
arguments null and the final flag true. Its return value is ignored. The spline
module parameter is unused. No state validation, null-path guard, empty-path
handling, or event deduplication is introduced. Unsigned subtraction at a zero
last index retains the original behavior rather than clamping it.

`ContinueTraversalForwardAStar` measures XY distance from the next point to the
physics position. When that distance is not at or below 1.5, it attempts the
original A-star setup, sets state to 2 on success or 1 otherwise, and resets the
previous squared distance. On the other branch it changes only state 2 to 4.
The navigation search/setup/update implementations remain original code.

### Walking controls and shared storage

The module constructor, inspected as original code, corroborates pointers to
physics at `+4`, navigation at `+8`, locomotion at `+0x0c`, and the script object
at `+0x10`, followed by traversal at `+0x20`. Walking methods independently use
these offsets. The prefix leaves bytes `+0x14` through `+0x1f` and the dispatch
word at zero opaque. It does **not** recover the complete class size, inheritance,
virtual interface, constructor or destructor. Do not allocate through this view
or assume that its direct method declarations model virtual dispatch.

The navigation constructor independently stores its locomotion argument at
`+0x20`, agreeing with forward traversal. `AILocomotion.cpp` initialization and
both objective getters establish the vector at `+0x10`; its constructor, stop/
continue methods, and spline controls agree on the four-byte walk selector at
`+0x3c`. The expanded locomotion prefix is documented below; untouched gaps
remain unknown.
Ordinary return types, member names, access control and the integer spelling of
the walk selector are reconstruction choices, not a recovered full declaration.

`StartSplinePathWalkForward` stops spline walking, stops A-star walking, prepares
the requested path with A-star setup enabled, sets walk selector 1, and immediately
computes the next objective point. `LoopSplinePathWalk` performs the same sequence
through the stored path's restart helper. `WalkNextSplinePathPoint` passes the
physics position and its second coordinate axis to traversal, then assigns the
result to the locomotion objective vector, preserving the vector's fourth word.

`StopSplinePathWalk` writes selector zero; `StopWalk` delegates to it;
`GetWalkType` returns 1. `ContinueSplinePathWalk` resumes states 1/3/5 directly;
states 2/4/6 first run A-star continuation and then deliberately fall through to
write selector 1. Other states leave it unchanged. The locomotion-specific stop
and continue methods write 0 and 3 respectively. Both objective getters expose
the same vector: one assigns it to the reference argument, the other returns its
address (represented as a reference in the reconstruction).

### Physics position and coordinate conversion

The physics declaration is a prefix through `+0x7f`, not a proven allocation or
inheritance model. Its original constructor corroborates the vector boundaries;
its update methods independently establish the following accesses:

| Offset | Observed storage/use |
| --- | --- |
| `+0x10` | Current position vector |
| `+0x20` | Euler-direction vector |
| `+0x30`, `+0x40`, `+0x50` | Three coordinate-axis vectors, in argument order |
| `+0x60` | Length of the most recent position displacement |
| `+0x70` | Position displacement vector |

The initial sixteen bytes, including original dispatch storage, and bytes
`+0x64` through `+0x6f` remain opaque. The original function signature preserves
`CAIFilterRealEulerDirection` and `CAIFilterRealCoordinateAxes`; their nested
member spelling/composition in the header represents observed storage, not
historical source. No axis is renamed right/front/up without separate evidence.

`UpdatePositionFrom` subtracts old position from the supplied point into the
stored displacement, assigns the new position, and stores the square root of
its squared length. This is displacement length, not a velocity calculation:
there is no division by elapsed time. It preserves component/write order and
vector-assignment temporaries, with no temporary snapshot to promise alias safety.

`UpdateOrientationFrom` assigns the three input axes in order, then calls
`CalculateDirectionsFromCoord`. That helper stores z as the game's
`MathFunAtan2F(-axis0.y, axis0.x)`, copies axis1 and rotates it about Z by the
negative stored angle, sets y to zero, then stores x as
`MathFunAtan2F(-rotated.z, rotated.y)`. It preserves the custom MathFun angle
convention and does not normalize axes or replace it with the library `atan2f`.
The fourth Euler-vector word is untouched. Local inline component/subtraction
helpers are descriptive reconstruction choices; no original helper spelling is
claimed.

## Player paths and arbitrary-point walking

The next batch reconstructs 16 functions in ten units: **1,320 executable bytes**
and 72 bytes of required constant pools. Only executable bytes receive progress
credit. The player fragments belong to the original `player.cpp` file record
(index 835); the spline evaluators retain their original local symbol binding.
The AI fragments use `AILocomotion.cpp` (1075) and `AIFilter.cpp` (970).

| Unit | Functions / code bytes | Required constant pool |
| --- | --- | --- |
| `player_path_controls` | `MoveOnPath`, `StopPath`: 2 / 80 at `0x800b472c` | 12 bytes at `0x8029f210` |
| `player_spline_position` | `EvaluateSplinePath`: 1 / 144 at `0x800b477c` | 16 bytes at `0x8029f220` |
| `player_spline_tangent` | `EvaluateSplineTangent`: 1 / 144 at `0x800b480c` | 16 bytes at `0x8029f230` |
| `ai_locomotion_distance` | `GetDistanceToArbitraryPoint`: 1 / 44 at `0x800ef004` | None |
| `ai_locomotion_setup` | `SetupArbitraryPointUpdate`: 1 / 16 at `0x800ef090` | None |
| `ai_locomotion_start` | `StartWalkToArbitraryPoint`: 1 / 224 at `0x800ef0a0` | 4 bytes at `0x802a4980` |
| `ai_locomotion_walk` | `WalkToArbitraryPoint`: 1 / 256 at `0x800ef180` | 12 bytes at `0x802a4984` |
| `ai_locomotion_dispatch` | `RegisterWalkType`, `StopWalkByType`: 2 / 144 at `0x800ef280` | None |
| `ai_locomotion_requests` | `CantReachTarget`, `SetArbitraryWalkMPType`, `TeleportTo`: 3 / 144 at `0x800ef310` | 4 bytes at `0x802a4990` |
| `ai_filter_clock` | Constructor, `UpdateFromGlobals`, `UpdateToGlobals`: 3 / 124 at `0x800d84d0` | 8 bytes at `0x802a2274` |

### Player controls and evaluation

The shared `CPlayerObject` member view remains in `CameraShake.h`; the existing
shake offsets are preserved. Path controls expose a separate flag word at
`+0xd44`, movement/look path pointers at `+0xe00`/`+0xe04`, and parameter/rate at
`+0xe08`/`+0xe0c`. These accesses also agree with the still-original
`UpdateMovePath` at `0x800b489c`. The intervening bytes, base classes, complete
allocation and virtual interface remain unknown.

`MoveOnPath` enables bit 19, installs both paths, resets the parameter to zero,
and stores `(1 / duration) * 0.016683351f`. That final factor is the observed
single-precision constant; it must not be rounded to `1 / 60`. There is no
zero-duration guard. `StopPath` only clears bit 19; it leaves the pointers and
parameter storage unchanged.

Both evaluation helpers multiply the supplied parameter by the path's unsigned
`lastSegment`, convert the result to a signed integer by truncation toward zero,
and evaluate that segment at the fractional remainder. The position helper
calls `Expand`; the tangent helper calls `ExpandDerivative`. Neither clamps the
parameter nor normalizes the tangent. Their complete integer-conversion pools
are retained separately. An input of 1 selects the stored final segment at local
parameter zero, agreeing with `GetLastPoint`.

### Locomotion storage and dispatch

The original constructor at `0x800eee50`, the previously accepted objective
getters, and the new methods corroborate the following prefix. It is not a
complete allocation or class inheritance model.

| Offset | Observed storage/use |
| --- | --- |
| `+0x04`, `+0x08`, `+0x0c` | Physics, targeting and script-object pointers |
| `+0x10`, `+0x20` | Current objective and arbitrary-point vectors |
| `+0x30` | `EMovePointType` selector |
| `+0x34`, `+0x38` | Arrival-distance value and `BS_STRUCT_Vector_struct` pointer |
| `+0x3c`, `+0x40` | Walk selector and update-time value |
| `+0x48` through `+0x58` | Five method pointers, cleared by the original constructor |
| `+0x60`, `+0x70` | Teleport-position vector and four-byte request flag |

The dispatch word at zero, words at `+0x44` and `+0x5c`, and any storage after
`+0x73` retain no reconstructed meaning. Targeting and the script vector remain
forward declarations. Enum tags are recovered from symbols; enumerator/member
names and ordinary return-type spelling are reconstruction choices.

`RegisterWalkType` stores its argument at the index returned by its virtual
`GetWalkType`; `StopWalkByType` invokes the selected method's virtual `StopWalk`.
The original `_vt.19CAISplinePathModule` and `_vt.18CAIAreaPathFinding` tables both
place the destructor, `GetWalkType`, and `StopWalk` in that order. Call sites use
a signed `this` adjustment at table `+0x10`/`+0x18` and the corresponding function
at `+0x14`/`+0x1c`. `IAILocomotionMethod` models only this dispatch contract; its
pure virtual declarations do not establish the historical base declaration.
No concrete subclass inheritance, constructor, destructor or table is rebuilt.
There are no added null/index checks.

`StartWalkToArbitraryPoint` stops registered methods 1 and 2, sets walk selector
3, assigns the supplied point to the stored arbitrary point, and then assigns
that point to the current objective. All vector assignments retain their
by-value return temporaries and leave destination fourth words unchanged.
`GetDistanceToArbitraryPoint` measures XY distance from physics position.

`SetupArbitraryPointUpdate` installs the movement-point selector, arrival distance
and script vector pointer. `WalkToArbitraryPoint` preserves a surprising original
branch: when `!(globalTime >= updateTime)`, it calls the still-original
`UpdateArbitraryPoint` and copies the arbitrary point into the current objective;
otherwise it only writes `updateTime = globalTime + 0.1f`. It then measures full
3D distance for point selector 4, or XY distance for other selectors. If
`!(distance - arrivalDistance >= 0.5f)`, it emits event 154. The negated comparisons
retain the original unordered floating-point behavior. This routine does not
stop walking after the event.

`CantReachTarget` emits event 194. Both event calls pass null context/sender and
true for the final flag. `SetArbitraryWalkMPType` only replaces the selector.
`TeleportTo` sets the request flag to one and copies the requested position; it
does not itself update physics or apply a teleport.

### AI clock

`CAIFilterGlobal` now exposes frame count at zero and accumulated time at `+4`.
The constructor initializes those values and the spline manager pointer to zero,
and writes `0x2400` at `+0x10`; the meaning of that last word is not established.
`UpdateFromGlobals` obtains `GetFrameCount()` and accumulates its argument times
`1.0f / 60.0f` into time. `UpdateToGlobals` is an original no-op. The word at
`+8` stays opaque, and this prefix does not establish the singleton's full size.

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
search and `CAISplinePath::GenerateTestSplinePath` remain useful next targets.
Spline-module construction and concrete virtual interfaces, player
`UpdateMovePath` (1,036 bytes), and locomotion `UpdateArbitraryPoint` (332 bytes)
also remain original. The verified controls, dispatch contract, clock and spline
evaluators now provide callers and storage evidence for those larger routines.
