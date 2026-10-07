# Light volumes and scene lights

`src/lighting/` reconstructs seven `CLightVolumeManager` methods (752 executable
bytes) and 32 `CLight`, `CPropertyAnimLight`, `CInstancedAnimLight`,
`CAnimLightManager` and light helper functions (1,908 bytes) from the pinned GR8E69 executable. Claude Code assisted the analysis and
verification. The later [BPD setup extension](BPD.md#property-setup-and-cleanup)
adds the 20-byte `CLight::Register` with Codex assistance. The [BPD evidence](BPD.md) supplies the light-volume prefix used here.

## Accepted fragments

| Manifest | Functions | Range | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `light_volume_manager` | constructor, destructor, `Reset`, `Update`, `AddVolume`, `RemoveVolume` | `0x80131758`-`0x801319c0` | 616 | 12 bytes at `0x802a7a64` |
| `light_volume_transition` | `UpdateTransition` | `0x80131dbc` | 136 | 68-byte message at `0x802a7a78` |

`GetVolume` (1,020 bytes) lies between them and remains original context; see below.

## Storage

The methods access seven words. `include/game/LightVolumeManager.h` names them
descriptively: four volume pointers, the outgoing volume, the remaining transition
time and the transition duration. The manager is embedded in objects (`CAnimObject`
at `+676`, `CStaticObject` at `+704`); `CStaticObject::SetLightVolumeTransitionDuration`
stores at `+728`, which is the duration word. The complete size is not established.
The manager's default-volume lookup uses `CLight::GetDefaultLightVolume`;
the broader scoped light declaration is described under Scene lights below.

## Behavior

- **Reset** clears the four slots and the outgoing volume, sets the remaining time
  to 0 and the duration to 10.0. The constructor and destructor call it.
- **AddVolume** does nothing when the fourth slot is occupied. Otherwise it inserts
  the volume before the first slot whose `priority` (`BPDLightVolume` `+0x28`) is
  lower, so higher priorities come first and equal priorities keep their arrival
  order. The shift uses `copy_backward` from the project's STLport subset; its
  guarded `memmove` matches the original exactly. If the first slot changed, it
  calls `UpdateTransition` with the previous first volume.
- **RemoveVolume** clears the first matching slot and shifts later volumes down,
  then calls `UpdateTransition` if the first slot changed.
- **UpdateTransition(outgoing)**: without a transition in progress it sets the
  remaining time to the duration. During a transition it prints the original
  "Still transitioning" message; if the volume being faded out is the new first
  volume, the remaining time becomes `duration - remaining` (the fade reverses);
  otherwise the remaining time is kept. The outgoing volume becomes the argument, or
  `CLight::GetDefaultLightVolume()` when it is null.
- **Update(step)** subtracts the step from the remaining time while a transition is
  active and ends it once the result is not greater than zero. Objects pass their
  `BeginUpdate` step, which is measured in ticks, so the default duration is ten
  ticks. `BIFunc_SetLightVolumeTransitionDuration` passes its script argument
  through unconverted.

## `GetVolume` (not reconstructed)

Read from the original instructions only:

- Without a transition it returns the first volume, or the default light volume.
- During a transition it fills a static output volume at `0x802c4d08` whose light
  records start at `0x802c4c48`. Light records are 48 bytes. The fields read are a
  type word at `+0`, a direction at `+16`-`+24`, a color at `+28`-`+36` and an
  intensity at `+40`; the light count and pointer are the volume's words at `+0x20`
  and `+0x24`. Type 1 lights are combined into output record 0; other lights use
  output record `index + 1`. Output intensity is 1.0, with the color scaled.
- The outgoing volume's lights are scaled by `intensity * remaining / duration`
  and the current volume's by `intensity * (1 - remaining / duration)`. Colors in
  an occupied output record are added. For a non-type-1 light in an occupied
  record, the direction moves from the earlier direction toward the new one by the
  current weight and is normalized with `sqrtf`.
- The direction blend builds two four-float `CVector3` temporaries.

A candidate using the [`CVector3` declaration](Matrix.md#cvector3-layout) and a
48-byte light view (the original `g_BlendLights` holds four records in 192 bytes)
reproduces the body except for about ten instructions in the blend temporaries'
store order and copy-back. It is not accepted. The output slot needs its own index
starting at one, rather than `i + 1`, to reproduce the strength-reduced addressing.

Type values other than 1, the output volume's full layout and the default volume's
contents are not established here.

## `CLight::SetLightBlock` (not reconstructed)

Its private helper `Clamp0to1` (`0x8012f6a8`, local to `light.cpp`) is accepted as
the `light_clamp` fragment with its two constants at `0x802a7944`: values above
1.0 become 1.0, values from 0.0 up are kept, and the rest become 0.0. The output
block is the private 112-byte `g_MOHLightBlock`.

`SetLightBlock(BPDLightVolume *, CDrawContext *, float)` is static. Its only
callers are `CAnimObject::DrawMesh`, `CStaticMesh::DrawMesh` and
`CStaticObject::ExecuteDraw`: it is the lighting path for objects and characters.
Level geometry uses a separate path (see below). It fills a global block at `0x8032d1e0`: three light slots with a
direction at `slot * 16` and a color at `0x30 + slot * 16`, and an ambient color
at `0x60`. The fourth component of each color is the clamped float argument.
Read from the original instructions:

1. **Volume lights.** For each 48-byte light of the volume (`GetVolume`'s result
   or a caller's choice): type 1 sets the ambient color to
   `clamp(color * intensity * 0.5)` and remembers `color * intensity * 1.75` as
   a fill color. For type 2, the weighted sum
   `0.3 r + 0.59 g + 0.11 b` of `color * intensity` is first compared with the
   lowest so far, which records the slot index it would take. The light is then
   skipped if the sum is not positive; otherwise it takes that slot with its
   direction and `clamp(color * intensity)`. Other types
   are ignored. The loop does not limit the slot index.
2. **Fill light.** With at most one directional slot used, a fill light with
   direction `(0.8, 0.566, 0.2)` and the clamped fill color takes the next slot.
   Otherwise it replaces the dimmest directional light if its own weighted sum is
   greater.
3. **Point lights.** With a draw context, each scene light in the list that context `+240`
   points to (begin pointer at `+0`, count at `+8`) is compared with the
   translation of the matrix that context `+192` points to. A light whose squared distance is
   less than its radius squared (`CLight` `+220`) gets a slot: its direction points
   from the object to the light, and its color is
   `clamp(color * (1 - distance^2 / radius^2) / 255)` from `CLight` `+200`-`+208`.
   Point lights fill unused slots first, then overwrite earlier slots from the
   last volume-filled slot downward, and stop after slot 0.
4. **Unused slots** get a zero direction and color `(0, 0, 0, 1)`.

`DistanceSquared`, `GetPosition` and the direction normalization use the
`CVector3` temporaries described in Next work.

## Scene lights

The `CLight` table at `0x802e8f48` follows the [scene-node slot map](SceneNode.md)
for slots 1-88 and adds `GetPropertyID` at slot 89. `CPropertyAnimLight`
(`0x802e8c70`) overrides the destructor, `Destroy`, `BeginUpdate` and
`GetPropertyID`; `CInstancedAnimLight` (`0x802e8998`) overrides the destructor and
`Destroy` again.
`include/game/Light.h` declares the hierarchy under `#pragma interface`.
`CLight`'s constructor writes the `IObserver` prefix, zeroes words up to `+0x3c`,
initializes the matrix at `+0x40`, and stores the color at `+0xc8`-`+0xd4`, a
float at `+0xd8` and the radius at `+0xdc`; storage between is not declared.

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `light_destroy` | `CLight::Destroy` (empty) | 4 | None |
| `light_register` | `Register` saves the pattern pointer and count in the original manager | 20 | None |
| `light_default_volume` | `GetDefaultLightVolume` returns the global `g_DefaultLightVolume` | 12 | None |
| `light_transforms` | `AttemptUpdate`, `CommitUpdate` (empty), `Attach`, `Detach`, `IsVisible`, `Reset`, `PreTransform`, `Transform`, `SetTMLocalToWorld`, `SetPosition`, `SetBasis`, `Move`, `Rotate` | 896 | 4 bytes at `0x802a79c0` |
| `light_axes` | `Orthonormalize`, `GetTMLocalToWorld`, `GetPosition`, `GetRightward`, `GetForward`, `GetUpward` | 400 | 16 bytes at `0x802a79dc` |
| `anim_light` | `CPropertyAnimLight` destructor, `InitFromProperty`, `Destroy` | 112 | None |
| `instanced_anim_light` | `CInstancedAnimLight` constructor and `Destroy` | 120 | None |
| `light_ids` | `GetPropertyID` (both), `AsLight` (both) | 28 | None |
| `light_mark_destruction` | `CLight::MarkForDestruction` | 92 | None |
| `anim_light_pools` | `CAnimLightManager::Destroy` (both overloads) | 192 | None |

The transform wrappers apply the matching `CMatrix` method to the matrix at
`+0x40`, like the camera's, without its validity flags. The axis getters assign
matrix rows through `CMatrix`'s inline row reads. `CLight::GetPropertyID` returns
0; the animated light returns the record's halfword at `+0x38`. Both `Destroy`
overrides call `CLight::Destroy` and then the manager's matching `Destroy` on the
private `g_AnimLightManager` (`light.cpp`). The color type is a four-float class
whose assignment copies each float, as `BeginUpdate`'s copy shows.

`CLight::MarkForDestruction` removes the light from `g_scene` when
`CScene::IsNodeInScene` reports it, then calls `ISubject::MarkForDestruction`.
`CAnimLightManager` keeps two 20-byte pools at `+0x44` (instanced lights) and
`+0x58` (property lights) with the same storage, used-head, free-head, capacity and
count shape as the particle-system pool; each `Destroy` overload unlinks the light
through the link at `+0xf0` without a membership check and pushes it onto the free
list. `Register(void *, int)` now establishes the pattern pointer at manager `+0x38`
and signed count at `+0x3c`. The original `Create(unsigned int)` independently
reads these fields, searches records at a 124-byte stride and passes a selected
record to `CInstancedAnimLight`'s constructor. The manager's remaining earlier
storage and base classes are not declared; the original private manager remains
bound to the `light.cpp` file record.

`Attach(node, offset, slot)` stores the slot at `+0xc4`, copies the offset into
the matrix at `+0x80` or, without one, sets that matrix to identity and copies the
node's world matrix (virtual slot 19) into `+0x40`. It then observes the node,
stores it at `+0xc0`, moves the light under it in `g_scene` (`Remove`, then
`Add`) and returns 1. `Detach` stops observing, clears `+0xc0` and returns 1. The
unaccepted `CLight::BeginUpdate` copies the parent's world matrix into `+0x40` and
multiplies the `+0x80` matrix onto it each update. The header names these fields
`attachSlot`, `parent` and `attachTransform`.

`IsVisible` builds a `CVolSphere` of the light's radius around its position and
returns `CVolSphere::TestVisibility`. `include/game/Volume.h` declares only what
this needs: `IVolume`'s inline destructor (it resets the table pointer, as the
original `_._7IVolume` does) and `CVolSphere`'s radius at `+12` and center at
`+16`, established by its `GetRadius`, `SetRadius`, `GetExtents` and 32-byte
`Create`. Other `IVolume` virtual slots are not declared.

## Animated lights (partly reconstructed)

`CPropertyAnimLight` derives from `CLight`. Its constructor keeps the record
pointer at `+224`, places the light at the record's floats `+16`-`+24`, and sets
the radius (`+220`) to the record's signed halfword at `+46` divided by 16.
`BeginUpdate` advances a frame index at `+228` by the update step converted to an
integer, using the mode byte at record `+112` and the count at `+114`, then copies
the four bytes of that frame's word from the color array at `+116` to the floats
at `+200`-`+212`. `PatchUpAnimLight` converts the per-frame floats at `+120` but
not the color words, so the bytes are used in file order. Animated lights reach
objects and characters through `SetLightBlock`'s point-light step, and level
geometry through the compartment light cache.

## Level geometry (not reconstructed)

`CCompartment::Draw` walks the same scene-light list. For each light it transforms
the light's position into the compartment's box axes and accumulates the squared
distance outside the box's half-extents; a light whose radius (`CLight` `+0xdc`)
reaches the box is recorded in a bit mask and added with
`CLightCache::CLightBlock::AddLight`. `AddLight` keeps at most sixteen 32-byte
entries per compartment: the color clamped to 0-255, the radius, the position and
1.0. The mask is then passed to `CCompartment::CullTreeNodes` for the partition
tree; `CActiveLightIndices::SetLightIndex` suggests per-group light slots. The
shading applied to level surfaces was not read.

## Next work

- **`CPropertyAnimLight`'s constructor** compiles to the original instructions,
  but its eight-byte integer-conversion constant is aligned relative to the whole
  `light.cpp` constant pool. A fragment beginning partway through that pool cannot
  reproduce the alignment without padding, so it waits for a larger contiguous
  fragment.
- **`BeginUpdate`** reproduces all but about six instructions, which concern
  whether one color component is forwarded from a register during the final copy.
- **`Pitch`, `Roll` and `Yaw`** save the position, zero it, rotate about the unit
  X, Y or Z axis by the negated angle with a pre-multiplication through
  `CMatrix::s_TempMat`, and restore the position. Their local `CMatrix` needs an
  inline default constructor that only calls `InitClass` when needed. A candidate
  is within about thirteen instructions of register and store scheduling.
- **`CLight::BeginUpdate`** is within about six instructions (the register that
  holds `this` before the virtual call).
- `GetVolume` (above) and `SetLightBlock` remain. With `IsVisible` accepted, the
  constructor's constant pool now only waits on `Pitch`, `Roll` and `Yaw` to form a
  contiguous fragment from `0x80130450` whose `.rodata` starts 8-aligned.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
accepted fragment, including the constants and message; the manager fragment also
uses the project's STLport include profile for `copy_backward`. The complete rebuilt analysis
image is identical. No instructions are patched, and no generated code or data is
discarded. Runtime behavior has not been tested.
