// HandlerQuadrantGroup, a screen split by quadrant; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "Shell.h"
int HandlerQuadrantGroup::Init(const char *name) {
    if (package->FindObjectByName("Quadrant Group")) {
        if (g_Shell.inMultiplayer) {
            int device = (unsigned long)package->controlMask >> 1;
            if (device == 4) device = 3;
            switch ((unsigned char)g_inputMgr.GetPortNum(device)) {
            case 0: SetScript(package, "Quadrant Group", "UpperLeft"); break;
            case 1: SetScript(package, "Quadrant Group", "UpperRight"); break;
            case 2: SetScript(package, "Quadrant Group", "LowerLeft"); break;
            case 3: SetScript(package, "Quadrant Group", "LowerRight"); break;
            }
        } else {
            if (package->controlMask == 175) SetScript(package, "Quadrant Group", "Top");
            else if (package->controlMask == 176) SetScript(package, "Quadrant Group", "Bottom");
            else {
                int *used = shell->quadrantUsed;
                if (!used[0]) SetScript(package, "Quadrant Group", "Top");
                else if (!used[1]) SetScript(package, "Quadrant Group", "Bottom");
            }
        }
        { unsigned char mask = package->controlMask; shell->coopJoinBits &= ~mask; }
    }
    return PackageHandler::Init(name);
}
