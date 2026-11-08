# EAGL rendering reconstruction

The rendering reconstruction covers material drawing, transforms, render state,
property parsing and texture loading. All rendering code was reconstructed from
the pinned target; generated data is checked separately and earns no code credit.
Current totals are recorded in the verified progress snapshot.

The first unit reconstructs `EAGL::TevStage`: 15 functions and
1,744 executable bytes, plus 1,488 initialized data bytes and a four-byte
constructor-table entry. Its unit manifest is
[`eagl_tevstage.json`](../config/GR8E69/eagl_tevstage.json). Data earns no code credit.
The work used AI assistance to reconstruct C++ from the pinned GR8E69 executable.
It imports no EA rendering implementation from another project.

## Evidence and scope

The original `tevstage.cpp` file record scopes its two compiler-generated local
initialization functions. Exported method and static-object names are preserved.
Member names and the present header organization are descriptive reconstructions;
original spelling, visibility and exact historical compiler release are unknown.

The setters, constructor and `SetHardware` independently agree on the following
92-byte storage layout. `Use` copies 92 bytes, material callers index stages in
92-byte increments, and the original 1,472-byte array holds 16 stages.

| Offset | Observed storage and use |
| --- | --- |
| `0x00`–`0x0c` | Four alpha inputs consumed by `GXSetTevAlphaIn` |
| `0x10`–`0x1c` | Four color inputs consumed by `GXSetTevColorIn` |
| `0x20`, `0x24`, `0x28`, `0x2c`, `0x30` | Alpha operation, bias, scale, byte clamp and output register |
| `0x34`, `0x38`, `0x3c`, `0x40`, `0x44` | Color operation, bias, scale, byte clamp and output register |
| `0x48`, `0x4c`, `0x50` | Texture map, texture coordinate and color channel |
| `0x54`, `0x58` | Constant color and alpha selectors |

The clamp bytes have ordinary three-byte alignment padding. The compiler copies
that padding in aggregate assignment, as the original does; no arbitrary padding
contents are assumed. Setter symbols preserve the GX enum parameter types.
The byte active-stage count and three four-byte booleans agree with both original
object sizes and their load/store instructions. Initial data sets the count and
dirty flag to one and both overbright flags to zero.

`SetHardware` uploads dirty state, masking the texture map's top bit. The last
active stage optionally increases one-times scaling to two-times and two-times
to four-times for color and alpha separately, without changing stored scales.
`ResetToDefault` constructs a default stage, copies it to all 16 slots, adjusts
stages 1–15 to pass through the configured constant inputs, and activates one
stage. No bounds checks have been added to the original indexed interface.

## Compilation and generated storage

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces this
unit. GNU interface/implementation pragmas retain the out-of-line inline-method
copies and allow constructor/reset inlining. The compiler generates both static
initialization functions and the constructor-table pointer; none is hand-written
assembly. The named stage array is explicitly placed in `.data`, matching its
original section, address and size. Its 1,472 initial zero bytes and subsequent
constructor loop are both compared, together with every other generated section.

The GX enum and function declarations come from the already attributed
[dolsdk2004 headers](Dolphin.md). `include/dolphin-sn/dolphin/types.h` is their
unchanged scalar typedef subset, excluding CodeWarrior libc includes so SN game
code can use the SDK interface. Its attribution remains
[`src/dolphin/NOTICE`](../src/dolphin/NOTICE); it grants no additional license.
No SDK implementation or new library-byte credit is introduced here.

Strict unit checks verify complete allocated sections, function symbols and
external dependencies. Complete-image verification and snapshot capture remain
required before publication. Runtime rendering has not been tested. This unit
establishes no broader `GeoPrim`, material, render-context or PCodeData layout.

## Material drawing fragments and supporting views

Twenty `CompBlock003` fragments add 49,044 matching code bytes. They cover the
mesh and CPT Color, Texture1, Texture1Lightmap, Texture2, Texture2Lightmap,
Falloff_Texture1 and Falloff_Texture2 variants; both
Texture1Envmap and Texture2Envmap variants; and both Falloff_Texture1Envmap
variants. Names describe the original entry points, not a promise that every
variant's rendering path is complete. Four Falloff_Texture1/Texture2 `CompBlock004` variants add 11,704 bytes.
Their behavior matches the corresponding block 3 routines, verified independently
at each original address. Other block 4 functions and startup routines remain original context in the
verified image. Eight mesh-lighting variants now have maintained partial source
as described below.
Twenty outer mesh/CPT `Render` fragments add another 20,280 bytes. Each retained local function is
scoped to its original file record and binding. Each generated constant pool is
compared in full; no pool is counted twice.

Three texture-coordinate cache functions add 228 bytes. Their setters and reset
loop establish eight texture generators, with type/source/matrix arrays at
`0x00`, `0x20` and `0x40`, eight normalization bytes at `0x60`, and post-matrix
indices at `0x68`. The original named cache is 136 bytes. The cache and generator
count remain external original data. Subsequent constructor, destructor and
instance-method work establishes an 84-byte `TARExtension` at offset four of the
88-byte TAR object; see the shared state/property/TAR evidence below.

Seventeen transform and matrix functions add 3,488 bytes, including both transpose
overloads, matrix multiplication, point transforms, identity/scale construction,
quaternion/translation and scale/quaternion/translation construction, matrix
composition and orthogonal inversion. Static transpose handles aliasing through
a local 64-byte matrix. The point-transform overloads use separate 12-byte and
16-byte temporaries when their input and output references coincide. Their
aggregate copies and component accesses support the `COORD3` and `COORD4` storage
views; these types are distinct from the still-opaque game `CVector3`.
`MATRIX4` and `Transform` agree on sixteen floats and 64-byte copies. Field names,
helper names and header organization remain descriptive. Arithmetic expression
order, including fused operations, is preserved and all generated constants are
checked. `BuildQuatTrans` and `ExtractQuatTrans` remain original context;
`PrependQuatTrans` calls the former through its observed interface.

`EAGLMaterial.h` deliberately supplies storage views rather than complete game
classes. The PCode interpreter independently establishes eight-byte parameter
slots and their value pointers at offset four. Material functions use those
slots for transforms, textures, state and scalar inputs; unused slot words retain
an unknown label. The 192-byte `PCodeData` storage is independently bounded by twenty callers:
each reserves a record from stack offsets `0x08` through `0xc7`, followed by a
separate color temporary. `ProcessPCode` uses its final word at `0xbc`, initializes
the execution flags, and clears the accessed scratch arrays. Render callers
initialize its configuration pointers before passing it to that interpreter.
The twenty named private `staticdata` objects independently have 116-byte sizes.
The declarations now cover those storage extents, retaining unused regions as
unknown bytes; original constructors, field names and unused field types are
unconfirmed. `GeoPrim` remains opaque. The render-context extension is accessed
through its observed `0x10` offset, without inventing a complete context class.

The drawing code preserves the original batch rounding, triangle-strip overlap,
byte and halfword indices, cache flush, and FIFO padding. SN's native `address`
attribute maps a volatile graphics-FIFO union to `0xcc008000`; it reserves no RAM
and inserts no hand-written instruction bodies. Original instructions and the
existing SDK FIFO declaration independently support this address and port widths.

Implementation inline bodies for the TevStage unit are separated from its
material-facing declarations to retain the original callers' out-of-line calls.
These are GNU/SN compilation fragments verified with this toolchain, not an
established portable C++ whole-source build. The original header organization,
source spelling and visibility remain unknown. All class storage declarations
are shared; no alternate TevStage layout is used by material callers.

## Partial mesh-lighting blocks

Four `CompBlock004` mesh functions are maintained under the
[partial-source workflow](Progress.md#partial-source-workflow). They reconstruct
normal-matrix setup, three directional-light inputs, ambient color, material and
texture state, and the display-list/array paths. They add **13,380 target code
bytes of partial source**, with no new exact or linked credit. The original
functions remain in the byte-identical verified image.

| Variant | Source | Original entry | Target bytes | Objdiff similarity | Differing bytes |
| --- | --- | --- | ---: | ---: | ---: |
| Color | [source](../src/eagl/eagl_msh_color_slots_lit.cpp) | `0x801c61c8` | 2,944 | 99.45652% | 16 |
| Texture1 | [source](../src/eagl/eagl_msh_texture1_lit.cpp) | `0x801c7884` | 3,164 | 99.469025% | 22 |
| Texture2 | [source](../src/eagl/eagl_msh_texture2_lit.cpp) | `0x801cafb4` | 3,636 | 99.53796% | 22 |
| Texture1Lightmap | [source](../src/eagl/eagl_msh_texture1_lightmap_lit.cpp) | `0x801c92e8` | 3,636 | 99.53796% | 22 |

The current measurements live in the public snapshot; these figures document the
initial maintained versions. Each manifest preserves the original **local**
function binding and source-file record. It reuses its corresponding accepted
block-3 compiler profile (ProDG 3.8.1, `-O2 -G0 -fno-exceptions
-fno-implicit-templates`) and adds only the standard `DECOMP_COMPARE` body guard.
No existing linked function or storage is counted again. The complete sixteen-byte
constant pool for each comparison matches at `0x802ba0ac`, `0x802ba268`,
`0x802ba720`, or `0x802ba4a8`, respectively. All external definitions resolve to
the pinned original. No renderer implementation is imported from another project.

The shared `EAGLMaterialLighting.h` declares only the accessed storage prefix:
three sixteen-byte direction records at zero, three sixteen-byte color records at
`0x30`, and three ambient floats at `0x60`. The fourth word of each color record
is unused here and retains unknown byte storage; the declaration does not infer
an alpha field, constructor, complete allocation or historical type name.
Directions use the independently reconstructed `COORD4` transform interface.
The view copy zeroes its translation and sets its final element to one before
transforming each active direction. A light is active if any RGB component is
nonzero; colors are multiplied by 255 and converted without adding a clamp.
The transformed direction is scaled by 1,048,576 for `GXInitLightPos`. Light-mask
bits progress through 1, 2 and 4. Ambient and light alpha are set to 255.

In the textured variants, the target reuses the light-object/direction temporary
area for a later texture matrix. Limiting those lighting temporaries to their
observed use reproduces the original **392-byte stack frame** and complete
function lengths, improving the old drafts from roughly 97.5% similarity. Color
retains its original 480-byte frame. These are reconstructed lifetimes, not proof
of original brace placement. Fog, texture-dirty decisions, TEV stage ordering,
triangle-strip overlap, array advancement, byte/halfword indices, cache flushing
and FIFO padding remain as observed; this work adds no runtime fixes.

The remaining differences are independent argument-setup instruction pairs:
two pairs before `TransformPoint` in Color, and those pairs plus one before the
third `GXLoadLightObjImm` in each textured variant. Focused original-dependent
tests check complete native output, original function/frame sizes, and that every
remaining instruction difference is an adjacent swap of independent `mr`/`addi`
argument writes before an unchanged direct call. They also reject an internal
branch entering halfway through a swap. All other instructions agree exactly.
This is static comparison evidence, **not an emulator or rendering test**.
Investigate source expression/scheduling choices next; do not pin registers,
patch instructions, or turn these near matches into exact credit.

The CPT block-4 variants use a different indexed point-light path and paired-single
stores. They are not registered by this change and must not be cloned from these
mesh routines without independent reconstruction.

### Additional lightmap and envmap lighting blocks

Four more `CompBlock004` mesh entry points add **13,884 target bytes of partial
source**. Each retains the original local binding and file record, reuses its
accepted block-3 compiler profile, and has a separate comparison manifest. The
original functions remain in the verified image and earn no new exact or linked
credit. This completes maintained source for the eight directional-lighting mesh
block-4 variants; the two mesh falloff Texture1/Texture2 block-4 variants were
already linked and verified.

| Variant | Source | Original entry | Target bytes | Initial objdiff similarity | Differing bytes |
| --- | --- | --- | ---: | ---: | ---: |
| Texture2Lightmap | [source](../src/eagl/eagl_msh_texture2_lightmap_lit.cpp) | `0x801ccf74` | 4,392 | 99.617485% | 22 |
| Texture1Envmap | [source](../src/eagl/eagl_msh_texture1_env_lit.cpp) | `0x801cede4` | 3,164 | 99.469025% | 22 |
| Texture2Envmap | [source](../src/eagl/eagl_msh_texture2_env_lit.cpp) | `0x801d0670` | 3,164 | 99.469025% | 22 |
| Falloff_Texture1Envmap | [source](../src/eagl/eagl_msh_falloff_texture1_env_lit.cpp) | `0x801d557c` | 3,164 | 99.469025% | 22 |

The shared lighting view, channel setup and active-light tests agree with each
original routine independently. Scoping the light object and transformed direction
before the later texture matrix reproduces all four original **392-byte frames**
and complete function lengths. Every sixteen-byte constant pool is checked in
full at `0x802ba9b4`, `0x802bac38`, `0x802bae58` and `0x802bb504`, respectively.
The constants are 1, 0, 255 and 1,048,576 in that order. No new layout declarations,
compiler flags, imported implementation or verification-tool changes are needed.

Texture2Lightmap preserves the float-to-integer mode test against six, both
four-stage TEV configurations, and three separately dirty texture bindings and
texture matrices. Its state, textures, mode and matrix pointers come from parameter
slots 10, 11–13, 14 and 16–18. The three envmap-named blocks use state slot 8,
texture slot 9, fog slot 10 and texture-matrix slot 12. Their names are original
symbols; they do not justify adding a second texture upload, an environment-matrix
calculation or an extra falloff light to these particular bodies. Those paths
are absent from the inspected instructions. The three bodies have equivalent
instruction sequences after accounting for their own function, constant-pool and
direct-call addresses; they remain separately checked original ranges.

In each new routine, only three adjacent instruction pairs remain reordered:
argument setup for the first two `TransformPoint` calls and the third
`GXLoadLightObjImm` call. All other instruction words agree exactly. The existing
static mesh-lighting tests now cover all eight partial functions, including full
native output, target/frame sizes and independence of every reordered pair.
No internal branch enters the middle of a pair. Runtime rendering remains untested;
the remaining compiler-scheduling differences are still partial matches.

## Render dispatch and state caching

The outer fragments initialize PCode configuration, reset graphics state when
requested, update cached vertex index widths, configure position/color/normal/UV
formats, bind arrays, dispatch blocks 3 and 4, and undo variation-pointer offsets.
Their compact six-byte variation records retain the observed unaligned word read
at offset two. The no-work dispatch cases use an inline empty helper to preserve
the compiler's switch lowering; the helper name and source grouping are
reconstruction choices, and earn no code credit. CPT also spells out the observed
no-work case 5. The comparison verifies every branch, including default behavior.

Private state is referenced through native symbol-name declarations such as
`__asm__("staticdata.1302")`. These name external objects; they contain no assembly
instructions or data definitions. Manifests bind each private state and drawing
callee to its original file record. Original state storage and the class's global
configuration variables remain uncredited binary context.

The falloff fragments configure a second color channel and light 7 at the origin,
then use that channel in extra alpha-combining stages. Their shared white-color
helper is descriptive source, not an imported EA implementation. Lightmap mode
selection preserves the original float-to-integer conversion before comparing
against six. No rendering behavior has been tested in an emulator.

The CPT Texture1Envmap block configures three TEV stages, clamps and filters its
texture, and builds the normal-based texture matrix from the transposed model
matrix plus scale and translation matrices. `CLAMP_MODE_0` and `FILTER_MODE_2`
record observed numeric arguments; the original enum member names are unknown.
The CPT Texture2Envmap block uses the existing texture stages and loads two TARs.

## Single-draw, particle and skin dispatch

Three additional drawing blocks add 5,324 bytes: `SingleDrawGouraud`,
`SingleDrawTexture` and `Particle`. Their state setup runs only when the PCode
reset flag is set. The single-draw variants disable enabled fog; the particle
block retains the existing fog state. Texture use remains conditional on its
dirty flag. Their display-list and array paths retain the same verified batching
and FIFO behavior as the material family.

Five outer render functions add 3,872 bytes: those three variants plus
`Moh3_Skin_LitTextureObjectFog` and `Moh3_PlayerSkin_LitTextureObjectFog`.
They call block 0 directly. The skin variants additionally enable direct position
matrix indices. Their lighting block bodies remain original context.
The single-draw functions bind arrays through original `gVolatileData` pointer
arrays, whose two/three-element declarations agree with their 8/12-byte symbols.
Their default PCode arrays and last-source arrays are likewise bounded by the
original named objects. These external objects earn no source credit.

## State-name lookup

`StringToPlatformStateEnum` and `StringToCommonStateEnum` add 4,096 code bytes
and 2,760 checked string-table bytes. They first classify each name by its prefix,
then compare full strings and return the observed numeric value. The original
strings include aliases, negative primitive-type values, and the error messages;
invalid values retain the original diagnostic call and zero result. The two
functions cover 79 platform names and 47 common names. The declared integer
return represents the observed ABI; the original return-type spelling is unknown.
Neither the strings nor the reconstructed lookup tables establish complete
historical enum declarations.

## Shared state, properties and texture loading

`GeoPrimStateExtension` now has a shared 60-byte storage view, supported by its
constructor, setters, full-record copies in `Use`, and the named current-state
object. `GeoPrimState` contains the extension at offset zero: the destructor's
member-destruction ABI flag distinguishes this from base-class destruction.
The extension fields retain descriptive names. Its custom blend fields are not
initialized by the default constructor, matching the original.

State use retains the original cache invalidation, culling and depth overrides,
blend/alpha tests and alpha-write override. Initial invalidation copies the state
and increments every byte of the cached record; it is not replaced by clearing
memory. The vertex-format cache configures the observed 22 attribute bits.
Unsupported primitive values and no-op shading/texture/chroma setters retain
their original failure results. Global cache storage remains original context.

The property dispatcher and runtime state/texture constructors share a recovered
parser. Twelve-byte property records contain a name, argument count and argument
array. Sixteen-byte parser locals contain the property count/array and copied
text length/pointer. Parsing recognizes `=`, `,` and `;`, preserves its original
termination behavior and frees both argument arrays and copied text. The
four-byte argument wrapper is descriptive; its original name is unknown. Its
zero initialization, native array cookies and sized cleanup are verified rather
than replaced with raw object bytes. State/texture argument quirks remain intact,
including the state loader's indexed vertex-format arguments and its repeated
first argument for the fractional format field.

The four-byte `Colour` view is independently supported by by-value argument
copies and the Gouraud vertex array's allocation, four-byte stride and copies.
Its packed member name does not establish channel order or the original union.

Texture constructors, copy/destruction, setters, upload and property loading
establish an 88-byte `TAR` containing its 84-byte extension at offset four. The
copy constructor copies the extension only; the unknown leading word is retained.
The runtime loader rounds allocation/element spacing to 96 bytes. That spacing
is distinct from the object size passed by the deleting destructor. Texture and
palette objects use the existing attributed GX declarations.

Only the first sixteen bytes of a variable-sized `SHAPE` are described by the
header view. Width/height accesses, format bytes, flags and inline/offset pixel
access agree across initialization, clamping, palette upload and drawing. The
original fallback shape and symbol pool remain external context. The texture
loader preserves its symbol lookup and fallback behavior, including the CLUT
lookup path that discards the secondary lookup result. Reconstructing that path
is not a runtime bug fix.

Texture initialization and palette-format switches use inline empty helpers for
the observed no-work cases, as the existing material dispatch does. These retain
native switch lowering without adding instructions or hiding comparisons.
Mipmap filtering preserves context overrides, palette restrictions, anisotropy
selection and the original LOD-bias bounds. Generated property strings, diagnostics
and float/double constants are all compared in full. Runtime loading and rendering
remain untested.
