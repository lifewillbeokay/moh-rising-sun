# Behaviour-script reconstruction

[kyleckroeger's PS2 research](research/README.md) supplied the opcode map,
class-description shapes and music-interface leads for this work. The accepted
bodies were independently reconstructed from the pinned GameCube GR8E69 executable
with Codex assistance. No PS2 source, offsets or file layout were assumed to apply
without checking the GameCube instructions and symbols.

## Accepted scope

384 fragments in `src/script/` reconstruct **619 functions and 107,608
executable bytes**: all **33 opcode handlers** (5,596 bytes), ten message-registration and index
helpers (964 bytes), nine thread/message-delivery and group-filter routines (1,700
bytes), event lookup (220 bytes), three music built-ins (372 bytes), all twelve named
timer routines (1,960 bytes), a spatial query (196 bytes), six game-object
default/list routines (132 bytes), three objective built-ins (368 bytes), three
ambient-sound built-ins (536 bytes), two value built-ins (144 bytes), and a
projectile aim helper with two projectile controls (832 bytes), and seven fog,
far-clip and particle-sorting controls (656 bytes), and 507 further built-in wrappers
and stubs (89,840 bytes; [see below](#built-in-wrappers-stubs-and-ai-behavior-controls)),
six follow-up wrappers (1,300 bytes; [evidence below](#follow-up-wrappers-from-pr-31)),
and five sound, path-flag and damage wrappers (1,340 bytes;
[evidence below](#sound-playback-path-flags-and-location-damage)), and four attachment,
enemy-count and texture-hash wrappers (1,212 bytes;
[evidence below](#soldier-attachments-enemy-counts-and-texture-crc)), and four startup-allocation/state
helpers (240 bytes; [see below](#startup-allocation-and-system-state)).
Their exact symbols, ranges and dependencies are in
`config/GR8E69/script_*.json` and `config/GR8E69/bsgo_*.json`. Original file markers
support the `bsmachin.cpp`,
`bsmessage.cpp`, `bsfile.cpp`, `bsbifunc.cpp`, `bstimer.cpp` and `objcreate.cpp`
groupings; these fragments are not complete original translation units. The
BSGO base defaults have no established original file owner. Private globals retain
their original file-record scope in the manifests and remain original storage, with no source/data
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

The [trigger startup-sizing helpers](TriggerObject.md#startup-sizing-player-starts-and-machine-gun-search)
now also establish the instance-memory size at class `+0x54`. The original
`BSUtilGetObjectInstanceMemory` independently uses that word as an allocation
size. `ScriptRuntime.h` exposes it within the existing gap, preserving level count
at `+0x58`, shared values at `+0x60` and the existing prefix extent. The member
name is descriptive; this is runtime storage evidence, not a `.sin` file layout.
The helper functions are counted under trigger reconstruction, not again here.

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
first integer argument to the corresponding `MUSIC_*` routine. Those music routines
and the table's initialization remain original context. `PathfinderFadeVolume`
now has matching source in the [follow-up batch](#follow-up-wrappers-from-pr-31).
Historical return-type spelling for ignored-return interfaces is not established
by this reconstruction.

## Objective, ambient-sound and value built-ins

Five additional fragments reconstruct eight complete `bsbifunc.cpp` wrappers,
using ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates`.
They were reconstructed independently from GR8E69 with Codex assistance and reuse
the established objective, sound, script-object and vector interfaces.

| Manifest | Functions (`BIFunc_` prefix omitted) | Code bytes |
| --- | --- | ---: |
| `script_objective_show` | `ShowObjective` | 132 |
| `script_objective_queries` | `GetObjective`, `CheckMainObjectiveStatus` | 236 |
| `script_ambience` | `AmbientTrack_Select`, `AmbientTrack_Volume` | 344 |
| `script_ambience_location` | `AmbientTrack_Azimuth` | 192 |
| `script_music_values_rotation` (value functions) | `FloatToInt`, `InitToInvalid` | 144 |

The wrappers locate the first argument one four-byte slot above the argument base
calculated from the table's signed `argumentCount`. After an external call they
re-read the current built-in index, table pointer and signed `stackAdjustment`.
Returning wrappers write their result at the adjusted top; they do not perform
an additional push. Valid table indices and stack storage remain preconditions.

`ShowObjective` forwards the integer ID to `g_Objectives.ShowObjective`.
`GetObjective` returns `GetObjectiveStatus`, and `CheckMainObjectiveStatus` returns
`CheckIfAllMainObjectivsAreCompleted` without reading an argument. All three callees
already have matching source. The original shared `g_Objectives` symbol is at
`0x803e81a4`, with a 212-byte symbol extent; the header adds only an external
declaration and no new allocation or data credit. The wrappers add no ID validation
beyond the existing [objective methods](Objectives.md).

`AmbientTrack_Select` forwards its integer track ID. `AmbientTrack_Volume` converts
an integer percentage to single precision, multiplies by `0.01f`, then clamps to
zero and one before calling the existing volume setter. The negated `>=` and `<=`
tests preserve the original comparison instructions. No range restriction is
imposed on the input integer before conversion.

`AmbientTrack_Azimuth` uses its integer argument as a flag. A nonzero value obtains
the current `g_pBSObject`'s native trigger position and passes a temporary
`CVector3` to `AmbientTrack_SetLocation`. Zero passes a null position, which the
existing sound helper uses to select player-relative ambience. This wrapper
does not calculate an angle despite its name. It uses the shared script object's
native pointer at `+8` and adds no null checks on the nonzero path; the unused
second wrapper parameter is not the position source.

`FloatToInt` interprets the first slot's bits as a float and truncates it to a
signed integer with the target's conversion instruction. It adds no clamp for
out-of-range or nonfinite values. `InitToInvalid` writes zero at the adjusted top;
its name does not justify changing that value to `-1` or `0xffffffff`.

All 24 generated read-only bytes are compared separately from code credit:
the volume fragment has a 16-byte pool at `0x802a5de0` (integer-conversion constant,
`0.01f`, zero), followed by its four-byte `1.0f` at `0x802a5df0`. The source gives
that upper bound a descriptive `.rodata.volume_maximum` storage annotation so
the fragment does not add alignment over the next routine's literal. The azimuth
wrapper generates that separate four-byte vector-constructor `1.0f` at
`0x802a5df4`. The annotation is not a recovered historical identifier. No emitted
data or padding is discarded, and no instruction is patched.

The initial experiments for `AddObjective`, `ChangeObjectivePrompt`, `SetObjective`,
the random-value wrappers and `PathfinderFadeVolume` had argument-load scheduling
and register-copy differences. The [larger built-in batch](#built-in-wrappers-stubs-and-ai-behavior-controls)
and [follow-up work](#follow-up-wrappers-from-pr-31) subsequently reconstructed them.
Only the accepted bodies and their complete generated sections receive credit;
the complete source-built analysis image is compared separately.
Runtime and emulator behavior remain untested.

## Script game-object bridge

Three additional fragments use ProDG 3.8.1 with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`:

| Unit | Functions | Original start | Code bytes |
| --- | --- | --- | ---: |
| `script_position` | `GetScriptPosition` | `0x8010389c` | 196 |
| `bsgo_basic_defaults` | `BSGO_Basic::Destroy`, `GetScriptData`, `GetSceneNode`, `GetProximityData` | `0x80283530` | 28 |
| `bsgo_object_list` | `AddObjectToList`, `RemoveObjectFromList` | `0x8012b380` | 104 |

`BSObject` contains a 24-byte `WeakPtr<BSGO_Basic, 8>` beginning at `+0x0c`;
its subject pointer is therefore at `+0x20`. This replaces the previously opaque
24-byte interval without moving the queue identity at `+0x24`. The reference uses
the shared [observer layout](Observers.md), not an owning raw pointer.

Evidence extends beyond the spatial callers. The original `CreateBSObject` at
`0x800f6b5c` unlinks the embedded observer's node at object `+0x14`, stores its
BSGO argument at object `+0x20`, and registers the observer at object `+0x0c` with
`ISubject::AddObserver` when that argument is nonnull. The named weak-pointer
vtable at `0x802e8228` and existing matched specialization of `HandleEvent` at
`0x80286664` corroborate the tag and destruction-event value 8. `CreateBSObject`
and the vtable remain original context and earn no new source credit. The inline
boolean conversion and arrow operator are descriptive source factoring of the
observed null test and subject access, not recovered historical declarations.

`BSGameObject.h` declares a scoped `BSGO_Basic : ISubject` prefix. The base table
at `0x802e5048`, player table at `0x802e8270`, and dummy table at `0x802e6bf0`
independently support the same virtual order:

| Table adjustment/function offsets | Method |
| --- | --- |
| `+0x08` / `+0x0c` | Inherited `ISubject::MarkForDestruction` |
| `+0x10` / `+0x14` | Destructor |
| `+0x18` / `+0x1c` | `Destroy` |
| `+0x20` / `+0x24` | `GetScriptData` |
| `+0x28` / `+0x2c` | `GetSceneNode` |
| `+0x30` / `+0x34` | `GetProximityData` |

`Destroy` is empty; the three base accessors return null. The script-data and
proximity-data returns use `void *` as pointer ABI placeholders: their actual
pointee types and extents remain unknown. `GetSceneNode` returns `ISceneNode *`,
corroborated by the subsequent scene virtual calls. No BSGO destructor or table is
emitted by these fragments, and no generated output is discarded.

The list routines establish next and previous BSGO pointers at `+0x0c` and `+0x10`.
The four bytes at `+8` remain opaque. Insertion sets the old head's previous link,
stores the old head as the new object's next link, clears its previous link and
replaces the head. Removal reconnects present neighbors and advances the head
when removing it; it deliberately leaves the removed object's links unchanged.
The view ends at `+0x14` and does not establish a complete allocation or derived
object layout. These object-list links are separate from the subject's observer
list at `+4`.

`GetScriptPosition` gives an explicitly supplied scene node priority. Without
one, it consults a nonnull script object's nonnull weak game-object reference for
a scene node. A node supplies its position through the existing scene virtual
slot at table `+0xa8` / `+0xac`. If there is no node, a nonnull script object's
nonnull native trigger supplies the position. With neither source it leaves the
output untouched. No vector initialization is added. The routine's ignored return
is modeled as `void`; original return spelling is not encoded in its symbol.

The [AI target position and forward queries](Targeting.md) share these interfaces.
They preserve two `GetSceneNode` calls where the original does, unlike the one-call
lookup in `GetScriptPosition`. The BSGO prefix, weak pointer and BSObject prefix
have target-compiler size checks; none authorizes allocating the larger original
runtime objects from these scoped views.

## Projectile aim and controls

Three fragments add **832 executable bytes** from the `bsbifunc.cpp` marker
interval. The local `AdjustAim` symbol also belongs to its original file record;
it retains internal linkage. These are independent reconstruction fragments,
not recovered original source-file boundaries.

| Manifest | Original function | Address | Code bytes |
| --- | --- | --- | ---: |
| `script_adjust_aim` | `AdjustAim__FR8CVector3f` | `0x8010cf24` | 664 |
| `script_grenade_timer_get` | `BIFunc_GetGrenadeTimer__FPPiPv` | `0x8010bdcc` | 92 |
| `script_fake_bullets` | `BIFunc_AllowFakeBullets__FPPiPv` | `0x8010da60` | 76 |

`AdjustAim` starts with a +Z reference axis, crosses the input direction with
that axis and normalizes the result, then crosses that result with the input
direction and normalizes the second basis vector. It draws two values between
negative and positive half the supplied spread, scales the two basis vectors,
adds them to the direction in order, and normalizes the final direction.
Each normalization skips division when its computed length equals zero. There
is no fallback axis for degenerate inputs, no spread clamp, and no zero-spread
shortcut: it still makes both random calls. Only x/y/z are changed; the input's
fourth word is preserved. The shared [vector declaration](Matrix.md#cvector3-layout)
and its existing compound operators reproduce the original temporaries and
arithmetic. `Cross` and `Normalize` are descriptive inline helpers, not recovered
historical names. Their intermediate copies and operation order are retained.

The zero, one and half literals occupy 12 fully compared bytes at `0x802a5c90`.
The same batch reconstructs `MathFunRandomReal` (108 bytes plus 12 literal bytes),
so both random calls now resolve to accepted source. Its always-consume behavior
and literal placement are described in [MathFun.md](MathFun.md).
The original direct-branch graph identifies three callers of `AdjustAim`:
`BIFunc_FireProjectile`, `BIFunc_FireAllTurret01s` and
`BIFunc_FireProjectileAtPath`. `MathFunRandomReal` has 41 distinct direct callers.
Those relationships guided this work; they do not establish runtime frequency
or award credit to the remaining callers.

`GetGrenadeTimer` treats its first script slot as a bullet pointer, reads float
`+0xb0`, truncates it to a signed integer and writes the result to the adjusted
stack top. It reuses `CBullet::field_b0` in the existing scoped
[bullet view](Bullets.md), without redefining the class or claiming a complete
allocation. Valid storage and representable conversion remain original
preconditions; no null check or saturation is added.

`AllowFakeBullets` copies the first integer slot directly into the original
four-byte `g_bFakeBulletsEnabled` at `0x802c47a4`, then adjusts the stack.
It does not normalize the value to zero or one. That global remains original
storage and earns no data credit. The timer setter remains unmatched; its
separate experiment is not part of the accepted getter fragment.

These functions were reconstructed from the pinned GameCube executable with
Codex assistance. A local Ghidra trial helped identify the projectile helper and
its dependencies; original instructions, symbols and complete compiled-byte
comparisons supplied the acceptance evidence. All four new objects use ProDG
3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates`, without stripping,
patching instructions or substituting assembly. This is a working compiler
profile, not proof of the historical release.

## Fog queries and rendering controls

Six fragments reconstruct seven complete built-ins, adding **656 code bytes**.
They were independently reconstructed from GR8E69 with Codex assistance; the
fog setup routines were inspected as layout evidence and remain original code.

| Manifest | Functions (`BIFunc_` prefix omitted) | Original start | Code bytes |
| --- | --- | --- | ---: |
| `script_fog_distances` | `GetFogStart`, `GetFogEnd` | `0x8011509c` | 152 |
| `script_fog_red` | `GetFogRed` | `0x80115134` | 108 |
| `script_fog_green` | `GetFogGreen` | `0x801151a0` | 112 |
| `script_fog_blue` | `GetFogBlue` | `0x80115210` | 108 |
| `script_particle_sorting` | `ReverseParticleVsTranslucentSorting` | `0x80115664` | 76 |
| `script_far_clip` | `SetFarClipPlane` | `0x80115820` | 100 |

The queries adjust the stack first, then read the fog state and write float bits
into the adjusted integer slot. Start and end are copied as floats, without
integer conversion. RGB channels are extracted from the packed word and converted
to floats on the **0–255** scale; there is no division by 255. The green extraction
retains the original halfword load and low-byte mask. Its expression, like red and
blue, is a shift/mask of the packed colour rather than a new byte-field layout.

`Fog.h` describes only a prefix of the original 96-byte `g_fog` symbol at
`0x802fad40`. The class tag `CFog` is corroborated by the original `SetW` method
called with this object. The prefix ends at `0x5c`; unknown bytes remain opaque,
and it must not be used to allocate or construct the original object.

| Offset | Descriptive member | Independent evidence |
| --- | --- | --- |
| `0x44` | `colour` | Queries read its high three bytes; `SetFogParams` and `SetDefaultFogParams` store a complete RGBA word here before converting it for EAGL. |
| `0x50` | `start` | Query reads a float; both setup routines store it and pass it to `SetFogStart`. |
| `0x58` | `end` | Query reads a float; both setup routines store it and pass it to `SetFogEnd`. |

The member names and packed-word spelling are reconstruction choices. No fog
storage, setup code or EAGL conversion earns credit from this prefix. The original
setup methods at `0x8011527c` and `0x801156b0` remain useful follow-ups.

The sorting wrapper copies its first integer argument without boolean
normalization to `CDrawContext::b_ReverseParticleVsTranslucentSorting`, the original
four-byte object at `0x802c4b00`. Its local class declaration exposes only this
static member and establishes no instance layout. It then adjusts the script stack.

The far-clip wrapper interprets its first argument as float bits, stores it in
the existing `CCamera::farClip`, clears world-to-clip and camera-to-clip validity,
and writes one to `field_160`. It leaves world-to-camera validity untouched and
adds no clamp or projection rebuild. The original 704-byte `g_camera` symbol at
`0x803e6f00` remains external; the existing scoped [camera view](Camera.md) is reused
without claiming that it describes the complete object.

The distance queries use ProDG **3.9.3** with
`-O2 -G0 -fno-exceptions -fno-implicit-templates`. This preserves their original
float temporary store/load; ProDG 3.8.1 collapses that same source to a shorter
integer transfer. The other five functions use the existing **3.8.1** profile.
These are working profiles, not evidence of different historical releases.
Each colour query emits its own complete eight-byte unsigned-integer conversion
constant, at `0x802a5ed8`, `0x802a5ee0` and `0x802a5ee8`. All 24 bytes are verified
separately and excluded from code credit. No functions, sections or padding are
discarded, and no instructions are patched.

## Built-in wrappers, stubs and AI behavior controls

PR [#31](https://github.com/lifewillbeokay/moh-rising-sun/pull/31) introduced
316 fragments containing **507 functions and 89,840 executable bytes** from the
`bsbifunc.cpp` marker interval. All use the ProDG 3.8.1 profile of the earlier
built-ins and are independent fragments, not recovered file boundaries.

The work was guided by kyleckroeger's CC0
[Frontline GameCube reconstruction](https://github.com/kyleckroeger/moh-frontline-decomp)
(GMFE69, CodeWarrior). Frontline's source names the built-ins' globals, members and
callees, but its bytes cannot match ProDG output; every body here was checked
against the GR8E69 instructions, and many differ from Frontline (see below).

**Stubs.** Seventy-seven built-ins only pop their arguments, return zero, return -1,
or return `g_bCoopMultiplayer`; most are co-op, split-screen, tutorial and debug
entries that this single-player build leaves empty. `RegisterMovie` and `BotDeath`
in the same block forward to `ShellManager::SetMovieName` and `BotDead`.
`EnterAStarVolume`, `ExitAStarVolume` and `SoldierSetScriptHighLevelState` never
pop their arguments; the reconstructions keep that.

**Argument access.** Two spellings reproduce the observed loads. Single-argument
built-ins read `args = *stack - argumentCount` and index it. Built-ins with two or
more arguments read each slot as `*(*stack - (count - n))` from a saved
`argumentCount`; this produces the duplicated base register (`mr r11,r9`) the
original keeps, which the `args[n]` form does not. Booleans that the callee receives
unconverted are read through a `bool *` view of the slot (GCC 2.x `bool` is four
bytes on this target).

**AI object modules.** The AI built-ins reach `CAIObject` through
`ISceneNode::GetAIObject` and pass the addresses of embedded modules: targeting at
`+0xa0`, locomotion at `+0x1b0`, A* area path finding at `+0x230` and the spline-path
module at `+0x2f0`. The scoped view in `AITargeting.h` now embeds the existing module
prefixes at those offsets with opaque gaps between them. It still does not establish
the complete allocation. Frontline's equivalents manipulate walk-state fields on
the AI object directly, so Rising Sun's module calls are a real design difference,
not a naming one.

**Behaviors.** `CAIObject::FindMatchingBehavior` takes the selectors 1 (buddy lead),
2 (buddy follow), 3 (mounted machine gun), 4 (cover point), 5 (scripted), 8 (sword)
and 10 (grenade response). Each value is the return of that class's `GetType`.
`AIBehavior.h` declares method-only views; the built-ins convert the returned
pointer without adjustment. Null checks, the weak game-object test, and whether a
built-in pops its arguments differ between otherwise similar wrappers, and each is
kept as observed.

**Player and weapon.** Player flags at `+0xd4c` gain `canCrouch` (bit 26),
`wielding` (bit 23) and `triggerChecks` (bit 22) beside the existing motion-shake
bit. Bit 27 of the word at `+0xd44` is read as "aiming". The weapon view adds the
property record pointer at `+0x300` (refire rate `+6`, projectile flag `+0xe`) before
the reload bit at `+0x350`. `TriggerObject` `+4` is its script object, used by
`SendEventParent`. The schedule-stop built-ins name their file-local
schedulers through `local_externals`.

**Scene objects and virtual slots.** `SceneObjects.h` declares member-only views of
`CStaticObject`, `CAnimObject` and `CSoldierObject`; each starts at the
`ISceneNode` subobject the casts return. `CAnimObjectInterface` names slots 89-113
from the original `CSoldierObject` table (0x802e5498): animation state, animation
callbacks, fades and the aim modifier, among others. Behavior views place their
table pointer at `+0xc` after twelve bytes of base data; slot 4 is
`UpdateHeuristicScore`, which the force-to-cover and force-to-MMG built-ins call.
The player view adds the clip settings at `+0x1150`, the heads-up display at
`+0x1410` and the look-at controller at `+0x1cf0`; the soldier view adds the weapon
slot table at `+0xb84` and the current slot at `+0xf20`.

**Observed rules.** A grenade's detonation time is the script value multiplied by
60. Tactical-position and run-away move points use move type 3 with an arrival
distance of 1.2. The A* last-position walk starts only when the closest network
point is more than 1 unit from the AI (squared distance 1.0), and
`AtClosestPointToPlayerOffNetwork` tests a squared distance of 4.0.
`GetClassFromBulletType` maps script bullet types to weapon classes (0, 14, 15, 19
and 20 to 3; 1, 4 and 6-9 to 2; 2 to 6; 3 and 21-24 to 7; 5, 13 and 16 to 5; 11, 17,
18 and 25 to 1; 12 to 4; anything else to 0), and weapon class 20 counts as not
holding a weapon. Hit directions map 0, 1, 2, 3 to HUD indicators 1, 3, 4, 2.
Triggered hints occupy a 20-entry table; removal shifts later entries down. Film
canister type 0 uses the first mask and any type other than 0 or 1 uses the second;
type 1 is ignored, as compiled. Awards apply only to levels 0-10.
`SetMotionBlurDuration` clamps the blur amount to [0, 1] and its times to zero or
more. Several built-ins (`EnterAStarVolume`, `ExitAStarVolume`,
`SoldierSetScriptHighLevelState`, `ActivateSoundBuss`, `RegisterAStarVolume`,
`RegisterAStarLinkPoint`, `Soldier_EnableSkeletalPartition` and
`LetterboxTransition`) never pop their arguments; the reconstructions keep that.

**Code-generation notes.** Literal values are checked by the full build, not by the
instruction comparison, so constants were confirmed against the original bytes.
`ActivateSoundBuss` keeps the unfolded `g_SoundMan` address only when it is taken
through a pointer. `LoadCompartment`'s `memset` call sets CR1 as for a variadic
callee, so its declaration is variadic. Switches whose return blocks appear in
result order are written with their case groups in that order.

| Manifest | Built-ins | Address | Code bytes |
| --- | --- | --- | ---: |
| `script_is_alarm_activated` | `IsAlarmActivated`, `SetAlarmActivated` | `0x800d83ac` | 136 |
| `script_asset_crc` | `AssetCRC` | `0x800f8d80` | 44 |
| `script_enable_partition` | `EnablePartition` | `0x800f968c` | 252 |
| `script_mp_set_aiming` | `MPSetAiming` | `0x800f9788` | 180 |
| `script_enable_partition_on_other` | `EnablePartitionOnOther`, `SetAnimationStatePartitionOnOther` | `0x800f983c` | 448 |
| `script_set_animation_state_on_other` | `SetAnimationStateOnOther` | `0x800f99fc` | 160 |
| `script_set_animation_on_hier_object` | `SetAnimationOnHierObject` | `0x800f9a9c` | 236 |
| `script_set_animation_state` | `SetAnimationState` | `0x800f9b88` | 184 |
| `script_set_animation_state_on_partition` | `SetAnimationStateOnPartition` | `0x800f9c40` | 244 |
| `script_play_weapon_animation` | `PlayWeaponAnimation` | `0x800f9d34` | 204 |
| `script_play_turn_animation` | `PlayTurnAnimation` | `0x800f9f2c` | 236 |
| `script_set_anim_callback` | `SetAnimCallback`, `SetAnimCallbackOnOther` | `0x800fa018` | 328 |
| `script_set_anim_adjust` | `SetAnimAdjust` | `0x800fa160` | 288 |
| `script_set_allow_animation_translation` | `SetAllowAnimationTranslation` | `0x800fa280` | 340 |
| `script_set_japanese` | `SetJapanese` | `0x800fa3d4` | 72 |
| `script_do_something_interesting` | `DoSomethingInteresting` | `0x800fa41c` | 148 |
| `script_ai_setup_target_match_list` | `AISetupTargetMatchList`, `AIChooseTarget` | `0x800fa4b0` | 364 |
| `script_choose_target_by_id` | `ChooseTargetByID` | `0x800fa61c` | 296 |
| `script_ai_choose_target_by_id` | `AIChooseTargetByID`, `Soldier_AIChooseTargetByID` | `0x800fa744` | 348 |
| `script_register_location_target` | `RegisterLocationTarget`, `DeRegisterLocationTarget` | `0x800fa8a0` | 288 |
| `script_set_location_target` | `SetLocationTarget` | `0x800fa9c0` | 200 |
| `script_ai_set_hearing_radius` | `AISetHearingRadius`, `AIGetSoldierMatchListStruct` | `0x800faa88` | 248 |
| `script_ai_start_spline_path_walk` | `AIStartSplinePathWalk` | `0x800fab80` | 192 |
| `script_is_on_astar_mesh` | `IsOnAStarMesh` | `0x800fac40` | 204 |
| `script_is_nearby_position_on_astar_mesh` | `IsNearbyPositionOnAStarMesh` | `0x800fad0c` | 212 |
| `script_soldier_ai_start_spline_path_walk` | `Soldier_AIStartSplinePathWalk` | `0x800fade0` | 228 |
| `script_ai_stop_spline_path_walk` | `AIStopSplinePathWalk` | `0x800faec4` | 116 |
| `script_soldier_ai_stop_spline_path_walk` | `Soldier_AIStopSplinePathWalk` | `0x800faf38` | 180 |
| `script_ai_continue_spline_path_walk` | `AIContinueSplinePathWalk` | `0x800fafec` | 116 |
| `script_soldier_ai_continue_spline_path_walk` | `Soldier_AIContinueSplinePathWalk` | `0x800fb060` | 180 |
| `script_ai_loop_spline_path_walk` | `AILoopSplinePathWalk` | `0x800fb114` | 116 |
| `script_ai_get_distance_to_target_xyz` | `AIGetDistanceToTargetXYZ` | `0x800fb240` | 128 |
| `script_ai_start_astar_path_walk` | `AIStartAStarPathWalk` | `0x800fb2c0` | 172 |
| `script_ai_start_astar_path_walk_to_last_position_on_network` | `AIStartAStarPathWalkToLastPositionOnNetwork`, `AtClosestPointToPlayerOffNetwork` | `0x800fb36c` | 620 |
| `script_ai_start_astar_path_walk_to_move_point` | `AIStartAStarPathWalkToMovePoint`, `AIStopAStarPathWalk`, `AIStartCoverPointWalk`, `AIStopCoverPointWalk` | `0x800fb5d8` | 520 |
| `script_ai_invalidate_cover_point_by_id` | `AIInvalidateCoverPointByID` | `0x800fb7e0` | 132 |
| `script_ai_invalidate_cover_point` | `AIInvalidateCoverPoint` | `0x800fb864` | 136 |
| `script_ai_match_object_id` | `AIMatchObjectID` | `0x800fb8ec` | 204 |
| `script_ai_find_tactical_position` | `AIFindTacticalPosition` | `0x800fb9b8` | 288 |
| `script_arrived_at_coverpoint` | `ArrivedAtCoverpoint`, `EndCoverpoint` | `0x800fbad8` | 368 |
| `script_ai_set_run_away_movepoint` | `AISetRunAwayMovepoint` | `0x800fbc48` | 308 |
| `script_register_nav_point` | `RegisterNavPoint` | `0x800fbd7c` | 132 |
| `script_get_nav_point` | `GetNavPoint` | `0x800fbe00` | 128 |
| `script_ai_fire_at_current_target` | `AIFireAtCurrentTarget` | `0x800fbe80` | 172 |
| `script_soldier_ai_fire_at_current_target` | `Soldier_AIFireAtCurrentTarget` | `0x800fbf2c` | 208 |
| `script_ai_fire_mmg_at_current_target` | `AIFireMMGAtCurrentTarget` | `0x800fbffc` | 248 |
| `script_ai_collision_test` | `AICollisionTest`, `Soldier_AICollisionTest` | `0x800fc0f4` | 412 |
| `script_set_move_point` | `SetMovePoint` | `0x800fc290` | 184 |
| `script_ai_set_move_point_on_other` | `AISetMovePointOnOther`, `AIStartMovePointWalk` | `0x800fcc64` | 404 |
| `script_ai_stop_move_point_walk` | `AIStopMovePointWalk`, `AIContinueMovePointWalk` | `0x800fcdf8` | 232 |
| `script_ai_snap_to_move_point` | `AISnapToMovePoint`, `AISnapToMovePointAndAngle` | `0x800fcee0` | 436 |
| `script_ai_get_distance_to_move_point` | `AIGetDistanceToMovePoint` | `0x800fd094` | 128 |
| `script_ai_get_pan_angle_to_move_point` | `AIGetPanAngleToMovePoint` | `0x800fd114` | 288 |
| `script_soldier_ai_get_distance_to_move_point` | `Soldier_AIGetDistanceToMovePoint` | `0x800fd234` | 200 |
| `script_ai_send_flag` | `AISendFlag` | `0x800fd2fc` | 320 |
| `script_ai_set_status_var` | `AISetStatusVar` | `0x800fd43c` | 396 |
| `script_soldier_setup_grenade_shot` | `Soldier_SetupGrenadeShot` | `0x800fd708` | 172 |
| `script_set_gravity` | `SetGravity`, `SetGroundCheck`, `InduceGroundCheck` | `0x800fd7b4` | 452 |
| `script_set_hd_collision` | `SetHDCollision` | `0x800fd978` | 288 |
| `script_set_collision_to_player` | `SetCollisionToPlayer` | `0x800fda98` | 224 |
| `script_set_can_be_pushed` | `SetCanBePushed` | `0x800fdb78` | 200 |
| `script_set_in_shadow` | `SetInShadow` | `0x800fdc40` | 224 |
| `script_enter_astar_volume` | `EnterAStarVolume`, `ExitAStarVolume` | `0x800fdd20` | 320 |
| `script_register_astar_link_point` | `RegisterAStarLinkPoint`, `RegisterAStarVolume` | `0x800fde60` | 192 |
| `script_disable_collision` | `DisableCollision` | `0x800fdf20` | 136 |
| `script_set_collision` | `SetCollision` | `0x800fdfa8` | 480 |
| `script_path_mech_allow_collision_damage` | `PathMechAllowCollisionDamage`, `SetHeadAsRoot`, `SetPlayerCollision` | `0x800fe188` | 472 |
| `script_send_event_self` | `SendEventSelf` | `0x800fe360` | 116 |
| `script_send_event_weapon_immediate` | `SendEventWeaponImmediate` | `0x800fe3d4` | 576 |
| `script_send_event_parent` | `SendEventParent` | `0x800fe614` | 120 |
| `script_send_event_parent_immediate` | `SendEventParentImmediate` | `0x800fe68c` | 120 |
| `script_send_event` | `SendEvent`, `SendEventImmediate`, `SendEventDelayed`, `SendMessage` | `0x800fe704` | 660 |
| `script_send_message_via_proxy` | `SendMessageViaProxy` | `0x800fe998` | 148 |
| `script_send_messages` | `SendMessages` | `0x800fea2c` | 224 |
| `script_match_group` | `MatchGroup` | `0x800feb0c` | 136 |
| `script_get_cookie` | `GetCookie`, `SetCookie` | `0x800feb94` | 364 |
| `script_get_current_frame_number` | `GetCurrentFrameNumber` | `0x800fed00` | 60 |
| `script_player_is_my_weapon_a_projectile` | `PlayerIsMyWeaponAProjectile`, `SetPlayerWielding`, `PlayerFireMyWeapon` | `0x800fed3c` | 352 |
| `script_player_reload` | `PlayerStartReloading`, `PlayerReloadSingle`, `PlayerClearReloading`, `PlayerFinishedReloading` | `0x800fee9c` | 472 |
| `script_player_set_camera_shake` | `PlayerSetCameraShake`, `PlayerStartCameraShake`, `PlayerStopCameraShake` and 2 more | `0x800ff074` | 2060 |
| `script_player_can_crouch` | `PlayerCanCrouch` | `0x800ff880` | 156 |
| `script_player_motion_shake` | `PlayerMotionShake` | `0x800ff91c` | 180 |
| `script_player_screen_flash` | `PlayerScreenFlash`, `PlayerScreenFadeIn`, `PlayerScreenFadeOut` | `0x800ff9d0` | 540 |
| `script_texture_fade` | `PlayerTextureFadeIn`, `PlayerTextureFadeOut` | `0x800ffbec` | 96 |
| `script_player_start_path` | `PlayerStartPath` | `0x800ffc4c` | 380 |
| `script_player_stop_path` | `PlayerStopPath` | `0x800ffdc8` | 104 |
| `script_player_death` | `PlayerDeath` | `0x800ffe30` | 164 |
| `script_player_get_weapon_refire_rate` | `PlayerGetWeaponRefireRate` | `0x800ffed4` | 136 |
| `script_player_add_hit_direction` | `PlayerAddHitDirection` | `0x800fff5c` | 244 |
| `script_is_player_holding_weapon` | `IsPlayerHoldingWeapon` | `0x80100050` | 92 |
| `script_is_player_aiming` | `IsPlayerAiming` | `0x801000ac` | 120 |
| `script_is_player_disguised` | `IsPlayerDisguised` | `0x80100124` | 196 |
| `script_is_player_crouching` | `IsPlayerCrouching`, `SetPlayerFallingDamage` | `0x801001e8` | 332 |
| `script_make_player_invincible` | `MakePlayerInvincible` | `0x80100334` | 148 |
| `script_player_respawn` | `PlayerRespawn` | `0x801003c8` | 112 |
| `script_player_get_weapon_class` | `PlayerGetWeaponClass` | `0x80100438` | 144 |
| `script_player_action_stubs` | `PlayerGeneratedTrigger`, `SetFullscreenMode`, `PlayerToggleDuckForShoulderClimb` and 4 more | `0x801004c8` | 344 |
| `script_play_facial_animation` | `PlayFacialAnimation` | `0x80100620` | 132 |
| `script_register_timer_event` | `RegisterTimerEvent` | `0x801006a4` | 152 |
| `script_register_multiple_timer_event` | `RegisterMultipleTimerEvent` | `0x8010073c` | 152 |
| `script_cancel_timer_event` | `CancelTimerEvent` | `0x801007d4` | 132 |
| `script_enable_non_vis_destruct` | `EnableNonVisDestruct` | `0x80100858` | 188 |
| `script_player_has_melee_weapon` | `PlayerHasMeleeWeapon`, `PlayerCanSwitchWeapon` | `0x80100914` | 260 |
| `script_player_switch_weapons` | `PlayerSwitchWeapons` | `0x80100a18` | 160 |
| `script_player_has_grenade` | `PlayerHasGrenade` | `0x80100ab8` | 112 |
| `script_player_throw_grenade` | `PlayerThrowGrenade` | `0x80100b28` | 128 |
| `script_player_reset_grenade_count` | `PlayerResetGrenadeCount` | `0x80100ba8` | 128 |
| `script_get_random_integer` | `GetRandomInteger` | `0x80100c28` | 176 |
| `script_get_random_float` | `GetRandomFloat` | `0x80100cd8` | 160 |
| `script_get_my_instance_data` | `GetMyInstanceData` | `0x80100d78` | 128 |
| `script_get_instance_data_for` | `GetInstanceDataFor` | `0x80100df8` | 136 |
| `script_bs_object_as_scene_node` | `BSObject_AsSceneNode` | `0x80100e80` | 180 |
| `script_create_child_root_hier_object` | `CreateChildRootHierObject`, `CreateChildSubHierObject`, `CreateChildSubHierAnimObject` | `0x80100f34` | 636 |
| `script_swap_hier_object` | `SwapHierObject` | `0x801011b0` | 164 |
| `script_destroy_sub_hier_object` | `DestroySubHierObject` | `0x80101254` | 188 |
| `script_create_child` | `CreateChild` | `0x80101310` | 316 |
| `script_destroy_child` | `DestroyChild` | `0x8010144c` | 232 |
| `script_destroy_object` | `DestroyObject` | `0x80101534` | 268 |
| `script_destroy_script` | `DestroyScript` | `0x80101640` | 96 |
| `script_destroy_dump_object` | `DestroyDumpObject` | `0x801016a0` | 124 |
| `script_can_i_create_child` | `CanICreateChild` | `0x8010171c` | 108 |
| `script_attach` | `Attach`, `AttachChild`, `AttachParent` | `0x80101788` | 572 |
| `script_register_distance_to_target` | `RegisterDistanceToTarget`, `RegisterCheckObjectsInsideAngularRange`, `RegisterCheckObjectsOutsideAngularRange` and 4 more | `0x801019c4` | 1616 |
| `script_stop_check_objects_inside_range_secondary` | `StopCheckObjectsInsideRange_Secondary`, `StopCheckObjectsOutsideRange_Secondary` | `0x80102014` | 344 |
| `script_set_game_fov` | `SetGameFOV` | `0x8010216c` | 160 |
| `script_change_clip_plane` | `ChangeClipPlane` | `0x8010220c` | 172 |
| `script_change_letterbox_width` | `ChangeLetterboxWidth` | `0x801022b8` | 124 |
| `script_register_target_search` | `RegisterTargetSearch` | `0x80102334` | 296 |
| `script_register_corpse_search` | `RegisterCorpseSearch` | `0x8010245c` | 220 |
| `script_register_target_check` | `RegisterTargetCheck` | `0x80102538` | 748 |
| `script_register_position_check` | `RegisterPositionCheck` | `0x80102824` | 192 |
| `script_register_for_property_message` | `RegisterForPropertyMessage`, `StopPropertyMessage` | `0x801028e4` | 280 |
| `script_stop_target_search` | `StopTargetSearch`, `StopTargetCheck`, `StopDistanceToTarget` | `0x801029fc` | 588 |
| `script_stop_check_objects_inside_range` | `StopCheckObjectsInsideRange`, `StopCheckObjectsOutsideRange`, `StopCheckObjectsInsideAngularRange` and 2 more | `0x80102c48` | 860 |
| `script_set_weapon_enable` | `SetWeaponEnable` | `0x80102fa4` | 168 |
| `script_break_point` | `BreakPoint` | `0x8010304c` | 100 |
| `script_debug_print` | `DebugTTYPrint`, `AILog`, `ScriptError` | `0x801030b0` | 144 |
| `script_get_target_for` | `GetTargetFor` | `0x80103140` | 184 |
| `script_get_target_range_for` | `GetTargetRangeFor` | `0x801031f8` | 572 |
| `script_set_obj_info` | `SetObjInfo`, `Soldier_SetObjInfo` | `0x8010567c` | 344 |
| `script_get_distance_to_object_from_trigger` | `GetDistanceToObjectFromTrigger` | `0x801057d4` | 252 |
| `script_make_noise` | `MakeNoise` | `0x801059b0` | 408 |
| `script_get_voice_handle` | `GetVoiceHandle` | `0x80105b48` | 128 |
| `script_stop_current_voice` | `StopCurrentVoice` | `0x80105bc8` | 164 |
| `script_register_coverpoint` | `RegisterCoverpoint` | `0x80105c6c` | 100 |
| `script_ai_force_to_closest_cover_point` | `AIForceToClosestCoverPoint`, `AIForceToClosestMMG`, `AIForceToMMGFeeder` | `0x80105cd0` | 696 |
| `script_display_letterbox` | `DisplayLetterbox` | `0x80105f88` | 140 |
| `script_letterbox_transition` | `LetterboxTransition` | `0x80106014` | 152 |
| `script_load_one_shot_sound_bank` | `LoadOneShotSoundBank` | `0x801060ac` | 172 |
| `script_set_sound_zone` | `SetSoundZone` | `0x80106158` | 84 |
| `script_debug_init_from_trigger` | `DebugInitFromTrigger` | `0x801061ac` | 48 |
| `script_display_prompt_text` | `DisplayPromptText`, `DisplayPromptInteger` | `0x80106cfc` | 676 |
| `script_tutorial_text` | `DisplayBIGPromptText`, `DisplayTutorialText`, `KillTutorialText` and 2 more | `0x80106fa0` | 240 |
| `script_play_sound_from_input_trigger` | `PlaySoundFromInputTrigger`, `PlaySoundAtOffset`, `PlaySound` | `0x80107098` | 604 |
| `script_play_sound_from_input_trigger2` | `PlaySoundFromInputTrigger2`, `PlaySoundAtOffset2` | `0x80107500` | 412 |
| `script_remove_vocal` | `RemoveVocal`, `Soldier_RemoveVocal` | `0x80107aa8` | 416 |
| `script_activate_sound_buss` | `ActivateSoundBuss` | `0x80107c48` | 92 |
| `script_end_sound` | `EndSound` | `0x80107ca4` | 148 |
| `script_music_values_rotation` (sound memory) | `GetGenericSoundStructureMemory` | `0x801086b4` | 60 |
| `script_music_values_rotation` (rotation) | `ResetParticleSystemRotation` | `0x80108780` | 280 |
| `script_create_particle_system_by_type` | `CreateParticleSystemByType`, `CreateParticleSystemByName`, `CreateParticleSystemByNameAtAttachPoint`, `CreateParticleSystemByNameAtWeaponsAttachPoint` | `0x80108898` | 1396 |
| `script_set_particle_system_velocity` | `SetParticleSystemVelocity` | `0x80108e0c` | 252 |
| `script_destroy_particle_system` | `DestroyParticleSystem` | `0x80108f08` | 152 |
| `script_clear_decals` | `ClearDecals` | `0x80108fa0` | 88 |
| `script_projector_create_by_type` | `ProjectorCreateByType`, `ProjectorCreateByName` | `0x80108ff8` | 1156 |
| `script_projector_destroy` | `ProjectorDestroy` | `0x8010947c` | 48 |
| `script_glare_create_by_type` | `GlareCreateByType`, `GlareCreateByName` | `0x801094ac` | 1228 |
| `script_glare_destroy` | `GlareDestroy` | `0x80109978` | 48 |
| `script_create_light_by_type` | `CreateLightByType` | `0x801099a8` | 340 |
| `script_destroy_light` | `DestroyLight` | `0x80109afc` | 156 |
| `script_set_max_light_priority` | `SetMaxLightPriority` | `0x80109c90` | 156 |
| `script_activate_animated_light_by_id` | `ActivateAnimatedLightByID` | `0x80109d2c` | 220 |
| `script_deactivate_animated_light_by_id` | `DeactivateAnimatedLightByID` | `0x80109e08` | 124 |
| `script_event_memory_block` | `GetEventSystemMemBlock` | `0x80109e84` | 96 |
| `script_add_objective` | `AddObjective`, `ChangeObjectivePrompt` | `0x80109ee4` | 340 |
| `script_static_object_play_animation` | `StaticObjectPlayAnimation` | `0x8010a038` | 224 |
| `script_hier_object_play_animation` | `HierObjectPlayAnimation`, `StartMotionPlayback` | `0x8010a118` | 608 |
| `script_stop_motion_playback` | `StopMotionPlayback` | `0x8010a378` | 276 |
| `script_resume_motion_playback` | `ResumeMotionPlayback` | `0x8010a48c` | 192 |
| `script_modify_motion_data` | `ModifyMotionData` | `0x8010a54c` | 352 |
| `script_translate_rel` | `TranslateRel`, `RotateRel` | `0x8010a6ac` | 888 |
| `script_get_rotate_rel` | `GetRotateRel`, `GetTranslateRel` | `0x8010aa24` | 152 |
| `script_set_objective` | `SetObjective` | `0x8010ab40` | 156 |
| `script_get_soldier_weapon_in_slot` | `GetSoldierWeaponInSlot` | `0x8010b908` | 112 |
| `script_get_current_weapon_class` | `GetCurrentWeaponClass` | `0x8010b978` | 148 |
| `script_get_current_npc_weapon_class` | `GetCurrentNPCWeaponClass` | `0x8010ba0c` | 300 |
| `script_get_weapon_info` | `GetWeaponInfo` | `0x8010bb38` | 228 |
| `script_catch_grenade` | `CatchGrenade` | `0x8010bce4` | 232 |
| `script_set_grenade_timer` | `SetGrenadeTimer` | `0x8010be28` | 124 |
| `script_player_weapon_info` | `PlayerWeaponInfo` | `0x8010bea4` | 444 |
| `script_player_super_aim_mode` | `PlayerSuperAimMode` | `0x8010c200` | 148 |
| `script_is_player_invincible` | `IsPlayerInvincible` | `0x8010c294` | 108 |
| `script_player_eject_shell` | `PlayerEjectShell` | `0x8010c300` | 280 |
| `script_disable_player_shoot` | `DisablePlayerShoot` | `0x8010c418` | 156 |
| `script_use_binoculars` | `UseBinoculars` | `0x8010c4b4` | 360 |
| `script_set_weapon_draw` | `SetWeaponDraw` | `0x8010c61c` | 196 |
| `script_set_prop_draw` | `SetPropDraw`, `SetDrawEnabled` | `0x8010c6e0` | 392 |
| `script_is_draw_enabled` | `IsDrawEnabled` | `0x8010c868` | 124 |
| `script_player_activate_lookat_trigger` | `PlayerActivateLookatTrigger`, `PlayerDeactivateLookatTrigger`, `PlayerStartLookAt` | `0x8010c8e4` | 696 |
| `script_player_stop_look_at` | `PlayerStopLookAt` | `0x8010cb9c` | 108 |
| `script_create_mp_dump_object` | `CreateMPDumpObject`, `CreateDumpObject` | `0x8010cc08` | 796 |
| `script_attach_to_prop` | `AttachToProp`, `AttachAndAlignToProp` | `0x8010f188` | 764 |
| `script_detach_from_waypoint` | `DetachFromWaypoint` | `0x8010f768` | 204 |
| `script_attach_player_to_prop` | `AttachPlayerToProp`, `DetachFromProp` | `0x8010f834` | 448 |
| `script_detach_all_from_prop` | `DetachAllFromProp` | `0x8010f9f4` | 168 |
| `script_detach_player_from_prop` | `DetachPlayerFromProp` | `0x8010fa9c` | 244 |
| `script_detach_from_soldier` | `DetachFromSoldier` | `0x8010ffdc` | 136 |
| `script_soldier_detach_from_soldier` | `Soldier_DetachFromSoldier` | `0x80110064` | 180 |
| `script_attach_to_anim_object` | `AttachToAnimObject` | `0x80110118` | 236 |
| `script_attach_to_hier_object` | `AttachToHierObject` | `0x80110204` | 636 |
| `script_send_event_to_all_attached` | `SendEventToAllAttached` | `0x80110480` | 324 |
| `script_set_jaw_motion` | `SetJawMotion` | `0x801111d4` | 156 |
| `script_set_muzzle_fire_for_player` | `SetMuzzleFireForPlayer` | `0x801113c8` | 200 |
| `script_create_explosion` | `CreateExplosion` | `0x80111550` | 400 |
| `script_update_player_stats` | `UpdatePlayerStats` | `0x80112290` | 616 |
| `script_change_players_team` | `ChangePlayersTeam` | `0x801125cc` | 200 |
| `script_change_npc_team` | `ChangeNPCTeam` | `0x80112694` | 236 |
| `script_ai_set_team` | `AISetTeam` | `0x80112780` | 184 |
| `script_unhinge_camera_pitch` | `UnhingeCameraPitch` | `0x80112838` | 156 |
| `script_toggle_player_vs_trigger_checks` | `TogglePlayerVsTriggerChecks` | `0x801128d4` | 96 |
| `script_add_hint` | `AddHint` | `0x80112934` | 48 |
| `script_end_level` | `EndLevel` | `0x80112964` | 104 |
| `script_scale_rel` | `ScaleRel` | `0x801129cc` | 212 |
| `script_attach_player_to_mounted_machine_gun` | `AttachPlayerToMountedMachineGun` | `0x80112c78` | 328 |
| `script_attach_triggering_player_mmg` | `AttachTriggeringPlayerToMMG` | `0x80112dc0` | 48 |
| `script_register_mounted_machine_gun` | `RegisterMountedMachineGun` | `0x80112df0` | 156 |
| `script_deregister_mounted_machine_gun` | `DeregisterMountedMachineGun` | `0x80112e8c` | 124 |
| `script_get_mounted_machine_gun_by_link_code` | `GetMountedMachineGunByLinkCode` | `0x80112f08` | 164 |
| `script_arrived_at_mmg` | `ArrivedAtMMG`, `CantReachMMG`, `HitFromBehindMMG` | `0x80112fac` | 552 |
| `script_register_mmg_feeder` | `RegisterMMGFeeder` | `0x801131d4` | 132 |
| `script_get_mmg_feeder_by_link_code` | `GetMMGFeederByLinkCode`, `RegisterBuddyFollowObject` | `0x80113258` | 284 |
| `script_buddy_cant_reach_target` | `BuddyCantReachTarget`, `BuddyFinishedMovingAway`, `SetDistances` | `0x80113374` | 632 |
| `script_register_gag_action` | `RegisterGagAction`, `StartGag`, `StartGagCRC` | `0x801135ec` | 420 |
| `script_get_bs_object` | `GetBSObject` | `0x80113790` | 164 |
| `script_set_mg_waypoint_flag` | `SetMGWaypointFlag` | `0x80113834` | 144 |
| `script_player_force_to_weapon` | `PlayerForceToWeapon` | `0x801138c4` | 156 |
| `script_register_save_point` | `RegisterSavePoint`, `PerformSavePointResponse` | `0x80113e8c` | 280 |
| `script_serialize_value` | `SerializeValue` | `0x80113fa4` | 156 |
| `script_serializing_in` | `SerializingIn`, `SerializingOut` | `0x80114040` | 208 |
| `script_serializable` | `Serializable`, `SerializeData`, `SaveGagState` | `0x80114110` | 632 |
| `script_moving_mech_force_update_trigger` | `MovingMechForceUpdateTrigger`, `MechSetFlag` | `0x8011461c` | 328 |
| `script_block_new_vibrations` | `BlockNewVibrations` | `0x80114978` | 272 |
| `script_fade_out` | `FadeOut`, `FadeIn` | `0x80114a88` | 384 |
| `script_attach_hier_base_to_mmg` | `AttachHierBaseToMMG` | `0x80114e60` | 220 |
| `script_set_detonation_time` | `SetDetonationTime` | `0x80114f3c` | 144 |
| `script_set_detonation_time_for_player_turret` | `SetDetonationTimeForPlayerTurret` | `0x80114fcc` | 160 |
| `script_single_anim_ends_event` | `SetSingleAnimEndsEventDelivery` | `0x8011506c` | 48 |
| `script_set_motion_blur` | `SetMotionBlur` | `0x80115498` | 208 |
| `script_set_motion_blur_duration` | `SetMotionBlurDuration` | `0x80115568` | 252 |
| `script_stop_corpse_search` | `StopCorpseSearch` | `0x80115884` | 196 |
| `script_hier_object_set_look_object` | `HierObjectSetLookObject` | `0x80115948` | 172 |
| `script_hier_object_map_weapon` | `HierObjectMapWeapon` | `0x801159f4` | 188 |
| `script_hier_object_fire` | `HierObjectFire` | `0x80115ca8` | 184 |
| `script_hier_object_register_bullet_damage` | `HierObjectRegisterBulletDamage` | `0x80115d60` | 232 |
| `script_ai_find_tank` | `AIFindTank` | `0x80115e48` | 108 |
| `script_get_constrained_rotate_angle` | `GetConstrainedRotateAngle` | `0x801160b8` | 176 |
| `script_bullet_type_class` | `GetClassFromBulletType` | `0x80116168` | 240 |
| `script_activate_aiming` | `ActivateAiming` | `0x80116370` | 172 |
| `script_suspend_aiming` | `SuspendAiming`, `ResumeAiming` | `0x8011641c` | 228 |
| `script_soldier_activate_aiming` | `Soldier_ActivateAiming` | `0x80116500` | 208 |
| `script_soldier_suspend_aiming` | `Soldier_SuspendAiming`, `Soldier_ResumeAiming`, `ActivatePropAiming` and 5 more | `0x801165d0` | 1280 |
| `script_set_head_track_player` | `SetHeadTrackPlayer` | `0x80116ad0` | 152 |
| `script_reset_mmg` | `ResetMMG` | `0x80116c28` | 216 |
| `script_get_flex_property` | `GetFlexProperty` | `0x80116d00` | 388 |
| `script_load_compartment` | `LoadCompartment` | `0x80116e84` | 256 |
| `script_is_in_any_other_compartment` | `IsInAnyOtherCompartment` | `0x80116f84` | 156 |
| `script_ai_set_high_level_state` | `AISetHighLevelState`, `SoldierSetScriptHighLevelState`, `AIChooseBehavior` and 9 more | `0x80117020` | 1756 |
| `script_ai_finished_behavior` | `AIFinishedBehavior` | `0x801176fc` | 48 |
| `script_ai_is_behavior_enabled` | `AIIsBehaviorEnabled` | `0x8011772c` | 156 |
| `script_enter_buddy_teleport_area` | `EnterBuddyTeleportArea`, `ExitBuddyTeleportArea`, `IsBuddyInArea` and 3 more | `0x80117ca0` | 1448 |
| `script_get_scripted_behavior_type` | `GetScriptedBehaviorType`, `SetScriptedBehaviorType` | `0x80118248` | 400 |
| `script_create_mp_health` | `CreateMPHealth`, `CreateMPRankedAmmoWeapon` | `0x801183d8` | 576 |
| `script_mp_get_player_body` | `MPGetPlayerBody` | `0x80118618` | 152 |
| `script_mp_get_player_velocity` | `MPGetPlayerVelocity` | `0x801186b0` | 176 |
| `script_set_mmg_location` | `SetMMGLocation` | `0x80118760` | 244 |
| `script_ai_get_previous_behavior_type` | `AIGetPreviousBehaviorType` | `0x80118854` | 192 |
| `script_ai_get_previous_behavior_script` | `AIGetPreviousBehaviorScript`, `AISetPreviousBehaviorScript` | `0x80118914` | 384 |
| `script_ai_get_next_behavior_type` | `AIGetNextBehaviorType`, `AIGetNextBehaviorScript` | `0x80118a94` | 384 |
| `script_soldier_set_current_weapon` | `Soldier_SetCurrentWeapon` | `0x80118c14` | 204 |
| `script_soldier_get_current_weapon` | `Soldier_GetCurrentWeapon` | `0x80118ce0` | 228 |
| `script_soldier_get_weapon_type` | `Soldier_GetWeaponType` | `0x80118ed4` | 216 |
| `script_soldier_get_locomotion_info` | `Soldier_GetLocomotionInfo`, `Soldier_EnableSkeletalPartition` | `0x80118fac` | 360 |
| `script_soldier_disable_skeletal_partition` | `Soldier_DisableSkeletalPartition` | `0x80119114` | 180 |
| `script_soldier_create_explosion` | `Soldier_CreateExplosion` | `0x80119424` | 440 |
| `script_anim_object_set_anim_callback` | `AnimObject_SetAnimCallback` | `0x801195dc` | 208 |
| `script_signal_buddy_lead_checkpoint_reached` | `SignalBuddyLeadCheckpointReached` | `0x801196ac` | 260 |
| `script_signal_buddy_lead_checkpoint_reached_by_player` | `SignalBuddyLeadCheckpointReachedByPlayer` | `0x801197b0` | 124 |
| `script_get_buddy_lead_checkpoint_from_id` | `GetBuddyLeadCheckpointFromID` | `0x8011982c` | 184 |
| `script_get_distance_to_player` | `GetDistanceToPlayer` | `0x801198e4` | 196 |
| `script_validation_build` | `ValidationBuild` | `0x801199a8` | 304 |
| `script_start_slow_moh` | `StartSlowMOH` | `0x80119ad8` | 248 |
| `script_stop_slow_moh` | `StopSlowMOH` | `0x80119bd0` | 88 |
| `script_initialize_animated_texture` | `InitializeAnimatedTexture`, `SetAnimatedTextureState` | `0x80119d6c` | 536 |
| `script_set_animated_texture_frame` | `SetAnimatedTextureFrame`, `SetAnimatedTextureFrameRate` | `0x80119f84` | 280 |
| `script_reset_animated_texture` | `ResetAnimatedTexture`, `AdvanceAnimatedTexture` | `0x8011a09c` | 264 |
| `script_add_replay_item` | `AddReplayItem` | `0x8011a1a4` | 128 |
| `script_has_replay_item` | `HasReplayItem` | `0x8011a224` | 276 |
| `script_add_film_canister` | `AddFilmCanister` | `0x8011a338` | 232 |
| `script_has_any_film_canister` | `HasAnyFilmCanister` | `0x8011a420` | 164 |
| `script_has_film_canister` | `HasFilmCanister` | `0x8011a4c4` | 212 |
| `script_give_cheat_number` | `GiveCheatNumber` | `0x8011a598` | 132 |
| `script_grenade_response_finished` | `GrenadeResponseFinished` | `0x8011a61c` | 196 |
| `script_finished_investigation` | `FinishedInvestigation` | `0x8011a6e0` | 140 |
| `script_sword_drawn` | `SwordDrawn` | `0x8011a76c` | 188 |
| `script_has_pistol` | `HasPistol` | `0x8011a828` | 248 |
| `script_ai_add_corpse_threat` | `AIAddCorpseThreat` | `0x8011a920` | 180 |
| `script_get_weapon_info_helper` | `GetWeaponInfoHelper` | `0x8011b95c` | 48 |
| `script_player_collision_id` | `GetCollisionIDFromScriptCollisionType` | `0x8011bbf8` | 60 |
| `script_add_triggered_hint` | `AddTriggeredHint` | `0x8011bc34` | 168 |
| `script_remove_triggered_hint` | `RemoveTriggeredHint` | `0x8011bcdc` | 244 |
| `script_coop_attach` | `AttachPlayerToPropCoop2`, `AttachPlayerToPropCoop`, `AttachPlayerIDToPropCoop` and 5 more | `0x8011c678` | 408 |
| `script_get_distance_between_players` | `GetDistanceBetweenPlayers` | `0x8011c810` | 76 |
| `script_coop_players` | `SetParanoiaParameters`, `MPAddBotHealth`, `ForcePlayersApart` and 11 more | `0x8011c85c` | 712 |
| `script_update_fire_position` | `UpdateFirePosition` | `0x8011cb24` | 76 |
| `script_coop_triggers` | `SetSplitScreen`, `AssignCptDistances`, `ForcePlayerWarp` and 31 more | `0x8011cb70` | 1856 |
| `script_bot_done_reloading` | `BotDoneReloading` | `0x8011d2b0` | 144 |

**Further code-generation idioms.** A float result is computed into a plain
local, the stack is popped, and the value is then copied through
`volatile float result = x; **stack = *(int *)&result;`, which reproduces the
`stfs`/`lwz` pair after the pop; a constant float result declares the volatile
before the pop, and a result that follows by-reference outputs reads it back with
`*(volatile int *)&result`. Schedule registrations read every argument into a
local in argument order before the call, square distances in place and type the
event as `unsigned short`. Floating tests compiled as `cror so,eq,gt` come from
`!(x >= c)`. A `CMatrix` local repeats the original inline class-initialization
check (`if (!CMatrix::s_ClassInit) CMatrix::InitClass();`); a `CMatrix *` local
keeps the matrix address in a saved register where the original does. Switch
decision trees depend on the case set: separate case bodies with the same value
give separate equality tests, and an explicit empty case below the smallest real
case makes GCC test that case first. The degree-to-radian scale in `RotateRel` is
`3.141593f / 180.0f`, one unit in the last place above pi/180. `CAIObject`'s table
pointer is at `+0x72c` (`CAIObjectInterface`): slot 2 is `AsAISoldierObject`,
slot 9 `OnDeath`, slot 10 `SetupTargetMatchList` and slot 11 `ChooseTarget`.

**Observed rules (continued).** Player turret detonation time is the script value
times 30. `EndLevel` sets the level-won bit (mask `0x200`) in the profile word at
`+0x31664` and sets `g_GameFlow+8`, which `CPauseMenu::RestartLevel` also sets.
`BreakPoint` prints `"break point!\n"`. The alarm built-ins belong to `alarm.cpp`,
not `bsbifunc.cpp`. `AISetStatusVar` treats status variable 11 reaching 15 as a
death (Frontline uses 13). `MechFollowPath` scales its speed by 1/59.94 and its
angles by degrees-per-frame in radians.

**Remaining blockers.** Some built-ins still need storage or scope work before
acceptance:
- `AttachMMGProp` and `DetachMMGProp` read the hand-name strings now owned by
  `script_soldier_attach`. They still need a verified shared-storage scope rather
  than duplicate ownership of those literals.
- `GiveEnemyGrenadeWeapon`, its `Soldier_` variant and the bearing built-ins use
  function-local statics with dynamic initialization in `.bss`.

`MechSetPathFlag` and `DoDamageIfLocationMatches` now have matching source and
complete generated-data verification through the literal placement described
[below](#sound-playback-path-flags-and-location-damage).

Others are close but not yet exact: `IsAliasSoundLooping`, `Soldier_GetObjInfo`,
`AddPlayerAmmo`, `SetExpression`, `TeleportToPlayerByNavPoint`,
`CanMeleeTarget`, `SetBlinkState`, `SetDefaultFogParams`,
`GetSpeedRelativeToPlayer`, `MoveFromObjToObj`, `MechFollowPath` and
`PlayVibrationEvent`. These remain original context.

## Follow-up wrappers from PR #31

kyleckroeger's [PR #31](https://github.com/lifewillbeokay/moh-rising-sun/pull/31)
provided the argument-access and float-return idioms above, along with useful
near-match and constant-ownership leads. Applying those notes to independently
checked GR8E69 instructions adds six complete functions, **1,300 executable
bytes**, with the same ProDG 3.8.1 profile:

| Function (`BIFunc_` prefix omitted) | Address | New code bytes | Observed behavior |
| --- | --- | ---: | --- |
| `GetObjInfo` | `0x801051b4` | 256 | Calls the original query helper with three script arguments and the native receiver, then returns either integer or float bits according to the helper's output flag. |
| `MakeNoiseFromInputTrigger` | `0x801058d0` | 224 | Reads the trigger position and calls `MakeNoise` with type 49 for a nonzero flag, otherwise 48, and the supplied float radius. |
| `PathfinderFadeVolume` | `0x80108584` | 180 | Converts the integer volume to float, scales by `0.01f`, and calls `MUSIC_Fade` with the second integer argument unchanged. It does not clamp the volume. |
| `SetLightVolumeTransitionDuration` | `0x80109b98` | 248 | Resolves an explicit script object's scene node or uses the current receiver, then sets the duration if `AsMovingNode` succeeds. |
| `SetGrenadeSlot` | `0x8010bc1c` | 200 | Searches weapon slots 0 through 3 for the first weapon with class 7, storing its slot or -1 in the receiver's script data at `+0x84`. |
| `EjectHelmet` | `0x80116b68` | 192 | Initializes the vector's fourth word to one and x/y/z to zero, replaces z with the script argument, and forwards the vector to the native soldier. |

`GetObjInfo`'s integer local precedes its boolean local, reproducing the original
initialization-store order. A plain, function-scope float temporary and a
`volatile int` read reproduce the by-reference result's store/load sequence.
This accepts only the 256-byte wrapper; the 6,228-byte query helper at
`0x80103960` and `Soldier_GetObjInfo` remain original context. The trigger-noise
wrapper reuses the flag local for the selected noise type, preserving its lifetime
across vector construction. `EjectHelmet` uses a descriptive inline `ClearVector`
helper and a local vector pointer; these are reconstruction choices, not recovered
historical names or evidence for another `CVector3` constructor.

The grenade-slot prefix is local to that wrapper. Only the signed word at `+0x84`
is established; earlier bytes remain opaque, and the view must not be used to
allocate script objects. Its weapon search uses the already reconstructed
`GetSoldierWeaponInSlot`. None of these wrappers adds validation or null checks
absent from the original.

**Complete constant ownership.** `music_values_rotation.cpp` compiles the
contiguous `0x80108584`–`0x80108897` range: the new fade wrapper followed by the
already accepted `PathfinderSetLatency`, `GetGenericSoundStructureMemory`,
`FloatToInt`, `InitToInvalid` and `ResetParticleSystemRotation`. It replaces four
smaller manifests. The six bodies total 788 bytes, but only the fade wrapper's
180 bytes are new progress; the other 608 bytes retain their existing credit.
This is a selected fragment of `bsbifunc.cpp`, not a recovered file boundary.

Together they generate the complete 16-byte read-only pool at `0x802a5b58`:
the integer-conversion double, fade scale `0.01f`, and rotation vector's `1.0f`.
Compiling fade alone would instead pad the pool over the neighboring vector
constant. Combining the actual neighboring bodies verifies all emitted data
without clipping padding, importing uncompiled bytes or patching instructions.
The other new wrappers generate 16 read-only bytes in total: four each for the
object-query and trigger-position wrappers and eight for helmet ejection.
Of the 32 generated data bytes checked by the six new manifests, four were
already accepted with the rotation wrapper; none earns code-progress credit.

## Sound playback, path flags and location damage

A second follow-up to kyleckroeger's [PR #31](https://github.com/lifewillbeokay/moh-rising-sun/pull/31)
uses its argument-access pattern, float-return sequence, switch-case ordering and
constant-ownership notes to reconstruct five more functions. They were independently
checked against GR8E69 with Codex assistance and the same ProDG 3.8.1 profile.
Three new manifests add **1,340 executable bytes** and **56 generated read-only
bytes**, with no changes to shared headers or prior accepted fragments.

| Manifest | Function (`BIFunc_` prefix omitted) | Address | Code bytes |
| --- | --- | --- | ---: |
| `script_alias_playback` | `PlayAliasSound` | `0x80107788` | 244 |
| `script_alias_playback` | `PlayAliasSoundOnOther` | `0x8010787c` | 312 |
| `script_alias_playback` | `PlayVocal` | `0x801079b4` | 244 |
| `script_mech_path_flag` | `MechSetPathFlag` | `0x80114518` | 260 |
| `script_location_damage` | `DoDamageIfLocationMatches` | `0x80116258` | 280 |

**Sound playback.** `PlayAliasSound` and `PlayVocal` compare their unsigned sound
ID with `GetStringCRC("none")`, returning -1 for that sentinel. Otherwise they
call the original `PlaySoundFromTriggerOrObject2` with the current script object,
its native trigger, the supplied native receiver and flags, and a zero x/y/z
offset whose fourth word is one. The last argument selects ordinary sound or
vocal playback. A zero sound ID is not separately rejected.

`PlayAliasSoundOnOther` resolves its first argument's native trigger and, when
its weak game-object pointer is present, its scene node. It returns -1 if both
are null; otherwise it uses them as the playback receiver while retaining
`g_pBSObject` as the script context. This variant does not check the `"none"`
sentinel. All three wrappers return the helper's handle after the table-directed
stack adjustment. The playback helper remains original context; `GetStringCRC`
already has accepted source.

The complete sound pool at `0x802a5b28` is 32 bytes: the five-byte `"none"` string,
three alignment bytes, then three zero/one float pairs. `IsAliasSoundLooping` also
references that string but remains unmatched. The accepted playback source emits
the shared literal itself at the existing address; accepting its caller does not
require crediting the earlier query function. A future reconstruction of that
query must share this pool rather than claim overlapping data ownership.

**Mech path flags.** The receiver is converted with `AsStaticObject` before
reading the arguments. Selector 1 writes bit 23 of the word at `+0x218`, normalized
from the supplied integer. Selector 0 and every other selector store the integer
converted to float and multiplied by `0.01f` at `+0x110`. The explicit case 0
preserves the original switch decision tree. There is no clamp or new null check.
The existing scoped `CStaticObject` view is reused without expanding its layout.

**Location damage.** The first argument is the existing `ScriptHitInfo` prefix.
If its location at `+0x10` differs from the requested location, the wrapper returns
the input float unchanged. On a match, an available hierarchy receiver supplies
a damage multiplier for that location and the mapped bullet class. The wrapper
multiplies the hit record's damage at `+4` in place, then subtracts that damage
from the input float. Without a hierarchy receiver, it subtracts the original
damage directly. The result becomes zero unless it is strictly greater than the
double constant `0.001`; the original unordered comparison also sends NaN to
zero on this path. The result is returned as float bits at the adjusted stack
top; PR #31's temporary-value idiom reproduces the observed store/load schedule.
The class-mapping helper already matches; hierarchy-specific damage multiplication
remains original context.

**Literal placement.** Each of the last two functions uses an eight-byte constant
followed by a four-byte float. The path wrapper's conversion double is at
`0x802a5ec0`, with `0.01f` at `0x802a5ec8`. The damage wrapper's double threshold
is at `0x802a5f18`, with zero at `0x802a5f20`. In separate translation units, leaving
the float in the compiler's double-aligned pool generates unwanted trailing
padding over the next function's literal. The source therefore places that float
in a four-byte `.rodata.*` subsection, using the same descriptive storage
annotation as the accepted arithmetic and ambience fragments. The labels are not
recovered historical globals. Both complete generated sections are compared;
no emitted bytes are discarded, no instructions are patched, and the neighboring
vibration and helmet-ejection constants retain their existing ownership. All 56
data bytes are excluded from executable progress.

## Soldier attachments, enemy counts and texture CRC

A further batch applies kyleckroeger's [PR #31](https://github.com/lifewillbeokay/moh-rising-sun/pull/31)
argument-loading and shared-literal leads to four built-ins, and follows the texture
hash wrapper into its three `CCRC` dependencies. Codex independently reconstructed
all seven functions from GR8E69. Four manifests add **1,484 executable bytes**
(1,212 in `src/script/`, 272 in `src/crc.cpp`) and **220 read-only bytes** using
ProDG 3.8.1 with the usual script flags. No compiler output is stripped or patched.

| Manifest | Function | Address | Code bytes |
| --- | --- | --- | ---: |
| `script_soldier_attach` | `BIFunc_AttachToSoldier` | `0x8010fb90` | 300 |
| `script_soldier_attach` | `BIFunc_AttachToSoldierEmptyHand` | `0x8010fcbc` | 376 |
| `script_enemy_count` | `BIFunc_ModifyTotalKillableEnemyCount` | `0x801124f8` | 212 |
| `script_animated_crc` | `BIFunc_GetAnimatedTextureCRC` | `0x80119c28` | 324 |
| `crc` | `CCRC::Reset` | `0x802868f8` | 12 |
| `crc` | `CCRC::operator+=(unsigned char)` | `0x80286904` | 176 |
| `crc` | `CCRC::u4Reflect(unsigned int, unsigned int)` | `0x802869b4` | 84 |

**Attachment behavior.** `AttachToSoldier` resolves the supplied script object's
scene node, attaches it at `lt_hand` for selector 0 or `rt_hand` for selector 1,
and passes the third argument unchanged as flags. It removes the node from the
scene regardless of the attachment result, including when neither selector
matches. This is the observed behavior, not a success-only removal policy.

`AttachToSoldierEmptyHand` first checks that the resolved node is in the scene.
Otherwise, it prints the existing duplicate/removed-prop diagnostic with the
trigger's `ID` when the script-object pointer is nonzero. For a present node it
tries the right hand, then the left hand only if the first attempt fails. It
returns 1 for right-hand success, 0 for left-hand success, or -1 for failure,
removing the node only after success. The shared exit preserves the original
branch order and stack adjustment. Both functions dereference the input before
any diagnostic-path pointer check; they retain the original valid-object
precondition. The existing scene and animated-object interfaces are unchanged.

**Enemy-count behavior.** `ModifyTotalKillableEnemyCount` adds the signed script
argument to `g_iNumberOfEnemiesSpawned` only when the current trigger's native
binding type is 3 and its `Team` integer is 2. That path also prints the original
message containing “Ignoring for total enemy count”; despite that wording, the
instructions do update the counter. The manifest keeps the original global and
trigger methods external. `TriggerObject.h` adds only the symbol-supported
`GetInt(const char *) const` declaration, with no storage or virtual-table change.

**Shared literal ownership.** The attachment fragment emits the complete diagnostic
pool at `0x802a5d20` (100 bytes), `lt_hand` at `0x802a5d18` (8 bytes), `rt_hand` at
`0x802a55fc` (8 bytes) and `ID` at `0x802a59ec` (4 bytes). The counter fragment
emits its diagnostic at `0x802a5e28` (68 bytes) and `Team` at `0x802a59e4` (8 bytes).
Each named literal is placed in a read-only subsection using the project's
existing literal-placement mechanism; names describe storage and are not
recovered globals. Character-list initializers keep `ID` and `Team` in those
sections instead of being folded into the compiler's ordinary string pool.
All terminators and emitted alignment bytes are compared. Other original callers
continue to use the same shared bytes and earn no new credit; future source must
not claim these ranges again.

**Texture hashing and CCRC.** The built-in copies at most 1,024 bytes of its string
argument into a stack buffer with `strncpy`, lowercases it with `strlwr`, allocates
28 bytes with `DWI_alloc` and initializes a reflected 32-bit CRC with polynomial
`0x04c11db7`, an all-ones mask, initial value and final XOR. After `Reset`, it feeds
bytes until the first zero, reflects the accumulated value and applies the final
XOR, deletes the CRC object, and returns the bits in the script stack. The original
24-byte allocation-label pool at `0x802a5564` is generated and checked. The wrapper
adds neither an allocation-failure branch nor an explicit buffer terminator after
`strncpy`; inputs must satisfy the original string/buffer preconditions.

`include/game/CRC.h` describes the seven four-byte fields established by that
28-byte allocation, initialization and the three methods. Field names, the
original constructor spelling and the unused return types of `Reset`/`operator+=`
are not recovered. The reconstruction initializes the fields explicitly, preserving
the emitted store order, rather than inventing a historical constructor signature.
The three globally named method ranges are verified directly; their original
translation-unit boundary is unproven and the manifest does not assign one.

`Reset` copies the initial value into the running value. The byte-update operator
optionally reflects eight input bits, XORs them into the high part of the CRC,
then performs eight shift/polynomial steps, masking after each step. `u4Reflect`
reverses the requested low bits while leaving higher input bits intact; a zero
count returns the input. The wrapper uses width 32 and reflection counts 8 and 32.
No new checks or portable behavior for invalid shift counts are claimed. This
bitwise class is separate from the previously accepted, table-driven
`GetStringCRC`; the texture wrapper also lowercases its input, which that helper
does not do.

The complete-image build verifies these fragments together with every prior unit.
A separate host build of `src/crc.cpp` agrees with Python's `zlib.crc32` for the
empty string, `123456789` and `texture.dds`; reflection checks also preserve upper
bits and the zero-count input. This tests the recovered CRC interpretation, not
game execution. All generated data is excluded from code progress. Runtime and
emulator behavior remain untested.

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
or sections are discarded or clipped. The manifests list the complete generated
code and read-only constant/string sections. Generated data earns no executable credit,
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

## Startup allocation and system state

Four functions independently reconstructed by Codex from GR8E69 add **240 code
bytes** and **68 read-only bytes**. They form part of the now-reconstructed
[trigger initialization sequence](TriggerObject.md#complete-trigger-initialization).
All four use ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates`;
this working profile does not establish the original compiler release.

| Function | Address | Code bytes |
| --- | --- | ---: |
| `BSInitBSObjectStartupAllocator(int)` | `0x800f6f28` | 120 |
| `BSUtilSetScriptingSystemState(EScriptSystemState)` | `0x800f8cb4` | 12 |
| `BSUtilGetScriptingSystemState()` | `0x800f8cc0` | 12 |
| `BSUtilInitStartupAllocator(int)` | `0x800f8ccc` | 96 |

Both initialization functions delete the previous MemStack if present, allocate
and construct a replacement, and store it in the original private global. The
object variant also stores `unsigned(size) / 48`, preserving the unsigned
arithmetic even though the parameter is signed. Neither adds allocation-failure
handling or a negative-size guard. The separately reconstructed
[MemStack lifecycle](Memory.md#memstack-lifecycle) supplies construction and
destruction; the underlying heap and the MemStack allocation method stay external.

`MemStack::operator new(size, label)` is a descriptive inline allocation boundary
that calls `DWI_alloc` with flag value 1024. Its target size is twelve bytes.
The complete diagnostic pools are checked at `0x802a518c` (36 bytes,
`source/ai_script/bsobject.cpp:578`) and `0x802a54bc` (32 bytes,
`source/ai_script/bsutil.cpp:906`), including terminators and alignment. No
neighboring `bad_alloc` strings or original storage receive credit.

The state setter stores its argument unchanged and the getter returns it;
there is no transition validation. `ScriptStartup.h` retains the enum tag from
the symbol and names the observed startup/running values 1 and 2. It does not
claim the complete original enum or impose those two values on the accessors.

Private original storage remains scoped in the manifests:

| Global | Address | Original file record |
| --- | --- | --- |
| `g_pBSObjectMemStack` | `0x802c34fc` | `bsobject.cpp`, 1116 |
| `g_numBSObjectsOnMemStack` | `0x802c3500` | `bsobject.cpp`, 1116 |
| `g_pScriptMemStack` | `0x802c3510` | `bsutil.cpp`, 1136 |
| `g_script_system_state` | `0x802c3520` | `bsutil.cpp`, 1136 |

These four-byte objects are not recreated or counted as source progress. Host
checks cover replacement, size/count calculations and state sequencing in the
trigger initializer; target compilation checks private bindings and the deleting
destructor ABI. Full-image verification and snapshot validation are separate
from host behavior checks. No runtime/emulator test was performed.
