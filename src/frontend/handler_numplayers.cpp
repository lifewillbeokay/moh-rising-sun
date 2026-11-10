// HandlerNumPlayers, the player-count menu; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "GameSettings.h"
#include "Shell.h"
// The low four bits hold one bit per player; the upper bits of these bytes are kept.
static inline void ClearPlayerBits(unsigned char &bits) {
    int value = bits;
    value &= ~0xf;
    bits = value;
}
int HandlerNumPlayers::Init(const char *name) {
    ClearPlayerBits(shell->unknown_18a2[0]);
    ClearPlayerBits(shell->unknown_18a2[1]);
    ClearPlayerBits(shell->coopJoinBits);
    ClearPlayerBits(shell->mpPlayerCount);
    g_Shell.unknown_3bc8 = 0;
    g_inputMgr.controllerChecksEnabled = 1;
    HandleEvent(FEHashUpper("Controllers Change"), 0, 0);
    return PackageHandler::Init(name);
}
int HandlerNumPlayers::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    if (event == FEHashUpper("NumPlayers 1")) {
        g_GameSettings.profile->unknown_3201c[0] = 1;
        shell->coopJoinBits |= 1;
        g_inputMgr.controllerChecksEnabled = 0;
        g_Shell.unknown_3bc8 = 1;
    } else if (event == FEHashUpper("NumPlayers 2")) {
        g_GameSettings.profile->unknown_3201c[0] = 2;
        shell->coopJoinBits |= 3;
        g_inputMgr.controllerChecksEnabled = 0;
        g_Shell.unknown_3bc8 = 1;
    } else if (event == FEHashUpper("NumPlayers 3")) {
        g_GameSettings.profile->unknown_3201c[0] = 3;
        shell->coopJoinBits |= 7;
        g_inputMgr.controllerChecksEnabled = 0;
        g_Shell.unknown_3bc8 = 1;
    } else if (event == FEHashUpper("NumPlayers 4")) {
        g_GameSettings.profile->unknown_3201c[0] = 4;
        shell->coopJoinBits |= 15;
        g_inputMgr.controllerChecksEnabled = 0;
        g_Shell.unknown_3bc8 = 1;
    } else if (event == FEHashUpper("Controllers Change")) {
        SetScript(package, "box 1", "Hide");
        DisableButton(package, "box 1");
        SetScript(package, "box 2", "Hide");
        DisableButton(package, "box 2");
        SetScript(package, "box 3", "Hide");
        DisableButton(package, "box 3");
        SetScript(package, "box 4", "Hide");
        DisableButton(package, "box 4");
        FEObject *button;
        switch ((unsigned char)g_inputMgr.GetNumDevices()) {
        case 0:
            package->SetCurrentButton(engine, 0, true);
            break;
        case 4:
            SetScript(package, "box 4", "Show");
            EnableButton(package, "box 4");
            SetScript(package, "box 3", "Show");
            EnableButton(package, "box 3");
            SetScript(package, "box 2", "Show");
            EnableButton(package, "box 2");
            SetScript(package, "box 1", "Show");
            EnableButton(package, "box 1");
            button = package->FindObjectByName("box 4");
            if (button) package->SetCurrentButton(engine, button, true);
            break;
        case 3:
            SetScript(package, "box 3", "Show");
            EnableButton(package, "box 3");
            SetScript(package, "box 2", "Show");
            EnableButton(package, "box 2");
            SetScript(package, "box 1", "Show");
            EnableButton(package, "box 1");
            button = package->FindObjectByName("box 3");
            if (button) package->SetCurrentButton(engine, button, true);
            break;
        case 2:
            SetScript(package, "box 2", "Show");
            EnableButton(package, "box 2");
            SetScript(package, "box 1", "Show");
            EnableButton(package, "box 1");
            button = package->FindObjectByName("box 2");
            if (button) package->SetCurrentButton(engine, button, true);
            break;
        case 1:
            SetScript(package, "box 1", "Show");
            EnableButton(package, "box 1");
            button = package->FindObjectByName("box 1");
            if (button) package->SetCurrentButton(engine, button, true);
            break;
        }
    }
    return 0;
}
