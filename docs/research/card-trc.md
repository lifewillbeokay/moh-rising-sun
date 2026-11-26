# Realmc TRC load, save and recovery state machines

This AI-assisted continuation of [card-tasks.md](card-tasks.md) reconstructs
**seven new functions / 13,724 original code bytes**. The three earlier TRC
queries are consolidated with them into one ten-function comparison covering
15,628 original bytes. All ten remain partial source. No additional exact or
linked coverage is claimed, and the earlier functions earn no second credit.

| New handler | Original address | Original bytes | Native bytes | Objdiff similarity |
| --- | --- | ---: | ---: | ---: |
| `UpdateTaskTrcStartGame` | `0x8020f9f4` | 948 | 884 | 79.85% |
| `UpdateTaskTrcLoadFile` | `0x802101ac` | 3,124 | 2,848 | 73.01% |
| `UpdateTaskTrcSaveFile` | `0x80210de0` | 2,820 | 2,568 | 60.99% |
| `UpdateTaskTrcDeleteFile` | `0x802118e4` | 1,752 | 1,544 | 66.18% |
| `UpdateTaskTrcMountCard` | `0x80212328` | 1,668 | 1,536 | 73.59% |
| `UpdateTaskTrcMountCardOperation` | `0x802129ac` | 1,524 | 1,304 | 62.87% |
| `UpdateTaskTrcCheckSpace` | `0x80212fa0` | 1,888 | 1,712 | 67.23% |

## Evidence and ownership

The target is the pinned GR8E69 Disc 1 `MOH3RDVD.ELF`, SHA-256
`fc9aeb766e79f58d2db8e6bd205c76a385952eab7e532c2f13809d3371283073`.
The source follows its symbols and instructions, local Ghidra exports and the
existing Realmc reconstruction. No external decompilation source was imported.
Original exports, diagnostic objects and compiler experiments remain ignored.

`gc_trc_updates.cpp` accounts for every function in the contiguous original
`gc_interface.cpp` interval `0x8020f9f4–0x80213700`. The previously separate
CardExists, GetCardInfo and ListFiles comparisons are removed from the registry
and maintained in this unit. Its compiler profile is inherited from
`gc_interface_thread.json`: ProDG 3.8.1 with
`-O2 -G8 -fno-exceptions -fno-implicit-templates -fno-strength-reduce`.
This is a working profile, not proof of the historical compiler release.

The compiler emits a complete 36-byte localization signature pool, verified at
`0x802bf5d4`: `s`, `ss`, `sss`, `ssss`, `ssdd` and `ssddsdd`, including native
padding. The original TRC instructions reference this pool. Consolidation gives
it one explicit owner; identical strings in other original fragments do not
justify borrowing their storage. No generated code or data is clipped,
discarded or patched. Data earns no code credit.

## Scoped interfaces and behavior

`RealmcTrcUpdate.h` describes the fields and virtual slots these functions use.
Names of members and convenience helpers are inferred. These views are local
to this reconstruction; they do not establish complete original classes or
replace independently verified interfaces in other source fragments.

The shared task prefix retains the state and substate at `+8/+12`, card ID at
`+0x14`, result at `+0x18`, and notification flag at `+0x1c`. Mounted and open
flags occupy `+0x24/+0x28`. File tasks copy the full 48-byte information record
at `+0x2c`, extending the driver's previously recovered 36-byte prefix. The
task-manager vptr and adjusted StartTask/EndTask calls retain their earlier
evidence. Unknown fields are preserved rather than initialized speculatively.

The load/save/delete handlers coordinate child mounting, user prompts, file
opening, asynchronous transfer messages, retry/cancel paths and completion.
The save path preserves temporary-file naming, deletion and rename order.
The mount operation preserves both-slot scanning, repeated probes, distinct
wrong-device and I/O errors, formatting prompts, and the difference between
scheduling completion and completing during the current update.

Space checking preserves file enumeration, notification copies, accumulated
reclaimable blocks, saturating required-space subtraction, temporary-file
capacity, and all three insufficient-space prompt forms. Message arguments,
message data and the destination virtual pointer are distinct storage.

Several unintuitive original behaviors are deliberate:

- Load completion continues to unmount, send its completion message and end
  the task even when closing the file selects an error state.
- A card-removal result during the capacity check does not stop the remaining
  calculation; sufficient capacity can overwrite that result with success.
- The scan records a wrong-device flag even if a later probe finds a card.
- Some prompt-option calls use a fixed task ID instead of the current task ID.
- The message timer expires only when unsigned elapsed time is strictly greater
  than duration; equality still waits.

The full source is guarded by `DECOMP_COMPARE` and excluded from the verified
image. Remaining native differences include inlining, store/instruction
scheduling, switch layout and register allocation. Similarity alone does not
establish equivalent behavior or justify linking these functions.

## Validation

`tests/test_card_trc.py` adds seven groups with **26,216 original/native PowerPC
case pairs** for the new handlers. It covers the recovered state/substate branches, result
codes, UI choices, timer boundaries, mount/format errors, transfer failures,
temporary-file handling, free-space boundaries and virtual `this` adjustments.
It checks all ten task records, complete messages, timer/flag storage and
external call traces, including message contents at service boundaries.
The earlier CardExists/GetCardInfo/ListFiles tests now execute the consolidated
native unit as well.

Card hardware, localization, task scheduling and other services are stubbed.
These are isolated state-machine checks, not an emulator boot, physical card
test, gameplay validation or proof for arbitrary inputs.

Verification commands:

```sh
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 tools/comparisons.py
scratch/script-pool/venv/bin/python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
python3 tools/progress_report.py
```

The original-object baseline and complete source build reproduce the
2,860,576-byte derived DOL, SHA-1
`6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. All **2,628 accepted units** pass;
linked coverage remains **1,346,180 bytes / 54.019371%**, comprising 953,964
reconstructed-game and 392,216 restored-library bytes. The comparison registry
now has **160 units**, reflecting the consolidation of three entries into one.

All **275 local tests** pass with Unicorn enabled and no skips. The complete
discovered suite ran once across eight disjoint groups with four worker
processes: the existing 268 tests and the seven new state-machine groups.
The serial discovery command above runs the same tests.

The local build encountered an assembler timeout in the unchanged
`thrown_bullet_penetrate` unit after 1,701 completed units. Those outputs were
retained only after checking all 6,215 frozen source/configuration/tool hashes
and their copied source files. The resumed run revalidated the completed native
objects, compiled the remaining units, and ran the normal complete-image and
load-layout checks. No verifier or native output was patched. The standard
commands above remain the reproduction path.

## Follow-up

Driver OpenFile, FindFirst/FindNext, the interface Find helper and localization
remain connected targets. Find and FindNext own local static records: their
complete data/BSS ownership must be established before registration. The new
state-machine fixtures provide callers and observable contracts for those
helpers, while exact matching of the maintained TRC source remains independent
work.
