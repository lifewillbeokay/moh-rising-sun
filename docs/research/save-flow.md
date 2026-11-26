# Save flow and GameCube memory-card operations

This batch builds on the shared frontend interfaces contributed by kyleckroeger
in [PR #64](https://github.com/lifewillbeokay/moh-rising-sun/pull/64) and
[PR #65](https://github.com/lifewillbeokay/moh-rising-sun/pull/65), and the popup
layouts verified in [PR #70](https://github.com/lifewillbeokay/moh-rising-sun/pull/70).
Those interfaces make it practical to reconstruct and test the callers that
coordinate popups, profile data and the card library. Thank you for that groundwork.

The implementation was reconstructed with AI assistance from GR8E69's pinned
symbols, instructions and data. Ghidra exports were checked against complete
original function boundaries and loaded bytes. No implementation from another
game was copied. Binaries, exports and exploratory variants remain ignored.

## Scope and accounting

The batch adds **25 functions / 6,292 original executable bytes**. Eight functions
in four exact fragments contribute **588 linked code bytes**:

| Fragment | Functions | Code bytes |
| --- | ---: | ---: |
| `mc_display_reply` | `CMemoryCard::Display`, `RequestReply` | 220 |
| `mc_wait` | `CMemoryCard::Wait` | 164 |
| `mc_cache_device` | `CMemoryCard::Cache` | 172 |
| `mcgc_results` | Save/load success, default reply, declined-to-save accessors | 32 |

The other **17 functions / 5,704 bytes** are registered partial source. Their
original code remains in the verified image:

| Comparison fragment | Functions | Target bytes |
| --- | ---: | ---: |
| `mc_startup` | Startup language selection and card initialization flow | 484 |
| `mc_save_load` | Profile save and load loops | 808 |
| `mc_verify_crc` | Scratchpad CRC and profile-version validation | 280 |
| `mcgc_operations` | 13 GameCube card operations, listed below | 4,132 |

`mcgc_operations` includes `CheckStart`, `IssueSave`, `CheckSave`, `IssueLoad`,
`CheckLoad`, `IssueScan`, `CheckScan`, `ProcessReply`, `DisplayTRCMessage`,
`IsOurs`, `TellTheGamecubeToShutUp`, and `CMCDevice::IssueCache` / `CheckCache`.
It preserves the contiguous original range `0x801a5dbc..0x801a6de0` and the
complete 132-byte generated read-only data range at `0x802b6e48`.

All fragments use the shared ProDG 3.8.1 `-O2 -G0 -fno-exceptions
-fno-implicit-templates` profile. Comparisons inherit `fecode_setcellcolor.json`;
partial bodies use `DECOMP_COMPARE`. No instruction patching, private flags,
clipped data or discarded target functions are used. Similarity and native sizes
are measured separately from byte/address exactness. For example, the verbosity
setter has matching instruction bytes but moves within the partial unit, so it
is still partial and earns no exact credit there.

The measured comparison scores are 97.570250% for startup, 95.396550% /
92.732560% for save/load, and 92.500000% for CRC validation. The GC operation
unit spans 0–99.99%: `CheckLoad` currently scores 0% and `CheckSave` 20.174826%,
even though their isolated message cases pass. Both remain partial. The native
outputs are measured in full; passing stubbed cases does not override low
similarity or prove all runtime behavior.

## Recovered boundaries and behavior

The preserved `CMemoryCardGC` virtual table supplies names for previously
anonymous `CMemoryCard` pure-virtual slots. The base object's table pointer is at
`+4`; `CMemoryCard::Device` and the Realmc client interface use `+0`. The existing
`DoIssueTRCScan` / `DoCheckTRCScan` names are retained for compatibility with older
scoped declarations. The new GC view is not constructed and does not claim a
complete class definition or recreate its virtual table.

`CardInterfaceView` describes slot order from `_vt.Q26Realmc11GCInterface` without
expanding the narrower declarations used by accepted library units. Only called
slots have recovered argument types; unused declarations reserve positions.
`CardFileInfoView` preserves the existing 36-byte `Realmc::FileInfo` prefix and
12 further bytes, rather than changing the driver's embedded layout. The GC
client copies 48 bytes and stores it at `+0x58`. `CardMessageView` records the
observed message prefix, including options at `+0x24`, file details at `+0x40`,
and the descriptor at `+0x54`. Descriptive fields and views are not recovered
original names or complete public/on-disc formats.

Key behavior retained:

- Startup selects the language only for the relevant SKU and invalid loader
  state, translates game language IDs to the two locale tables, and waits even
  on the poll that completes initialization. Its private `wasAlreadyHere.4236`
  flag is read but not set by this function.
- Save/load retry status `4` restarts the request; status `2` finishes polling.
  Save chooses an available device when the profile contains `0xbad`. Load
  preserves the current language across the profile copy. Save's final return
  values distinguish failure, declining, a write, and success without a write.
- `IssueSave` initializes private `fileInfo.3256` once. It changes the filename
  and **comment** pointer/length (`+0x14/+0x18`), preserving the title fields.
  This distinction is covered by cold and warm cache tests.
- CRC validation allocates the observed 28-byte `CCRC`, uses the existing CRC
  helpers, releases it before checking the trailer, and checks both checksum and
  `SPlayerProfile::VERSION`. Its entire 24-byte allocation-label pool is verified
  at `0x802b6688`.
- Card polling preserves reply precedence, restart status, reset gating, audio
  pause/resume ordering and scan callbacks. File ownership checks compare the
  company/game codes before requiring a name shorter than nine characters.
- Cache operations use 16-bit card names and replace nonbreaking spaces with
  spaces. The two block-count fields retain the previously descriptive names;
  the original clamps the second to the first. No broader semantic claim about
  those names is inferred from this operation.
- `DeclinedToSave` returns zero in this GameCube implementation. The nearby
  `+0x90` field gates reset handling; it is not the result of this accessor.

The preserved `PCUw` signature records a wide-character ABI whose original
source spelling remains unknown. `CMemoryCard::Display` and the popup call now
use a documented 16-bit pointer view with explicit labels for their **existing
symbols**. These declarations preserve the call boundary; they do not substitute
assembly or invent symbols for anonymous data. The forwarding function matches
exactly. `ProcessReply` emits its four-byte space-and-terminator data from a
local `const unsigned short[]`, matching the original generated data. The
[save-popup follow-up](save-popups.md) now reconstructs the popup's own
`Display` implementation and `CSaveGamePopup::SetString` as measured partials.

## Tests and verification

`tests/test_save_flow.py` adds **16 test groups / 1,182 original/native case
pairs**. It checks callback order, state and buffer contents across startup
languages, cold/warm file-info initialization, save/load retries, success/failure
returns, checksum/version corruption, zero-length CRC input, reply states,
virtual `this` adjustments, card message/TRC dispatch, scans, ownership checks,
16-bit name conversion and result accessors.

The optional Unicorn tests execute original and compiled PowerPC functions.
They stub game services, audio, timing and card I/O; they are isolated behavioral
checks, not gameplay, emulator boot, real-card testing or proof of equivalence
for every input. Public CI skips original-dependent execution. The full-image
build continues to link original code at all partial ranges.

Verification commands:

```sh
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 tools/comparisons.py
scratch/script-pool/venv/bin/python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
python3 tools/progress_report.py
```

The original-object baseline and the complete source build both reproduce the
2,860,576-byte derived DOL, SHA-1
`6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. The build verifies **2,616 accepted
units**, with **1,343,148 linked code bytes** (950,932 reconstructed game bytes
and 392,216 restored-library bytes), or **53.897703%**. The comparison registry
contains **136 units**. All **224 local tests** pass with Unicorn enabled and
no skips. These totals are a verification record for this batch;
the README continues to link to the live report.

## Follow-up work

`CMemoryCardGC::Init` / `IssueStart`, `CMemoryCard::ScanVisibly`, popup stack/draw
handling and the remaining wide-text implementations can use these interfaces.
These functions are now reconstructed in the [save-popup follow-up](save-popups.md).
The new message tests also provide a base for combining real popup and card
helpers in more integrated tests.

A `GenerateScratchpadCRC` draft was explored but is not registered in this
batch. It shares the allocation-label pool with `ScratchpadDataOkay`, and two
independent partial comparisons cannot claim the same native storage. Its
remaining instruction-scheduling differences prevented exact promotion in the
bounded matching pass. No duplicate storage or code credit is claimed.
