# Frontend screen events and co-op helpers

This batch builds on the frontend interfaces and callees contributed by
kyleckroeger in [PR #64](https://github.com/lifewillbeokay/moh-rising-sun/pull/64)
and [PR #65](https://github.com/lifewillbeokay/moh-rising-sun/pull/65). Thank you
for the shared handler, settings, package and input layouts: they make these
screen paths substantially easier to reconstruct and check.

The source was reconstructed with AI assistance from the pinned GR8E69
executable's symbols, instructions and data. Ghidra 12.1.4 exports were checked
against the original function boundaries and loaded bytes. No new Frontline or
other-game source was copied. Original files, bulk exports and experiments stay
in ignored directories.

## Scope and accounting

The batch covers **24 functions / 7,152 original executable bytes** in 18 source
fragments. Three functions are registered for exact integration:

| Function | Original bytes |
| --- | ---: |
| `HandlerMPBots::IsSkinUnlocked` | 200 |
| `HandlerSpecialFeatures::HandleEvent` | 184 |
| `ShellManager::GetIndexPadLastPressedAccept` | 32 |

The other 15 comparison fragments contain **20 partial functions / 6,688 bytes**
and one **exact-unlinked function / 48 bytes**:
`HandlerCoop4WaySplit::GotoNextScreen`. Its fragment also contains the partial
`Init`; it has not been promoted into the verified image.

The retained source covers list selection and wraparound, slider key endpoints,
multiplayer skin checks, co-op join records and controller masks, co-op menu
setup and options, level selection, save loading, virtual keyboard input, and
briefing/history text. Partial bodies use `DECOMP_COMPARE` and the shared
`fecode_setcellcolor.json` ProDG 3.8.1 profile. No function-specific flags,
instruction patches, clipped pools or discarded target bodies are used.
Original bytes remain in the verified image for every unlinked function.

## Evidence and storage ownership

Existing shared headers suffice for this batch. The two private `skinLock`
tables and their `NumLockSkins` counts belong to different original file scopes:
`handlerMPBots.cpp` and `handlerOnlineOptions.cpp`. Each observed record is
12 bytes (cheat, skin and team integers); these descriptive member names do not
claim original type names. They remain original external data.

Several handlers cache event hashes in original private symbols. C++ declaration
labels such as `asm("sKeyPressed.3279")` bind descriptive source names to those
real symbols; they contain no assembly instructions. Each manifest records
`local_externals`, `original_file` and `original_file_index`, preserving the
otherwise ambiguous local names and guards. The original zero-initialized cache
storage stays external. The source retains guard checks, first-use hash calls,
and guard writes, including cached hashes that the body does not subsequently
use. This reconstructs the functions without claiming new BSS ownership or
changing the comparison tool's BSS restrictions.

The complete generated string pools are placed at the original addresses:

| Fragment | Pool address | Bytes |
| --- | --- | ---: |
| `handler_briefing_events` | `0x802b4fb8` | 128 |
| `handler_coop_level_start` | `0x802b0ca0` | 264 |
| `handler_coop_options_events` | `0x802b3718` | 212 |
| `handler_coop_split` | `0x802b3664` | 40 |
| `handler_loadgame_accept` | `0x802b3c6c` | 60 |
| `handler_object_lists` | `0x802b0908` | 24 |
| `handler_special_features_events` | `0x802b35b4` | 32 |
| `handler_two_player_events` | `0x802b3af8` | 208 |
| `handler_virtual_keyboard_events` | `0x802b44dc` | 104 |
| `shell_control_masks` | `0x802ae3ec` | 68 |
| `slider_initialize` | `0x802ad934` | 8 |

The control-mask comparison explicitly lists `shell_dompcontrollerchecks` as
data context. Its complete 68-byte pool contains that accepted unit's entire
32-byte pool at `0x802ae410`. It emits no context functions and earns no second
credit for the strings or accepted code. The two-player handler is associated
with `handlerCoopProfile.cpp`, which contains the surrounding co-op profile
code; the symbol table does not record a separate `handler_2P_Menu.cpp` file.
These fragments are not claims about complete original translation units.

## Preserved behavior and open questions

- Object-list indices are one-based. Next/previous scan for the first object
  whose current script is case-insensitively `Show`, hide it, and wrap using
  failed object/script lookups. Missing selected scripts can leave an unbounded
  scan; no new limit is added.
- Resetting a join record compares its signed stored port with the unsigned
  argument. Passing 255 does not match the stored -1 sentinel. Joining with
  both records occupied writes the following storage at index two, as the
  original does; this is not repaired or treated as a valid third player.
- Controller updates preserve upper bits of the joined-player byte, ignore
  unmapped devices, and set split-package masks and group IDs. Co-op's spare
  controller mask is the complement of the waiting package's low mask byte,
  restricted to four bits; its group ID is a different field.
- Co-op sequence traversal preserves direction, wraparound, duplicate entries,
  and the original fallback for a value absent from the sequence. The options
  handler keeps the existing split-sequence data external and always refreshes
  its displayed properties, including on unhandled events.
- The keyboard's concrete handlers share the observed cursor word at `+0x50`.
  This body uses a scoped access without adding a field to the base class and
  moving derived members. First-key clearing, backspace, explicit clearing,
  virtual length calls, and spaces after the first terminator preserve their
  original order.
- Save loading validates the selected save's mission, selects the co-op player
  when reading it, and retains the separate player-zero restore call. Disc-swap
  failures stop the transition; co-op shell loads defer restoration through
  `mSaveSlot`. Changing levels, restarting the current level, controller-port
  selection, marker recording and package switching remain distinct branches.
- The two-player menu's setup finally writes a control mask of 15, even after
  earlier mask calculations. Briefing and history screens use different labels;
  missing localization results assign an empty string, and both paths mark
  the text object changed.

Exact instruction scheduling remains unresolved in the partials. In particular,
slider vector copies currently use integer loads/stores where the original uses
floating loads/stores. The comparison therefore does not establish equivalent
behavior for all floating representations or overlapping storage. Source and
isolated tests also do not establish safety for arbitrary invalid inputs,
callback mutations, aliasing or a running game.

Further drafts for bonus options, multiplayer global options, co-op outro,
join-screen updates and unjoining remain research-only. They reuse literal
storage from other functions, so independent function compilation does not yet
reproduce the complete original pools. No invented external string names or
shared storage between comparison units are used to bypass that issue. A
follow-up can recover the corresponding source/data grouping, retaining exact
context where its addresses and bytes remain unchanged.

The deferred entry points are `HandlerMPGameGlobalOptions::HandleEvent`
(`0x8018cb38`, 584 bytes), `HandlerBonusOptions::HandleEvent` (`0x80194fc4`,
944 bytes), `HandlerPauseOutroCoop::HandleEvent` (`0x80197e28`, 760 bytes),
`DoMPJoinScreenUpdate` (`0x80182060`, 280 bytes), `DoCoopJoinScreenUpdate`
(`0x801822dc`, 352 bytes), and `DoCoopUnjoin` (`0x8018265c`, 456 bytes).
The bonus handler reuses its accepted constructor's `bonus%.2d text` literal.
The outro's new suffix starts at `0x802b4c98` but shares earlier outro literals;
the multiplayer global-options suffix starts at `0x802b1bbc` and shares Init's
pool. The shell drafts reuse the pool already owned by the control-mask
comparison and its accepted data context. Those are concrete grouping problems
for the next batch, rather than missing event or controller interfaces.

## Validation

`tests/test_frontend_screens.py` adds **17 test groups / 1,341 scenarios** covering
all 24 functions with original and native PowerPC execution under optional
Unicorn. It compares memory, integer results and callback order. Cases include
empty/full join records, signed port sentinels, skin-table duplicates and misses,
list wraparound, all implemented menu events, cold and warm hash caches,
controller assignment, disc swaps, co-op save routing, keyboard clearing and
briefing localization. Services are stubbed; these are isolated checks, not game
runtime tests or a proof of full equivalence.

Original-object baseline, full source-image verification, native comparison
measurements and behavior tests are separate checks. Public CI validates the
locally verified snapshot and exports its report; it does not rebuild the game
or have access to original files.

Local validation passed the pinned-target audit, original-object baseline
(2,860,576-byte normalized DOL), all **2,607 accepted source units** in the
byte-identical complete-image rebuild, all **123 comparison units**, and all
**192 tests** with Unicorn available and no skips. Capture reproduced the
comparisons and the public report export passed. Linked source is **1,341,464
bytes / 7,052 functions** (949,248 reconstructed-game bytes and 392,216 restored
library bytes). Including 536 exact-unlinked bytes, exact source totals
**1,342,000 / 2,492,032 bytes (53.851636%)**; linked coverage is **53.830127%** and
similarity is **58.465601%**. The snapshot records the verified inputs.

## Recorded comparison measurements

Complete-function objdiff 3.8.3 measurements, weighted by target function size
within each fragment. Differing bytes include size differences and are separate
from similarity. Exact-unlinked source earns exact credit, but not linked credit.

| Source fragment | Functions | Original bytes | Similarity | Differing bytes |
| --- | ---: | ---: | ---: | ---: |
| `handler_briefing_events` | 1 | 424 | 74.64% | 250 |
| `handler_coop_level_start` | 1 | 476 | 98.29% | 87 |
| `handler_coop_options_events` | 1 | 916 | 85.72% | 456 |
| `handler_coop_sequence` | 1 | 144 | 52.31% | 90 |
| `handler_coop_split` | 2 | 344 | 90.38% | 59 |
| `handler_loadgame_accept` | 1 | 600 | 86.79% | 526 |
| `handler_object_lists` | 4 | 968 | 77.13% | 699 |
| `handler_online_skin` | 1 | 140 | 32.14% | 128 |
| `handler_two_player_events` | 1 | 628 | 91.36% | 241 |
| `handler_virtual_keyboard_events` | 1 | 588 | 86.77% | 200 |
| `shell_control_masks` | 2 | 724 | 77.90% | 556 |
| `shell_coop_join_records` | 2 | 340 | 57.08% | 299 |
| `shell_coop_join_update` | 1 | 168 | 82.24% | 98 |
| `shell_start_mp` | 1 | 124 | 93.42% | 90 |
| `slider_initialize` | 1 | 152 | 73.42% | 36 |
