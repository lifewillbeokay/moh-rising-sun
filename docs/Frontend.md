# Front-end and game settings

Rising Sun code with no Frontline counterpart. The evidence is the GR8E69 disassembly,
its symbol table and the existing headers. Exact native output is strong evidence for this target;
it does not establish every original type or statement.

## CGameSettings and its saved data

Verified by matching code: `CGameSettings` points at a `SCoreGameSettings` block of 0x321ec
bytes. The constructor compares that with `GameSettingReserve`, an absolute symbol of 0x321f8
in the linked image, and `Init` prints both. The block holds a language word,
four `SPlayerProfile` records of 0xc598 bytes at `+4`, the current `SSaveData` at
`+0x31664`, a separate `SPlayerStats` at `+0x31fb8`, the multiplayer player entries,
death-match statistics and a table mapping players to profile indices at `+0x321e0`.
An `SSaveData` is 0x954 bytes and each profile holds twenty at `+0x6c`. Struct tags come
from method symbols; `SPlayerStats`' four counters are named by the `Set_` methods.

Inferred: other member names describe what the code does with them. `SSaveData`'s
closing fields are called a time stamp only because their widths (16, 4, 6, 5, 6 and 6
bits) fit one. `inCoop` and `inMultiplayer` are two `g_Shell` flags: `UpdateUnplugged` chooses the controller
checks by them, and `CGameSettings` tests them alongside `g_bCoopMultiplayer` and
`g_bInMultiplayerMode`. The array bound of 20 saves is from the
space between `+0x6c` and the next known field.

Runtime remains untested.

Each accepted fragment shares `GameSettingsViews.h`, whose views of the player, shell and
pause objects hold only the members these methods touch.

`SPlayerProfile` keeps one `SPlayerStats` per level at `+0xbafc` and a co-op set at `+0xbf68`,
eleven each, indexed by level minus one; `CalculateCumulativeStats` adds the level's deltas to them,
and it names the two counters at `+4` and `+6` of `SPlayerStats` as completed main and bonus
objectives. `Add_MPPlayerKilled` names the death-match counters and the team scores at `+0x32138`, and
ends the match when a byte at `+0x32021` is reached. `SLevelStatsSummary` is a descriptive name for
the 52-byte value `GetLevelStats` and `GetTotalStatsForLevel` return; the original's name is not in
the symbols. `PerformanceStars` is an original tag, and its values are observed as star counts from
zero to three. `CalculatePerformanceRating` reads its thresholds from level resource 27, records 71
to 79, and defaults its top bonus threshold to the level's bonus objective count.
`Timestamp` is the save time's class, per `Now__9Timestamp`.

Not yet exact: `SetIfCurrentWeaponSelectable` (5 differences), `ActivateCheatNamed_All`,
`ActivateCheatNamed_Unlock` (register allocation only), `PerformSavePointResponse` (one
threaded branch), `Set_currentDifficulty` (two scheduled instructions), `ResetDMStats` (an
unreduced multiply in its loop) and `SCoreGameSettings::TotalReset`, whose loop treats the word
before each player's data as belonging to the entry being set up. The landed
`SMPPlayerSlot` view places that word first because `Respawn` and `SetHealthMax` compile
to that form; the two views have not been reconciled. `SetSkuType` and
`SPlayerProfile::TotalReset` use the strings `game` and `DEFAULT`, which the movie settings
fragment owns, so they wait until the fragments can be combined into one unit.

## FEng library

The [animation and list-box follow-up](research/feng-animation-lists.md) documents
the exact and measured partial bodies built on these interfaces, including
original wrap, resize, callback and floating-point edge cases. The
[package, object and localization batch](research/feng-packages.md) extends those
interfaces to package parsing, object copying, script events and rendering. The
[input, lifecycle and reader follow-up](research/feng-lifecycle.md) adds controller
and mouse processing, engine/object updates, package commands and the remaining
large locale/script readers, with exact and partial accounting kept separate.

Rising Sun's front end runs on EA's FEng library. Source paths in allocation labels name its
files (`source/FEngine/FEWideString.cpp`), and symbols give the classes and methods. Verified by
matching code: `FEMinNode` holds two links and its table at `+8`; unlinking writes `0xabadcafe`
into both links. `FEMinList`, `FERefList`, `FEPackageList`, `FEMsgTargetList`, `FEJoyPad`,
`FEMouse`, `FEColor` and `FEWideString` are as in `FEngLists.h` and `FEngStrings.h` or their
source files. Inferred: member names, and that the fields shown are all a class holds.
`FEMinNode`'s destructor has a weak out-of-line copy in the image, so the real class defines it
inline; these units only call it through the table, so the declaration is enough for them.

`FEngine` keeps three lists at `+0xf0`, `+0x100` and `+0x110`, a stack of sixteen package-name
markers and a ring of thirty-two remembered package buttons. `PackageHandler` holds its table at
`+0x4c`. Its third `SetObjectString` overload takes a pointer to a type mangled `Uw`, for which no
spelling has been found, so that overload is unreconstructed, and the two others stay out because
they share a string with `EnableButton` across it.

`CShellMovieSettings` holds the `game.cfg` movie paths, named by their keys: four types of eleven
entries of three 64-byte strings, a demo entry and a credits entry, starting at `+0x84`. It ends at
`+0x2304`, which fixes the position of `ShellManager`'s following members.

`FEMessageResponse` objects come from a static pool. `FEPoolNode`'s name and arguments are in its
table symbols, and `source/FEngine/ObjectPool.h:87` labels its allocation; the methods and members in
`FEObjectPool.h` are descriptive. `FEMinNode` and `FEMinList` construct inline; `FEngLists.h` and
`FEMessageResponse.h` use `#pragma interface` so that the inline destructors reproduce their callers
without emitting weak copies. Static pool objects (`NodePool`) are declared but not defined yet: the original
static initializers have no destructor arm, and defining the object here gives one.

`FESlotPool.h` holds the block allocator: an `FEMultiPool` is a list of `FESlotPool`s by block size, and
each pool a list of `FESlotNode`s of 32 blocks tracked in a 4-byte mask. `FEScript.h` holds
`FEScript` (pooled like `FEMessageResponse`), `FEKeyTrack` and `FEKeyNode`, whose sizes come from their
pools and allocations. `FEKeyTrack`'s members are inferred from `FEScript`'s track array construction.
`FEEventList` is the symbol-backed class of the count and pointer at `FEScript+0x24`;
`FEEvent.h` models its twelve-byte entries as message, target and time, inferred
from `IssueScriptMessages`. This replaces the earlier descriptive `FEScriptEventList` view.
`FERefList::GetHead` is inline, recursive through a reference, which is why it has an out-of-line copy.

`CPauseMenu` derives from `CUIObject` (tables at `_vt.10CPauseMenu` and `_vt.9CUIObject`); `PauseMenu.h` holds the
members its methods touch. `FEListBox` holds rows and columns of 48-byte cells; the meanings of most cell
members are not known. `FEPackageReader` reads little-endian chunk files; its shared view now includes
object-reading state, list-box indices, type sizes, referenced libraries and message targets.
The package batch records sentinel reads and other format assumptions that remain unchecked
by the original routines.

`FEObject` derives from `FEMinNode`, not `FENode`: its constructor stores the minimal node's table directly,
and its name and hash sit at `+0x14` and `+0x10`, with a GUID from `FEngine::SysGUID` at `+0xc`. Its flag
word at `+0x1c`, resource fields at `+0x22` to `+0x28`, data block at `+0x2c`, response and script lists
at `+0x34` and `+0x44`, and current script at `+0x54` are verified by its methods and `PackageHandler`'s.
`FEMessageNode` and `FEPackageCommand` are the queued messages and package commands; `FEngine` holds
them at `+0x4128` and `+0x4138`. The game interface slots named in `ShellManager.h` follow the calls that
use them; `CanUnloadPackages` is a descriptive name, from its one call. Its data comes from the file-local
pool `ObjDataPool`.

Not yet exact: `Close` (FEObject.cpp, a float range test), `GetStringLength` (register allocation), `FEColor::operator-` (load order),
`ShellManager::RenderObjectList`, `RecallPackageMarker`, `RecordPackageMarker` and the
`PackageHandler` object-list index functions.

### FELocalizer

`FELocalizer` and `FELocalString` hold localized strings as a table of 28-byte records keyed by the upper-case
label hash, each with one `FEWideString` per language. Class and method names are symbols; member names are
inferred from use. Verified: `ReadLocaleData` parses `[HEADER]`, a `Languages` line, `[LABEL]`, `[PLATFORM]`,
`[COMMENT]`, `[FLAGS]` and `[MAX_SIZE]` records; `SetSize` ignores its argument. Inferred: `storeLabels`.

The unit's helpers `FECopyString`, `ItoA` and the rest are private symbols of the original file `FELocalizer.cpp`,
which the symbol table names beside them and which the DWI labels in `SetNumLanguages`, `AllocStrings` and
`SetString` repeat as `source/FEngine/FELocalizer.cpp`. The fragments that call them record that file. The helpers
now have exact or measured partial source, including `ReadLocaleData` and `ReadString`;
see the package/localization and lifecycle notes above for their verification and limits.

### FEPackageReader

`FEPackageReader` builds an `FEPackage` from a package file whose values are little-endian. Verified: the chunk
identifiers tested in `ReadHeaderChunk`, `ReadResourceChunk` and `ReadObjectChunk`, the header version check, the
DWI labels, and the sizes `CreateObject` allocates for each object type. Inferred: reader member names, and the
`FEngElements.h` constructors that take a type, from the store to `FEObject::type` repeated before the reader's own
store. `FEngine::LoadPackage`, `FENode` and the object tag readers use the same classes. `FECodeListBox` derives from `FEObject`, not `FEListBox`: its constructor calls `FEObject`'s directly.

### FECodeListBox and rendering

`FECodeListBox` is a 196-byte `FEObject`, per `CreateObject` and `Clone`; member names come from use and are inferred.
`FEngine::RenderGroup` walks children with a world and combined matrix; `FEObjectSorter<1024>` is a symbol, its second
array is inferred from the size of the space before `messageQueue`.

### Package handlers

Each shell screen is a `PackageHandler` subclass; class names, tables and method names are symbols, and `PackageHandlers.h`
declares only the members their reconstructed methods touch. The base table's slots are the destructor, `Init`, `HandleTick`,
the pure `HandleEvent` and `HandleSoundEvent`, as dumped from `_vt.14PackageHandler`. A handler's `HandleEvent` compares the
event against upper-case hashes of the script's message names. `PackageHandlers.h` uses `#pragma interface` so that the tables
and implicit destructors, which sit elsewhere in the image, stay external to these units.

Verified: the message and object names and the control flow of the reconstructed handlers. Inferred: `buttonHashes`,
`unknown_18c0`, the language table layout
and `quadrantUsed` are named from use, and `quadrantUsed` is only tested for being clear.

`ShellFactory::CreatePackageHandler` compares the package name against one line per screen; the DWI label on each
allocation gives that line's number in `source/UserInterface/shellFactory.cpp`, which is why the macro takes it as an argument.
Allocation sizes there are the evidence for the handler sizes in `PackageHandlers.h`; members with no reader stay `unknown_`.
`VolumeKnobs.h`'s `CSliderVector` is a descriptive name for the twelve-byte vectors the factory's inline constructors zero.
Not reconstructed: handlers whose methods use function-local statics with `.bss` guards, such as `HandleSoundEvent`.

`SMPSetup` in `GameSettingsViews.h` views the multiplayer setup block at `+0x3201c`: screens update its flag word through an
address register pointing at the block, which a direct member access does not produce, so the block is one object in the
original. `SCoreGameSettings` keeps naming its members separately until it is restructured. `SControllerPrefs` is the
twelve bytes `HandlerControllerOptions` copies whole, with fields named for the buttons tied to them; `g_cheats`' words are
`bool`s, as the unnormalized returns and `xori` in `HandlerBonusOptions` show.

Not yet exact: `HandlerPauseBriefingOverlay::HandleEvent` and `HandlerPauseOutro::HandleEvent` (address registers),
`HandlerLoadGame::OnAccept` (register allocation), `HandlerNameEntry::HandleEvent` (two copies) and
`HandlerBonusOptions::HandleEvent`.

The handlers' smaller methods follow the same layout. `HandlerVirtualKeyboard`'s two extra table slots return its string and
its length, which is why `ClearStringField` and `AddLetterToString` call through the table. `HandlerStatsScreen`'s
`specialPearlStats` is named after `CheckToSeeIfSpecialPearlStatsAreNeeded`, the only method that sets it.

The handlers that scan memory cards derive from `CMemoryCard::ScanTask` as well, so `+0x50` holds its table and their members start
at `+0x54`. Each `Execute` copies a file's name and size into a table of `SProfileScanEntry`, whose name is inferred;
`HandlerCoopProfile`'s copy leaves the terminator to the zeroed table. `HandlerProfileSelect` keeps its table in the object, the others
in a static. `keyboardAllowsEmpty` in `ShellManager` is inferred from `HandlerOnlineKeyboard`, which rejects an empty string only while it
is clear.

`HandlerCustomController`'s `controllerFunctionNames`, `controllerKeyNames`, `analogKeyNames`, `analogStickNames` and
`analogEquivalence` are symbols; the members of their entries (`SFunctionNames`, `SKeyName`, `SAnalogEquivalence`) are named from
the lookups. `HandlerMPGameGlobalOptions` derives from `HandlerMPGameOptions`, since its `Init` calls that class's directly and its
own member follows the base's 92 bytes.

`CFengInterface` is the plain FEng game interface: its virtual table in the image names every `FEGameInterface` slot, so the base class
in `ShellManager.h` now carries those names. `SFengRenderContext`, `SFengColor` and their members are inferred from
`GenerateRenderContext` and `GetContextTransform`; the object's color bytes come in blue, green, red, alpha order. `uiObjects` is the
tree its symbols name, wrapped in `CUIObjectSet` because the project has no `<set>`. `FEObjectKind` names the object types by what
`RenderObject` does with each, and its range comes from that switch: the tree omits the test for the top value, which the compiler only
does when an enum's range ends there. `FEObjData::pivot` and `position` were the wrong way round: `FEngine::RenderGroup` rotates about
the first, and `GenerateXformMatrix` translates by the second.

Not yet exact: `CFengInterface::NotificationMessage` and `NotifySoundMessage`, where the iterator's end stays in a register.

`CSaveGamePopup` screens are `CUIObject`s run from inside the save and load flow; the virtual tables name `Update`, `Draw` and the event slots.
`CMemoryCardPopup`'s and the other popups' member names are inferred from their methods. `SoundAliasEvent`'s `SetAlias` helper in these units is
local to them: the alias store goes through a reference, as its inline body shows. `CMemoryCardPopup::OnPush` shares its empty string with the
constructor across `OnUIEvent`, which needs a guarded static, and `Display` and `SetString` take a pointer the source spelling for which
is unknown (`PCUw`), so they stay out.

`ShellManager::handlers` maps packages to their handlers. Only the units that touch it include `ShellHandlerMap.h` first, which gives the member
its real type; the rest see sixteen bytes, so they build without the container headers. `updateOnce` is named from `Process`, which runs one engine
update and returns to the factory and loader while it is set.

The [screen-event and co-op helper follow-up](research/frontend-screens.md)
maintains 24 more functions, including partial screen handlers and three exact
fragments. It documents private cache/guard symbols, complete string-pool
ownership, controller masks, keyboard cursor access and isolated behavior tests.

## Popup, settings and rendering follow-up

The [popup and list evidence](research/frontend-popup-list.md) records 24 further
functions, complete data ownership and isolated original/native tests. Popup
lifecycle, ticking, event forwarding and duplicate-favorite removal are exact;
settings restoration/serialization, render contexts, notifications and menu paths
are registered partials. The popup allocation establishes a CFengInterface extent
of `0x18a0`; its previously unrepresented `0x1c`-byte tail remains unknown.
