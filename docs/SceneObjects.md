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
remove all match. `CBehavior` itself is described in [Behaviors.md](Behaviors.md). `CAIObject` holds the previous, next and current
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

## Object creation

`CreateDumpObject` builds a stack `BSGO_Dummy` (`BSGO_Basic` plus a `WeakPtr<ISceneNode, 8>` and four
proximity words) so `ForceUpdateProximityTriggerStatus` can see the new object. `BSGO_Soldier`'s scene
node is a `WeakPtr<CSoldierObject, 8>` (the table symbol is in the image), and `CreateGerman` assigns it. `BSGO_Basic`'s destructor
is implicit: the weak copy has no table store and callers reduce it to `ISubject`'s. The string "Team"
at `0x802a705c` is first used by `CreateGerman` and shared by `CreateCollisionVolume`,
`CreateSpecialThrownObject`, `CreateRootHierObject`, `CreateDestructable` and `CreatePlayer`, so those
functions can only verify inside one unit with `CreateGerman`. The double literals also mean each unit's
`.rodata` must end on an eight-byte boundary, which is why `CreateGerman` and `CreateBasicAnimatedMech`
share a unit.
`CSoldierObject`'s attach slots compile as `(this + i * 112) + offset`, which the compiler only does when the
element's alignment differs from the access mode's, so the element is declared above word alignment. That
alignment is inferred: the real type's other members are unknown.

`objcreate.cpp` keeps its pools and queues in file statics, declared in `ObjCreate.h`. The dump
container holds a fixed array of 56-byte `DumpObject` entries linked oldest first. Its capacity comes
from a level resource, with 25 as the fallback, and `DeleteOldest` skips entries marked to keep.
`CreateObject`'s types are 1 mechanic, 2 environment-modifier mechanic, 3 enemy, 4 powerup, 7 to 9
particle systems, 10 projector and 11 animated mechanic. A delayed destruction runs once its entry is
more than one frame old. A queued German counts towards the non-visible soldier cull once it has
waited more than six frames. `CreateEnemy` compares the child script with `BOT3` to choose a bot.
`ShutdownObjectCreators` rebuilds the container in place, and the original tests the address first.
Each `BSGO_*` destruction unlinks the object, removes its scene node, destroys the script object and
deletes itself; each class keeps a 24-byte block with its scene node at `+0x14` after the script data.
`DestroySingleQueueElement` sends message 264 before calling `Destroy`.
`DeleteSpecialThrownObject` calls `CleanUp` at table offset 840, one slot before the attachable table's,
because a thrown object's table has no `FindAttachNode`.

`CStaticObject::SetMovementPath` keeps its path and look traversals at `+0xd0` and `+0xa0`. In
`MakeNoise`, the object skipped by the loop is read from a stack structure that is never written in this
build, so the comparison reads uninitialized memory.

`CStaticObject::AttemptPathMotionUpdate` steers toward `pathDest` at `+0x220` using the yaw and pitch
rates at `+0x11c` and `+0x120`, limited by `+0x124` and changed by `+0x128` per update. The rotation about
the x axis is an inline with its own axis local: the axis sits directly below the inline's matrix and the
position copies in the frame.

A motion key frame is 32 bytes: a position vector, then a rotation quaternion at `+0x10`. While keys advance,
`CHierObject::BeginAnimMotionUpdate` measures the distance to the next key once per key and stores it times
60 over the key length at `+0x288`, remembering the key at `+0x28c`.

`CStaticObject::UpdatePathMotionDestAndSpeed` runs the path states: 2 and 3 steer along the traversal, 4
brakes to the final point and raises script event 53 on arrival. State 3 keeps the object level with
`pathFollowTarget`, adjusting the wanted speed by the signed gap; mission 4 stage 2 lets it reverse. States 5
and 6 stop the object, and a stopped state 6 becomes state 1.

Bullet hits on static and animated objects raise script event 40 with a 32-byte record: source, damage,
direction (0 to 3, from the bullet's facing against the object's forward and right vectors), bullet type and
the contact's feature. Animated objects also scale damage by location: four times for features 0 and 1, half
for 3 to 8 and 13 to 20. Static objects add the hit position relative to the object at `+0x18` and `+0x1c`.

## `CAnimObject` drawing and initial state

`CAnimObject::Draw` refuses an object whose animation has not been updated, then queues the
mesh's first pass in bin 5 and, if the mesh word at `+0x30` is zero, a second pass in bin 9 sorted
by the object's squared view depth clamped to 1,000,000. The depth comes from `ClipSphere`,
whose own visibility result `Draw` ignores. `+0x140` is a `WeakPtr<ISceneNode, 8>`, identified by
the table the constructor stores and by the link and subject that `Init` clears, and `+0x2d0` is
the `AO_OpcodeHandler` that `Init` hands to the choreographer.

`+0x2c4` is a `CAnimObject::StateDone`, the task `SetAnimationState` passes to the choreographer:
a table, an event and the owner, whose `Execute` sets the word at `+0x2dc` and triggers the event
on the owner's script when it is not negative. `SetMoveTarget` chooses the locomotion type from the
angle between the move direction and, with a valid AI target, the direction to that target, or
otherwise the object's yaw.

`AO_OpcodeHandler::Execute` switches on the opcode's id at `+2`, with a 32-bit parameter from the
words at `+4` and `+6` and a duration at `+8`. It raises script events, plays enemy-movement,
reload and triggered sounds, toggles head tracking and sword state, and starts controller
feedback on the owning player's port. Unknown ids format an error message into a local buffer
that nothing reads, and return 0. For the feedback case the binary tests `g_bInMultiplayerMode`
and `g_bCoopMultiplayer`, yet both paths do the same work; the two-armed source is inferred.

## `CStaticObject` drawing

`CStaticObject::Draw` selects the mesh frame, and in multiplayer a dump object first wraps its
angle at `+0x288`, doubles its scale and turns about Z. It then queues the mesh's first part in bin
4 and the second in bin 9, sorted by squared camera distance, both through `ExecuteDraw`, and restores
frame 0 and the saved model-view pointer. `ExecuteDraw` sets the fade as a global colour scale and
the light block around `CStaticMesh::DrawMesh`.

## Soldier accessors with no Frontline counterpart

`CSoldierObject`'s partition, blink, expression and upper-body accessors exist only in this
game, so the code decides their names. The `m_partitionLower` and `m_partitionUpper` symbols name
the two partitions; `Partition3` selects the lower and `Partition2` the upper. Everything else
named in those fragments is descriptive and inferred, including the upper-body angle pairs at
`+0xb70`, the two state-done records at `+0x15ac` and the attach-slot name at `+0x50`.

`SetBlink`'s literal pool starts with 0 and 86400, the values of the state 0 and state 8 arm.
State 0 is written first in the source so that the pool comes out in that order; the compiler
then merges its identical body into state 8's. `SetUpperTranslation` is only an assignment:
`CVector3::operator=` returns by value, which is the unused four-word copy on its stack.

`CStaticObject::ModifyMotionData` and `TranslateFromScript` share one unit because their literals
are neighbors: the first ends four bytes short of an eight-byte boundary after its double.

## Heads-up display updates

`CHeadsUpDisplay::Update`, `UpdateCompass`, `UpdateAmmo`, `UpdateWeaponName` and
`LetterboxTransition` share a unit because the last one's double closes a pool that starts at
`UpdateCompass`. They take the file group's `-ffast-math` profile. Member names are inferred:
the "last" counters hold the values `Update` saw on the previous frame, and any change restarts a
panel's 4.5-tick timer. `lastHealth` is the first word of the player's script data; that this word
is health is inferred. `case 1` in the letterbox switch has no body, which is what leaves the
comparison tree's left side empty.

`CPlayerObject::TargetFind`'s methods use more of the object than the player header's view
declares. An eight-entry `TargetInfo` array at `+0x30` with its count at `+0x1b0` follows a
`WeakPtr<CPlayerObject, 8>` prefix and a second weak pointer at `+0x18`, which holds the chosen
target; the source models them in the unit. The view's placement inside `CPlayerObject` and
the array's relation to that view's `+0x40` fields are not settled here.

## Hearing volume

`CHearingVolume` is a moving scene node with two spheres, which `Init` places at a given height
above an owner it then follows each update. Its table is original; field names are descriptive.
`CCheckedMatrix` in `HearingVolume.h` is a project name: the original `CMatrix` default constructor
checks that the class is initialized, which the shared header writes out at each local, and a
member needs the check to run in member order. `HandleBulletCollision`, `PlayBulletWhizBys` and
the destructor are not reconstructed; the destructor differs only in `r10` and `r11`.

## Parked

These still differ, with instruction differences in brackets; drafts are kept outside the
tree and a later reader should expect them to be close rather than solved.

- `CStaticObject::CalculateMovementTime` (27): register allocation of the three times.
- `CStaticObject::AnimUpdate`: one missing `fmr`, which shifts the rest; the period and its
  conversion share a register in this build.
- `CAnimObject::DetectGround` and `Yaw`: a local's address stays in a saved
  register where the original rematerializes it.
- `CHierObject::GetWorldLinearVelocity` (19): register choice.
- `BumpType`, private to `Soldier_object.cpp`: the first search step appears twice with the index folded to 0, including the hit path; the loop forms tried reproduce everything but that. `CSoldierObject::OnCollision` calls it through `local_externals`.
- `CSoldierObject::SetHelmetOffset` and `EjectHelmet`: the "head" string is owned by `CleanUp`'s unit.
- `CAITankObject::SetInitialStatus` and `UpdateStatus`: a 15.0 literal sits at a 4-byte
  boundary before the double. A `static const float` in a `.rodata.*` section is folded back
  into the pool, so the subsection mapping cannot place it.
- `NullDeallocateArchiveBuffer`, `CompareObjectDefinitions` and `CompareStaticObjectDefinitions` have
  local binding and no caller in a matched unit; the two comparisons match when compiled alone.
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

All 19 accepted `hierobject.cpp` fragments share the `-ffast-math` profile.
`SwapOut` needs it: the zero product on the box centre's x disappears while its
square-root call stays. Applying the profile to the whole file group preserves
all accepted bytes; it is a working reconstruction profile, not proof of the
original setting. `GetAttachPointMatrix` is `void`, as its call in
`CTankObject::FireSubObject` shows through the argument schedule; `CAITargeting::SetTarget`
returns the current target, which its body loads last. `CVolBox::SetHeight` stores its argument
unchanged for the player's box, so `SwapOut` halves the height through a local helper.

## Maintained partial scene helpers

PR #59 supersedes the three partial scene helpers from PR #60 with byte-exact,
linked source: `CStaticObject::Init`, `CStaticObject::AttemptAnimMotionUpdate` and
`CTankObject::AddWeapon` (860 target bytes). Their former comparison registrations
and duplicate partial bodies are removed. The original/native differential
fixtures now exercise the accepted native units, preserving the checks for flag
bits, motion gates, bounds, weapon allocation and sentinel handling.

`pathScale` is now named `pathTurnSlowdown` in the scoped view; its offset remains
`+0x110`. The historical partial measurements remain in Git history and the
[batch notes](research/shortlist-partials.md). None is counted twice.

## Animated object factory loaders

`CAnimObjectFactory`'s loaders read tables from a `LevelFileContentsStruct_`: table 0 names the
skeleton, 1 the body mesh, 3 the head mesh, and 21 the mesh parts to hide; `LoadTextures` binds a
table of texture files to a mesh. A `CAnimObjDef` holds counted pointers to the skeleton, body and
head at `+0x48`, `+0x4c` and `+0x50`. Verified: the loaders' code. Inferred: member names and what
the table indices mean. `LoadCollisionData` is not reconstructed.
