// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "AIBehavior.h"

void BIFunc_AIInvalidateCoverPointByID(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int id = args[1];
    if (id)
        CCoverpointBehavior::InvalidateCoverPointByID(id);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
