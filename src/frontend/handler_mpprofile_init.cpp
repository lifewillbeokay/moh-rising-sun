// HandlerMPProfile's constructor and setup; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "Shell.h"
#include <stdio.h>
#include <string.h>
HandlerMPProfile::HandlerMPProfile() {}
static inline void ClearBits(unsigned char &bits, unsigned char mask) { bits &= ~mask; }
int HandlerMPProfile::Init(const char *name) {
    unknown_54 = 0;
    g_Shell.RemoveMPViewPorts();
    ClearBits(shell->mpPlayerCount, package->controlMask);
    ClearBits(shell->unknown_18a2[0], package->controlMask);
    ClearBits(shell->unknown_18a2[1], package->controlMask);
    ClearBits(shell->unknown_18a2[2], package->controlMask);
    if (package->FindObjectByName("Quadrant Group")) {
        if (g_Shell.inMultiplayer) {
            unsigned long player = (unsigned long)package->controlMask >> 1;
            if (player == 4) {
                player = 3;
            }
            switch ((unsigned char)g_inputMgr.GetPortNum(player)) {
            case 0:
                if (g_Shell.inMultiplayer) {
                    SetScript(package, "Quadrant Group", "UpperLeft");
                } else {
                    SetScript(package, "Quadrant Group", "Top");
                }
                break;
            case 1:
                if (g_Shell.inMultiplayer) {
                    SetScript(package, "Quadrant Group", "UpperRight");
                } else {
                    SetScript(package, "Quadrant Group", "Bottom");
                }
                break;
            case 2:
                SetScript(package, "Quadrant Group", "LowerLeft");
                break;
            case 3:
                SetScript(package, "Quadrant Group", "LowerRight");
                break;
            }
        } else if (g_Shell.IsCoopOnTop(package->controlMask)) {
            SetScript(package, "Quadrant Group", "Top");
        } else {
            SetScript(package, "Quadrant Group", "Bottom");
        }
    }
    unknown_58 = 0;
    for (int i = 0; i < 2; i++) {
        char groupName[40];
        sprintf(groupName, "Profile Group %d", i);
        groupHashes[i] = FEHashUpper(groupName);
    }
    return PackageHandler::Init(name);
}
