# FEng packages, objects and localization

This batch builds on kyleckroeger's frontend interfaces and reconstructed callees
from [PR #64](https://github.com/lifewillbeokay/moh-rising-sun/pull/64) and
[PR #65](https://github.com/lifewillbeokay/moh-rising-sun/pull/65), plus the
[animation and list-box follow-up](feng-animation-lists.md). Thank you for making
the object, package, list-box and game-interface layouts available for this work.
Reusing those shared views lets the new bodies connect to already reconstructed
code instead of introducing separate layout guesses.

The new bodies were reconstructed with AI assistance from the pinned GR8E69
instructions, symbols and data. No additional Frontline or other game's source
was copied. Ghidra 12.1.4 exports were checked against the original bytes and
function boundaries; inferred casts, member names and floating comparisons were
treated as leads. Bulk exports, original bytes and compiled objects stay ignored.

## Scope and accounting

The batch covers **39 functions / 11,012 original code bytes**:

- Little-endian child lookup, type sizes, referenced libraries, message-target
  lists, list-box and code-list-box tags, and list-box resource binding.
- Object, group and script copying, track values and move-to initialization,
  position changes, track updates and script event routing.
- String comparisons/conversion, decimal helpers, locale line reading and event
  array resizing.
- Engine rendering, object depth collection, release, library unloading and
  package markers; type lookup and field/default management.

Four functions totaling **468 bytes** are promoted to the exact build:
`FEStricmp`, private `FECopyString`, `FETypeLib::FindType` and
`FEngine::UnloadLibraryPackage`. Four more totaling **340 bytes** are exact but
remain unlinked in comparison groups: `WideToCharString`,
`FEFieldNode::SetDefault`, `FETypeNode::UpdateOffsets` and `FETypeNode::GetField`.
The remaining **31 functions / 10,204 bytes** are partial. Original bytes remain
in the verified image for all comparison functions, including exact-unlinked
ones. Similarity is separate from exact or linked credit.

All units use the existing FEng ProDG 3.8.1 profile
(`-O2 -G0 -fno-exceptions -fno-implicit-templates`). Comparisons inherit
`fecode_setcellcolor.json` and guard unfinished bodies with `DECOMP_COMPARE`.
Generated data and dependencies are checked completely; no instruction patches,
function-specific flags, omitted differences or clipped constant pools are used.
Private functions retain their original local binding and file scope. Fragments
do not claim to recover complete original translation units.

## Layout and behavioral evidence

`FEEventList` replaces the descriptive script-event view at `FEScript+0x24`.
Its symbol-backed assignment and allocation routines establish a count/pointer
pair and twelve-byte entries. `IssueScriptMessages` reads message, target and
time from those entries. The two existing array units now share `FEEvent.h`.

`FETypeNode.h` models the observed field/type nodes. `FEFieldNode` has key type,
size, offset and default-value storage at `+0x14` through `+0x20`;
`FETypeNode` holds a field list at `+0x14` and type word at `+0x24`. Node names,
class tags and method signatures are symbol evidence; member names are inferred.
`FEKeyTypeSize` stays external. Allocation labels place both strings in the
complete type-field constant pool at `0x8029888c`.

The package word at `+0x24`, already named `priority`, also holds the use count
for library packages. `libraryUseCount` is an inferred union alias for that same
storage. The separate control mask remains at `+0x28`. `FETypeLib::Startup`
returns an integer in the original; its constructor caller ignores that result.

Several behaviors are deliberately preserved:

- `FindChild` reads the header at the end of the parent body. A matching sentinel
  can be returned. The response branch scanner can likewise read the entry at
  `responses[count]`. These are not newly bounded readers.
- Locale line reading sign-extends ordinary bytes into shorts, accepts decimal
  backslash escapes, ignores non-digits inside escapes, skips CR/LF runs and can
  fill the last line while returning zero. Repeated backslashes retain the
  original one-byte advancement. `ItoA(0)` produces an empty string; its power
  loop is not made safe for values that cause repeated unsigned overflow.
- The zero-count event resize uses scalar delete, while replacement releases the
  previous array with vector delete. Other constructors preserve fields left
  uninitialized by the original.
- Object copying clones responses and scripts, then rebinds current/next script
  pointers by hash. Reference script copies preserve the base key's destination
  virtual table and reference the source key list.
- Move-to integer tracks subtract integer words, without converting a float.
  Quaternion tracks multiply the current value by the conjugated base; color
  tracks use the existing color subtraction and assignment interfaces.
- Track updates use the low byte of the count. The first color/visibility track
  may be evaluated before and again inside the visible-object loop. Dirty flags
  preserve the bits selected by the original `uHoldDirtyFlags` symbol.
- Script events keep the target sentinels, half-open time window with duration
  adjustment, repeated GUID lookup on the current-button branch, and the check
  of the sender on the general queued-message branch.
- Rendering sets the object's context before the depth check, and admits NaN
  depth through the original unordered branch. There is no added sorter capacity
  check. Release drains the same lists as the original; it does not drain the
  command queue. Library unload keeps callback and shutdown ordering.

## Validation and limits

`tests/test_feng_packages.py` adds **31 test groups / 822 scenarios** comparing
original and native PowerPC bodies using optional Unicorn 2.1.4. They cover every
new function, with representative branches for missing chunks/libraries,
callback refusal and load failure, owned package buffers, empty/growing/shrinking
arrays, every implemented list tag, nested response conditions, reference/deep
script copies, script-link rebinding, resource indices, all move-to kinds, dirty
flags, event targets/times, render traversal and teardown.

Integer returns, memory and callback order compare exactly. Floating values use
the shared fixture's six-decimal normalization. External allocation, selected
callees and game callbacks are stubbed; test coverage is not a proof of semantic
equivalence. Arbitrary aliasing, malformed files, allocation failures, arbitrary
callback mutations and every constructor response-copy combination are not
covered. These checks do not run the game. Runtime remains untested.

Original-object baseline, complete source-image verification, native comparison
measurements and tests are separate checks. Public CI validates the locally
verified snapshot and exports the report; it does not rebuild the game with the
proprietary compiler. Original-dependent tests skip without the pinned original
and optional emulator.

Local validation passed the original-object baseline (2,860,576-byte normalized
DOL), the complete source rebuild of all **2,602 accepted units**, all **96
comparison units**, and **160 tests** with Unicorn available and no skipped tests.
The reconstructed image is byte-identical. Linked source is **1,339,836 bytes**
(7,047 functions); reconstructed game code is 947,620 bytes and restored library
code remains 392,216 bytes. Including the 488 exact-unlinked bytes across all
comparisons, exact source totals **1,340,324 / 2,492,032 bytes (53.784382%)**.
Snapshot capture recompiles the comparisons and records the verified inputs.

## Follow-up

The larger script-tag reader, object update, package push, locale-table reader,
button navigation and type-library construction are now maintained in the
[input/lifecycle follow-up](feng-lifecycle.md). They received no source credit
in this earlier batch. The four exact-unlinked functions above are also
candidates for independent promotion with full data/ownership verification.

## Recorded comparison measurements

Objdiff 3.8.3 results for the complete compared functions, weighted by original
function size within each group. The byte-difference totals are a separate native
byte comparison and include size changes; similarity is not an equivalence test.

| Source fragment | Functions | Original bytes | Similarity | Differing bytes |
| --- | ---: | ---: | ---: | ---: |
| `fe_wide_length` | 1 | 52 | 76.85% | 22 |
| `fecolor_subtract` | 1 | 120 | 95.17% | 45 |
| `feevent_setcount` | 1 | 216 | 89.85% | 117 |
| `fegroup_copy` | 1 | 188 | 89.19% | 34 |
| `felocale_compare` | 3 | 440 | 41.14% | 222 |
| `felocale_numbers` | 2 | 200 | 73.90% | 100 |
| `felocale_readstring` | 1 | 288 | 82.62% | 186 |
| `fengine_record_marker` | 1 | 80 | 79.50% | 7 |
| `fengine_release` | 1 | 420 | 67.67% | 351 |
| `fengine_render` | 1 | 332 | 91.86% | 270 |
| `fengine_render_object` | 1 | 252 | 74.60% | 174 |
| `feobject_close` | 1 | 40 | 64.00% | 38 |
| `feobject_constructors` | 2 | 740 | 80.63% | 530 |
| `feobject_moveto` | 1 | 856 | 54.04% | 693 |
| `feobject_position` | 1 | 304 | 68.78% | 240 |
| `feobject_track_value` | 1 | 340 | 80.29% | 167 |
| `fepackage_script_messages` | 1 | 516 | 75.39% | 320 |
| `fepackage_tracks` | 1 | 484 | 36.00% | 342 |
| `fereader_findchild` | 1 | 216 | 83.52% | 66 |
| `fereader_list_tags` | 2 | 1,952 | 50.70% | 1,432 |
| `fereader_message_targets` | 1 | 516 | 78.39% | 382 |
| `fereader_readtypes` | 1 | 124 | 90.00% | 68 |
| `fereader_referenced_packages` | 1 | 444 | 81.37% | 316 |
| `feresource_listbox` | 1 | 316 | 56.11% | 267 |
| `feresponse_branch_target` | 1 | 144 | 35.47% | 103 |
| `fescript_copy` | 1 | 556 | 59.46% | 339 |
| `fetypenode_fields` | 4 | 408 | 99.70% | 9 |
