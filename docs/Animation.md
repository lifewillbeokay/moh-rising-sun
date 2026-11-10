# EAGL animation reconstruction

The animation work adapts the public NFS Most Wanted reconstruction to Rising
Sun's older `EAGLAnim` interface. It includes scalar/vector delta decoding,
compressed channels, composition, events, skeleton poses, run blending, raw and
cyclic channels, allocation support and stateless control interfaces. The first
two delta-compressed animation decoder units cover
22 functions and 15,012 executable bytes: `FnDeltaF1` contributes 6,388 bytes and
`FnDeltaF3` contributes 8,624. Each generated 184-byte virtual table and 80-byte
constant pool is checked separately and earns no code credit. AI assistance from Codex and Claude Code was
used for reconstruction and review.

## Reference and target evidence

The reference is [dbalatoni13/nfsmw at revision
1f2cdd7996791c81a580b3f7b36b44d4f9f6719c](https://github.com/dbalatoni13/nfsmw/tree/1f2cdd7996791c81a580b3f7b36b44d4f9f6719c/src/Speed/Indep/Src/EAGL4Anim).
Its CC0 license and attribution are preserved in
[`src/eagl_anim/NOTICE`](../src/eagl_anim/NOTICE) and
[`LICENSE.nfsmw`](../src/eagl_anim/LICENSE.nfsmw). This is reconstructed game code,
not restored SDK/library credit. Reference names are useful reconstruction names;
they do not establish original Rising Sun member spelling.

Original mangled symbols establish the classes and method interfaces. The
`FnDeltaF1` constructor, destructor and evaluators independently establish a
48-byte object: previous key/block/value fields at `0x10`/`0x14`/`0x18`, next
key/block/value fields at `0x1c`/`0x20`/`0x24`, a mask pointer at `0x28`, and a
pointer to 16-byte unquantization records at `0x2c`. The deleting destructor
passes 48 to the original allocation callback. The base constructor zeroes its
unknown word at offset four and animation pointer at offset twelve. The unknown
word's original type remains unspecified.

The memory-mapped animation header occupies twenty bytes before its per-bone
records in both formats. The original accesses establish the index/time pointers, frame and bone
counts, bin-length exponent and constant-bone count. Each following `DeltaF1` record has
two floats and two unsigned halfwords, with a twelve-byte stride. Physical
samples are unsigned halfwords; delta samples are bytes. Arithmetic and integer
conversion order follow the target, including signed promotion during sample
unquantization and unsigned conversion in direct frame-count casts.

The original `BoneMask` copy constructor at `0x801eddc8` copies 32 bytes, supporting
the eight-word mask view independently of the reference. Masked evaluation uses
an eighty-byte temporary index array, established by its stack accesses. No new
bounds checks are added. Base and memory-manager virtual-slot ordering is checked
against the original tables. The default base implementations and memory-map methods are now reconstructed;
allocation callbacks remain external original context.

## Three-component decoder

`FnDeltaF3` uses the same 48-byte object layout with animation type 20; the
`FnDeltaF1` constructor stores type 21. Its value buffers contain 16-byte `COORD4`
elements, with three components used, and its decoded range records occupy
48 bytes. Each encoded per-bone record has a 36-byte stride: three physical
minimum floats at offset zero, three range floats at twelve, and unsigned
halfword minimum/range triplets at twenty-four and thirty. Original allocation
sizes, accesses and the three-iteration decoding loop support these layouts.
The coordinate declarations reuse the established [transform interface](Rendering.md).
Reference field names remain descriptive; there is no claim to original member names.

Both evaluators recover physical samples at bin boundaries, accumulate forward
or reverse deltas, cache the adjacent sample and interpolate when required.
The masked evaluator preserves the original mask-cache invalidation and constant
bone handling. The target's expression order is preserved, including differences
between its masked and unmasked interpolation formulas. In `EvalSQT`, one shared
frame-loop counter and the original pointer/index update order are needed to
reproduce the compiler's scheduling. These source decisions follow the target
instructions; no register pins or instruction substitutions are used.

## Compilation and verification

The working profile is ProDG 3.8.1 with `-O2 -G0 -fno-exceptions
-fno-implicit-templates`. It is not proof of the original compiler release.
The compiler emits the virtual tables in their native named
`.gnu.linkonce.d._vt.Q28EAGLAnim9FnDeltaF1` and
`.gnu.linkonce.d._vt.Q28EAGLAnim9FnDeltaF3` input sections. The linker resolves each
weak object to the original global binding. The manifest places that complete
section at the original `.data` address; there are no instruction or object
patches.

The verifier permits only named native vtable sections for this additional data
mapping. It checks the complete original object, compiler ownership and binding,
linked ownership and original binding, allocated-section flags, relocations,
external definitions and all bytes. Mutation tests reject partial/moved vtables,
altered metadata and attempts to map arbitrary link-once sections or code.
Existing code and BSS mapping restrictions remain in force.

The shared class declarations describe the checked interface and storage needed
by these fragments. Exact historical header organization and visibility remain
unknown. Undecompiled animation formats earn no credit.
Complete-image verification is required before snapshot publication; no animation
runtime or emulator testing has been performed.


## Composition, masks and base methods

`CompoundChannel` adds 19 functions and 4,604 code bytes, with its complete
80-byte constant pool checked separately. Its map prefix has attributes at four,
unsigned halfword channel/frame counts at eight/ten, then a variable pointer
array. The function object is 28 bytes: inherited map at twelve, channel array
at sixteen, four-byte FPS-enable flag at twenty and byte FPS at twenty-four.
Its deleting destructor and all evaluators independently support this storage.

The composition routines initialize and dispatch subchannels through the original
memory-manager slots. They preserve the original new-map-before-cleanup order,
reverse iteration and non-short-circuit accumulation of child results. The
attribute helper retains a two-byte ID, its by-value copies and the returned
pointer slot's high byte for FPS. Reference member names remain descriptive.
The composition vtable stays original context under a scoped native interface
pragma; no generated data is silently removed.

The six `BoneMask` functions add 524 bytes. Construction, complement, equality
and bit mutation agree on eight 32-bit words. Separate constructor and operation
fragments preserve the compiler's inline constructor copies without duplicate
credit. Seventeen default `FnAnim` methods add 128 bytes; their no-op, false and
null results are present in the original, not placeholders for unfinished code.
Six `FnAnimMemoryMap` methods add 120 bytes.

The recovered base destructor corrects the initial allocator placement:
`FnAnimMemoryMap` performs ordinary deletion, while the decoded leaf classes
use the EAGL free callback. The class-body allocation include now belongs to those
leaf classes. Both previously accepted delta decoders remain byte-identical after
this correction. The original macro/header organization is still unknown.

## Compressed scalar and quaternion channels

`DeltaCompressedData` contributes four functions and 3,592 code bytes; `DeltaChan`
contributes 25 functions and 9,136 bytes. Their complete 32/128-byte constant pools
also match. The former has a four-byte count/quantization prefix followed by
12-byte minimum/range/initial-value records and the delta stream. The latter's
plain and keyed maps have variable index arrays starting at ten and fourteen;
rounded C++ object size is not used as serialized-header length.

Constructors, buffer accesses and deleting destructors support the two 36-byte
function-object layouts. Scalar and quaternion evaluators retain their cache,
mask and interpolation behavior. The target reallocates when the new DOF count
is greater than or equal to the previous count; this is not changed to a
conventional capacity check. The indexed four-bit decoder retains its unusual
initial nibble selection. Masked decompression supports the two observed sample
widths only.

Key-time index minus one denotes the implicit initial sample at time zero.
The equivalent expression `lowKey - 1 == -1` preserves the original compiler's
separate comparison in the four keyed evaluators. The source retains this index
interpretation rather than substituting machine instructions. Masked evaluation
uses an eight-bit bone index; bone enumeration retains its observed 16-bit result.
Original vtables remain external context for these channel fragments.

Four derived channel destructors add 768 bytes at `0x80293368`–`0x80293668`:
`FnDeltaLerpChan`, `FnDeltaQuatChan`, `FnKeyLerpChan` and `FnKeyQuatChan`.
Each releases the previous-value buffer and DOF mask independently when non-null,
then invokes `FnAnimMemoryMap` destruction with a zero deletion flag. If the
incoming deletion flag's low bit is set, it finally calls `EAGLFree` with the
object pointer and size 36. The target installs the appropriate plain/keyed base
vtable before buffer cleanup. No additional derived fields are implied.

`DeltaChanDestruction.cpp` supplies scoped inline copies of the two already
reconstructed base destructor bodies so ProDG can inline inherited cleanup into
the four empty derived destructors. The out-of-line base bodies remain in
`DeltaChan.cpp`; their behavior must stay synchronized. This follows the native
compilation-fragment approach used for the channel and BoneMask interfaces,
not a recovered historical header boundary. The resulting object contains exactly
the four expected functions, with no generated allocated data, extra base
functions or discarded code. It uses the existing ProDG 3.8.1 animation profile.

Isolated PowerPC fixtures compare original and compiled destructors in 576 cases:
both nullable buffers, six deletion flags, signed virtual-call adjustments, and
memory-manager replacement after the first buffer free. They check call order,
the 36-byte object-free argument, object guards and preserved registers/stack.
External callbacks are stubbed; this does not test the game or allocator runtime.

## Event and allocation support

The event, bank, allocator, scratch-buffer and raw-pose support fragments add
35 functions and 4,028 code bytes. Each complete generated constant pool is
checked. Event traversal retains looping and cached-time behavior. CSIS records
have an eight-byte event, a twelve-byte dictionary prefix with variable metadata,
and a sixteen-byte parameter view. The original function reserves a 255-integer
parameter buffer. Raw event records occupy sixteen bytes after their eight-byte
channel prefix. Original handler accesses place the successor at zero and native
virtual pointer at four; no handler storage beyond the observed interface is
inferred. Both function wrappers have 24-byte extents established by deletion.

`AnimBank` describes only its observed twenty-byte prefix. Factories use the
original embedded object offsets: minus 24 for stateless formats and minus 16
for pose animation. They call through the established base interface. Compound
construction initializes the independently established 28-byte object. Type-ID
copies and destruction retain the observed halfword temporary.

Memory-pool fragments reconstruct cleanup, indexed free, usage and map-based
factory behavior. Embedded animation types bypass deletion; ordinary objects
retain the original virtual destruction followed by type-based free. The named
free-list storage has 26 pointers but its size table has only 25 halfwords; these
extents are deliberately not conflated. The manager deleting destructor passes
four bytes. Original pool storage and allocation bodies still outside the
accepted fragments remain external.

Scratch buffers have twelve-byte records and a named three-record global array.
Freeing decrements the reference count first and clears only the buffer pointer.
Raw-pose initialization preserves its two variable signature tables, native helper
addresses and invalid-type diagnostic. Its map prefix is sixteen bytes; the
function wrapper is twenty bytes, independently established by deletion. The
interpolated raw-pose evaluator remains original context; the completed frame
evaluator and six transformation helpers are described below.

## Stateless control interfaces

The stateless F3 and quaternion control fragments contribute ten functions each,
852 and 896 code bytes respectively. Each complete native 184-byte vtable and
8-byte conversion constant pool also matches. The larger evaluators remain
original context in the verified image; the Q evaluators now have
[maintained partial source](#partial-stateless-quaternion-evaluators).
A matching control fragment does not imply decoded format coverage.

Both function objects occupy 24 bytes, with previous-key halfword at sixteen,
FPS-enable/FPS bytes at eighteen/nineteen and mask pointer at twenty. The Q
constructor initializes the previous key; F3 does not. F3's serialized prefix
holds key count at sixteen and byte bone counts at eighteen/nineteen. Q has a
companion F3 function pointer at sixteen, then key/bone counts at twenty through
twenty-three. Q's bone enumerator forwards to that companion when present.
F3 frame-count conversion retains unsigned-halfword truncation; Q uses a signed
integer result. These differences are established by the original instructions.
All formats remain runtime untested.

### Partial stateless quaternion evaluators

`src/eagl_anim/FnStatelessQEval.cpp` maintains `EvalSQT` and `EvalSQTMask` under
`DECOMP_COMPARE`, inheriting the accepted Q control profile. Their original sizes
are 1,480 and 900 bytes; native output is 1,476 and 900 bytes, with the second
entry four bytes early. Objdiff similarity is 92.24% and 97.22%, respectively.
Both remain partial, with no exact or linked credit. The full 24-byte constant
pool at `0x802bf228` matches; there are no discarded functions or emitted vtables.

The existing 24-byte Q map is followed by four unsigned halfwords per animated
bone per key, then constant-bone data. Constant indices follow animated indices.
Each component expands to float bits as
`((value & 0x8000) << 16) | ((value & 0x7fff) << 15)`. The guarded helpers and
temporary union are reconstruction choices. The pinned NFS reference supplies
attributed names/interfaces, but its corresponding evaluator/format bodies are
empty; these bodies follow GR8E69 instructions.

Time selection preserves FPS scaling, cached forward/backward sparse-key search,
the implicit first sparse key at time zero, and the final-key interpolation gate.
Four decoded components are interpolated linearly without normalization or
shortest-path sign adjustment. Quaternion outputs occupy `sqt + 4 + 12 * bone`;
other pose components are preserved. The linked F3 evaluator receives the already
selected time/key/scale and its return value is ignored.

For masked constant bones, the packed-data pointer advances only when the bone
is selected. Skipping a constant bone therefore leaves its values for the next
selected constant bone. This surprising target behavior is preserved and checked
explicitly. Input key counts, cached indices and backing storage remain original
preconditions, not a validated file format.

Key-loop scheduling, the shared last-key comparison and register allocation remain
unmatched. [Batch verification](research/shortlist-partials.md) exercises sparse
and dense time selection, FPS mode, cache histories, masks, constant skipping,
shifted native entry addresses and the optional F3 call with isolated PPC checks.

## Skeleton poses and matrix callback

The skeleton fragment adds six functions and 3,916 code bytes, with its complete
four-byte constant pool. An adjacent private matrix callback adds 708 code bytes
and the original four-byte initialized `MatrixMultiply` pointer. The latter's
local function binding is scoped to the original `System.cpp` file record; that
scope is not used to infer ownership of unrelated global functions.

The serialized skeleton prefix occupies sixteen bytes, followed by 112-byte
bone records. Independent pose, skinning and mirroring loops establish the record
stride and its scale, signed parent index, quaternion, translation, signed mirror
index and 64-byte inverse base matrix. The prefix's first eight bytes remain
unknown storage; only its bone count and inverse-scale pointer are interpreted.
The implementation reuses the independently checked transform and coordinate
types. Reference headers supply attributed field and interface names; their empty
Skeleton implementation supplies no method bodies.

Mirroring preserves separate masked, unmasked, in-place and separate-buffer
paths. Paired bones exchange quaternion and translation components with the
original sign changes. Separate-buffer paths also copy scale xyz; neither path
silently initializes untouched pose slots. Nonlocal mirroring adjusts the root
quaternion and translation. Mask checks apply to the source bone.

Pose conversion retains the extra first-column scale, parent composition and
root assignment. Unselected transforms remain untouched. Still-pose generation
uses inverse bone scales when available and writes the original homogeneous
components. Skinning composes the current transform with the inverse base matrix,
then transposes the result.

The callback's argument order is output, first input, second input. Its arithmetic
uses a real 64-byte temporary before copying to the output, preserving aliasing.
It adapts the project's existing matrix expression sequence with the target's
operand order; it must not be substituted for the rendering helper's different
calling interface. No new imported implementation, compiler flags, generated-code
exclusions or runtime claims are introduced.

## Single-axis quaternion delta format

`FnDeltaSingleQ` reconstructs eight functions and 13,472 code bytes, plus its
complete 160-byte constant pool and native 192-byte virtual table. Native unused
function stripping removes the separately emitted inline `InitBuffersAsRequired`
body; its constants remain verified, and the discarded code receives no credit.
The compiler profile otherwise matches the existing animation units.

The function object occupies 48 bytes, as established by construction, deletion
and field accesses. Its sixteen-byte serialized map is followed by fourteen-byte
per-bone range records, then bins. Physical samples contain two bytes: the selected
axis component and quaternion w. Each one-byte delta holds two four-bit values.
Expanded ranges occupy 28 bytes, including the observed byte-sized axis index.
The record/header extents follow target strides rather than C++ padding guesses.

Initialization allocates three independent quaternion buffers. The destructor
frees all three only when the previous-sample allocation is nonnull. The reference
`GetArrays` interface returns range and bin pointers by reference; this reproduces
the observed access ordering without artificial memory barriers. Encoded angles
expand over minus pi through pi, and the two constant Euler components establish
pre- and post-alignment quaternions for the selected axis.

Backward requests reload the physical sample; forward requests accumulate
component deltas. Linear interpolation normalizes the result before specialized
quaternion products restore the constant alignment. Output uses the established
twelve-float pose stride and quaternion offset four. Masked evaluation preserves
its previous-key cache and selects physical, delta and output updates separately.
The generic `Eval` wrapper retains the original virtual call and null mask argument;
no unverified assumption of null-safe runtime behavior is added.

The reference implementation bodies are empty. Mathematical bodies are newly
reconstructed from the pinned instructions using the already checked scalar/vector
decoder structure. Helper names describe their operations and do not claim original
inline boundaries. Every generated allocated byte is checked; runtime behavior in
the game remains untested.

## Quaternion support and control fragments

### Maintained partial evaluators

The registered `src/eagl_anim/FnDeltaQFast.cpp` and `FnDeltaQ.cpp`
comparison units reproduce the earlier drafts against the pinned original. Their
four unfinished evaluators total 15,228
bytes, all still original context. Both fast evaluators differ at the same
reverse-playback guard: the draft selects CR0 where the original selects CR7,
changing three bytes per function. Both regular evaluators also differ in
instruction scheduling, for fifteen differing bytes each. Function addresses,
sizes and the full generated constant/vtable sections agree in these drafts.
These are maintained partial source, measured by `tools/comparisons.py` and the
public snapshot. They earn similarity credit, zero exact or linked credit, and
have not been tested in the game.

| Evaluator | Target bytes | Objdiff similarity | Differing bytes |
| --- | ---: | ---: | ---: |
| `FnDeltaQFast::EvalSQT` | 2,692 | 99.82169% | 3 |
| `FnDeltaQFast::EvalSQTMask` | 2,020 | 99.762375% | 3 |
| `FnDeltaQ::EvalSQT` | 5,040 | 99.42857% | 15 |
| `FnDeltaQ::EvalSQTMasked` | 5,476 | 99.47407% | 15 |

The full comparison units retain sixteen already-linked support functions as
explicit exact context, without double counting. Their complete pools/vtables
(224/184 bytes for Fast, 88/192 bytes for regular) match. `DECOMP_COMPARE` enables
the unfinished evaluator bodies and their inline helpers under the same ProDG
3.8.1 animation profile. No optimization settings change. The independent exact
fragments remain the source of linked code; a context drift fails comparison.

Regular `DeltaQ` comparison declarations add scoped packed storage views:
twelve-byte range records hold three unsigned halfword minima and ranges;
six-byte physical samples use a fifteen-bit x plus a sign bit for w, then
sixteen-bit y/z. Three-byte deltas use seven-bit x plus w sign and eight-bit y/z.
The decoded x/y/z determine w through a square root; if their squared length
exceeds one, the draft normalizes them and sets w to zero. Interpolation normalizes
the result. These operations and access strides follow the target instructions,
not an implemented NFS reference. Helper boundaries and historical field names
remain unproven, and the bitfield views are compiler-specific.

The masked regular evaluator intentionally tests constant-bone selection through
`mBoneIdxs`, writes through `mConstBoneIdxs`, and invalidates `mPrevKey` afterward.
The existing wrapper still passes a null mask to the masked virtual method.
Do not silently change these observed quirks or infer safe input ranges from a
high similarity score. Tests here verify native output and reporting safeguards;
they do not establish runtime safety for arbitrary animation data.

ProDG 3.5, 3.5b140, 3.7, 3.8.1 and 3.9.3 reproduced the same differences.
Equivalent guard rewrites and tested whole-file optimization variations did not
resolve them while preserving the rest of each unit. A compiler-version switch
alone is therefore not the next useful experiment. Investigate the condition's
source lifetime and surrounding cache logic; retain complete comparisons and
avoid register constraints or instruction patches. The reference quaternion
implementation bodies remain empty, so they cannot supply the missing fix.

### Accepted support

Three `FnDeltaQFast` fragments add ten functions and **4,104 code bytes**:
map binding and buffer initialization, unmasked next-key reconstruction, three
masked delta/next-key helpers, construction/destruction, length lookup, the generic
`Eval` wrapper and animated-bone enumeration. Their three complete constant pools
occupy 24 bytes at `0x802bebf0`, 24 at `0x802bec28` and 80 at `0x802bec60`.
The complete native virtual table occupies 184 bytes at `0x802ec948`.

These are independent source fragments, not a completed decoder translation unit.
The two main quaternion evaluators still have unresolved compiler differences and
remain original context, with zero exact or linked credit; their partial source is measured above. The masked/control fragment's
native link discards the separately emitted inline `InitBuffers` body. That helper
is used by the accepted map-binding fragment; its generated constant pool is fully
retained and compared in the masked/control fragment. No mismatching target
function is compiled and then stripped, and no pool or padding is clipped. The unfinished accumulation helper bodies are enabled only for comparison builds.

The `FnDeltaQFast` object has 64 bytes and type value 18, supported by constructor,
deleting destructor and field accesses. It allocates one block containing a
32-byte expanded range record and two 16-byte quaternion slots per animated bone.
The recorded previous/next block pointers point inside this allocation. Destruction
frees the range-block base once; it does not free those interior pointers.

The encoded map's records begin at byte 18, independently of the C++ header's
rounded size. Each encoded range record is sixteen bytes: four unsigned halfword
minima and four ranges. Minima expand with `2/65535 * value - 1`, ranges with
`2/65535 * value`. Six-byte physical samples hold four twelve-bit components: the
first three occupy the upper twelve bits of successive halfwords, and w is
assembled from their three low nibbles. Each component expands over [-1, 1].
Three-byte delta samples use the upper six bits of each byte for x/y/z and combine
the three low two-bit fields for w. Per-component deltas use the expanded minimum
and range with a `1/63` factor. Bitfield declarations describe the verified SN
compiler's target layout, not a portable serialization interface.

Next-key reconstruction reloads a physical sample when crossing a bin boundary,
otherwise adds one decoded delta to the previous quaternion. It updates the cached
next key afterward. Masked variants update only selected bones while advancing the
encoded sample position for every bone. Forward and reverse accumulation preserve
the target's component order and frame/bone traversal. These support routines add
no normalization or interpolation behavior beyond their matched instructions.

A separate `FnDeltaQ` control fragment adds six functions and **636 bytes**, plus
an eight-byte conversion pool at `0x802bed70` and the complete 192-byte virtual table
at `0x802ecab8`. Construction and deletion establish its 48-byte extent and type
value 17. Its encoded range/sample types remain opaque in exact builds; comparison mode
adds the scoped views above. The two main evaluators and buffer initialization
remain original context in the verified image. The
wrapper calls the protected virtual masked evaluator with a null mask, preserving
the original call without asserting that this is a safe runtime entry point.
Both decoder families enumerate animated bone indices and derive length from the
key count or the final stored key time. Existing valid-map/count preconditions are
preserved.

The reference headers supply attributed names and interface leads, but their
relevant implementation bodies are empty. These bodies were reconstructed from
the pinned GameCube instructions with Codex assistance. Inline helper boundaries
and historical member names remain unproven. The normal ProDG 3.8.1 animation
profile matches every generated allocated byte; no new compiler options, register
constraints, assembly bodies or instruction patches are used.

## Raw state decoding

Two fragments add four `FnRawStateChan` functions and **560 code bytes**: its
packed-field decoder, deleting destructor, length lookup and generic `Eval`
wrapper. The control fragment also verifies its eight-byte conversion pool at
`0x802beb98` and complete 184-byte native virtual table at `0x802ec720`.
The separately emitted inline metadata accessor is natively stripped; it generates
no extra constant pool and is used in the accepted decoder. The main `EvalState`
and `FindTime` implementations remain original code. In particular, the original
backward-search path at `0x801edb4c` stores the key pointer to a stack slot and
loads those bits as a float, rather than loading the pointed-to timestamp. That
observation is an unresolved reconstruction lead, not accepted evaluator source
or a proposed behavior correction.

Deletion establishes a twenty-byte function object. Independent inspection of
the original evaluator identifies the cached key index at `+0x10`. The map view records frame count at `+4`, field count at `+8`,
and a variable array of halfword field descriptions beginning at `+0x0a`.
The key-count and key-size fields follow the attributed reference and independent
inspection of the original evaluator; they establish no complete allocation or
portable file format. `State` and `StateTest` remain opaque.

Each field description uses bits 15–13 as a stored-width exponent, bits 12–11
plus one as the destination byte width, and its low byte as a destination offset.
Bits 10–8 are not interpreted by this decoder. Exponents 0, 1 and 2 extract one,
two or four bits from the current source byte, most-significant bits first. After
eight consumed bits the source advances and the bit counter resets. Exponents
3, 4 and 5 read one, two or four bytes directly. The decoder writes only destination
widths one, two and four. Other exponents retain the previous decoded value;
width three produces no write. It does not insert alignment fixes, clear the
output record or add bounds checks. The original map and destination-storage
preconditions remain the caller's responsibility.

The generic wrapper forwards to the original virtual `EvalState`, and length
lookup converts the unsigned frame count. The CC0 `RawStateChan.h` supplied
interface names; its decoder/evaluator bodies were empty. Implementation and
packed-field evidence come from the pinned target. Complete-image comparison is
required, and runtime behavior remains untested.

## Raw-pose, cyclic and linear channels

Six raw-pose transformation helpers and the frame evaluator add 2,380 code bytes.
Each complete generated constant pool matches. Quaternion/translation copies
advance the input pointer per component. Interpolation retains the reference's
quaternion sign selection and normalization, and the target's translation math.
Euler conversion follows the original half-angle trigonometric calls and degree
conversion. The original 28-byte `qt0` global remains external storage.

The frame evaluator consumes a count and patched helper addresses from each
signature record. Masked-out channels advance their input by three or four floats
according to the helper, without writing output. The larger interpolated evaluator
has a code-exact research candidate but an unresolved complete constant pool; it
is excluded from accepted source and progress. No padding is removed to accept it.

The cyclic wrapper adds six functions and 1,460 bytes. Four evaluators agree on
its 28-byte extent and start/end/length/child fields, forwarding through the
established virtual interface. Time wrapping retains signed truncation and the
original unusual below-start expression. Four independent conversion pools are
checked; its constructor and virtual table remain original context.

The raw-linear wrapper adds four functions and 708 bytes. Its deleting destructor
establishes twenty bytes, including the four-byte interpolation flag. The map has
an eight-byte prefix with halfword DOF/frame counts, an index array aligned to an
even halfword count, then float frame data. Clamping, interpolation and per-element
count reloads are preserved. The two complete conversion pools also match.
Reference method bodies for these wrappers are empty; their implementations are
reconstructed from original instructions using attributed interface names.

## Run and pose blending fragments

Seven run-blender functions add 3,940 code bytes and verify 72 constant bytes.
They cover destruction, pose evaluation, weight changes, velocity alignment and
blending, root-quaternion computation and root alignment. The 128-byte object
extent follows its deleting destructor; repeated accesses establish its arrays,
four child evaluators, timing values and alignment quaternion. Unproven storage
at `0x6c` remains unknown, and the owned allocation at `0x7c` is an untyped pointer.
The associated phase-channel declaration describes only its observed twelve-byte
prefix. Remaining timing, facing and initialization methods stay original context.

Weight changes preserve adjacent-channel reuse and optional velocity channels.
Pose evaluation retains the original use of unmasked child calls, scratch storage,
and cycle/root alignment even though its public signature accepts a mask.
Quaternion multiplication uses ordinary float-array inputs, preserving the target's
component reloads where arrays may overlap. There are no register constraints or
special compiler flags.

The static pose-blending function adds 1,048 bytes and its complete eight-byte
constant pool. It blends and normalizes quaternions, linearly blends translation,
and preserves scale and untouched pose components. Masked paths skip unselected
bones. The shared header exposes this static interface only; the pose-blender
instance layout and larger transition evaluators remain unreconstructed.

The quaternion blending helper retains CC0 `AnimUtil.h` attribution. The original
run/pose implementation bodies are not supplied by that reference; target
instructions establish the reconstructed control flow and pointer behavior.
All these fragments require complete-image verification and remain runtime untested.

## Turn blending and additional run controls

The turn-blender fragments reconstruct all fifteen `FnTurnBlender` methods
identified by the original symbols, totaling **4,796 code bytes**. The main
fragment spans `0x80202a48` through `0x80203c4b`; the separate cycle-time helper
occupies 184 bytes at `0x80203c4c`. Their complete constant pools are 288 bytes at
`0x802bf0f8` and eight at `0x802bf218`. The main fragment also reproduces the
160-byte native virtual table at `0x802ed490` and the four-byte initialized
function-static diagnostic counter at `0x802d3314`. The counter's source identifier,
local binding, address and size agree with the original `i.1497` object in the
`FnTurnBlender.cpp` symbol scope; the compiler's numeric suffix is not a recovered
source name. None of this generated data earns code credit.

The constructor, deleting destructor and repeated accesses establish a 92-byte
function object. It contains an owned animation-pointer array at `+0x0c`, two
borrowed child pointers at `+0x10`, weight at `+0x18`, count/index at `+0x1c/+0x20`,
skeleton at `+0x24`, paired cycle lengths and offsets at `+0x28/+0x30`, frequency
at `+0x38`, previous time and time offset at `+0x3c/+0x40`, cycle index at `+0x44`,
alignment quaternion at `+0x48`, and the target compiler's four-byte boolean at
`+0x58`. Type value nine is stored by construction. Destruction releases scratch
buffer one when the animation count is nonzero, frees the pointer-array allocation
when present, and leaves the borrowed children alone. Constructor-uninitialized
fields and valid-animation/count preconditions remain unchanged.

Weight changes choose adjacent animations, reuse an existing child pointer when
moving to a neighboring pair, and update frequency while maintaining the time
offset. They clamp the pair index, without additionally clamping the fractional
weight. Calls and accesses independently identify run-blender children where their
frequency, time offset and endpoint root quaternions are used. The new
`FnRunBlender::GetOffset` inline accessor reads the already established `+0x50`
field; its name follows the reference header. This does not make every arbitrary
`FnAnim` child a valid run blender.

Pose evaluation uses scratch buffer one and passes null masks to both children,
including when its own caller supplied a mask. Velocity blending preserves the
weighted vector length after directional interpolation. Both evaluations map
through twice each child cycle length, subtract the child offsets and apply cycle
alignment. Facing blends use quaternion sign selection and normalization, rotate
the original `(0, 1, 0, 1)` vector, and return its x/z components. The first alignment
call initializes an identity quaternion and cycle index minus one; later changes
compose the begin/end-facing rotation, reversing its y component for an adjacent
backward step. No extra clamping, normalization or zero-length checks are added.

The cycle helpers preserve signed truncation, the original nonnegative result for
below-start cycle-index requests, and the distinction between the `< start` and
`>= end` branches. Their shared delta and integer temporaries express the target's
calculation order. `EvalPhase` returns false in the original; it is not a temporary
stub. Original velocity, facing and alignment diagnostic prints, formatting,
argument order and the incrementing counter are retained.

The main turn fragment natively strips only the separately emitted inline
`ComputeAlignQ` helper, which is used in its accepted cycle-alignment method and
has no original standalone function symbol. All constants emitted for that helper
remain in the fully compared pool. No mismatching target method is compiled then
stripped, and no data or padding is clipped. The helper's historical inline boundary
is unproven; it is a descriptive reconstruction supported by the reference interface
and observed calculations.

Four additional run-blender fragments add nine functions and **1,520 code bytes**:
constructor, generic evaluation wrapper, both cycle helpers, the original false
phase result, frequency lookup, begin/end root-quaternion wrappers, and facing
blending. They verify complete pools of 16 bytes at `0x802bf0c8`, four at
`0x802bf0ec`, eight at `0x802bf0f0`, and 24 at `0x802bf09c`, plus the complete
160-byte run-blender virtual table at `0x802ed3f0`. The control fragment natively
strips only the separately emitted `GetOffset` inline accessor used by turn
blending; it adds no data. The constructor confirms type eight and the existing
128-byte layout. Endpoint wrappers sample zero or each phase map's final frame.
Facing evaluation retains its scratch-buffer-zero use, separate pose copies and
original failure returns. Run-blender phase search, cycle alignment and velocity
evaluation remain original context; matching research fragments do not give those
methods source credit.

These bodies were reconstructed from the pinned GR8E69 target with Codex
assistance. The pinned CC0 nfsmw headers supply attributed interface/member names,
but their relevant implementation bodies are empty. Quaternion blend math retains
its existing `AnimUtil.h` attribution; rotation expressions reuse this project's
checked transform reconstruction with target operand order. ProDG 3.8.1 and the
normal animation flags match every accepted generated allocated byte. Complete
local image verification is required before publication; runtime remains untested.

## Character skeleton lookups

`EALA::Character::Skeleton`'s joint lookups forward to its `SkeletonData` when the pointer at `+0` is
set. The view is in [`CharacterSkeleton.h`](../include/game/CharacterSkeleton.h). The test as a
`HasData` inline is inferred from the materialized boolean in each body; that name is descriptive.

`GetStillPose` and `StorePose` go through the word at `+12` of `SkeletonData`, an `EAGLAnim::Skeleton`; the
member name is inferred. `GetJointIndex` maps the two `TYPE_JOINT` values it tests to themselves and anything
else to -1; only the type name is recovered.

## Character task tree

`EALA::Character::Task` ([`CharacterTask.h`](../include/game/CharacterTask.h)) is a reference-counted
`Resource` that is also an `ISubject`, with a weak pointer to its parent task, a counted pointer to its
`State`, a substate index and a core pose. The virtual table order, member offsets and every method name come
from the image; member names are inferred from use. The constant 99 stored as the initial core pose acts as
"no pose": `StateIterator::Set` will not look for an authored transition when either side has it.

A parent-less task's messages go to the current `Choreographer`. `StateIterator` keeps one child task and,
when the child stops being busy, asks the state for the next substate; after eleven attempts it installs a
`Sentinel`. `DumpPrefix` writes its indent spaces after the four-character `<!> ` tag but puts the terminator
at the unshifted index, so the printed prefix is shorter than the indent it wrote: that is the target's
behavior and is kept.

`EALA::Resource`'s destructor is inline in its class because every derived destructor inlines it, while its
out-of-line copy is a separate weak function; `ResourceTypes.h` is a `#pragma interface` header so that users
do not emit that copy. `StateIterator`'s pool allocator and its size live in the file's anonymous namespace,
whose compiler-generated symbol names cannot be spelled in source, so `operator new`, `operator delete` and the
static initializer are not reconstructed yet. Both classes' destructors are also out: defining them emits their
secondary `_vt.X.8ISubject` tables, which the unit manifests cannot own yet. `StateIterator::Set(unsigned int,
float)` stays out too: the source form has its argument address loads one block later than the image.

`Choreographer` runs a map from skeleton partition to context (a counted task pointer, the `EALA::Task` to run
when that task sends a message, an enabled word and a float) and passes rotation, duration and cycle-end events to its
`ChoreographerClient`. Its context stack is in an anonymous namespace, so `Current`, `EvalPose`, `SetState` and
the string form of `SetAuthoredTransitionState` wait for the same reason as the pool allocators. `State` keeps a
`Substate` pointer per index after a 40-byte header, a name map, and a counted loader pointer; the `State`
methods here come from the image and member names are inferred. `Operation`'s virtual order comes from its table.
