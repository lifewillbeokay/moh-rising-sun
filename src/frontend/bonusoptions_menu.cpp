// AI-assisted reconstruction from GR8E69; see docs/research/frontend-popup-list.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include <stdio.h>
#ifdef DECOMP_COMPARE
HandlerBonusOptions::HandlerBonusOptions() {
    char name[64];
    for (int i = 0; i < 12; i++) {
        sprintf(name, "bonus%.2d text", i + 1);
        textHashes[i] = FEHashUpper(name);
    }
}

int HandlerBonusOptions::HandleEvent(unsigned int event, FEObject *object, unsigned long) {
    char name[68];
    if (event == FEHashUpper("Setup Menu")) {
        printf("setting up menu\n");
        if (!g_Shell.inCoop && (!g_GameFlow.levelIsShell || g_GameSettings.WasLastGameSinglePlayer())) SetProfileNameOnScreen(0);
        else SetObjectString(package, "Profile Name text", g_GameSettings.GetNameFromSettings(g_Shell.coopPausePlayer));
        bool any = false; int first = 13;
        for (int i = 12; i > 0; --i) {
            sprintf(name, "bonus%.2d group", i);
            if (g_GameSettings.HasCheat(i - 1)) {
                any = true; SetScript(package, name, "Appear");
                if (package->FindObjectByName(name)) {
                    sprintf(name, "bonus%.2d mark", i);
                    SetScript(package, name, GetCheatByIndex(i - 1) ? "Show" : "Hide"); first = i;
                }
                sprintf(name, "bonus%.2d text", i); EnableButton(package, name);
            } else {
                SetScript(package, name, "Hide");
                sprintf(name, "bonus%.2d mark", i); SetScript(package, name, "Hide");
                sprintf(name, "bonus%.2d text", i); DisableButton(package, name);
            }
        }
        if (first < 13) {
            sprintf(name, "bonus%.2d text", first); FEObject *button = package->FindObjectByName(name);
            if (button) package->SetCurrentButton(engine, button, true);
        }
        if (any) { SetScript(package, "No Bonuses text", "Hide"); DisableButton(package, "No Bonuses text"); }
        else {
            SetScript(package, "No Bonuses text", "Show"); FEObject *button = package->FindObjectByName("No Bonuses text");
            if (button) package->SetCurrentButton(engine, button, true);
            EnableButton(package, "No Bonuses text");
        }
    } else if (event == 0xc407210) {
        for (int i = 0; i < 12; ++i) if (textHashes[i] == object->hash) {
            sprintf(name, "bonus%.2d mark", i + 1);
            SetScript(package, name, ToggleCheatByIndex(i) ? "Show" : "Hide"); break;
        }
    }
    return 0;
}

#endif
