# AI targeting reconstruction

Sixteen fragments in `src/ai/` reconstruct **27 functions and 3,080 executable
bytes**, plus 100 required read-only bytes (constants, a diagnostic string and
alignment padding). The original `AITargeting.cpp`
file record (index 1071) supports their grouping; these fragments are not complete
historical translation units. Codex assisted the reconstruction from the pinned
GameCube GR8E69 executable. No external source was imported.

## Accepted functions

| Unit | Functions / executable bytes | Original start | Read-only data |
| --- | --- | --- | --- |
| `ai_target_position` | `GetPosition`: 1 / 304 | `0x800eea68` | 4 bytes at `0x802a48a0` |
| `ai_target_forward` | `GetForward`: 1 / 304 | `0x800eeb98` | 4 bytes at `0x802a48a4` |
| `ai_target_identity` | `Matches`, `GetScriptObject`, `SetAsNonAITarget`, `Nullify`, `SetAsAITarget`, both constructors: 7 / 236 | `0x800eecc8` | None |
| `ai_targeting_last_seen` | `GetLastSeenTargetPosition`: 1 / 64 | `0x800ece88` | 4 bytes at `0x802a47a4` |
| `ai_targeting_modified` | `GetModifiedTargetPosition`: 1 / 64 | `0x800ecec8` | 4 bytes at `0x802a47a8` |
| `ai_targeting_seen_state` | `SetLastSeenTargetPosition`, `IsLastSeenTargetPositionSet`, `CouldVisionSeeTarget`: 3 / 100 | `0x800ecf08` | 4 bytes at `0x802a47ac` |
| `ai_targeting_update` | `Update`: 1 / 128 | `0x800ecf6c` | None |
| `ai_targeting_reaction` | `UpdateReactionTime`: 1 / 120 | `0x800ecfec` | 4 bytes at `0x802a47b0` |
| `ai_targeting_queries` | `GetTargetScript`, `IsNonAITarget`, `HasValidTarget`: 3 / 180 | `0x800edb18` | None |
| `ai_targeting_distance` | `GetDistanceToTargetSquaredXYZReal`: 1 / 116 | `0x800ed948` | None |
| `ai_targeting_guess` | `GetTargetGuessPosition`: 1 / 60 | `0x800ee468` | None |
| `ai_targeting_non_ai_update` | `UpdateNonAITargetPosition`: 1 / 336 | `0x800ed424` | 4 bytes at `0x802a47c0` |
| `ai_targeting_set_non_ai` | `SetNonAITarget`: 1 / 412 | `0x800edbcc` | 48 bytes at `0x802a47ec` |
| `ai_targeting_sniper_mode` | `ChooseSniperAimMode`: 1 / 88 | `0x800edd68` | None |
| `ai_targeting_bazooka_mode` | `ChooseBazookaAimMode`: 1 / 124 | `0x800ede90` | None |
| `ai_targeting_aim_queries` | `GetAdjustedAimTargetPosition`, `DoBlindFire`: 2 / 444 | `0x800ee4a4` | 24 bytes at `0x802a4888` |

Exact function ranges, bindings and external dependencies are recorded in the
corresponding `config/GR8E69/ai_target*.json` manifests. Data earns no code credit.

## Target value and object prefixes

`include/game/AITargeting.h` shares one declaration for each original class tag.
Member names and the inline `GetTarget`/`HasTarget` helper names are descriptive
reconstruction choices. Ordinary return types and integer signedness are not
encoded in the method symbols.

`CAITarget` is a 12-byte value: a four-byte selector at zero, an AI-object pointer
at `+4`, and a script-object pointer at `+8`. Its constructors/setters and the
three-word value copies in `Update`, `HasValidTarget` and the distance query agree
on this representation. No observer registration or reference-counting operation
is present in the accepted value operations.

Default construction and `Nullify` clear all three words. The AI constructor and
`SetAsAITarget` store the AI pointer, selector zero and script pointer zero.
`SetAsNonAITarget` stores selector one and the script pointer, clearing the AI
pointer. `GetScriptObject` returns the script pointer for a nonzero selector;
otherwise it reads the AI object's script pointer at `+0x0c`, or returns null when
the AI pointer is null.

`Matches` preserves a notable original behavior. Different selectors return
false. With equal selectors, a nonzero selector and equal script pointers return
true; otherwise it compares the AI pointers. Consequently, two values produced
by `SetAsNonAITarget` compare equal even if their script pointers differ, because
both AI pointers are null. This fallback is retained, rather than replacing it
with a conventional tagged-union equality rule.

The `CAIObject` declaration is only a prefix through `+0x6f`. It exposes the
script-object pointer at `+0x0c`, a position vector at `+0x30`, and a forward vector
at `+0x60`. `GetScriptObject`, the distance query and the two target spatial
queries corroborate these accesses. No complete allocation, inheritance
relationship, virtual interface or meaning for the intervening bytes is claimed.

`CAITargeting` is a prefix through `+0x107`:

| Offset | Observed storage/use |
| --- | --- |
| `+0x04`, `+0x08`, `+0x0c` | Physics, object-parameter and scene-node pointers |
| `+0x10` | Embedded 12-byte target value |
| `+0x20` | Last-seen position vector |
| `+0x30` | Time value used by `UpdateReactionTime` |
| `+0x34`, `+0x38` | Four-byte last-seen and vision-visible values |
| `+0x40` | Target position written by scene/trigger queries |
| `+0x70` | Modified target-position vector |
| `+0xf8` | Time written after updating a script target |
| `+0xfc` | Blind-fire selector; observed values 0, 1 and 2 |
| `+0x100` | Integer weapon selector passed to `AdjustAimPosition` |
| `+0x104` | `AIFILTER_WEAPON_AIM_MODE` value passed by address |

The original constructor at `0x800ecde0` corroborates the first three pointer
stores, calls the target constructor at `+0x10`, initializes the flag/time fields,
and initializes vector fourth words at `+0x2c` and `+0x7c`. That constructor and its
virtual table remain original. The new position and trailing fields follow the
accesses in `SetNonAITarget`, `UpdateNonAITargetPosition` and `DoBlindFire`; a
compiler size check fixes this view at `0x108` bytes. The initial dispatch word,
`+0x1c`, `+0x3c`, `+0x50`–`+0x6f` and `+0x80`–`+0xf7` stay opaque. The complete
class extends beyond this view; do not allocate an instance from this prefix.

## Position, validity and update behavior

`CAITarget::GetPosition` and `GetForward` return vectors by value. For an AI target
(selector zero), they copy the respective vector directly from the AI object.
For a non-AI target, they first test the script object's embedded
[weak game-object reference](Script.md#script-game-object-bridge). If that reference
is nonnull and its virtual `GetSceneNode` returns nonnull, they call `GetSceneNode`
again and query the node's position or forward direction. Otherwise they query
the script object's native `TriggerObject`.

The repeated virtual call and intervening pointer re-reads are preserved; the
node is not cached across the first callback. The non-AI branch default-constructs
a local vector (only its fourth word is initialized), then returns a copy. The
shared CVector3 copy operation writes XYZ and resets the returned fourth word to
1.0f. The queries add no null checks for the selected AI/script object or fallback
trigger. The original TriggerObject spatial implementations remain external.

The remembered-position getters use the shared [CVector3 assignment](Matrix.md#cvector3-layout),
including its by-value temporary and preservation of the destination's fourth
word. `SetLastSeenTargetPosition` assigns the position and sets the last-seen
value to one; it does not update time. The two flag getters return their stored
four-byte values without normalization. `GetTargetGuessPosition` copies the
last-seen position and returns true when that flag is nonzero; otherwise it
returns false without changing the output vector.

`GetTargetScript` delegates to the target value. `IsNonAITarget` requires both a
nonzero selector and a nonnull script pointer. `HasValidTarget` checks the receiver
and then a by-value target copy, returning true if either payload pointer is
nonnull, independently of the selector. Its original null-receiver branch is
preserved by the target compiler; this is not a portable C++ guarantee for calls
through a null object pointer.

`GetDistanceToTargetSquaredXYZReal` uses the existing physics-position prefix at
`+0x10`. For a valid non-AI target, it measures squared XYZ distance to the modified
position. Otherwise it copies the target value and measures squared XYZ distance
to the AI object's position. That second branch assumes a valid AI pointer;
there is no added fallback or distance clamp.

`UpdateReactionTime` always returns true. Its stores preserve the original
comparison direction:

- With neither target pointer set, it clears the last-seen flag when
  `!(lastSeenTime + 3.0f <= globalTime)`.
- With a target pointer and a nonzero last-seen flag, it stores the current global
  time in `lastSeenTime`.
- Otherwise it leaves those fields unchanged.

The negated comparison deliberately retains unordered floating-point behavior.
It must not be rewritten as a conventional expiration check. The time source is
the [reconstructed AI global clock](Paths.md#ai-clock).

`Update` first calls `UpdateReactionTime`. When that returns true, it uses a target
value copy to check the AI pointer and calls the still-original
`UpdateTargetPosition` if present. Otherwise it uses another value copy to check
the script pointer and calls the reconstructed `UpdateNonAITargetPosition` if
present. It does not dispatch from the selector word or clear positions when both
pointers are absent.

## Script-target setup and updates

`SetNonAITarget` first nullifies the target value. For a nonnull script argument,
it sets the target as non-AI and keeps a reference to that object's embedded weak
pointer. If the weak reference and its scene node are present, it calls
`GetSceneNode` a second time and writes the scene position to `targetPosition`.
Otherwise a nonnull native trigger can supply that position. Either successful
path copies the target position into both modified and last-seen positions, then
sets the last-seen flag to one. The vector assignments preserve destination fourth
words and retain the shared by-value temporaries.

A nonnull script argument also causes the original `AI::Log` call with the receiver
scene node's `GetAIObject` result and the modified XYZ coordinates. The original
43-character diagnostic and terminator occupy 44 bytes at `0x802a47ec`; the
assignment constant 1.0f follows at `0x802a4818`. The log implementation remains
external. If neither position source exists, the routine still logs the existing
modified position. A null script argument only nullifies the target: it does not
clear remembered positions or the last-seen flag. No additional scene-node guard
or logging filter is introduced.

`UpdateNonAITargetPosition` requires a script target, a live weak game-object
reference and a scene node. It has no native-trigger fallback. It caches the
address of the embedded weak reference across the first scene lookup, then reads
its subject again for the second lookup. It obtains the target position, copies
it into modified and last-seen positions, sets the last-seen flag, and calls the
still-original `AdjustAimPosition` with the modified-position address, weapon
selector and aim-mode address. Only after that call does it write the global AI
time into `targetUpdateTime` at `+0xf8`. It leaves the separate `lastSeenTime` field
at `+0x30` unchanged. Missing prerequisites leave the stored state untouched.

## Aim selection and blind fire

The original mangled signatures establish the `AIFILTER_WEAPON_AIM_MODE` tag.
Its named members in the shared header describe observed numbers, not recovered
historical enumerators or a complete enumeration. Both mode choosers leave any
nonzero input mode unchanged without consuming random values.

`ChooseSniperAimMode` generates one signed 64-bit percentage value and tests it
against 10 with the existing [MathFun percentage helper](MathFun.md). A successful
test selects mode 1; otherwise it selects mode 2. `ChooseBazookaAimMode` also draws
one value, testing that same value first against 10 and then, if needed, against
80. Both successful branches select mode 6; only failure of both tests selects
mode 8. This repeated mode value and the separate helper calls are preserved.
The numeric modes' gameplay meanings are not inferred from this fragment.

`GetAdjustedAimTargetPosition` first requires at least one target payload pointer.
With an AI pointer it obtains the modified target position. Without an AI pointer
but with a last-seen flag, it obtains the remembered position and adds 1.0f to Z.
If neither path supplies a position, it adds the physics position at `+0x10` to
the second physics axis at `+0x40`, writes XYZ directly, then adds 1.0f to Z.
Those two vector addresses are retained across the output stores, preserving the
original aliasing behavior. It does not use a remembered position when both
target payload pointers are absent. Every branch preserves the output's fourth
word.

`DoBlindFire` chooses an integer threshold of 10, 25 or 50 for selectors 0, 1 or 2;
other selectors retain zero. It calls `MathFunRandomRealSigned(0.0f, 100.0f)` and
returns whether the result is **greater than** the threshold. The helper's signed
random behavior and this comparison direction are retained; these thresholds
must not be presented as literal firing probabilities. No clamping or replacement
random generator is used.

The two adjacent query functions share one complete 24-byte constant pool: 1.0f
at `0x802a4888`, 0.0f at `0x802a488c`, 100.0f at `0x802a4890`, four alignment bytes,
and the eight-byte integer-conversion constant at `0x802a4898`. All bytes are
verified and receive no code credit.

## Verification and remaining work

All accepted fragments use ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. The production compile/link
commands verify every emitted function, symbol binding and allocated section.
No instructions are patched, assembly substituted, functions discarded or
partial comparisons counted. This is a working compiler profile, not proof of
the historical compiler release.

The original-object baseline and complete source build are checked separately;
the baseline earns no source credit. Full-image verification covers the complete
2,860,576-byte analysis DOL, allocated ELF bytes, entry point and BSS extent. See
[Progress.md](Progress.md) for snapshot capture and the public CI boundary.
Runtime/emulator behavior remains untested.

The original `CAILocomotion::UpdateArbitraryPoint` (332 bytes) now has matching
last-seen and visibility helpers, but its candidate still differs in the selector
branches. It remains private research with zero source credit. The candidate for
`UpdatePostionOffsetMin` (192 bytes, original spelling) also remains unaccepted.
The recovered script/game-object bridge now supports script-target setup and
updates. AI-target updates and setters, `CalculateTargetPosition`, weapon-specific
aim adjustment, location-target containers and the full targeting constructor
remain useful follow-ups. Several aim-adjustment routines call through the
`CAIObject` table pointer at `+0x72c`. The original constructor at `0x800d99c8`
stores the named table `0x802e78f0` there; its slot 12 (adjustment at `+0x60`,
function at `+0x64`) is `IsCrouching`. That constructor also initializes a
`CAIPhysics` member at `+0x20`, corroborating the existing object-position and
forward-vector offsets. These are leads for recovering a shared native virtual
interface; the constructor, table and crouching queries earn no new credit here.
Concrete BSGO accessors offer further evidence for derived prefixes, but their
complete storage and ownership still need investigation.
