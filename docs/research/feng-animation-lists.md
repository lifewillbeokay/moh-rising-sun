# FEng animation and list-box follow-up

This batch builds on kyleckroeger's shared frontend interfaces and reconstructed
callees in [PR #64](https://github.com/lifewillbeokay/moh-rising-sun/pull/64) and
[PR #65](https://github.com/lifewillbeokay/moh-rising-sun/pull/65). Thank you for
making the FEng object, key-track, color, list and game-interface layouts usable
across a wider part of the frontend. These additions use that work rather than
introducing parallel class views.

The new bodies were reconstructed with AI assistance from the pinned GR8E69
instructions and symbols. No additional Frontline source was copied. Ghidra
12.1.4 exports were used as research leads, with every exported body checked
against the original bytes and boundaries. Its inferred signatures, floating
comparisons and signedness were not treated as authoritative. Bulk exports and
native objects stay local under ignored directories.

## Scope and integration

The batch covers **41 functions / 13,640 original code bytes** across
`FEKeyInterp.cpp`, `FEKeyInterpLinear.cpp`, `FEKeyInterpNone.cpp`, `FEKeyTrack.cpp`,
`FEMath.cpp`, `FEListBox.cpp` and `FECodeListBox.cpp`. The original symbol-table file
records support these groups; the source fragments are not claims to have
recovered complete historical translation units.

Eight functions, totaling 964 bytes, are accepted exact source: the script
overload of `FEInterpLinear`, integer/float/vector2/vector3 lerps, `FELerpColor`,
the two-axis `FECodeListBox::ScrollSelection` wrapper and the `FEListBox`
constructor. The other bodies are registered comparisons. Complete exact
functions within a comparison earn exact-unlinked credit only; partial similarity
does not earn exact or linked credit. See the current snapshot for measurements.

The comparison groups include:

- Interpolation dispatch, stepped interpolation and quaternion interpolation.
- Key lookup, insertion, vector/color setters and track assignment.
- Quaternion-to-matrix conversion, identity and matrix/vector products.
- List-box cell initialization, row/column resizing, scrolling and updates.
- Code-list copying, initialization, string allocation, cell filling, logical
  indices, selection color, movement limits and cell recycling during scrolling.

All use the existing FEng ProDG 3.8.1 profile (`-O2 -G0 -fno-exceptions
-fno-implicit-templates`). Comparison manifests inherit it from
`fecode_setcellcolor.json`; no function-specific compiler flags or patches are
used. Unfinished bodies and helpers use `DECOMP_COMPARE`.

The scalar/vector lerps and quaternion lerp share a source file. Alignment at the
end of the scalar constant pool overlaps the first quaternion constant in the
original. The quaternion comparison therefore declares the four exact lerps as
explicit, independently verified context. It accounts for the full combined
constant pool and awards no duplicate credit. The exact build excludes the
quaternion body. Nothing clips a pool or borrows undeclared storage.

## Comparison measurements

These objdiff 3.8.3 values are weighted by original function size and exclude
linked context. They measure instruction similarity, not semantic correctness.
The snapshot retains per-function hashes, sizes, statuses and scores.

| Fragment | Functions | Target bytes | Weighted similarity |
| --- | ---: | ---: | ---: |
| `fecode_allocate_strings` | 1 | 488 | 87.54% |
| `fecode_copy` | 2 | 780 | 87.62% |
| `fecode_current_from_target` | 1 | 112 | 78.57% |
| `fecode_fill_cells` | 1 | 584 | 93.72% |
| `fecode_indices` | 2 | 240 | 88.17% |
| `fecode_initialize` | 1 | 556 | 90.86% |
| `fecode_movement` | 2 | 724 | 87.40% |
| `fecode_scroll_selection` | 1 | 2,404 | 37.03% |
| `fecode_select_callback` | 1 | 112 | 53.82% |
| `fekeytrack_assign` | 1 | 364 | 81.30% |
| `fekeytrack_lookup` | 3 | 460 | 98.48% |
| `fekeytrack_set_values` | 2 | 396 | 75.51% |
| `felistbox_initialize_cells` | 1 | 132 | 83.09% |
| `felistbox_resize` | 2 | 844 | 91.68% |
| `felistbox_scroll` | 1 | 1,160 | 71.77% |
| `felistbox_update` | 1 | 336 | 63.58% |
| `feng_interp_dispatch` | 3 | 344 | 92.37% |
| `feng_interp_none` | 2 | 256 | 83.88% |
| `feng_lerp_quaternion` | 1 | 1,340 | 58.62% |
| `feng_matrix` | 4 | 1,044 | 68.07% |

## Evidence and preserved behavior

`FEVector2` is a two-float view supported by the loads/stores and the external
assignment symbol. `FEQuatScale`, `FEQuatAdd`, `FEQuatSubtract`, `FEQuatMultiply`
and `FEWrapIndex` are descriptive helpers, not recovered original method names.
Callback words at `FECodeListBox +0xbc/+0xc0` are cast locally to the observed
function/context pair. Their broader ownership remains unknown.

The following surprising behaviors are retained:

- The signed wrap helper returns `count` for a negative exact multiple when
  `count > 1`. It does not use a mathematically normalized modulo operation.
  Callers still require valid nonzero dimensions where the original does.
- Quaternion interpolation negates the second quaternion for the spherical
  path when the dot product is negative. Its near-linear path uses the original
  second input, including the antipodal case. Unordered floating comparisons
  follow the original branches; normalizing the formulas would change them.
- `FEListBox::Update` clamps an unordered blink value to **one**, reverses the
  blink rate, and completes scrolling for an unordered remaining distance.
- `FEListBox::SetNumColumns` copies `keep` **bytes** per retained row of cells;
  `SetNumRows` copies `keep * numColumns` bytes. Both also construct and initialize
  cells as shown in the original. These short copy counts are not silently
  changed to `sizeof(FEListBoxCell)` multiples.
- `FECodeListBox::Initialize` copies the new row width, even when growing. This
  can read beyond the old row; changing the algorithm belongs to a separate
  behavior-change proposal, not this reconstruction.
- Negative vertical code-list scrolling passes the same current column to each
  refill callback, while positive vertical scrolling advances the column.
  Cell moves recycle their string pointers and preserve the original ordering.
- `CheckMovement` sends both minimum and maximum notifications when the visible
  range already covers the total under flag 4. `MakeMove` can update a target
  without moving the visible window and returns zero in that case.
- Track value setters assume `CreateKeyAt` succeeds; the original dereferences
  its return. Assignment preserves reference-list versus copied-node behavior.

The new `FEListBoxCell` constructor describes the initialized subset seen in the
array allocation loops. It leaves the same other fields uninitialized. The
all-ones word at `+0x24` is exposed through a union with the existing float view;
the sentinel's meaning is still unknown. The six-argument code-list scroll method
returns an observed integer result; its existing wrapper ignores it.

## Validation and remaining work

`tests/test_feng_animation_lists.py` executes original and native PowerPC bodies
with the optional Unicorn 2.1.4 fixture: 21 test groups and 1,037 scenarios. Cases cover empty/referenced key lists,
duplicate insertion, stepped interpolation boundaries, cached dispatch,
key-value changes, aliased matrix outputs, quaternion branches including
antipodal/NaN inputs, negative/wrapped indices, movement flags and notifications,
four-direction cell recycling, override/game/absent callbacks, zero/grow/shrink
allocation paths, string migration, copying and track ownership.

Integer returns, memory and callback ordering compare exactly. Floating results
use the shared fixture's six-decimal normalization; math calls and allocation
are stubbed. The checks do not establish every aliasing combination, arbitrary
callback mutations, allocation failure behavior or live game behavior. Public
CI skips original-dependent checks when the pinned original/optional emulator
is unavailable.

Local validation passed the original-object baseline (2,860,576-byte normalized
DOL), a clean rebuild of all 2,598 accepted units with a byte-identical complete
analysis image, all 69 registered comparison units, and all 129 tests. The test
suite used Python with Unicorn 2.1.4 available, so the original-dependent checks
ran. Snapshot capture reruns the comparisons and records the pinned inputs.

These are separate checks: original-object relinking earns no source credit,
isolated differential tests do not run the game, and public CI validates the
snapshot rather than rebuilding with the proprietary compiler. Runtime remains
untested.

Most remaining compiler differences involve register scheduling, temporaries,
branch arrangement and duplicated versus shared loop bodies. Keep them measured
instead of treating objdiff similarity as semantic proof. `FEMatrix4::Identify`
and the script `FEInterpNone` wrapper are exact-unlinked inside their comparison
groups and are candidates for a later independent promotion.

The 1,568-byte track overload of `FEInterpLinear` remains original context. Its
flag-2 path appears to leave the left-key pointer null before a later use. That
path needs further instruction/caller investigation; this batch does not invent
a replacement or count it as reconstructed source. Spline interpolation and the
remaining frontend rendering paths are other follow-up targets.
