// HandlerPausePreferences, the pause menu screens; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "PauseMenu.h"
#include "Objectives.h"
void DebugMsg(const char *, ...);
int HandlerPausePreferences::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    if (event == FEHashUpper("Quit Game")) {
        g_PauseScreen.QuitToShell(false);
    } else if (event == FEHashUpper("Restart Level")) {
        g_PauseScreen.RestartLevel();
    } else if (event == FEHashUpper("Setup Menu")) {
        int noSaves = 1;
        for (unsigned int i = 0; i < 20; i++) {
            int player = 0;
            if (g_bCoopMultiplayer) player = g_Shell.coopPausePlayer;
            SSaveData *save = g_GameSettings.GetSaveData(i, player);
            if (save->mission && save->level) noSaves = 0;
        }
        if (noSaves) {
            SetScript(package, "Save Points button", "Disabled");
            DisableButton(package, "Save Points button");
        } else {
            SetScript(package, "Save Points button", "Show");
            EnableButton(package, "Save Points button");
        }
    } else if (event == FEHashUpper("Update Objectives")) {
        g_Objectives.UpdateUI(package);
    } else if (event == FEHashUpper("Show Coop Options")) {
        if (!g_bCoopMultiplayer) {
            FEObject *button = package->FindObjectByName("Coop Options button");
            if (button) {
                button->flags |= 0x04000000;
                button->SetCurrentScript(button->FindScript(FEHashUpper("Hide")));
                button->flags |= 0x03c00000;
            } else {
                DebugMsg("** Can't find Coop Options button!!\n");
            }
        }
    } else if (event == FEHashUpper("MP Pause Replay")) {
        g_PauseScreen.MPRestartLevel();
    } else if (event == FEHashUpper("MP Pause Exit To MP Setup")) {
        g_PauseScreen.QuitToShell(false);
    } else if (event == FEHashUpper("MP Pause Exit to Main")) {
        g_PauseScreen.QuitToShell(true);
    } else {
        DebugMsg("** Don't know how to handle event %08x from package '%s'", event, package->name);
    }
    return 0;
}
