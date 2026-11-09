# Frontline-derived collision batch

This batch maintains 16 complete function bodies covering 32,024 target code
bytes. Three `separate` overloads are exact (360 bytes). Thirteen other functions
cover 31,664 target bytes as registered, unlinked partial source. Similarity is
not behavioral equivalence or exact-source credit. All are reconstructed game
code; none are classified as restored library code.

AI assisted the adaptation, target analysis and tests. The source uses the
existing ProDG 3.8.1 game profile (`-O2 -G0 -fno-exceptions
-fno-implicit-templates`); comparison manifests inherit `cdbgeom_box_plane.json`.
No compiler setting is chosen per function. `DECOMP_COMPARE` enables partial
bodies, and the verified image retains their original code.

## Upstream attribution

Adapted from kyleckroeger's [Medal of Honor: Frontline decompilation](https://github.com/kyleckroeger/moh-frontline-decomp),
pinned at **`9ce7833e4dab6dd788bb86b43216159d6b628b35`**. Its root CC0 1.0
license is byte-identical to this project's [LICENSE](../../LICENSE), SHA-256
`a2010f343487d3f7618affe54f789f5487602331c0a8d03f49e9a7c547cf0499`.
The draft sources are explicitly unverified research upstream. Their checker
status was a lead, not evidence of Rising Sun correctness. Unrelated weak-method
stubs preceding the capsule draft were not imported.

| Reference at the pinned revision | SHA-256 |
| --- | --- |
| [src/game/cdbgeom_volbox_ctor.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/cdbgeom_volbox_ctor.cpp) | `a2c1110e81d58e0e34d4865174154d78805f31484a1e30bd3e57dad3323b8f81` |
| [src/game/cdbgeom_volbox.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/cdbgeom_volbox.cpp) | `5bbdd2467ca0ffca2d97d50d9e66099e09a461dca322be12fdce4ca302c07317` |
| [src/game/cdbgeom_volbox_line.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/cdbgeom_volbox_line.cpp) | `5eeb481974a1ba356f4841e94126e273623e0ac735cd35c9f372518305a36ac1` |
| [src/game/cdbgeom_sphere.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/cdbgeom_sphere.cpp) | `44dff0b3101ec0521ea652e0acd4cff88524dfbab5d0a35fd71ece530527fa2f` |
| [src/game/cdbgeom_triangle_line.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/cdbgeom_triangle_line.cpp) | `ec1acb425706a680954dddbd8fb665c3b4a4ee2468ff0e2c682ba66aa84550df` |
| [src/game/box_ivolume.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/box_ivolume.cpp) | `cd2b3495081ad7e913e5f7ec13ff27862782f509f50064bdb8818885c41d75c5` |
| [src/game/box_visibility.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/box_visibility.cpp) | `048548777b33c37344d6c7fac5da7f5cab37eef6f22b65d0095057af42ba7b7b` |
| [research/drafts/TestCollision.8006e0b0.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/research/drafts/TestCollision.8006e0b0.cpp) | `e05e4c1209189f411c491059f8bd6e1746dc1d44c265972c7079100a8b1bf8c5` |
| [research/drafts/TestCollision.800c15ec.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/research/drafts/TestCollision.800c15ec.cpp) | `ba92da09f4ff3d94ffb217225368594ac519b8b2fdca74b726c4216b51744e6b` |
| [research/drafts/TestCollision.8006a71c.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/research/drafts/TestCollision.8006a71c.cpp) | `7bf71a7ba4fe31865087474edde39fa9b4694568dcdf13a5a190130bd7fac335` |

The accepted reference files guide the constructor, interval/slab helpers,
box/sphere and triangle tests, oriented-box collision and visibility. The three
`TestCollision` drafts guide CDB-box/triangle, box/plane and triangle/capsule
respectively. Local source/header filenames and inline helper names are project
choices; upstream's CodeWarrior-specific class declarations were not imported.

## Rising Sun evidence and changes

Every function's complete GR8E69 range, native output, dependencies and constant
pool are inventoried. Constants remain at their own target function's pool,
including the line test's tolerance/one/zero sequence at `0x8029af54`. No pool
bytes are clipped, no other unit's data is borrowed and no native instructions
are patched. Local `separate`, `boxline_testaxis` and `BoxBoxTest` names/types
come from symbols; callers explicitly resolve them in their original file scope.
The separating-axis enum names are recovered, but the enumerator names are not.

- Rising Sun classifies negative separation as contact (2) and other separation
  as apart (1). Frontline's CONTACT_EPSILON and third result code do not carry
  over. The interval helpers preserve the target's unordered-comparison behavior;
  helper tests include NaN and infinities.
- `CCollision::result` is at +0x18, with depth at +0x0c and the line-test bit at
  +0x10. Contacts use the existing `SClsnContact` view. The original triangle/box
  test does not initialize contact position; the tests do not invent or compare
  values for that unwritten field. Other unwritten contact words are also left
  unspecified, rather than assigned inferred meanings.
- `CCDBVolBox` constructs an 80-byte box, fills four three-component rows and
  halves **all three** input dimensions. The existing `CDBBox::halfSize` member
  spelling is retained for compatibility; in this constructor those input values
  are full dimensions. Using the current `CVolBox::SetHeight` helper would be
  wrong here because it does not halve its argument.
- Triangle/line calls `CSurfaceType::IsSeeThru(tri->surface, false)` unless the
  line-test bit is set. It does not call `ClearContacts`. A hit updates the line's
  cached length and collision depth even when contact generation is disabled.
- Box visibility clears draw-context +0x100 and tests six planes at camera
  +0x170. Frontline's clipping-mask accumulation does not apply. Splitting the
  opaque draw-context bytes names that word without changing the layout.
- `CTriangle` is only the observed three-vector prefix. `CCDBTriangle` gains the
  symbol-backed nearest-point declaration; no new class allocation is inferred.
  Vector copies, assignment and homogeneous words use the existing Rising Sun
  `CVector3` interface, not Frontline's doubleword-copy representation.
- Box/box retains all 15 separating-axis/contact branches, including the unusual
  axis-13 divisor using `R10 * R10` from the target. It writes contact feature 0
  at +0x24; Frontline's analogous source writes a different field.
- Capsule/triangle compares squared distances and a squared angular threshold
  (`0x364c6ccc`), clamps a negative/unordered computed squared distance to zero,
  and calls virtual `GetRadius` for classification and again for contact depth.
  Its nearest-point and segment-distance dependencies remain original context.

## Measurements

These are whole-function objdiff measurements, including every native instruction.
A score does not waive code-size, address, data or behavioral differences.

| Partial unit | Original address | Target bytes | Native bytes | Objdiff % |
| --- | --- | ---: | ---: | ---: |
| `box_box_axis` | `0x80137db4` | 128 | 128 | 93.593750 |
| `box_box_partial` | `0x80138510` | 9,940 | 9,252 | 57.296177 |
| `box_plane_partial` | `0x8013c3b4` | 1,724 | 1,652 | 84.459400 |
| `box_visibility` | `0x8013ca78` | 328 | 328 | 85.670730 |
| `cdb_box_ctor` | `0x8007576c` | 244 | 244 | 94.508194 |
| `cdb_box_line` | `0x80075ba0` | 308 | 312 | 96.038960 |
| `cdb_box_sphere` | `0x80075860` | 336 | 340 | 92.750000 |
| `cdb_box_triangle` | `0x80075cd4` | 2,332 | 2,048 | 57.684390 |
| `cdb_boxline_axis` | `0x80075654` | 280 | 280 | 94.785710 |
| `cdb_triangle_box` | `0x80076ae4` | 4,776 | 4,548 | 73.278050 |
| `cdb_triangle_capsule` | `0x8007833c` | 9,612 | 10,016 | 83.044110 |
| `cdb_triangle_line` | `0x800765f0` | 1,268 | 1,276 | 47.832810 |
| `cdb_triangle_sphere` | `0x80078198` | 388 | 344 | 71.422680 |

The exact overloads are `cdb_separate3` at `0x800754ec` (160 bytes),
`cdb_separate2` at `0x8007558c` (128 bytes) and `cdb_separate1` at `0x8007560c`
(72 bytes), with one independently verified zero literal each. Matching was
intentionally time-boxed after complete bodies and dependency/data validation.
Register assignment, temporary lifetimes, conditional dispatch and floating-point
contraction remain matching work. The two long contact dispatches use readable
conditional chains in partial source; they are not claims of original spelling.

## Known capsule limitation

The capsule distance quadratic loses precision when the segment intersects the
triangle: its true minimum distance is zero. Normalizing the resulting tiny
residual amplifies rounding differences. In the deterministic 2,000-case sweep,
12 original/native outputs exceeded the ordinary comparison tolerance, all in
this intersection regime. Classification and contact position remained consistent;
normals could differ substantially, and the maximum separation difference was
**0.0013810694**. This is an unresolved behavior difference, not a byte-only
matching issue. Do not promote this function on the strength of a high objdiff
score or the tests below.

The test uses an independent double-precision segment/triangle intersection
calculation to identify this regime. There it checks classification, state,
contact positions, finite unit-or-zero normals and a 0.003 bound around the
geometric separation `-radius` for the bounded test inputs. It deliberately does
**not** assert original/native normal or depth equivalence there. That bound is a
test-domain observation, not a universal numerical-error guarantee. Future work
should recover the precise evaluation/temporary order of the quadratic solver.

## Validation

`tests/test_collision_batch.py` contains five original/native PowerPC tests:

- 2,000 deterministic cases for each of 11 complete volume bodies (22,000 pairs),
  with rotated boxes, varying dimensions, both contact flags, line-test/material
  flags and preexisting collision-result states. Box/box covers **all 15** winning
  contact axes in both implementations.
- 396 boundary pairs plus 24 additional parallel, zero-length and endpoint line
  cases (420 pairs).
- 2,336 interval-helper pairs, including signed zero, NaN and infinities; 72
  box-axis pairs; and 225 line-slab pairs.

The full local suite passes **104 tests with no skips**, including these five
collision tests. Total: **25,053 original/native pairs**. Integer results, modified cache words and
constructor storage compare exactly. Other floating-point outputs use 3e-5
absolute/relative tolerance, with the explicit capsule exception above. `sqrtf`,
contact collection and material lookup are controlled callbacks. Original
nearest-point, segment-distance, virtual-radius and other math callees execute
normally in the isolated PowerPC fixture. Unwritten contact fields are not
compared. Volume inputs in this suite are finite; the scalar-helper NaN/infinity
coverage does not establish nonfinite-input equivalence for the volume bodies.
This is not game or Dolphin runtime validation.

Reproduction (optional Unicorn 2.1.4 is required for the local differential tests):

```sh
python3 tools/audit.py
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 tools/comparisons.py
python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
python3 tools/progress_report.py
```

Originals, disassembly, compiler probes and diagnostic objects stay ignored. The
original-object baseline succeeds separately and earns no source credit. The
complete source build also reproduces the 2,860,576-byte analysis image, SHA-1
`6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. All 2,046 previous exact-unit records
retain identical object and linked hashes; all 25 previous comparison receipts
also remain unchanged. The refreshed snapshot reports 1,076,936 exact linked
code bytes (43.215175%) and 46.246868% weighted similarity, up from 45.334907%.
The new exact helpers add 360 bytes; partial similarity earns no linked credit.
No runtime testing has been performed.
