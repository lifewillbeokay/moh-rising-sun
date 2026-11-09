# Collision distances and database traversal

This follow-up reconstructs eleven complete GR8E69 functions spanning **14,920
target code bytes**: two shared distance helpers, two box queries, two visibility
functions, and five collision-database traversal/query functions. All eleven are
maintained partial source. They use the shared ProDG 3.8.1 game profile
(`-O2 -G0 -fno-exceptions -fno-implicit-templates`), inherited from
`cdbgeom_box_plane.json`, and `DECOMP_COMPARE` body guards. Their original bytes
remain in the verified image; this batch adds no exact or linked code credit.

AI assisted the source reconstruction, independent GameCube analysis and tests.
Source filenames, helper names, locals and newly named storage fields are project
choices, not recovered historical declarations. Original method names, parameter
types and constness come from GR8E69 symbols.

## References and attribution

The two distance solvers and line traversal use kyleckroeger's
[Medal of Honor: Frontline reconstruction](https://github.com/kyleckroeger/moh-frontline-decomp)
as a semantic starting point, pinned at `9ce7833e4dab6dd788bb86b43216159d6b628b35`. The upstream root license is CC0 1.0,
byte-identical to this project's [LICENSE](../../LICENSE); see the
[previous collision batch's provenance](collision-batch.md#upstream-attribution).
Research drafts are unverified upstream and do not establish Rising Sun behavior.

| Reference at the pinned revision | SHA-256 |
| --- | --- |
| [src/game/line.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/line.cpp) | `f05fc7e4c6eb1fd503d92bae6f4efc5139d79ca0ce1b9a8f4dda4f472de8b940` |
| [research/drafts/GetNearestPoint.8006c800.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/research/drafts/GetNearestPoint.8006c800.cpp) | `6604ee77d340acbd4ad2892ed223b86642694412cb051c3a73f116c7532c6bc5` |
| [src/game/cdbobject_line.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/cdbobject_line.cpp) | `9a1aca01d57b33016c9bee73e665ee19bac161dca1e16b28ce4a1f094c76c3f1` |
| [research/drafts/TestCDBBranchNodeCollision_6CLine3_.800701d4.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/research/drafts/TestCDBBranchNodeCollision_6CLine3_.800701d4.cpp) | `50cc6b37de9a21d30dcd1a685bfe524771acee4973bd48901c7e91fb13749028` |
| [src/game/capsule_plane.cpp](https://github.com/kyleckroeger/moh-frontline-decomp/blob/9ce7833e4dab6dd788bb86b43216159d6b628b35/src/game/capsule_plane.cpp) | `26ca56cd94b82eebac0caed910fa49431962f026f3ecfc74c3ffece45a100522` |

The four volume traversal bodies also reuse the already reconstructed Rising Sun
leaf tests and container interfaces. Box queries and the visibility bodies were
reconstructed from Rising Sun instructions and existing geometry interfaces.
Frontline's capsule visibility reference is a constant return; it cannot supply
Rising Sun's projected-bounds implementation and was not copied as that body.
Upstream profiling calls and Frontline's early exit on collision state 3 were
not carried into the Rising Sun line query.

## Target behavior and interfaces

- `CLine3::GetSquaredDistanceBetweenSegments` returns a 0/1 parallel-case flag in
  `r3` (the extraction at `0x8007c7a0`–`0x8007c7a4`). Its distance goes through
  the optional pointer; the determinant incidentally remains in `f1`. The prior
  float-return declaration is corrected to `bool`. Known callers ignore this
  return. The function updates both squared-length caches, uses a `1e-5f`
  determinant threshold, and preserves optional-distance and degenerate cases.
- Triangle nearest-point takes its input by reference, returns a 16-byte vector
  with the established homogeneous word, and independently checks both optional
  barycentric outputs. Frontline's by-value point interface was not imported.
- Box/sphere keeps the target's sign-bit/exponent classification, maximum interior
  face distance, zero interior contact normal, virtual radius lookup and
  contact-generation flag. Box/capsule retains its early axis rejection,
  center projection, support-distance rejection and original segment/box-distance
  dependency. Radius reads remain virtual, including repeated contact-time reads.
- Sphere visibility writes the projected screen rectangle at draw-context
  `+0x104..+0x110` and the six plane bits at `+0x100`; its rejection mask is
  separate. The plane-to-bit order is 26, 27, 30, 31, 29, 28. It still writes the
  bounds when invisible. Four float fields replace opaque bytes without changing
  any subsequent offset or the existing class allocation view.
- Capsule visibility projects four endpoint offsets, compares clip coordinates
  to `+/-abs(w)`, and performs the target's combined outcode test. It divides by
  each homogeneous coordinate and merges the two endpoint rectangles. It leaves
  draw-context `+0x100` unchanged, including on rejection. No new clipping or
  division-by-zero guards are inserted.
- Volume traversal preserves the 20-entry LIFO branch stack (growth word 11),
  front/back eligibility and processing order, leaf box checks and positive-only
  triangle-group marking. Box leaves call the base `CVolBox` query, while the
  other leaf shapes use `CCDBVolBox`. Each overload gathers groups and returns
  the largest leaf result; it does not restore the marks itself.
- The line query handles a null database and a leaf-only root. Its separate
  48-byte local frames contain segment endpoints and a node pointer; the frame
  type/alignment spelling is descriptive. It splits the segment for branch
  selection, tests leaves with the **original full line**, deduplicates groups,
  retains maximum results without Frontline's state-3 early exit, and restores
  every mark it changed. The 2,048-entry group storage and 20-frame stack retain
  the original bounds assumptions; no new capacity behavior is claimed.

The dependency scanner finds two distinct direct callers each for segment distance
and triangle nearest-point, with 30 and 17 call sites respectively. These are
syntactic references, not runtime frequency or additional source credit. Both
helpers are dependencies of the maintained triangle/capsule partial from PR #62.

## Complete-function measurements

Each manifest inventories the whole original function and its complete native
output, external dependencies and own constant pool. The eleven new units emit
96 checked data bytes. No generated data is clipped, borrowed or patched.
Native code-size and instruction differences remain visible in the receipts.

| Partial unit | Original address | Target bytes | Native bytes | Objdiff % |
| --- | --- | ---: | ---: | ---: |
| `cdb_branch_box` | `0x80074784` | 948 | 904 | 78.949360 |
| `cdb_branch_sphere` | `0x80074b38` | 764 | 760 | 86.047120 |
| `cdb_branch_capsule` | `0x80074e34` | 956 | 916 | 78.736404 |
| `cdb_branch_animatedvolume` | `0x800751f0` | 764 | 760 | 86.047120 |
| `cdb_object_line` | `0x80073ba0` | 2,236 | 1,976 | 65.919500 |
| `triangle_nearest` | `0x80077d8c` | 1,036 | 932 | 61.274130 |
| `box_capsule` | `0x8013b2e8` | 1,588 | 1,576 | 64.219150 |
| `line_segment_distance` | `0x8007c668` | 1,424 | 1,516 | 85.553370 |
| `sphere_visibility` | `0x801377f0` | 1,284 | 1,164 | 59.448597 |
| `box_sphere` | `0x8013abe4` | 1,636 | 972 | 38.261612 |
| `capsule_visibility` | `0x8014402c` | 2,284 | 1,912 | 58.763573 |

The corrected segment-distance return declaration also changes the generated
comparison for the existing triangle/capsule partial: its native size remains
10,016 bytes, with similarity moving from 83.044110% to 83.177280%. This is not
an exact promotion or a resolution of its previously documented numerical
limitation. The other 37 previous comparison records remain unchanged.

## Known box/capsule contact limitation

When the segment intersects the box, the computed closest points should coincide.
Tiny residuals from the recovered multiply/add order can differ; normalizing
those residuals produces substantially different normals and contact positions.
In the deterministic 2,000-case sweep, **106 cases** exceed the ordinary comparison
tolerance, all independently classified as segment/box intersections. Return
classification, collision state, contact separation and feature agree in those
cases. The maximum observed contact-position difference is **2.4778835 world
units**. This is a real unresolved behavior difference, not just a byte mismatch.

The test uses an independent double-precision slab intersection check on the
stored float inputs to identify this regime. For those intersections it compares
classification, state, contact count, separation and feature, and checks finite
unit-or-zero normals and contact positions within the capsule radius of the
segment (with a test-domain `1e-4` tolerance). It explicitly **does not assert
original/native normal or position equivalence** there. The ordinary full-output
comparison applies elsewhere. These checks do not justify promotion of this
function or establish a universal error bound. Recovering the precise vector
temporary and fused multiply/add order remains future work.

## Validation

The optional Unicorn 2.1.4 differential fixture in
`tests/test_collision_distance_traversal.py` executes **22,566 original/native
pairs** across four tests:

- 4,522 shared-distance pairs: 2,000 randomized inputs plus 261 boundary and
  degenerate inputs for each helper, including cached lengths, zero/parallel
  segments, collapsed triangles and independently null optional outputs.
- 4,522 box-query pairs: randomized rotations/dimensions and boundary cases,
  both contact flags and existing result states, with the explicit intersection
  limitation above.
- 8,522 visibility pairs: identity transforms, boundary cases, rotated and
  translated camera views, perspective and orthographic projections, visibility
  results, clip flags and all four projected bounds.
- 5,000 traversal/query pairs: nested trees, front/back branches, null children,
  leaf-only and null databases, empty and repeated triangle groups, pre-marked
  groups and callback result states 1/2/3. Group order and mark restoration are
  checked, along with line-query callback arguments.

Integer state and call traces compare exactly. Other floating-point outputs use
`3e-5` absolute/relative tolerance, with paired NaNs accepted for degenerate
geometry. These tests cover bounded finite geometry inputs; they do not establish
arbitrary nonfinite-input behavior. `sqrtf` and contact collection are controlled
callbacks; the line-query group callback returns controlled states to isolate
traversal. Other math/leaf/radius callees execute original code. This is not game
or Dolphin runtime testing.

The original-object baseline passed separately and earns no source credit. The
complete source rebuild verified all 2,049 accepted units and produced an
identical image; every prior exact unit's object/link receipt is unchanged.
All 108 tests passed locally, with no skips. Snapshot capture rebuilt all 49
comparisons and passed the image, data, dependency and source-inventory checks;
the public report export also passed.

Exact and linked coverage remain 1,076,936 of 2,492,032 code bytes (43.215175%),
across 5,152 functions: 684,720 reconstructed-game bytes and 392,216 restored-library
bytes. Weighted instruction similarity increases from 46.246868% to 46.644417%;
that increase is not exact or linked credit. Reproduction:

```sh
python3 tools/audit.py
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 tools/comparisons.py
python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
python3 tools/progress_report.py
```

Original executables, disassembly, probes and raw comparison objects stay ignored.
CI validates the public snapshot and tests; it does not rebuild the game.
