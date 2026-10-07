# TriggerObject

`src/triggers/` reconstructs 12 `TriggerObject` functions (548 executable bytes) from
the pinned GR8E69 executable. Claude Code assisted the analysis and verification.

## Scoped view

`include/game/TriggerObject.h` gives `TriggerObject` the two parts every accepted
accessor uses; the rest of the object is not declared. The script headers include it
in place of the earlier method-only interface.

- **Flags at `+0`.** Bit 30 selects the property format: set, the object's
  properties are an embedded [`FlexProp`](FlexProp.md) at `+8`; clear, `+8` holds a
  pointer to a legacy `xyzProperty_Struct` ([BPD.md](BPD.md)). Bit 0 is the
  machine-gun "used" flag of `MarkMGAsUsed`/`IsMGUsed`.
- **Properties at `+8`**, modeled as a union of the two forms.

Names of the flag fields and the union are descriptive.

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

`GetInt`, `GetFloat`, `GetString`, `GetBool`, `GetList` and `GetEnum` fall back to a
templated legacy-field reader and remain unmatched, as do `GetForward`, `GetMatrix`
and the larger creation helpers.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
fragment. The complete rebuilt analysis image is identical, including every script
unit that now sees the scoped view. Runtime behavior has not been tested.
