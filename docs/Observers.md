# Observer and deferred-destruction reconstruction

The accepted observer subset has **22 functions and 2,172 matching game-code
bytes**: nine weak-pointer event handlers (648 bytes), eight subject/observer
methods (476 bytes), and five destruction-queue methods (1,048 bytes). Two queue
string pools add 112 verified data bytes and no code credit. AI assistance was
used to reconstruct and verify these functions from the pinned executable.
The short-string helper described below adds another 60 game-code bytes.

## Evidence and shared declarations

Original method symbols and named vtable entries establish `IDestructible`,
`ISubject`, `IObserver`, and the `WeakPtr<T, N>` specializations. The relevant
vtables are at `0x802e9af8`, `0x802e9ad0`, and `0x802e9aa0`; they remain original
context. The declarations in `include/game/IDestructible.h` and `ObserverTypes.h`
are scoped reconstruction interfaces, shared with the target-sorting declarations
in [GameAlgorithms.md](GameAlgorithms.md).

The observed sizes are 4 bytes for the destructible prefix, 8 for a subject, and
24 for an observer. A subject's observer-list head is at offset four. An observer
adds a list node at offset eight, containing two links and an owner pointer,
then its subject pointer at offset twenty. `ObserverLinkBase`, `ObserverLink`,
`ObserverHead`, `field_04`, and helper/member names are descriptive choices;
original nested class names, access control, and source factoring are unknown.

The backward link is represented as a pointer to the preceding next-pointer
field. This supports both the subject's four-byte head and a preceding node
without treating the head as a complete node. `AddObserver` independently
confirms the head/node addresses and writes; insertion and unlinking compile to
the original instructions. The existing target algorithms and weak handlers
also use this representation and pass complete comparisons.

`#pragma interface` preserves the SN/GCC boundary between inline operations and
external class/vtable definitions. The empty `IDestructible` destructor is inline
for the verified subject destructor. Its separate 52-byte out-of-line body and
compiler-emitted vtable remain unaccepted context. No generated vtable is dropped
from an accepted object. The source fragments are not recovered historical
translation-unit boundaries.

The [script/game-object bridge](Script.md#script-game-object-bridge) now reuses this
observer layout for the `WeakPtr<BSGO_Basic, 8>` embedded in `BSObject`. Its boolean
conversion and arrow access preserve the original spatial callers' null checks
and subject reads. The bridge contributes no additional observer event handlers;
its accepted functions are counted with script and AI targeting code.

## Accepted methods

| Function | Original address | Code bytes |
| --- | --- | ---: |
| `ISubject::~ISubject` | `0x8014acd0` | 92 |
| `ISubject::MarkForDestruction` | `0x8014ad2c` | 56 |
| `IObserver::~IObserver` | `0x8014ad64` | 112 |
| `ISubject::NotifyObservers` | `0x8014add4` | 112 |
| `ISubject::AddObserver` | `0x8014ae44` | 64 |
| `IObserver::HandleEvent` | `0x8014ae84` | 4 |
| `IDestructible::MarkForDestruction` | `0x8014ae88` | 32 |
| `IDestructible::Destroy` | `0x8014b2f4` | 4 |
| `CDestructorQueue::CDestructorQueue` | `0x8014aea8` | 92 |
| `CDestructorQueue::Init` | `0x8014af04` | 68 |
| `CDestructorQueue::Reset` | `0x8014af48` | 104 |
| `CDestructorQueue::Add` | `0x8014afb0` | 372 |
| `CDestructorQueue::Execute` | `0x8014b124` | 412 |

Notifications cache the next node before calling the current observer, allowing
that callback to unlink its node. Subject marking forwards the countdown to the
queue and sends event value 4. Subject destruction sends value 8. Observer
destruction unlinks and clears its subject before invoking the subject destructor.
The base event handler and base `Destroy` are empty, as the original bodies show.
The enum's placeholder zero value does not claim a complete event enumeration.

The 16-byte destruction queue contains an STLport map from `IDestructible*` to
`int`. Its constructor, node allocation, key comparison, and template symbols
agree on this storage. The byte initialized at offset twelve belongs to the
map's comparator storage; it is not an invented queue flag. The singleton at
`0x802c4d58` remains externally owned storage.

`Add` checks for an existing object and prints the original duplicate-request
message instead of changing its countdown. `Execute` decrements each countdown,
calls the object's virtual `Destroy` when it reaches zero or below, and removes
the entry. Otherwise it stores the new countdown. The loop caches its end
iterator and preserves the original increment/erase order. `Reset` clears entries;
it does not call each object's `Destroy`.

`Init` allocates 16 bytes with `DWI_alloc`, using flags 1024 and the original
`source/scene/destructor_queue.cpp:38` label. The class allocation overload is a
reconstruction of that observed allocation-and-construction sequence, not proof
of the original macro or overload spelling. See [Memory.md](Memory.md) for the
allocator boundary and unresolved flag semantics. Both entire string pools,
including padding, match at `0x802a8998` (40 bytes) and `0x802a89c0` (72 bytes).

## Short-string helper

`CopyString<short>` at `0x80277f18` has 60 matching bytes. The mangled signature
fixes the destination to `short*` and templates the source element type. The
retained short specialization returns immediately for a null destination, copies
until a zero source element, and appends a zero. A null source produces an empty
destination. It takes no capacity argument. Its separately named bounded overload
remains unmatched. No complete string class or original source ownership is
inferred from this helper.

## Verification boundary

All these fragments use ProDG 3.8.1 with `-O2 -G0 -fno-exceptions
-fno-implicit-templates`. This is a working profile, not a proven historical
compiler release. Each complete generated object, symbol, external reference,
section and function range is checked before the full reconstructed analysis-image
comparison. No instruction patches, partial credit, discarded generated output,
or original-context credit are used. Runtime and emulator behavior remain untested.
