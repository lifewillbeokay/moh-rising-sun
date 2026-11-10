// HandlerMainMenu, the main menu; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "GameSettingsViews.h"
#include "Shell.h"
int HandlerMainMenu::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    HandleSaveGameEvent(event, object);
    HandleFEngQueries(event, object);
    if (event == FEHashUpper("Setup Main Menu")) {
        SetProfileNameOnScreen(0);
        DisableButton(package, "Special Features tab");
        SetScript(package, "Special Features tab", "Hide");
        SetScript(package, "Credits button", "Show");
        EnableButton(package, "Credits button");
        if (g_GameSettings.skuType == 1) {
            SetScript(package, "Language Button", "Show");
            EnableButton(package, "Language Button");
        } else {
            SetScript(package, "Language Button", "Hide");
            DisableButton(package, "Language Button");
        }
        g_bInMultiplayerMode = 0;
        g_bCoopMultiplayer = 0;
        g_bTeamDM = 0;
        g_NumPlayers = 1;
        MPSetup()->flags &= ~0x40000000;
        MPSetup()->flags &= ~0x08000000;
        MPSetup()->flags &= ~0x01000000;
        g_GameSettings.profile->unknown_3201c[0] = 1;
        g_inputMgr.CheckControllers(false);
    }
    HandleMovieEvent(event, object);
    return 0;
}
