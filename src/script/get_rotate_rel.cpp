// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"

void BIFunc_GetRotateRel(int **stack, void *) {
    volatile float result = 0.0f;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = *(int *)&result;
}

void BIFunc_GetTranslateRel(int **stack, void *) {
    volatile float result = 0.0f;
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = *(int *)&result;
}
