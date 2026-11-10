// HandlerMPJoin, the multiplayer join screen; see docs/Frontend.md.
#include "PackageHandlers.h"
#include "Shell.h"
int HandlerMPJoin::HandleEvent(unsigned int event, FEObject *, unsigned long param) {
    if (event == FEHashUpper("Player Join")) {
        { unsigned char mask = package->controlMask; shell->coopJoinBits |= mask; }
    } else if (event == FEHashUpper("Player Cancel Join")) {
        { unsigned char mask = package->controlMask; shell->coopJoinBits &= ~mask; }
    } else if (event == FEHashUpper("Game Player Joined")) {
        g_Shell.PlayShellSound((EShellSoundType)10);
        if (g_Shell.inCoop) {
            package->controlMask = param;
            unsigned char device = param >> 1;
            if (device == 4) device = 3;
            package->groupId = g_inputMgr.GetPortNum(device);
            { unsigned char mask = package->controlMask; shell->coopJoinBits |= mask; }
            shell->SetCoopJoinInfo(package, package->controlMask);
            FEPackageList *list = (FEPackageList *)&engine->packages;
            if (list) {
                unsigned int hash = FEHashUpper("MPJoin.fng");
                for (FEPackage *p = (FEPackage *)list->head; p; p = (FEPackage *)p->next) {
                    if (p->hash == hash && p != package) p->controlMask = ~param & 0xf;
                }
            }
            engine->QueueMessage(FEHashUpper("Feng Player Joined"), 0, package, (FEObject *)-3, 0);
        } else {
            engine->QueueMessage(FEHashUpper("Feng Player Joined"), 0, package, (FEObject *)-3, 0);
        }
    }
    return 0;
}
