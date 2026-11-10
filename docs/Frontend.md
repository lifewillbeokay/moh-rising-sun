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
`FEScriptEventList` is a descriptive name for the two words at `FEScript+0x24` that its destructor releases.
`FERefList::GetHead` is inline, recursive through a reference, which is why it has an out-of-line copy.

`CPauseMenu` derives from `CUIObject` (tables at `_vt.10CPauseMenu` and `_vt.9CUIObject`); `PauseMenu.h` holds the
members its methods touch. `FEListBox` holds rows and columns of 48-byte cells; the meanings of most cell
members are not known. `FEPackageReader` reads little-endian chunk files; only its reset fields and type-size
table are modeled so far.

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
