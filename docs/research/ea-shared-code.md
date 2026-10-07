# Related EA GameCube code references

This comparison selects research targets for Rising Sun GR8E69; it does not
establish byte identity, common class layouts or permission to import unrelated
source. Codex assisted the comparison and the resulting independent reconstructions.
Thanks to the NFL Street 2 maintainers for making the reference inventory available,
and to the NFS reconstruction projects for their shared-engine research.

## Pinned references

- [GoldenEye: Rogue Agent symbol inventory](https://github.com/mitsevox/nflstreet2/blob/98fea259ef2817871897ce68c47b25188437d90e/references/GoldenEye%20-%20Rogue%20Agent/DOL-GOYE-USA/symbols/symbols.tsv).
- [Medal of Honor: European Assault symbol inventory](https://github.com/mitsevox/nflstreet2/blob/98fea259ef2817871897ce68c47b25188437d90e/references/Medal%20of%20Honor%20-%20European%20Assault/DOL-GONE-USA/symbols/symbols.tsv).
- [NFS Underground GameCube symbols](https://github.com/dbalatoni13/nfsug/blob/7e718a6997a738a4ce2004e4186f16160cb69f3e/config/GNDP8P/symbols.txt), from its September 2003 prototype target.
- [NFS Most Wanted EAGL reference](../ReuseMap.md): already supports 66,524 accepted
  animation bytes and 4,528 loading bytes here, with attribution and license records
  in the existing subsystem documents.

These symbol inventories were read as reference metadata. Original executables,
bulk symbol dumps and other games' source implementations were not added here.

## Observed overlap

Comparing named function symbols whose names contain `CMatrix`, `CQuaternion`,
`CVector3`, `MathFun`, `FlexProp` or `DWI_` found 269 common names with Rogue Agent
(212 also have equal recorded sizes), and 142 with European Assault (71 equal
sizes). Names containing vector/matrix arguments include callers, not just class
members. Counts are distinct symbol names, not unique executable ranges or
predictions of reusable bytes.

| Rising Sun symbol/family | Rising Sun bytes | Rogue Agent | European Assault |
| --- | ---: | ---: | ---: |
| `CQuaternion::SetFromEuler` | 244 | 244 | 244 |
| `CQuaternion::GetMatrix` | 200 | 200 | 200 |
| `CQuaternion::Normalize` | 136 | 136 | 136 |
| `MathFunRandomReal` | 108 | 108 | 108 |
| `MathFunNormalizeAngleNegativePiToPi` | 156 | 156 | 156 |
| `MathFunClamp<float>` / `<int>` | 40 / 36 | 40 / 36 | 40 / 36 |
| `FlexProp::GetString(int)` | 120 | 120 | 88 |
| `FlexProp::GetMatrix` | 240 | 240 | 136 |
| `DWI_alloc` | 212 | 332 | 332 |
| `DWI_free` | 64 | 256 | 144 |

The differing allocator and property sizes are a reason to check each target
independently. Equal names/sizes also do not establish instruction equality or
historical field declarations. NFS Underground separately exposes the same
100-byte `FEHashUpper` plus EAGL animation and symbol-pool names.

## Resulting work

The first pass reconstructed ten Rising Sun functions / 600 code bytes in eight complete
fragments: [front-end hashing](../FEHash.md), the two [clamp instances](../MathFun.md),
and [FlexProp string access, string-table creation/initialization and database
queries](../FlexProp.md). Source was recovered from our pinned executable, using
existing attributed STLport headers for the database's vector methods. The other
games' addresses and layouts are not used as Rising Sun definitions.

A follow-up reconstructs the two FlexProp database loaders and their two cleanup
methods: a further **3,396 code bytes**, with complete generated objects and no
new data ownership. Their record conversions, class-parent resolution and property
pointer relocation are documented in [FlexProp](../FlexProp.md#database-loading-and-cleanup).
The different related-game loader sizes did not prevent useful target selection;
all accepted bodies are verified against Rising Sun independently.

Useful remaining candidates are the FlexProp matrix getter, string lookup,
quaternion conversion/multiplication/rotation, and the two remaining MathFun
bodies. Investigated variants that still differ in instructions or complete data
pools remain private research with zero credit. Continue the existing NFS-based
EAGL animation work separately; shared EA branding alone does not establish
compatible game code.
