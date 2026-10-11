// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "Fog.h"
void BIFunc_GetFogStart(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    float result;
    result = g_fog.start;
    **stack = *reinterpret_cast<int *>(&result);
}

void BIFunc_GetFogEnd(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    float result;
    result = g_fog.end;
    **stack = *reinterpret_cast<int *>(&result);
}
