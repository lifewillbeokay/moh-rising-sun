# AI targeting reconstruction

Nine fragments in `src/ai/` reconstruct **19 functions and 1,068 executable
bytes**, plus 16 bytes of required float constants. The original `AITargeting.cpp`
file record (index 1071) supports their grouping; these fragments are not complete
historical translation units. Codex assisted the reconstruction from the pinned
GameCube GR8E69 executable. No external source was imported.

## Accepted functions

| Unit | Functions / executable bytes | Original start | Constant pool |
| --- | --- | --- | --- |
| `ai_target_identity` | `Matches`, `GetScriptObject`, `SetAsNonAITarget`, `Nullify`, `SetAsAITarget`, both constructors: 7 / 236 | `0x800eecc8` | None |
| `ai_targeting_last_seen` | `GetLastSeenTargetPosition`: 1 / 64 | `0x800ece88` | 4 bytes at `0x802a47a4` |
| `ai_targeting_modified` | `GetModifiedTargetPosition`: 1 / 64 | `0x800ecec8` | 4 bytes at `0x802a47a8` |
| `ai_targeting_seen_state` | `SetLastSeenTargetPosition`, `IsLastSeenTargetPositionSet`, `CouldVisionSeeTarget`: 3 / 100 | `0x800ecf08` | 4 bytes at `0x802a47ac` |
| `ai_targeting_update` | `Update`: 1 / 128 | `0x800ecf6c` | None |
| `ai_targeting_reaction` | `UpdateReactionTime`: 1 / 120 | `0x800ecfec` | 4 bytes at `0x802a47b0` |
| `ai_targeting_queries` | `GetTargetScript`, `IsNonAITarget`, `HasValidTarget`: 3 / 180 | `0x800edb18` | None |
| `ai_targeting_distance` | `GetDistanceToTargetSquaredXYZReal`: 1 / 116 | `0x800ed948` | None |
| `ai_targeting_guess` | `GetTargetGuessPosition`: 1 / 60 | `0x800ee468` | None |

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

The `CAIObject` declaration is only a prefix through `+0x3f`. It exposes the
script-object pointer at `+0x0c` and a position vector at `+0x30`, as used by
`GetScriptObject` and the distance query. The original `CAITarget::GetPosition`
independently uses the same position offset. No complete allocation, inheritance
relationship, virtual interface or meaning for the intervening bytes is claimed.

`CAITargeting` is a prefix through `+0x7f`:

| Offset | Observed storage/use |
| --- | --- |
| `+0x04`, `+0x08`, `+0x0c` | Physics, object-parameter and scene-node pointers |
| `+0x10` | Embedded 12-byte target value |
| `+0x20` | Last-seen position vector |
| `+0x30` | Time value used by `UpdateReactionTime` |
| `+0x34`, `+0x38` | Four-byte last-seen and vision-visible values |
| `+0x70` | Modified target-position vector |

The original constructor at `0x800ecde0` corroborates the first three pointer
stores, calls the target constructor at `+0x10`, initializes the flag/time fields,
and initializes vector fourth words at `+0x2c` and `+0x7c`. That constructor and its
virtual table remain original. The initial dispatch word, `+0x1c`, and the bytes
between the visibility value and modified position stay opaque. The complete
class extends beyond this view; do not allocate an instance from this prefix.

## Position, validity and update behavior

The position getters use the shared [CVector3 assignment](Matrix.md#cvector3-layout),
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
the script pointer and calls the still-original `UpdateNonAITargetPosition` if
present. It does not dispatch from the selector word or clear positions when both
pointers are absent.

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
`CAITarget::GetPosition`/`GetForward`, larger target-position updates, target
selection, location-target containers and the full targeting constructor and
virtual interface are useful follow-ups. Scene-backed script targets require
additional evidence for the bridge stored in `BSObject` at `+0x20`; the accepted
work does not invent that interface.
