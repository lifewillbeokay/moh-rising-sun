# Save popups and card-start follow-up

This AI-assisted GR8E69 batch extends the [save-flow work](save-flow.md) with
**21 functions / 7,592 target code bytes**: **13 exact functions / 1,944 bytes**
and **8 measured partials / 5,648 bytes**. Existing popup lifecycle and sound
functions are consolidated without receiving duplicate credit. Source was
reconstructed from the pinned GameCube executable, symbols and call sites;
no new Frontline code is imported.

The working profile is ProDG 3.8.1, `-O2 -G0 -fno-exceptions
-fno-implicit-templates`. Container users retain the existing FEng/STLport include
profile. This establishes a reproducible build, not the original compiler release.

## Source inventory

| Source family | New functions | Target bytes | Status |
| --- | ---: | ---: | --- |
| Base popup default callbacks | 3 | 12 | Exact |
| Popup Push / Pop | 2 | 220 | Exact |
| Memory-card popup OnUIEvent / OnPush | 2 | 492 | Exact |
| Keyboard popup CreateName | 1 | 192 | Exact |
| Language-screen static cleanup / Instance | 2 | 136 | Exact |
| GameCube card Init / destructor | 2 | 372 | Exact |
| Visible card scan | 1 | 520 | Exact |
| Popup SetString | 1 | 272 | Partial |
| Memory-card popup Display | 1 | 340 | Partial |
| Keyboard popup OnUIEvent | 1 | 2,132 | Partial |
| Language SelectLanguage / OnUIEvent | 2 | 772 | Partial |
| Popup Draw / Wait | 2 | 1,844 | Partial |
| GameCube card IssueStart | 1 | 288 | Partial |

Each manifest records original function names, addresses, whole sizes and private
file scopes. Partial bodies remain behind `DECOMP_COMPARE`; the verified image
continues to use original code for those ranges.

The first complete measurement reports these native instruction similarities:

| Partial function | Original / native bytes | Objdiff similarity |
| --- | ---: | ---: |
| SetString | 272 / 268 | 52.6912% |
| Display | 340 / 316 | 77.0824% |
| Keyboard OnUIEvent | 2,132 / 2,128 | 97.6961% |
| SelectLanguage | 140 / 136 | 81.5143% |
| Language OnUIEvent | 632 / 652 | 93.3861% |
| Draw | 1,128 / 896 | 64.4716% |
| Wait | 716 / 696 | 87.2123% |
| IssueStart | 288 / 280 | 86.6667% |

These are measurements of complete native functions, not correctness scores.

## Data ownership and static initialization

`CMemoryCardPopup::OnPush` reuses the constructor's empty string at `0x802b6854`.
The constructor/class fragment, new UI handler, existing sound handler and new
OnPush are therefore compiled as one exact `memcardpopup_class` unit. Its text is
`0x801a3de8..0x801a42e0`; its complete 212-byte read-only pool begins at
`0x802b6810`. The old `memcardpopup_events` unit is removed. All six previously
accepted functions retain their exact bytes and addresses.

The UI handler's dynamic static array owns 16 bytes at `0x803f502c`, followed by
the four-byte guard at `0x803f503c`. The ordinary C++ static initializer reproduces
the original stack temporary, aggregate copy and guard update. The verifier now
normalizes only the final compilation-local numeric suffix of private SN guard
names such as `_.tmp_0.3409` / `_.tmp_0.241`. Guard index, private binding, size,
address and original FILE record remain checked. Public name tests and native
ELF mutations check that changing those ownership properties still fails.

Other generated read-only pools are checked in full: 48 bytes for Draw at
`0x802b6400`, 24 for Wait at `0x802b6430`, 8 for CreateName at `0x802b6aa8`,
420 for keyboard events at `0x802b6ab0`, and 76 for language events at
`0x802b6d48`. No literal pool is clipped or replaced by an invented symbol.
Original named globals, language-screen storage, card comments, icons, banners
and virtual tables remain context unless explicitly owned by a manifest.

## Behavior and scoped interfaces

- Popup Push resets its frame counter and calls OnPush after inserting at the
  front of the original STL list. Pop removes the front. Wait updates the top
  popup and draws from bottom to top, brackets drawing with the original draw
  context, and runs input, unplugged-controller, sound and synchronization work.
  The reverse traversal captures its end before drawing; a callback inserting a
  new front popup does not extend the current traversal.
  Its frustum transformation uses `g_camera.localToWorld` at `+0x50`.
- Draw preserves the double angle scale, float rounding, both trig calls,
  frame-counter rollover, video-mode placement, texture coordinates and RGBA
  bytes. Its native partial builds the two quads directly; the original also
  copies temporary vertex arrays. The 24-byte vertex fields and primitive value
  5 are observed interfaces, not recovered member or enum names.
- SetString wraps at the most recent ordinary space after 37 characters, then
  converts nonbreaking spaces. It preserves the original 1,024-element stack
  buffer and assumptions about input length. Display distinguishes label-object
  presence from option-text presence and hides selections while not waiting.
- Keyboard events preserve the ordered A–Z, digit and punctuation hash checks,
  empty-name rejection, profile selection, save-name flag and localization calls.
  The shared view now identifies eight characters plus a terminator at `+0x20`;
  the remaining three bytes before `nameStarted` remain unknown. CreateName waits
  for the file-local response and clears the pause menu's `inMenu` field.
- Language selection plays the demo movie after 900 idle frames. The singleton
  retains original named storage and a single registered cleanup. Event handling
  uses the observed button/hash and locale tables. As in the original, the
  selected-language path assumes the event object is one of five buttons; an
  absent match would index beyond the five named language entries. No new bounds
  behavior is invented, and invalid-button behavior is not claimed as validated.
- Card Init constructs the observed 48-byte file-info record, localized comment
  and icon/banner pointers. IssueStart clears and fills the 76-byte start record,
  calls block-calculator slots 8/16/24, disables loader reset and issues the card
  operation. The local start-record and calculator views describe these accesses
  only; unknown fields and broader library semantics remain unresolved.
- ScanVisibly scans available devices, expands the narrow message's percent
  substitutions into 16-bit card names, polls completion and enforces the
  original one-second display interval. Its return is corrected to `void` from
  the earlier provisional declaration. Destructor release order and virtual
  adjustment handling are preserved.

`PCUw` is retained as the original symbol spelling at the 16-bit SetString
boundary. Method-only viewport/screen declarations and added input, frustum,
sound and sprite methods do not establish new object allocations or complete
historical classes.

## Validation and remaining work

`tests/test_save_popups.py` adds **17 groups / 922 original-native case pairs**.
Checks cover cold/warm caches, duplicate event hashes, every letter/digit event,
empty/default names, option and label masks, text wrapping and nonbreaking spaces,
idle-movie thresholds, card startup records, scan selection/timing, virtual `this`
adjustments, list restoration/insertion, quad contents and frame-loop callback ordering.
Three empty callbacks are verified by exact compilation. The optional Unicorn
fixture stubs external game, UI, input, audio and card services. These are isolated
PowerPC checks, not gameplay, emulator boot, real-card validation or proof for
all possible inputs. Draw's vertex bytes compare exactly in the checked cases;
floating callback arguments use the fixture's six-decimal normalization.

The eight partial functions retain differences in instruction scheduling,
register allocation, loop layout or temporary copies. Complete generated data
and all native output are accounted for. The public comparison receipt supplies
reproducible per-function sizes, hashes, differing-byte counts and objdiff scores;
similarity is never counted as linked code.

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
`6abed07aefb9be8cb2cd3c4e0fa53a1fde8db04d`. The source build verifies **2,622
accepted units**, **7,082 linked functions** and **1,345,092 linked code bytes**
(952,876 reconstructed-game and 392,216 restored-library bytes), or **53.975711%**.
The registry has **144 comparison units**. All **243 local tests** pass with
Unicorn enabled and no skips. These totals record this batch's verification;
they are not a replacement for the current public snapshot.

The public README links to live progress rather than embedding a stale percentage.
Further work can connect the new popup bodies to the existing save/load tests or
extend GameCube card allocation/construction. The CRC-generator draft described
in the prior batch still needs joint ownership of its shared label pool; this
batch does not change that limitation.
