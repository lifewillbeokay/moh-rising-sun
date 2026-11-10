# Sound

The ambient-track and settings fragments reconstruct eight functions (536
executable bytes) from the pinned GR8E69 executable, all from the original
`sound.cpp` (file record 1378). Claude Code
assisted the analysis and verification. Private `sound.cpp` storage and the private
`err` helper stay external original context.

| Manifest | Functions | Code bytes | Generated data |
| --- | --- | ---: | --- |
| `sound_getters` | `SoundGetVolumes`, `SoundGetOneShotBankID`, `SoundGetAmbientTrackEvent`, `SoundGetMusicEventID` | 92 | None |
| `ambient_track` | `AmbientTrack_Start` (private), `AmbientTrack_Select`, `AmbientTrack_Volume` | 320 | 144 bytes at `0x802a9bc4` |
| `ambient_track_location` | `AmbientTrack_SetLocation` | 124 | 4 bytes at `0x802a9c90` |

## Behavior

- `SoundGetVolumes` copies `g_sfxVolume` and `g_musicVolume` unless the global
  `g_bDisableSound` is set. The other getters copy `g_OneShotBankID`, the ambient
  event's current track (`+0x48`) and the global `gMusicCurEvent`.
- One ambient stream is driven through the private 76-byte `gAmbientTrackEvent`
  (`AmbientStreamSoundEvent`) and its handle `g_hAmbientTrack`; -12 means no stream.
- `AmbientTrack_Start` clears the event's word at `+0x24`, sets the float at `+0x28`
  to 0 and both halfwords at `+0x3c`/`+0x3e` to 70, then calls `SendEvent`. A
  negative result prints the original error through `err` and `DebugMsg` and stores
  -12.
- `AmbientTrack_Select(track)` stores the requested track at `+0x44`; without a
  stream it starts one, otherwise it calls `UpdateEvent(handle, 1)` only when the
  track differs from the current one.
- `AmbientTrack_Volume` stores the volume at `+0x34` while a stream exists.
- `AmbientTrack_SetLocation(position)` sets `g_ambientSourceOnPlayer` when the
  position is null; otherwise it copies the position into the 16-byte
  `g_ambientSourcePos` (a `CVector3`).

`include/game/SoundAmbience.h` is a member-only view of the event: it names the
touched fields and the two methods, and declares no table pointer or base class.

The `.rodata` between the two fragments holds an `AmbientTrack_Stop(): EndEvent()
FAILED` message. No `AmbientTrack_Stop` code exists in the executable; it is left as
original context rather than given an invented body.

Three [script built-ins](Script.md#objective-ambient-sound-and-value-built-ins)
now call these accepted ambient-track helpers. They select a track, convert and
clamp an integer volume percentage, and choose the current script trigger's
position or player-relative ambience. These 536 code bytes and 24 literal bytes
are verified in `src/script/` and counted separately from the sound functions above.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
fragment and its strings and constants. The complete rebuilt analysis image is
identical. Runtime behavior has not been tested.

## Grenade and bazooka sound events

Following the [explosion and thrown-bullet investigation](research/explosion-grenade-updates.md),
six fragments reconstruct twelve functions and **556 executable bytes**. These
include the `SoundAliasEvent::SendEvent` dispatcher and
`PositionSoundEmitter::SetPosition` used by `CThrownBullet::CommitUpdate`.
Codex assisted reconstruction directly from GR8E69; no external implementation
is imported. The fragments generate no allocated data and use the same ProDG
3.8.1 profile above.

| Manifest | Functions | Address | Code bytes |
| --- | --- | --- | ---: |
| `sound_alias_send` | `SoundAliasEvent::SendEvent` | `0x801679b0` | 320 |
| `sound_alias_destructor` | `SoundAliasEvent` deleting destructor | `0x80167b30` | 52 |
| `position_sound_copy` | `PositionSoundEmitter::Clone`, `SetPosition` | `0x801680a4` | 76 |
| `position_sound_defaults` | deleting destructor, `GetObjRef`, `GetType` | `0x8016831c` | 68 |
| `sound_event_defaults` | `SoundEvent::UpdateEvent`, `EndEvent`, `SetEmitter`, `SetListener` | `0x80283618` | 32 |
| `sound_emitter_defaults` | `SoundEmitter::GetFilterDef` | `0x8028366c` | 8 |

`SendEvent` first clears the private `SoundAliasEvent.cpp` pool, including when
the alias is zero. For a nonzero alias it asks `g_SoundMan` to fill the pool.
An empty list returns zero. Otherwise it sets each entry's emitter; when the
event's word at `+8` is nonzero, it also copies that word, `+0x1c` and the three
words at `+0x10` to entry offsets `+0x84`, `+0x88` and `+0x90` respectively.
It then schedules the list and returns the schedule result. The words retain their offset names. The entry-lifecycle reconstruction below
identifies the first two as a callback address and event mask; the three copied
parameter words still have unknown higher-level meanings.
The listener and float at `+0x0c` are not read by this function.

The initial `SoundEntryPool` view exposed its first 32 bytes, a
`dwi::list<SoundEntry *>`. The lifecycle work below extends that view through
the free-entry link at `+0x12e0`. The original private `s_entries` at `0x803e5874`
is 4,840 bytes; its storage and initialization stay original context. Its
manifest dependency is explicitly scoped to original file record 1455.
`Clear`, `SoundEntry::SetEmitter` and `SoundSchedule::AddSounds` are now
reconstructed separately below. Alias lookup remains an external body. `GetEntries`, `GetSchedule` and `SetFields` are descriptive inline
reconstruction helpers, not recovered original names or source boundaries.
The iterator's postfix increment preserves the observed old-iterator copies;
the existing prefix operation is unchanged.

`SoundEvents.h` follows the original virtual-slot order and observed stack
storage: emitter base 4 bytes, positioned emitter 16, event base 36 and alias
event 40. The positioned emitter holds three floats at `+4`, `+8` and `+12`;
it is not a four-component `CVector3`. `SetPosition` copies those three values
and preserves all other storage. `Clone` does nothing for a null destination;
otherwise placement copy construction writes the derived table pointer and
three floats. It does not allocate a destination or check its capacity.

The positioned emitter returns null for `GetObjRef` and 1 for `GetType`.
The base filter getter and event update/end defaults return zero. The setters
write only their respective pointer at event `+0` or `+4`. Pointer pointee types
for `GetObjRef`/`GetFilterDef`, and the unused return type of alias lookup,
remain provisional ABI declarations. The complete deleting-destructor bodies
are verified, including restoration of the base table pointer and conditional
object deletion. The inline base-destructor declarations earn no separate
credit for the still-original standalone base destructors.

Default event/emitter constructors and distance/azimuth methods are declared
but not reconstructed as matching source here. The maintained partial
`src/bullets/ThrownBulletCommit.cpp` supplies comparison-only inline initialization,
destructor and emitter-setter bodies using these declarations. Its complete caller
is measured under `DECOMP_COMPARE`; the shared header and accepted sound units
remain unchanged. No standalone constructor receives exact or linked credit.

All allocated sections, function boundaries and external references are
compared before complete-image verification. Sixty-three isolated PowerPC
fixtures compare original and production-linked execution: null and self clone,
three-component copying, pointer setters, deletion flags, zero aliases, empty
and multi-entry lists, optional parameter copying and scheduler return values.
Pool population, entry attachment, scheduling and heap deletion are stubbed.
These checks are separate from the unmatched explosion drafts and are not
Dolphin or game-runtime tests.

## Entry lifecycle and spatial playback

A follow-up reconstructs **eight functions / 1,352 executable bytes** across six
fragments. Codex assisted reconstruction directly from GR8E69, using the same
ProDG 3.8.1 `-O2 -G0 -fno-exceptions -fno-implicit-templates` profile. These are
dependencies of the sound dispatcher above and other events, rather than new
matching bytes in the still-unmatched explosion helpers or `CommitUpdate`.

| Manifest | Functions | Address | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `sound_entry_attach` | `SetEmitter`, `DoNotifyCallback` | `0x8015b8f4` | 176 | None |
| `sound_entry_spatial` | `Calculate3D` | `0x8015ba60` | 272 | None |
| `sound_entry_update` | `UpDateSoundParams` | `0x8015bb70` | 212 | None |
| `sound_entry_volume` | `CalculateVolDistance` | `0x8015bc44` | 84 | 8 bytes at `0x802a9fe8` |
| `sound_pool_entries` | `SoundEntryPool::Clear`, `AddEntry` | `0x80168720` | 364 | None |
| `sound_schedule_add` | `SoundSchedule::AddSounds` | `0x8015a284` | 244 | None |

Original file records are `SoundEntry.cpp` (1409), `SoundEntryPool.cpp` (1462)
and `soundschedule.cpp` (1406). The project sources remain fragments of those
files. Every generated allocated section and complete function boundary is
compared; no partial matches, discarded functions or original context earn
credit. The two volume constants are the float representations of `1/30` and
zero; these data bytes earn no code credit.

### Emitter attachment and callback filtering

`SetEmitter` stores null at entry `+0x50` for a null argument. Otherwise it points
that field at inline storage `+0x5c`, then invokes the supplied emitter's virtual
`Clone` with that destination. It does not retain the supplied pointer, allocate
storage, or destroy an earlier emitter. The original scene-emitter clone writes through destination `+24`, so the
aligned buffer occupies 28 bytes through entry `+0x77`, immediately before
the event ID. This does not reconstruct the emitter subclass itself.

`DoNotifyCallback` calls the address in entry `+0x84` with `(this, eventCode)`
only when the address is nonzero and the code shares a bit with the mask at
`+0x88`. There is no callback-result handling. This independently clarifies the
corresponding event words copied by `SendEvent`: event `+8` is the callback
address and `+0x1c` its mask. The declarations retain raw words to preserve that
32-bit interface. `SoundCBEventCode` is a recovered enum type name; its historical
enumerator names and full set of values remain unknown.

### Spatial state and volume

`Calculate3D` saves the previous azimuth and distance, then returns unchanged
when the emitter pointer is null. With an emitter it first calls `GetObjRef`;
only a null result leads to `GetType`. If both results are zero, a looping entry
sets its `+0x4c` word to 1 and keeps the cached spatial values. Otherwise it
queries azimuth, then distance. A changed distance sets dirty bit `0x2`, and a
changed azimuth sets `0x4`, preserving existing bits. These comparisons also run
on the missing-emitter-object path: a cached NaN distance sets `0x2` even when
no new distance was fetched. No higher-level name for the `+0x4c` word is claimed.

The distance-volume calculation preserves the original operation grouping:

```text
slope = volumeLimit * (attenuation * float(1 / 30))
if not (distance <= 0): v = volumeLimit - slope * distance
else:                  v = volume
if not (v < volumeLimit): v = volumeLimit
return v
```

The fields are `volume` at `+0x1c`, `volumeLimit` at `+0x20`, `attenuation` at
`+0x24`, and cached distance at `+0x54`. These are descriptive names, not recovered
member names. The final comparison imposes only an upper bound; negative results
are retained. The negated comparisons intentionally preserve unordered behavior.
No fast-math, reassociation or extra lower clamp is introduced.

`UpDateSoundParams` handles dirty flags in this order:

- `0x4`: call `SND3dpos(handle, azimuth, 0)` and retain its result.
- `0x2` or `0x1`: store distance-adjusted volume when entry `+8` is nonzero,
  otherwise the `+0x1c` value, at `+0xc4`; pass it to virtual `SetNormSndVol`.
- `0x8`: pass the signed halfword at `+0x18` to virtual `SetSndPitch`.

It clears the entire dirty halfword at `+0xa0`, including unrecognized bits, and
returns the `SND3dpos` result or zero when that call was skipped. It reloads flags
between callbacks; this is not a single snapshot of the initial flags.

### Pool reuse and scheduling

`Clear` walks the active list, explicitly invokes each entry's virtual destructor
without deleting the slot, places the slot on the free-entry chain at pool
`+0x12e0`, then erases/recycles the list node. Clearing an empty list does nothing.
The source starts a small free-link object's lifetime in the destroyed entry's
storage using placement construction; `SoundFreeEntry` and `Recycle` are
reconstruction helper names. No complete historical pool template is claimed.

`AddEntry` pops a free slot, asks the supplied entry to clone into it, and appends
the result to the active list. Both the entry slot and a list node must already
be available: the original supplies neither an exhaustion check nor allocation
fallback. The pool view is now a 4,836-byte prefix through that free-entry link;
it still does not reconstruct the full 4,840-byte object or its constructor.

`AddSounds` immediately returns zero when sound is disabled. Otherwise, a nonempty
list consumes one event ID from schedule `+0x228`, with 32-bit wraparound. For
each entry it applies the buss manager at `+0x22c`, assigns the event ID at entry
`+0x78`, conditionally calculates 3D state using entry `+8`, then calls `Add`.
A negative `Add` result returns zero immediately. Earlier entries and the consumed
ID are not rolled back, and later entries are untouched. A successfully processed
list returns its ID; an empty list returns zero without incrementing the counter.

The schedule remains a prefix view, and `cSoundBussMan` is method-only. Expanding
the schedule prefix preserves its position at sound manager `+0x837c` and the
sound-zone field at `+0x85f4`. Neither a prefix size nor a call establishes the
complete allocation of those objects. Sound-entry virtual slots are confirmed
by both original bank and stream tables, including signed `this` adjustments.
Unused external return types remain provisional where the accepted callers do
not consume them.

### Verification and remaining work

Six production-header builds verify all eight complete functions and generated
constants. Local original-versus-production-linked PowerPC fixtures exercise
**636 cases**: 6 emitter attachments, 60 callback masks, 80 spatial states, 220
volume cases, 144 parameter updates, 9 clears, 9 additions and 108 scheduling
cases. Checks include adjusted virtual calls, NaNs/infinities, signed pitch,
unknown dirty bits, ID wraparound, partial scheduling failure, free-list order
and preservation of selected untouched storage.

External callback, audio, clone, destructor, buss and scheduler operations are
stubbed where appropriate. These isolated checks do not establish exhaustive
equivalence or integrated Dolphin/game behavior. Complete-image verification,
the repository validation suite and the captured progress snapshot are separate
checks; existing matched function records must remain unchanged.

`SoundEntry::GetOpts` was left as research in that batch; it is now matched in
the bank-playback work below. The pool constructor, scheduler's `Add`, full buss
processing and alias lookup remain external work. Neither batch promotes the
explosion/update drafts documented in
[the previous research](research/explosion-grenade-updates.md).

## Bank-backed playback and entry options

Eight fragments reconstruct **sixteen functions / 1,236 executable bytes**.
Codex assisted reconstruction directly from GR8E69 with the same ProDG 3.8.1
`-O2 -G0 -fno-exceptions -fno-implicit-templates` profile; no external
implementation is imported.

| Manifest | Functions | Address | Code bytes | Generated data |
| --- | --- | --- | ---: | --- |
| `sound_entry_destructor` | `SoundEntry` destructor | `0x8015b1cc` | 120 | None |
| `sound_entry_copy` | `SoundEntry` copy constructor | `0x8015b420` | 60 | None |
| `sound_entry_options` | `GetOpts` | `0x8015b9a4` | 188 | 12 bytes at `0x802a9fdc` |
| `sound_bank_copy` | `SoundEntryBank` copy constructor, `Clone` | `0x8015bc98` | 128 | None |
| `sound_bank_play` | `Play` | `0x8015bd18` | 280 | 12 bytes at `0x802aa0b4` |
| `sound_bank_status` | `IsFinished`, `SetFade`, `ResolveSlotNum`, `Stop` | `0x8015be30` | 236 | None |
| `sound_bank_defaults` | destructor, `GetTypeID`, `UpdatePlayStatus`, `CheckFade` | `0x8015bf1c` | 68 | None |
| `sound_bank_parameters` | `SetSndPitch`, `SetNormSndVol` | `0x8015bf60` | 156 | 8 bytes at `0x802aa0c0` |

Every generated allocated section and complete function boundary matches. The
32 constant bytes earn no code credit; no discarded functions or original
context earn credit. The symbol table contains `SoundEntry.cpp` (1409) and
`SoundEntryBank.cpp` (1411), but lacks a separate code-start marker for the
latter. These new manifests leave original-file ownership unset: the class
names and file records do not prove the boundary between those source files.
The earlier association with `SoundEntry.cpp` is not a complete boundary map.

### Copying, destruction and patch lookup

The base copy constructor installs its table pointer and calls `copyData`, whose
body remains original context. The bank copy constructor then installs the bank
table and copies the word at `+0xd8` and byte at `+0xdc`, leaving tail padding
untouched. `Clone` does nothing for a null destination; otherwise it uses
placement copy construction in supplied storage, with no allocation or capacity
check. The header's `#pragma interface` keeps original tables and unrelated
out-of-line constructors external; it does not discard generated differences.

The base destructor installs the base table, invokes a present emitter's virtual
destructor with hidden flags 2, then clears the emitter pointer. Compiler-generated
code deletes the entry only when its incoming destructor flags have bit 0 set.
The bank destructor installs the derived table and forwards those flags to the
base destructor. The accepted 120-byte and 48-byte functions include these
compiler-generated paths; the source does not manually delete the inline emitter.

`ResolveSlotNum` acts only when the signed byte at `+0xc8` exceeds 1. It adds the
signed patch halfword at `+0xa8` to the signed variation byte at `+0xdc`, indexes
the signed-halfword table at `g_SoundMan + 0x10`, and replaces the patch. There is
no range or null check. These member names describe observed use; they are not
recovered original names. The manager layout remains a scoped view.

### Options, playback and volume conversion

`SoundPlayOptions.h` repeats the existing `SNDplaysetdef` unit's 24-byte record
declaration, using offset-based names. `GetOpts` first calls that initializer.
When entry `+4` equals 1, it gets distance-adjusted volume and copies the low
halfword of azimuth to options `+8`; otherwise it uses volume at `+0x1c` and
retains the default azimuth. It returns the unscaled volume through the reference,
sets the option's volume byte, and copies signed pitch to options `+0xc`.

The three volume paths have different conversion order and unordered behavior:

- `GetOpts` computes `volume * 127 + 0.5`, caps values above 127, maps negative
  or unordered values to zero, then converts directly to the byte result.
- `Play` first multiplies current volume by the global gain at manager `+0x80b8`,
  then computes `product * 127 + 0.5`. It compares the float against 127 and zero
  before converting directly to a byte. An unordered value reaches conversion.
- `SetNormSndVol` computes that same scaled expression, converts to `int` first,
  then clamps the integer to `[0, 127]` and calls `SNDvol`. Exceptional floating
  conversions retain target-instruction behavior; no portable C++ result is
  claimed for NaN, infinity or values outside the integer range.

The generated code preserves the separate gain multiplication and fused
multiply-add. Direct float-to-byte conversion resolved the register difference
in the earlier `GetOpts` draft. Helper names are descriptive, and an instruction
match does not establish the historical helper's exact declared return type.

`Play` obtains options and current volume, replaces the volume byte using global
gain, and calls `SNDBANK_play` with the bank word at `+0xd8` and signed patch.
A negative result stores handle -12 and returns -1. A nonnegative result is saved
as the handle. If filtering is enabled and an emitter exists, it requests the
emitter's filter; a nonnull result is added inside enter/leave-critical calls.
The filter-add result is ignored, and successful playback returns zero.
`SetSndPitch` forwards the handle and integer pitch without clamping or updating
entry storage. Opaque emitter return types remain provisional ABI views.

### Completion and verification

`Stop` always enters the critical section, calls `SNDstop`, leaves the critical
section, and returns -1 for a negative result or zero otherwise. It does not
reset the handle. This establishes an `int` return type, correcting the earlier
provisional `void` declaration. `IsFinished` calls virtual `Stop`, stores -12,
and returns 1 only when `SNDtimeremaining` is negative. Zero remaining time still
returns 0. The stop result is ignored. `SetFade` and `UpdatePlayStatus` are
no-ops; `GetTypeID` and `CheckFade` return zero.

Local original-versus-production-linked PowerPC fixtures pass **396 cases**:
16 copy/clone, 48 destruction, 4 default methods, 15 stop, 18 completion, 72
signed slot lookups, 7 pitch, 80 options, 48 normalized-volume, and 88 playback
cases. They check null destinations, preserved padding/storage, signed virtual
adjustments, destructor flags, negative lookup offsets, zero-time completion,
playback failure, filter gating/order, and finite/nonfinite volume inputs.
The playback fixtures use the new `GetOpts` body and the previously matched
`SNDplaysetdef`; copy fixtures stub the still-external `copyData`. External audio
and selected virtual operations are stubbed. These are isolated instruction
checks, not integrated Dolphin or game-runtime tests. Complete-image verification
and the repository tests are separate checks.

## EA audio and REAL runtime libraries

`src/sound/ea/` and `src/ea/` hold EA's C-style audio (`SND*`, `SFILTER_*`) and
runtime (`FILESYS_*`, `MEM_*`, `MUTEX_*`, `SIGNAL_*`, `FONT_*`) units, one per file
group in the code map. The Frontline reconstruction is their semantic reference;
every function was compiled with ProDG 3.8.1 and checked against GR8E69. Their
record layouts and most member names are inferred; a type named in a mangled symbol
is evidence for the name only.

`sndgs` is 0x324 bytes here and its members sit at different offsets from
Frontline's: the bank slot table is at `+0x1f4`, the memory pool at `+0x220` and the
abort-message hook at `+0xc8`. The sound system's public calls are partly C++ in this
build (`Snd::System::Restore`, `Snd::CODASetNew`), where Frontline uses C names.

These libraries were built with `-fno-strength-reduce`: a kept `mulli` or indexed `lfsx`
in a loop is the absence of strength reduction, not a code-shape puzzle. The `-G` value
differs by library; see the note on small data below.

Other layouts that differ from Frontline: a voice record is 140 bytes rather than 128,
and the voice table sits at `sndgs+0x1f0` with the voice count at `sndgs+0x17e`.
`sndmix` holds a pointer to its 96-byte channel records at `+0x1d8`. The stream record's
members above about `+0xd0` sit 40 bytes further along than Frontline's.

An effect bus record (`SNDFXBUS`, 20 bytes) has two tables in `sndgs`, one at `+0x1f8`
and one at `+0x20c`, and the voice send levels start at `+0x61` in a voice record.
`mapchannel` is 216 bytes and `remap` indexes it as a 6 x 6 x 6 table.

Several allocators and filter loops here reach table records through byte offsets: a
named `char*` record gives the table base first in the `add`, which `&table[i]` does not.
A guard followed by a bottom-tested loop is written as `if (c) do { ... } while (c);`.

`hlafile`'s `numrequests`, `freequeue` and `requestidcounter` are small statics of that
file. The original symbol table gives `.sbss` statics a size of 0, so the unit declares
them `extern` as original context instead of defining them. Their ownership is
therefore not verified by the build.

`include/game/SoundGlobals.h` and `SoundMixerView.h` are inferred member views of
`sndgs` and `sndmix`. Both are single struct objects in the original: their members are
reached through one base register, which a character-array view does not reproduce.

Small data is not uniform across these libraries, and units follow it. The REAL runtime
(`FONT_*`, `FILESYS_*`, `MEM_*`, `TIMER_*`, `THREAD_*`, `PRINT_*`) reaches its small
statics through `r13`, so its units use `-G8 -fno-strength-reduce`. The `SND*` code from
`0x80218efc` to `0x8022bed0` has 0 `r13` references across all 19,445 of its instructions,
and its 4-byte globals such as `Snd::gMutexLockFn`, `MIXstopfn` and `everyother` sit
outside `.sdata` and are reached with `lis`, so new units there use `-G0
-fno-strength-reduce`. That is an inference from the instruction stream, not a recovered
build setting. `SND*` units landed before this was established keep `-G8`; they match as landed and
have not been migrated.

Ownership of some small statics is not verified by the build: `g_thMain`,
`MEM_allocfailcallback` and the timer statics are declared `extern` as original context,
like `hlafile`'s statics above, rather than defined in a unit.

The pathx layer (`PATHX_*`) sits inside `sound.cpp`'s sections: its `.rodata` continues
that file's pool and `pathxinfo` follows `g_objSound` in `.bss`, while its private helpers
carry C++ mangled names and its public functions C names. Its units use the game's flags,
without `-fno-strength-reduce`: `PATHX_igetslot` and `PATHX_initservice` have strength-reduced
loops. A slot is 64 bytes: stream or bank `+0`, sound handle `+4`, stream request `+8`,
patch `+12`, node count `+16`, requested volume `+20`, saved pitch `+24`, sound flag `+26`,
name `+27`, bank data `+36`, play settings `+40`. Those offsets are verified; the member names are
inferred. `PATHX_status` is not reconstructed: it matches, but its `.rodata` run (two doubles
and a float, 20 bytes) is directly followed by another function's string, so a fragment cannot
be verified alone.

The music path player (`PATH_*`, `PATHI_*`) is one stretch of code from `0x800356a0` with no
file evidence. Its units use the game's flags, without `-fno-strength-reduce`: loops such as
`PATHI_gettrackptr`'s walk by 540 bytes. `pfstates` holds four 540-byte project states; each
has a 24-entry track pointer table at `+444` and 16 queued 24-byte events at `+60`. A handle's
top nibble selects projects, the next nibble songs and the low 24 bits tracks. Member names are
inferred. Not reconstructed here: `PATHI_sortprojects` (the struct-swap loop's address computations
are hoisted differently), `PATHI_eventstart` and `PATH_addmapfile` (their structure is right but
their registers are allocated differently), and `PATH_createtrackfrombuf` and `PATH_event` (drafts
that have not reached a match).

The MicroTalk unpackers (`supmutf.c`, `supmutlf.c`, `supmutpf.c`) drive `Snd::CMTBLKDecf`, a 3,432-byte
decoder. A state record has its decoder at `+40` (`supmutf.c`), `+28` (`supmutlf.c`) or `+52` (`supmutpf.c`);
`Snd::MTFSTATE` is a 12-byte record whose `+8` word is set to 1 on a restart. Those offsets are verified;
the member names are inferred. The driver's per-channel record is 64 bytes with its AX voice at `+60`.
`SNDPLATFORM_setpitch` scales the voice's rate by `+138` of the voice record.

`sndgs` keeps a table of periodic clients at `+388`, counted by the signed byte at `+375`;
`SNDSYSI_100hzserver` calls them after `iSNDserve`. A voice record's envelope segments are 8-byte
{duration, level} records and a negative duration means the segment holds. A packet player's
platform record is 9,600 bytes of buffer followed by staging pointers (`+9600`), its voice (`+9624`),
the buffer half playing (`+9625`), a start countdown (`+9626`), its DSP addresses (`+9628`), the buffer
midpoint (`+9652`) and the frames consumed from each half (`+9656`). Those offsets are verified; the
member names are inferred.

`SNDPLATFORM_set3dpos` reads a per-layout table of 4-byte {azimuth offset, gain} records at `+548` of
`sndgs`, with 6 channels per layout; the layout comes from the voice's channel count and the output
count. `SNDDRV_dolbypl2balances` is 256 rows of four 16-bit levels indexed by azimuth over 256. A voice
record's channel azimuths are at `+80`. Those offsets are verified; the member names are inferred.

The DSP mixer's channel records (`__MIXChannel`, 76 bytes each) hold a current and target pair for each level
from `+36`. `SNDDRV_DSPMixerUpdate` copies targets to the current values on flags in the channel's mode word
(`0x20000000` and `0x10000000` for the volume, `0x80000000` and `0x40000000` for the pan) and writes deltas over
320 samples to the AX voice. Those offsets and flags are verified; the member names are inferred.
`SNDDRV_DSPMixerResetChannel` is not reconstructed: its run of stores is in a different order.

`sndgs` holds a channel map at `+716`, six rows of six 16-bit values indexed by channel count less one; `SNDI_patchtohdr`
adds it to each channel's pan offset. A big-file entry's offset field may be wider than 4 bytes; `BIG_locateentryz` reads
it as 64 bits and stores the low word. Those offsets are verified; the member names are inferred.

The runtime's variadic entry points (`REAL_abortmessage`, `SYSTEM_abortmessage`, `PRINT_string`, `FONT_drawtextfa`)
copy the record `__builtin_saveregs` builds with inline word moves. Under `cc1plus` the standard `va_start` expands to
`__builtin_memcpy`, which here is a call to `memcpy`, so these units write the same copy as a record assignment. That
form is inferred. The same functions also match with the standard macro when compiled as C, but the code map names
their files `.cpp`. The mixer hooks (`MIXinitfn` through `MIXsetpitchfn`) are declared with the signatures of the
`MIX_*` functions they hold; `MIX_audioslice`'s parameters are inferred from its caller in the sound driver thread,
since it is not reconstructed yet. `TIMER_init` reads the bus clock from low memory at `0x800000F8`; `OSLOWMEM` is an
inferred view.

`MEM_free` passes a record to two class hooks at `+100` and `+104` around the free; the alloc hooks at `+92` and `+96`
are the same pattern in `MEM_allocaligna`, which is not reconstructed yet. The record's members are inferred from their
stores. `openfile` opens host files through `PCopen` and `PCcreat`, with the file number stored as an `int`, as
`openfile_plat`'s mangled name shows; `MyStrCmpi`, despite its name, returns the prefix's length when the name starts
with it. The name also belongs to a different function near `0x801b1448`.

`RCMP::AV_PLAYER` is reconstructed in fragments (`avplayer_ctor`, `avplayer_dtor`, `avplayer_time`, `avplayer_chunk`). Not
reconstructed here: `Init`, `GetFirstFrame` and `GetRCMPChunk`, which have small differences, and `GetFrame`, whose code
matches but whose `.rodata` holds a second, unreferenced copy of the 2^31 literal at `0x802b8560`. `CHUNK` has an empty
inline destructor: `delete chunk` tests for null, which it does not for a class without one.

## Sound busses and event destructors

`cSoundBuss` records are 216 bytes: the free list threads `g_AllBusses` (32 records) through `+0`,
with eight 24-byte events from `+4`. The clock at `+208` advances on each update, and when it passes
the current event's time the buss moves on or stops and takes that event's level as its volume. Member
names in [`SoundBuss.h`](../include/game/SoundBuss.h) are inferred from use. `InsertSoundBus`,
`SetSoundBuss`, `UpdateBussEntry` and `RetroFit` are not reconstructed.

`cSoundBussMan` is 76 bytes, bounded by `soundZone` after the schedule in `g_SoundMan`: an active count at `+4`,
a buss list linked through `next` at `+8`, and the `dwi::list`s of inactive (`+0xc`) and active (`+0x2c`) busses.
A new sound takes the inactive busses it names by CRC in `bussCRCs` and then each active buss's retrofit.
Member names are inferred. `Update`, `FindBuss`, `RetroFitNewSound` and `ApplyActiveBusses` are reconstructed;
the constructor, `Init`, `Shutdown`, `AddBuss`, `ActivateBuss`, `AddInactiveRetroFit`, `ApplyInactiveBusses` and
`AddActiveBusses` are not.

## Sound manager

`g_SoundMan` (`cSoundMan`, 0x8700 bytes) loads the level's sound table and bus table and owns the
stream pool. The table offsets come from `LoadLevelUbr` and `LoadLevelBus`. A file is a 32-byte
`UberFileHeader`, an array of 12-byte `ColumnMap` records, and fixed-size rows; the sound table is then
followed by the slot map and, when the header word at `+0xc` is set, a 132-byte bank table.
`GetUbrCRC` and `GetBussCRC` find the column of type 6 and read each row's alias CRC from it, which fills
the two sorted row tables. Their extents are inferred from their neighbors: 4096 sound rows at `+0xb0`,
50 bus rows at `+0x80d4`. Names of members, header fields and `ESoundUberTask` enumerators are inferred
from use; `UberFileHeader`, `ColumnMap`, `ESoundUberTask` and `BankMan` are symbol-table names.

Observed quirks, kept as they are: `RemoveLevelStreamFile` passes the stream handle word where
`FILESYS_delbigsync` takes a name, and `VerifyLevelBanks` builds its bank file names and never uses them.
Not reconstructed: `SetSfxVol`, `ServiceStreams` and `PauseBankSounds` (they own the
`source/tl/dwi_memory.h:56` literal in this file's shared header strings), `GetMultSettings` and
`GetSoundEntries` (inlined `SoundEntryBank` destructor), `LoadLevelBus` and `SearchForAlias`.

## Sound system front end and block decoders

`Snd::System` and `Snd::Memory` (`ssys.cpp`) wrap the `SNDSYS_getopts` and `SNDSYS_setopts` record:
each setter copies the 212-byte record out, changes one field and writes it back. `SetHeap` keeps the
main heap in `Snd::pheap` and `Snd::heapsize` and the fast heap in the record. The record's field names
are inferred from these setters. `SetVoices` and `System::Init` are not reconstructed: both set `r3` for
`SNDSYS_setopts` before their switch, which no plain form tried reproduces.

`Snd::CMTBLKDecf` (`mtdecf.cpp`) derives from `UTALKSTATE_CODA`, whose layout is verified: bit reader
at `+0`, a first-bit flag at `+0xc`, shift at `+0x10`, a 64-entry gain table at `+0x14`, reflection and
filter state at `+0x114` and `+0x144`, 324 history samples at `+0x174` and 432 output samples at `+0x684`;
the decoder's own members follow at `+0xd44`. When the first-bit flag is clear `decodemut` reads no grid
fields and takes samples one apart instead of two. Codec version 1 streams carry a run of raw 16-bit
samples, introduced by the marker byte `0xEE`, that `Decode` copies over the output between frames.
Member names are inferred. `readsamples` and `decodemut` are not reconstructed; `Snd::CEAXABLKDecf`'s
`process_raw_block`, `decodexac` and `Decode` are not either. All of these units use `-G0
-fno-strength-reduce`, like the other new units in this library.

`Moh3SoundEvent::Pause` stores its argument in the static `s_paused` and in `g_npcVocalMan` at `+0x44`;
`EndEvent` removes the event's sound from the schedule. The class is a method-only view so far: its table
and its data members are not reconstructed, and the `+0x44` member name is inferred.
