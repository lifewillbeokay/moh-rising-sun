// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "AIBehavior.h"

void BIFunc_AIInvalidateCoverPoint(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CCoverPoint *point = (CCoverPoint *)((BSObject *)args[1])->nativeObject;
    if (point)
        point->Invalidate();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
