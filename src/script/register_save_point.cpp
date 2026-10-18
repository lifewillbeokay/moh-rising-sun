// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "GameSettings.h"

void BIFunc_RegisterSavePoint(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    g_GameSettings.RegisterSavePoint(*(*stack - (count - 1)), *(bool *)(*stack - (count - 2)));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}

void BIFunc_PerformSavePointResponse(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    g_GameSettings.TriggerSavePointResponse(*(*stack - (count - 1)), *(bool *)(*stack - (count - 2)));
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
}
