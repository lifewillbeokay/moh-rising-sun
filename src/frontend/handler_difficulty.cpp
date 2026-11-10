// HandlerDifficulty, the difficulty menu; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include <stdio.h>
void DebugMsg(const char *, ...);

HandlerDifficulty::HandlerDifficulty() {
    buttonHashes[0] = FEHashUpper("Easy text");
    buttonHashes[1] = FEHashUpper("Normal text");
    buttonHashes[2] = FEHashUpper("Hard text");
}

int HandlerDifficulty::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    if (event == FEHashUpper("Setup Difficulty")) {
        if (g_Shell.inCoop || (g_GameFlow.levelIsShell && !g_GameSettings.WasLastGameSinglePlayer()))
            SetProfileNameOnScreen(g_Shell.coopPausePlayer);
        else
            SetProfileNameOnScreen(0);
        const char *name;
        switch (g_GameSettings.Get_currentDifficulty()) {
        case 1: default: name = "Easy text"; break;
        case 2: name = "Normal text"; break;
        case 3: name = "Hard text"; break;
        }
        FEObject *button = package->FindObjectByName(name);
        if (button) package->SetCurrentButton(engine, button, true);
    } else if (event == FEHashUpper("Set Difficulty Easy")) {
        g_GameSettings.Set_currentDifficulty(1);
    } else if (event == FEHashUpper("Set Difficulty Normal")) {
        g_GameSettings.Set_currentDifficulty(2);
    } else if (event == FEHashUpper("Set Difficulty Hard")) {
        g_GameSettings.Set_currentDifficulty(3);
    } else {
        DebugMsg("Unknown event from difficulty screen\n");
    }
    int selected = -1;
    FEObject *current = package->currentButton;
    if (current) {
        for (int i = 0; i < 3; i++) {
            if (current->hash == buttonHashes[i]) { selected = i; break; }
        }
    }
    switch (selected) {
    case 0: {
        FEWideString *text = g_Shell.localizer->GetString("DIFFICULTY_DESCRIPTION_EASY", 0);
        if (text) SetObjectString(package, "Difficulty text 1", text);
        text = g_Shell.localizer->GetString("DIFFICULTY_DESCRIPTION_EASY2", 0);
        if (text) SetObjectString(package, "Difficulty text 2", text);
        break;
    }
    case 1: {
        FEWideString *text = g_Shell.localizer->GetString("DIFFICULTY_DESCRIPTION_NORMAL", 0);
        if (text) SetObjectString(package, "Difficulty text 1", text);
        text = g_Shell.localizer->GetString("DIFFICULTY_DESCRIPTION_NORMAL2", 0);
        if (text) SetObjectString(package, "Difficulty text 2", text);
        break;
    }
    case 2: {
        FEWideString *text = g_Shell.localizer->GetString("DIFFICULTY_DESCRIPTION_HARD", 0);
        if (text) SetObjectString(package, "Difficulty text 1", text);
        text = g_Shell.localizer->GetString("DIFFICULTY_DESCRIPTION_HARD2", 0);
        if (text) SetObjectString(package, "Difficulty text 2", text);
        break;
    }
    default:
        SetObjectString(package, "Difficulty text 1", "");
        SetObjectString(package, "Difficulty text 2", "");
        break;
    }
    switch (g_GameSettings.Get_currentDifficulty()) {
    case 1:
        SetScript(package, "Easy Mark", "Show");
        SetScript(package, "Normal Mark", "Hide");
        SetScript(package, "Hard Mark", "Hide");
        break;
    case 2:
        SetScript(package, "Easy Mark", "Hide");
        SetScript(package, "Normal Mark", "Show");
        SetScript(package, "Hard Mark", "Hide");
        break;
    case 3:
        SetScript(package, "Easy Mark", "Hide");
        SetScript(package, "Normal Mark", "Hide");
        SetScript(package, "Hard Mark", "Show");
        break;
    default:
        printf("Unknown difficulty value %d!\n", g_GameSettings.Get_currentDifficulty());
        break;
    }
    return 0;
}
