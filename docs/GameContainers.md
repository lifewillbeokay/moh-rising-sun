# Game-container storage and behavior evidence

This batch adds **44 STLport container functions (16,592 code bytes)** and the
**256-byte `PathFindingNetwork` copy constructor**. Each complete generated
function matches its original symbol, binding, address, size, instructions and
external references. The compiler emits no allocated data for these fragments.
Integration and evidence review used AI assistance.

The algorithms use the same pinned STLport 4.5.3 reference and ProDG profile as
[the earlier containers](STLport.md). Four additional unmodified upstream headers
supply list support and its dependencies. Their hashes and original notices are
preserved in [stlport-source.json](../config/GR8E69/stlport-source.json).
The explicit instantiation files are project build fragments; they do not claim
historical translation-unit boundaries.

## Scope of the declarations

[ContainerTypes.h](../include/game-containers/ContainerTypes.h) is an intentionally
scoped representation for these operations. Original mangled symbols establish
the type names, template arguments and copy-constructor signatures. Node
allocation sizes, value offsets, complete copy sequences, vector strides and
original lifecycle calls establish the storage required by the retained code.

Unknown storage is represented by aligned unsigned-byte arrays. These ranges
may contain original fields and padding; they do not assert semantic field types,
member names or original C++ declaration spelling. The four-byte alignment is the
compatible representation used for the observed container operations, not proof
of every original type's complete alignment requirements. Enclosing classes used
only to declare nested names have no established object size.

The lifecycle declarations model the direct calls used here. They do not generally
establish whether the original class was polymorphic or which unknown bytes held
a vptr: an explicit destructor call can be direct for either kind of class.
Likewise, equivalent comparator expressions can generate identical instructions;
the recovered fact is their ordering behavior, not the original expression text.

The header is included only by manifests that need this profile. Do not use these
shells as complete gameplay class declarations, instantiate their enclosing
classes, or combine them with other definitions of the same classes. Recover
additional fields and lifecycle behavior from independent callers before using
them elsewhere. Exact bytes here establish the retained operations, not general
source fidelity for unexamined game methods.

## Retained families

| Family | Functions | Code bytes | Storage and behavior evidence |
| --- | ---: | ---: | --- |
| `AnimShape::InitParams` map | 3 | 1,204 | A 28-byte tree node has a four-byte key and eight copied value bytes. `InitByCRC` at `0x8006a2c0` independently supplies the two value words, including a float store. Their original member names remain unknown. |
| `Mesh::Geometry` map | 4 | 1,372 | A 32-byte node holds a pointer key and 12 copied value bytes. Comparison calls the original `strcasecmp` and tests its signed result against zero. |
| `Model::TARList` map | 4 | 1,360 | A 40-byte node holds a pointer key and 20 copied value bytes. `Mesh::Bind` at `0x80081cec` independently copies the five-word value from `GetTARList`. |
| `SkeletonData` string-to-integer map | 4 | 2,300 | Original inline comparisons use `memcmp` over the shorter string length, then compare lengths; `string::compare < 0` reproduces all four functions. |
| `SkeletonData::AttachPoint` map | 4 | 2,316 | Same comparator; a 40-byte node contains the 16-byte library string and eight copied value bytes. Strings retain their real constructors/destructors. |
| `PathFindingNetwork` map | 4 | 1,304 | A 32-byte node contains the byte key and 12-byte value. Insertion calls the named copy constructor; erasure clears the contained list before freeing the node. |
| `_LocationTargetInfo` map | 4 | 1,324 | A 32-byte node contains an integer key and 12 copied value bytes. |
| `BSMessageToEventRecord` multimap | 3 | 512 | A 28-byte node contains the unsigned-short key and eight copied value bytes at a four-byte-aligned offset; equal-key insertion is retained. [Message registration/removal](Script.md#message-storage-dispatch-and-event-mapping) independently establishes an object pointer at value `+0` and an unsigned-short event at `+4`; the final two bytes remain opaque. |
| `CProjectorSystemDef` map | 4 | 1,300 | A 32-byte node contains an unsigned key and 12-byte value. Original copy constructor `0x80131eb8` copies three words; constructor `0x80131e44` initializes that extent. The original destructor remains a call. |
| `StaticMesh*` to `vector<AnimShape>` map | 4 | 1,668 | Original pointer ordering and the library vector layout; each copied/destroyed `AnimShape` still calls its original lifecycle routine. |
| `vector<CFont::FontRef>::reserve` | 1 | 320 | Allocation, copy and destruction loops use a 12-byte stride; values copy three words with no destructor call. |
| `vector<AnimShape>::reserve` | 1 | 320 | Allocation and both copy/destruction loops use a 60-byte stride; calls retain the original named nontrivial copy constructor and destructor. |
| `vector<CCompartment::CInstanceModel>::reserve` | 1 | 304 | Allocation and both lifecycle loops use a 32-byte stride and call original named copy/destructor routines. |
| `vector<HandlerLeaderboard::Player>::reserve` | 1 | 380 | Allocation and iteration use a 32-byte stride; the complete 32-byte copy and empty destruction loop match. No player-field meanings are inferred. |
| Pathfinding-link list clear and assignment | 2 | 608 | A 28-byte list node consists of the two library links and 20 copied value bytes. All copy, reuse, allocation and deletion paths match. |
| **Restored library total** | **44** | **16,592** | Entire generated text is compared; no generated bytes are discarded. |

The simple value records above are copied as storage only in the retained
operations. In particular, the declarations do not replace nontrivial game
copy/destructor routines with byte copies: `AnimShape`, `CInstanceModel` and
`CProjectorSystemDef` retain their original external lifecycle calls. Their
unreconstructed bodies earn no source credit.

## Pathfinding copy constructor

[PathFindingNetwork.cpp](../src/pathfinding/PathFindingNetwork.cpp) reconstructs
`__18PathFindingNetworkRC18PathFindingNetwork` at `0x80285dc8`, size 256 bytes.
The original first copies a byte at offset zero, then constructs an STLport list
at offset four. The library list occupies eight bytes in this profile. Its
sentinel/node allocation and element copies agree with the independently retained
map, list assignment and clear functions.

The constructor uses ordinary member initialization. `field_00` and `links` are
project descriptive names; their original member names are not recovered. The
20-byte link payload remains unknown storage. This function is counted as
**reconstructed game code**, separately from the 44 library instantiations.
The filename and explicit constructor definition are project reconstruction
choices. The original translation-unit boundary, and whether this constructor
was explicitly written or compiler-synthesized, remain unconfirmed.

## Verification

The same strict object, dependency and whole-image checks apply as for the other
units. No verifier behavior was changed for this batch. The public snapshot
records every source/header/configuration hash; local tests recheck all accepted
compiled units and the exact progress categories. Only after the complete source
build and tests pass is the snapshot captured, following [Progress.md](Progress.md).
The original-object baseline is a separate result. Runtime remains untested.
