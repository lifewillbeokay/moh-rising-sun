# Behaviour-script reconstruction

[kyleckroeger's PS2 research](research/README.md) supplied the opcode map,
class-description shapes and music-interface leads for this work. The accepted
bodies were independently reconstructed from the pinned GameCube GR8E69 executable
with Codex assistance. No PS2 source, offsets or file layout were assumed to apply
without checking the GameCube instructions and symbols.

## Accepted scope

Forty fragments in `src/script/` reconstruct **68 functions and 10,812 executable
bytes**: all **33 opcode handlers** (5,596 bytes), ten message-registration and index
helpers (964 bytes), nine thread/message-delivery and group-filter routines (1,700
bytes), event lookup (220 bytes), three music built-ins (372 bytes), and all twelve named
timer routines (1,960 bytes).
Their exact symbols, ranges and dependencies are in
`config/GR8E69/script_*.json`. Original file markers support the `bsmachin.cpp`,
`bsmessage.cpp`, `bsfile.cpp`, `bsbifunc.cpp` and `bstimer.cpp` groupings; these fragments are not
complete original translation units. Private globals retain their original
file-record scope in the manifests and remain original storage, with no source/data
credit.

## Verified instruction behaviour

The interpreter stack uses four-byte slots and its top pointer addresses the current
value. The high 16 bits of a four-byte instruction word carry an argument or type
selector. Branch and slot arguments and the `CAST` selector are signed; arithmetic,
`NEG` and comparison selectors are unsigned. Byte accesses below refer to GameCube
big-endian memory, not PS2 file offsets.

| Handler | Behaviour established by the matched body |
| --- | --- |
| `BNE` | Pops one value; a nonzero value advances one instruction, while zero jumps by the signed argument in words relative to the current instruction. The original name is preserved. |
| `JMP` | Jumps by that same signed relative argument. |
| `ADD` / `SUB` / `DIV` / `MULT` | Applies the named operation to the lower (left) and top (right) operands, writes the result in the lower slot, pops one slot and advances one word. Integer pairs produce integers; mixed or float pairs produce float bits. Unsupported type pairs produce zero. |
| `NEG` | Negates an integer or float in place according to the type selector; other selectors replace the value with zero. Advances one word. |
| `GT` / `GTE` / `LT` / `LTE` / `EQ` / `NE` | Compares the lower (left) operand with the top (right) operand, stores integer zero or one in the lower slot, pops one slot and advances one word. Unsupported type pairs produce zero. |
| `CAST` | Selector 2 converts the top float to an integer; selector 3 converts the top integer to a float. Other selectors leave the value unchanged. Advances one word without popping. |
| `AND` / `OR` | Applies logical conjunction/disjunction to the two integer slots, writes normalized zero or one into the lower slot, pops one slot and advances one word. These are logical rather than bitwise operations. |
| `NOT` | Replaces the top value with normalized integer logical negation and advances one word. |
| `POP` | Subtracts the signed argument from the stack pointer and advances one word. |
| `PUSH` | Pushes the following word and advances two words. |
| `SW` / `LW` | Stores and pops, or pushes, a frame slot indexed by the signed argument. |
| `SG` / `LG` | Performs the corresponding access through the current globals pointer. |
| `SM` / `LM` | Treats the top value as an address of integer slots. Store takes the value below it and pops both; load replaces the address with the selected slot. No complete object layout is implied. |
| `LS` | Pushes the class shared-value slot indexed by the signed argument and advances one word. |
| `GP` | Pushes the result of `TriggerObject::GetLegacyField`. Instruction byte 1 selects the field; a nonzero byte 0 selects the native receiver from frame slot 3, otherwise the current script object supplies it. Advances one word. |
| `NEXTSTATE` | Takes the low 16 bits of the top slot as the target state ID, handles the exit-event phase and message-registration changes, then transfers execution as described below. It does not increment the instruction pointer. |
| `FNCALL` | Saves the frame and return address in two stack slots. Instruction byte 1 gives the argument count. The following word selects a code-base entry with its high byte and a word offset with its low 24 bits. |
| `BIFNCALL` | Sets the current built-in index from the signed argument, calls that table entry with the stack-pointer address and current native object pointer, then advances one word. |
| `RETURN` | Uses instruction byte 0 to locate the saved frame and return address. Byte 1 selects whether to keep the top value at the old frame or discard the frame's values. |
| `BREAK` | Sets the stop flag to one and also advances the instruction pointer by the signed argument. |
| `TRACE` | Advances one word without other work. |

Valid stacks, indices, code positions and referenced storage are preconditions of
the original interpreter. The reconstruction preserves its accesses and ordering;
it adds no range checks or fixes for malformed scripts. Pointer/integer conversions
describe the original 32-bit target, not a host-portable interpreter implementation.

## Numeric representation and conversions

The matched numeric handlers establish selector 2 for a signed 32-bit integer and
selector 3 for a single-precision float. Stack slots retain their four-byte value
bits; interpreting a float slot is different from converting an integer to float.
The local unions in negation and casting expose the result's float representation
and integer storage bits for the target compiler, without asserting a larger runtime
type or a portable host implementation.

For binary arithmetic and the six comparisons, the high-halfword selector encodes
the **right operand's type first**, followed by the left operand's type:

| Selector | Lower / left slot | Top / right slot |
| --- | --- | --- |
| `0x0202` | integer | integer |
| `0x0203` | float | integer |
| `0x0302` | integer | float |
| `0x0303` | float | float |

Mixed arithmetic and comparisons convert the integer to single precision before
applying the operator. Float arithmetic uses single-precision addition, subtraction,
division or multiplication and stores the result bits in the integer stack slot.
Subtraction and division preserve lower-slot minus/divided-by top-slot order. The
integer division path uses signed division, truncating representable results toward
zero. No overflow handling or divide-by-zero guard is present in these handlers.

In mixed comparisons, large integers can therefore lose precision before equality
or ordering is decided. Float comparisons use the original unordered comparison
instructions: NaN makes `NE` true and the other five comparisons false; positive
and negative zero compare equal.
`NEG` uses the target's float sign-negation instruction for float values.

`CAST` uses the target's truncation-toward-zero instruction for float-to-integer
conversion. Integer-to-float conversion rounds to single precision before storing
the result bits. No clamps, error reporting or checks for invalid conversions have
been added. These are verified target-instruction observations, not a claim that
host C++ gives defined results for out-of-range float casts or signed negation
overflow.

The arithmetic fragment generates 60 read-only bytes: four zero literals, four
eight-byte integer-conversion constants and 12 alignment bytes. `ADD`'s zero is at
`0x802a5054`, immediately before its aligned conversion constant at `0x802a5058`.
The other three zero/conversion pairs begin at `0x802a5060`, `0x802a5070` and
`0x802a5080`. A descriptive one-element constant array in `.rodata.add_initial`
places the first four-byte literal separately from the 56-byte compiler pool. This
is a storage annotation, not a recovered historical identifier or a claim about
original source declarations. Every generated byte, including alignment, is checked;
no section or padding is dropped and no instruction is patched.

The comparisons add six eight-byte conversion constants at
`0x802a5090`–`0x802a50bf`, and casting adds one at `0x802a50c0`. All **116 generated
read-only bytes** are compared separately from executable progress.

## Object access and state transitions

`LS` reads the class pointer at script object `+0`, then the shared-value pointer at
class `+0x60`. `GP` reads the native receiver at script object `+8`, or treats frame
slot 3 as that receiver when instruction byte 0 is nonzero. Its field argument is
unsigned byte 1. The native field getter remains original code, and the declaration's
integer return represents the four-byte value pushed by this caller; the meanings
and types of individual legacy fields are not established here.

`NEXTSTATE` confirms these additional GameCube runtime accesses. Field names in
`ScriptRuntime.h` are descriptive reconstruction choices; original type names are
retained where required by external function signatures.

| View | Accesses established by the handler |
| --- | --- |
| State entry | Entry code at `+0`, events at `+4`, messages at `+8`, parent ID at `+0x0c`, depth at `+0x0e`, message count at `+0x0f`, event count at `+0x10`. |
| Event entry | Eight-byte stride; code at `+0`, event number at `+4`, flags at `+7`. Bit 0 enables event 1 for the exit phase. Other flag bits and byte `+6` remain unknown. |
| Message entry | Twelve-byte stride passed to `BSRegisterMessage`; `NEXTSTATE` does not inspect the entry fields. Registration accesses are described below. |
| Interpreter thread | Current state pointer at `+0x10`, level pointer at `+0x14`, registration-list head at `+0x18`, state ID at `+0x1c`, flags at `+0x1e`. |
| Registration-list node | Handler pointer at `+0`, associated registration at `+4`, next node at `+8`, depth at `+0x0c`, state ID at `+0x0e`. |
| Associated registration | Bit 1 of the flags byte at `+0x0d` is cleared when the node is removed. Other fields remain opaque. |

The byte named `depth` orders states during parent traversal and registration cleanup.
These accesses establish runtime roles and minimum extents, not complete allocations,
historical member names, enum names or GameCube `.sin` serialization. The shared
header also supplies the previously verified event-lookup views, avoiding divergent
class/state declarations in separate fragments. `TriggerObject`'s scoped flags
and property view are described in [TriggerObject.md](TriggerObject.md).

The transition follows these paths:

1. Read the target state's depth. If thread flag bit 0 is clear and the current depth
   is at least the target depth, search the current state's level table upward through
   parent IDs. Stop at `0xffff`, below the target depth, or upon finding an enabled
   event 1. Within each event list, stop at a matching event or a number greater than
   1; this preserves the original expectation of ordered event entries.
2. If an exit event was found, set thread flag bit 0, place the target state ID and
   current `BSObject` pointer in frame slots 0 and 1, set the stack top to frame slot 1,
   and jump to that event's code. The current state and its registrations are retained
   on this path.
3. Otherwise, clear thread flag bit 0. When the state ID changes, transitions to the
   same or a shallower depth remove registration nodes from deeper states, and nodes
   at the target depth belonging to a different state. Removed nodes go to the free
   list after `BSMessageRemoveHandler`; an associated registration has bit 1 cleared.
   Retaining a node for the target state suppresses duplicate registration. Moving to
   a deeper state keeps existing nodes and registers the target state's messages.
4. When the state ID changes, bind the target state and register messages when needed
   through `BSRegisterMessage` and `BSMachineGetFreeMessageList`. Jump to the state's
   entry code and reset the stack top to one slot below the frame. If the state ID
   was already current, skip registration changes and still take this entry-code path.

Both code transfers use the high byte as a code-base index and the low 24 bits as a
word offset. Message registration/removal and free-list allocation now have matching
source as described below. Native field lookup remains original code.

## Message registrations and index helpers

The following complete bodies are reconstructed. They extend the registration path
used by `NEXTSTATE`; the subsequent thread and queue work is described below.

| Functions | Executable bytes | Verified behaviour |
| --- | ---: | --- |
| `BSRegisterMessage` | 404 | Finds a compatible registration, reuses it or marks it as associated, and creates a new registration when needed. |
| `BSMessageRemoveHandler` | 160 | Unlinks a table-linked registration and pushes it onto the registration free list. |
| `BSMessageGetFreeRegistration` | 60 | Pops the registration free list through its signed next index. |
| `BSGetMessageHandlerByIndex` / `BSGetMessageHandlerIndex` | 76 | Converts signed indices and pointers using a 16-byte registration stride; `-1` and null are the sentinel pair. |
| `BSGetThreadIndex` / `BSGetThreadByIndex` | 80 | Searches an object's thread array or directly indexes it using a 32-byte stride. |
| `BSInitMessageFreeList` / `BSMachineGetFreeMessageList` | 164 | Reserves and links 16-byte state-registration list nodes in the interpreter arena, then pops nodes from that list. |
| `BSMessageGetMemoryRequirements` | 20 | Returns `16 * g_iBSMessageRegistrationListSize + 1776`. The constant is preserved without inferring an allocation breakdown. |

The shared runtime header now establishes these additional accesses:

| View | Established fields and extents |
| --- | --- |
| Message entry | Code word at `+0`; a 32-bit comparison value at `+4`; message ID at `+8`; a comparison byte at `+0x0a`; flags at `+0x0b`. The descriptive names `matchValue` and `matchKind` do not establish the underlying enum or split the comparison word into PS2 halfword meanings. |
| Registration | Signed object index at `+0`, next index at `+2`, previous index at `+4`, state ID at `+6`, message pointer at `+8`, thread index at `+0x0c`, and flags at `+0x0d`. The stride is 16 bytes; the final two bytes remain opaque. |
| Script object | Thread-array pointer at `+4`. The class level count bounds the linear thread search. |

When message flag bit 0 is set, registration searches the head indexed by message
ID. It compares the resolved thread, the word at message `+4`, and byte `+0x0a`.
If the code word also matches, it returns the existing registration without updating
its state ID. Otherwise it sets bit 1 of that registration's flags, returns it through
the associated-registration output, and allocates a new registration. The search
stops at this first compatible entry. The output is initialized to null on all paths.
The original `BSSendMessage` body tests this registration bit at `0x800f7838` and skips
the entry when set; that dispatch body is inspected evidence, not accepted source.
`NEXTSTATE` clears the bit when it removes the replacing state's list node.

A new registration stores the object index, thread index, state ID and message
pointer, and starts with zero flags. With message flag bit 0 set, it is inserted at
the message-ID list head and repairs the former head's previous index. Otherwise
both indices become `-1` and it is not inserted into that table. Removal repairs
both neighbours or the head as applicable, then links the entry into the free list;
it does not clear the remaining fields.

The two free lists are distinct: `BSMessageListView` nodes belong to interpreter
states, while `BSMessageRegistration_struct` entries belong to the message system.
The state-node initializer advances the arena offset by `16 * count`, writes the
next links, terminates the final node and returns zero. It requires a positive count
and sufficient arena space. Its pop helper returns null for an empty list; the
registration pop helper dereferences its head without an empty-list guard. These
original preconditions are preserved, even though `BSRegisterMessage` subsequently
checks the allocator's return value.

Thread lookup returns zero both for the first thread and for no match. Direct
thread lookup has no range check. Registration index conversion recognizes only
the `-1`/null sentinel and adds no validation for other out-of-range indices or
unrelated pointers. Object-index conversion remains original code. These are
32-bit GameCube runtime views and contracts, not portable containers or recovered
GameCube file layouts.

## Thread lifecycle, queued delivery and group filters

Seven more fragments reconstruct nine complete functions:

| Functions | Executable bytes | Verified behaviour |
| --- | ---: | --- |
| `BSCreateThread` / `BSDestroyThread` | 360 | Bind the initial state and its message registrations, execute the thread, and later return its registration nodes to the free list. |
| `BSMessageQueueMessage` | 248 | Append a delivery record to the fixed ring, returning 12 when the next tail would equal the head, otherwise zero. |
| `BSMessageProcessSingleMessageInQueue` | 200 | Consume one record, validate the receiver identity and current registration, and dispatch an eligible handler. |
| `BSMessageExecuteHandler` | 192 | Prepare the handler's stack arguments and code address, then call the interpreter. |
| `FindCurrentMessageListEntry` | 108 | Find a current registration with equal message ID, comparison word and comparison byte. |
| Three `BSMessage*Group*` functions | 592 | Test group membership, any shared group, or a shared group within the requested category. |

`BSCreateThread` confirms the original `BSCode_struct` tag from its exported
signature; `BSLevelView` remains a descriptive alias for that 28-byte runtime view.
The code-base table is at level `+0`, and the initial state ID is at `+0x16`.
The thread's formerly opaque first sixteen bytes hold frame, stack-top, context
and instruction pointers at `+0`, `+4`, `+8` and `+0x0c`. Creation sets the stack top
to one word above the current interpreter top and the frame one word above that,
registers each initial-state message, then calls `BSExecuteThread`. Destruction
removes each handler and recycles each list node. It does not clear the thread's
list pointer or restore the associated registration's flags; these differences
from `NEXTSTATE` are preserved.

The queue has 1,024 records with a verified 36-byte stride. Head and tail use the
same wrap rule at 1,024; one slot is reserved to distinguish a full ring. The
record holds sender and target script pointers at `+0`/`+4`, receiver and thread
pointers at `+8`/`+0x0c`, native object and context at `+0x10`/`+0x14`, a copied
receiver identity word at `+0x18`, a message pointer at `+0x1c`, and state ID at
`+0x20`. Its final two bytes remain opaque. The identity word is read from script
object `+0x24`; its wider lifecycle remains unestablished. These are descriptive
field names, not recovered historical declarations.

Consumption advances the head before checking that identity word. An unequal
word discards the record. Lookup walks the thread's current message list and
returns null immediately if a visited message has flag bit 2 set; this is not a
skip-to-the-next-entry condition. Otherwise it returns the first entry matching
the ID and both comparison fields. Delivery requires that entry and either an
equal state ID or flag bit 1 in its current message. Processing assumes the queue
is nonempty; no empty-ring guard is present in this function.

Execution resolves the receiver and thread again from the registration indices,
sets the thread frame and top to two words above the interpreter top, then writes
context, sender, target and native-object arguments into four successive slots.
The thread top advances to the fourth slot. The handler's code word selects a
code-base entry with its high byte and a word offset with its low 24 bits, as in
the accepted opcode handlers. `BSExecuteThread` itself remains original context.

The group filters call the original `TriggerObject::GetList("Group")`. Their
accesses establish a signed count followed by a variable tail of signed 32-bit
values in `FlexPropList`; the view is not a complete allocation or serialized
format. Membership compares each value's low byte against the unsigned high
halfword of the supplied match word. The shared-group filter compares complete
values. The category filter compares arithmetic-right-shifted value halves
against the unsigned high halfword of the match word, then requires equal full
values. Null script/native arguments and null lists follow the original checks;
`BSMessageAreInSameGroup` requires valid native objects once its script-object
arguments are nonnull. Negative counts naturally produce no iterations. The
shared `"Group"` literal and its alignment occupy eight fully compared read-only
bytes at `0x802a5274`; those bytes receive no code credit.

## Script timers

Twelve fragments reconstruct all twelve named routines in the original
`bstimer.cpp` code range, `0x8011dde0`–`0x8011e587`. The fragments use the same
ProDG 3.9.3 profile as the interpreter. Their complete code and 28 generated
read-only bytes match; no data, functions or padding are discarded. This is a
reconstruction of the named timer routines, not a claim to recover the original
translation unit, global definitions or historical source spelling.

| Functions | Executable bytes | Verified behaviour |
| --- | ---: | --- |
| `BSTimerGetMemoryRequirements`, `BSInitTimer`, `BSEndTimer` | 264 | Reserve a 7,104-byte arena, initialize 120 timer buckets and 256 event records, then later clear the three active table/pool pointers. |
| `BSUpdateTimer` | 332 | Advance the floating-point clock, visit crossed integer ticks, dispatch eligible events and recycle their records. |
| `BSRegisterTimerEvent` | 420 | Apply the replacement mode, handle event-memory references, and append a record to its time bucket. |
| `BSUnregisterTimerEvents`, `BSCancelTimerEvent` | 80 | Forward object-wide or object/event cancellation to the removal helpers. |
| `BSGetFreeTimerEvent`, `BSTimerRemoveTimerEvent` | 108 | Pop a free record or unlink a live record and push it back onto the free list. |
| `BSTimerRemoveDuplicateTimerInstances`, `BSTimerRemoveLatterTimerInstances`, `BSTimerRemoveTimerInstances` | 756 | Traverse buckets to remove matching records, optionally retaining earlier or equal timers. |

`ScriptTimers.h` records the independently checked GameCube storage. The original
`BSTimerEvent_struct` tag is present in exported signatures. Each record has a
24-byte stride: signed integer time at `+0`, event number at `+4`, object pointer
at `+8`, context pointer at `+0x0c`, a four-byte event-memory flag at `+0x10`, and
next pointer at `+0x14`. The two bytes at `+6` remain unknown. Each eight-byte bucket
contains head and tail pointers. Initialization independently confirms both strides
and both array lengths. Field names and the bucket-view name are descriptive;
these are runtime records, not recovered disc-file formats.

The original `ETimerReplaceMethod` tag is also preserved. Its descriptive enumerators
represent modes observed at direct callers and in the registration body:

- Mode 0 removes matching object/event records before insertion. The removal
  function's final integer argument is unused, including the `-1` supplied by
  cancellation.
- Mode 1 removes later matching records, and rejects the new timer if an earlier or
  equal matching record was found.
- Mode 2 appends without either replacement pass. A direct caller loads 2 at
  `0x80100778` before calling registration at `0x80100798`; callers also establish
  modes 0 and 1. Other values take the same no-replacement path in the observed body.

Registration computes the stored time by adding the integer delay to the float
clock and truncating to an integer, then selects the bucket with signed remainder
modulo 120. It preserves the `0xffffffff` object-identity rejection. The method-only
`BSUtilObjectInstanceMemoryAllocator` declaration supplies its original
`DoWeOwnThisMemory` call; it does not establish an allocator layout or permit local
allocation of that class. The original object named `g_pMemBlockAllocator`, timer
globals, arena allocator, event reference-count functions and event dispatcher
remain external storage/code and earn no credit from these fragments.

Several surprising original behaviours are retained. Removal scans use an inclusive
range of 121 ticks for a 120-bucket table, revisiting the first bucket. When mode 1
keeps an earlier/equal match, that branch sets its result to zero without advancing
the predecessor link or remembered tail. The reconstruction does not repair that
behaviour. `BSUpdateTimer` retains a record only when its integer time converted to
float is greater than the current clock; the dispatch condition is expressed as
`!(event->time > g_CurrentTime)` to preserve the original unordered-comparison
branch as well. Clock-to-integer conversions retain the original representability
preconditions; this is not a claim about useful behaviour for a NaN clock.

Rejected mode-1 registration checks event-memory ownership and decrements an owned
context without first incrementing it. Accepted registration increments an owned
context before testing the invalid object identity, then decrements it on that
rejection path. Removal decrements owned context memory before unlinking. These
orders are preserved. The free-list pop has no exhaustion check, and registration
adds no null-object, negative-delay or out-of-range-time guards. Shutdown clears the
three pointers without freeing the shared arena or resetting its clock/offset.

The generated pool contains the four-byte zero at `0x802a6234` and three eight-byte
integer-conversion constants at `0x802a6238`, `0x802a6240` and `0x802a6248`. All 28
bytes are compared and receive no executable-progress credit. Timer globals keep
the original private `bstimer.cpp` file-record scope in each manifest.

## Event lookup and music interfaces

`BSFindAnyEventHandler` searches every class level and present state, returning one
on the first matching event number and zero otherwise. This independently confirms
the research's 28-byte level stride, state-pointer array at level `+8`, halfword
state count at `+0x0c`, event-list pointer at state `+4`, byte event count at state
`+0x10`, eight-byte event stride and halfword event number at event `+4`. The class
holds its level pointer at zero and halfword level count at `+0x58`.

The local views expose only those accesses. Unknown bytes remain opaque, and the
views do not establish complete state/class allocations, historical member names,
or GameCube `.sin` serialization. Do not allocate objects from these prefixes.

The shared `ScriptBuiltins.h` view describes the 16-byte runtime table stride seen
in built-in dispatch and music wrappers. The function pointer is at zero. The music
wrappers read signed halfwords at `+0x0c` to locate arguments and at `+0x0a` to adjust
the stack after the call. The latter field is re-read after the music call, preserving
the original dependency on current table state. Other fields remain unknown.

`PathfinderSetLevel`, `PathfinderEvent` and `PathfinderSetLatency` each forward their
first integer argument to the corresponding `MUSIC_*` routine. Those music routines,
the table's initialization, and `PathfinderFadeVolume` remain original context.
Historical return-type spelling for ignored-return interfaces is not established
by this reconstruction.

## Verification and next work

All fifteen accepted `bsmachin.cpp` fragments, ten `bsmessage.cpp` fragments and
twelve `bstimer.cpp` fragments use ProDG **3.9.3** with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. This profile reproduces all 33
interpreter handlers, the nineteen thread/message/group helpers, all twelve timer
routines and the generated constants. Event lookup and music
built-ins retain their verified ProDG 3.8.1 profile.

The arithmetic work distinguishes these working profiles for the tested source:
ProDG 3.5, 3.5b140, 3.7 and 3.8.1 remove the initial float-temporary store from the
straightforward `ADD` candidate, producing 308 rather than 320 bytes. ProDG 3.9.3
preserves it and matches all four arithmetic bodies once the literal storage is
placed as described above. The previous 24 handlers also match with 3.9.3; changing
their compiler profile earns no new source credit. These comparisons establish a
working profile, not the historical compiler release or original source spelling.

No instructions are patched, no assembly bodies are substituted, and no functions
or sections are discarded or clipped. These fragments generate code and the 152
read-only constant/string bytes described above. Generated data earns no executable credit,
and all external storage remains original context.

The complete rebuilt 2,860,576-byte analysis image also matches the original,
including its header, allocated ELF bytes, entry point and BSS extent. Follow the
[build, test and snapshot process](Progress.md) for future changes. Runtime and
emulator behaviour remain untested. The opcode-handler set is complete, but the
interpreter as a whole is not: interpreter initialization/execution, class loading,
event dispatch, message-send filtering and music internals remain future work. The
[script](research/ps2/script-opcodes.md), [class-description](research/ps2/sin-class-descriptions.md)
and [music](research/ps2/pathfinder-music.md) research retains broader hypotheses and
cross-platform questions beyond the matched bodies here.
