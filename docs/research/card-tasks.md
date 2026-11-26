# Realmc card tasks and buffered I/O

This AI-assisted batch reconstructs **35 functions / 7,536 original code bytes**
underneath the save-screen work in [save-popups.md](save-popups.md). Fifteen
functions / 1,088 bytes are exact source; twenty functions / 6,448 bytes remain
registered partials. Partial source is compiled with `DECOMP_COMPARE`, measured
as complete functions and excluded from the verified image.

| Area | Functions | Original bytes | Status |
| --- | ---: | ---: | --- |
| Ordinary card/file task updates | 13 | 3,696 | Close, Seek and SetAttribute exact; ten partial |
| TRC card existence, card information and file listing | 3 | 1,904 | Partial |
| Public TRC card-query entry points | 2 | 400 | Partial |
| Buffered WriteFile, sports BIOS creation and header checksum | 3 | 960 | Partial |
| Allocation callback setters and dispatch | 8 | 264 | Exact |
| Block calculator Init, Clear, SetFileInfo and GetResult | 4 | 192 | Exact |
| Task-manager EndTask and retail singleton assertion | 2 | 120 | Partial |

## Evidence and ownership

The target is the pinned GR8E69 Disc 1 `MOH3RDVD.ELF`, SHA-256
`fc9aeb766e79f58d2db8e6bd205c76a385952eab7e532c2f13809d3371283073`.
Existing symbols, original instructions, local Ghidra exports and the accepted
Realmc fragments establish these reconstructions. No external decomp source was
imported for this batch. Original exports and compiler experiments remain ignored.

- The ordinary update interval is `0x80213700–0x80214570` in `gc_interface.cpp`.
  The three TRC entries begin at `0x8020fda8`, `0x8020ff74` and `0x80211fbc`.
- The public query pair occupies `0x8020eba0–0x8020ed30` in `public.cpp`.
- Driver WriteFile starts at `0x80215454`; sports BIOS creation and checksum
  verification occupy `0x802163b0–0x8021659c` in `gc_driver.cpp`.
- The allocation wrappers occupy `0x802166d4–0x802167dc` in `memvectors.cpp`.
  The four original private callback slots remain externally owned, with scoped
  local-external symbol evidence and small-data relocations in the manifest.
- Block-calculator methods cover `0x802165e0–0x80216698` and
  `0x802166cc–0x802166d4`. The native 48-byte derived vtable at `0x802eda38` is
  verified in full and owned by the accepted calculator unit. Data earns no
  code credit. Constructor and destructor bodies remain original context.
- Task-manager EndTask and TrcSingletonAssert begin at `0x802952dc` and
  `0x802953d8`. As with the existing late StartTask fragment, these symbols do
  not establish an original FILE boundary; no FILE owner is invented.

The shared `gc_interface_thread.json` ProDG 3.8.1 profile is used for the library
updates/helpers: `-O2 -G8 -fno-exceptions -fno-implicit-templates
-fno-strength-reduce`. Public query and late task-manager comparisons inherit the
existing `mc_public_calls.json` and `mc_task_manager_start.json` G0 profiles.
These are working profiles, not proof of the historical compiler release.
There are no function-specific flags, native instruction patches or discarded
function bodies. Remaining differences include register allocation, store and
instruction scheduling, switch layout, and inlining.

## Behavior and scoped views

`RealmcTaskUpdate.h` models only the fields and virtual slots used by these
updates. Its views are separate from the existing public/interface fragments;
they are not complete allocation layouts or recovered original member names.
The task-manager vptr is at `+0x4c`, with StartTask and EndTask at virtual slots
8 and 16. Task state/substate are at `+8/+12`, the card ID at `+0x14`, result at
`+0x18`, and notification enable at `+0x1c`.

The ordinary handlers preserve driver-result translations, payload offsets,
notification gating and the order of EndTask relative to message creation.
Read/write failures set sticky flags without clearing earlier failures on
success. File enumeration maintains its first/next/end states and increments
the original count even when individual notifications are disabled.

Message assignment preserves the destination vptr at `+0x90`. The original
compiler copies the full record and restores that pointer; treating it as an
ordinary data member changed observable storage in the first runtime probes.
The scoped Message/GCMessage inheritance now expresses the original assignment
behavior without a custom memory patch or invented copy boundary.

TRC updates suspend their parent for the shared mount task, retain the separate
mounted/result fields and unmount before completion where the original does.
The mount task's options and parent ID are at `+0x30/+0x34`; its start path
clears `+0x2c`, `+0x38` and `+0x3c` while preserving other unknown fields.
File-list message timing uses the original strict **unsigned** duration-versus-
elapsed comparison. Equality still waits; expiry clears the active flag.

Buffered writes retain partial-sector state, busy retries, error exits and the
sports BIOS dispatch. The original read-ahead branch compares total capacity
**less than** the updated offset; that unusual direction is preserved. Its
MEM_clear call passes the sector size in r4, which the initial decompiler output
omitted. The checksum adds **signed** bytes and reads the trailing checksum
separately. The sports BIOS creation wrapper stores its own negative result
mapping while returning the raw card result.

The calculator clears block/file counts, accumulates driver-reported blocks and
preserves virtual `this` adjustments. The task-manager header factors out the
same scoped declarations previously embedded in the accepted StartTask source;
that earlier function must still match in the complete build. EndTask restores
the interrupted-task stack and adjusts the public-TRC count. The retail singleton
assertion has no ABI-visible side effect: its original null comparison updates a
volatile condition register, while the maintained empty partial emits only a
return. It is not counted as exact source.

## Validation

`tests/test_card_tasks.py` adds **25 groups / 1,300 original-native case pairs**:

- Ordinary-operation result codes, enabled/disabled notifications, message
  contents, callback ordering, sticky failure flags and virtual adjustments.
- First/next/end enumeration, mount completion and failure, card queries, list
  timer boundaries including unsigned high values and wrap-shaped inputs.
- Both public query paths, allocation callback arguments, calculator state,
  task-stack restoration, and the retail assertion's memory behavior.
- Buffered writes spanning sectors, partial/full starting buffers, negative
  offsets, busy retries, output-count pointers, I/O errors and sports BIOS paths;
  signed-byte checksum success, mismatch, empty headers and read failures.

The optional Unicorn fixture stubs card hardware, threading, allocation and
other external services. It compares memory, return values where defined, and
call traces for the selected cases. This is not an emulator boot, real memory
card test, gameplay validation or a proof for all possible inputs.

Verification commands:

```sh
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 tools/comparisons.py
scratch/script-pool/venv/bin/python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
python3 tools/progress_report.py
```

The original-object baseline and complete source build both reproduce the
2,860,576-byte derived DOL, SHA-1
`6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. The build verifies **2,628 accepted
units**, **7,097 linked functions** and **1,346,180 linked code bytes** (953,964
reconstructed-game and 392,216 restored-library bytes), or **54.019371%**.
The registry contains **162 comparison units**. All **268 local tests** pass
with Unicorn enabled and no skips. These figures record this batch's complete
verification; the current public snapshot remains the source for live totals.

The public comparison receipts record complete-function sizes, hashes, differing
bytes and objdiff scores. Similarity is not substituted for exact linked coverage.
The README continues to link to live progress rather than embedding a percentage.

## Follow-up

The next connected targets are driver OpenFile/FindFirst/FindNext, interface Find,
and the remaining localization helpers. The larger TRC delete/load/save/mount
state machines are now maintained in the [TRC continuation](card-trc.md), which
also consolidates this batch's three TRC query comparisons. Interface Find and
FindNext own local static records; they need complete data/BSS ownership rather
than borrowing partial storage or clipping generated output. The current tests
provide driver-result, task-completion and message fixtures for that work.
