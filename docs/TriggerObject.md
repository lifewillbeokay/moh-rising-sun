# TriggerObject

`src/triggers/` reconstructs 24 trigger-related functions (2,068 executable bytes)
from the pinned GR8E69 executable. Claude Code assisted the initial 12-function,
548-byte subset; Codex independently reconstructed and verified the 12-function,
1,520-byte property-access extension below. Original callback implementations,
property storage and generated research remain outside the source credit.

## Scoped view

`include/game/TriggerObject.h` gives `TriggerObject` the two parts every accepted
accessor uses; the rest of the object is not declared. The script headers include it
in place of the earlier method-only interface.

- **Flags at `+0`.** Bit 30 selects the property format: set, the object's
  properties are an embedded [`FlexProp`](FlexProp.md) at `+8`; clear, `+8` holds a
  pointer to a legacy `xyzProperty_Struct` ([BPD.md](BPD.md)). Bit 0 is the
  machine-gun "used" flag of `MarkMGAsUsed`/`IsMGUsed`.
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

`GetForward`, `GetMatrix`, `GetRotationMatrix`, save/load and the larger creation
helpers remain unmatched. The typed accessors now have source; their legacy-field
templates and callbacks remain original code.

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
corresponding already reconstructed accessor. Otherwise they call the original
`GetLegacyField<T>` template only when the legacy record's word at `+8` is 4,
passing the matching `propdat.cpp` callback and the result by reference. Other
legacy records retain the initial default. This selector is separate from the
legacy object's type word at `+0x0c`.

`LegacyField.h` declares the symbol-supported template/callback signatures and a
scoped `ANY_PROP` union containing the observed four-byte record pointer. The
aggregate name comes from the symbols; its `xyz` member name is descriptive.
The caller passes a copy of that pointer in the target's aggregate-by-value ABI.
Other original union members and `LegacyFieldInfo` storage remain unknown.
Neither helper bodies nor their original storage receive source credit. ProDG's
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
`GetInt` and native-binding accessors; legacy conversion internals remain future
work.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
fragment. The complete rebuilt analysis image is identical, including every script
unit that now sees the scoped view. A separate host build of `GetBaseName` passes
six cases covering mixed separators, first-dot truncation, case preservation and
empty input. That check does not execute the game; runtime/emulator behavior
has not been tested.
