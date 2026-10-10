#include "ScriptBuiltins.h"

void BIFunc_FloatToInt(int **stack, void *) {
    int *args = *stack - g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float value = reinterpret_cast<float *>(args)[1];
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = static_cast<int>(value);
}

void BIFunc_InitToInvalid(int **stack, void *) {
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    **stack = 0;
}
