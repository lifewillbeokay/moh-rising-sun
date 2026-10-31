// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptRuntime.h"

extern unsigned char *g_pBSMessageMemory;
extern int g_pBSMessageMemoryOffset;

int BSInitMessageRegistrationArray() {
    g_MessageRegistrationArray = reinterpret_cast<BSMessageRegistration_struct **>(
        g_pBSMessageMemory + g_pBSMessageMemoryOffset);
    g_pBSMessageMemoryOffset += 444 * sizeof(BSMessageRegistration_struct *);
    for (int i = 443; i >= 0; --i) g_MessageRegistrationArray[i] = 0;
    return 0;
}

int BSInitMessageRegistrationFreeList() {
    g_MessageRegistrationFreeList = reinterpret_cast<BSMessageRegistration_struct *>(
        g_pBSMessageMemory + g_pBSMessageMemoryOffset);
    g_pBSMessageMemoryOffset += g_iBSMessageRegistrationListSize * sizeof(BSMessageRegistration_struct);
    for (int i = 0; i < g_iBSMessageRegistrationListSize - 1; ++i)
        g_MessageRegistrationFreeList[i].nextIndex = i + 1;
    g_MessageRegistrationFreeList[g_iBSMessageRegistrationListSize - 1].nextIndex = -1;
    g_MessageRegistrationFreeListHead = g_MessageRegistrationFreeList;
    return 0;
}
