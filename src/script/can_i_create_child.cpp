// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

extern int g_iGermanCreationQueueHeadIndex;
extern int g_iGermanCreationQueueTailIndex;
extern int g_numActiveSoldiers;
extern int g_maxActiveSoldiers;

void BIFunc_CanICreateChild(int **stack, void *) {
    int can = 0;
    if (g_iGermanCreationQueueHeadIndex == g_iGermanCreationQueueTailIndex)
        can = g_numActiveSoldiers < g_maxActiveSoldiers;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = can;
}
