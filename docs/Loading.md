# EAGL loader reconstruction

The loader fragments reconstruct symbol-pool insertion/removal and lifecycle,
constructor registration, dynamic-loader lifecycle, constructor dispatch, symbol
inspection and lookup iteration. They adapt the public NFS Most Wanted
reconstruction to Rising Sun's older `EAGL` interface. The remaining initializer,
resolver and unfinished lookup bodies stay original context. Current accepted
ranges are recorded in the project manifest and verified progress snapshot.

## Reference and storage evidence

The reference is [dbalatoni13/nfsmw at revision
1f2cdd7996791c81a580b3f7b36b44d4f9f6719c](https://github.com/dbalatoni13/nfsmw/tree/1f2cdd7996791c81a580b3f7b36b44d4f9f6719c/src/Speed/Indep/Src/EAGL4Anim),
specifically the `eagl4supportdlopen`, `eagl4supportsympool` and
`eagl4supportconspool` sources and headers. Attribution and the upstream CC0 text
are preserved in [NOTICE](../src/eagl_loading/NOTICE) and
[LICENSE.nfsmw](../src/eagl_loading/LICENSE.nfsmw). Codex assisted this adaptation.
These are reconstructed game bytes, not restored SDK/library credit. Reference
field names are descriptive, not recovered original member spelling.

Original constructor accesses, deleting destructors and named global extents
independently support the following storage:

| Type | Observed extent and important fields |
| --- | --- |
| `SymbolPool` | 20 bytes; size/length at 0/4, table at 8, unknown word at 12, callback chain at 16 |
| `SymbolEntry` | One address word followed by a variable-length name; allocation is name length plus five bytes |
| `FunctionEntry` | 12 bytes; callback, previous and next pointers |
| Constructor pools | Two 20-byte symbol pools; named globals are 40 bytes |
| `DynamicLoader` | 48 bytes; handle, destructor count/list, runtime destructor list, input data/length/relocations, callback, patch count/capacity/array and resolved flag |
| Runtime destructor entry | 16 bytes; destructor, data, auxiliary argument and next pointer |
| `HashPointer` | 1,072 bytes; list links, string/symbol/section/header references, 256 hash buckets, chain and ownership arrays, loader and callback |

The ELF declarations retain native 32-bit ELF field widths and the independently
accessed 52/40/16-byte header, section and symbol records. The loader's four-byte
boolean storage follows this SN ABI. Original global pools, hash head, callbacks
and unimplemented methods remain external; their bytes earn no source credit.
The earlier texture-loading declarations now use this same shared interface.
`GetAddr`'s return is a boolean in its observed implementation; existing texture
callers ignore it and retain their original generated behavior.

## Preserved behavior and verification

Constructor-pool destruction explicitly destroys both member pools before the
compiler's member cleanup invokes their destructors again. The source preserves
these original calls. Loader destruction passes the patch capacity directly to
the free callback; it does not multiply that argument by pointer size.

`GetSymbol` initializes the returned name/data fields, then clears them again.
The other fields remain uninitialized on early returns. Valid records retain the
0x7f name/type separator and original unsigned internal-reference predicate.
The private `dlsym` diagnoses an out-of-range chain index and continues as the
original does. These are reconstructions, not runtime fixes. The reference's
explicitly described fake `Search` cast was not imported to force a match.

Native ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates`
reproduces the accepted fragments; it does not establish the original release.
Separate fragments preserve the observed inlining while sharing one storage
model. The local `dlsym` and private `hashhead` dependency are bound to the
original `dlopen.cpp` file record. All generated strings, code, symbols and
section boundaries are checked, with complete-image verification required before
publication. No instruction substitutions or clipped constant pools are used.
The game's loading and destructor path has not been exercised in Dolphin.

## Toilet

`Toilet` is the game's file loader, with one global instance, `g_toilet`. The image records no
name for its source file. `include/game/Toilet.h` holds a view of the class: names of the class,
functions and global come from the symbol table, and member names are inferred from use.

`TLT_CloseFile` returns without freeing when the pointer lies inside the level big file or either
of the two other big-file buffers at `+0x120` and `+0x128`, and otherwise passes it to `DWI_free`.
A level-contents entry is 12 bytes: a type, an unknown word and a table of value records at `+8`.
`TLT_FindNextResourceByType` walks these entries from the one after its argument, or from the
start for null. `GetCompartmentNameFromIndex` looks the index up in resource type 0, comparing the
value of table record 1, and formats record 0's value with `"|%s"`. Each compartment has three
consecutive indices: index * 3 is its collision file, + 1 its visual file and + 2 its relocation
file. A missing compartment big file or entry makes the size queries return -1.
`TLT_Async_Load` runs one of 20 state functions from `g_ToiletLoadStates` and adds its result to
the state. Its progress denominator is 21 for missions 2 through 4 and 22 for the others.

The local symbols of the load states name the file `toilet.cpp`. The load states read the relocation file, the level big file, the pause big file, the animation
bank and the optional MVD file through the asynchronous file calls. The animation bank and MVD are
copied up to `VMStartLoc`; the bank then advances it by the size rounded past the next 32-byte
boundary. `+0x194` indexes the sound banks while they load and also feeds the progress percentage.
`TLT_Sound_GetCurrentAbk` returns null past the second bank. `TLT_Load_InitContent` checks version 2
and checksum 0xa5658, then rebases the entries and name tables of resource types 0 and 4; the types'
meanings are inferred. `TLT_Load_StartPause`, `TLT_Load_StartAnimation` and `TLT_Load_InitRepository`
share strings with code outside these units and remain unreconstructed.

Not reconstructed: the class constructor and language and system-file setup,
`TLT_FindNextResourceByTypeAndVal` (6 differences, all parameter registers), and the two functions
that use the file-local `s_ignoreLoadFileNormalLocked` (`TLT_ForceLoadFileNormalLocked`,
`TLT_LoadFileNormal`), plus the repository functions after them. The extension search walks the
level big file with `BIG_locateentry` and compares each entry name's tail case-insensitively.

## Small-object pools and the DWI allocator

The original `sys_memory.cpp` puts pools of fixed-size elements in front of the general allocator:
`g_smallPool` has `g_numSmallPools` 24-byte records, each with its memory, a word per 32 elements of
free bits, its element count, the free-word count, a used count and the element size in words.
`DWI_alloc` tries the pools for sizes up to `g_maxSmallSize` before `MEM_allocalign` with 64-byte
alignment and fills the result with zero. Sizes above 256 add flag 0x100 unless the caller's flag
0x80 is set; that bit is removed before the call. `DWI_realloc` copies the smaller of the two sizes.
Verified: the code. Inferred: the member names and the flags' meanings. Not reconstructed:
`allocSmall` and `freeSmall`.

`BitBuffer` is a 2048-byte store written least significant bit first. The reader and writer are
file statics; `Load` sign-extends results of 2 to 31 bits.

## Background thread

`Thread`, from the allocation labels' `thread.cpp`, runs jobs on one worker. `Init` allocates the job
records and a stack filled with 0xbad4c0de, queues every record as free and starts the worker at
priority -3. The worker pops a pending job, runs its priority, then its `begin`, `run` and `end`
callbacks in that order, restores the priority and returns the record to the free queue. A mutex
at the object's start guards both queues. The queues are `_STL::queue<ThreadJob *>`; that header is
not in this tree, so the fragment declares the adapter. Verified: the code. Inferred: member names.

## Lua state scripts

The original `mohlua.cpp` opens one Lua state and registers nine functions the state scripts call:
`SetStateChoice` and queries on the current object. `LUAEvaluateState` sets that object, clears the
choice, runs a buffer and returns the choice. `GetStateVariable` calls `GetValue` through the
object's table at `+0x38`. `BeginHurtling` reports `StartScriptDeath`'s argument message, as in the
image. Verified: the code. Inferred: the script data's member names at `+0x8c` to `+0x9c`, and the
meanings of the event numbers `StartScriptDeath` sends.

## Locale files

The original `locale.cpp` reads little-endian string files: a table of per-language offsets at
`+0x10`, each language holding offsets to its strings at `+0x10` with a count at `+0xc`. With bit 0 of
the flags at `+8`, `LOCALE_getstr` first turns the id into an index through a sorted id table found at
the offset at `+4`. `LOCALE_create` byte-swaps each string's 16-bit characters when asked. Verified:
the code. Inferred: all member names.

## Disc drive checks

`drivecheck.cpp` maps `DVDGetDriveStatus` to a state, 0 for no message and 1 for busy, and shows the
message for any higher state from a table per language, with English as the fallback. Disk number 1
in `CheckDiscDriveStatus` and 0 in `CheckDiscChangeStatus` distinguish the two door states; a busy
drive keeps the last of those states. Status -1 spins forever. Verified: the code. Inferred: the
state names.

`DVDSafeRead` waits for a clean drive, reads asynchronously at priority 2, and cancels and repeats the read if the
drive reported an error during it. The error messages are drawn with the ROM font through `InitRomFont`, `DrawFontChar`,
`RFPuts`, `GetRFTextWidth` and `DrawMessage`, the SDK's `DEMOPuts` code with the z coordinate dropped. Not reconstructed
yet: `LoadSheet`, whose `.rodata` is 20 bytes with a double in it and runs into the strings of
`LevelExistsOnCurrentDisc`, and `SwapDiscs` (saved registers only).

## File device reads

`filedev.cpp` reads a disc file through a bounce buffer on its read thread. The request record holds the destination
and file positions with their 32-byte aligned bounds, and a state: 1 for a short read, 2 for a misaligned head, 3 for a
run read in place, 4 for a chunk of at most 16 KB, 5 for finished. Verified: `QEndOp` and `AyncDVDRead`. Inferred: the
record's member names and the state meanings. Not reconstructed yet: `StartNonAlignedAyncRead`, `AyncDVDCallback`,
`ReadFileThreadFunc`, `readfile`, `initfiledev`.

## Movie subtitles

`RCMP::AV_SUBTITLE_ARRAY` holds the parsed subtitles, 52 bytes each, and `RCMP::VD_SUBTITLE` draws them with one `TAR` per
shape in the subtitle shape file. The display's loops are unreduced, so its fragments build with `-fno-strength-reduce`;
that describes the code, not necessarily the original setting. Verified: the array's construction, destruction and lookup, a subtitle's defaults and setters, and the display's `Init`,
`DeleteTars`, `CreateTars` and accessors. Inferred: member names. Not reconstructed: `FindSubtitle` (four instructions of
order), the subtitle script parser (`AV_SUBTITLE_ARRAY::Init` and its helpers), `CreateWrappedStrings`, `Draw`.

## MAD movie codec

`rcmp_mad_codec.cpp` is whole: one unit, in the original's order, with the codec's table. It hands out frames from a free
list to a used list, through a link inside each frame; `DecodeChunk` reads a chunk's header on the first chunk, makes the
decoder's frames and then decodes each 16x16 macroblock against the held frame. `rcmp_mad_codec_chunk_types.cpp` holds the
three chunk tags and the test for them. Both files' `.rodata` start with the empty allocation label of the codec's
`operator new`. Verified: all of both files' code. Inferred: member, local and helper names, and what the chunk tags mean.
`CODEC` is declared with plain virtuals so that its table stays outside these units; the original's slots are pure.

## MAD macroblock decoder

`maddec.cpp` builds its code tables on first use and decodes a macroblock at a time: six blocks (four luma and two chroma)
each come from the previous frame (`mode` set), a DC value, or the inverse transform. `maddeca.cpp` reads one block's
coefficients through the run-length tables, and `madidct.cpp` holds the transform, with `fixedmult` the 16.16 multiply. These
units use small data for the bit reader's three words, so they build with `-G8 -fno-strength-reduce`; their loops are
unreduced. Verified: all of `maddec.cpp` and `maddeca.cpp`, `idctcompute` and `fixedmult`. Inferred: parameter, local and
helper names, and the mode bit meanings. The tables are referenced, not defined, by these units. Not reconstructed:
`IdctColumn` and `IdctRow`, whose final statements are not in the original's order yet.

## RCMP flags

`-fno-strength-reduce` holds for the RCMP library's code: `GetFrame` and `DecodeChunk` here and the subtitle display show
unreduced loops, and every other RCMP unit (`audioplayer`, the `AV_PLAYER` fragments, `DECODER`, `GCHW_VD`, `rcmp2real`)
compiles identically with or without it, so they all pass it. `rcmpplayer.cpp` is the game's `CMoviePlayer`, not part of the
library, and keeps the game's flags; its fragments also compile identically with the flag. The flag describes the code,
not necessarily the original setting.

## MohLoader

`MohLoader` and `MohLoaderGc` hold the console-side start-up state. `g_pPersistent` points to a
block that survives a module reload: game data magic `0xc0de0101` at `+0`, returning magic
`0xc0de0bac` at `+4`, a reset button flag at `+8` and the game settings at `+0xc`. Without the
returning magic, `InstanceGc` clears the game data magic and reset flag first. The constructor
clears the game settings unless the game data magic is valid.

`LoadModule(const void *, unsigned long)` copies a loaded module's sections from its section table,
zeroes each section first, flushes instruction and data caches, and jumps to the entry point at
`+0x18`.

Not reconstructed: `CheckForControllerReset` (function-local statics), the path `LoadModule`, and
the two compiler-placed destructors, which are declared but emitted far from these units.

`LevelExistsOnCurrentDisc` (`drivecheck.cpp`) asks DVD for `data/%d/%d_%d/comp.viv`, and
`GetLoadscreenId` (`LoadImage.cpp`) picks a load screen from its flags. From `Moh3.cpp`,
`RestoreCheats` reapplies the option cheats when game data is valid and the profile byte at
`+0x31664` is 1, and `OnePassScanTask::Execute` records that the scan ran. `ShutdownCORE`, `InitCORE` and `RestartGame` are private to the file and
need `main` in the same unit, because an unreferenced private function is not emitted.

## CMemoryCard

`CMemoryCard` (a `EALA::Resource`, with the table pointer at `+4`) loads two localization files at
construction and creates locale data from each. The small reconstructed accessors read file globals.
The destructor, `Display` and the rest of the class remain original.

## Maintained partial loading functions

Three previously parked functions are registered comparisons, using the accepted
`eagl_loading_dynamic_loader_symbol.json` ProDG profile. They retain the existing
NFS attribution and native ELF/loader declarations; no fake callback cast or
function-specific compiler setting is used.

| Function | Target bytes | Compiled bytes | Objdiff similarity |
| --- | ---: | ---: | ---: |
| `DynamicLoader::GetAddr` | 476 | 476 | 99.92% |
| `DynamicLoader::Initialize` | 2,752 | 2,848 | 73.14% |
| `SymbolPool::Search` | 280 | 272 | 93.71% |

`GetAddr` distinguishes the name and optional `0x7f`-prefixed type, returns either
an absolute symbol or section-relative address, and falls back to the user callback.
A failed callback can still set the output pointer. Its remaining two byte
differences select registers in the final hash-index shift/load pair. The original
out-of-range-index diagnostic continues through the chain; this is not repaired.

`Initialize` swaps the ELF header, section and symbol fields in place, resolves
section offsets through `ELFAddr`, rewrites `__type:::name` strings into
`name\0\x7ftype`, builds the hash chains and original-section flags, and inserts
the handle into the global doubly-linked list. Unsupported section diagnostics
continue as in the target; this is not a general-purpose validated ELF parser.
Both contiguous and separate relocation storage are exercised. `_4EAGL.hashhead`
retains its original private `dlopen.cpp` file scope. Register allocation, loop
induction and instruction scheduling remain unresolved.

`Search` preserves linear probing, wraparound, repeated table reads and the
original `count > mTableLength` stopping condition. Its loop induction still
differs. Valid nonzero table length and suitable input storage remain caller
preconditions; no new malformed-input checks are inferred.

The complete 80-byte pool at `0x802be900` and 456-byte pool at `0x802be738` match;
`Search` emits no allocated data. All three bodies stay out of the verified
image and earn similarity only. [The batch verification notes](research/shortlist-partials.md)
record the original/native checks and their limits.
