// AI-assisted reconstruction from GR8E69; see docs/Script.md.
#include "ScriptBuiltins.h"
#include "MathFun.h"

void BIFunc_GetRandomFloat(int **stack, void *) {
    int count = g_pBuiltInFunctions[g_iCurrentBIFIndex].argumentCount;
    float low = *(float *)(*stack - (count - 1));
    float high = *(float *)(*stack - (count - 2));
    float value;
    if (low == high)
        value = low;
    else
        value = MathFunRandomReal(low, high);
    *stack -= g_pBuiltInFunctions[g_iCurrentBIFIndex].stackAdjustment;
    volatile float result = value;
    **stack = *(int *)&result;
}
