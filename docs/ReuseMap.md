# Reuse index and next work

Use the code map to find candidates, then use shared-library references and dependency
evidence to choose work. A symbol-name match is a research lead; only complete generated
code/data and full-image verification establish accepted source coverage.

## References that have already produced matches

| Family | Reference | Accepted code bytes | Practical lesson |
| --- | --- | ---: | --- |
| Dolphin SDK | [dolsdk2004](https://github.com/doldecomp/dolsdk2004/tree/2328b4164b1a98422a2255d83ce9a5a7548990cc), [Prime](https://github.com/PrimeDecomp/prime/tree/693fa74f9a9a7cdbb97f09a09d30853415e40927), older GX reference | 166,200 | Recover the game's older revision and storage layout; a later SDK is a reference, not a drop-in replacement. |
| Lua | [Lua 4.0.1](https://www.lua.org/ftp/lua-4.0.1.tar.gz) | 51,640 | Shared headers, allocator integration and constant placement unlock whole units. |
| Newlib | [Newlib 1.8.2](https://www.sourceware.org/ftp/newlib/newlib-1.8.2.tar.gz) | 37,496 | Correct FILE and varargs declarations unlock many libc routines. |
| SN library adaptations | [RE4 formatter and math support](https://github.com/adonis-singh/re4/tree/9b76ff003081d6ece8b838453fb8310743a972c7/src/game) plus Newlib | 16,396 | Rising Sun's bounded string writes and grouping flag distinguish its formatter. |
| GCC runtime | [GCC 2.95.2](https://github.com/gcc-mirror/gcc/tree/releases/gcc-2.95.2/gcc) | 5,532 | Target ABI, configuration and private tables matter as much as function names. |
| STLport | [STLport 4.5.3](https://sourceforge.net/projects/stlport/files/STLport%20archive/STLport%204/) | 71,452 | Six tree helpers and 186 container/algorithm instances match; scoped game-operation models support sorting, heaps and resource containers. The exact historical release remains unconfirmed. |
| Network helpers and RSA MD5 | [SocketLibrary](https://github.com/Cuyler36/dolsocketlibrary/tree/e02356692695e64429dd70dc421a4a71a493e9c8), [NetworkBasePackage](https://github.com/Cuyler36/dolnetbp/tree/ba3846a83bf86505fe60e07aefe89e3e0ffc0e34) | 43,500 | Older transport, TCP output/timers, ICMP and PPP control join routing and diagnostics; unverified stack and driver code remains. The digest source is the RSA Data Security, Inc. MD5 Message-Digest Algorithm. |
| EAGL animation (game code) | [NFS Most Wanted reconstruction](https://github.com/dbalatoni13/nfsmw/tree/1f2cdd7996791c81a580b3f7b36b44d4f9f6719c/src/Speed/Indep/Src/EAGL4Anim) | 66,524 | The attributed reference supports delta decoders, channels, skeleton poses and blending. Original target instructions establish the older layouts and fill empty reference implementations. |
| EAGL loading (game code) | [NFS Most Wanted reconstruction](https://github.com/dbalatoni13/nfsmw/tree/1f2cdd7996791c81a580b3f7b36b44d4f9f6719c/src/Speed/Indep/Src/EAGL4Anim) | 4,528 | Symbol pools and loader fragments match after recovering the target's older ELF views, private symbol scope and allocation behavior. |

These are separate, already accepted byte totals, not estimates of future gains.
See the individual library evidence documents for exact scopes, provenance and licenses.
Current combined coverage is recorded in the [verified snapshot](../progress/GR8E69.snapshot.json).
The [sorting/resource containers](GameAlgorithms.md), [observers and destruction
queue](Observers.md), and [matrix routines](Matrix.md) build on the earlier
[game-container storage models](GameContainers.md).
The [rendering reconstruction](Rendering.md) establishes shared TevStage, state,
property-parser and TAR interfaces alongside transform, material and particle
drawing paths. Runtime constructors now connect the property parser to state
and texture objects. The [animation reconstruction](Animation.md) adds scalar/vector and single-axis
quaternion delta decoders, channels, skeleton poses and checked common interfaces.
The [loader reconstruction](Loading.md) supplies scoped symbol-pool and ELF interfaces. These game
code matches demonstrate reuse from another EA reconstruction without assuming
its newer headers describe Rising Sun unchanged.
Runtime remains untested.

## Ranked research queue

This is a qualitative work order, not a prediction of hours or a promise of matches.

| Priority | Candidate | Evidence / size | Next useful step |
| --- | --- | --- | --- |
| 1 | EAGL animation formats | Several delta formats, channels and skeleton routines now match using an attributed public reference and shared interfaces | Finish remaining quaternion, stateless and blending routines against original layouts and complete generated vtables/constants. See [animation evidence](Animation.md). |
| 2 | EAGL material families | Unlit mesh/CPT blocks, state and texture runtime constructors now match | Extend the remaining lighting blocks, startup registration and PCode interpreter using the established interfaces. See [rendering evidence](Rendering.md). |
| 3 | Remaining STL instances | Remaining custom values, anonymous-namespace containers and string support | Builtin/pointer containers and the reviewed game-storage subset now match. Recover the remaining custom comparators and value representations before extending coverage. This includes game routines, not just reusable library code. |
| 4 | `powf` | 1,780 bytes; the research candidate differs in three indexed-load operand encodings | Further source/flag probes leave the same difference. A compiler word-size address-canonicalization difference is a research lead, not yet a proven cause; do not patch instructions. |
| 5 | SN `add_separators` | 392 bytes; its ABI and formatter callers now match | Finish the helper's C reconstruction and register allocation; it remains context. |
| 6 | Older network stack | Twenty accepted fragments establish older transport and PPP layouts; the Ethernet driver and unretained stack routines still differ | Compare protocol structures and behavior with original call sites, keeping retained code and storage complete. |
| 7 | Shared game math, strings and allocation | `MathFun.cpp` has 264 unmatched bytes; quaternion and line helpers now have scoped storage views | Extend [geometry helpers](Geometry.md) and transform callers; resolve the remaining MathFun constant ownership and allocator contracts. |

The verified STL container profile now covers builtin, string, pointer and
[scoped game-storage instances](GameContainers.md). The exact original release and broader configurations remain
unconfirmed. Do not invent game class layouts to instantiate further templates.
[NFL Street 2](https://github.com/mitsevox/nflstreet2) is a useful existing compiler-tooling
reference; shared EA game/engine source compatibility has not been established here.

## Mapping work with the highest payoff

The direct-call portion of item 2 is now implemented by `tools/dependencies.py`.
Its [ranked work packets](Dependencies.md) selected CMatrix for shared game-type
work: initialization had 173 distinct unfinished callers. Initialization,
assignment, multiplication and 20 related matrix functions now match using one
shared header. The independently recovered 16-byte CVector3 layout supports
[vector methods](Matrix.md#cvector3-layout), [geometry helpers](Geometry.md) and
camera/particle vector getters.
Vtable/data references and broader type recovery remain future work.

1. **Reference/version inventory.** For each family, record source URL and pinned commit,
   license, symbol/constant evidence, compiler profile, compared ranges, and unresolved
   differences. Keep candidate, partial research match, and accepted source distinct.
2. **Dependency and type map.** Record direct calls, data references, vtables, and shared
   structures. Rank helpers by how many unfinished callers depend on their declarations.
   A direct-call scan misses indirect calls and virtual dispatch; label those gaps.
3. **Reusable comparison batches.** Once a header/compiler profile is supported, compile
   a coherent library family and retain per-function *and* whole-section results. This
   finds common blockers without accepting isolated green functions over bad data.
4. **Contributor work packets.** Link a grey group to its candidate source, byte count,
   precise blocker and verification command. Small, reproducible packets reduce duplicate
   research and make AI-assisted proposals easier to review.

The target's preserved DWARF covers seven GBA-library units, not all game classes.
Game types and file ownership still need evidence. Keep original bytes, full debug
exports and local paths out of public reports. None of these maps earns code credit.
