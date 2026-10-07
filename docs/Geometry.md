# Quaternion, line and integer-angle reconstruction

Twelve functions compile to **2,120 matching executable bytes** in ten fragments,
with **64 generated read-only bytes**. They were reconstructed from the pinned
GR8E69 executable with Codex assistance; no implementation was imported from
another decompilation. The working compiler is ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. This profile does not identify
the original compiler release.

This work builds on kyleckroeger's [CVector3 contribution in PR #13](https://github.com/lifewillbeokay/moh-rising-sun/pull/13).
Its recovered constructors and assignment behavior made the matrix, line and
camera reconstructions possible without inventing vector storage.

## Accepted ranges

| Manifest | Functions | Original text start | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `quaternion_normalize` | `Set`, `Normalize` | `0x8007ccc8` | 156 | 4 bytes at `0x8029b110` |
| `quaternion_get_matrix` | `GetMatrix` | `0x8007cd64` | 200 | 8 bytes at `0x8029b11c` |
| `quaternion_from_euler` | `SetFromEuler` | `0x8007cff8` | 244 | 4 bytes at `0x8029b13c` |
| `quaternion_slerp` | `Slerp` | `0x8007d0ec` | 372 | 12 bytes at `0x8029b140` |
| `line3_length_squared` | `GetLengthSquared` | `0x8027c878` | 60 | None |
| `line3_degenerate` | `IsDegenerate` | `0x8007cbf8` | 80 | 4 bytes at `0x8029b0b8` |
| `line3_set` | `Set` | `0x80281690` | 176 | 4 bytes at `0x8029e25c` |
| `line3_projection` | `GetProjection` | `0x8007c510` | 344 | 8 bytes at `0x8029b098` |
| `line2_closest_point` | `GetClosestPointSquared` | `0x8007c358` | 360 | 4 bytes at `0x8029b08c` |
| `math_sin_cos` | `MathLLAngleInit`, `MathSinCos` | `0x8007cc48` | 128 | 16 bytes at `0x8029b0e0` |

The manifests record every original symbol, function boundary, binding, generated
section and external reference. Separate source files are reconstruction
fragments; they do not establish original translation-unit boundaries. All
allocated output is compared, including native constant-pool alignment bytes.
There are no instruction patches, assembly implementations, discarded functions
or partial-match credit.

## Quaternion storage and behavior

[CQuaternion.h](../include/game/CQuaternion.h) represents four floats in memory
order `w, x, y, z`. `Set(x, y, z, w)` stores its fourth float argument at offset
zero and its first three at offsets 4, 8 and 12. `Normalize`, `GetMatrix` and
`SetFromEuler` independently use this ordering. The original, unreconstructed
`CHierObject::SetRotation` and `GetRotation` at `0x800d073c` and `0x800d0764`
copy four words between this quaternion view and the object's storage at
`+0x1b0`. These independent copies support the 16-byte value representation.
No historical constructor, inheritance, access control or member names are
claimed. Original method names, argument encodings and constness come from the
ELF; return types and storage names are inferred from instructions and ABI use.

`Normalize` computes the reciprocal square root of the sum of the four squared
components, then scales all four. It has no zero-length fallback.
`GetMatrix` expands doubled component products into the existing 64-byte matrix,
writes zero translation and homogeneous basis entries, and writes one at offset
60. It does not normalize its input. The expression grouping, operand order and
stores match the original.

`SetFromEuler` halves all three angles, evaluates cosine and sine in the observed
order, combines four pair products, and calls the original `Set` entry point.
The recovered formulas determine the mapping; parameter names do not assert a
particular external yaw/pitch/roll convention.

`Slerp(a, b, t)` snapshots the four components of `a`, computes their dot product
with `b`, and copies either `a` or its negation into the destination. It uses
spherical weights when `1 - dot` is greater than the observed `0.0005f` threshold
under the original comparison, and linear weights otherwise. The final formula
weights `b` by the weight derived from `1 - t`, and the selected sign of `a` by
the weight derived from `t`. Consequently the endpoint direction must not be
silently reversed to fit a conventional interpolation API. There is no extra
clamping or final normalization. Unordered comparisons retain their original
behavior; the code is not a general-purpose numerical robustness guarantee.

`SetFromMatrix`, quaternion multiplication and quaternion vector rotation remain
research candidates without source credit. The accepted declaration contains only
the methods used by accepted source.

## Three-dimensional line view

[CLine3.h](../include/game/CLine3.h) describes the observed 64-byte prefix:

| Offset | Descriptive field | Evidence |
| --- | --- | --- |
| `0x00`, `0x10` | `start`, `end` | `Set` uses the recovered vector assignment for both endpoints |
| `0x20` | `direction` | `Set` writes end minus start; projection and length routines read it |
| `0x30`, `0x34` | `length`, `lengthSquared` | Original plane intersection takes the square root of `GetLengthSquared` and caches it at `0x30`; accepted helpers read/write `0x34` |
| `0x38`, `0x3c` | `lengthValid`, `lengthSquaredValid` | `Set` clears both; original plane intersection and accepted length helpers independently update their respective four-byte flag |

The three vectors use the established 16-byte [CVector3](Matrix.md#cvector3-layout)
representation. This view does not establish the complete class allocation or
historical declaration. These functions receive existing line objects; they do
not allocate or construct them. Mutable cache fields reproduce writes performed
by const methods. Original flag typedefs and signedness remain unknown.

`GetLengthSquared` and the descriptive inline `CachedLengthSquared` helper compute
and cache the squared length only when its flag is zero. The explicitly grouped
sum reproduces the original floating operations. `IsDegenerate` tests that value
against `1.0e-12f`. It does not compare each direction component to zero.

`Set` copies both endpoints, computes direction component by component, and clears
both validity flags. It preserves the vectors' fourth words. `GetProjection`
projects onto the unbounded line and returns its parameter through the float
reference; the parameter is not clamped to a segment. For a degenerate line it
copies the start point and sets the parameter to `0.5f`. Its scaled-direction
helper takes the parameter by value, preserving the observed reuse across output
stores. No non-aliasing assumption or independent output buffer is introduced.

## Two-dimensional closest point

[CLine2.h](../include/game/CLine2.h) declares two eight-byte `CVector2` endpoints
at offsets zero and eight. The original closest-point routine copies each vector
as two words and uses eight-byte stack temporaries. The independently inspected
`CLine2::Intersect` at `0x8007c21c` uses the same endpoint offsets and copy shape.
The inline vector operations and field names are descriptive; their historical
names are not recovered. The line declaration is an endpoint prefix view, not a
claim about a complete class allocation.

`GetClosestPointSquared` computes the direction and the displacement from the
start, then selects the end, the start, or the interior projected point using the
original comparisons. It optionally copies that point to the supplied output,
subtracts the input point from the local result and returns squared distance.
The output copy occurs before the final input reads, preserving possible aliasing
with the input. The source does not add a division guard or change the branch
behavior for degenerate or unordered values.

## Integer-angle helper

`MathLLAngleInit` is an observed no-op. `MathSinCos` converts its signed integer
input to float, multiplies by `2*pi / 2^24` rounded to float (`0x34c90fdb`), calls
`sinf`, stores that result, then calls and stores `cosf`. It performs no masking
of the integer input. This complements the inverse scale used by the accepted
[matrix rotation builders](Matrix.md).

The complete generated pool at `0x8029b0e0` contains the native integer-to-float
conversion constant, the angular scale, and four bytes of alignment padding.
The nearby original constants outside that generated interval stay original
context and receive no reconstructed-data credit.

## Verification and limits

Each accepted fragment passes strict object validation, full generated-section
comparison and original symbol-boundary validation. The complete source build
then compares the entire 2,860,576-byte derived analysis image, including header
and padding, and all original allocated section bytes and BSS extents. The
required DOL SHA-1 is `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`.
The original-object baseline is verified separately and earns no source credit.
Runtime and emulator behavior remain untested. See [Progress.md](Progress.md)
for snapshot capture and the distinction between local reconstruction and public
snapshot CI.
