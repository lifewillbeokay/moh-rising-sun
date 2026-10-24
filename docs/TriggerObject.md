# TriggerObject

`src/triggers/` reconstructs 39 trigger-related functions (5,084 executable bytes)
from the pinned GR8E69 executable. Claude Code assisted the initial 12-function,
548-byte subset; Codex independently reconstructed and verified the 12-function,
1,520-byte property-access extension and the 10-function, 2,300-byte legacy-field
extension below. The transform and lifecycle extension adds five functions /
716 bytes here and a 240-byte shared [FlexProp matrix getter](FlexProp.md#matrix-reconstruction).
Original field tables, property storage and generated research remain outside
the source credit.

## Scoped view

`include/game/TriggerObject.h` gives `TriggerObject` the three observed fields
used by the accepted queries and event loops. The script headers include it
in place of the earlier method-only interface.

- **Flags at `+0`.** Bit 30 selects the property format: set, the object's
  properties are an embedded [`FlexProp`](FlexProp.md) at `+8`; clear, `+8` holds a
  pointer to a legacy `xyzProperty_Struct` ([BPD.md](BPD.md)). Bit 0 is the
  machine-gun "used" flag of `MarkMGAsUsed`/`IsMGUsed`.
- **Script object at `+4`**, a nullable `BSObject *` used by the save/load loops.
- **Properties at `+8`**, modeled as a union of the two forms.

Names of the flag fields and the union are descriptive. The reconstructed
[BPD setup](BPD.md#property-setup-and-cleanup) independently establishes a 12-byte
stride in the original `g_pTriggerObjects` array and stores its legacy pointer at
`+8`. The array remains original storage.

## Accepted fragments

| Manifest | Functions | Code bytes |
| --- | --- | ---: |
| `trigger_mg_flag` | `MarkMGAsUsed`, `IsMGUsed` | 52 |
| `trigger_field_offset` | `GetFieldOffset` (string CRC, then `FlexProp::GetFieldOffset`), `GetData` | 96 |
| `trigger_crc` | `GetCRC` returns `GetEnum(name)` | 32 |
| `trigger_field_position` | `HasField`, `GetPosition` | 160 |
| `trigger_position_z` | `SetPositionZ` | 36 |
| `trigger_class_queries` | `GetLegacyField`, `GetClassName`, `IsAnimatedLight` | 136 |
| `trigger_point_test` | `IsPointInTrigger` | 36 |
| `trigger_typed_fields` | `GetInt`, `GetFloat`, `GetString`, `GetBool` | 436 |
| `trigger_enum_list` | `GetEnum`, `GetList` | 328 |
| `trigger_identity` | `GetNativeBindingType`, `GetChildType`, `GetID`, `GetScript`, `GetChildScript` | 600 |
| `trigger_base_name` | `GetBaseName` | 156 |
| `legacy_field_callbacks` | Five typed legacy-field readers | 1,060 |
| `legacy_field_lookup` | Five `GetLegacyField<T>` instantiations | 1,240 |
| `trigger_save_load` | `Save`, `Load` | 240 |
| `trigger_transform_queries` | `GetForward`, `GetMatrix` | 312 |
| `trigger_rotation_matrix` | `GetRotationMatrix` | 164 |

Behavior read from the bodies:

- `HasField` is true for every legacy object; for a `FlexProp` it tests the field's
  CRC with `IsFieldValid`.
- `GetPosition` copies the legacy record's floats at `+0x10`-`+0x18` or asks the
  `FlexProp`.
- `GetClassName` returns null for legacy objects.
- `IsAnimatedLight` is true only for a legacy object whose record word at `+0x0c` is
  13.
- `GetLegacyField` and `SetPositionZ` pass straight through without checking the
  format bit; `IsPointInTrigger` always uses the `FlexProp` form.

The larger creation helpers remain unmatched. The typed accessors and their legacy-field templates
and callbacks now have source; the original descriptor tables remain external.

## Typed properties and identity queries

These four fragments reconstruct **12 functions / 1,520 code bytes**, with **52
read-only bytes** checked separately. The functions lie in the `propdat.cpp`
compiler-marker interval; this identifies an inferred file group, not a complete
original translation-unit boundary. The private callback dependencies are pinned
to file record 1204 rather than resolved solely by an unscoped name.

| Function | GR8E69 address | Code bytes |
| --- | --- | ---: |
| `TriggerObject::GetInt` | `0x80122844` | 108 |
| `TriggerObject::GetFloat` | `0x801228b0` | 112 |
| `TriggerObject::GetString` | `0x80122920` | 108 |
| `TriggerObject::GetBool` | `0x8012298c` | 108 |
| `GetBaseName` | `0x80122a18` | 156 |
| `TriggerObject::GetEnum` | `0x80122ab4` | 220 |
| `TriggerObject::GetList` | `0x80122b90` | 108 |
| `TriggerObject::GetNativeBindingType` | `0x80122e9c` | 252 |
| `TriggerObject::GetChildType` | `0x80122f98` | 64 |
| `TriggerObject::GetID` | `0x80122fd8` | 68 |
| `TriggerObject::GetScript` | `0x8012301c` | 112 |
| `TriggerObject::GetChildScript` | `0x8012308c` | 104 |

**Typed reads.** `GetInt`, `GetFloat`, `GetString`, `GetBool` and `GetList`
initialize their result to zero, false or null. For FlexProp data they call the
corresponding already reconstructed accessor. Otherwise they call the now-reconstructed
`GetLegacyField<T>` template only when the legacy record's word at `+8` is 4,
passing the matching `propdat.cpp` callback and the result by reference. Other
legacy records retain the initial default. This selector is separate from the
legacy object's type word at `+0x0c`.

`LegacyField.h` declares the symbol-supported template/callback signatures and a
scoped `ANY_PROP` union containing the observed four-byte record pointer. The
aggregate name comes from the symbols; its `xyz` member name is descriptive.
The caller passes a copy of that pointer in the target's aggregate-by-value ABI.
Other original union members remain unknown. The legacy-field extension below
now establishes the `LegacyFieldInfo` descriptor view; its original tables remain
external storage. ProDG's
four-byte `bool` result is passed and returned as observed, without adding a
second boolean normalization step.

**Enum/CRC reads.** For FlexProp field type 2, `GetEnum` hashes the complete string
with `GetStringCRC`. If the first type check fails, it calls `GetFieldType` again:
type 8 copies a basename into a 256-byte local buffer and hashes that; other types
use `FlexProp::GetEnum`. Legacy format follows the same integer callback path as
`GetInt`, including the initial zero and `+8 == 4` gate. The repeated field-type
call and branch order are retained. The earlier 32-byte `TriggerObject::GetCRC`
wrapper continues to call this now-reconstructed method and keeps its prior credit.

`GetBaseName` starts after whichever occurs later: the last backslash or the last
forward slash. It copies that suffix, then terminates it at its **first dot**.
For example, `dir/name.part.ext` becomes `name`; a leading dot makes the result
empty. It performs no case folding. Its caller retains the original nonnull-string
and sufficient-buffer preconditions; no length guard or alternate path semantics
have been added.

**Identity and scripts.** All legacy accesses reuse the existing BPD views without
changing their layout:

| Query | FlexProp behavior | Legacy behavior |
| --- | --- | --- |
| Native binding | Read `eRuntimeBinding` as an enum if present, otherwise return 0. | Map type word `+0x0c`: 2→1, 8→2, 3→3, 4→4, 5→5, 13→6; all others return 0. |
| Child type | Call `TriggerObject::GetEnum("ChildType")`, preserving its string/filename handling. | Read the integer at `+0x48`. |
| ID | Read `FlexProp::GetInt("ID")`. | Sign-extend the halfword at `+0x38`. |
| Script | If `TriggerClass` has field type 2, return that string; otherwise return the FlexProp class name. | Return the string pointer at `+0x40`. |
| Child script | If `TriggerClass` has field type 2, return the FlexProp class name; otherwise return null. | Return the string pointer at `+0x4c`. |

The native-binding switch keeps explicit cases for 0, 1, 6 and 7 and the original
result-block order. The names/meanings of the numeric binding constants are not
inferred from this mapping alone. `TriggerObject.h` adds only nonvirtual method
declarations; object storage and existing virtual interfaces are unchanged.

**Generated constants.** The typed getters emit a four-byte float zero at
`0x802a6a68`. The identity fragment emits `TriggerClass` at `0x802a6a7c` (16 bytes),
`eRuntimeBinding` at `0x802a67d8` (16 bytes), `ChildType` at `0x802a6880` (12 bytes)
and `ID` at `0x802a6574` (4 bytes). Shared literals use descriptive read-only
subsections, following the existing literal-placement mechanism. Character-list
initializers prevent those arrays from being folded into a different string pool.
All terminators and emitted alignment bytes are compared. These exact locations,
including this distinct copy of `ID`, must not be merged with equally spelled
strings at other addresses or claimed again by a later fragment.

The direct-dependency scan at this checkpoint finds **58 distinct caller ranges
outside this batch**, of which **47 remain unfinished**. These are static direct
references, not a runtime-frequency measurement or additional source credit. The
accepted script attachments and enemy-count wrapper now reach reconstructed
`GetInt` and native-binding accessors. The legacy-field extension below supplies
the shared lookup and conversion bodies used by those typed reads.

## Legacy-field lookup and typed readers

This extension independently reconstructs **10 functions / 2,300 code bytes**
from GR8E69, plus **36 read-only bytes** checked separately. No implementation
from another game is imported. The callback names and `LegacyFieldInfo` type name
come from the original symbols. Descriptor member names are descriptive.

| Function | GR8E69 address | Code bytes |
| --- | --- | ---: |
| `GetLegacyFieldBool` | `0x801223c0` | 196 |
| `GetLegacyFieldInt` | `0x80122484` | 224 |
| `GetLegacyFieldFloat` | `0x80122564` | 280 |
| `GetLegacyFieldString` | `0x8012267c` | 172 |
| `GetLegacyFieldList` | `0x80122728` | 188 |
| `GetLegacyField<int>` | `0x80286ac4` | 248 |
| `GetLegacyField<float>` | `0x80286bbc` | 248 |
| `GetLegacyField<const char *>` | `0x80286cb4` | 248 |
| `GetLegacyField<bool>` | `0x80286dac` | 248 |
| `GetLegacyField<FlexPropList *>` | `0x80286ea4` | 248 |

**Descriptor layout.** Each callback advances by sixteen bytes. Its reads establish
a name pointer at `+0`, a signed type selector at `+4`, a byte offset at `+8` and
a four-byte mask at `+12`. `LegacyFieldInfo.h` models that observed stride;
these descriptors are separate from the variable-length BPD records they index.
No allocation size for a property record or complete original enum is inferred.

The readers search in order with `strcasecmp`. The first matching name stops the
search, including when its type is incompatible. A missing name, zero count or
negative count returns false and leaves the output untouched. A recognized name
returns true; an incompatible type first writes zero, false or null and prints
`Invalid Field type.`. This is a successful **name lookup**, not a successful type
conversion, and must not trigger a fallback search.

| Reader | Supported descriptor types and conversion |
| --- | --- |
| Boolean | Type 0: sign-extend the byte at the descriptor offset, apply the mask, then compare with zero. The byte load precedes the type check, including on the error path. |
| Integer | Type 1: signed byte; type 2: signed halfword; types 3 and 4: four-byte integer. |
| Float | Type 5: float load; type 6: signed halfword converted to float and multiplied by `0.0625f`. No physical unit is inferred. |
| String | Type 7: load a string pointer without copying it. |
| List | Type 8: load a `FlexPropList *` without examining the list contents. |

The explicit rejected cases in the switches preserve the observed branch trees.
The code adds no bounds checks, pointer validation or fallback on a type error.
The existing caller's `+8 == 4` gate remains in `TriggerObject`; the generic lookup
itself does not repeat that check.

**Lookup order.** All five template instantiations share one source definition.
The legacy record's signed type word at `+0x0c` selects an initial table:

| Record type | First table | Entries | Original storage |
| --- | --- | ---: | --- |
| 3 | `s_enemyFields` | 19 | `0x802c4990`, 304 bytes |
| 2 | `s_mechanicFields` | 3 | `0x802c4960`, 48 bytes |
| 4 | `s_powerupFields` | 1 | `0x802c4ac0`, 16 bytes |
| All others | `s_coreFields` | 25 | `0x802c47d0`, 400 bytes |

For types 2, 3 and 4, a false callback result searches the core table next; a true
result stops immediately. Other record types search the core table once. The
lookup ignores the final core result and does not initialize the output itself.
The explicit cases for 1 and 5–8 preserve the original dispatch tree without
assigning names to those values.

The four table symbols and sizes independently corroborate the descriptor stride
and counts. They remain original data, with private references scoped to
`propdat.cpp` file record 1204. The callbacks retain their original local binding.
The template symbols are global; their manifest's file scope resolves the private
tables and does not establish an original template translation-unit boundary.

**Constants and scope.** The callback fragment owns the complete 20-byte diagnostic
at `0x802a6a44`, placed in a named read-only subsection, and the 16-byte numeric
pool at `0x802a6a58`: the compiler's signed-integer conversion bias, `0.0625f`,
and zero. All emitted bytes and alignment are compared. The earlier typed-getter
zero at `0x802a6a68` retains its separate ownership. No table data or caller code
is counted again, and no functions are discarded.

A separate host behavior check covers signed byte/halfword loads, sign extension
before a boolean mask, fractional conversion, case-insensitive lookup, missing
names and nonpositive counts, incompatible types, duplicate-name precedence,
and specialized-table success/fallback for record types -1 through 15. It uses
host-native test records and disables only target section-placement annotations;
it does not validate the target ABI or run the game. The production compiler and
complete-image checks validate target layouts and generated code separately.

## Transform queries and save/load events

Five further functions reconstruct **716 executable bytes**, plus **16 read-only
bytes**. The shared 240-byte `FlexProp::GetMatrix` is accepted separately in
[FlexProp.md](FlexProp.md#matrix-reconstruction); its credit is counted once.
These bodies were independently reconstructed from GR8E69 with Codex assistance,
using the existing matrix, vector, quaternion and BPD views.

| Function | GR8E69 address | Code bytes |
| --- | --- | ---: |
| `TriggerObject::Save` | `0x80121210` | 120 |
| `TriggerObject::Load` | `0x80121288` | 120 |
| `TriggerObject::GetForward` | `0x80122c9c` | 148 |
| `TriggerObject::GetMatrix` | `0x80122d30` | 164 |
| `TriggerObject::GetRotationMatrix` | `0x80122df8` | 164 |

**Transforms.** `GetMatrix` delegates to `FlexProp::GetMatrix` when the format bit
is set. For legacy records it copies the floats at `+0x1c`, `+0x20`, `+0x24`,
`+0x28` into a quaternion in the established memory order `w, x, y, z`. It also
snapshots the position at `+0x10`–`+0x18` before calling quaternion-to-matrix
conversion, then calls `SetPos`. No quaternion normalization or format gate
beyond the format bit is added.

`GetRotationMatrix` uses that same quaternion conversion for legacy records.
For FlexProp it obtains the complete matrix, then clears its position through
`SetPos` and a temporary zero vector. It does not change basis values or scale,
and it is not an orthonormalization operation. The three query bodies reuse the
previously verified 64-byte matrix, 16-byte quaternion and 16-byte vector views;
no constructors or storage fields are added to those shared classes.

`GetForward` is different: it **always** reads the FlexProp representation at
`+8`, without testing the format bit. It initializes the matrix class only when
`s_ClassInit` is zero, fills a local matrix and assigns its front row to the output
vector. The output's fourth word is preserved by the existing vector assignment.
This behavior must not be replaced by a call to `TriggerObject::GetMatrix`, which
would introduce legacy-format dispatch absent from the original.

The private inline helpers have descriptive names. `InitLocalMatrix` models only
the observed class-initialization check and preserves the local matrix address;
it does not initialize matrix contents or establish the full historical default
constructor. `SetLegacyRotation` assigns four observed quaternion components and
is not a new recovered method. All emitted temporaries and constants are checked.

The contiguous forward/full-matrix fragment owns two float-one constants at
`0x802a6a6c` and `0x802a6a70`. The rotation fragment owns float one and zero at
`0x802a6a74` and `0x802a6a78`. All sixteen bytes are compared separately and earn
no executable credit. `SetPositionZ`, which lies between the full-matrix and
rotation-matrix functions, retains its existing fragment and ownership.

**Save/load notifications.** Both functions walk `g_pTriggerObjects` using the
already established twelve-byte stride and the **unsigned** `g_pNumTriggers` at
`0x80326d64`. Each nonnull script-object pointer at `+4` receives
`BSObjectTriggerEvent(object, 422, null, null, true)`. **Both entry points use
422**, despite their different names. The event's original symbolic name and
purpose are not established by these bodies; neither a different load event nor
serialization logic is invented.

The loop reloads the count after each iteration, including after an event call,
and skips null script-object slots. The count and 24,000-byte trigger array remain
original storage. Static membership is supported by the calls in
`CGameSettings::StoreSaveData(bool)` at `0x80170ad8` and
`CGameSettings::RestoreSerializedData()` at `0x80170668`: neither supplies a
trigger receiver. These call sites and the bodies establish a global traversal,
not a per-object save/load method.

A separate host check combines these six new bodies with the accepted quaternion
conversion, matrix setters and identity initializer. It checks distinct FlexProp
rows, legacy identity and non-unit quaternions, zeroed rotation-only position,
forward output and its preserved fourth word, one-time class initialization,
identical save/load event arguments, null slots and count shrink/growth during a
callback. Host-native fixtures and an opaque event stub exercise behavior; they
do not model target pointer sizes or the `BSObject` allocation. Target ABI and
complete-image verification remain separate, and no runtime/emulator test was run.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
fragment. The complete rebuilt analysis image is identical, including every script
unit that now sees the scoped view. A separate host build of `GetBaseName` passes
six cases covering mixed separators, first-dot truncation, case preservation and
empty input. That check does not execute the game; runtime/emulator behavior
has not been tested.
