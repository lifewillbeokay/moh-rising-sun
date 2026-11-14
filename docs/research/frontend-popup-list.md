# Frontend popup, settings and rendering follow-up

This work reuses the handler, settings, input and FEng interfaces contributed by
kyleckroeger in [PR #64](https://github.com/lifewillbeokay/moh-rising-sun/pull/64)
and [PR #65](https://github.com/lifewillbeokay/moh-rising-sun/pull/65), and the
[screen-event follow-up](frontend-screens.md). Thank you for recovering those
shared interfaces: they support larger callers and make independent behavior
checks practical.

The source was reconstructed with AI assistance from the pinned GR8E69 symbols,
instructions and data. Ghidra exports were checked against complete original
function boundaries and loaded bytes. No implementation from another game was
copied. Original binaries, bulk exports and unsuccessful experiments remain
under ignored directories.

## Scope and accounting

The batch adds **24 functions / 5,608 original executable bytes**. Nine functions
in five fragments reproduce **1,096 exact code bytes**:

| Fragment | Functions | Code bytes |
| --- | ---: | ---: |
| `savepopup_background` | Enable, disable and query the background | 44 |
| `savepopup_lifecycle` | `CSaveGamePopup` constructor and destructor | 556 |
| `savepopup_update` | `CSaveGamePopup::Update` | 132 |
| `savepopup_events` | UI and sound event forwarding | 112 |
| `favorites_remove_duplicates` | `HandlerOnlineFavorites::RemoveDuplicates` | 252 |

The other **15 functions / 4,512 bytes** are maintained partial source. The
comparison registry retains their complete native outputs; the verified image
continues to use original code at their ranges.

| Comparison fragment | Functions | Target bytes | Objdiff similarity |
| --- | ---: | ---: | --- |
| `settings_button_config` | 1 | 368 | 86.858696% |
| `settings_restore_slot` | 1 | 388 | 94.010310% |
| `settings_matrix_delta` | 1 | 280 | 96.285710% |
| `fenginterface_notifications` | 2 | 320 | 93.075000% each |
| `shell_render_contexts` | 5 | 1,172 | 90.333336–99.990000% |
| `shell_joypad_mask` | 1 | 292 | 76.438354% |
| `bonusoptions_menu` | 1 | 944 | 69.728810% |
| `memorycard_selections` | 2 | 568 | 90.778760%, 99.990000% |
| `slider_value` | 1 | 180 | 63.200000% |

Similarity is an instruction-comparison measure, not exact credit or evidence of
complete semantic equivalence. Native sizes, addresses and relocation bytes are
also checked independently. For example, the render-context and hide-selection
routines still differ even where objdiff sees equivalent instruction structure.

All fragments use the existing ProDG 3.8.1 `-O2 -G0 -fno-exceptions
-fno-implicit-templates` profile. Container callers inherit the matching
`fenginterface_files.json` include profile; other comparisons inherit
`fecode_setcellcolor.json`. Partial bodies are guarded by `DECOMP_COMPARE`.
No per-function optimization flags, patched instructions or clipped generated
data are introduced.

## Layout and behavior evidence

- **Popup ownership:** the constructor allocates an FEngine of `0x5270` bytes,
  a CFengInterface of `0x18a0`, and a CSprite of `0x24`. The earlier interface
  declaration ended at `0x1884`; the added `0x1c` bytes remain explicitly unknown.
  This establishes the allocation extent, not the purpose of the tail. The
  constructor and destructor now match in full, including these allocation sizes.
- **Popup storage:** `+4` is the engine, `+8` its CFengInterface, `+0xc` the
  package, and `+0x10` the sprite. The word at `+0x14` is cleared but its meaning
  remains unknown. Shared assets load only for the first instance and are freed
  when the final instance is destroyed. Deletion order is interface, engine,
  shared assets when appropriate, then sprite. UI and sound entrypoints forward
  only the event and object to the corresponding virtual callbacks.
- **Popup ticking:** controller checks occur only when both `g_GameFlow`'s first
  word and `levelEnding` are zero. The engine receives the integer conversion of
  `delta * 20.0f`, including truncation toward zero.
- **Favorite ownership:** duplicate comparison uses field zero, or `s_xsNull`
  when there are no fields. Removal precedes the explicit row destructor, and the
  inner index backs up so consecutive duplicates are examined. Null rows are
  skipped. `ShellListBox.h` reuses the already accepted ListBox/vector storage;
  descriptive member names do not claim recovery of the historical declaration.
- **Save restoration:** the destination core pointer is cached before the first
  `GetProfile` call. The requested slot is copied to `levelStartData`, then to
  that cached core's save data. The subsequent stats copy rereads `profile`.
  Tests preserve this distinction even when the profile callback replaces it.
- **Matrix serialization:** a whole-matrix equality flag precedes per-element
  change flags. Only changed elements are serialized. Reading unchanged elements
  does not copy defaults into the destination. Float equality makes a NaN differ
  from itself, while positive and negative zero compare equal.
- **Notifications:** CFengInterface walks its existing STLport tree in iterator
  order, caching the end iterator. UI and sound callbacks use their distinct
  virtual slots and pass message, mask, object and package in that order.
- **Rendering:** shell transforms retain the observed scale, quaternion/identity
  translation, translation and optional parent multiplication call order. Color
  conversion treats channels as signed integers, scales by `1/255`, and multiplies
  by the parent color even for parent index zero. The draw list traverses entries
  backwards; render dispatch distinguishes supported types from diagnostics.
  Transform callees remain external in isolated tests.
- **Input and sliders:** shell stick thresholds are strict comparisons with
  `-63` and `63`; the first active digital entry wins, followed by X then Y stick
  priority. A blocked or unready controller leaves previous stick values alone.
  `CSlider::SetValue` stores a float at `+0x1c`, computes the base vector plus
  the scaled vector at `+0x10`, sets track 2 at time `-1`, and marks the object
  changed. The named aliases preserve the earlier storage and allocation sizes.
- **Selection caching:** memory-card option hashes, input-enable/disable hashes
  and the single-choice label hash use their original symbol-backed caches and
  guards. All four option hashes are computed before their initial array stores;
  only selectable choices are queued. The special label message requires option
  zero to be the sole selection. Tests exercise cold and warm caches.
- **Bonus menu:** unlocked items are enabled and shown, locked items hidden and
  disabled. The selected button is the lowest available item encountered by the
  reverse walk. Empty menus select the fallback entry; toggle events search the
  twelve stored text hashes. Co-op and shell state select the profile-name path.

## Generated data and original context

Every retained data section is compared in full. Its placement follows the
original instructions and source-file scope, rather than choosing an arbitrary
copy of the same bytes elsewhere in the executable.

| Fragment | Complete generated `.rodata` |
| --- | --- |
| Popup lifecycle | `0x802b636c`, 144 bytes |
| Popup update | `0x802b63fc`, 4 bytes |
| Shell render contexts and dispatch | `0x802adf58`, 312 bytes |
| Bonus menu plus constructor context | `0x802b3d1c`, 140 bytes |
| Memory-card selections | `0x802b68e4`, 112 bytes |

The bonus event handler was previously deferred because its strings share the
preceding constructor's pool. The comparison now emits that already accepted
116-byte constructor as explicit `handler_bonusoptions_ctor` context, retaining
its original address and exact bytes. Its code and previously owned data receive
no second credit. The native pool includes all strings and padding; reordering
source branches preserves behavior while reproducing the original pool order.

The five adjacent shell rendering functions similarly retain their complete
shared pool. Emitting the dispatcher with the context functions accounts for the
string bytes immediately following the color conversion constant. No padding or
strings are dropped to make a fragment pass.

File-local popup globals and memory-card caches resolve through their actual
`SaveGameCore.cpp` and `MemoryCardPopup.cpp` symbol records. `asm` names only bind
C++ declarations to those existing symbols; there are no assembly bodies or
invented symbols for anonymous strings.

The ListBox display/update drafts and `SPlayerProfile::TotalReset` were also
investigated, but their native pools still require earlier literal/alignment
context. ListBox Update references `Hide` at `0x802b067c` and the integer-to-float
double at `0x802b06b0`; the standalone native pool has different alignment.
SetRowScript/CountRows reuse that earlier field-name format and ellipsis. Profile
reset reuses an earlier `DEFAULT` literal while its float constants begin at
`0x802ac458`. They remain unregistered scratch experiments. No progress is claimed
for them. Useful next work is to recover that surrounding source context or
continue the popup draw/wait path using the now-verified ownership layout.

## Verification and limits

Reproduction commands, run from the repository root:

```sh
python3 tools/baseline.py
python3 tools/reconstruct.py
python3 tools/comparisons.py
python3 -m unittest discover -s tests -v
python3 tools/progress_report.py --capture
python3 tools/progress_report.py
```

`tests/test_frontend_popup_list.py` adds sixteen original/native PPC test groups
using optional Unicorn across 585 original/native case pairs. They cover all 24
new functions, including multiple
live popup instances, adjusted virtual callbacks, first/repeated cache use,
controller edge thresholds, missing menu objects, repeated duplicate rows,
serialization input/output and aliasing of parent render contexts. Existing
STLport, FEng and image-verification tests remain applicable.

These checks stub external allocation, rendering, storage and other game
services. They are not emulator/gameplay tests and do not establish complete
runtime equivalence for the partial sources. Original-object baseline success,
exact source integration and isolated behavior tests are separate results.
CI validates the locally verified snapshot and exports the report; it does not
rebuild the game.

Local verification for this batch passed independently:

- Original-object baseline: complete 2,860,576-byte analysis DOL, SHA-1
  `6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`.
- Complete source build: 2,612 accepted units, the same complete-image identity,
  and 1,342,560 linked code bytes (53.874108%). Reconstructed game code is 950,344
  bytes; restored library code remains 392,216 bytes.
- Comparison registry: 132 units, including the nine new partial fragments.
- Full unittest suite with Unicorn installed: 208 tests passed with no skips,
  including the 585 original/native case pairs added here.
