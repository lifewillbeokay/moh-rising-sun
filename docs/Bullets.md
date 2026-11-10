# Bullets

`src/bullets/` reconstructs bullet accessors, movement, collision and explosion
behavior from the pinned GR8E69 executable. Accepted fragments and their evidence
are described below; current totals live in the verified progress snapshot.
Claude Code and Codex assisted the analysis and verification.

## Declarations

The `CBullet` table at `0x802e9eb0` follows the [scene-node slot map](SceneNode.md)
for slots 1-88 and adds slots 89-102: `Init`, `Shutdown`, `GetUp`, `GetDamage`,
`GetScriptBulletType`, `GetFiredBy`, `AsThrown`, `AsProjectile`, `GetVelocity`,
`SetExclusionPair`, `Penetrate`, `SetDamage`, `SetBlastRadius` and
`SetExplosionParticleSystem`. `CProjectileBullet` (`0x802ea210`) adds
`SetDrawForward` and `CThrownBullet` (`0x802ea5a8`) adds `IsCollisionEnabled` at slot
103. `include/game/Bullet.h` declares `CBullet` under `#pragma interface` with a
scoped view: a collision ID at `+0x40` (`Shutdown` resets it to -1, and both derived
`GetCollisionId` overrides read it), the local-to-world matrix at `+0x70` (the axis
getters read its rows) and two floats at `+0xb0`/`+0xb4`. Return types follow the
same conventions as [SceneNode.md](SceneNode.md): `AsThrown`/`AsProjectile` return the
named classes; `GetFiredBy` returns `ISceneNode *`, inferred from its name and the
derived classes' pointer field; `GetScriptBulletType` is an `int` placeholder.

## Accepted fragments

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `bullet_base` | `Shutdown`, `Draw`, `BulletBaseDraw`, `IsDrawEnabled` | 80 | 8 bytes at `0x802a8c1c` |
| `bullet_defaults` | axis getters, `GetTMLocalToWorld`, `AsBullet`, and the base defaults of slots 92-102 | 440 | 20 bytes at `0x802a8c50` |

`Shutdown` sets `+0xb0` to 0, the collision ID to -1 and `+0xb4` to the largest
float, in that order. `Draw` calls the empty global `BulletBaseDraw`. The base
`GetDamage` returns 0, `GetScriptBulletType` -1, and the casts and `GetFiredBy`
null; the other defaults are empty.

## Derived accessors

`Init` stores its `ProjectileBulletProperties_struct` or `ThrownBulletProperties_struct`
argument at `+0xc8`; the scoped record views name only the fields read below.

| Manifest | Functions | Code bytes |
| --- | --- | ---: |
| `projectile_collision_id`, `thrown_collision_id` | `GetCollisionId` (both) return the collision ID at `+0x40` | 16 |
| `projectile_exclusion_pair` | `CProjectileBullet::SetExclusionPair` (empty) | 4 |
| `projectile_set_damage` | `SetDamage` writes record `+76` | 12 |
| `projectile_accessors` | `GetDamage` (record `+76`), `GetFiredBy` (`+0xe0`), `AsProjectile`, `SetDrawForward` (bit 28 of the word at `+0x10c`) | 40 |
| `thrown_setters` | `SetDamage` (record `+92`), `SetBlastRadius` (record `+96`), `SetExplosionParticleSystem` (word array at `+0x264` indexed by type) | 40 |
| `thrown_collision_light` | `IsCollisionEnabled` (returns 1), `GetAttachedLight` (`+0x254`) | 16 |
| `thrown_accessors` | `GetDamage` (record `+92`), `GetFiredBy` (`+0xe0`), `AsThrown` | 24 |

`+0xe0` is named from `GetFiredBy`. Both original `Init` routines pass its value
as the `ISceneNode` reference argument to `CScene::IncludeCollision`, supporting
the declared pointer type. The surrounding observer storage remains unknown. The
bounding-volume getters (embedded 80-byte volumes at `+0x110`/`+0x160` and
`+0x1a0`/`+0x1f0`) and `CThrownBullet::SetLightVolumeTransitionDuration` are not
reconstructed.

Maintainer review independently checked the thrown-bullet initializer at
`0x80150b24`: its default explosion setup writes CRCs to `+0x264`, `+0x268`,
`+0x26c` and `+0x270` (stores at `0x801519fc`, `0x80151a0c`, `0x80151a1c`
and `0x80151a2c`). The header therefore represents four observed particle-ID
slots, replacing the earlier one-element placeholder. This avoids indexing
beyond the declared array for those slots without claiming a complete object
size or recovering the enum member names. The setter retains the original
unchecked indexing; no new range check is introduced. This correction and its
independent GR8E69 verification used Codex.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces both
fragments, including their constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.

## Factory and later accessors

`CBulletFactory` holds the projectile and thrown property tables (96 and 104 bytes per
row, from the indexing) and three `CBulletFactoryHelper<T, P, E>` members, each 64 bytes:
projectiles, projectiles with collision id 3, and thrown bullets. The helper instantiations
are external here. `include/game/Bullet.h` names the row's CRC `+0x00`, weight `+0x04`,
sprite `+0x48`. Names not in a symbol, such as `mountedProjectiles`, are inferred.

`CProjectileBullet::OnCollision` sets `hitPending`, forwards to the other node's
`HandleBulletCollision`, and takes the collision time only if the handler cleared the bit.
`CThrownBullet::ApplyForceTo` scales the direction by the force, 1/512 and the row's
weight, and mirrors the result to `+0x150`. `Explode` writes the inverse of bit 27 to bit
28 of `+0x25c`. `IsVisible` calls `CVolFiber::TestVisibility` directly in both classes.

`CThrownObject` in `include/game/Thrown.h` is a scoped view with `CStaticObject` as its
base; `LimitSpeed` caps the speed at twice the world volume's radius while `+0x20c` is not
-1. Its source lives in `src/weapons/`.

## Explosions and projectile updates

`CExplosion` derives from `dwi::IVisitor<ISceneNode>` (symbols); its sphere is at
+0x10, then strength, damage, the script bullet type, the source node and two flags
(+0x30 to +0x44). `Visit` matches at 1,996 bytes. It skips animated players and player
weapon objects, scales damage by a falloff of `1 - (distance - 1) / radius` clamped to
0.5, gives a direct-hit node full damage in multiplayer (`g_pDirectHitNode`), and sends
events 178 and 40 with an eight-word `BS_STRUCT_HitByBulletInfo_struct`. Only the field
meanings are inferred; event numbers and constants are read from the code. Unlike
Frontline, Rising Sun applies no explosion push here: the original still calls
`AsPlayerObject` and discards the result at that point.

`explosion_constructor` accepts the 228-byte constructor and its eight bytes of
float constants. It builds the sphere, stores strength/damage/type/source and both
flags, makes noise of type 2 at six times the strength, visits nodes in the sphere
with volume type 0 while excluding the world node, then notifies the world object
when present. Neither the unused visitor return value nor the final world callback
changes the constructor's returned object pointer.

The two reordered argument moves in the earlier draft came from an incorrect
`void` declaration of `CScene::VisitAllNodesInVolume`. Its original body returns
1 when a visitor stops traversal and 0 otherwise. Correcting the declaration to
`bool` reproduces the constructor with the existing ProDG profile. The scene
routine itself is now reconstructed too; see [scene visitors](SceneNode.md#scene-visitors).

Thanks to kyleckroeger for PR #47's explosion and thrown-bullet work and the
[Frontline constructor reconstruction](https://github.com/kyleckroeger/moh-frontline-decomp/blob/33f5adccac2e27f26c2abcbb7acd7093ab16f2d6/src/game/explosion_dtor.cpp),
used as a semantic reference under that project's CC0 license. Rising Sun's
reference position, noise type, additional flag and all accepted bytes were
checked independently against GR8E69. Parameter and field names are descriptive.

`ShouldHeightCheckBeFlipped`, `TestRay` and `CheckForObstructions` are maintained
as [registered partial source](research/explosion-grenade-updates.md#maintained-partial-source).
They receive measured similarity but no exact or linked credit. The obstruction
routine normalizes the horizontal direction and tests three rays, adjusting the sample heights for crouching players,
soldier look positions and the height-flip predicate. Understanding these operations
does not establish a byte match or complete class layouts.

`SetScaled` and `SetSum` in `ProjectileUpdate.cpp` are descriptive inline helpers; the
original store and reload order requires a by-reference result, but their names and
their existence as functions are inferred.

## Thrown object updates

`CThrownObject::OnCollision`, `UpdateCollisionVolumes`, `Draw`, `BeginUpdate`,
`AttemptUpdate`, `CommitUpdate` and `HandleBulletCollision` are reconstructed. The field
names at +0x300 to +0x3a0 and the bit names in the flags word at +0x394 are Frontline's
readings or inferred. Scene node slots are checked against the disc: where Frontline
tests `AsWorldObject`/`AsSoldierObject` for script state 1, Rising Sun tests
`AsAnimObject`/`AsPlayerObject`. Events are 224 and 225, against Frontline's 231 and 232.

`CThrownBullet::Constrain` (bounce, friction and the optional keep-speed cheat word at
`g_cheats` +0x1c, then a clamped time-of-impact position) and `CreateExplosionEffect` are
reconstructed. `CExplosion` is 0x50 bytes: slot reuse across `CThrownBullet::CommitUpdate`
fixes it. `CommitUpdate` is maintained as partial source alongside the explosion
helpers, with its explosion/effect construction, grenade and bazooka sound aliases,
event 141 notification, destruction and attached-light update. The four functions
cover 3,968 target bytes; all remain original context in the verified image.
See the linked research notes for measurements and reproducible isolated PPC checks.

`thrown_bullet_penetrate` reconstructs the complete 396-byte `Penetrate` at
`0x80151e18` and its 16 constant bytes at `0x802a9510`. It uses the stored normal
at `+0x160`, not the collision argument. The forward-vector dot product selects
`+0.02` times that normal when the dot is not `<= 0`, or `-0.02` otherwise.
It adds the offset to the current matrix position, writes the matrix and cached
position, then clears bits 30 and 28 of the word at `+0x25c`. Other bits and
the vectors' fourth components remain unchanged. The normal is not normalized
here. The negated comparison also preserves the original unordered-float path.
The staged dot-product sum and by-reference vector helpers reproduce the original
operations; their spelling and names are reconstruction choices.

## Grenade response queries

`grenade_response_queries` reconstructs seven adjacent functions from
`GrenadeResponseBehavior.cpp` (original file record 1079), totaling 924 executable
bytes and 48 constant bytes at `0x802a4abc`. Together with `Penetrate`, these
fragments add eight whole functions and 1,320 game-code bytes. Codex assisted the
reconstruction from GR8E69, building on the interfaces contributed in PR #47.

| Function | Address | Code bytes | Behavior |
| --- | --- | ---: | --- |
| `GetVectorToGrenade` | `0x800f07dc` | 204 | Queries both positions and returns grenade minus soldier position. |
| `IsAlive` | `0x800f08a8` | 76 | Tests whether the soldier's AI status selector 0 is positive. |
| `IsCatchable` | `0x800f08f4` | 384 | Checks relative height and projections onto the soldier's forward and right vectors. |
| `IsStoppedOnGround(soldier, grenade)` | `0x800f0a74` | 96 | Computes the relative vector and full three-dimensional velocity magnitude, then calls the scalar predicate. |
| `IsStoppedOnGround(delta, speed)` | `0x800f0ad4` | 52 | For finite values, requires relative height below 0.09 and speed below 0.06. |
| `DistanceSquaredToGrenade` | `0x800f0b08` | 60 | Returns squared XY distance; height is ignored. |
| `InDanger` | `0x800f0b44` | 52 | Tests squared XY distance strictly below 36. |

Catchability requires height strictly between 1.3 and 1.9, forward projection
strictly between 0 and 1, and side projection strictly between -0.1 and 0.5.
A nonzero signed halfword at weapon properties `+0x62` selects side bounds
(-0.3, 0.2). `field_62` names only that observed selector; its original meaning
is unknown. A missing current weapon uses the default bounds. The grenade pointer
and float argument are unused by this helper. It does not itself check whether
the soldier is alive or whether the grenade has exploded.

`GrenadeResponse.h` exposes the recovered free-function names and parameter types.
Return types follow the original bodies and callers. The soldier remains a
member-only view, so virtual queries use the established `ISceneNode` interface;
these helpers do not establish a complete soldier or behavior-object layout.
Comparisons retain the original unordered-float behavior rather than replacing
negated comparisons with superficially equivalent finite-value expressions.

Both fragments use the unchanged ProDG 3.8.1
`-O2 -G0 -fno-exceptions -fno-implicit-templates` profile. Forty-eight isolated
PowerPC scenarios execute the production-linked functions, covering signed and
non-unit normal offsets, flag preservation, adjusted virtual calls, life-state
values, strict catch/ground/danger boundaries, alternate weapon selectors, and
unordered comparisons. External queries, matrix `SetPos`, AI status, weapon
lookup and `sqrtf` are stubbed. These are not game or Dolphin runtime tests.

The explosion height/ray/obstruction helpers, thrown-bullet `CommitUpdate`, and
the larger grenade-response update remain unmatched research. No partial credit
is included for them. The [follow-up behavior notes](research/explosion-grenade-updates.md)
record their branch conditions, isolated original/candidate execution comparisons
and remaining compiler differences. The grenade/bazooka
[sound dispatcher and supporting methods](Sound.md#grenade-and-bazooka-sound-events)
are now matching source, with separate credit for their own ranges.

## Factory resources

`CBulletFactory::LoadBulletGUISprites` allocates through `DWI_alloc` with the labels
`source/weapons/bulletfactory.cpp:567` and `:603`, which record the original path and
line. Projectile sprite names sit at +0x27 of each 96-byte record and thrown ones at +0x27
of each 104-byte record. `DeleteBulletGUISprites` is not yet
accepted: only two loop registers are swapped.

`CWeaponFactory::LoadWeaponGUISprites` builds `data\<name>` in a stack buffer and then
loads the sprite from the bare name, so the built path goes unused in this build. The
source keeps that as it stands; the icon field names at +0x38 and +0x58 are inferred.

## Factory helper instantiations

`CBulletFactoryHelper`'s twelve instantiated methods are explicit instantiations, written per method
because the original image interleaves the two types' functions. `AllocateGameObject` is an inline
template of `CGameObjectFactory` here: the allocator's record is inferred from its two callers. The
pooled sizes of 448 and 640 bytes are the arguments `Init` passes; the bullet views stop short of them,
so `PooledSize` states them. Each bullet's `IGameObject` base at `+0x38` is written as an explicit
conversion because the headers do not model that base yet.
