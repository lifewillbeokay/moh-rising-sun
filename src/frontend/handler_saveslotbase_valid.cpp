// HandlerSaveSlotBase, the save slot screens: which saves load here; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include <string.h>
#include <stdio.h>
int HandlerSaveSlotBase::IsValidLoadGameType(int mission) {
    int isShellLevel = 0;
    if (g_GameSettings.profile->saveData.mission == 1) isShellLevel = g_GameSettings.profile->saveData.level == 16;
    int coop;
    if (isShellLevel) coop = g_Shell.inCoop;
    else coop = g_bCoopMultiplayer;
    if (mission == 1 && !coop) return 1;
    if (mission == 5 && coop) return 1;
    return 0;
}
