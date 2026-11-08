# Static, attachable and jaw scene objects

These classes are scoped views over the original objects: `include/game/SceneObjects.h`
declares only the members and methods the reconstructed code reaches. Bases, table
layouts and complete allocations stay unestablished. Frontline (CodeWarrior) supplies
control flow and meanings; every offset below comes from the Rising Sun disassembly, and
member names are descriptive unless a symbol names them. See
[SceneNode.md](SceneNode.md) for the node tables and [Targeting.md](Targeting.md) for AI.

## Provenance of the fragments

| Source | Original file evidence | Functions |
|---|---|---|
| `src/scene/StaticObjectAccess.cpp`, `StaticObjectMotionFlags.cpp`, `StaticObject*.cpp` | `static_obj.cpp` in the code map | see each manifest |
| `src/scene/AttachableObjectA.cpp`, `B`, `C` | `attachobject.cpp` in the code map | slot and child handles; `Draw`, `IsVisible`; the update forwards, `FindAttachNode`, `GetObjectType` |
| `src/ai/AIObject*.cpp` | `AIObject.cpp` in the code map | location, behavior, status and global-list helpers |
| `src/scene/jaw_controller_ctor.cpp`, `jaw_controller_motiontype.cpp` | **none** | constructor and `SetMotionType` |

The jaw controller sits in a code-map group with no file name (`.text @ 800d3588`). The
`jaw_controller_*` file names are borrowed from Frontline's reconstruction, so they are
not evidence of a Rising Sun source file, and those manifests record no `original_file`.
`CJawController`, its methods and `EJawType` are named by the mangled symbols; member
names are descriptive.

## AI object helpers (commit 8ea27c6)

`GetLocation` (both overloads) and the two `GetDistance*Real` functions, the behavior
selection group from `ChooseBehavior` to `GetBehaviorCapability`, the previous and next
behavior accessors, the `As*` casts through `SetAIStatusVar`, and the global-list add and
remove all match. `CBehavior` is a view added for them: the enable word is at `+8`, its
table pointer at `+0xc`; slot names come from the original table where it names them, and
the pure slots are descriptive only. `CAIObject` holds the previous, next and current
behaviors at `+0x370`, `+0x378` and `+0x37c`.

The AI object constructors and destructors are deliberately absent. Matching them would
need `CAIObject` modelled as a real base with its table pointer at `+0x72c`, and the
symbols do not establish that layout.

## Static object fields

Offsets are verified by the matching functions: motion fields from `+0x234` to `+0x290`,
the sort-list nodes at `+0x20`, four bounding-volume slots from `+0x130`, the flags word
at `+0x218` and the fade manager at `+0x2dc`. The older field names `worldVolume` (`+0x134`)
and `field_13c` are misleading and retained so that other units keep compiling:
`+0x134` is the type-0 *local* volume and `+0x13c` the type-0 *world* volume.

The `.rodata` of `static_obj.cpp` is not continuous across the matched functions. After
`SerializeSaveData`'s literal, eleven words of 1.0 and 0.0 precede the literal of
`GetWorldLinearVelocity`, and no function in `.text` references them. They look like the
pool of a linker-stripped function, which is an inference. The units split there
rather than reconstruct a dead function.

## Animated and hierarchy objects

`CAnimObject` and `CHierObject` follow the same per-offset rule. `CHierObject` derives
from the `CStaticObject` view: both `CStaticObject::Draw` and
`CAttachableObject::UpdateAttachedObjects` run on the same pointer, so those views start at
the first byte. Its members begin near `+0x980`; each entry of its bullet-damage table is
12 bytes. `CHierAnimObject` holds its animated object at `+0xb40`, and that object has a
second table pointer at `+0x38` whose `+0x10` slot returns the skeleton; the attach point
is named `JO_gun` in the data. `CTankObject` stores its AI doodad bytes at `+0xb40`, the AI
object at `+0xb4c`, slot mappings at `+0xb58` and three weapons at `+0xb78`. In
`objcreate.cpp` the box, sphere and animated volume allocators use the file's local pools;
each `BSGO_*` class places script data at `+0x14` and its node after it.

`CSoldierObject` stays a view unrelated to `CAnimObject` because existing members use other
names for the same offsets; its methods convert the pointer unadjusted. Eight 112-byte attach
slots start at `+0xba0`. `CWorldObject` returns one embedded volume at `+0xd0` for every type.

The scene-object casts in these units pass a pointer unadjusted. That follows the scoped
views and does not assert the original class layouts or inheritance order.

## Parked

These still differ, with instruction differences in brackets; drafts are kept outside the
tree and a later reader should expect them to be close rather than solved.

- `CStaticObject::CalculateMovementTime` (27): register allocation of the three times.
- `CStaticObject::AnimUpdate`: one missing `fmr`, which shifts the rest; the period and its
  conversion share a register in this build.
- `CStaticObject::StartMotionPlayback` (60): the key-frame temporaries.
- `CAnimObject::BeginUpdate`, `DetectGround` and `Yaw`: a local's address stays in a saved
  register where the original rematerializes it.
- `CHierObject::GetWorldLinearVelocity` (19): register choice.
- `AllocateBoxVolume`: needs `CVolBox` data members that `Box.h` does not declare.
- `CSoldierObject::CleanUp`, `DetachObject` and `GetBulletEmitterMatrix`: the original
  computes attach-slot addresses as `(this + i * 112) + offset`; no array or pointer form
  tried reproduces it.
- `CAITankObject::SetInitialStatus` and `UpdateStatus`: a 15.0 literal sits at a 4-byte
  boundary before the double. A `static const float` in a `.rodata.*` section is folded back
  into the pool, so the subsection mapping cannot place it.
- `NullDeallocateArchiveBuffer` has local binding and no caller in a matched unit.
- `CAIObject::ComputeStatusThreatValue` (18): the value and status pointer swap registers and
  the result is not kept in a saved register.
- `CAIObject::IsObjectInLineOfSight(const CVector3 &, float, bool, const CAITarget *)` (49):
  the only cause is that the target-vector address is hoisted out of the height loop. It holds
  the const overload back too: that function and `IsObjectOccluded` share the two merged
  height tables and the literals are contiguous, so it cannot land in a unit of its own.
- `CAIObject::IsObjectReachable` (44): a second temporary copy of the start vector.
- `CAIObject::FindRunAwayPoint` (25): the shared failure return sits before the search, where this
  build moves the zero result into a conditional store.

The script-motion bits in `CStaticObject` are tested through inline accessors that each return
one bit; without them `||` merges the tests into one mask. The accessor names are descriptive.

## Maintained partial scene helpers

Three functions from the older parked list are now maintained under
`DECOMP_COMPARE` with the accepted `scene_StaticObjectHead.json` profile. The
earlier contributor drafts were unavailable; the bodies were reconstructed using
the current shared views and independently checked GR8E69 instructions, with
[Frontline revision b7b6fff](https://github.com/kyleckroeger/moh-frontline-decomp/tree/b7b6fffaba0edd2989a8a8b02977c1236c72b68b)
as a CC0 semantic reference. The earlier draft instruction counts do not describe
these new builds. Codex assisted the reconstruction and checks.

| Function | Target bytes | Compiled bytes | Objdiff similarity |
| --- | ---: | ---: | ---: |
| `CStaticObject::Init` | 144 | 132 | 54.97% |
| `CStaticObject::AttemptAnimMotionUpdate` | 548 | 544 | 92.52% |
| `CTankObject::AddWeapon` | 168 | 168 | 95.00% |

`Init` synchronizes orientation with the transform, enables drawing, clears the
three script-motion flags and velocity xyz, and resets the existing scale/path/
priority fields. It preserves the velocity's fourth word and other flag bits.
The collision-damage and path flags are cleared; `pathScale` becomes 0.5 and
`maxLightPriority` -1. Store scheduling and flag grouping remain different.

`AttemptAnimMotionUpdate` requires nonzero `motionLoop` and `motionRate` for the
motion path. It stores the raw step, scales velocity by it, and clamps the
interpolation amount `dt * motionSampleRate` with the original negated comparisons
(including the unordered-to-one path). It interpolates the two established
quaternions, updates the matrix and position, then updates the world's type-0
volume and XY sort bounds. Without that volume it queries the node position for
all four bounds. This differs from Frontline's motion guard and interpolation.

`AddWeapon` clears the selected slot and returns zero for CRC `0xfb9630c9`.
Otherwise it requests a weapon with ammunition 999. A successful allocation sets
`bulletClass` to 4 for a nonzero signed property halfword at `+0x0e`, otherwise 10,
and returns one. A failed allocation leaves a null slot and returns zero. The
original adds no slot bounds check or cleanup of a replaced weapon.

No new class layouts or complete allocations are inferred. Complete 12-byte and
8-byte pools at `0x8029eb88` and `0x8029ebd0` match; the tank helper emits no data.
All 860 target bytes remain original in the verified image, with no exact or
linked credit. [The batch notes](research/shortlist-partials.md) describe the
isolated state/flag/branch checks and remaining verification limits.
