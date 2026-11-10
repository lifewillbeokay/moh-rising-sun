# AI behaviors

The soldier's high-level behaviors derive from `CBehavior`. Every class name, method
name and table comes from the symbol table; member names are inferred from use. The
functions are fragments of the original `Behavior.cpp`, `SwordBehavior.cpp`,
`PistolBehavior.cpp`, `RifleBehavior.cpp`, `NullWeaponBehavior.cpp` and
`PolemineBehavior.cpp`, compiled with the standard game profile and verified byte for
byte, with their native tables and literal pools. There is no Frontline counterpart.
`BuddyFollowBehavior.cpp` and `ScriptedBehavior.cpp` follow, along with fragments of
`AIThreats.cpp`.

## Verified

- `CBehavior` holds the owning AI object at `+0`, a frame count at `+4` and the enable
  word at `+8`; its table pointer follows at `+0xc`. Its virtual slots are declared in table
  order in `AITargeting.h`. `Update` adds its frame argument and calls
  `UpdateHeuristicScore` when forced or when the count passes
  `GetHeuristicUpdateFrequency`.
- The weapon behaviors add slot 14, `SetWeaponScript`. All five update their heuristic
  every 60 frames and report whether the score changed. `GetType` returns 6 for the
  pistol, 7 for the rifle and the polemine, 8 for the sword and 9 for the null weapon.
- `StartBehavior` triggers event 269 on the weapon script and event 336 on the owner's
  script. `StopBehavior` triggers 271 when its argument is true and 270 otherwise.
- `ChooseSwordOrPistol` returns 1 for the sword when only the sword is usable, 0 when
  only the pistol is, and otherwise 1 when the target is within 10 units (squared
  distance below 100). It tests the embedded targeting module's address for null, as
  the original does.

- `CBuddyFollowBehavior` holds the original static defaults (`m_pBuddyFollowObject` and
  the four `m_default*` distances) and copies them at `Enable` unless `SetDistances` has
  already run. High-level state 4 selects the attack pair of distances. Its score is 90
  when the buddy is farther than its threshold or was bumped, and 0 otherwise.
- `CScriptedBehavior` scores 100 while a script is active and the current state's bit is
  set in the allowed-state mask (`0xffff` until `SetAllowedHighLevelState` first runs). A
  script started while one is running is held and swapped in by `StopBehavior`.
- `CAIThreatEvaluation` keeps fifteen 64-byte `ThreatInfo` slots from `+0x10`, a used
  count at `+0x3d0` and the most important slot at `+0x3d4`. `RankThreats` sorts all
  slots by score, numbers the used ones and records the last as most important. Score
  decays by 0.01 per `Update`; a slot below zero is cleared. Event 456 or 457 (from the
  threat type and state) tells the owner's script about a threat, and 458 that a
  threat's source died.

- `CBotObject` is the multiplayer bot. `AIBot.h` names only the members the landed
  functions touch; offsets are verified by those functions, but the class extent and its
  soldier base are not modeled. `GetClock` reads the clock inside `g_BotMan`.

- `CCoverpointBehavior`: the constructor, `StopBehavior`, `ReleaseResources` and the
  small setters match. It scores 70 while its cover point stays useful. The soldier's
  line-of-sight override, at `+0xdb8` and `+0xdc0` of the soldier AI object, follows the
  embedded coverpoint behavior and ends the 3536-byte allocation.

- `CMountedMachineGunBehavior`: the constructor, destructor (which withdraws any bid on
  its gun), `StopBehavior`, `ReleaseResources`, capabilities and the setters match.
- `AI::Initialize`, `Restart` and `Shutdown`, `CAIActivityZone`'s point test and setters,
  and `CAILaggedVector::Init` and `Reset` match.

- `CBuddyLeadBehavior`: the constructor, heuristic, start and stop match; it scores 100
  while an unfinished checkpoint remains. `Checkpoint_t` is the original tag; only its
  prefix is observed.
- `CGrenadeResponseBehavior`: only `StopBehavior`, `ReleaseResources`, the capability test
  and accessors so far. Its constructor and queries use a fixed-pool list that is not
  yet modeled.
- `AreaPathNode.cpp`: the node array (`g_pPathFindingAreas`, sixteen bytes per node), the
  round-robin out-of-area update (fifty nodes per call) and the in-area byte at `+6`.
  `ImportAreaPathNodeNetwork` fixes up the BPD's `areas`, `points` and `bsp` arrays; its template
  instances name the record types `BPDPathFindingArea`, `BPDLinkEdge`, `BPDPathFindingBSP`
  and `BPDPathFindingBSPBranch`, and the vertices are `PropVec3`. A node lists
  `borderVertexCount + linkCount` vertex indices and `linkCount` four-byte link edges. A link's low
  14 bits are the neighbor's node index, and its two bytes index the owner's vertex list.
  A BSP walk ends on a non-positive branch value, whose complement is the node index. The
  geometry queries (containment, closest point, border and link edge intersection, centroid
  bias and line clipping) are matched. `CAreaPathNode`'s member names are inferred. The
  literal pool holds an unreferenced 0 and 1 after `ContainsPoint`'s (at `0x802a4194`), so
  the units around it leave those two words as original context.
- Bot state code: `SetCampingLookat`, `UpdateLookDir` and `WantToStrafe` and the bot's state handlers (`BS_Enter_Dead`, `BS_Dead`, `BS_Enter_Goto`,
  `BS_Enter_Combat`, `BS_Chase`, `BS_Enter_Camp`, `BS_Exit_Camp`), the move state, the threat
  and boredom queries, and a few team and weapon queries. Member names in `CBotObject` are
  inferred from these uses, and `CBotWeapon`'s table starts at `+0x1a20`. `eBotState` and
  `eBotMoveState` are original enum tags; their values are plain numbers here because their
  names are not known. `OUT_OF_SIGHT_DELAY` is a function-local static as its suffixed symbol
  shows.
- `BotMan.cpp`: bot memory (`GetBotMem` hands out 11,568-byte slots), setup and cleanup. `MMG_Info`
  and `CampingPoint` strides come from `BS_Enter_UseMMG` and the camping point table.
- `BotArea.cpp`: the area and edge arrays and their accessors. `CBotArea` (sixteen bytes) and
  `CAreaEdge` (64 bytes) come from the symbol table with sizes from array strides; `jumpIndex`, `coverPoints`,
  `coverData`, `vertex0` and `vertex1` are inferred. `DwiAllocIdScope` is an inferred name for
  the save-and-restore of the allocator tag, reproducing the saved stack slot.
  `CAIAreaPathFinding`'s volume counters, finish flag and walk-type query, the path walk
  stop, trim and step functions, and the on-network test in `Update`. Its member names are
  inferred from these uses; `CAIPhysics` is at `+0x1c` and the solution list at `+0x88`.

## Not reconstructed yet

- `CAreaPathNode::LineEndsAtLinkEdge` and `GetNextNode`, `CAreaEdge::GetEdgeVector` and
  `GetMidPoint`: near misses (register allocation; the last two inline `GetEdgePoints`).
- `BS_Enter_Retreat`, `BS_Enter_Chase` and `BS_Camp`: their strings are shared with neighbors in
  the same pool that are not reconstructed yet.
- `CBotObject::DirectChase`: drafted, but its literal pool has a double at 4 mod 8 and needs
  `ChaseDecision` and `CoverPointChase` in the same unit.
- `CAIAreaPathFinding::PathDistance`: a near miss (a register for the first node).
- `GetBestPath`: a private function called only by `ComputeAStarPath`, so it waits for that unit.
- `ShrinkAreaMesh`: an empty body with a 224-byte frame; the locals behind that frame are unknown.
- `CCoverpointBehavior::UpdateHeuristicScore`, `StartBehavior`, `GetCapabilities` and
  `ChooseCoverpoint`: near misses left out rather than steered.
- `CAIThreatEvaluation`'s constructor and the module destructors: their tables and
  destructors are weak in the original, and no current unit form owns those.
- `ScoreInitialThreat`: one address computation (the position argument) still differs.
- `PropagateSoundToMe`: a function-local static with a guard.

## Inferred

- The names `aiObject`, `framesSinceUpdate`, `heuristicScore`, `weaponScript` and
  `swordDrawn`, and the description of the event arguments.
- `highLevelState` at `+0x10` of the pistol, rifle and null weapon: the constructor
  sets it to 4 and it is passed where the sword passes the AI object's high-level state.
- The `ECapability` enumerators are numbered only; what each capability means is not
  known. The enum's symbol-table tag is original.
- `CAIThreat`'s 16-byte alignment, from the gaps around it in `ThreatInfo` and from the
  slot address forms in six functions. The threat type values have no recovered names.
- The inline-virtual methods that the original emits at the end of each file (`GetType`,
  `GetScriptObject`) are written as ordinary out-of-line definitions in that order.
