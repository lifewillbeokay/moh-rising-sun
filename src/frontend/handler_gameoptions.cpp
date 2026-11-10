// HandlerGameOptions, the options overlay; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
int HandlerGameOptions::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    if (event == FEHashUpper("Options Exit")) {
        int crc = g_GameSettings.GetProfilesCRCValue();
        if (g_Shell.savedProfilesCRC != crc) {
            g_Shell.savedProfilesCRC = crc;
            engine->QueuePackageSwitch("SaveConfirmProfile.fng", package);
            return 0;
        }
        if (g_Shell.inCoop) engine->QueueMessage(FEHashUpper("Goto CoopOptions"), 0, package, (FEObject *)-3, 0);
    }
    if (event == FEHashUpper("Setup Menu")) {
        if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer())) {
            SetProfileNameOnScreen(g_Shell.coopPausePlayer);
            SetScript(package, "Bonus text", "Hide");
            DisableButton(package, "Bonus text");
            SetScript(package, "Multiplayer text", "Hide");
            DisableButton(package, "Multiplayer text");
            SetScript(package, "Online Options button", "Hide");
            DisableButton(package, "Online Options button");
            SetScript(package, "Option Choreography", "CoopMode");
        } else {
            SetProfileNameOnScreen(0);
            SetScript(package, "Bonus text", "Unhighlight");
            EnableButton(package, "Bonus text");
            SetScript(package, "Multiplayer text", "Unhighlight");
            EnableButton(package, "Multiplayer text");
            SetScript(package, "Online Options button", "Hide");
            DisableButton(package, "Online Options button");
        }
    } else if (event == FEHashUpper("Split Screen Continue")) {
        if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer()))
            engine->QueueMessage(FEHashUpper("Goto CoopOptions"), 0, package, (FEObject *)-3, 0);
    }
    HandleSaveGameEvent(event, object);
    HandleFEngQueries(event, object);
    return 0;
}
