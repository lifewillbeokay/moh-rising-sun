# EAGL loader reconstruction

The loader fragments reconstruct symbol-pool insertion/removal and lifecycle,
constructor registration, dynamic-loader lifecycle, constructor dispatch, symbol
inspection and lookup iteration. They adapt the public NFS Most Wanted
reconstruction to Rising Sun's older `EAGL` interface. The remaining initializer,
resolver and unfinished lookup bodies stay original context. The initializer and
two lookup bodies now have maintained partial source, described below. Current accepted
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
