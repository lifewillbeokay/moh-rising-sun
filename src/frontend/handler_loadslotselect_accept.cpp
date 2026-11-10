// HandlerLoadSlotSelect, the load slot screen: accepting a slot; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
void HandlerLoadSlotSelect::OnAccept() {
    int player;
    if (g_bCoopMultiplayer || g_Shell.inCoop) player = g_Shell.coopPausePlayer;
    else player = 0;
    SSaveData *save = g_GameSettings.GetSaveData(GetSelection(), player);
    int mission = save->mission;
    if (mission == 0) return;
    int level = save->level;
    if (level == 0) return;
    int levelChanged = 0;
    if (g_GameFlow.levelIsShell || mission != g_GameSettings.profile->saveData.mission
        || level != g_GameSettings.profile->saveData.level)
        levelChanged = 1;
    g_GameSettings.RestoreSaveData(GetSelection(), player);
    g_GameSettings.SetNextMissionAndLevel(save->mission, save->level, save->savePointID);
    g_PauseScreen.UnpauseGame();
    if (levelChanged) g_GameFlow.startingMP = 1;
    else g_GameFlow.unknown_10 = 1;
}
