# FlexProp and string CRC evidence

[kyleckroeger’s research offer in issue #4](https://github.com/lifewillbeokay/moh-rising-sun/issues/4) highlighted CRC-derived FlexProp setting names. That useful lead prompted this reconstruction of the GameCube lookup path. The detailed notes and 241-name list were shared after these fragments were reconstructed and are now available in the [PS2 FlexProp research](research/ps2/flexprop-crc-names.md). These bodies and scoped storage views were independently reconstructed from the pinned GR8E69 executable with Codex assistance; no name from that list or PS2 layout is claimed as evidence for the accepted source here.

## Accepted lookup path

`src/flexprop/` reconstructs field search and its comparator, name-to-key wrappers, typed scalar/list access, position access and class ancestry queries. `src/string_crc.cpp` reconstructs their string hash dependency. The original lookup fragments comprise 25 functions and 1,552 executable bytes; the string-table and database extension below adds six functions and 400 bytes. Individual ranges and generated string/constant storage are recorded in their manifests.

`FlexProp` holds a format pointer at offset zero. The format’s pointer at `+4` leads to a class-description record containing its name at `+0`, parent at `+8`, field count at `+12`, and a variable tail of twelve-byte field records at `+16`. Each field contains a signed CRC key, a type word and a byte offset. The original `bsearch` stride, comparator and field accesses independently establish those three words. The zero-length array in the scoped header represents the variable tail; it is not a complete class allocation.

`FlexPropFormat::GetField` searches each class’s sorted fields, then its parent if no field matches. Its twelve-byte search record initializes only the key, which is the only word the comparator reads. Comparisons use signed 32-bit ordering; CRC bit patterns must retain that ordering when interpreting these field arrays.

`GetDataPtr` returns null for a missing field; otherwise it adds the field offset to the format’s `0x3c`-byte prefix. `GetFieldOffset` returns -1 for a missing field, and `GetData` adds an already supplied byte offset without validation. Integer, enum, float and boolean getters retain original diagnostics and zero/false fallback results. List lookup returns the original shared empty-list object when absent; the script group filters now establish its signed count and variable tail of signed
32-bit values; complete allocation and serialization remain unknown. See
[the script evidence](Script.md#thread-lifecycle-queued-delivery-and-group-filters). The private empty-list symbol is scoped to the original `flexprop.cpp` record and remains uncredited context.

Name-taking wrappers hash the exact input and call their integer-key overloads. Both string-getter overloads are accepted; the integer-key implementation uses the recovered string-table virtual interface described below. `GetFieldType` returns zero for an absent field. `GetClassName` reads the first class name; `IsA` compares names with `strcmp` while following parents. Case folding is not added.

Position accesses confirm floats at format offsets `0x30`, `0x34` and `0x38`, with `SetPositionZ` writing the last. Independent inspection of the original matrix getter supports the twelve-float transform prefix beginning at `0x0c`; that matrix getter is not reconstructed here. The position helper writes only the first three floats of the independently recovered 16-byte CVector3 representation; its fourth word is preserved. Unknown words, complete allocation sizes, original field spelling and historical return-type spelling remain unproven.

## CRC details useful for future name recovery

The matching `GetStringCRC` operates on unsigned string bytes, stops before the terminating zero, and performs no case conversion or other normalization. Null and empty inputs return zero. Nonempty inputs start at `0xffffffff`, update as `table[(byte ^ crc) & 255] ^ (crc >> 8)`, and complement the final result.

The original private `crcTable` is a 256-word read-only object at `0x802b7fc0`, scoped to `crc.cpp`. All 256 original values were independently compared with a table generated from reflected polynomial `0xedb88320`; they agree. The implementation keeps that table external and receives no data credit. This establishes the exact GameCube hash convention, but a matching CRC alone still does not uniquely establish a setting’s spelling or semantics.

## String-table and database extension

The [EA reference comparison](research/ea-shared-code.md) highlighted the same
FlexProp and string-table families in Rogue Agent and European Assault. The
following six functions were independently reconstructed from Rising Sun's
instructions with Codex assistance; no source from those games was imported.

| Function | GR8E69 address | Code bytes |
| --- | --- | ---: |
| `FlexProp::GetString(int) const` | `0x801265b4` | 120 |
| `StringTable::Create(char *)` | `0x801268ec` | 68 |
| `StringTableImpl::StringTableImpl(char *)` | `0x80126930` | 140 |
| `FlexPropDatabase::Rewind()` | `0x8012617c` | 20 |
| `FlexPropDatabase::GetNumProperties()` | `0x80126190` | 28 |
| `FlexPropDatabase::GetPropertyByIndex(int, FlexProp &)` | `0x801261ac` | 24 |

The integer-key string getter reads a four-byte index from the field returned by
`GetDataPtr`, then dispatches through `g_pStringTable` (`0x802c4ae0`). The original
virtual call reads the signed adjustment at vtable `+8` and function pointer at
`+12`. The original 24-byte `StringTableImpl` vtable at `0x802e8258` identifies
`LookupString(int)` in that slot and contains no destructor entry. The source
uses a virtual C++ interface, not a manually assembled dispatch table.

A missing property prints the original diagnostic and returns an empty string.
An existing property delegates even if its index is zero; the original lookup
returns null for zero. These two cases must not be conflated. The complete
56-byte diagnostic/empty-string pool at `0x802a6d68` matches, including padding.

`Create` requests eight bytes from `DWI_alloc`, with the observed flag value 1024
and original diagnostic label `source/ai_script/flexprop.cpp:700`, then invokes
the constructor. Its complete 36-byte string pool at `0x802a6e04` matches. The
allocation, constructor stores at offsets zero and four, virtual call and original
lookup independently support an eight-byte implementation object containing a
vtable pointer and data pointer. Original field names and access control are not
recovered. No extra allocation-failure check or virtual destructor is introduced.

The serialized data view has a signed four-byte count followed by pointer-sized
entries. The constructor converts the count in place, converts each entry using
the original `ChangeEndian<char *>`, then adds the original buffer base. It keeps
the entry reference across that call and reloads the stored buffer/count for each
loop test. A nonpositive count skips the loop; no bounds validation is added.
`StringTableData` names this variable-length prefix, not a fixed allocation or a
portable host-side parser. The original lookup body, native vtable and endian
template remain external and earn no new source credit.

The three database methods are static: the index arrives in the first argument
register and the output reference in the second. The private `g_properties`
object is a 16-byte STLport vector at `0x8032cda0`; its iterator is a four-byte
pointer at `0x8032cdb0`. The original load/unload paths corroborate the pointer
array, destruction/reset and end-pointer accesses. The existing attributed
STLport subset reproduces all three methods without a replacement container
layout. `Rewind` selects `begin()`, count uses `size()`, and indexed access stores
the selected format pointer without checking bounds. Both private globals remain
original storage, resolved within the `flexprop.cpp` file record.

The matrix getter and string lookup were also investigated but remain unaccepted;
their partial source comparisons do not increase progress.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every accepted function, diagnostic string and generated constant. No byte patches, inline assembly implementations, discarded functions, clipped sections or fuzzy matches are used. The private comparator and dependencies retain their original symbol binding/file scope. Whole-image verification and the public snapshot checks remain required; runtime behavior has not been emulator-tested.

These declarations and the recipe evidence in [ParticleRecipes.md](ParticleRecipes.md) provide a starting point for applying the [contributed PS2 research](research/README.md). The script/opcode, `.bpd` and music-rule notes record their own GameCube comparisons; they do not add reconstructed source in this batch.
