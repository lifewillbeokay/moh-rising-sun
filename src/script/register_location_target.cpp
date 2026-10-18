// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "ScriptRuntime.h"
#include "AITargeting.h"

void BIFunc_RegisterLocationTarget(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    CAITargeting::RegisterLocationTarget(*(*stack - (count - 1)), *(bool *)(*stack - (count - 2)),
                                         (ETeam)*(*stack - (count - 3)), g_pBSObject);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_DeRegisterLocationTarget(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int id = args[1];
    if (id == -1)
        id = (int)g_pBSObject;
    CAITargeting::DeRegisterLocationTarget(id);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
