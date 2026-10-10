#include "ScriptBuiltins.h"
#include "Objectives.h"

void BIFunc_GetObjective(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int id = args[1];
    int result = g_Objectives.GetObjectiveStatus(id);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}

void BIFunc_CheckMainObjectiveStatus(int **stack, void *) {
    int result = g_Objectives.CheckIfAllMainObjectivsAreCompleted();
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = result;
}
