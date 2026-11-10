# Collision volumes

`src/geometry/` reconstructs the double-dispatch layer of the collision volumes, all
of `csg_volume.cpp`, most of `collision.cpp` and `collisionvolume.cpp` from the pinned
GR8E69 executable. Claude Code assisted the reconstruction. The user's
Medal of Honor: Frontline decompilation was a semantic reference; every function was
compiled with ProDG 3.8.1 and compared with the original instructions, and no runtime
testing has been performed.

## Evidence

- **Virtual order.** `_vt.7IVolume` names all 19 virtual functions, and every subclass
  table adds `TestCollision(const CCSGVolume &)` after them. A table entry that still
  names `IVolume` shows the subclass does not override it. Emitted tables are verified
  byte for byte.
- **Return type.** `TestCollisionWithSubVolumes__H1Z7CVolBox_...` records `EClsnState`
  as the template's return type. Its enumerators are not recovered; the sphere tests
  return 1 when apart and 2 on contact.
- **Inheritance.** `CAnimatedVolume` keeps `CVolSphere`'s visibility, extents and
  radius entries, and spheres forward animated volumes unadjusted, so `CVolSphere` is
  a base at offset zero.

## Inferred names and storage

`lineTest` (the top bit of the word at `CCollision+0x10`), `result` (`+0x18`), the
member names of `CVolSphere` and `CWorldVolume` and the header names are descriptive
project choices. Layouts differ from Frontline: the Rising Sun result word is at
`+0x18`, and `TestCollision(const CVector3 &)` takes its point by reference.

## Emission

Fiber, capsule, CSG and CDB object destructors sit among the other methods of their
files, so they are out of line and are each class's key function: the unit defining
one also emits the class table. `CVolSphere` and `CVolBox` destructors follow the last
regular method, with the sphere's radius accessors, which is how GCC 2 emits inline
virtual functions along with a table whose key function is `Create`. Those functions
can only be emitted by a source that also defines `Create`, so they wait for the rest
of those files. `CLight::IsVisible` inlines the sphere destructor, confirming that it
is inline. Fragment files here are project boundaries, not original ones.

`sphere_tail.cpp` and `box_tail.cpp` emit those tail functions and class tables with
`#pragma implementation`, because the file's other functions are not all reconstructed
and `CVolSphere::Create` cannot land yet: see below. `CVolBox::Create` uses `__builtin_new`
and has no label.
Their inline constructors are recorded as discarded. Their literals are placed by
inference at `0x802a7f70..78` and `0x802a80e4`, unreferenced `1.0f` words near the end of
each file's literals; no instruction reads those words, so the position is not proven.

**Why `Create` is parked.** `.rodata` holds strings from inline functions even when no
code uses them, because ProDG writes them at parse time. 124 files begin with the label
`source/volume/sphere.h:29`, the class's inline `operator new`, and most follow it with
`bad_alloc` and `source/system/crc.h:241..268`. `Create` reads that label and a literal
after those strings, so a source that defines only `Create` has a non-contiguous
`.rodata`. Matching it means giving the headers the unused inline functions those strings
came from, and their bodies are unknown.

A class's inline default constructor is emitted with its table too. The original link
strips it, but its literal stays: the word at `0x802a8490` between the fiber line
constructor's and `Create`'s literals belongs to the fiber's. The fiber manifest records
it as a discarded function, as `strip_unused` documents in [Lua.md](Lua.md).

These discarded inline constructors are inferred from layout (the orphan literal and the
order of the fiber and capsule files), not proven by any symbol.

`IVolume`'s virtuals are all inline: they sit in table order in the linker's weak-code
region at `0x8027982c`, away from any file's own methods. `IVolume.h` keeps its
`#pragma interface`, as the other volume headers do so that a fragment does not emit
them, and `volume_defaults.cpp` emits them with `#pragma implementation`. That pragma
pair and the file are project arrangements: the original's file for these is unknown.

## World volume

`CWorldVolume` holds its sub-volumes in a `dwi::list`, a fixed-pool intrusive list.
`RemoveSubVolume`'s mangled symbol names `dwi::list_iter`, and the string
`source/tl/dwi_memory.h:56` is the label of the pool allocation, so the node allocator
lived at that line of a header the original tree calls `dwi_memory.h`. The list's other
members, the helpers (`insert`, `erase`, `swap`, `Exchange`) and the `DwiList.h` header
name are inferred from the inlined code. `operator!=` takes its operand by value because
the original keeps a copy in the frame each time round the loop.

Two `1.0f` words at `0x802a83c4` and `0x802a83c8` sit between the constructor's literals
and `UpdateExtents`'s. No retained function reads them, so the world volume's text is
split at `UpdateExtents` to leave them out; what emitted them is unknown.

## CDB objects

`CCDBObject`'s box, sphere, capsule and animated tests gather tri groups into a
`dwi::fast_vec` over 2048 stack words, starting at the database's root branch or, with no
branches, its root leaf (`branchCount` at `+28`). The sphere's test inlines its own plane
test instead of calling `TestCollisionWithTriGroups<CVolSphere>`, which is why no such
instance exists. The leaf test for a box calls `CVolBox::TestCollision`, not the override
`CCDBVolBox` also has. Field names, `CDBTri`'s words and `CDBVector` are descriptive.

`_._10CCDBVolBox` sits in the weak-code region like `IVolume`'s virtuals; a small file with
`#pragma implementation` emits it with the class table, again as a project arrangement.

## Contacts and the test driver

`CCollision` is 36 bytes. Contacts come from `CCollision::ms_contactPool`, over the 300
48-byte records of `g_aClsnContacts`, and form a list through `+44`. `AddContact` adds
the word at `+20` to the separation; copying and merging set that word to zero first so
the stored values are not adjusted twice. `Test` pads both volumes' radii by
`LARGEST_MOVE_PER_TICK`, runs the test and restores them, and sets the function-local
`isCSFWarningEnabled.6234` after a checked test. The source names that local with an `__asm__` label, so the
compiler's own number is kept. `SClsnContactPool` and its inline
helpers are project names: no symbol records the pool's type.

## Remaining exact-matching work

`CCollisionVolume::~CCollisionVolume` inlines the `ISceneNode` destructor, which
`SceneNode.h` declares out of line, and stays original context until that header decision
is made. `CVolSphere::Create` waits on its file's header strings, above, and with it any
whole `sphere.cpp`. Several of the long tests now have [maintained partial source](research/collision-batch.md).
The long vector tests still outside the verified image are listed by
`config/GR8E69` by omission: the box's box, sphere, capsule and plane tests and visibility, the sphere's visibility, the capsule's
visibility, the fiber's extents, `CCDBObject`'s line test and branch
node tests, the CDB box and triangle tests, `CCompartment`'s constructor and loading, and the segment distance.

## Compartment update and instance models

`CCompartment::BeginUpdate` updates the compartment's animated shapes and then its primer table's
material animations; `DoTextureSwap` steps one shape chosen through the primer table's texture entries.
The compartment's vectors are read through a small begin-and-end view here, because `Compartment.h`
is included by fragments that must not emit the STL headers' `.rodata`. Verified: offsets and control
flow. Inferred: the member names and the meaning of the primer table's words.

`CCompartment::CInstanceModel` has two vectors, of `EAGL::Model *` and of loader resource pointers;
their `reserve` instantiations in the symbol table establish the element types.

## Point tests and code shape

The point and sphere tests of spheres, capsules and fibers match with `void` helpers that
take their result by reference (`SetSub`, `SetSum`, `SetScaled`), a local passed to them
directly, and by-value copies of the line's ends. No original helper name is recovered; they
are descriptive, and so is `CLine3::CachedLength`, which stands for the length cache inlined
into `CVolFiber`'s point test. What the shape shows is that the original used small inline
vector operations, not which ones. Plain references such as `s` in the capsule plane test
are marked in the source: they reproduce the code shape and carry no other evidence.

`CCDBObject`'s `TestCollisionWithTriGroups` instances for box, capsule and animated volumes
sit in the weak-code region, with their literals right after `cdbobject.cpp`'s. Their
manifest names `cdbobject.cpp` on that basis: they read the local `g_bDisablePassThru`, which
needs a file, and the literals' position supports this one. No file symbol proves it.

`FindIntersection` is `capsule.cpp`'s local function, named by its own symbol. Its arithmetic is
the usual quadratic for a line against a capsule: an orthonormal basis along the axis, the
infinite cylinder, then each hemisphere. Its locals are declared at function scope because
that is what reproduces their register assignment; their names are descriptive. Frontline's
reconstruction of the caller has no source for it.

`box_distance.cpp` holds the point, line and segment distances against a box. `Face`,
`CaseNoZeros`, `Case0`, `Case00` and `Case000` are `box.cpp`'s local functions, named by their
own symbols; the structure is the usual case split on which direction components are zero.
Which of a function's zero-initialized extents is the literal and which are copies changes
between them (`Case00` and `Face` against `Case0`), so their declaration orders differ.

Each `TransformedCopy` transforms through an inline with a by-reference result, whose local and
the assignment's return value account for the original's two temporaries. The box also
normalizes its axes afterwards, which is why its half sizes in `SetCorners` are measured
along the axes before normalizing.

## File attributions

`cdbobject_head`'s `original_file` and its index 533 were chosen from the file symbol's
name (`cdbobject.cpp`), not from the code map, which gives that address range no file.
`csg_volume` and `animated_volume_tests` carry the file symbols 1336 and 1325 in the same
way.

## Frontline collision follow-up

The [larger collision batch](research/collision-batch.md) maintains 16 complete
function bodies: three exact interval helpers and 13 measured partials, including
box/box, box/plane, box visibility, CDB-box queries and triangle/box, line, sphere
and capsule tests. The notes record Rising Sun-specific behavior changes,
source provenance, constant-pool ownership and the known near-zero capsule
contact differences. Partial functions keep their original bytes in the verified
image; their similarity is not linked or exact credit.

The [distance and traversal follow-up](research/collision-distance-traversal.md)
maintains the segment-distance and triangle nearest-point helpers, box/sphere and
box/capsule queries, sphere/capsule visibility, the four volume branch traversals,
and the CDB line query. These remain partial, with the box/capsule contact limitation
explicitly documented; its tests do not claim equivalence at true intersections.
