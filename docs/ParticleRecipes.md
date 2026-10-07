# Particle recipe access

The lead for this work came from [kyleckroeger’s offer of PS2 research in issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4), specifically the connection between particle recipes and `CParticleSystem` getters. We appreciate that direction: it led to matching GameCube code and a usable field map. The detailed PS2 notes had not been supplied when these fragments were reconstructed. Every accepted layout and body below was recovered independently from the pinned GR8E69 executable; no PS2 offsets or code were imported. Codex assisted reconstruction and verification.

## Accepted scope

The `src/particles/` fragments reconstruct 68 functions and 3,688 executable bytes: recipe getters, seed and state access, seed advance, the system pool, placement operators, scene-node overrides, destruction and stop paths, transform wrappers, particle-clock access, procedural-definition validity, emission setters, lifetime handling, position/velocity bound setters, and cached vector/axis access. Their exact original names and ranges live in `config/GR8E69/particle_*.json` and `mesh_particle_placement.json`. The original misspelling `GetEmmisionRate` / `GetEmmisionDelay` is retained.

`include/game/ParticleRecipe.h` supplies scoped runtime storage views, not a complete particle engine or an on-disc `.lfc` parser. Only these accesses are established:

| Storage | Observed access |
| --- | --- |
| `CParticleSystem` | Matrix at `0x90`; definition pointer at `0x140`; deactivation tick value at `0x150`; unsigned seed at `0x160`; flag word at `0x170`; pool-list link at `0x180` |
| `g_particleSystemList` | 20-byte global: storage block, used-list head, free-list head, capacity, in-use count |
| `CProcParticleDef` | Contents pointer at `0x04`; leading word remains unknown |
| `LevelFileContentsStruct_` | Pointer to an entry-pointer array at `0x08`; first two words remain unknown |
| Recipe entry | Integer or float value at `0x08`; first two words remain unknown |

The recipe-entry union describes the two access types, not a recovered historical union declaration. `CParticleSystem` now derives from `IParticleSystem` and the scene-node classes recovered from original virtual tables; see [SceneNode.md](SceneNode.md). The definition prefix and all base-class data members are still omitted. Do not allocate objects or infer their full size from these views. Return-type spelling, field/helper names and visibility are reconstruction choices; method names and parameter encodings come from original symbols. The earlier output helpers write only the first three floats of `CVector3`; the type itself is now declared (see [Matrix.md](Matrix.md#cvector3-layout)).

## GameCube recipe indices

These are zero-based positions in the runtime entry-pointer array. Numeric indices and accessed values are supported directly by matching getters, and paired setters independently corroborate emission, lifetime and bounds. They are not recovered field-name strings, file offsets, timing units or enum definitions.

| Index | Access | Meaning supported by these functions |
| --- | --- | --- |
| 3 | int | Emission rate |
| 4 | int | Emission delay |
| 5 | float | System lifetime |
| 7–9 / 10–12 | float triples | Minimum / maximum particle position |
| 13–15 / 16–18 | float triples | Minimum / maximum particle velocity |
| 22–24 / 39–41 / 26–28 / 42–44 | int triples, narrowed to bytes | First / second / third / fourth RGB outputs of `GetParticleColor` |
| 25 / 45 / 29 | int, converted to float | First / second / third outputs of `GetParticleAlpha` |
| 30 | int | Render-type value; enum members unknown |
| 31, 33 / 32, 34 | float pairs | First / second size-vector X and Y outputs; both Z outputs are zero |
| 35 | float | Additional output of `GetParticleSize`; exact role unconfirmed |
| 36 | float | Particle lifetime |
| 37 / 38 | float | First / second rotation outputs |
| 46 | int | No-fog flag: `GetFogEnable` returns one when this entry is zero |
| 47 / 48 | float | First / second fade outputs |

Emission setters clamp nonpositive integers to one. The lifetime setter uses `!(value >= 0.0f)`, so negative and unordered inputs become zero while negative zero is retained. Position/velocity setters first store the requested triple and then adjust the opposite bounds using negated `>=` or `<=` comparisons. Their unordered-input behavior and repeated pointer reloads are preserved. No null checks, additional bounds checks or runtime fixes have been added.

The size getter preserves its three-float writes and alias-sensitive ordering. Seed methods access the system directly. The particle lifetime setter delegates to the original named procedural-definition method; its implementation is separately accepted in this batch. The fog result is modeled as a normalized integer flag, matching its emitted ABI and inversion.

The alpha getter converts signed integers to floats without normalization or clamping. Its compiler-generated eight-byte integer-conversion constant is checked at `0x80299cc0`. The color getter takes the low byte of each integer component and writes four packed colors with alpha `0xff`. It constructs and copies each output before looking up the next, preserving alias-sensitive ordering. `CColor` remains opaque: the local `ColorPrefix` union describes only the four accessed bytes. Its signed alpha member supplies the observed `-1` initialization; it does not establish the original member's signedness. Independent inspection of `Color(const CColor&)` and `CFont::SetColor(CColor)` corroborates the four-byte color prefix, without establishing the complete historical class.

## State, transforms and clock

The state accessors establish the following bits in the word at `+0x170`, using conventional least-significant-bit numbering. The scoped bitfield view follows the target compiler's big-endian allocation order; other bits remain explicitly unknown. Names and the declaration are reconstruction choices supported by the accesses, not a recovered historical header.

| Bit | Access |
| --- | --- |
| 30 | Active: `Start` sets it; `DeActivate` clears it; `IsActive` reads it |
| 28 | `IsMoving` |
| 27 | `IsRotating` |
| 26 | `IsDestroyable` / `SetDestroyable` |
| 25 | `IsDying` |
| 24 | `UseFade` |
| 23 | `IsEternal` |
| 22 | `Profile(bool)` writes it |

The setters preserve all other bits. Flag getters are represented with normalized unsigned integer returns, matching the observed ABI; original return-type spelling is unproven. `GetDef` exposes the already modeled definition pointer.

Both `GetLocalToWorld` and `GetTMLocalToWorld` assign the matrix at `+0x90` through the accepted `CMatrix::operator=`. The particle `Orthonormalize` wrapper passes that same matrix to the separately reconstructed `CMatrix::Orthonormalize` body; see [matrix evidence](Matrix.md). These independent accesses establish the matrix's placement and use the separately verified 64-byte matrix representation. No missing constructor or complete particle inheritance layout is supplied.

`DeActivate` clears the active bit and then stores `CPSManager::GetCurrentTicks()` at `+0x150`. The scoped `ParticleClock.h` interface reconstructs two static manager methods: `Update(float)` adds its argument to the shared float, and `GetCurrentTicks()` reads it. The original private `g_numCurrentTicks` at `0x802c1c44` is scoped to the `particlesystemmanager.cpp` symbol record and remains external storage, with no source/data credit. Tick units, initialization policy and full manager storage are not inferred from these methods.

## Seed advance and trivial members

`IncrementSeed(unsigned int, int)` stores its first argument in the global
`g_CurrentSeed` (`0x802c1c00`, original external data, no credit), applies the
linear-congruential step `seed * 1103515245 + 12345` eleven times per counted
item, and returns the stored seed. A nonpositive count leaves the seed as given.
`CParticleSystem::Render` draws exactly eleven values from the same generator per
particle, masking each to 31 bits, which supports reading the count as a number of
particles to skip. That reading is an interpretation of call shape, not runtime
evidence. The inline step helper and its name are reconstruction choices; the
target compiler emits the same bytes whether or not the step returns the masked
value, so Render's draw helper is not established by this function.

The class-scoped placement `operator new(unsigned int, void *)` returns its storage
argument, and `operator delete(void *)` is empty. `IsDrawEnabled` returns one;
`Reset` and `Halt` are empty. All three override scene-node virtual slots (27, 68
and 69). The integer return of `IsDrawEnabled` describes the emitted ABI rather than
a recovered return type.

## Virtual overrides

The original `CParticleSystem` table at `0x802e18c8` places each method below; the
slot map is in [SceneNode.md](SceneNode.md).

| Function | Body |
| --- | --- |
| `Stop` | Virtual `DeActivate()` (slot 91) |
| `SetLocalToWorld` | Virtual `SetTMLocalToWorld(matrix)` (slot 71); not itself a table entry |
| `Terminate` | Clears flag bit 31 only when the destroyable bit is clear, then calls virtual `MarkForDestruction(1)` (slot 1) |
| `MarkForDestruction(int)` | When destroyable, calls `ISubject::MarkForDestruction` directly, then `g_scene.Remove(*this)` |
| `Destroy` | When destroyable, `ReleaseSystem(this)` followed by `delete this` (virtual deleting destructor, slot 2, flag 3) |
| `AsMovingNode`, `AsProceduralParticleSystem` | Return `this` (both const overloads) |

Bit 31's meaning is still unknown. The `As*` return types follow the casts' names and the zero-offset base
layout. They are not recovered declarations. `g_scene` (`0x803e6d90`) and
`CScene::Remove` remain original context.

## System pool

`g_particleSystemList` (`0x802fae7c`, 20 bytes, original external storage) holds
five words. `InitClass` stores 128 at `+12`, allocates `128 * 400` bytes through
`DWI_alloc`, stores the block at `+0` and `+8`, clears `+4` and `+16`, and links
consecutive 400-byte elements through `+0x180`. `ResetSystem` repeats that
threading. The scoped view names these words storage, used head, free head,
capacity and count. The 400-byte stride is pool evidence; the prefix view still
does not declare a complete `CParticleSystem` allocation, and `InitClass` and
`ResetSystem` are not reconstructed here.

`AllocSystem(int)` takes the free-list head, moves it to the front of the used
list and increments the count. A positive argument allows allocation while
`capacity - count >= 0`; zero requires more than nine unused slots; a negative
argument never allocates. A null result means no system. The argument is named
`priority` descriptively. The second path's explicit null assignment is
required by the emitted block order; the static pop helper is a reconstruction
choice. `ReleaseSystem` unlinks the system from the used list (from the head, or
after the first predecessor whose link matches), decrements the count and pushes
it onto the free list. It does not check membership, so a system that is not in
the used list still decrements the count.

The `MeshParticleSystem` placement operators match the `CParticleSystem` pair.
Its class view declares only those members.

## Verification and remaining work

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` matches every allocated byte in each accepted fragment, including the two four-byte zero constants and eight-byte integer-conversion constant. This is a working profile, not proof of the original compiler release. All fragments also pass the complete rebuilt analysis-image comparison and snapshot safeguards; constants and original dependencies earn no code credit. Runtime and emulator behavior remain untested.

The vector members at `+0xd0`, `+0xe0` and `+0xf0` are the system's initial
velocity, acceleration and particle acceleration, named after their getters.
`GetSystemInitialVelocity`, `GetSystemAcceleration` and `GetParticleAcceleration`
assign them to the output. `SetSystemInitialVelocity` assigns the initial
velocity, clears flag bit 29, and sets the moving bit (28) when the initial
velocity's or the acceleration's squared length is at least `1e-6`; the two
comparisons are spelled as the original branch forms require. `GetPosition`,
`GetRightward`, `GetForward` and `GetUpward` assign the local-to-world matrix's
fourth, first, second and third rows through `CMatrix`'s inline row reads.

Construction, simulation and rendering remain outside this batch. The contributor’s [PS2 particle recipe notes](research/ps2/particle-recipes.md) are now available to help interpret additional fields and names, but each cross-platform claim still needs a separate GR8E69 check. These matched accessors provide concrete locations for those checks.
