#include "ScriptBuiltins.h"
#include "Objectives.h"

void BIFunc_ShowObjective(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int id = args[1];
    g_Objectives.ShowObjective(id);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
