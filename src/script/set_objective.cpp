// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Objectives.h"

void BIFunc_SetObjective(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    int objective = *(*stack - (count - 1));
    bool complete = *(*stack - (count - 2)) != 0;
    g_Objectives.SetObjectiveStatus(objective, complete);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
