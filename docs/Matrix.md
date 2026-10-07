# CMatrix reconstruction evidence

Twenty-three game-specific functions now compile to **4,876 matching code bytes**,
with **100 generated read-only data bytes**, in twelve accepted fragments. They use
ProDG 3.8.1 with `-O2 -G0`; the general inverse additionally requires
`-ffast-math`. This does not identify the original compiler release. Reconstruction
and verification used AI assistance; these functions were reconstructed from the
pinned executable rather than imported from another decompilation.

## Shared declaration and limits

[The shared header](../include/game/CMatrix.h) represents a matrix as four groups
of four floats, occupying 64 bytes. This representation is supported by several
independent observations:

- Original `_Mat_Data`, `_Mat_Unit` and `_7CMatrix.s_TempMat` symbols each have
  64 bytes of storage. `_Mat_Data` contains a four-by-four identity matrix.
- Assignment copies four 16-byte groups at offsets 0, 16, 32 and 48. It retains
  the destination in the return register, consistent with a reference return.
- `SetRight`, `SetFront`, `SetUp` and `SetPos` identify the first three components
  of those groups, in that order. They leave each fourth component untouched.
- `GetSlot(0)` writes all sixteen identity components. `Multiply` reads all
  sixteen components from each operand and computes all sixteen output values.
  The generated instructions from both routines agree with the same declaration.

`CMatrix` and its accepted method names and parameter types come from original
symbols. `CMatrixRow`, `row`, component names and parameter names are descriptive
reconstruction names. They do not claim the original nested types, member names,
access control or complete historical class declaration. Ordinary return types
and static membership are inferred from the observed calling behavior; they are
not encoded by these method symbols. The four-byte initialization flag is
represented as an `int`; its original declared signedness is not established. Constructors and other
unmatched methods are not supplied by this header; default construction must not
be assumed to reproduce the game's constructor behavior.

`CVector3` is now declared in [its own header](../include/game/CVector3.h); see
[CVector3 layout](#cvector3-layout). The earlier fragments still read vectors
through `Vector3Components`, which remains for them. The matrix also gains inline
row reads (`GetRight`, `GetFront`, `GetUp`, `GetPos`) that return a row as a
`CVector3` copy; their names mirror the original setters and are not recovered.

## Accepted fragments

| Manifest | Original text start | Functions | Code bytes | Generated data |
| --- | --- | ---: | ---: | --- |
| `matrix_core` | `0x8007a9b4` | 4 | 456 | None |
| `matrix_rotations` | `0x8007ac80` | 3 | 360 | 12 bytes at `0x8029afa8` |
| `matrix_translation` | `0x8007ade8` | 7 | 376 | None |
| `matrix_getslot` | `0x802790e0` | 1 | 92 | 8 bytes at `0x80299754` |
| `matrix_multiply` | `0x802794b8` | 1 | 836 | None |
| `matrix_8007ab7c` | `0x8007ab7c` | 1 | 260 | 12 bytes at `0x8029af9c` |
| `matrix_8007afe0` | `0x8007afe0` | 1 | 212 | 12 bytes at `0x8029afb4` |
| `matrix_8007b0b4` | `0x8007b0b4` | 1 | 176 | 8 bytes at `0x8029afc0` |
| `matrix_8007b290` | `0x8007b290` | 1 | 788 | 12 bytes at `0x8029afd0` |
| `matrix_fast_inverse` | `0x8007b164` | 1 | 300 | 8 bytes at `0x8029afc8` |
| `matrix_orthonormalize` | `0x8007b5a4` | 1 | 784 | 8 bytes at `0x8029afdc` |
| `matrix_to_euler` | `0x8007b8b4` | 1 | 236 | 20 bytes at `0x8029afe4` |

The core fragment contains `InitClass`, assignment and both `BuildScale`
overloads. Translation contains `BuildTrans`, the four named setters,
`PreTranslate` and `Translate`. Rotations contains `BuildRotX`, `BuildRotY` and
`BuildRotZ`. Manifests record every individual function's original mangled name,
address, size and binding.

The three earlier fragments lie in the interval from the `matrix.cpp` compiler
marker at `0x8007a9b4` to the next file's marker at `0x8007be5c`. This is the
existing code map's inferred file-ownership evidence. The later `GetSlot` and
`Multiply` bodies have insufficient original-file evidence; their manifests
intentionally do not assign an original source file. The source files
are reconstruction fragments, not a claim about original translation-unit splits.

## Behavior, constants and original context

`InitClass` copies the 64-byte identity data into `_Mat_Unit` and writes 1 to
`s_ClassInit`. The compiler expands the copy inline. These globals remain
original context, with no source or data credit:

| Original symbol | Address | Bytes | Evidence |
| --- | --- | ---: | --- |
| `_7CMatrix.s_ClassInit` | `0x802c2100` | 4 | Global initialized object |
| `_Mat_Data` | `0x802c2104` | 64 | Private `matrix.cpp` identity data |
| `_Mat_Unit` | `0x802fbf40` | 64 | Private `matrix.cpp` BSS object |

The core manifest scopes both private dependencies to the original `matrix.cpp`
file record. The separate temporary matrix and global-constructor routines also
remain original context. Declaring private dependencies as external here is a
linking boundary between accepted source and original storage, not a claim that
the original source exported them.

`GetSlot` writes identity only for slot zero; other values leave the matrix
unchanged. Its generated constants are the floats 1 and 0. Each axis rotation
multiplies its input by the observed float **2670176.75** (`0x4a22f983`), converts
the result to an integer and calls original `MathSinCos`. The three copies of
that constant are verified at the addresses referenced by the original
instructions. The original spelling of the scale expression remains unproven. The matching
`MathSinCos` body is now documented in [Geometry.md](Geometry.md); it converts
that integer angle back to float radians and calls sine and cosine.

`Multiply` preserves explicit output order and floating-point expression order.
It writes directly to the destination as the original does; no temporary matrix
or non-aliasing promise is introduced. Do not assume that in-place multiplication
has the same result as computing through an independent temporary. `PreTranslate`
adds a basis-weighted offset, whereas `Translate` adds components directly.
Both leave the other matrix components untouched.

## General rotation, projections and inverse

`BuildRot` uses the observed three-float axis prefix and the same angular scale
and `MathSinCos` boundary as the axis-specific rotations. It constructs the
three-by-three rotation using cached axis components, writes zero to the other
homogeneous components, and writes one at offset sixty. It does not normalize the
axis. The original source's expression spelling and parameter preconditions are
unknown; no full CVector3 construction is introduced.

`Perspective` and `Orthographic` call `GetSlot(0)` before replacing their observed
projection entries. The parameter names describe their formulas, not recovered
historical names. In particular, the retained orthographic routine writes zero
to offset sixty, as the original does; it is not replaced with a conventional
projection formula. The constants 1, -2 and 0 are compared at the addresses
referenced by the original functions.

`Inverse` separately accumulates positive and negative determinant terms. It
returns without writing the destination when the determinant is zero or its
observed relative-magnitude check falls below the single-precision constant
`1.0e-15f` (`0x26901d7d`). Otherwise it writes the inverse three-by-three entries
and translated position directly, with homogeneous zeroes and a final one.
Expression and write order are retained; in-place aliasing must not be assumed
safe. This range requires `-ffast-math` to reproduce its original comparison and
arithmetic instructions, so the high-level code is not a promise of IEEE NaN
behavior under different compiler settings.

These four routines add 1,436 game-code bytes and 44 read-only data bytes. They
lie within the original `matrix.cpp` marker interval, but their separate source
files are project build fragments. They reuse the existing matrix storage and
opaque-vector prefix view; no additional complete game type was invented.

## Fast inverse, orthonormalization and Euler angles

Three more functions use the existing 64-byte matrix and 16-byte vector views.
They use the ordinary ProDG 3.8.1 `-O2 -G0` profile, without fast-math. Their
small inline arithmetic helpers have descriptive names; no original helper
names or inline/source boundaries are claimed.

`FastInverse` copies the input's three basis vectors and position, transposes the
basis into the output, and computes the negative dot products of the saved
position with those saved basis vectors. It sets the first three homogeneous
entries to zero and the last to one. It assumes the inverse can be obtained this
way; it does not test orthogonality or add a singularity fallback. Transposition
reads directly from the input while writing the destination, as the original
does. The saved vectors do not make every operation safe for in-place aliasing.

`Orthonormalize` copies the three original basis vectors and applies successive
projection subtraction: normalize right; remove right's component from front
and normalize it; remove right and normalized front's components from the
original up vector and normalize that result. Each normalization skips scaling
when the computed length equals zero. It writes only the nine basis components,
preserving position and all four homogeneous words. It does not synthesize a
replacement axis for degenerate inputs. The original temporary vectors, dot
product grouping and order of floating operations are preserved.

`ToEulerXYZ` retains its unusual entry selection: it branches on `row[0].z`,
then, in the nonsingular branch, computes the first angle as
`asinf(-row[1].z)`, the second as `atan2f(-row[0].z, row[2].z)`, and the third as
`atan2f(row[0].y, row[0].x)`. The two boundary branches use opposite half-pi
constants, signed `atan2f(row[1].x, row[1].y)` and zero for the third angle.
The source preserves these formulas and comparisons, rather than assuming a
standard Euler convention or clamping the asin argument.

The [quaternion and line helpers](Geometry.md) now reuse these vector/matrix
representations. The independently matched `MathSinCos` implementation also
resolves the earlier angular-scale observation: its integer input is multiplied
by the float representation of `2*pi / 2^24` before calling sine and cosine.

## CVector3 layout

The vector is **16 bytes**: three float components and a fourth word that every
constructor sets to 1.0. The evidence is independent of the matrix routines:

- `ISceneNode::GetWorldLinearVelocity` returns a vector by value and writes all
  four words: zero at `+0`, `+4`, `+8` and 1.0 at `+12`.
- Local vector temporaries in `Distance`, `DistanceXY`, `DistanceSquared`,
  `DistanceSquaredXY` and `Constrain` are 16 bytes with 1.0 stored at `+12`.
- `CParticleSystem`'s constructor stores only 1.0 into the fourth word of each
  vector member, at a 16-byte stride; the particle getters read members at
  `+0xd0`, `+0xe0` and `+0xf0`.
- Assignment from another vector writes only `+0`, `+4` and `+8` of the
  destination and also builds a 16-byte temporary with 1.0 at `+12`.

The declaration reproduces these with: a default constructor that sets only the
fourth word; three-component and copy constructors that set the three components
and then the fourth word; and an assignment that copies three components and
returns a vector **by value**. The distance methods default-construct their
temporary and then set its components, which stores the fourth word first; the
zero vector returned by `GetWorldLinearVelocity` stores it last, which the
three-component constructor reproduces.
These shapes were chosen because others change the emitted order of stores or
constants; the member names, inline helpers and the fourth member's purpose are
not recovered. The methods with original symbols are:

| Manifest | Functions | Range | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `vector_methods` | `Constrain`, `RotateAboutX`, `RotateAboutZ`, `Distance`, `DistanceXY`, `DistanceSquared`, `DistanceSquaredXY` | `0x8007ba98`-`0x8007be5c` | 964 | 32 bytes at `0x8029b050` |
| `vector_dot` | `Dot` | `0x80279490` | 40 | None |

`Constrain(target, cosLimit)` copies the target when the two vectors are nearly
opposite (`dot + 1 < 0.0001`). Otherwise, when their dot product is below
`cosLimit`, it rotates toward the target by spherical interpolation so the angle
reaches `acos(cosLimit)`, scaling the target in place. Both tests are written with
negated `>=` comparisons, as the original branches require. The XY distances
build their temporary with z = 0 and sum only x and y.

Using the declaration, these further functions match: the `ISceneNode`
vector defaults and the `CParticleSystem` vector getters, setter and axes (see
[SceneNode.md](SceneNode.md) and [ParticleRecipes.md](ParticleRecipes.md)). All
accepted matrix fragments produce unchanged objects with the complete type.

## Verification and next work

All generated allocated sections, function boundaries, external dependencies and
constants are checked, followed by the complete reconstructed analysis image.
No instruction patches, assembly implementations, discarded generated code or
partial-match credit are used. The unchanged original-object baseline is a
separate earlier result; emulator and on-disc loading behavior remain untested.

The dependency scan originally ranked initialization, assignment and
multiplication at 173, 127 and 75 distinct unfinished callers respectively.
Those numbers overlap and are prioritization evidence, not newly reconstructed
caller code. The reusable declaration is the main benefit beyond these verified
functions: future transform-related functions can now use an independently checked
matrix representation.

The [particle fragments](ParticleRecipes.md) now use this declaration for their
two matrix-copy getters and an `Orthonormalize` wrapper. The matrix body is now reconstructed as well; the wrapper and matrix body
retain separate, nonoverlapping source credit.

The [camera fragments](Camera.md) reuse the same representation and declare the
original shared `CMatrix::s_TempMat` (64 bytes at `0x802fbf00`) for pre- and
post-transform operations. The header also exposes `Rotate(const CVector3 &,
float)` as an external original method. Neither that method nor the temporary's
storage earns new matrix-source credit. The camera transform wrappers pass vectors by reference; the new vector getters
construct and assign the recovered row copies.

Useful next work includes `Rotate` and the remaining matrix functions that take
or build vectors. A research `TibToMOHFL`
implementation has the expected 88-byte size but still differs in instruction
scheduling/register allocation; it earns no credit. Extend the vector
declaration only where emitted code requires it, and record the evidence.
