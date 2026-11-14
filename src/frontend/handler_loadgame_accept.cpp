// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
extern int g_bCoopMultiplayer;
enum GcSwapDiscsMode { GcSwapDiscsMode0 = 0 };
int LevelExistsOnCurrentDisc(int, int);
int SwapDiscs(GcSwapDiscsMode);
void HandlerLoadGame::OnAccept() {
    unsigned int player = g_Shell.coopPausePlayer;
    if (!g_bCoopMultiplayer && !g_Shell.inCoop) player = 0;
    SSaveData *save = g_GameSettings.GetSaveData(GetSelection(), player);
    if (save->mission && save->level) {
        if (!IsValidLoadGameType(save->mission)) {
            engine->QueueMessage(FEHashUpper("Invalid Load Game"), 0, package, (FEObject *)-3, 0);
        } else {
            bool different = g_GameFlow.levelIsShell || save->mission != g_GameSettings.profile->saveData.mission || save->level != g_GameSettings.profile->saveData.level;
            if (LevelExistsOnCurrentDisc(save->mission, save->level) || !SwapDiscs((GcSwapDiscsMode)1)) {
                if (save->mission == 5 && g_GameFlow.levelIsShell) HandlerCoopGameOptions::mSaveSlot = GetSelection();
                else g_GameSettings.RestoreSaveData(GetSelection(), 0);
                g_GameSettings.SetNextMissionAndLevel(save->mission, save->level, save->savePointID);
                g_PauseScreen.UnpauseGame();
                if (save->mission != 5) {
                    if (different) {
                        g_GameFlow.startingMP = 1;
                        if (save->mission == 1) {
                            signed char device = g_Shell.GetIndexPadLastPressedAccept();
                            if (device != -1) g_GameSettings.profile->unknown_321e4 = g_inputMgr.GetPortNum(device);
                        }
                    } else g_GameFlow.unknown_10 = 1;
                    if (save->mission != 5) return;
                }
                if (g_GameFlow.levelIsShell) {
                    engine->RecordPackageMarker("LoadGameOverlay.fng"); engine->QueuePackageSwitch("CoopGameOptions.fng", package);
                }
            }
        }
    }
}
#endif
