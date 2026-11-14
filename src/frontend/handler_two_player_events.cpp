// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
int Handler_2P_Menu::HandleEvent(unsigned int event, FEObject *object, unsigned long mask) {
    g_Shell.inCoop = 1;
    HandleSaveGameEvent(event, object); HandleFEngQueries(event, object);
    if (event == FEHashUpper("Setup Two Player Menu")) {
        DisableButton(package, "Special Features tab");
        SetScript(package, "MIssions Tab Continue", "Disappear");
        SPlayerProfile *p1 = g_GameSettings.GetProfile(0), *p2 = g_GameSettings.GetProfile(1);
        SetObjectString(package, "Profile1 Name text", p1->name);
        SetObjectString(package, "Profile2 Name text", p2->name);
        if (g_GameSettings.skuType == 1) {
            SetScript(package, "Language Button", "Show"); EnableButton(package, "Language Button");
        } else {
            SetScript(package, "Language Button", "Hide"); DisableButton(package, "Language Button");
        }
        if (!g_GameFlow.levelIsShell || g_GameSettings.WasLastGameSinglePlayer())
            package->controlMask = g_Shell.coopJoinBits;
        else {
            unsigned int mask = g_GameSettings.profile->unknown_321e5 | (g_Shell.mpPlayerCount & ~15);
            g_Shell.mpPlayerCount = mask; package->controlMask = mask;
        }
        package->controlMask = 15;
    } else if (event == FEHashUpper("Load Saved Game")) {
        if (g_Shell.inCoop) { g_Shell.coopPausePlayer = !shell->IsCoopOnTop(mask); package->controlMask = mask; }
    } else if (event == FEHashUpper("Goto Options") && g_Shell.inCoop) {
        g_Shell.coopPausePlayer = !shell->IsCoopOnTop(mask); package->controlMask = mask;
        engine->QueuePackageSwitch("Option_overlay.fng", package);
    }
    return 0;
}
#endif
