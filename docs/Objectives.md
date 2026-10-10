# Player objectives

`src/objectives/` reconstructs nine `CPlayerObjectives` functions (396 executable
bytes) from the pinned GR8E69 executable. Claude Code assisted the analysis and
verification.

## Scoped view

`include/game/Objectives.h` declares ten 20-byte objective records (prompt text
pointer, prompt length byte, completed, bonus and shown words), the main and bonus
counts at `+200`/`+204`, and a word at `+208` that `Reset` sets to 1. Objective IDs
are 1-10; every accessor ignores IDs outside that range. `AddObjective` and
`ChangeObjectivePrompt` look prompts up in the `CUserMessageTable` at `+1632` of the
global `g_pPopUpMessageHandler`, declared as method-only and scoped views. Names are
descriptive.

| Manifest | Functions | Code bytes |
| --- | --- | ---: |
| `objectives_init` | constructor (calls `Reset`), `Reset` | 112 |
| `objectives_show` | `ShowObjective` | 32 |
| `objectives_status` | `SetObjectiveStatus`, `GetObjectiveStatus`, `GetNumCompletedObjectives`, `CheckIfAllMainObjectivsAreCompleted`, `GetNumBonusObjectives`, `GetNumCompletedBonusObjectives` | 252 |

- `GetNumCompletedObjectives` counts completed records that are not bonus
  objectives; `GetNumCompletedBonusObjectives` counts completed bonus records.
- `CheckIfAllMainObjectivsAreCompleted` (original spelling) compares the first count
  with the main-objective count kept by `AddObjective`.
- `AddObjective(id, message, bonus, shown, completed)` stores the prompt and its
  length from the message table and increments the bonus or main count. It and
  `ChangeObjectivePrompt` match except for the order in which the parameters are
  copied and the index is computed; they are not accepted.

Three [script built-ins](Script.md#objective-ambient-sound-and-value-built-ins)
now call the accepted visibility and status methods through the original shared
`g_Objectives`: `BIFunc_ShowObjective`, `BIFunc_GetObjective` and
`BIFunc_CheckMainObjectiveStatus`. Their 368 bytes are counted in `src/script/`,
separately from the nine methods above. The shared object's symbol has a 212-byte
extent at `0x803e81a4`; it remains external original storage.

## Verification

ProDG 3.8.1 with `-O2 -G0 -fno-exceptions -fno-implicit-templates` reproduces every
accepted fragment. The complete rebuilt analysis image is identical. Runtime
behavior has not been tested.
