// HandlerMPWaiting, the multiplayer waiting screens; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "Shell.h"
int HandlerMPWaiting::Init(const char *) {
    FEObject *group = package->FindObjectByName("Quadrant Group");
    mbSplitScreenContinueSend = 0;
    if (group) {
        if (g_Shell.inMultiplayer) {
            int device = (unsigned long)package->controlMask >> 1;
            if (device == 4) device = 3;
            switch ((unsigned char)g_inputMgr.GetPortNum(device)) {
            case 0: SetScript(package, "Quadrant Group", "UpperLeft"); break;
            case 1: SetScript(package, "Quadrant Group", "UpperRight"); break;
            case 2: SetScript(package, "Quadrant Group", "LowerLeft"); break;
            case 3: SetScript(package, "Quadrant Group", "LowerRight"); break;
            }
        } else if (shell->unknown_3bc8) {
            if (g_Shell.IsCoopOnTop((unsigned char)package->controlMask)) SetScript(package, "Quadrant Group", "Top");
            else SetScript(package, "Quadrant Group", "Bottom");
        } else {
            int *used = shell->quadrantUsed;
            int taken = used[0];
            if (!taken) {
                SetScript(package, "Quadrant Group", "Top");
                shell->quadrantUsed[taken] = 1;
            } else if (!used[1]) {
                SetScript(package, "Quadrant Group", "Bottom");
                shell->quadrantUsed[1] = 1;
            }
        }
    }
    return 0;
}
int HandlerMPWaiting::HandleEvent(unsigned int event, FEObject *, unsigned long) {
    if (event == FEHashUpper("Player Ready")) {
        unsigned char bits = shell->mpPlayerCount;
        unsigned int mask = package->controlMask;
        if ((bits & mask) == 0) {
            mask |= bits;
            shell->mpPlayerCount = mask;
            if (shell->coopJoinBits == shell->mpPlayerCount && !mbSplitScreenContinueSend) {
                mbSplitScreenContinueSend = 1;
                engine->QueueMessage(FEHashUpper("Split Screen Continue"), 0, 0, (FEObject *)-2, 0);
            }
        }
    } else if (event == FEHashUpper("Player Cancel Ready")) {
        { unsigned char m = package->controlMask; shell->mpPlayerCount &= ~m; }
        { unsigned char m = package->controlMask; shell->unknown_18a2[0] &= ~m; }
        { unsigned char m = package->controlMask; shell->unknown_18a2[1] &= ~m; }
        { unsigned char m = package->controlMask; shell->unknown_18a2[2] &= ~m; }
    } else if (event == FEHashUpper("All Players Joined")) {
        if (g_Shell.inCoop && g_Shell.GetNumCoopPlayers() <= 2 && g_Shell.GetNumCoopPlayers() != 2) return 0;
        g_Shell.PlayShellSound((EShellSoundType)11);
        engine->QueueMessage(FEHashUpper("All Players Joined"), 0, 0, (FEObject *)-2, 0);
    } else if (event == FEHashUpper("Player Join")) {
        if (g_Shell.inMultiplayer) { unsigned char m = package->controlMask; shell->coopJoinBits |= m; }
    } else if (event == FEHashUpper("Create Profile")) {
        unsigned char m = package->controlMask; shell->unknown_18a2[0] |= m;
    } else if (event == FEHashUpper("Edit Profile")) {
        unsigned char m = package->controlMask; shell->unknown_18a2[1] |= m;
    } else if (event == FEHashUpper("Load Profile")) {
        unsigned char m = package->controlMask; shell->unknown_18a2[2] |= m;
    } else if (event == FEHashUpper("Game Player Unjoin")) {
        if (shell->inCoop) shell->DoCoopUnjoin(package, -1);
        engine->QueueMessage(FEHashUpper("Feng Player Unjoin"), 0, package, (FEObject *)-3, 0);
    }
    return 0;
}
