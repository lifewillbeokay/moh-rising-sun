// AI-assisted GR8E69 reconstruction; see docs/research/frontend-screens.md.
#ifdef DECOMP_COMPARE
#include "PackageHandlers.h"
#include "Shell.h"
#include "GameSettings.h"
// Original file-scoped cache and guard symbols; these remain externally owned.
extern unsigned long joinHash asm("szMPJoinNameHash.8829");
extern int joinGuard asm("_.tmp_0.8830");
extern unsigned long waitingHash asm("szMPWaitingNameHash.8831");
extern int waitingGuard asm("_.tmp_1.8832");
extern unsigned long splitHash asm("szMPFourWaySplitNameHash.8833");
extern int splitGuard asm("_.tmp_2.8834");
extern unsigned long coopHash asm("szMPCoopFourWaySplitNameHash.8835");
extern int coopGuard asm("_.tmp_3.8836");
extern unsigned long coopJoinHash asm("szMPJoinNameHash.8840");
extern int coopJoinGuard asm("_.tmp_4.8841");
extern unsigned long coopWaitingHash asm("szMPWaitingJoinNameHash.8842");
extern int coopWaitingGuard asm("_.tmp_5.8843");
void ShellManager::DoPackageControlMaskUpdating() {
    FEList *list = &engine->packages;
    if (list && (list->FindNode("4WaySplit.fng") || list->FindNode("Coop4WaySplit.fng"))) {
        if (!joinGuard) { joinHash = FEHashUpper("MPJoin.fng"); joinGuard = 1; }
        if (!waitingGuard) { waitingHash = FEHashUpper("MPWaitingJoin.fng"); waitingGuard = 1; }
        if (!splitGuard) { splitHash = FEHashUpper("4WaySplit.fng"); splitGuard = 1; }
        if (!coopGuard) { coopHash = FEHashUpper("Coop4WaySplit.fng"); coopGuard = 1; }
        coopJoinBits &= 0xf0;
        for (FEPackage *p = (FEPackage *)list->head; p; p = (FEPackage *)p->next) {
            if (p->hash == joinHash || p->hash == waitingHash) {
                int device = g_inputMgr.FindDeviceIDFromPortNum(p->groupId);
                if (device != -1) { p->controlMask = 1 << device; if (p->hash == waitingHash) coopJoinBits |= p->controlMask; }
            }
            if (p->hash == splitHash || p->hash == coopHash) { p->controlMask = 255; p->groupId = -1; }
        }
    }
}
void ShellManager::DoCoopPackageControlMaskUpdating() {
    if (GetNumCoopPlayers() == 1) {
        FEList *list = &engine->packages;
        if (list) {
            if (!coopJoinGuard) { coopJoinHash = FEHashUpper("MPJoin.fng"); coopJoinGuard = 1; }
            if (!coopWaitingGuard) { coopWaitingHash = FEHashUpper("MPWaitingJoin.fng"); coopWaitingGuard = 1; }
            FEPackage *waiting = (FEPackage *)list->head;
            for (; waiting && waiting->hash != coopWaitingHash; waiting = (FEPackage *)waiting->next) {}
            if (waiting) for (FEPackage *p = (FEPackage *)list->head; p; p = (FEPackage *)p->next) {
                if (p->hash == coopJoinHash) { p->controlMask = ~(unsigned char)waiting->controlMask & 15; return; }
            }
        }
    }
}
#endif
