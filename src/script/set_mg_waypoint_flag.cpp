// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"

void BIFunc_SetMGWaypointFlag(int **stack, void *) {
    unsigned int *flags = (unsigned int *)&g_pBSObject->nativeObject->flags;
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    unsigned int mask = *(*stack - (count - 1));
    int set = *(*stack - (count - 2));
    if (set)
        *flags |= mask;
    else
        *flags &= ~mask;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
