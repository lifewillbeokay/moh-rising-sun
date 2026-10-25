# sys_memory.cpp: allocation operators

Four game allocation operators are reconstructed in `src/sys_memory.cpp`:
**192 code bytes and 32 read-only data bytes**. Only code earns progress credit.
AI assistance was used for reconstruction and verification.

| C++ operation | Original symbol | Address | Code bytes |
| --- | --- | --- | ---: |
| `operator new` | `__builtin_new` | `0x801a8910` | 64 |
| `operator new[]` | `__builtin_vec_new` | `0x801a8950` | 64 |
| `operator delete` | `__builtin_delete` | `0x801a8990` | 32 |
| `operator delete[]` | `__builtin_vec_delete` | `0x801a89b0` | 32 |

The `sys_memory.cpp` file record and its local compiler marker precede the
`configfile.cpp` marker at `0x801a89d0`. These global functions fall within that
adjacent-marker interval; file association is therefore inferred. Their names,
addresses and sizes are direct symbol evidence. This is a fragment of the original
unit; the allocator implementation remains original context.

## Behavior and dependencies

Both allocation operators read the four-byte global `g_memLabel` at `0x802c7dc0`.
If it is null, they use their respective strings, `operator new` and
`operator new[]`. The original instructions establish both string addresses;
the complete compiled `.rodata` block, including alignment padding, matches
`[0x802b77e8, 0x802b7808)`. The strings are neither guessed labels nor arbitrary
copies selected elsewhere in the executable.

They call `DWI_alloc` at `0x801a8670` with the selected label, requested size and
the value **1024**. The third argument is named `flags` descriptively; its complete
semantics are not recovered, and it is not claimed to mean alignment. `DWI_alloc`
is declared with two integer parameters, consistent with its calls to the local
`allocSmall__FPCcii`. Passing the unsigned allocation size preserves the target
compiler's observed 32-bit argument bits; this does not establish portable
behavior for every size.

Both deletion operators call `DWI_free` at `0x801a8894` and ignore its result.
That routine's observed paths return an integer status; its implementation and
`MEM_free` remain unreconstructed. The wrappers add no null check, exception,
new-handler loop, or replacement failure behavior. `g_memLabel` remains externally
owned data. Its declaration as a pointer to const characters reflects its use;
the original typedef and const spelling are unknown.

The [dependency scan](Dependencies.md) finds **424 distinct unfinished caller
ranges** across these four operators. Many container instantiations use this
path. Reconstructing these small wrappers establishes a useful allocation
boundary; it does not reconstruct their callers or the underlying heap.

## Verification

ProDG 3.8.1 with `-O2 -G0` compiles ordinary C++ operator definitions to the
original SN/GCC symbol names. The manifest checks all four function ranges,
bindings, external definitions and both generated sections. All 192 code bytes
and 32 data bytes match. No code/data is discarded or patched.

The complete analysis-image comparison includes this fragment. The original
compiler release is still unconfirmed, and runtime/emulator behavior has not
been tested. Use `python3 tools/reconstruct.py` and the validation suite for
extensions; follow [Progress.md](Progress.md) before updating coverage.

## MemStack lifecycle

`src/memory/memstack.cpp` independently reconstructs two further game functions
with Codex assistance: the constructor at `0x801acbe8` (**80 bytes**) and destructor
at `0x801acc38` (**76 bytes**). The complete 36-byte diagnostic pool at `0x802b83c0`
contains `source/system/sys_memstack.cpp:7`, its terminator and alignment. All
156 code bytes and 36 data bytes match under ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`; only code earns progress credit.
No code from another game is imported.

`MemStack.h` gives the target object three words: a buffer pointer at zero,
a current pointer at `+4`, and signed capacity at `+8`. The constructor's stores,
the original `Alloc(int, int)` reads, and the twelve-byte object allocations in
the [script startup callers](Script.md#startup-allocation-and-system-state)
independently agree on this representation. Names are descriptive; no historical
inheritance or virtual methods are invented.

Construction requests exactly the supplied size from `DWI_alloc` with flag 1024
and the diagnostic above, stores the result in both pointers and records the
capacity. It does not initialize buffer contents or handle a null result.
Destruction calls array deletion on the buffer. The compiler's complete deleting
destructor also calls ordinary object deletion when its hidden delete flag
requires it; that code is included in the verified function range.

The inline labelled object-allocation operator supplies the existing DWI boundary
for the startup callers. It is not claimed to be the original spelling or a
separate recovered function. The heap implementations and all global stack
pointers remain original dependencies.

`MemStack::Alloc` at `0x801acc84` remains **unreconstructed**. Inspection establishes
alignment of the current pointer, a capacity comparison and cursor advancement;
the current candidate differs in two instructions' scheduling and receives no
partial-match credit. Its declaration supports the interface without providing
an accepted implementation. The complete source-built image, all generated
sections and unchanged prior units are verified separately. Integrated host
checks exercise construction, buffer deletion, null buffer results and startup
stack replacement with host-sized allocation fixtures; they do not validate
target pointer sizes, destructor flags or game runtime.
